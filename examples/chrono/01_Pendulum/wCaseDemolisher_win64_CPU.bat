@echo off
setlocal EnableDelayedExpansion
rem Don't remove the two jump line after than the next line [set NL=^]
set NL=^


rem "name" and "dirout" are named according to the testcase

set name=CaseDemolisher
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

%dualsphysicscpu% %dirout%/%name% %dirout% -dirdataout data -svres
if not "%ERRORLEVEL%" == "0" goto fail

:postprocessing
set dirout2=%dirout%\particles
%partvtk% -dirin %diroutdata% -savevtk %dirout2%/PartFloating -onlytype:-all,+floating
if not "%ERRORLEVEL%" == "0" goto fail

%partvtkout% -dirin %diroutdata% -savevtk %dirout2%/PartFluidOut -SaveResume %dirout2%/_ResumeFluidOut
if not "%ERRORLEVEL%" == "0" goto fail

set dirout2=%dirout%\boundary
%boundaryvtk% -loadvtk AutoActual -motiondata %diroutdata% -savevtkdata %dirout2%/Box.vtk -onlymk:20
if not "%ERRORLEVEL%" == "0" goto fail

%boundaryvtk% -loadvtk AutoActual -motiondata %diroutdata% -savevtkdata %dirout2%/MotionPendulum -onlymk:10-13
if not "%ERRORLEVEL%" == "0" goto fail

%boundaryvtk% -loadvtk AutoActual -motiondata %diroutdata% -savevtkdata %dirout2%/MotionBall -onlymk:14
if not "%ERRORLEVEL%" == "0" goto fail

%boundaryvtk% -loadvtk AutoActual -motiondata %diroutdata% -savevtkdata %dirout2%/MotionBoxes -onlymk:61-76
if not "%ERRORLEVEL%" == "0" goto fail



:success
echo All done
goto end

:fail
echo Execution aborted.

:end
pause
