@echo off
cd /d "%~dp0"
gcc -Wall -Wextra main.c destino.c terminal.c viajes.c -o app.exe
if errorlevel 1 (
    echo Error al compilar.
    pause
) else (
    echo Compilacion exitosa.
    echo Ejecutando programa...
    app.exe
)