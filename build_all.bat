@ECHO OFF

REM Build script for cleaning and/or building everything

SET PLATFORM=%1
SET ACTION=%2
SET TARGET=%3

if "%ACTION%" == "build" (
    SET ACTION=all
    SET ACTION_STR=Building
    SET ACTION_STR_PAST=built
) else (
    if "%ACTION%" == "clean" (
        SET ACTION=clean
        SET ACTION_STR=Cleaning
        SET ACTION_STR_PAST=cleaned
    ) else (
        echo Invalid action: %ACTION%
        exit /b 1
    )
)

if "%PLATFORM%" == "windows" (
    SET ENGINE_LINK=-luser32 -lOpenGL32
    SET PLATFORM_DIR=windows
) else (
    if "%PLATFORM%" == "linux" (
        SET ENGINE_LINK=
        SET PLATFORM_DIR=linux
    ) else (
        if "%PLATFORM%" == "macos" (
            SET ENGINE_LINK=
            SET PLATFORM_DIR=macos
        ) else (
            echo Invalid platform: %PLATFORM%
            exit /b 1
        )
    )
)

ECHO %ACTION_STR% everything on %PLATFORM% (%TARGET%)...

@REM --------------------------------------------------------------------------
@REM Libraries
@REM --------------------------------------------------------------------------

REM Engine core library
make -j -f Makefile.lib.mak %ACTION% TARGET=%TARGET% ASSEMBLY=engine-lib OUTPUT=engine PLATFORM=%PLATFORM_DIR% LDFLAGS="%ENGINE_LINK%"
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit /b %ERRORLEVEL%)

REM Game library
make -j -f Makefile.lib.mak %ACTION% TARGET=%TARGET% ASSEMBLY=game-lib OUTPUT=game PLATFORM=%PLATFORM_DIR% LDFLAGS="-Lbuild/%PLATFORM_DIR%/bin -lengine"
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit /b %ERRORLEVEL%)

REM Editor library
make -j -f Makefile.lib.mak %ACTION% TARGET=%TARGET% ASSEMBLY=editor-lib OUTPUT=editor PLATFORM=%PLATFORM_DIR% LDFLAGS="-Lbuild\%PLATFORM_DIR%\bin -lengine"
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit /b %ERRORLEVEL%)

@REM --------------------------------------------------------------------------
@REM Executables
@REM --------------------------------------------------------------------------

REM Editor
make -j -f Makefile.exe.mak %ACTION% TARGET=%TARGET% ASSEMBLY=editor PLATFORM=%PLATFORM_DIR% LDFLAGS="-Lbuild\%PLATFORM_DIR%\bin -lengine -leditor -lgame"
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit /b %ERRORLEVEL%)

REM Game
make -j -f Makefile.exe.mak %ACTION% TARGET=%TARGET% ASSEMBLY=game PLATFORM=%PLATFORM_DIR% LDFLAGS="-Lbuild\%PLATFORM_DIR%\bin -lengine -lgame"
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit /b %ERRORLEVEL%)

ECHO All assemblies %ACTION_STR_PAST% successfully.
