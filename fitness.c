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


void loadFromCSV();
void saveToCSV();
void addMember();
void searchMember();
void updateMember();
void deleteMember();
void displayMembers();
void loadMember();
void menu();


#include <stdio.h>

void loadFromCSV() {
    FILE *fp = fopen("member.csv", "r");
    if (!fp) {
        printf("Cannot open member.csv\n");
        return;
    }
    fclose(fp);
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

void addMember() {
    if (memberCount >= MAX_MEMBERS) {
        printf("Member list is full!\n");
        return;
    }

    printf("Enter name: ");
    scanf(" %[^\n]", name[memberCount]);
    printf("Enter age: ");
    scanf("%d", &age[memberCount]);
    printf("Enter membership type (Gold/Silver): ");
    scanf(" %s", membershipType[memberCount]);
    printf("Enter registration date (YYYY-MM-DD): ");
    scanf(" %s", registrationDate[memberCount]);

    memberCount++;
    saveMembers();
    printf("Member added successfully!\n");
}

void displayMembers() {
    if (memberCount == 0) {
        printf("No members available.\n");
        return;
    }

    printf("\n%-20s %-5s %-15s %-12s\n", "Name", "Age", "Membership", "Registration");
    printf("-----------------------------------------------------------\n");
    for (int i = 0; i < memberCount; i++) {
        printf("%-20s %-5d %-15s %-12s\n",
               name[i], age[i], membershipType[i], registrationDate[i]);
    }
}

void searchMember() {
    char keyword[50];
    printf("Enter name or membership type: ");
    scanf(" %[^\n]", keyword);

    int found = 0;
    for (int i = 0; i < memberCount; i++) {
        if (strcasecmp(name[i], keyword) == 0 ||
            strcasecmp(membershipType[i], keyword) == 0) {
            printf("Found: %s, %d, %s, %s\n",
                   name[i], age[i], membershipType[i], registrationDate[i]);
            found = 1;
        }
    }
    if (!found) printf("No member found.\n");
}

void updateMember() {
    char keyword[50];
    printf("Enter member name to update: ");
    scanf(" %[^\n]", keyword);

    for (int i = 0; i < memberCount; i++) {
        if (strcasecmp(name[i], keyword) == 0) {
            printf("Enter new membership type: ");
            scanf(" %s", membershipType[i]);
            saveMembers();
            printf("Member updated!\n");
            return;
        }
    }
    printf("Member not found.\n");
}

void deleteMember() {
    char keyword[50];
    printf("Enter member name to delete: ");
    scanf(" %[^\n]", keyword);

    for (int i = 0; i < memberCount; i++) {
        if (strcasecmp(name[i], keyword) == 0) {
            for (int j = i; j < memberCount - 1; j++) {
                strcpy(name[j], name[j+1]);
                age[j] = age[j+1];
                strcpy(membershipType[j], membershipType[j+1]);
                strcpy(registrationDate[j], registrationDate[j+1]);
            }
            memberCount--;
            saveMembers();
            printf("Member deleted!\n");
            return;
        }
    }
    printf("Member not found.\n");
}

int main() 
{
    loadMembers();
    menu();
    return 0;
}

void menu(){
    int choice;
    loadFromCSV();

do{
    printf(" Fitness system \n");
    printf("1. Display Members\n");
    printf("2. Add Member\n");
    printf("3. Search Member\n");
    printf("4. Update Member\n");
    printf("5. Delete Member\n");
    printf("6. Save & Exit\n");
    printf("7. Unit Test\n");
    printf("enter choice: \n");
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
        searchMember();
        break;
    case 4:
        updateMember();
        break;
    case 5:
        deleteMember();
        break;
    case 6:
        printf("data saved. exiting...\n");
        saveMembers();
        exit(0);
    case 0:
        printf("exiting without saveing...\n");
        exit(0);
    default:
        break;
    }
} while (1);
}