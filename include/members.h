#ifndef MEMBERS_H
#define MEMBERS_H

#include "platform.h"
#include <stdio.h>

#define MAX_MEMBERS 100
#define FILENAME "member.csv"

/* Global storage */
extern char name[MAX_MEMBERS][50];
extern int  age[MAX_MEMBERS];
extern char membershipType[MAX_MEMBERS][20];
extern char registrationDate[MAX_MEMBERS][20];
extern int  memberCount;

/* utilities */
void clearScreen(void);
void toLowerCase(char* s);
int  isValidNameOrType(const char* s);

/* core IO */
void loadMembers(void);                   /* uses FILENAME */
void saveMembers(void);                   /* uses FILENAME */
void loadMembersFromFile(const char* filename);
void saveMembersToFile(const char* filename);

/* core features */
void displayMembers(void);
void addMember(void);
void searchMember(void);
void updateMember(void);
void deleteMember(void);

/* test hooks */
int  addMemberDirect(const char* n, int a, const char* t, const char* d);
int  findByNameCI(const char* n);
int  deleteByNameCI(const char* n);

#endif /* MEMBERS_H */