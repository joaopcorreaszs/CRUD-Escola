@echo off
REM Compila e roda o sistema de notas escolares.
REM O compilador GCC (WinLibs) foi instalado na pasta do usuario.

set "GCC=%LOCALAPPDATA%\PackiaTools\gcc\mingw64\bin"
set "PATH=%GCC%;%PATH%"

gcc -std=c11 -Wall -Wextra -pedantic -o notas_escola.exe notas_escola.c
if errorlevel 1 (
    echo.
    echo Deu erro na compilacao. Leia as mensagens acima.
    pause
    exit /b 1
)

echo.
echo Compilado com sucesso. Iniciando o programa...
echo.
notas_escola.exe
pause
