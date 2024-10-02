@echo off
:: Clean engine first
echo --- Cleaning Engine ---
cd engine
make clean
if %errorlevel% neq 0 (
    echo Failed to clean engine.
    exit /b %errorlevel%
)
cd ..

:: Clean game next
echo --- Cleaning Game ---
cd game
make clean
if %errorlevel% neq 0 (
    echo Failed to clean game.
    exit /b %errorlevel%
)
cd ..

echo --- Cleaned all successfully ---
