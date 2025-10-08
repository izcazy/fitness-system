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
void runE2ETests(void);


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
int findByNameCI(const char* n) {
    if (!n) return -1;
    for (int i = 0; i < memberCount; i++)
        if (STRCASECMP(name[i], n) == 0) return i;
    return -1;
}


// Unit Test: Add + Search เท่านั้น 
static int t_run=0, t_fail=0;
#define CHECK(c) do{ t_run++; if(!(c)){ t_fail++; \
  fprintf(stderr,"[FAIL] %s:%d -> %s\n", __FILE__, __LINE__, #c); } }while(0)
#define CHECK_EQ_INT(a,b) CHECK((a)==(b))
#define CHECK_STREQ(a,b)  CHECK(strcmp((a),(b))==0)

/* helper: ล้างคลังข้อมูลในหน่วยความจำ */
static void resetStore(void){
    for (int i=0;i<memberCount;i++){
        name[i][0] = '\0';
        membershipType[i][0] = '\0';
        registrationDate[i][0] = '\0';
        age[i] = 0;
    }
    memberCount = 0;
}

/* helper: เติมข้อมูลแบบปลอดภัย (แปลงเป็นตัวเล็กเพื่อจำลอง flow จริง) */
static int addLowered(const char* n, int a, const char* t, const char* d){
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

    /* ---------- 0) Empty dataset behavior ---------- */
    CHECK(findByNameCI("any") == -1);

    /* ---------- 1) addMemberDirect: invalid params ---------- */
    CHECK(addMemberDirect(NULL, 20, "gold", "2025-01-01") == -1);
    CHECK(addMemberDirect("john", 20, NULL, "2025-01-01") == -1);
    CHECK(addMemberDirect("john", 20, "gold", NULL) == -1);
    CHECK(memberCount == 0);

    /* ---------- 2) add + search (normal flow, case-insensitive) ---------- */
    CHECK(addLowered("John Doe",   25, "Gold",   "2025-01-05") > 0);
    CHECK(addLowered("Jane Smith", 30, "Silver", "2025-02-10") > 0);

    /* ค้นหาชื่อ: ต้องเจอแบบไม่สนตัวพิมพ์ */
    CHECK(findByNameCI("john doe") == 0);
    CHECK(findByNameCI("JOHN DOE") == 0);
    CHECK(findByNameCI("Jane Smith") == 1);

    /* ค้นหาตาม membership (จำลอง logic searchMember) */
    int foundGold=0, foundSilver=0;
    for (int i=0;i<memberCount;i++){
        if (STRCASECMP(membershipType[i], "gold")==0)   foundGold=1;
        if (STRCASECMP(membershipType[i], "silver")==0) foundSilver=1;
    }
    CHECK(foundGold==1 && foundSilver==1);

    /* ---------- 3) duplicate names: expect first index ---------- */
    CHECK(addLowered("JOHN DOE", 28, "gold", "2025-03-01") > 0); /* ชื่อซ้ำ */
    int idxFirst = findByNameCI("john doe");
    CHECK(idxFirst == 0); /* ต้องคืนตัวแรกเสมอ */
    CHECK(STRCASECMP(name[idxFirst], "john doe")==0);

    /* ---------- 4) boundary length & truncation ---------- */
    /* สร้างชื่อเกิน 49 ตัว, type เกิน 19 ตัว → addMemberDirect จะตัดให้พอดี buffer */
    char longName[200]; char longType[200];
    memset(longName, 'a', sizeof(longName)); longName[sizeof(longName)-1]='\0';
    memset(longType, 'b', sizeof(longType)); longType[sizeof(longType)-1]='\0';
    int before = memberCount;
    CHECK(addLowered(longName, 22, longType, "2025-04-01") == before+1);
    /* ตรวจว่าปิดสตริงด้วย '\0' และไม่ crash (แค่อ่านปลอดภัย) */
    int last = memberCount-1;
    CHECK(name[last][49] == '\0');            /* การันตี null-terminated */
    CHECK(membershipType[last][19] == '\0');  /* การันตี null-terminated */

    /* ---------- 5) isValidNameOrType should reject weird chars ---------- */
    CHECK(isValidNameOrType("normal name") == 1);
    CHECK(isValidNameOrType("gold") == 1);
    CHECK(isValidNameOrType("silver vip") == 1);
    CHECK(isValidNameOrType("john_doe") == 0);   /* _ ไม่อนุญาต */
    CHECK(isValidNameOrType("john123") == 0);    /* ตัวเลขไม่อนุญาต */
    CHECK(isValidNameOrType("@silver") == 0);    /* สัญลักษณ์ */

    /* ---------- 6) capacity full ---------- */
    /* เติมให้เต็ม แล้วพยายาม add เพิ่ม ต้องล้มเหลวและไม่เพิ่ม memberCount */
    while (memberCount < MAX_MEMBERS){
        char tmp[32]; snprintf(tmp, sizeof(tmp), "x%03d", memberCount);
        CHECK(addLowered(tmp, 20, "gold", "2025-05-01") > 0 || memberCount==MAX_MEMBERS);
    }
    int mcBefore = memberCount;
    CHECK(addLowered("cannot-add", 20, "gold", "2025-06-01") == -1);
    CHECK(memberCount == mcBefore);

    /* ---------- 7) search on not-found ---------- */
    CHECK(findByNameCI("nonexistent") == -1);

    /* ---------- Report ---------- */
    if (t_fail==0) printf("\nAll unit tests (Add + Search: robust) passed! (%d checks)\n", t_run);
    else           printf("\nUnit tests FAILED: %d/%d checks failed.\n", t_fail, t_run);

    /* reset counters for re-run */
    t_run = t_fail = 0;
    resetStore();
}

/* -------- helpers สำหรับโหลด/เซฟจากไฟล์ที่ระบุ (ใช้ใน E2E) -------- */
static void loadMembersFromFile(const char* filename){
    FILE *file = fopen(filename, "r");
    memberCount = 0;
    if (!file) return;

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

/* เคลียร์สโตร์ในหน่วยความจำ */
static void resetStore(void){
    for (int i=0;i<memberCount;i++){
        name[i][0] = '\0';
        membershipType[i][0] = '\0';
        registrationDate[i][0] = '\0';
        age[i] = 0;
    }
    memberCount = 0;
}

/* เติมแบบแปลงเป็นตัวเล็กก่อน เพื่อจำลอง flow ของ addMember() */
static int addLowered(const char* n, int a, const char* t, const char* d){
    char nn[50], tt[20], dd[20];
    if (!n||!t||!d) return -1;
    strncpy(nn, n, sizeof(nn)-1); nn[sizeof(nn)-1]='\0'; toLowerCase(nn);
    strncpy(tt, t, sizeof(tt)-1); tt[sizeof(tt)-1]='\0'; toLowerCase(tt);
    strncpy(dd, d, sizeof(dd)-1); dd[sizeof(dd)-1]='\0';
    return addMemberDirect(nn, a, tt, dd);
}

/* ------------------------- E2E TEST ------------------------- */
void runE2ETests(void){
    const char* TFILE = "e2e_test.csv";
    printf("\n--- Running E2E Test (file: %s) ---\n", TFILE);

    /* 1) เริ่มจากข้อมูลว่าง → เพิ่ม 2 รายการ → เซฟลงไฟล์ */
    resetStore();
    CHECK(addLowered("John Doe", 25, "Gold", "2025-01-05") > 0);
    CHECK(addLowered("Jane Smith", 30, "Silver", "2025-02-10") > 0);
    saveMembersToFile(TFILE);

    /* 2) เคลียร์หน่วยความจำ → โหลดกลับจากไฟล์ → ตรวจว่าตรง */
    resetStore();
    loadMembersFromFile(TFILE);
    CHECK(memberCount == 2);
    CHECK(STRCASECMP(name[0], "john doe")==0);
    CHECK(STRCASECMP(membershipType[0], "gold")==0);
    CHECK(STRCASECMP(name[1], "jane smith")==0);
    CHECK(STRCASECMP(membershipType[1], "silver")==0);

    /* 3) อัปเดตข้อมูล: เปลี่ยน Jane เป็น gold → เซฟ → โหลดใหม่ → ตรวจ */
    int idxJane = findByNameCI("jane smith");
    CHECK(idxJane >= 0);
    strcpy(membershipType[idxJane], "gold");
    saveMembersToFile(TFILE);

    resetStore();
    loadMembersFromFile(TFILE);
    idxJane = findByNameCI("jane smith");
    CHECK(idxJane >= 0);
    CHECK(STRCASECMP(membershipType[idxJane], "gold")==0);

    /* 4) ลบ John → เซฟ → โหลดใหม่ → ตรวจว่าจำนวนลดและ John หายไป */
    int idxJohn = findByNameCI("john doe");
    CHECK(idxJohn >= 0);
    for (int j=idxJohn; j<memberCount-1; j++){
        strcpy(name[j], name[j+1]);
        age[j] = age[j+1];
        strcpy(membershipType[j], membershipType[j+1]);
        strcpy(registrationDate[j], registrationDate[j+1]);
    }
    memberCount--;
    saveMembersToFile(TFILE);

    resetStore();
    loadMembersFromFile(TFILE);
    CHECK(memberCount == 1);
    CHECK(findByNameCI("john doe") == -1);
    CHECK(findByNameCI("jane smith") >= 0);

    /* 5) ค้นหาหลังรีโหลด: ต้องเจอ Jane */
    int found = 0;
    for (int i=0;i<memberCount;i++){
        if (STRCASECMP(name[i], "jane smith")==0) { found=1; break; }
    }
    CHECK(found == 1);

    /* ทำความสะอาดไฟล์ชั่วคราว */
    remove(TFILE);

    if (t_fail==0) printf("E2E test PASSED! (%d checks)\n", t_run);
    else           printf("E2E test FAILED: %d/%d checks failed.\n", t_fail, t_run);

    /* รีเซ็ตตัวนับ เพื่อพร้อมรันใหม่ได้ */
    t_run = t_fail = 0;
    resetStore();
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
    printf("8. E2E Test (file flow)\n");
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
        runE2ETests();
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