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

//เมนูหลัก
int main(){
    int choice;
    loadFromCSV();

do{
    printf("\n=== Fitness system ===\n");
    printf("1. Display Members\n");
    printf("2. Add Member\n");
    printf("3. Search Member\n");
    printf("4. Update Member\n");
    printf("5. Delete Member\n");
    printf("6. Save & Exit\n");
    scanf("%d",&choice);

    switch (choice)
    {
    case 1:
        displayMembers();
        break;
    case 2:
        addMember();
        break;
    case 3:
        searchMenber();
        break;
    case 4:
        updateMember();
        break;
    default:
        break;
    }


} while (choice!=0);
{
    return 0;
}
}