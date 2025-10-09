#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "members.h"
#include "platform.h"

char name[MAX_MEMBERS][50];
int  age[MAX_MEMBERS];
char membershipType[MAX_MEMBERS][20];
char registrationDate[MAX_MEMBERS][20];
int  memberCount = 0;

void clearScreen(void) 
{
    system(CLEAR_SCREEN);
}

void toLowerCase(char *s) 
{
    for (; *s; ++s) *s = (char)tolower((unsigned char)*s);
}

int isValidNameOrType(const char *s) 
{
    if (!s || !*s) return 0;
    for (int i = 0; s[i] != '\0'; ++i) 
    {
        if (!isalpha((unsigned char)s[i]) && !isspace((unsigned char)s[i])) return 0;
    }
    return 1;
}

/* storage helpers exposed for tests */
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
    for (int i = 0; i < memberCount; ++i) 
    {
        if (STRCASECMP(name[i], n) == 0) return i;
    }
    return -1;
}

int deleteByNameCI(const char* n) 
{
    if (!n || memberCount <= 0) return 0;
    for (int i = 0; i < memberCount; ++i) 
    {
        if (STRCASECMP(name[i], n) == 0) 
        {
            for (int j = i; j < memberCount - 1; ++j) 
            {
                strcpy(name[j], name[j + 1]);
                age[j] = age[j + 1];
                strcpy(membershipType[j], membershipType[j + 1]);
                strcpy(registrationDate[j], registrationDate[j + 1]);
            }
            memberCount--;
            return 1;
        }
    }
    return 0;
}

/* CSV IO */
static int parseRow(char* line, char* outName, int* outAge, char* outType, char* outReg) 
{
    char nbuf[50], tbuf[20], dbuf[20];
    int a;
    int read = sscanf(line, " %49[^,],%d,%19[^,],%19[^\n]", nbuf, &a, tbuf, dbuf);
    if (read == 4) 
    {
        strncpy(outName, nbuf, 49); outName[49] = '\0';
        *outAge = a;
        strncpy(outType, tbuf, 19); outType[19] = '\0';
        strncpy(outReg, dbuf, 19); outReg[19] = '\0';
        return 1;
    }
    return 0;
}

void loadMembersFromFile(const char* filename) 
{
    FILE* file = fopen(filename, "r");
    memberCount = 0;
    if (!file) return;

    char line[256];
    if (!fgets(line, sizeof(line), file)) { fclose(file); return; }

    if (strstr(line, "MemberName") == NULL) 
    {
        if (parseRow(line, name[memberCount], &age[memberCount],
                     membershipType[memberCount], registrationDate[memberCount])) {
            memberCount++;
        }
    }

    while (fgets(line, sizeof(line), file)) {
        if (memberCount >= MAX_MEMBERS) break;
        if (parseRow(line, name[memberCount], &age[memberCount],
                     membershipType[memberCount], registrationDate[memberCount])) {
            memberCount++;
        } else 
        {
            /* skip malformed row */
        }
    }
    fclose(file);
}

void saveMembersToFile(const char* filename) 
{
    FILE* file = fopen(filename, "w");
    if (!file) 
    {
        printf("Error: cannot open file to save.\n");
        return;
    }
    fprintf(file, "MemberName,Age,MembershipType,RegistrationDate\n");
    for (int i = 0; i < memberCount; ++i) 
    {
        fprintf(file, "%s,%d,%s,%s\n",
                name[i], age[i], membershipType[i], registrationDate[i]);
    }
    fclose(file);
}

/* public wrappers */
void loadMembers(void)  { loadMembersFromFile(FILENAME); }
void saveMembers(void)  { saveMembersToFile(FILENAME);   }

/* ---------- core features ---------- */
void displayMembers(void) 
{
    if (memberCount == 0) 
    {
        printf("No members available.\n");
        return;
    }
    printf("\n%-20s %-5s %-15s %-12s\n", "Name", "Age", "Membership", "Registration");
    printf("-----------------------------------------------------------\n");
    for (int i = 0; i < memberCount; ++i) 
    {
        printf("%-20s %-5d %-15s %-12s\n",
               name[i], age[i], membershipType[i], registrationDate[i]);
    }
}

void addMember(void) 
{
    if (memberCount >= MAX_MEMBERS) 
    {
        printf("Member list is full!\n");
        return;
    }

    char nn[50], tt[20], dd[20];
    int aa;

    do {
        printf("Enter name (letters only): ");
        if (scanf(" %49[^\n]", nn) != 1) { printf("Input error.\n"); return; }
        if (!isValidNameOrType(nn)) printf("Invalid input! Name must contain only letters and spaces.\n");
    } while (!isValidNameOrType(nn));
    toLowerCase(nn);

    printf("Enter age: ");
    if (scanf("%d", &aa) != 1) { printf("Input error.\n"); return; }

    do 
    {
        printf("Enter membership type (Gold/Silver): ");
        if (scanf(" %19s", tt) != 1) { printf("Input error.\n"); return; }
        if (!isValidNameOrType(tt)) printf("Invalid input! Type must contain only letters.\n");
    } 
    while (!isValidNameOrType(tt));
    toLowerCase(tt);

    printf("Enter registration date (YYYY-MM-DD): ");
    if (scanf(" %19s", dd) != 1) { printf("Input error.\n"); return; }

    addMemberDirect(nn, aa, tt, dd);
    saveMembers();
    printf("Member added successfully!\n");
}
/* --------- helpers for flexible, case-insensitive search --------- */
static void str_tolower_copy(char *dst, const char *src, size_t dstsz) {
    if (dstsz == 0) return;
    size_t i = 0;
    for (; src[i] && i + 1 < dstsz; ++i) {
        unsigned char c = (unsigned char)src[i];
        dst[i] = (char)tolower(c);
    }
    dst[i] = '\0';
}

/* return 1 if 'sub' occurs anywhere inside 's' (case-insensitive), else 0 */
static int icontains(const char *s, const char *sub) {
    char hs[128], nd[128];
    str_tolower_copy(hs, s, sizeof(hs));
    str_tolower_copy(nd, sub, sizeof(nd));
    return strstr(hs, nd) != NULL;
}

/* return 1 if key is a prefix of the FIRST token (first name), case-insensitive */
static int firstNamePrefixMatch(const char *fullName, const char *key) {
    char buf[64], k[64];
    str_tolower_copy(buf, fullName, sizeof(buf));
    str_tolower_copy(k,   key,      sizeof(k));
    /* ตัดเอาเฉพาะคำแรก (ก่อนเว้นวรรค) */
    char *sp = strchr(buf, ' ');
    if (sp) *sp = '\0';
    return strncmp(buf, k, strlen(k)) == 0;
}

void searchMember(void) 
{
    char keyword[50];
    do 
    {
        printf("Enter name or membership type (letters only): ");
        if (scanf(" %49[^\n]", keyword) != 1) { printf("Input error.\n"); return; }
        if (!isValidNameOrType(keyword)) printf("Invalid input! Please use only letters and spaces.\n");
    } 
    while (!isValidNameOrType(keyword));
    toLowerCase(keyword);

    int found = 0;
    for (int i = 0; i < memberCount; ++i) 
    {
        if (STRCASECMP(name[i], keyword) == 0 ||
            STRCASECMP(membershipType[i], keyword) == 0) 
            {
            printf("Found: %s, %d, %s, %s\n",
                   name[i], age[i], membershipType[i], registrationDate[i]);
            found = 1;
        }
    }
    if (!found) printf("No member found.\n");
}

    void searchMember(void) 
{
    char keyword[50];
    do 
    {
        printf("Enter name or membership type (letters only): ");
        if (scanf(" %49[^\n]", keyword) != 1) 
        { 
            printf("Input error.\n"); 
            return; 
        }
        if (!isValidNameOrType(keyword)) 
            printf("Invalid input! Please use only letters and spaces.\n");
    } 
    while (!isValidNameOrType(keyword));

    int found = 0;
    for (int i = 0; i < memberCount; ++i) 
    {
        
        if (firstNamePrefixMatch(name[i], keyword) ||
            icontains(name[i], keyword) ||
            STRCASECMP(membershipType[i], keyword) == 0) 
        {
            printf("Found: %s, %d, %s, %s\n",
                   name[i], age[i], membershipType[i], registrationDate[i]);
            found = 1;
        }
    }
    if (!found) printf("No member found.\n");
}


void updateMember(void) 
{
    char keyword[50];
    printf("Enter member name to update: ");
    if (scanf(" %49[^\n]", keyword) != 1) { printf("Input error.\n"); return; }

    for (int i = 0; i < memberCount; ++i) 
    {
        if (STRCASECMP(name[i], keyword) == 0) 
        {
            printf("Enter new membership type: ");
            if (scanf(" %19s", membershipType[i]) != 1) { printf("Input error.\n"); return; }
            saveMembers();
            printf("Member updated!\n");
            return;
        }
    }
    printf("Member not found.\n");
}

void deleteMember(void) 
{
    char keyword[50];
    printf("Enter member name to delete: ");
    if (scanf(" %49[^\n]", keyword) != 1) { printf("Input error.\n"); return; }

    if (deleteByNameCI(keyword)) 
    {
        saveMembers();
        printf("Member deleted!\n");
    } else 
    {
        printf("Member not found.\n");
    }
}
