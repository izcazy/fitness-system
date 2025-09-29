#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MEMBERS 100
#define FILENAME "member.csv"

char name[MAX_MEMBERS][50];
int age[MAX_MEMBERS];
char membershipType[MAX_MEMBERS][20];
char registrationDate[MAX_MEMBERS][20];
int memberCount = 0;


void loadMembers();
void saveMembers();
void addMembers();
void displayMembers();
void searchMembers();
void updateMembers();
void deleteMembers();
void menu();

int main()
{
    loadMembers();
    menu();
    return 0;
}

void loadMembers() {
    FILE *file = fopen(FILENAME, "r");
    if (!file) {
        printf("No existing data found. A new file will be created.\n");
        return;
    }

    char line[200];
    fgets(line, sizeof(line), file); // Skip header

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%49[^,],%d,%19[^,],%19[^\n]",
               name[memberCount],
               &age[memberCount],
               membershipType[memberCount],
               registrationDate[memberCount]);
        memberCount++;
    }
    fclose(file);
}

void saveMembers() {
    FILE *file = fopen(FILENAME, "w");
    if (!file) {
        printf("Error: cannot open file to save. \n");
        return;
    }

    fprintf(file, "MemberName,Age,MembershipType,RegistrationDate\n");
    for (int i = 0; i < memberCount; i++) {
        fprintf(file, "%s,%d,%s,%s\n",
                name[i], age[i], membershipType[i], registrationDate[i]);
    }
    fclose(file);
}


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