@echo off
:: Build engine first
echo --- Building Engine ---
cd engine
make
if %errorlevel% neq 0 (
    echo Failed to build engine.
    exit /b %errorlevel%
)
cd ..

:: Build game next
echo --- Building Game ---
cd game
make
if %errorlevel% neq 0 (
    echo Failed to build game.
    exit /b %errorlevel%
)
cd ..

echo --- Build All Completed Successfully ---
