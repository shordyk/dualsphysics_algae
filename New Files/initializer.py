# Starting with just generating random distribution of spheres
# Using package xml.etree.ElementTree and random library

import random
import xml.etree.ElementTree as ET

# Key Parameters

count = 10 #Number of algae elements
x_range = (0,5)
y_range = (0,5)
z_range = (0,5)
z_val = (z_range[0]+z_range[1])/2 # midpoint, or wherever water level is 
sphere_rad = 0.25

# Physical parameters, materials. Buoyancy


# Generate random distribution of a specified number of algae elements
    # Eventually, want to control density of connections
    # Form connections with each immediate neighbor, then randomly delete based on density
def generate_algae_spheres(count, x_range, y_range, z_val, sphere_rad):
    algaes = []
    for _ in range(count):
        a = random.uniform(x_range[0], x_range[1])
        b = random.uniform(y_range[0], y_range[1])
        algaes.append((a,b, z_val, sphere_rad))
    return algaes

def create_xml_file(count, x_range, y_range, z_range, z_val, sphere-rad):

    # Create root element, from package
    case = ET.Element("case")

    casedef = ET.SubElement(case, "casedef")

    # Add in constants
    constantsdef = ET.SubElement(casedef, "constantsdef")
    ET.SubElement(constantsdef, "gravity", x="0", y="0", z="-9.81", comment="Gravitational acceleration", units_comment="m/s^2")
    ET.SubElement(constantsdef, "rhop0", value="1000", comment="Reference density of the fluid", units_comment="kg/m^3")
    ET.SubElement(constantsdef, "rhopgradient", value="2", comment="Initial density gradient 1:Rhop0, 2:Water column, 3:Max. water height (default=2)")
    ET.SubElement(constantsdef, "hswl", value="0.8", auto="false", comment="Maximum still water level to calculate speedofsound using coefsound", units_comment="metres (m)")
    ET.SubElement(constantsdef, "gamma", value="7", comment="Polytropic constant for water used in the state equation")
    ET.SubElement(constantsdef, "speedsystem", value="0", auto="true", comment="Maximum system speed (by default the dam-break propagation is used)")
    ET.SubElement(constantsdef, "coefsound", value="30", comment="Coefficient to multiply speedsystem")
    ET.SubElement(constantsdef, "speedsound", value="0", auto="true", comment="Speed of sound to use in the simulation (by default speedofsound=coefsound*speedsystem)")
    ET.SubElement(constantsdef, "coefh", value="1.2", comment="Coefficient to calculate the smoothing length (h=coefh*sqrt(3*dp^2) in 3D)")
    ET.SubElement(constantsdef, "cflnumber", value="0.2", comment="Coefficient to multiply dt")
    
    mkconfig =
    
    # Add geometry element
    geometry = ET.SubElement(casedef, "geometry")
    definition = ET.SubElement(geometry, "definition", dp="0.01", units_comment="metres (m)")
    ET.SubElement(definition, "pointref", x="0", y="0", z="0")
    ET.SubElement(definition, "pointmin", x=str(x_range[0]), y=str(y_range[0]), z=str(Z_VALUE - 0.02))
    ET.SubElement(definition, "pointmax", x=str(X_RANGE[1]), y=str(Y_RANGE[1]), z=str(Z_VALUE + 6))  # Adjust z max as needed
    

    
    # Add floatings
   
    # Add properties
    

    # Add execution element and parameters
    execution = ET.SubElement(case, "execution")
    parameters = ET.SubElement(execution, "parameters")
    ET.SubElement(parameters, "parameter", key="SavePosDouble", value="0", comment="Saves particle position using double precision (default=0)")
    ET.SubElement(parameters, "parameter", key="StepAlgorithm", value="2", comment="Step Algorithm 1:Verlet, 2:Symplectic (default=1)")
    ET.SubElement(parameters, "parameter", key="VerletSteps", value="40", comment="Verlet only: Number of steps to apply Euler timestepping (default=40)")
    ET.SubElement(parameters, "parameter", key="Kernel", value="2", comment="Interaction Kernel 1:Cubic Spline, 2:Wendland (default=2)")
    ET.SubElement(parameters, "parameter", key="ViscoTreatment", value="1", comment="Viscosity formulation 1:Artificial, 2:Laminar+SPS (default=1)")
    ET.SubElement(parameters, "parameter", key="Visco", value="0.01", comment="Viscosity value")
    ET.SubElement(parameters, "parameter", key="ViscoBoundFactor", value="1", comment="Multiply viscosity value with boundary (default=1)")
    ET.SubElement(parameters, "parameter", key="DensityDT", value="0", comment="Density Diffusion Term 0:None, 1:Molteni, 2:Fourtakas, 3:Fourtakas(full) (default=0)")
    ET.SubElement(parameters, "parameter", key="DensityDTvalue", value="0.1", comment="DDT value (default=0.1)")
    ET.SubElement(parameters, "parameter", key="Shifting", value="0", comment="Shifting mode 0:None, 1:Ignore bound, 2:Ignore fixed, 3:Full (default=0)")
    ET.SubElement(parameters, "parameter", key="ShiftCoef", value="-2", comment="Coefficient for shifting computation (default=-2)")
    ET.SubElement(parameters, "parameter", key="ShiftTFS", value="0", comment="Threshold to detect free surface. Typically 1.5 for 2D and 2.75 for 3D (default=0)")
    ET.SubElement(parameters, "parameter", key="RigidAlgorithm", value="2", comment="Rigid Algorithm 0:collision-free, 1:SPH, 2:DEM, 3:Chrono (default=1)")
    ET.SubElement(parameters, "parameter", key="FtPause", value="0.0", comment="Time to freeze the floatings at simulation start (warmup) (default=0)", units_comment="seconds")
    ET.SubElement(parameters, "parameter", key="CoefDtMin", value="0.05", comment="Coefficient to calculate minimum time step dtmin=coefdtmin*h/speedsound (default=0.05)")
    ET.SubElement(parameters, "parameter", key="DtIni", value="0", comment="Initial time step. Use 0 to defult use (default=h/speedsound)", units



