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

void runUnitTests();

static void loadMembersFromFlie(const char* filename);
static void saveMembersToFile(const char* filename);



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
    memberCount = 0;
    char line[200];

    // อ่านบรรทัดแรก: ถ้าเป็น header ให้ข้าม
    if (!fgets(line, sizeof(line), file)) { fclose(file); return; }
    if (strstr(line, "MemberName") == NULL) {
        // ไม่ใช่ header → parse เป็นข้อมูลแถวแรก
        sscanf(line, "%49[^,],%d,%19[^,],%19[^\n]",
               name[memberCount], &age[memberCount],
               membershipType[memberCount], registrationDate[memberCount]);
        memberCount++;
    }

    while (fgets(line, sizeof(line), file)) {
        if (memberCount >= MAX_MEMBERS) break;
        sscanf(line, "%49[^,],%d,%19[^,],%19[^\n]",
               name[memberCount], &age[memberCount],
               membershipType[memberCount], registrationDate[memberCount]);
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

static int t_run = 0, t_fail = 0;
#define CHECK(cond) do{ t_run++; if(!(cond)){ t_fail++; \
  fprintf(stderr,"[FAIL] %s:%d -> %s\n", __FILE__, __LINE__, #cond); } }while(0)
#define CHECK_EQ_INT(a,b) CHECK((a)==(b))
#define CHECK_STREQ(a,b)  CHECK(strcmp((a),(b))==0)

static void writeFixtureWithHeader(const char* fname){
    FILE* f = fopen(fname, "w");
    fprintf(f, "MemberName,Age,MembershipType,RegistrationDate\n");
    fprintf(f, "John Doe,25,Gold,2025-01-05\n");
    fprintf(f, "Jane Smith,30,Silver,2025-02-10\n");
    fprintf(f, "Alex Park,27,Gold,2025-03-03\n");
    fclose(f);
}
static void writeFixtureNoHeader(const char* fname){
    FILE* f = fopen(fname, "w");
    fprintf(f, "Chris Kim,41,Silver,2025-04-12\n");
    fprintf(f, "Chris Kim,29,Gold,2025-04-13\n");
    fclose(f);
}

void runUnitTests(){
    // ==== Scenario 1: ไฟล์มี header ====
    writeFixtureWithHeader("test_with_header.csv");
    memberCount = 0;
    loadMembersFromFile("test_with_header.csv");
    CHECK_EQ_INT(memberCount, 3);
    CHECK_STREQ(name[0], "John Doe");
    CHECK_EQ_INT(age[1], 30);
    CHECK_STREQ(membershipType[2], "Gold");

    // ค้นหาแบบ case-insensitive
    int foundJD = 0;
    for (int i=0;i<memberCount;i++) if (strcasecmp(name[i], "john doe")==0) { foundJD=1; break; }
    CHECK(foundJD == 1);

    // เพิ่ม Mary → เซฟ → อัปเดตเป็น Gold → ลบ Alex → เซฟ → โหลดใหม่ → เช็ค Mary ยังอยู่
    strcpy(name[memberCount], "Mary Lee");
    age[memberCount] = 29;
    strcpy(membershipType[memberCount], "Silver");
    strcpy(registrationDate[memberCount], "2025-04-01");
    memberCount++;
    saveMembersToFile("test_with_header.csv");

    // update Mary → Gold
    for (int i=0;i<memberCount;i++)
        if (strcasecmp(name[i], "Mary Lee")==0) { strcpy(membershipType[i], "Gold"); break; }
    saveMembersToFile("test_with_header.csv");

    // delete Alex
    for (int i=0;i<memberCount;i++){
        if (strcasecmp(name[i], "Alex Park")==0){
            for (int j=i;j<memberCount-1;j++){
                strcpy(name[j], name[j+1]);
                age[j] = age[j+1];
                strcpy(membershipType[j], membershipType[j+1]);
                strcpy(registrationDate[j], registrationDate[j+1]);
            }
            memberCount--;
            break;
        }
    }
    saveMembersToFile("test_with_header.csv");

    // reload & check
    memberCount = 0;
    loadMembersFromFile("test_with_header.csv");
    int maryFound = 0;
    for (int i=0;i<memberCount;i++) if (strcasecmp(name[i], "Mary Lee")==0) { maryFound=1; break; }
    CHECK(maryFound == 1);

    // ==== Scenario 2: ไฟล์ไม่มี header + ชื่อซ้ำ ====
    writeFixtureNoHeader("test_no_header.csv");
    memberCount = 0;
    loadMembersFromFile("test_no_header.csv");
    CHECK_EQ_INT(memberCount, 2);
    // ลบ Chris Kim ตัวแรก
    for (int i=0;i<memberCount;i++){
        if (strcasecmp(name[i], "Chris Kim")==0){
            for (int j=i;j<memberCount-1;j++){
                strcpy(name[j], name[j+1]);
                age[j] = age[j+1];
                strcpy(membershipType[j], membershipType[j+1]);
                strcpy(registrationDate[j], registrationDate[j+1]);
            }
            memberCount--;
            break;
        }
    }
    CHECK_EQ_INT(memberCount, 1);
    CHECK_STREQ(name[0], "Chris Kim");
        // สรุปผล
    if (t_fail==0) printf("\nAll unit tests passed! (%d checks)\n", t_run);
    else printf("\nUnit tests FAILED: %d/%d checks failed.\n", t_fail, t_run);

    // reset counters for re-run
    t_run = t_fail = 0;
}

static void loadMembersFromFile(const char* filename){
    FILE *file = fopen(filename, "r");
    if (!file) { memberCount = 0; return; }

    memberCount = 0;
    char line[200];
    if (!fgets(line, sizeof(line), file)) { fclose(file); return; }

    if (strstr(line, "MemberName") == NULL) {
        sscanf(line, "%49[^,],%d,%19[^,],%19[^\n]",
               name[memberCount], &age[memberCount],
               membershipType[memberCount], registrationDate[memberCount]);
        memberCount++;
    }
    while (fgets(line, sizeof(line), file)) {
        if (memberCount >= MAX_MEMBERS) break;
        sscanf(line, "%49[^,],%d,%19[^,],%19[^\n]",
               name[memberCount], &age[memberCount],
               membershipType[memberCount], registrationDate[memberCount]);
        memberCount++;
    }
    fclose(file);
}
static void saveMembersToFile(const char* filename){
    FILE *file = fopen(filename, "w");
    if (!file) return;
    fprintf(file, "MemberName,Age,MembershipType,RegistrationDate\n");
    for (int i=0;i<memberCount;i++){
        fprintf(file, "%s,%d,%s,%s\n",
                name[i], age[i], membershipType[i], registrationDate[i]);
    }
    fclose(file);
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
    printf("----------------\n");
    printf("enter choice: ");
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
    case 7:
    runUnitTests();
    break;
    case 0:
        printf("exiting without saveing...\n");
        exit(0);
     default:
        printf("Invalid choice. Try again.\n");
        break;
    }
} while (1);
}