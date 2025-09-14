#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define FILENAME "member.csv"

typedef struct {
    int id;
    char name[50];
    int age;
    char membership[20];
} Member;

//โหลดข้อมูลจากไฟล์
void loadFromCSV(){
    FILE *fp = fopen(FILENAME,"r");
    if(!fp)
    {
        printf("ไม่พบไฟล์ %s ");
        return;
    }
}