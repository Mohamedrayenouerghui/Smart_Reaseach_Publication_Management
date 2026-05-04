@echo off
echo ========================================
echo Fix CMake Error and Build Script
echo ========================================
echo.

REM Set Qt path (adjust if needed)
set QT_PATH=C:\Qt\6.7.3\mingw_64
set CMAKE_PATH=C:\Qt\Tools\CMake_64\bin
set MINGW_PATH=C:\Qt\Tools\mingw1120_64\bin

REM Add to PATH
set PATH=%CMAKE_PATH%;%MINGW_PATH%;%PATH%

echo Step 0: Cleaning old project build directory...
if exist "C:\Users\User\Desktop\Smart_Reaseach_Publication_Management-main\build" (
    rmdir /s /q "C:\Users\User\Desktop\Smart_Reaseach_Publication_Management-main\build"
    echo   - Old project build directory removed
) else (
    echo   - Old project build directory not found (OK)
)

echo.
echo Step 1: Cleaning current build directory...
if exist build (
    rmdir /s /q build
    echo   - Old build directory removed
) else (
    echo   - No old build directory found
)

echo.
echo Step 2: Cleaning Qt Creator cache...
if exist .qtcreator\qtc-cmake-presets-ubotNfSV (
    rmdir /s /q .qtcreator\qtc-cmake-presets-ubotNfSV
    echo   - Qt Creator cache 1 cleaned
)
if exist .qtcreator\qtc-cmake-presets-wpJgEpYO (
    rmdir /s /q .qtcreator\qtc-cmake-presets-wpJgEpYO
    echo   - Qt Creator cache 2 cleaned
)

echo.
echo Step 3: Creating new build directory...
mkdir build
cd build

echo.
echo Step 4: Running CMake configuration...
echo   Source: %CD%\..
echo   Build:  %CD%
echo   Qt:     %QT_PATH%
echo.
cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH=%QT_PATH%

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ========================================
    echo ERROR: CMake configuration failed!
    echo ========================================
    echo Please check:
    echo   1. Qt is installed at: %QT_PATH%
    echo   2. CMake is installed at: %CMAKE_PATH%
    echo   3. MinGW is installed at: %MINGW_PATH%
    echo.
    echo Try adjusting the paths at the top of this script.
    echo.
    pause
    exit /b 1
)

echo.
echo Step 5: Building the project...
cmake --build . --config Debug

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ========================================
    echo ERROR: Build failed!
    echo ========================================
    echo Check the error messages above.
    echo.
    pause
    exit /b 1
)

echo.
echo ========================================
echo BUILD SUCCESSFUL!
echo ========================================
echo.
echo Executable location: build\Debug\ResearchManagementSystem.exe
echo                  or: build\ResearchManagementSystem.exe
echo.
echo To run the application:
echo   1. Make sure Oracle database is running
echo   2. Run the executable from build directory
echo.
echo Press any key to launch the application...
pause

REM Try to launch the application
if exist Debug\ResearchManagementSystem.exe (
    echo Launching from Debug folder...
    start Debug\ResearchManagementSystem.exe
) else if exist ResearchManagementSystem.exe (
    echo Launching from build folder...
    start ResearchManagementSystem.exe
) else (
    echo.
    echo Could not find executable. Please check build directory manually.
    echo.
    pause
)
