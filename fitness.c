#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MEMBERS 100
#define FILENAME "member.csv"

#if defined(_MSC_VER) || defined(_WIN32)
  #define STRCASECMP _stricmp
#else
  #include <strings.h>  
  #define STRCASECMP strcasecmp
#endif

#ifdef _WIN32
  #define CLEAR_SCREEN "cls"
#else
  #define CLEAR_SCREEN "clear"
#endif

void clearScreen() {
    system(CLEAR_SCREEN);  
}
#include <ctype.h>  

void toLowerCase(char *s) {
    for (; *s; s++) {
        *s = tolower((unsigned char)*s);
    }
}



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

    printf("Enter name (any case will be converted to lowercase): ");
    scanf(" %[^\n]", name[memberCount]);
    toLowerCase(name[memberCount]);  

    printf("Enter age: ");
    scanf("%d", &age[memberCount]);

    printf("Enter membership type (Gold/Silver, converted to lowercase): ");
    scanf(" %19s", membershipType[memberCount]);
    toLowerCase(membershipType[memberCount]);  

    printf("Enter registration date (YYYY-MM-DD): ");
    scanf(" %19s", registrationDate[memberCount]);

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

int addMemberDirect(const char* n, int a, const char* t, const char* d) {
    if (!n || !t || !d) return -1;
    if (memberCount >= MAX_MEMBERS) return -1;
    strncpy(name[memberCount], n, 49); name[memberCount][49] = '\0';
    age[memberCount] = a;
    strncpy(membershipType[memberCount], t, 19); membershipType[memberCount][19] = '\0';
    strncpy(registrationDate[memberCount], d, 19); registrationDate[memberCount][19] = '\0';
    memberCount++;
    return memberCount;
}
int findByNameCI(const char* n) {
    if (!n) return -1;
    for (int i = 0; i < memberCount; i++)
        if (STRCASECMP(name[i], n) == 0) return i;
    return -1;
}

/* ================== Unit Test: Add + Search เท่านั้น ================== */
static int t_run=0, t_fail=0;
#define CHECK(c) do{ t_run++; if(!(c)){ t_fail++; \
  fprintf(stderr,"[FAIL] %s:%d -> %s\n", __FILE__, __LINE__, #c); } }while(0)
#define CHECK_EQ_INT(a,b) CHECK((a)==(b))
#define CHECK_STREQ(a,b)  CHECK(strcmp((a),(b))==0)

void runUnitTests() {
    /* ทำในหน่วยความจำล้วน ๆ ไม่แตะไฟล์ */
    memberCount = 0;

    /* เตรียมข้อมูลตั้งต้น 2 รายการ */
    CHECK(addMemberDirect("John Doe", 25, "Gold",   "2025-01-05") > 0);
    CHECK(addMemberDirect("Jane Smith", 30, "Silver","2025-02-10") > 0);

    /* ---- TEST #1: ADD ---- */
    int before = memberCount;
    int rc = addMemberDirect("Mary Lee", 29, "Gold", "2025-03-01");
    CHECK(rc == before + 1);
    int idxMary = findByNameCI("mary lee");   /* case-insensitive */
    CHECK(idxMary >= 0);
    CHECK_STREQ(name[idxMary], "Mary Lee");
    CHECK_EQ_INT(age[idxMary], 29);
    CHECK_STREQ(membershipType[idxMary], "Gold");
    CHECK_STREQ(registrationDate[idxMary], "2025-03-01");

    /* ---- TEST #2: SEARCH ---- */
    /* 2.1 พบชื่อ (ไม่สนพิมพ์เล็กใหญ่) */
    CHECK(findByNameCI("JOHN DOE") == 0);
    /* 2.2 ไม่พบชื่อ */
    CHECK(findByNameCI("Nobody") == -1);
    /* 2.3 ค้นหาตาม membership แบบที่ searchMember ทำ */
    int foundSilver = 0;
    for (int i=0;i<memberCount;i++)
        if (STRCASECMP(membershipType[i], "silver")==0) { foundSilver=1; break; }
    CHECK(foundSilver == 1);

    if (t_fail==0) printf("\nAll unit tests (Add + Search) passed! (%d checks)\n", t_run);
    else printf("\nUnit tests FAILED: %d/%d checks failed.\n", t_fail, t_run);

    /* รีเซ็ตตัวนับเผื่อผู้ใช้กดเทสต์ซ้ำ */
    t_run = t_fail = 0;
}

int main() 
{
    loadMembers();
    menu();
    return 0;
}



void menu(){
    int choice; 

do{
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
    printf("----------------\n");
    printf("enter choice: ");
    if(scanf("%d",&choice)!=1)
    {
        int c; while ((c=getchar())!='\n' && c!=EOF){}
        choice = -1;
        
    }

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

    printf("\nPress Enter to continue...");
        getchar(); getchar();
    } while (1);
}