
using namespace std;

//==============================================================================//
/// Outline of InteractionForcesDEM from JSphCpu
//==============================================================================//

// Notes on acronyms
// DEMDT: DEM time step 
// OpenMP for parallel processing
// 

// First, initializes demdtth, array of maximum demdt from each parallel loop
// Starts loops over all particles, search for neighbors in adjacent cells
  // Determines interactions between particles. floating (regular) 
  // Creates referene distances between neighbors
  // Gets material values from DEMdata for neighbor particle,
  // Caclulate new mass of pair. Calculate relative velocity
  // Calculate viscous damping coefficent
  // Calculates DEM force from over_lap (Soft DEM)
    // Contact and tangential
// Force stops when overlap is less than or equal to 0

//==============================================================================//
/// Draft of our JSph Struct
//==============================================================================//

// For our version, we want forces to always exist
// Damping force, which depends on velocity (justification?)
// Contact force, spring force that keeps bringing neighboring pairs back to specific length
// Not parallelized
// Need spring constant and damping coefficents ??? 
// Use anything from DEM DATA

// Rough outline of our code
  // Set spring constant and rest length range
  // Loop over neighboring particles, compute if neighbor is not fluid 
    // Need to change - want to always compute, find nearest particle? search in all directions?
    // Exclude water 
  // Compute forces
  // Apply force

void CalculateForcesWithSpringsAndDamping(unsigned nfloat, const StDivDataCpu& divdata,
                                          const unsigned* dcell, const unsigned* ftridp,
                                          const StDemData* demdata, const tdouble3* pos,
                                          const tfloat4* velrhop, const typecode* code,
                                          const unsigned* idp, float& maxDampingDt, tfloat3* ace) {
    // Initialize the maximum damping time step for each thread
    float dampingDtThread[OMP_MAXTHREADS * OMP_STRIDE];
    for (int th = 0; th < OmpThreads; th++) dampingDtThread[th * OMP_STRIDE] = -FLT_MAX;
    
    const int nft = int(nfloat);

    #ifdef OMP_USE
    #pragma omp parallel for schedule(guided)
    #endif
    for (int cf = 0; cf < nft; cf++) {
        const unsigned p1 = ftridp[cf];
        if (p1 != UINT_MAX) {
            tfloat3 forceAccum = TFloat3(0);
            const tdouble3 posp1 = pos[p1];
            const typecode tavp1 = CODE_GetTypeAndValue(code[p1]);
            const float massp1 = demdata[tavp1].mass;
            const float dampingCoeffP1 = demdata[tavp1].kfric;
            const float restLengthRange = 1.0f;  // Define a range for rest length adjustments
            const float springConstant = 1.0f;   // Define spring constant
            const float dampingCoeff = dampingCoeffP1;

            // Loop over neighbors to compute forces
            for (byte tpfluid = 0; tpfluid <= 1; tpfluid++) {
                const StNgSearch ngs = nsearch::Init(dcell[p1], !tpfluid, divdata);
                for (int z = ngs.zini; z < ngs.zfin; z++) {
                    for (int y = ngs.yini; y < ngs.yfin; y++) {
                        const tuint2 pif = nsearch::ParticleRange(y, z, ngs, divdata);
                        for (unsigned p2 = pif.x; p2 < pif.y; p2++) {
                            if (CODE_IsNotFluid(code[p2]) && tavp1 != CODE_GetTypeAndValue(code[p2])) {
                                const float drx = float(posp1.x - pos[p2].x);
                                const float dry = float(posp1.y - pos[p2].y);
                                const float drz = float(posp1.z - pos[p2].z);
                                const float rr2 = drx * drx + dry * dry + drz * drz;
                                const float rad = sqrt(rr2);

                                // Compute relative velocities
                                const float dvx = velrhop[p1].x - velrhop[p2].x;
                                const float dvy = velrhop[p1].y - velrhop[p2].y;
                                const float dvz = velrhop[p1].z - velrhop[p2].z;
                                const float vDot = dvx * drx + dvy * dry + dvz * drz;
                                const float normalLength = sqrt(rr2);
                                const float nx = drx / normalLength;
                                const float ny = dry / normalLength;
                                const float nz = drz / normalLength;

                                // Compute spring force
                                const float restLength = restLengthRange;  // Randomly set this if needed
                                const float springForceMagnitude = springConstant * (restLength - rad);
                                const float fn = springForceMagnitude;

                                // Compute damping force
                                const float dampingForce = dampingCoeff * vDot;

                                // Apply forces
                                float forceNormal = fn - dampingForce;
                                float accel = forceNormal / massp1;

                                forceAccum.x += accel * nx;
                                forceAccum.y += accel * ny;
                                forceAccum.z += accel * nz;
                                
                                // Apply tangential force
                                const float dvxt = dvx - vDot * nx;
                                const float dvyt = dvy - vDot * ny;
                                const float dvzt = dvz - vDot * nz;
                                const float vt = sqrt(dvxt * dvxt + dvyt * dvyt + dvzt * dvzt);
                                float tx = 0, ty = 0, tz = 0;
                                if (vt != 0) {
                                    tx = dvxt / vt;
                                    ty = dvyt / vt;
                                    tz = dvzt / vt;
                                }
                                const float tangentialDamping = dampingCoeff * vt;
                                float forceTangential = tangentialDamping;

                                // Final tangential force application
                                accel = forceTangential / massp1;
                                forceAccum.x += accel * tx;
                                forceAccum.y += accel * ty;
                                forceAccum.z += accel * tz;
                            }
                        }
                    }
                }
            }
            if (forceAccum.x || forceAccum.y || forceAccum.z) {
                ace[p1] = ace[p1] + forceAccum;
                const int th = omp_get_thread_num();
                if (dampingDtThread[th * OMP_STRIDE] < dampingCoeff) dampingDtThread[th * OMP_STRIDE] = dampingCoeff;
            }
        }
    }
    
    // Update the maximum damping coefficient
    float dampingDt = dampingDtThread[0];
    for (int th = 1; th < OmpThreads; th++) if (dampingDt < dampingDtThread[th * OMP_STRIDE]) dampingDt = dampingDtThread[th * OMP_STRIDE];
    if (maxDampingDt < dampingDt) maxDampingDt = dampingDt;
}

void JSphCpu::InteractionForcesSpringDamping(
    unsigned nfloat,
    StDivDataCpu divdata,
    const unsigned *dcell,
    const unsigned *ftridp,
    const StDemData* demdata,
    const tdouble3 *pos,
    const tfloat4 *velrhop,
    const typecode *code,
    const unsigned *idp,
    float &viscdt,
    tfloat3 *ace
) const
{
    //- Initialize demdtth to calculate max demdt with OpenMP.
    float demdtth[OMP_MAXTHREADS*OMP_STRIDE];
    for(int th=0; th<OmpThreads; th++) demdtth[th*OMP_STRIDE] = -FLT_MAX;
    
    //- Initialize execution with OpenMP.
    const int nft = int(nfloat);
    #ifdef OMP_USE
    #pragma omp parallel for schedule (guided)
    #endif
    for(int cf = 0; cf < nft; cf++) {
        const unsigned p1 = ftridp[cf];
        if (p1 != UINT_MAX) {
            float demdtp1 = 0;
            tfloat3 acep1 = TFloat3(0);

            //- Get data of particle p1.
            const tdouble3 posp1 = pos[p1];
            const typecode tavp1 = CODE_GetTypeAndValue(code[p1]);
            const float masstotp1 = demdata[tavp1].mass;
            const float taup1 = demdata[tavp1].tau;
            const float kfricp1 = demdata[tavp1].kfric;
            const float restitup1 = demdata[tavp1].restitu;
            const float ftmassp1 = demdata[tavp1].massp;

            //- Search for neighbors in adjacent cells (excluding water).
            for(byte tpfluid = 0; tpfluid <= 1; tpfluid++) {
                const StNgSearch ngs = nsearch::Init(dcell[p1], !tpfluid, divdata);
                for(int z = ngs.zini; z < ngs.zfin; z++) {
                    for(int y = ngs.yini; y < ngs.yfin; y++) {
                        const tuint2 pif = nsearch::ParticleRange(y, z, ngs, divdata);

                        //- Interaction of particles with type Fluid or Bound.
                        for(unsigned p2 = pif.x; p2 < pif.y; p2++) {
                            if (p2 != p1 && CODE_IsNotFluid(code[p2])) {
                                const float drx = float(posp1.x - pos[p2].x);
                                const float dry = float(posp1.y - pos[p2].y);
                                const float drz = float(posp1.z - pos[p2].z);
                                const float rr2 = drx*drx + dry*dry + drz*drz;
                                const float rad = sqrt(rr2);

                                const float restLength = 1.0f; // Set to a suitable rest length
                                const float k_spring = 100.0f; // Spring constant
                                const float k_damping = 0.1f; // Damping coefficient

                                //- Calculate the spring force
                                const float delta = rad - restLength;
                                const float f_spring = -k_spring * delta;

                                //- Calculate the damping force
                                const tfloat3 velp1 = TFloat3(velrhop[p1].x, velrhop[p1].y, velrhop[p1].z);
                                const tfloat3 velp2 = TFloat3(velrhop[p2].x, velrhop[p2].y, velrhop[p2].z);
                                const tfloat3 relative_velocity = velp1 - velp2;
                                const float damping = k_damping * (relative_velocity | TFloat3(drx, dry, drz) / rad);

                                //- Calculate the resultant force
                                const float force_magnitude = f_spring - damping;
                                const float norm_factor = 1.0f / rad;

                                tfloat3 force = TFloat3(drx * norm_factor, dry * norm_factor, drz * norm_factor) * force_magnitude;

                                //- Apply the force to particle p1
                                acep1.x += force.x / ftmassp1;
                                acep1.y += force.y / ftmassp1;
                                acep1.z += force.z / ftmassp1;
                            }
                        }
                    }
                }
            }

            //- Sum results together.
            if (acep1.x || acep1.y || acep1.z) {
                ace[p1] += acep1;
            }
        }
    }

    //- Update viscdt with max value of viscdt or demdt*.
    float demdt = demdtth[0];
    for(int th = 1; th < OmpThreads; th++) {
        if (demdt < demdtth[th*OMP_STRIDE]) demdt = demdtth[th*OMP_STRIDE];
    }
    if (viscdt < demdt) viscdt = demdt;
}
