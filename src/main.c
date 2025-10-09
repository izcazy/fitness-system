#include <stdio.h>
#include <stdlib.h>
#include "members.h"
#include "tests.h"
#include "platform.h"

static void pressEnterToContinue(void)
{
    int c;
    printf("\nPress Enter to continue...");
    do 
    {
        c = getchar();
        if (c == '\n') break;
    } 
    while (c != EOF);
    getchar();
}

static void menu(void)
{
    int choice;
    do 
    {
        clearScreen();
        printf("=================\n");
        printf(" Fitness system\n");
        printf("=================\n");
        printf("1. Display Members\n");
        printf("2. Add Member\n");
        printf("3. Search Member\n");
        printf("4. Update Member\n");
        printf("5. Delete Member\n");
        printf("6. Save & Exit\n");
        printf("7. Unit Test\n");
        printf("8. E2E Test\n");
        printf("----------------\n");
        printf("enter choice: ");
        if (scanf("%d", &choice) != 1)
        {
            int c; while ((c=getchar())!='\n' && c!=EOF){}
            choice = -1;
        }

        switch (choice)
        {
            case 1: displayMembers(); break;
            case 2: addMember(); break;
            case 3: searchMember(); break;
            case 4: updateMember(); break;
            case 5: deleteMember(); break;
            case 6: printf("data saved. exiting...\n"); saveMembers(); exit(0);
            case 7: runUnitTests(); break;
            case 8: runE2EAllErrors_Pretty(); break;
            case 0: printf("exiting without saving...\n"); exit(0);
            default: printf("Invalid choice. Try again.\n"); break;
        }
        pressEnterToContinue();
    } while (1);
}

int main(void)
{
    loadMembers();
    menu();
    return 0;
}
