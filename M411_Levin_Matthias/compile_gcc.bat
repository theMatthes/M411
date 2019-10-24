cls
gcc main.cpp -Wno-narrowing -o Aufgabe.exe -g -O2 -fPIE -fstack-protector
rem -Wextra -Wall -D_FORTIFY_SOURCE=2 -D_GLIBCXX_ASSERTIONS -fasynchronous-unwind-tables -fexceptions -fpie -Wl,-pie -fstack-protector -fstack-clash-protection -fcf-protection
Aufgabe.exe
pause;
compile_gcc.bat