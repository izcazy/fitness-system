#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "members.h"
#include "platform.h"

static int t_run=0, t_fail=0;
#define CHECK(c) do{ t_run++; if(!(c)){ t_fail++; \
  fprintf(stderr,"[FAIL] %s:%d -> %s\n", __FILE__, __LINE__, #c); } }while(0)

static void resetStoreE(void)
{
    for (int i=0;i<memberCount;i++)
    {
        name[i][0]='\0'; membershipType[i][0]='\0'; registrationDate[i][0]='\0'; age[i]=0;
    }
    memberCount=0;
}
static int addLoweredE(const char* n,int a,const char* t,const char* d)
{
    char nn[50], tt[20], dd[20];
    if (!n||!t||!d) return -1;
    strncpy(nn,n,sizeof(nn)-1); nn[sizeof(nn)-1]='\0'; toLowerCase(nn);
    strncpy(tt,t,sizeof(tt)-1); tt[sizeof(tt)-1]='\0'; toLowerCase(tt);
    strncpy(dd,d,sizeof(dd)-1); dd[sizeof(dd)-1]='\0';
    return addMemberDirect(nn,a,tt,dd);
}
static void writeTextFile(const char* filename,const char* content)
{
    FILE *f=fopen(filename,"w"); if(!f) return; if(content) fputs(content,f); fclose(f);
}

static int printSectionResult(const char* title, int startRun, int startFail)
{
    int dRun = t_run - startRun;
    int dFail= t_fail- startFail;
    if (dFail==0) printf("%s: PASS (%d checks)\n", title, dRun);
    else          printf("%s: FAIL (%d checks, %d failed)\n", title, dRun, dFail);
    return dFail;
}

/* sections */
static void sec_missing_empty_header(const char* TFILE)
{
    remove(TFILE);
    resetStoreE(); loadMembersFromFile(TFILE);
    CHECK(memberCount==0);

    writeTextFile(TFILE, "");
    resetStoreE(); loadMembersFromFile(TFILE);
    CHECK(memberCount==0);

    writeTextFile(TFILE, "MemberName,Age,MembershipType,RegistrationDate\n");
    resetStoreE(); loadMembersFromFile(TFILE);
    CHECK(memberCount==0);
}

static void sec_add_save_load_basic(const char* TFILE)
{
    resetStoreE();
    CHECK(addLoweredE("John Doe",25,"Gold","2025-01-05")>0);
    CHECK(addLoweredE("Jane Smith",30,"Silver","2025-02-10")>0);
    saveMembersToFile(TFILE);

    resetStoreE(); loadMembersFromFile(TFILE);
    CHECK(memberCount==2);
    CHECK(STRCASECMP(name[0],"john doe")==0);
    CHECK(STRCASECMP(membershipType[0],"gold")==0);
    CHECK(STRCASECMP(name[1],"jane smith")==0);
    CHECK(STRCASECMP(membershipType[1],"silver")==0);
}

static void sec_duplicate_delete_first(const char* TFILE)
{
    CHECK(addLoweredE("JOHN DOE",28,"GOLD","2025-03-01")>0);
    saveMembersToFile(TFILE);

    resetStoreE(); loadMembersFromFile(TFILE);
    CHECK(findByNameCI("john doe")==0);

    int before = memberCount;
    CHECK(deleteByNameCI("JoHn DoE")==1);
    CHECK(memberCount==before-1);
    CHECK(findByNameCI("john doe")>=0);
    CHECK(deleteByNameCI("john doe")==1);
    CHECK(findByNameCI("john doe")==-1);
}

static void sec_long_fields_trunc(const char* TFILE)
{
    char longName[200], longType[200];
    memset(longName,'a',sizeof(longName)); longName[sizeof(longName)-1]='\0';
    memset(longType,'b',sizeof(longType)); longType[sizeof(longType)-1]='\0';
    CHECK(addLoweredE(longName,22,longType,"2025-04-04")>0);
    saveMembersToFile(TFILE);

    resetStoreE(); loadMembersFromFile(TFILE);
    CHECK(name[memberCount-1][49]=='\0');
    CHECK(membershipType[memberCount-1][19]=='\0');
}

static void sec_capacity_full(const char* TFILE)
{
    while (memberCount < MAX_MEMBERS)
    {
        char t[32]; snprintf(t,sizeof(t),"x%03d",memberCount);
        int ok = addLoweredE(t,20,"gold","2025-05-05");
        if (ok<0) break;
    }
    int capBefore = memberCount;
    CHECK(addLoweredE("overflow",20,"gold","2025-06-06")==-1);
    CHECK(memberCount==capBefore);
    saveMembersToFile(TFILE);

    resetStoreE(); loadMembersFromFile(TFILE);
    CHECK(memberCount==capBefore);
}

static void sec_malformed_rows(const char* TFILE)
{
    writeTextFile(TFILE,
        "MemberName,Age,MembershipType,RegistrationDate\n"
        "badline-without-commas\n"
        "only,two,columns\n"
        "good guy,21,gold,2025-07-07\n"
    );
    resetStoreE(); loadMembersFromFile(TFILE);
    int ok=0;
    for(int i=0;i<memberCount;i++)
    {
        if(STRCASECMP(name[i],"good guy")==0 && STRCASECMP(membershipType[i],"gold")==0){ ok=1; break; }
    }
    CHECK(ok==1);
}

static void sec_update_persist(const char* TFILE)
{
    int idx = findByNameCI("good guy");
    CHECK(idx>=0);
    strcpy(membershipType[idx], "silver");
    saveMembersToFile(TFILE);

    resetStoreE(); loadMembersFromFile(TFILE);
    idx = findByNameCI("good guy");
    CHECK(idx>=0 && STRCASECMP(membershipType[idx],"silver")==0);
}

static void sec_delete_not_exist_empty(const char* TFILE)
{
    (void)TFILE;
    CHECK(deleteByNameCI("nobody")==0);
    while (memberCount>0){ CHECK(deleteByNameCI(name[0])==1); }
    CHECK(memberCount==0);
    CHECK(deleteByNameCI("any")==0);
}

void runE2EAllErrors_Pretty(void)
{
    const char* TFILE = "e2e_all_errors.csv";
    int totalFailed = 0;
    printf("\n===== E2E (all error paths) =====\n");
    remove(TFILE);

    int sr, sf;

    sr=t_run; sf=t_fail; sec_missing_empty_header(TFILE);
    totalFailed += printSectionResult("Missing/Empty/Header-only", sr, sf);

    sr=t_run; sf=t_fail; sec_add_save_load_basic(TFILE);
    totalFailed += printSectionResult("Add+Save+Load (basic)", sr, sf);

    sr=t_run; sf=t_fail; sec_duplicate_delete_first(TFILE);
    totalFailed += printSectionResult("Duplicate + Delete-First", sr, sf);

    sr=t_run; sf=t_fail; sec_long_fields_trunc(TFILE);
    totalFailed += printSectionResult("Long Fields (Trunc/Null-Term)", sr, sf);

    sr=t_run; sf=t_fail; sec_capacity_full(TFILE);
    totalFailed += printSectionResult("Capacity Full", sr, sf);

    sr=t_run; sf=t_fail; sec_malformed_rows(TFILE);
    totalFailed += printSectionResult("Malformed Rows (robust load)", sr, sf);

    sr=t_run; sf=t_fail; sec_update_persist(TFILE);
    totalFailed += printSectionResult("Update + Persist", sr, sf);

    sr=t_run; sf=t_fail; sec_delete_not_exist_empty(TFILE);
    totalFailed += printSectionResult("Delete not-exist & on empty", sr, sf);
    

    printf("---------------------------------\n");
    if (totalFailed==0) 
        printf("E2E Summary: PASS (total checks: %d)\n", t_run);
    else 
        printf("E2E Summary: FAILED sections=%d (failed checks: %d / %d)\n",
                totalFailed, t_fail, t_run);

    remove(TFILE);
    resetStoreE(); 
    t_run=t_fail=0;
}
