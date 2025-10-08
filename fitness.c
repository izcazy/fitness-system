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

void clearScreen() 
{
    system(CLEAR_SCREEN);  
}
#include <ctype.h>  

void toLowerCase(char *s) 
{
    for (; *s; s++) {
        *s = tolower((unsigned char)*s);
    }
}

int deleteByNameCI(const char* n);

/* ตรวจสอบว่า string มีแต่ตัวอักษรกับช่องว่างเท่านั้น */
int isValidNameOrType(const char *s) 
{
    for (int i = 0; s[i] != '\0'; i++) 
    {
        if (!isalpha((unsigned char)s[i]) && !isspace((unsigned char)s[i])) {
            return 0;  // พบอักขระแปลก
        }
    }
    return 1;
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

static void loadMembersFromFile(const char* filename);
static void saveMembersToFile(const char* filename);
/* E2E Test */
void runE2EAllErrors_Pretty(void);
static void resetStoreE2E2(void);
static int  addMemberE2E2(const char* n,int a,const char* t,const char* d);
static void loadMembersE2E2(const char* filename);
static void saveMembersE2E2(const char* filename);
static int  deleteByNameCIE2E2(const char* n);
static void writeTextFileE2E2(const char* filename, const char* content);



void loadFromCSV() 
{
    FILE *fp = fopen("member.csv", "r");
    if (!fp) 
    {
        printf("Cannot open member.csv\n");
        return;
    }
    fclose(fp);
}

void loadMembers() 
{
    FILE *file = fopen(FILENAME, "r");
    if (!file) 
    {
        printf("No existing data found. A new file will be created.\n");
        return;
    }
    memberCount = 0;
    char line[200];

    
    if (!fgets(line, sizeof(line), file)) 
    { fclose(file); return; }

    if (strstr(line, "MemberName") == NULL) 
    {
        
        sscanf(line, "%49[^,],%d,%19[^,],%19[^\n]",
               name[memberCount], &age[memberCount],
               membershipType[memberCount], registrationDate[memberCount]);
        memberCount++;
    }

    while (fgets(line, sizeof(line), file)) 
    {
        if (memberCount >= MAX_MEMBERS) break;
        sscanf(line, "%49[^,],%d,%19[^,],%19[^\n]",
               name[memberCount], &age[memberCount],
               membershipType[memberCount], registrationDate[memberCount]);
        memberCount++;
    }
    fclose(file);
}

void saveMembers() 
{
    FILE *file = fopen(FILENAME, "w");
    if (!file) 
    {
        printf("Error: cannot open file to save. \n");
        return;
    }

    fprintf(file, "MemberName,Age,MembershipType,RegistrationDate\n");
    for (int i = 0; i < memberCount; i++) 
    {
        fprintf(file, "%s,%d,%s,%s\n",
                name[i], age[i], membershipType[i], registrationDate[i]);
    }
    fclose(file);
}

void addMember() 
{
    if (memberCount >= MAX_MEMBERS) 
    {
        printf("Member list is full!\n");
        return;
    }

    do
    {
        printf("Enter name (letters only):");
        scanf(" %[^\n]", name[memberCount]);
        if (!isValidNameOrType(name[memberCount]))
        {
            printf("Invalid input! Name must contain only letters and spaces.\n");
        }
    }
    while (!isValidNameOrType(name[memberCount]));
    toLowerCase(name[memberCount]);

    printf("Enter age: ");
    scanf("%d", &age[memberCount]);

    do {
        printf("Enter membership type (Gold/Silver): ");
        scanf(" %19s", membershipType[memberCount]);
        if (!isValidNameOrType(membershipType[memberCount])) {
            printf("Invalid input! Type must contain only letters.\n");
        }
    } while (!isValidNameOrType(membershipType[memberCount]));

    toLowerCase(membershipType[memberCount]);

    printf("Enter registration date (YYYY-MM-DD): ");
    scanf(" %19s", registrationDate[memberCount]);

    memberCount++;
    saveMembers();
    printf("Member added successfully!\n");


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

void displayMembers() 
{
    if (memberCount == 0) 
    {
        printf("No members available.\n");
        return;
    }

    printf("\n%-20s %-5s %-15s %-12s\n", "Name", "Age", "Membership", "Registration");
    printf("-----------------------------------------------------------\n");
    for (int i = 0; i < memberCount; i++) 
    {
        printf("%-20s %-5d %-15s %-12s\n",
               name[i], age[i], membershipType[i], registrationDate[i]);
    }
}

void searchMember() 
{
    char keyword[50];
    do 
    {
        printf("Enter name or membership type (letters only): ");
        scanf(" %[^\n]", keyword);
        if (!isValidNameOrType(keyword)) 
        {
            printf("Invalid input! Please use only letters and spaces.\n");
        }
    } 
    while (!isValidNameOrType(keyword));
    toLowerCase(keyword);


     int found = 0;
    for (int i = 0; i < memberCount; i++) 
    {
        if (strcasecmp(name[i], keyword) == 0 ||
            strcasecmp(membershipType[i], keyword) == 0) 
            {
            printf("Found: %s, %d, %s, %s\n",
                   name[i], age[i], membershipType[i], registrationDate[i]);
            found = 1;
        }
    }
    if (!found) printf("No member found.\n");
}


void updateMember() 
{
    char keyword[50];
    printf("Enter member name to update: ");
    scanf(" %[^\n]", keyword);

    for (int i = 0; i < memberCount; i++) 
    {
        if (strcasecmp(name[i], keyword) == 0) 
        {
            printf("Enter new membership type: ");
            scanf(" %s", membershipType[i]);
            saveMembers();
            printf("Member updated!\n");
            return;
        }
    }

    printf("Member not found.\n");
}

void deleteMember() 
{
    char keyword[50];
    printf("Enter member name to delete: ");
    scanf(" %[^\n]", keyword);

    for (int i = 0; i < memberCount; i++) 
    {
        if (strcasecmp(name[i], keyword) == 0) 
        {
            for (int j = i; j < memberCount - 1; j++) 
            {
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

int addMemberDirect(const char* n, int a, const char* t, const char* d)
{
    if (!n || !t || !d) return -1;
    if (memberCount >= MAX_MEMBERS) return -1;
    strncpy(name[memberCount], n, 49); name[memberCount][49] = '\0';
    age[memberCount] = a;
    strncpy(membershipType[memberCount], t, 19); membershipType[memberCount][19] = '\0';
    strncpy(registrationDate[memberCount], d, 19); registrationDate[memberCount][19] = '\0';
    memberCount++;
    return memberCount;
}
int findByNameCI(const char* n) 
{
    if (!n) return -1;
    for (int i = 0; i < memberCount; i++)
        if (STRCASECMP(name[i], n) == 0) return i;
    return -1;
}

int deleteByNameCI(const char* n) 
{
    if (!n || memberCount <= 0) return 0;
    for (int i = 0; i < memberCount; i++) 
    {
        if (STRCASECMP(name[i], n) == 0) {
            for (int j = i; j < memberCount - 1; j++) 
            {
                strcpy(name[j], name[j+1]);
                age[j] = age[j+1];
                strcpy(membershipType[j], membershipType[j+1]);
                strcpy(registrationDate[j], registrationDate[j+1]);
            }
            memberCount--;
            return 1; /* ลบสำเร็จ (ลบตัวแรกที่เจอเท่านั้น) */
        }
    }
    return 0; /* ไม่พบ */
}

// Unit Test: Add + Search เท่านั้น 
static int t_run=0, t_fail=0;
#define CHECK(c) do{ t_run++; if(!(c)){ t_fail++; \
  fprintf(stderr,"[FAIL] %s:%d -> %s\n", __FILE__, __LINE__, #c); } }while(0)
#define CHECK_EQ_INT(a,b) CHECK((a)==(b))
#define CHECK_STREQ(a,b)  CHECK(strcmp((a),(b))==0)

/* helper: ล้างคลังข้อมูลในหน่วยความจำ */
static void resetStore(void){
    for (int i=0;i<memberCount;i++)
    {
        name[i][0] = '\0';
        membershipType[i][0] = '\0';
        registrationDate[i][0] = '\0';
        age[i] = 0;
    }
    memberCount = 0;
}

/* helper: เติมข้อมูลแบบปลอดภัย (แปลงเป็นตัวเล็กเพื่อจำลอง flow จริง) */
static int addLowered(const char* n, int a, const char* t, const char* d)
{
    char nn[50], tt[20], dd[20];
    if (!n||!t||!d) return -1;
    strncpy(nn, n, sizeof(nn)-1); nn[sizeof(nn)-1]='\0'; toLowerCase(nn);
    strncpy(tt, t, sizeof(tt)-1); tt[sizeof(tt)-1]='\0'; toLowerCase(tt);
    strncpy(dd, d, sizeof(dd)-1); dd[sizeof(dd)-1]='\0';
    return addMemberDirect(nn, a, tt, dd);
}

void runUnitTests(void)
{
    resetStore();
    t_run = t_fail = 0;

    printf("\n===== Running Unit Tests: Search + Delete =====\n");

    /* เตรียมข้อมูล: john (idx0), jane (idx1), JOHN ซ้ำ (idx2) */
    CHECK(addLowered("John Doe",   25, "Gold",   "2025-01-05") > 0); // 0
    CHECK(addLowered("Jane Smith", 30, "Silver", "2025-02-10") > 0); // 1
    CHECK(addLowered("JOHN DOE",   28, "Gold",   "2025-03-01") > 0); // 2

    /* ---- SEARCH ---- */
    CHECK(findByNameCI("john doe") == 0);    /* case-insensitive */
    CHECK(findByNameCI("JOHN DOE") == 0);    /* ชื่อซ้ำ → ต้องคืนตัวแรกเสมอ */
    CHECK(findByNameCI("jane smith") == 1);
    CHECK(findByNameCI("nobody") == -1);     /* ไม่พบ */

    /* ---- DELETE (ลบชื่อซ้ำ: ต้องลบตัวแรกเท่านั้น) ---- */
    int before = memberCount;
    CHECK(deleteByNameCI("JoHn DoE") == 1);  /* ลบ john ตัวแรก (idx0) */
    CHECK(memberCount == before - 1);

    /* หลังลบ: jane ควรเลื่อนมาเป็น idx0 และ JOHN เดิมเลื่อนเป็น idx1 */
    CHECK(STRCASECMP(name[0], "jane smith") == 0);
    CHECK(STRCASECMP(name[1], "john doe")   == 0);

    /* ลบ JOHN ที่เหลืออยู่ */
    before = memberCount;
    CHECK(deleteByNameCI("john doe") == 1);
    CHECK(memberCount == before - 1);
    CHECK(findByNameCI("john doe") == -1);   /* ไม่มี john เหลือแล้ว */
    CHECK(STRCASECMP(name[0], "jane smith") == 0);

    /* ลบชื่อที่ไม่มีในระบบ → ไม่ควรเปลี่ยนจำนวน */
    before = memberCount;
    CHECK(deleteByNameCI("not-exist") == 0);
    CHECK(memberCount == before);

    /* ลบจนคลังว่าง แล้วลองลบอีกครั้ง → ต้องคืน 0 */
    CHECK(deleteByNameCI("jane smith") == 1);
    CHECK(memberCount == 0);
    CHECK(deleteByNameCI("any") == 0);
    CHECK(memberCount == 0);

    /* สรุปผล */
    if (t_fail == 0)
        printf("Search+Delete unit tests passed! (%d checks)\n", t_run);
    else
        printf("Search+Delete unit tests FAILED: %d/%d failed.\n", t_fail, t_run);

    /* พร้อมรันใหม่ */
    t_run = t_fail = 0;
    resetStore();
}

/*Helpers*/
static void resetStoreE2E2(void)
{
    for (int i=0; i<memberCount; i++)
    {
        name[i][0] = '\0';
        membershipType[i][0] = '\0';
        registrationDate[i][0] = '\0';
        age[i] = 0;
    }
    memberCount = 0;
}

static int addMemberE2E2(const char* n,int a,const char* t,const char* d)
{
    char nn[50], tt[20], dd[20];
    if (!n || !t || !d) return -1;
    strncpy(nn, n, sizeof(nn)-1); nn[sizeof(nn)-1]='\0'; toLowerCase(nn);
    strncpy(tt, t, sizeof(tt)-1); tt[sizeof(tt)-1]='\0'; toLowerCase(tt);
    strncpy(dd, d, sizeof(dd)-1); dd[sizeof(dd)-1]='\0';
    return addMemberDirect(nn, a, tt, dd);
}

static void loadMembersE2E2(const char* filename)
{
    FILE *file = fopen(filename, "r");
    memberCount = 0;
    if (!file) return;
    char line[200];
    if (!fgets(line, sizeof(line), file)) { fclose(file); return; }
    if (strstr(line, "MemberName") == NULL){
        sscanf(line, "%49[^,],%d,%19[^,],%19[^\n]",
               name[memberCount], &age[memberCount],
               membershipType[memberCount], registrationDate[memberCount]);
        memberCount++;
    }
    while (fgets(line, sizeof(line), file))
    {
        if (memberCount >= MAX_MEMBERS) break;
        sscanf(line, "%49[^,],%d,%19[^,],%19[^\n]",
               name[memberCount], &age[memberCount],
               membershipType[memberCount], registrationDate[memberCount]);
        memberCount++;
    }
    fclose(file);
}

static void saveMembersE2E2(const char* filename)
{
    FILE *file = fopen(filename, "w");
    if (!file) return;
    fprintf(file, "MemberName,Age,MembershipType,RegistrationDate\n");
    for (int i=0; i<memberCount; i++){
        fprintf(file, "%s,%d,%s,%s\n",
                name[i], age[i], membershipType[i], registrationDate[i]);
    }
    fclose(file);
}

static int deleteByNameCIE2E2(const char* n)
{
    if (!n || memberCount <= 0) return 0;
    for (int i=0; i<memberCount; i++)
    {
        if (STRCASECMP(name[i], n) == 0)
        {
            for (int j=i; j<memberCount-1; j++)
            {
                strcpy(name[j], name[j+1]);
                age[j] = age[j+1];
                strcpy(membershipType[j], membershipType[j+1]);
                strcpy(registrationDate[j], registrationDate[j+1]);
            }
            memberCount--;
            return 1;
        }
    }
    return 0;
}

static void writeTextFileE2E2(const char* filename, const char* content)
{
    FILE *f = fopen(filename, "w");
    if (!f) return;
    if (content) fputs(content, f);
    fclose(f);
}

/*Pretty section reporter*/
static int printSectionResult(const char* title, int startRun, int startFail)
{
    int dRun  = t_run  - startRun;
    int dFail = t_fail - startFail;
    if (dFail == 0)
    {
        printf("%s: PASS (%d checks)\n", title, dRun);
    } else 
    {
        printf("%s: FAIL (%d checks, %d failed)\n", title, dRun, dFail);
    }
    return dFail;
}

/*Sections (use CHECK directly)*/
static void sec_missing_empty_header_E2E2(const char* TFILE)
{
    /* missing file */
    remove(TFILE);
    resetStoreE2E2();
    loadMembersE2E2(TFILE);
    CHECK(memberCount==0);

    /* empty file */
    writeTextFileE2E2(TFILE, "");
    resetStoreE2E2(); loadMembersE2E2(TFILE);
    CHECK(memberCount==0);

    /* header only */
    writeTextFileE2E2(TFILE, "MemberName,Age,MembershipType,RegistrationDate\n");
    resetStoreE2E2(); loadMembersE2E2(TFILE);
    CHECK(memberCount==0);
}

static void sec_add_save_load_basic_E2E2(const char* TFILE)
{
    resetStoreE2E2();
    CHECK(addMemberE2E2("John Doe",25,"Gold","2025-01-05")>0);
    CHECK(addMemberE2E2("Jane Smith",30,"Silver","2025-02-10")>0);
    saveMembersE2E2(TFILE);

    resetStoreE2E2(); loadMembersE2E2(TFILE);
    CHECK(memberCount==2);
    CHECK(STRCASECMP(name[0],"john doe")==0);
    CHECK(STRCASECMP(membershipType[0],"gold")==0);
    CHECK(STRCASECMP(name[1],"jane smith")==0);
    CHECK(STRCASECMP(membershipType[1],"silver")==0);
}

static void sec_duplicate_delete_first_E2E2(const char* TFILE)
{
    CHECK(addMemberE2E2("JOHN DOE",28,"GOLD","2025-03-01")>0);
    saveMembersE2E2(TFILE);

    resetStoreE2E2(); loadMembersE2E2(TFILE);
    CHECK(findByNameCI("john doe")==0);

    int before = memberCount;
    CHECK(deleteByNameCIE2E2("JoHn DoE")==1); // ลบตัวแรกเท่านั้น 
    CHECK(memberCount==before-1);
    CHECK(findByNameCI("john doe")>=0); //ตัวซ้ำยังเหลือ

    //ลบตัวที่เหลือ
    CHECK(deleteByNameCIE2E2("john doe")==1);
    CHECK(findByNameCI("john doe")==-1);
}

static void sec_long_fields_trunc_E2E2(const char* TFILE)
{
    char longName[200], longType[200];
    memset(longName,'a',sizeof(longName)); longName[sizeof(longName)-1]='\0';
    memset(longType,'b',sizeof(longType)); longType[sizeof(longType)-1]='\0';
    CHECK(addMemberE2E2(longName,22,longType,"2025-04-04")>0);
    saveMembersE2E2(TFILE);

    resetStoreE2E2(); loadMembersE2E2(TFILE);
    CHECK(name[memberCount-1][49]=='\0');
    CHECK(membershipType[memberCount-1][19]=='\0');
}

static void sec_capacity_full_E2E2(const char* TFILE)
{
    while(memberCount<MAX_MEMBERS)
    {
        char t[32]; snprintf(t,sizeof(t),"x%03d",memberCount);
        CHECK(addMemberE2E2(t,20,"gold","2025-05-05")>0 || memberCount==MAX_MEMBERS);
    }
    int capBefore = memberCount;
    CHECK(addMemberE2E2("overflow",20,"gold","2025-06-06")==-1);
    CHECK(memberCount==capBefore);
    saveMembersE2E2(TFILE);

    resetStoreE2E2(); loadMembersE2E2(TFILE);
    CHECK(memberCount==capBefore);
}

static void sec_malformed_rows_E2E2(const char* TFILE)
{
    writeTextFileE2E2
    (TFILE,
        "MemberName,Age,MembershipType,RegistrationDate\n"
        "badline-without-commas\n"
        "only,two,columns\n"
        "good guy,21,gold,2025-07-07\n"
    );
    resetStoreE2E2(); loadMembersE2E2(TFILE);
    int ok=0;
    for(int i=0;i<memberCount;i++)
    {
        if(STRCASECMP(name[i],"good guy")==0 &&
           STRCASECMP(membershipType[i],"gold")==0){ ok=1; break; }
    }
    CHECK(ok==1);
}

static void sec_update_persist_E2E2(const char* TFILE)
{
    int idx = findByNameCI("good guy");
    CHECK(idx>=0);
    strcpy(membershipType[idx],"silver");
    saveMembersE2E2(TFILE);

    resetStoreE2E2(); loadMembersE2E2(TFILE);
    idx = findByNameCI("good guy");
    CHECK(idx>=0 && STRCASECMP(membershipType[idx],"silver")==0);
}

static void sec_delete_not_exist_empty_E2E2(const char* TFILE)
{
    (void)TFILE;
    CHECK(deleteByNameCIE2E2("nobody")==0);
    while(memberCount>0){ CHECK(deleteByNameCIE2E2(name[0])==1);}
    CHECK(memberCount==0);
    CHECK(deleteByNameCIE2E2("any")==0);
}

/* ---------- Runner (pretty) ---------- */
void runE2EAllErrors_Pretty(void){
    const char* TFILE = "e2e_all_errors.csv";
    int totalFailed = 0;

    printf("\n===== E2E (all error paths) =====\n");
    remove(TFILE);

    int sr, sf;

    sr=t_run; sf=t_fail; sec_missing_empty_header_E2E2(TFILE);
    totalFailed += printSectionResult("Missing/Empty/Header-only", sr, sf);

    sr=t_run; sf=t_fail; sec_add_save_load_basic_E2E2(TFILE);
    totalFailed += printSectionResult("Add+Save+Load (basic)", sr, sf);

    sr=t_run; sf=t_fail; sec_duplicate_delete_first_E2E2(TFILE);
    totalFailed += printSectionResult("Duplicate + Delete-First", sr, sf);

    sr=t_run; sf=t_fail; sec_long_fields_trunc_E2E2(TFILE);
    totalFailed += printSectionResult("Long Fields (Trunc/Null-Term)", sr, sf);

    sr=t_run; sf=t_fail; sec_capacity_full_E2E2(TFILE);
    totalFailed += printSectionResult("Capacity Full", sr, sf);

    sr=t_run; sf=t_fail; sec_malformed_rows_E2E2(TFILE);
    totalFailed += printSectionResult("Malformed Rows (robust load)", sr, sf);

    sr=t_run; sf=t_fail; sec_update_persist_E2E2(TFILE);
    totalFailed += printSectionResult("Update + Persist", sr, sf);

    sr=t_run; sf=t_fail; sec_delete_not_exist_empty_E2E2(TFILE);
    totalFailed += printSectionResult("Delete not-exist & on empty", sr, sf);

    printf("---------------------------------\n");
    if (totalFailed==0)
    {
        printf("E2E Summary: PASS (total checks: %d)\n", t_run);
    } else 
    {
        printf("E2E Summary: FAILED sections=%d (failed checks: %d / %d)\n",
               totalFailed, t_fail, t_run);
    }

    remove(TFILE);
    resetStoreE2E2();
    t_run = t_fail = 0;
}

// main
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
    printf("8. E2E Test\n");
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
    case 8:
    runE2EAllErrors_Pretty();
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