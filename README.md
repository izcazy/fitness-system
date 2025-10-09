# Project final
# fitness-system
# How to compile

C:\Users\MSII\OneDrive\Programming Fundamental\fitness-system

gcc -std=c11 -Wall -Wextra -O2 -Iinclude src/main.c src/members.c src/tests_unit.c src/tests_e2e.c -o fitness.exe

.\fitness.exe

//โปรแกรมนี้ไม่รองรับภาษาไทย
//เมื่อ run unit test และ e2e test จะมีให้ยืนยัน กรุณากด 'y' เพื่อที่จะสามารถ test ต่อได้