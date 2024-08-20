@echo off
setlocal EnableDelayedExpansion
rem Don't remove the two jump line after than the next line [set NL=^]
set NL=^


REM Altomare, Tagliafierro, Dominguez, Suzuki, Viccione. Improved relaxation zone method in SPH-based model for coastal engineering applications, Applied Ocean Research, Volume 81, 2018, Pages 15-33, ISSN 0141-1187, https://doi.org/10.1016/j.apor.2018.09.013.

rem "name" and "dirout" are named according to the testcase for DualSPHysics and SWASH

set name=CaseRZ_IrregularWaves
set dirout=%name%_out
set diroutdata=%dirout%\data

rem "executables" are renamed and called from their directory

set dirbin=../../../bin/windows
set gencase="%dirbin%/GenCase_win64.exe"
set dualsphysicscpu="%dirbin%/DualSPHysics5.2CPU_win64.exe"
set dualsphysicsgpu="%dirbin%/DualSPHysics5.2_win64.exe"
set boundaryvtk="%dirbin%/BoundaryVTK_win64.exe"
set partvtk="%dirbin%/PartVTK_win64.exe"
set partvtkout="%dirbin%/PartVTKOut_win64.exe"
set measuretool="%dirbin%/MeasureTool_win64.exe"
set computeforces="%dirbin%/ComputeForces_win64.exe"
set isosurface="%dirbin%/IsoSurface_win64.exe"
set flowtool="%dirbin%/FlowTool_win64.exe"
set floatinginfo="%dirbin%/FloatingInfo_win64.exe"
set tracerparts="%dirbin%/TracerParts_win64.exe"

:menu
if exist %dirout% ( 
	set /p option="The folder "%dirout%" already exists. Choose an option.!NL!  [1]- Delete it and continue.!NL!  [2]- Execute post-processing.!NL!  [3]- Abort and exit.!NL!"
	if "!option!" == "1" goto run else (
		if "!option!" == "2" goto postprocessing else (
			if "!option!" == "3" goto fail else ( 
				goto menu
			)
		)
	)
)

:run
rem "dirout" to store results is removed if it already exists
if exist %dirout% rd /s /q %dirout%

rem CODES are executed according the selected parameters of execution in this testcase

rem Executes GenCase to create initial files for simulation.
%gencase% %name%_Def %dirout%/%name% -save:all
if not "%ERRORLEVEL%" == "0" goto fail

rem Executes DualSPHysics to simulate SPH method.
%dualsphysicscpu% %dirout%/%name% %dirout% -dirdataout data -svres
if not "%ERRORLEVEL%" == "0" goto fail

:postprocessing
rem Executes PartVTK to create VTK files with particles.
set dirout2=%dirout%\particles
%partvtk% -dirin %diroutdata% -savevtk %dirout2%/PartFluid -onlytype:-all,fluid -vars:+idp,+vel,+rhop,+press,+vor
if not "%ERRORLEVEL%" == "0" goto fail

rem water surface elevation and velocity measurements
set dirout2=%dirout%\measuretool
%measuretool% -dirin %diroutdata% -points elevation.txt -onlytype:-all,+fluid -elevation:0.4 -savecsv %dirout2%/Elevation
if not "%ERRORLEVEL%" == "0" goto fail
%measuretool% -dirin %diroutdata% -points velocity.txt -onlytype:-all,+fluid -vars:-all,+vel -kclimit:0.4 -savecsv %dirout2%/Velocity 
if not "%ERRORLEVEL%" == "0" goto fail


:success
echo All done
goto end

:fail
echo Execution aborted.

:end
pause

