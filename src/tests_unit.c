#include <stdio.h>
#include <string.h>
#include "members.h"
#include "platform.h"

/* Minimal test framework */
static int t_run=0, t_fail=0;
#define CHECK(c) do{ t_run++; if(!(c)){ t_fail++; \
  fprintf(stderr,"[FAIL] %s:%d -> %s\n", __FILE__, __LINE__, #c); } }while(0)
#define CHECK_EQ_INT(a,b) CHECK((a)==(b))

/* helpers for test preparation */
static void resetStore(void)
{
    for (int i=0;i<memberCount;i++)
    {
        name[i][0]='\0'; membershipType[i][0]='\0'; registrationDate[i][0]='\0'; age[i]=0;
    }
    memberCount = 0;
}

static int addLowered(const char* n,int a,const char* t,const char* d)
{
    char nn[50], tt[20], dd[20];
    if (!n||!t||!d) return -1;
    strncpy(nn,n,sizeof(nn)-1); nn[sizeof(nn)-1]='\0'; toLowerCase(nn);
    strncpy(tt,t,sizeof(tt)-1); tt[sizeof(tt)-1]='\0'; toLowerCase(tt);
    strncpy(dd,d,sizeof(dd)-1); dd[sizeof(dd)-1]='\0';
    return addMemberDirect(nn,a,tt,dd);
}

void runUnitTests(void)
{
    resetStore(); t_run=0; t_fail=0;
    printf("\n===== Running Unit Tests: Search + Delete =====\n");

    CHECK(addLowered("John Doe",   25, "Gold",   "2025-01-05") > 0);
    CHECK(addLowered("Jane Smith", 30, "Silver", "2025-02-10") > 0);
    CHECK(addLowered("JOHN DOE",   28, "Gold",   "2025-03-01") > 0);

    CHECK(findByNameCI("john doe") == 0);
    CHECK(findByNameCI("JOHN DOE") == 0);
    CHECK(findByNameCI("jane smith") == 1);
    CHECK(findByNameCI("nobody") == -1);

    int before = memberCount;
    CHECK(deleteByNameCI("JoHn DoE") == 1);
    CHECK(memberCount == before - 1);

    CHECK(STRCASECMP(name[0], "jane smith") == 0);
    CHECK(STRCASECMP(name[1], "john doe") == 0);

    before = memberCount;
    CHECK(deleteByNameCI("john doe") == 1);
    CHECK(memberCount == before - 1);
    CHECK(findByNameCI("john doe") == -1);
    CHECK(STRCASECMP(name[0], "jane smith") == 0);

    before = memberCount;
    CHECK(deleteByNameCI("not-exist") == 0);
    CHECK(memberCount == before);

    CHECK(deleteByNameCI("jane smith") == 1);
    CHECK(memberCount == 0);
    CHECK(deleteByNameCI("any") == 0);
    CHECK(memberCount == 0);

    if (t_fail==0) printf("Search+Delete unit tests passed! (%d checks)\n", t_run);
    else printf("Search+Delete unit tests FAILED: %d/%d failed.\n", t_fail, t_run);

    resetStore(); t_run=t_fail=0;
}