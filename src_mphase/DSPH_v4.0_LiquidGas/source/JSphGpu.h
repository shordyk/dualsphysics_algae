//HEAD_DSPH
/*
<DUALSPHYSICS>  Copyright (c) 2018 by Dr Jose M. Dominguez et al. (see http://dual.sphysics.org/index.php/developers/). 

EPHYSLAB Environmental Physics Laboratory, Universidade de Vigo, Ourense, Spain.
School of Mechanical, Aerospace and Civil Engineering, University of Manchester, Manchester, U.K.

This file is part of DualSPHysics. 

DualSPHysics is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License 
as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

DualSPHysics is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details. 

You should have received a copy of the GNU Lesser General Public License along with DualSPHysics. If not, see <http://www.gnu.org/licenses/>. 
*/

/// \file JSphGpu.h \brief Declares the class \ref JSphGpu.

#ifndef _JSphGpu_
#define _JSphGpu_

#include "DualSphDef.h"
#include "JSphTimersGpu.h"
#include "JSph.h"
#include <string>

class JPartsOut;
class JArraysGpu;
class JCellDivGpu;
class JBlockSizeAuto;

//##############################################################################
//# JSphGpu
//##############################################################################
/// \brief Defines the attributes and functions used only in GPU simulations.

class JSphGpu : public JSph
{
private:
  JCellDivGpu* CellDiv;

///Structure that stores the block size to be used in each interaction kernel during GPU execution.
public:
  typedef struct {
    unsigned forcesfluid;
    unsigned forcesbound;
    unsigned forcesdem;
  }StBlockSizes;

protected:
  StBlockSizes BlockSizes;        ///<Almacena configuracion de BlockSizes. Stores configuration of BlockSizes
  std::string BlockSizesStr;      ///<Almacena configuracion de BlockSizes en texto. Stores configuration of BlockSizes in text form
  TpBlockSizeMode BlockSizeMode;  ///<Modes for BlockSize selection.
  JBlockSizeAuto *BsAuto;         ///<Object to calculate the optimum BlockSize for particle interactions.

  //-Vars. con informacion del hardware GPU.
  //-Variables with information for the GPU hardware
  int GpuSelect;          ///<Gpu seleccionada (-1:sin seleccion).  GPU Selection (-1:no selection)
  std::string GpuName;    ///<Nombre de la gpu seleccionada.  Name of the selected GPU
  size_t GpuGlobalMem;    ///<Tama�o de memoria global en bytes.  Size of global memory in bytes
  unsigned GpuSharedMem;  ///<Tama�o de memoria shared por bloque en bytes. Size of shared memory for each block in bytes
  unsigned GpuCompute;    ///<Compute capability: 10,11,12,20... 

  std::string RunMode;     ///<Almacena modo de ejecucion (simetria,openmp,balanceo,...). Stores execution mode (symmetry,OpenMP,balance)

  //-Numero de particulas del dominio.
  //-Number of particles in the domain
  unsigned Np;     ///<Numero total de particulas (incluidas las duplicadas periodicas). ETotal number of particles (including duplicate periodic particles)
  unsigned Npb;    ///<Numero de particulas contorno (incluidas las contorno periodicas). Number of boundary particles (including periodic boundaries)
  unsigned NpbOk;  ///<Numero de particulas contorno cerca del fluido (incluidas las contorno periodicas). Number of boundary particles interacting the fluid (including the periodic bounaries)

  unsigned NpfPer;   ///<Numero de particulas fluidas-floating periodicas. Number of periodic particles (fluid-floating)
  unsigned NpbPer;   ///<Numero de particulas contorno periodicas. Number of periodic boundary particles
  unsigned NpfPerM1; ///<Numero de particulas fluidas-floating periodicas (valores anteriores). Number of fluid-floating periodic particles (previous values)
  unsigned NpbPerM1; ///<Numero de particulas contorno periodicas (valores anteriores). Number of periodic boundary particles (previous values)

  bool WithFloating;
  bool BoundChanged; ///<Indica si el contorno seleccionado a cambiado desde el ultimo divide. Indicates if a selected boundary particle has changed since the last time step

  unsigned CpuParticlesSize; ///<Numero de particulas para las cuales se reservo memoria en cpu. Number of particles for which CPU memory was allocated
  llong MemCpuParticles;     ///<Mermoria reservada para vectores de datos de particulas. Allocated CPU memory for arrays with particle data

  //-Variables con datos de las particulas para ejecucion (size=ParticlesSize).
  //-Variables holding particle data for the execution (size=ParticlesSize)
  unsigned *Idp;   ///<Identificador de particula. Particle identifier
  word *Code;      ///<Indica el grupo de las particulas y otras marcas especiales. Indicates particle group and other special marks
  unsigned *Dcell; ///<Celda dentro de DomCells codificada con DomCellCode. Cell within DomCells encoded with DomCellCode.
  tdouble2 *Posxy;
  double *Posz;
  tfloat4 *Velrhop;

  //-Variables auxiliares para conversion (size=ParticlesSize).
  //-Auxiliary variables for the conversion (size=ParticlesSize)
  tdouble3 *AuxPos;
  tfloat3 *AuxVel; 
  float *AuxRhop;

  unsigned GpuParticlesSize;  ///<Numero de particulas para las cuales se reservo memoria en gpu. Number of particles for which GPU memory was allocated
  llong MemGpuParticles;      ///<Mermoria reservada para vectores de datos de particulas. Allocated GPU memory for arrays with particle data
  llong MemGpuFixed;          ///<Mermoria reservada en AllocGpuMemoryFixed. Allocated memory in AllocGpuMemoryFixed

  //-Posicion de particula segun id para motion.
  //-Particle position according to the identifier for the motion
  unsigned *RidpMoveg;  ///<Solo para boundary moving particles [CaseNmoving] y cuando CaseNmoving!=0 Only for moving boundary particles [CaseNmoving] and when CaseNmoving!=0

  //-Lista de arrays en Gpu para particulas.
  //-List of arrays in the GPU gor the particles
  JArraysGpu* ArraysGpu;
  //-Variables con datos de las particulas para ejecucion (size=ParticlesSize).
  //-Variables holding particle data for the execution (size=ParticlesSize)
  unsigned *Idpg;   ///<Identificador de particula. Particle identifier
  word *Codeg;      ///<Indica el grupo de las particulas y otras marcas especiales. Indicates paricle group and other special marks
  unsigned *Dcellg; ///<Celda dentro de DomCells codificada con DomCellCode. Cell within DomCells encoded within DomCellCode
  double2 *Posxyg;
  double *Poszg;
  float4 *Velrhopg;

//�����������������������������������������������������������������������//MP
  tsymatrix3f *Surfg;		///<Array used for calculating surface tension forces
  bool SurfCoef;			///<Shows whether surface tension formulation will be used and initialises the required array
  StPhaseData *PhaseData;	///<Array storing data for each component
  StPhaseData *PhaseDatag;	///<Array storing data for each component
//�����������������������������������������������������������������������//MP
    
  //-Vars. para compute step: VERLET
  //-Variables for compute step: Verlet
  float4 *VelrhopM1g;  ///<Verlet: para guardar valores anteriores Verlet: for maintaining previous values
  int VerletStep;

  //-Vars. para compute step: SYMPLECTIC
  //-Variables for compute step: Symplectic
  double2 *PosxyPreg;  ///<Sympletic: para guardar valores en predictor Symplectic: for maintaining predictor values
  double *PoszPreg;
  float4 *VelrhopPreg;
  double DtPre;   

  //-Variables for floating bodies.
  unsigned *FtRidpg;         ///<Identifier to access the particles of the floating object [CaseNfloat] in GPU.
  float *FtoMasspg;          ///<Mass of the particle for each floating body [FtCount] in GPU (used in interaction forces).

  float4 *FtoDatag;    ///<Datos constantes de floatings {pini_u,np_u,radius_f,mass_f} [FtCount] //__device__ int __float_as_int(float x) //__device__ float __int_as_float(int x) Constant data of floating bodies {pini_u,np_u,radius_f,mass_f} [FtCount] //__device__ int __float_as_int (float x) //__device__ float __int_as_float(int x)
  float3 *FtoForcesg;  ///<Almacena fuerzas de floatings {face_f3,fomegavel_f3} equivalente a JSphCpu::FtoForces [FtCount]. Stores forces for the floating bodies {face_f3,fomegavel_f3} equivalent to JSphCpu::FtoForces [FtCount]

  double3 *FtoCenterg; ///<Mantiene centro de floating. [FtCount]   Maintains centre of floating bodies [Ftcount]
  float3 *FtoVelg;     ///<Mantiene vel de floating. [FtCount]  Maintains velocity of floating bodies [FtCount]
  float3 *FtoOmegag;   ///<Mantiene omega de floating. [FtCount]  Maintains omega of floating bodies [FtCount]

  StFtoForces *FtoForces; ///<Almacena fuerzas de floatings en CPU [FtCount]. Stores forces for floating bodies on the CPU
  tdouble3 *FtoCenter;    ///<Almacena centro de floating en CPU. [FtCount]  Stores centre of floating bodies on the CPU

  //-Variables for DEM. (DEM)
  float4 *DemDatag;          ///<Data of the object {mass, (1-poisson^2)/young, kfric, restitu} in GPU [DemObjsSize].

  //-Vars. para computo de fuerzas.
  //-Variables for computing forces
  float4 *PsPospressg; ///<Posicion y press para interaccion Pos-Simple. press=cteb*(powf(rhop/rhopzero,gamma)-1.0f); Position and pressure for the interaction Pos-Simple press=cteb*(powf(rhop/rhopzero,gamma)-1.0f);

  float *ViscDtg;
  float3 *Aceg;      ///<Acumula fuerzas de interaccion EAccumulates acceleration of the particles
  float *Arg; 
  float *Deltag;     ///<Acumula ajuste de Delta-SPH con DELTA_DynamicExt Accumulates adjustment of Delta-SPH with DELTA_DynamicExt

  float3 *ShiftPosg;    ///<Particle displacement using Shifting.
  float *ShiftDetectg;  ///<Used to detect free surface with Shifting.

  double VelMax;      ///<Maximum value of Vel[] sqrt(vel.x^2 + vel.y^2 + vel.z^2) computed in PreInteraction_Forces().
  double AceMax;      ///<Maximum value of Ace[] (ace.x^2 + ace.y^2 + ace.z^2) computed in Interaction_Forces().
  float ViscDtMax;    ///<Valor maximo de ViscDt calculado en Interaction_Forces(). Maximum value of ViscDt computed in Interaction_Forces()

//�����������������������������������������������������������������������//MP
  double RhopMax;      ///<Maximum value of Rhop[] computed in Interaction_Forces().
//�����������������������������������������������������������������������//MP

  //-Variables for Laminar+SPS viscosity.  
  tsymatrix3f *SpsTaug;       ///<SPS sub-particle stress tensor.
  tsymatrix3f *SpsGradvelg;   ///<Velocity gradients.
  
  TimersGpu Timers;  ///<Declares an array with timers for CPU (type structure \ref StSphTimerGpu).

    void InitVars();
  void RunExceptionCuda(const std::string &method,const std::string &msg,cudaError_t error);
  void CheckCudaError(const std::string &method,const std::string &msg);

  void FreeGpuMemoryFixed();
  void AllocGpuMemoryFixed();
  void FreeCpuMemoryParticles();
  void AllocCpuMemoryParticles(unsigned np);
  void FreeGpuMemoryParticles();
  void AllocGpuMemoryParticles(unsigned np,float over);

  void ResizeGpuMemoryParticles(unsigned np);
  void ReserveBasicArraysGpu();

  template<class T> T* TSaveArrayGpu(unsigned np,const T *datasrc)const;
  word*        SaveArrayGpu(unsigned np,const word        *datasrc)const{ return(TSaveArrayGpu<word>       (np,datasrc)); }
  unsigned*    SaveArrayGpu(unsigned np,const unsigned    *datasrc)const{ return(TSaveArrayGpu<unsigned>   (np,datasrc)); }
  float*       SaveArrayGpu(unsigned np,const float       *datasrc)const{ return(TSaveArrayGpu<float>      (np,datasrc)); }
  float4*      SaveArrayGpu(unsigned np,const float4      *datasrc)const{ return(TSaveArrayGpu<float4>     (np,datasrc)); }
  double*      SaveArrayGpu(unsigned np,const double      *datasrc)const{ return(TSaveArrayGpu<double>     (np,datasrc)); }
  double2*     SaveArrayGpu(unsigned np,const double2     *datasrc)const{ return(TSaveArrayGpu<double2>    (np,datasrc)); }
  tsymatrix3f* SaveArrayGpu(unsigned np,const tsymatrix3f *datasrc)const{ return(TSaveArrayGpu<tsymatrix3f>(np,datasrc)); }
  unsigned*    SaveArrayGpu_Uint(unsigned np,const unsigned *datasrc)const;
  template<class T> void TRestoreArrayGpu(unsigned np,T *data,T *datanew)const;
  void RestoreArrayGpu(unsigned np,word        *data,word        *datanew)const{ TRestoreArrayGpu<word>       (np,data,datanew); }
  void RestoreArrayGpu(unsigned np,unsigned    *data,unsigned    *datanew)const{ TRestoreArrayGpu<unsigned>   (np,data,datanew); }
  void RestoreArrayGpu(unsigned np,float       *data,float       *datanew)const{ TRestoreArrayGpu<float>      (np,data,datanew); }
  void RestoreArrayGpu(unsigned np,float4      *data,float4      *datanew)const{ TRestoreArrayGpu<float4>     (np,data,datanew); }
  void RestoreArrayGpu(unsigned np,double      *data,double      *datanew)const{ TRestoreArrayGpu<double>     (np,data,datanew); }
  void RestoreArrayGpu(unsigned np,double2     *data,double2     *datanew)const{ TRestoreArrayGpu<double2>    (np,data,datanew); }
  void RestoreArrayGpu(unsigned np,tsymatrix3f *data,tsymatrix3f *datanew)const{ TRestoreArrayGpu<tsymatrix3f>(np,data,datanew); }
  void RestoreArrayGpu_Uint(unsigned np,unsigned *data,unsigned *datanew)const;

  llong GetAllocMemoryCpu()const;
  llong GetAllocMemoryGpu()const;
  void PrintAllocMemory(llong mcpu,llong mgpu)const;

  void ConstantDataUp();
  void ParticlesDataUp(unsigned n);
  unsigned ParticlesDataDown(unsigned n,unsigned pini,bool code,bool cellorderdecode,bool onlynormal);
  
  void SelecDevice(int gpuid);
  void ConfigBlockSizes(bool usezone,bool useperi);

  void ConfigRunMode(std::string preinfo);
  void ConfigCellDiv(JCellDivGpu* celldiv){ CellDiv=celldiv; }
  void InitFloating();
  void InitRun();

  void AddAccInput();

  void PreInteractionVars_Forces(TpInter tinter,unsigned np,unsigned npb);
  void PreInteraction_Forces(TpInter tinter);
//�����������������������������������������������������������������������//MP
  void PreInteraction_ForcesMP(TpInter tinter);
//�����������������������������������������������������������������������//MP
  void PosInteraction_Forces();
  
//�����������������������������������������������������������������������//MP
  void ComputeVerletMP(double dt);
  void ComputeSymplecticPreMP(double dt);
  void ComputeSymplecticCorrMP(double dt);
//�����������������������������������������������������������������������//MP
  void ComputeVerlet(double dt);
  void ComputeSymplecticPre(double dt);
  void ComputeSymplecticCorr(double dt);
  double DtVariable(bool final);
  void RunShifting(double dt);

  void RunMotion(double stepdt);

  void ShowTimers(bool onlyfile=false);
  void GetTimersInfo(std::string &hinfo,std::string &dinfo)const;
  unsigned TimerGetCount()const{ return(TmgGetCount()); }
  bool TimerIsActive(unsigned ct)const{ return(TmgIsActive(Timers,(CsTypeTimerGPU)ct)); }
  float TimerGetValue(unsigned ct)const{ return(TmgGetValue(Timers,(CsTypeTimerGPU)ct)); }
  const double* TimerGetPtrValue(unsigned ct)const{ return(TmgGetPtrValue(Timers,(CsTypeTimerGPU)ct)); }
  std::string TimerGetName(unsigned ct)const{ return(TmgGetName((CsTypeTimerGPU)ct)); }
  std::string TimerToText(unsigned ct)const{ return(JSph::TimerToText(TimerGetName(ct),TimerGetValue(ct))); }

public:
  JSphGpu();
  ~JSphGpu();
};

#endif


