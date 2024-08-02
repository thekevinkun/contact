// Declares a contact's functionality

#ifndef CONTACT_H
#define CONTACT_H

#include <stdbool.h>

#define FIRST_NAME_LENGTH 25
#define LAST_NAME_LENGTH 25
#define PHONE_NUMBER_LENGTH 20
#define EMAIL_LENGTH 65
#define WORK_LENGTH 50
#define CITY_LENGTH 65
#define ADDRESS_LENGTH 65

int main_menu(void);
void draw_blocks(void);
unsigned int hash(const char *person);
bool check(const char *person);
bool load(const char *contact);
bool unload(void);
bool add(const char *contact);
bool edit(const char *contact);
bool search(void);
void list(const char *contact);
bool deleted(const char *contact);
void remove_trailing_newline(char *str);

#endif // CONTACT_H