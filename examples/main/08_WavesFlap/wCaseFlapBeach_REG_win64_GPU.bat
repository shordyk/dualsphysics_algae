@echo off
setlocal EnableDelayedExpansion
rem Don't remove the two jump line after than the next line [set NL=^]
set NL=^


rem "name" and "dirout" are named according to the testcase

set name=CaseFlapBeach_REG
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

%gencase% %name%_Def %dirout%/%name% -save:all 
if not "%ERRORLEVEL%" == "0" goto fail

%dualsphysicsgpu% -gpu %dirout%/%name% %dirout% -dirdataout data -svres
if not "%ERRORLEVEL%" == "0" goto fail

:postprocessing
set dirout2=%dirout%\particles
%partvtk% -dirin %diroutdata% -savevtk %dirout2%/PartFluid -onlytype:-all,+fluid
if not "%ERRORLEVEL%" == "0" goto fail

%partvtk% -dirin %diroutdata% -savevtk %dirout2%/PartFlap -onlytype:-all,+moving
if not "%ERRORLEVEL%" == "0" goto fail

%partvtkout% -dirin %diroutdata% -savevtk %dirout2%/PartFluidOut -SaveResume %dirout2%/_ResumeFluidOut
if not "%ERRORLEVEL%" == "0" goto fail

set dirout2=%dirout%\tracer
%tracerparts% -dirin %diroutdata% -savevtk %dirout2%/ColParts -onlytype:-all,+fluid -onlypos:2:-1:0.05:7:1:1 -nearpartsdist:0.04 -nearpartsdef:ptls[x=3:1:4,z=0.1:0.1:11] -tailsize:40
if not "%ERRORLEVEL%" == "0" goto fail

set dirout2=%dirout%\measuretool
%measuretool% -dirin %diroutdata% -pointsdef:ptls[x=5:1:3,y=0:0:1,z=0.2:0.001:1000] -onlytype:-all,+fluid -elevation:0.4 -savecsv %dirout2%/_elevation -savevtk %dirout2%/elevation 
if not "%ERRORLEVEL%" == "0" goto fail

%measuretool% -dirin %diroutdata% -pointsdef:pt=6:0:0.4 -onlytype:-all,+fluid -vars:-all,+vel -savecsv %dirout2%/_velocity -savevtk %dirout2%/velocity
if not "%ERRORLEVEL%" == "0" goto fail

set dirout2=%dirout%\surface
%isosurface% -dirin %diroutdata% -saveslice %dirout2%/Slices 
if not "%ERRORLEVEL%" == "0" goto fail


:success
echo All done
goto end

:fail
echo Execution aborted.

:end
pause
