@echo off

if "%1" == "build" (
    nasm -f win64 map.asm -o map.o
    gcc -shared map.o -o map.so
    gcc demo.c map.so -o c_demo
)

if "%1" == "c_demo" (
    c_demo.exe
)

if "%1" == "py_demo" (
    python demo.py
)