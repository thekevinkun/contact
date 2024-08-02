// Implements a contact's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#include "contact.h"

// Represents a persons's contact as a node in a hash table
typedef struct person
{
    char firstName[FIRST_NAME_LENGTH + 1];
    char lastName[LAST_NAME_LENGTH + 1];
    char phoneNumber[PHONE_NUMBER_LENGTH + 1];
    char email[EMAIL_LENGTH + 1];
    char work[WORK_LENGTH + 1];
    char city[CITY_LENGTH + 1];
    char address[ADDRESS_LENGTH + 1];
}
person;

typedef struct node
{
    person data;
    struct node *next;
}
node;


// Number of buckets in hash table
const unsigned int N = 676;

// Get first two letter in word/name to be hashed for a keys in a table
#define MAX_HASHED_LETTERS 2

// Hash Table
node *table[N];

// Loads contact to memory, return true if success
bool load(const char *contact)
{
    // Open contact file
    FILE *file = fopen(contact, "r");
    if (file == NULL)
    {
        printf("Could not open %s file at load().\n", contact);
        return false;
    }

    // Create person to temporary store values read from csv
    person p;

    node *tmp;
    node *prev;

    // Reading csv file, store each value in person
    while (fscanf(file, "%[^,], %[^,], %[^,], %[^,], %[^,], %[^,], %[^\n] ", 
            p.firstName, p.lastName, p.phoneNumber, p.email, p.work, p.city, p.address ) == 7)
    {
        // Skip header in csv
        if (strcmp("FirstName", p.firstName) == 0)
        {
            continue;
        }

        // Create new node for each person
        node *new = malloc(sizeof(node));
        if (new == NULL)
        {
            printf("Failed to allocate memory at load().\n");
            return false;
        }

        // Store person in node's data 
        new->data = p;

        // Set next to NULL since no one to point yet
        new->next = NULL;

        // Ready to hash

        // Produce key by person first name
        int hash_key = hash(new->data.firstName);
        if (hash_key == -1)
        {
            printf("Failed to hash %s.\n", new->data.firstName);
            return false;
        }


        // Prepare to store node in table in sorted way
        tmp = table[hash_key];
        prev = NULL;

        while (tmp != NULL && strcasecmp(tmp->data.firstName, new->data.firstName) < 0)
        {
            /* 
                While tmp still point at some node and the name in Node less than name in NEW Node,
                Traverse prev and tmp to find next node, til we find name in Node that's greater
                EX: Carrie
                    Andreas Beckham David
                              prev   tmp (keep traverse til prev point at Beckham, and tmp is David) 
            */

            prev = tmp;
            tmp = tmp->next;
        }

        // Insert new node in sorted way with prev and tmp as determinant position
        if (prev == NULL)
        {
            /*
                If prev not point at any node, means no node in table 
                Ex: Carrie
                    Tb = 
            
                or the node it's only one and its greater
                Ex: Carrie
                    Tb = David
            */

            new->next = table[hash_key];
            table[hash_key] = new;
        }
        else
        {
            /*
                If there's node in many Node in Table, 
                insert new node between node that currently prev point and it's prev->next
                EX: Carrie
                    Andreas Beckham David
                            prev    tmp (prev->next)
                    Andreas Beckham Carrie David
            */

            new->next = prev->next;
            prev->next = new;
        }   

        // END OF WHILE LOOP TO READ CSV FILE
    }

    // Close file    
    fclose(file);

    return true;
}

// Hashes person to number
unsigned int hash(const char *person)
{
    // Check if no name entered
    if (person == NULL)
    {
        return -1;
    }

    // Prepare to hash first two letter in person's name
    unsigned int L[MAX_HASHED_LETTERS] = {0, 0};
    unsigned int next_letter = 0;

    // Iterate through each character in person's name
    for (char c = *person; c != '\0'; c = *(++person))
    {
        // Make sure it's alphabet, otherwise go to next letter
        // Ex: A's | get A and S
        if (isalpha(c))
        {
            L[next_letter++] = toupper(c) - 'A';
            
            if (next_letter == MAX_HASHED_LETTERS)
            {
                break;
            }
        }
    }

    unsigned int hash_index = L[0] * 26 + L[1];

    return hash_index;
}

bool check(const char *person)
{
    // int hash_index;

    // if (isalpha(person[0]))
    // {
    //     hash_index = hash(person);
    //     node* tmp = table[hash_index];

    //     while (tmp != NULL)
    //     {
    //         if (strcasecmp(person, tmp->data.firstName))
    //         {

    //         }
    //     }
        
    // }
    return true;
}

// Add new person to a contact, return true if success
bool add(const char *contact)
{
    // START Draw Add Contact Menu
    system("clear");
    
    printf("\n\n");
    printf("\t\t\t\t\t\t\t  ADD CONTACT\n");
    draw_blocks();
    printf("\n");
    // END Draw Add Contact MENU

    // Prepare to ask user for contact information
    int len = 0;

    // Ask user for first name. 
    // Make sure user input is valid, whether it will no input or too long
    char firstName[FIRST_NAME_LENGTH + 1];
    do
    {
        printf("\33[2K\r");
        printf("\t\t\t\tFirst Name (Max. %i Characters)\t: ", FIRST_NAME_LENGTH);
        fgets(firstName, FIRST_NAME_LENGTH, stdin);

        // remove trailing '\n' from fgets
        remove_trailing_newline(firstName);

        // Make sure first letter is capital
        firstName[0] = toupper(firstName[0]);

        len = strlen(firstName);

        if (len <= 0)
        {
            printf("\33[A\33[J\r");
            printf("\t\t\t\t\t\t   Please enter valid Name!");
            sleep(3);
        }
        else if (len >= FIRST_NAME_LENGTH - 1)
        {
            printf("\33[A\33[J\r");
            printf("\t\t\t\t\t\tExceeded the maximum number of %i charactes!", FIRST_NAME_LENGTH);
            sleep(3);
        }
    } 
    while (len <= 0 || len >= FIRST_NAME_LENGTH - 1);
    
    printf("\n");

    // Ask user for last name
    // Make sure user input is valid, whether it will no input or too long 
    char lastName[LAST_NAME_LENGTH + 1];

    do
    {
        printf("\33[2K\r");
        printf("\t\t\t\tLast Name (Max. %i Characters)\t: ", LAST_NAME_LENGTH);
        fgets(lastName, LAST_NAME_LENGTH, stdin);
        // remove trailing '\n' from fgets
        remove_trailing_newline(lastName);

        // Make sure first letter is capital
        lastName[0] = toupper(lastName[0]);
        
        len = strlen(lastName); 

        if (len <= 0)
        {
            printf("\33[A\33[J\r");
            printf("\t\t\t\t\t\t   Please enter valid Name!");
            sleep(3);
        }
        else if (len >= LAST_NAME_LENGTH - 1)
        {
            printf("\33[A\33[J\r");
            printf("\t\t\t\t\t\tExceeded the maximum number of %i charactes!", LAST_NAME_LENGTH);
            sleep(3);
        }
    } 
    while (len <= 0 || len >= LAST_NAME_LENGTH - 1);

    printf("\n");

    // Ask user for phone number
    // Make sure user input is valid, whether it will no input or too long 
    char phoneNumber[PHONE_NUMBER_LENGTH + 1];
    do
    {
        printf("\33[2K\r");
        printf("\t\t\t\tPhone (Max. %i Characters)\t: ", PHONE_NUMBER_LENGTH);
        fgets(phoneNumber, PHONE_NUMBER_LENGTH, stdin);
        // remove trailing '\n' from fgets
        remove_trailing_newline(phoneNumber);

        len = strlen(phoneNumber); 

        if (len <= 0)
        {
            printf("\33[A\33[J\r");
            printf("\t\t\t\t\t\t   Please enter valid Phone Number!");
            sleep(3);
        }
        else if (len >= PHONE_NUMBER_LENGTH - 1)
        {
            printf("\33[A\33[J\r");
            printf("\t\t\t\t\t\tExceeded the maximum number of %i charactes!", PHONE_NUMBER_LENGTH);
            sleep(3);
        }
    } 
    while (len <= 0 || len >= PHONE_NUMBER_LENGTH - 1);
    
    printf("\n");

    // Ask user for email
    // Make sure user input is valid, whether it will no input or too long 
    char email[EMAIL_LENGTH + 1];
    do
    {
        printf("\33[2K\r");
        printf("\t\t\t\tEmail (Max. %i Characters)\t: ", EMAIL_LENGTH);
        fgets(email, EMAIL_LENGTH, stdin);
        // remove trailing '\n' from fgets
        remove_trailing_newline(email);

        len = strlen(email); 

        if (len <= 0)
        {
            printf("\33[A\33[J\r");
            printf("\t\t\t\t\t\t   Please enter valid Email!");
            sleep(3);
        }
        else if (len >= EMAIL_LENGTH - 1)
        {
            printf("\33[A\33[J\r");
            printf("\t\t\t\t\t\tExceeded the maximum number of %i charactes!", EMAIL_LENGTH);
            sleep(3);
        }
    } 
    while (len <= 0 || len >= EMAIL_LENGTH - 1);

    printf("\n");

    // Ask user for work
    // Make sure user input is valid, whether it will no input or too long 
    char work[WORK_LENGTH + 1];
    do
    {
        printf("\33[2K\r");
        printf("\t\t\t\tWork (Max. %i Characters)\t: ", WORK_LENGTH);
        fgets(work, WORK_LENGTH, stdin);
        // remove trailing '\n' from fgets
        remove_trailing_newline(work);

        // Make sure first letter is capital
        work[0] = toupper(work[0]);

        len = strlen(work); 

        if (len <= 0)
        {
            printf("\33[A\33[J\r");
            printf("\t\t\t\t\t\t   Please enter valid Work!");
            sleep(3);
        }
        else if (len >= WORK_LENGTH - 1)
        {
            printf("\33[A\33[J\r");
            printf("\t\t\t\t\t\tExceeded the maximum number of %i charactes!", WORK_LENGTH);
            sleep(3);
        }
    } 
    while (len <= 0 || len >= WORK_LENGTH - 1);

    printf("\n");

    // Ask user for city
    // Make sure user input is valid, whether it will no input or too long
    char city[CITY_LENGTH + 1];
    do
    {
        printf("\33[2K\r");
        printf("\t\t\t\tCity (Max. %i Characters)\t: ", CITY_LENGTH);
        fgets(city, CITY_LENGTH, stdin);
        // remove trailing '\n' from fgets
        remove_trailing_newline(city);

        // Make sure first letter is capital
        city[0] = toupper(city[0]);

        len = strlen(city); 

        if (len <= 0)
        {
            printf("\33[A\33[J\r");
            printf("\t\t\t\t\t\t   Please enter valid City!");
            sleep(3);
        }
        else if (len >= CITY_LENGTH - 1)
        {
            printf("\33[A\33[J\r");
            printf("\t\t\t\t\t\tExceeded the maximum number of %i charactes!", CITY_LENGTH);
            sleep(3);
        }
    } 
    while (len <= 0 || len >= CITY_LENGTH - 1);

    printf("\n");

    // Ask user for address
    // Make sure user input is valid, whether it will no input or too long
    char address[ADDRESS_LENGTH + 1];
    do
    {
        printf("\33[2K\r");
        printf("\t\t\t\tAddress (Max. %i Characters)\t: ", ADDRESS_LENGTH);
        fgets(address, ADDRESS_LENGTH, stdin);
        // remove trailing '\n' from fgets
        remove_trailing_newline(address);

        // Make sure first letter is capital
        if (isalpha(address[0]))
        {
            address[0] = toupper(address[0]);
        }

        len = strlen(address); 

        if (len <= 0)
        {
            printf("\33[A\33[J\r");
            printf("\t\t\t\t\t\t   Please enter valid Address!");
            sleep(3);
        }
        else if (len >= ADDRESS_LENGTH - 1)
        {
            printf("\33[A\33[J\r");
            printf("\t\t\t\t\t\tExceeded the maximum number of %i charactes!", ADDRESS_LENGTH);
            sleep(3);
        }
    } 
    while (len <= 0 || len >= ADDRESS_LENGTH - 1);

    // Two new line in terminal
    printf("\n\n");
    
    // Check if phone number already existed in contact
    bool checked = check(phoneNumber);
    if (!checked)
    {
        // Clear 8 line move up in terminal before print new line
        printf("\33[15A\33[J\r");
        printf("\t\t\t\t\t\t   Phone Number is already existed.\n");
        return false;
    }

    // Prepare to write valid phone number to csv file

    // Open contact file as append mode
    FILE *file = fopen(contact, "a");
    if (file == NULL)
    {
        // Clear 8 line move up in terminal before print new line
        printf("\33[15A\33[J\r");
        printf("\t\t\t\t\t\t   Could not open %s at add().\n", contact);
        return false;
    }
    // Write to csv file
    fprintf(file, "%s, %s, %s, %s, %s, %s, %s\n", firstName, lastName, phoneNumber, email, work, city, address);

    // Close file
    fclose(file);

    printf("\t\t\t\t\t\t   ADD NEW CONTACT SUCCESSFUL!\n\n");
    
    // Draw end blocks
    draw_blocks();
    
    return true;
}

// Search a person in a contact, return true if found
bool search(void)
{
    // START Draw Search Contact Menu
    system("clear");
    
    printf("\n\n");
    printf("\t\t\t\t\t\t\tSEARCH CONTACT\n");
    draw_blocks();
    printf("\n");
    // END Draw Search Contact Menu

    // Prepare to ask user on which method they want to search
    int search_status;

    printf("\t\t\t\t\t\t\t1. Search by Name\n");
    printf("\t\t\t\t\t\t\t2. Search by Number\n\n");

    do
    {
        printf("\33[2K\r");
        printf("\t\t\t\t\t\t\tEnter: ");
        scanf("%d", &search_status);
        getchar();

        if (search_status < 1 || search_status > 2)
        {
            printf("\33[A\33[J\r");
            printf("\t\t\t\t\t\t\tPlease enter valid Search!");
            sleep(3);
        }
    } 
    while (search_status < 1 || search_status > 2);

    // Ready to get user input
    char firstName[FIRST_NAME_LENGTH + 1];
    char lastName[LAST_NAME_LENGTH + 1];
    char phoneNumber[PHONE_NUMBER_LENGTH + 1];

    int len;

    int hash_key;
    bool found = false;

    switch (search_status)
    {
        case 1:
            printf("\33[4A\33[J\r");
            
            // Ask user for first name, make sure the input is valid
            do
            {
                printf("\33[2K\r");
                printf("\t\t\t\tFirst Name (Max. %i characters)\t: ", FIRST_NAME_LENGTH);
                fgets(firstName, FIRST_NAME_LENGTH, stdin);
                remove_trailing_newline(firstName);

                len = strlen(firstName);

                if (len <= 0)
                {
                    printf("\33[A\33[J\r");
                    printf("\t\t\t\t\t\t   Please enter valid Name!");
                    sleep(3);
                }
                else if (len >= FIRST_NAME_LENGTH - 1)
                {
                    printf("\33[A\33[J\r");
                    printf("\t\t\t\t\t\t   Exceeded the maximum number of %i charactes!", FIRST_NAME_LENGTH);
                    sleep(3);
                }
            } 
            while (len <= 0 || len >= FIRST_NAME_LENGTH - 1);
            
            printf("\n");

            // Ask user for last name, make sure the input is valid
            do
            {
                printf("\33[2K\r");
                printf("\t\t\t\tLast Name (Max. %i characters)\t: ", LAST_NAME_LENGTH);
                fgets(lastName, FIRST_NAME_LENGTH, stdin);
                remove_trailing_newline(lastName);

                len = strlen(lastName);

                if (len >= LAST_NAME_LENGTH - 1)
                {
                    printf("\33[A\33[J\r");
                    printf("\t\t\t\t\t\t   Exceeded the maximum number of %i charactes!", LAST_NAME_LENGTH);
                    sleep(3);
                }
            } 
            while (len >= LAST_NAME_LENGTH - 1);

            printf("\33[3A\33[J\r");

            // Hash by name to get keys
            hash_key = hash(firstName);
            if (hash_key == -1)
            {
                printf("\n\n");
                printf("\t\t\t\t\t\t\tFailed to hash %s at search().\n", firstName);
                return false;
            }

            // Create node to point at table at keys position
            node *tmp = table[hash_key];

            int lastname_len = strlen(lastName);

            // Keep traverse to all buckets in table
            // If found, print all contact information. Otherwise, go to next node
            while (tmp != NULL)
            {
                // If user give empty input for last name, just use first name for searching
                if (lastname_len <= 0)
                {
                    if (strcasecmp(firstName, tmp->data.firstName) == 0)
                    {
                        printf("\t\t\t\t\t\t    Name: %s %s\n", tmp->data.firstName, tmp->data.lastName);
                        printf("\t\t\t\t\t\t    Phone: %s\n", tmp->data.phoneNumber);
                        printf("\t\t\t\t\t\t    Email: %s\n", tmp->data.email);
                        printf("\t\t\t\t\t\t    Work: %s\n", tmp->data.work);
                        printf("\t\t\t\t\t\t    City: %s\n", tmp->data.city);
                        printf("\t\t\t\t\t\t    Address: %s\n", tmp->data.address);
                        printf("\n");

                        // Set found to true to tell at the end of while loop, that we got something
                        found = true;

                        // Keep checking, we may find there's more than one information in one person
                        tmp = tmp->next;
                    }
                    else
                    {
                        tmp = tmp->next;
                    }
                }
                // Otherwise, search for first name and last name from user input
                else 
                {
                    if (strcasecmp(firstName, tmp->data.firstName) == 0 && strcasecmp(lastName, tmp->data.lastName) == 0)
                    {
                        printf("\t\t\t\t\t\t    Name: %s %s\n", tmp->data.firstName, tmp->data.lastName);
                        printf("\t\t\t\t\t\t    Phone: %s\n", tmp->data.phoneNumber);
                        printf("\t\t\t\t\t\t    Email: %s\n", tmp->data.email);
                        printf("\t\t\t\t\t\t    Work: %s\n", tmp->data.work);
                        printf("\t\t\t\t\t\t    City: %s\n", tmp->data.city);
                        printf("\t\t\t\t\t\t    Address: %s\n", tmp->data.address);
                        printf("\n");

                        // Set found to true to tell at the end of while loop, that we got something
                        found = true;

                        // Keep checking, we may find there's more than one information in one person
                        tmp = tmp->next;
                    }
                    else
                    {
                        tmp = tmp->next;
                    }
                }
                
            }
            break;
        case 2:
            // printf("\33[4A\33[J\r");

            // printf("\t\tSearch by Number: ");
            // fgets(search, FIRST_NAME_LENGTH + PHONE_NUMBER_LENGTH, stdin);
            // remove_trailing_newline(search);
            break;
        default:
            break;
    }

    // Tell there's no contact they searching for
    if(!found)
    {
        printf("\t\t\t\t\t\t    Could not find contact.\n");
        return false;
    }

    // Draw end of blocks
    draw_blocks();

    return true;
}

// Edit Contact
bool edit(const char *contact)
{
    // START Draw Search Contact Menu
    system("clear");
    
    printf("\n\n");
    printf("\t\t\t\t\t\t\t  EDIT CONTACT\n");
    draw_blocks();
    printf("\n");
    // END Draw Search Contact Menu

    // Prepare to ask user for name
    char firstName[FIRST_NAME_LENGTH + 1];
    char lastName[LAST_NAME_LENGTH + 1];

    // To store length of first and last name
    int len;

    // Ask user for first name, make sure the input is valid
    do
    {
        printf("\33[2K\r");
        printf("\t\t\t\tFirst Name (Max. %i characters)\t: ", FIRST_NAME_LENGTH);
        fgets(firstName, FIRST_NAME_LENGTH, stdin);
        remove_trailing_newline(firstName);

        len = strlen(firstName);

        if (len <= 0)
        {
            printf("\33[A\33[J\r");
            printf("\t\t\t\t\t\t    Please enter valid name!");
            sleep(3);
        }
        else if (len >= FIRST_NAME_LENGTH - 1)
        {
            printf("\33[A\33[J\r");
            printf("\t\t\t\t\t\t   Exceeded the maximum number of %i charactes!", FIRST_NAME_LENGTH);
            sleep(3);
        }
    } 
    while (len <= 0 || len >= FIRST_NAME_LENGTH - 1);
    
    printf("\n");
    
    // Ask user for last name, make sure the input is valid
    do
    {
        printf("\33[2K\r");
        printf("\t\t\t\tLast Name (Max. %i characters)\t: ", LAST_NAME_LENGTH);
        fgets(lastName, FIRST_NAME_LENGTH, stdin);
        remove_trailing_newline(lastName);

        len = strlen(lastName);

        if (len >= LAST_NAME_LENGTH - 1)
        {
            printf("\33[A\33[J\r");
            printf("\t\t\t\t\t\t   Exceeded the maximum number of %i charactes!", LAST_NAME_LENGTH);
            sleep(3);
        }
    } 
    while (len >= LAST_NAME_LENGTH - 1);

    
    // Prepare to check in hash table if maybe there's two contact with same name
    
    // Get key by hashing first name
    int hash_key = hash(firstName);
    if (hash_key == -1)
    {
        printf("\33[3A\33[J\r");
        printf("\t\t\t\t\t\t    Failed to hash %s at delete().\n", firstName);
        return false;
    }

    // Go to first node in table
    node *tmp = table[hash_key];

    // Get length of last name to check if it's empty or not
    int lastname_len = strlen(lastName);

    // To tell how many people we found with same name
    int people_count = 0;
    
    // Tell node position of a person in table
    // This only for a person that only found one (look line 498 and 517)
    int node_position = 0;

    // Count each node in table
    int count_node = 0;
    
    printf("\33[3A\33[J\r");
    
    while (tmp != NULL)
    {
        /* 
            If first name and last name user enter is exist,
            First, we get it's node position by using count_node
            Then, we count people that we found
            Print all information
            Then, go to next node to find if there's more person with same name
        */
        count_node++;

        // If user give empty input for last name, just use first name for searching
        if (lastname_len <= 0)
        {
            if (strcasecmp(firstName, tmp->data.firstName) == 0)
            {
                node_position = count_node;
                people_count++;

                printf("      \t\t[%i] ", people_count);
                printf("%s %s  ", tmp->data.firstName, tmp->data.lastName);
                printf("%s  ", tmp->data.phoneNumber);
                printf("%s    ", tmp->data.email);
                printf("%s  ", tmp->data.work);
                printf("%s  ", tmp->data.city);
                printf("%s", tmp->data.address);
                printf("\n\n");

                tmp = tmp->next;
            }
            else
            {
                tmp = tmp->next;
            }
        }
        // Otherwise, search for first name and last name from user input
        else 
        {
            if (strcasecmp(firstName, tmp->data.firstName) == 0 && strcasecmp(lastName, tmp->data.lastName) == 0)
            {
                node_position = count_node;
                people_count++;

                printf("      \t\t[%i] ", people_count);
                printf("%s %s  ", tmp->data.firstName, tmp->data.lastName);
                printf("%s  ", tmp->data.phoneNumber);
                printf("%s    ", tmp->data.email);
                printf("%s  ", tmp->data.work);
                printf("%s  ", tmp->data.city);
                printf("%s", tmp->data.address);
                printf("\n\n");

                tmp = tmp->next;
            }
            else
            {
                tmp = tmp->next;
            }
        }
    }

    // Get user choice if there's more than one person with same name
    // This will use as a node position of the person that user want to edit
    int edited_person = 0;

    // If there's one person with same name, ask user by number which one to edit
    if (people_count > 1)
    {
        printf("\n");
        printf("\t\t\t\t\t      Which person you want to edit (1/2)?\n");
        printf("\t\t\t\t\t      Enter: ");
        scanf("%d", &edited_person);
        getchar();

        printf("\33[5A\33[J\r");
        printf("\33[%dA\33[J\r", people_count);
    }
    // If there's only one person with given name
    // We just use node_position since that's the only person we found
    else if (people_count == 1)
    {       
        edited_person = node_position;
        printf("\33[2A\33[J\r");    
    }
    // If no person found, failed
    else
    {
        printf("\t\t\t\t\t\t    Could not find contact.\n");
        return false;
    }

    // Prepare to get node of a person that want to be edit

    // After choose or found which to be edit
    // We go to the table again to get number of person that want to be edit
    tmp = table[hash_key];

    for (int i = 1; i < edited_person; i++)
    {
        /*
            Go to node in table as many as total of edited_person
            Cause edited_person is position of node that contain a person to edit

            Ex:    Node 1        Node 2         Node 3
                Emma Mason -> Emma Watson -> Emma Zander

            Look at line 458
            There's a case where there's only one person with given name,
            we just need to use node_position since it will stop at that node with help of count_node
            But if there's more than one person (as people_count tell),
            we ask user a number which represent position of node that will store in edited_person
        */
        tmp = tmp->next;
    }
    
    if (tmp == NULL)
    {
        printf("\t\t\t\t\t    Could not find contact.\n");
        return false;
    }

    // Print again, but this time only information about person to be edit
    printf("\t\t\t\t\t\t   [1] Name: %s %s\n", tmp->data.firstName, tmp->data.lastName);
    printf("\t\t\t\t\t\t   [2] Phone: %s\n", tmp->data.phoneNumber);
    printf("\t\t\t\t\t\t   [3] Email: %s\n", tmp->data.email);
    printf("\t\t\t\t\t\t   [4] Work: %s\n", tmp->data.work);
    printf("\t\t\t\t\t\t   [5] City: %s\n", tmp->data.city);
    printf("\t\t\t\t\t\t   [6] Address: %s\n", tmp->data.address);
    printf("\n\n");

    // Get user input of edit field
    const int edit_length = 75;
    char edit[edit_length];

    // Ask user which to be delete with number
    int edit_status;

    printf("\t\t\t\t\t\t   Which one you want to edit?\n");
    printf("\t\t\t\t\t\t   Enter: ");
    scanf("%d", &edit_status);
    getchar();

    printf("\n\n");

    switch (edit_status)
    {
        case 1:
            printf("\33[4A\33[J\r");
            printf("\t\t\t\t\t\t   First Name: ");
            fgets(firstName, FIRST_NAME_LENGTH, stdin);
            remove_trailing_newline(firstName);

            printf("\t\t\t\t\t\t   Last Name: ");
            fgets(lastName, LAST_NAME_LENGTH, stdin);
            remove_trailing_newline(lastName);
            break;
        case 2:
            printf("\33[4A\33[J\r");
            printf("\t\t\t\t\t\t   Phone: ");
            fgets(edit, edit_length, stdin);
            remove_trailing_newline(edit);
            break;
        case 3:
            printf("\33[4A\33[J\r");
            printf("\t\t\t\t\t\t   Email: ");
            fgets(edit, edit_length, stdin);
            remove_trailing_newline(edit);
            break;
        case 4:
            printf("\33[4A\33[J\r");
            printf("\t\t\t\t\t\t   Work: ");
            fgets(edit, edit_length, stdin);
            remove_trailing_newline(edit);
            break;
        case 5:
            printf("\33[4A\33[J\r");
            printf("\t\t\t\t\t\t   City: ");
            fgets(edit, edit_length, stdin);
            remove_trailing_newline(edit);
            break;
        case 6:
            printf("\33[4A\33[J\r");
            printf("\t\t\t\t\t\t   Address: ");
            fgets(edit, edit_length, stdin);
            remove_trailing_newline(edit);
            break;
        default:
            printf("\t\t\t\t\t\t    Contact will not be edited!\n\n");
            draw_blocks();
            return true;
    }

    // Ready to renew csv file

    // Open contact file
    FILE *file = fopen(contact, "r");
    if (file == NULL)
    {
        printf("\t\t\t\t\t   Could not open %s file at edit().\n", contact);
        return false;
    }

    // Open and create new contact file to copying new values
    FILE *new_file = fopen("copy_contact.csv", "a");
    if (new_file == NULL)
    {
        printf("\t\t\t\t\t    Could not open copy_contact.csv file at delete().\n");
        return false;
    }

    // Reading csv file, store each value in person p
    person p;
    while (fscanf(file, "%[^,], %[^,], %[^,], %[^,], %[^,], %[^,], %[^\n] ", 
            p.firstName, p.lastName, p.phoneNumber, p.email, p.work, p.city, p.address ) == 7)
    {
        // Check first name and last name ALSO phone number if it's match with value in csv
        if (strcasecmp(tmp->data.firstName, p.firstName) == 0 && strcasecmp(tmp->data.lastName, p.lastName) == 0 
            && strcasecmp(tmp->data.phoneNumber, p.phoneNumber) == 0)
        {
            // Ensure which field user want to edit
            if (edit_status == 1)
            {
                strcpy(p.firstName, firstName);
                strcpy(p.lastName, lastName);
            }
            else if (edit_status == 2)
            {
                strcpy(p.phoneNumber, edit);
            }
            else if (edit_status == 3)
            {
                strcpy(p.email, edit);
            }
            else if (edit_status == 4)
            {
                strcpy(p.work, edit);
            }
            else if (edit_status == 5)
            {
                strcpy(p.city, edit);
            }
            else if (edit_status == 6)
            {
                strcpy(p.address, edit);
            }
        }

        // Write to copy of csv file
        fprintf(new_file, "%s, %s, %s, %s, %s, %s, %s\n", 
                p.firstName, p.lastName, p.phoneNumber, p.email, p.work, p.city, p.address);
    }

    // Close all files
    fclose(file);
    fclose(new_file);

    // Remove old csv
    remove(contact);

    // Rename copy csv as old csv name
    rename("copy_contact.csv", contact);

    printf("\n\n");
    printf("\t\t\t\t\t\t     EDIT CONTACT SUCCESSFUL!\n\n");
    
    // Draw end blocks
    draw_blocks();

    return true;
}

// View all contacts
void list(const char *contact)
{
    // START Draw View Contact Menu
    system("clear");
    
    printf("\n\n");
    printf("\t\t\t\t\t\t\tLIST OF CONTACT\n");
    draw_blocks();

    // Print Header
    printf("No\t   Name\t\t      Phone\t\t Email\t\t    Work\t   City\t\t\t Address");
    printf("\n\n");
    // END Draw View Contact MENU

    // Ready to get all contact in table in each node
    node *tmp = NULL;

    // As a number position in terminal
    int people_count = 1;

    // Iterate through all table
    for (int i = 0; i < N; i++)
    {
        // Set tmp node to each keys in table
        tmp = table[i];

        // Traverse through every node in keys
        while (tmp != NULL)
        {
            printf("%i", people_count);
            printf("\t%s %s", tmp->data.firstName, tmp->data.lastName);
            printf("\t %s", tmp->data.phoneNumber);
            printf("\t  %s", tmp->data.email);
            printf("\t  %s", tmp->data.work);
            printf("\t%s", tmp->data.city);
            printf("\t %s", tmp->data.address);
            printf("\n");

            people_count++;
            tmp = tmp->next;
        }
    }
    
    // Draw end blocks
    printf("\n");
    draw_blocks();

    return;
} 

// Delete a person in a contact, return true if success
bool deleted(const char *contact)
{
    // START Draw Search Contact Menu
    system("clear");
    
    printf("\n\n");
    printf("\t\t\t\t\t\t\tDELETE CONTACT\n");
    draw_blocks();
    printf("\n");
    // END Draw Search Contact Menu
    
    // Prepare to ask user for first name and last name
    char firstName[FIRST_NAME_LENGTH + 1];
    char lastName[LAST_NAME_LENGTH + 1];

    // To store length of first and last name
    int len;

    // Ask user for first name, make sure the input is valid
    do
    {
        printf("\33[2K\r");
        printf("\t\t\t\tFirst Name (Max. %i characters)\t: ", FIRST_NAME_LENGTH);
        fgets(firstName, FIRST_NAME_LENGTH, stdin);
        remove_trailing_newline(firstName);

        len = strlen(firstName);

        if (len <= 0)
        {
            printf("\33[A\33[J\r");
            printf("\t\t\t\t\t\t    Please enter valid name!");
            sleep(3);
        }
        else if (len >= FIRST_NAME_LENGTH - 1)
        {
            printf("\33[A\33[J\r");
            printf("\t\t\t\t\t\t   Exceeded the maximum number of %i charactes!", FIRST_NAME_LENGTH);
            sleep(3);
        }
    } 
    while (len <= 0 || len >= FIRST_NAME_LENGTH - 1);
    
    printf("\n");

    // Ask user for last name, make sure the input is valid
    do
    {
        printf("\33[2K\r");
        printf("\t\t\t\tLast Name (Max. %i characters)\t: ", LAST_NAME_LENGTH);
        fgets(lastName, FIRST_NAME_LENGTH, stdin);
        remove_trailing_newline(lastName);

        len = strlen(lastName);

        if (len <= 0)
        {
            printf("\33[A\33[J\r");
            printf("\t\t\t\t\t\t    Please enter valid name!");
            sleep(3);
        }
        else if (len >= LAST_NAME_LENGTH - 1)
        {
            printf("\33[A\33[J\r");
            printf("\t\t\t\t\t\t   Exceeded the maximum number of %i charactes!", LAST_NAME_LENGTH);
            sleep(3);
        }
    } 
    while (len <= 0 || len >= LAST_NAME_LENGTH - 1);
    
    // Prepare to check in hash table if maybe there's two contact with same name
    
    // Get key by hashing first name
    int hash_key = hash(firstName);
    if (hash_key == -1)
    {
        printf("\33[3A\33[J\r");
        printf("\t\t\t\t\t\t    Failed to hash %s at delete().\n", firstName);
        return false;
    }

    // Go to first node in table
    node *tmp = table[hash_key];

     // To tell how many people we found with same name
    int people_count = 0;

    // Tell node position of a person in table
    // This only for a person that only found one (look line 843 and 864)
    int node_position = 0;

    // Count each node in table
    int count_node = 0;

    printf("\33[3A\33[J\r");

    while (tmp != NULL)
    {
        /* 
            If first name and last name user enter is exist,
            First, we get it's node position by using count_node
            Then, we count people that we found
            Print all information
            Then, go to next node to find if there's more person with same name
        */
        count_node++;

        if (strcasecmp(firstName, tmp->data.firstName) == 0 && strcasecmp(lastName, tmp->data.lastName) == 0)
        {
            node_position = count_node;
            people_count++;

            printf("\t\t%i.   ", people_count);
            printf("%s %s  ", tmp->data.firstName, tmp->data.lastName);
            printf("%s  ", tmp->data.phoneNumber);
            printf("%s    ", tmp->data.email);
            printf("%s  ", tmp->data.work);
            printf("%s  ", tmp->data.city);
            printf("%s", tmp->data.address);
            printf("\n\n");

            tmp = tmp->next;
        }
        else
        {
            tmp = tmp->next;
        }
    }

    // Get user choice if there's more than one person with same name
    // This will use as a node position of the person that user want to delete
    int deleted_person;

    // If there's one person with same name, ask user by number which one to delete
    if (people_count > 1)
    {
        printf("\n");
        printf("\t\t\t\t\t     Which person you want to delete (1/2)?\n");
        printf("\t\t\t\t\t     Enter: ");
        scanf("%d", &deleted_person);
        getchar();

        printf("\33[5A\33[J\r");
        printf("\33[%dA\33[J\r", people_count);
    }
    // If there's only one person with given name
    // We just use node_position since that's the only person we found
    else if (people_count == 1)
    {       
        deleted_person = node_position; 
        printf("\33[2A\33[J\r");    
    }
    // If no person found, failed
    else
    {
        printf("\t\t\t\t\t\t    Could not find contact.\n");
        return false;
    }

    // Prepare to get node of a person that want to be delete

    // After choose or found which to be delete
    // We go to the table again to get number of person that want to be delete
    tmp = table[hash_key];
    
    // Go to node in table as many as total of deleted_person
    // Cause deleted_person is position of which node contain that person to delete
    for (int i = 1; i < deleted_person; i++)
    {
        /*
            Go to node in table as many as total of deleted_person
            Cause deleted_person is position of node that contain a person to delete

            Ex:    Node 1        Node 2         Node 3
                Emma Mason -> Emma Watson -> Emma Zander

            Look at line 803
            There's a case where there's only one person with given name,
            we just need to use node_position since it will stop at that node with help of count_node
            But if there's more than one person (as people_count tell),
            we ask user a number which represent position of node that will store in deleted_person
        */
        tmp = tmp->next;
    }

    if (tmp == NULL)
    {
        printf("\t\t\t\t\t    Could not find contact.\n");
        return false;
    }

    // Print again, but this time only information about person to be deleted
    printf("\t\t\t\t\t\t    Name: %s %s\n", tmp->data.firstName, tmp->data.lastName);
    printf("\t\t\t\t\t\t    Phone: %s\n", tmp->data.phoneNumber);
    printf("\t\t\t\t\t\t    Email: %s\n", tmp->data.email);
    printf("\t\t\t\t\t\t    Work: %s\n", tmp->data.work);
    printf("\t\t\t\t\t\t    City: %s\n", tmp->data.city);
    printf("\t\t\t\t\t\t    Address: %s\n", tmp->data.address);
    printf("\n\n");

    // Ready to make sure to delete person
    char status_delete;

    // Ask whether to delete it or not
    printf("\t\t\t\t\t    Are you sure you want to delete (y/n)? ");
    scanf("%c", &status_delete);
    getchar();

    printf("\n\n");

    if (status_delete == 'n')
    {
        printf("\t\t\t\t\t\t   Contact will not be deleted!\n\n");
        draw_blocks();
        return true;
    }
    

    // Ready to renew csv file

    // Open contact file
    FILE *file = fopen(contact, "r");
    if (file == NULL)
    {
        printf("\t\t\t\t\t    Could not open %s file at delete().\n", contact);
        return false;
    }

    // Open and create new contact file to copying new values
    FILE *new_file = fopen("copy_contact.csv", "a");
    if (new_file == NULL)
    {
        printf("\t\t\t\t\t    Could not open copy_contact.csv file at delete().\n");
        return false;
    }

    // Reading csv file, store each value in person p
    person p;
    while (fscanf(file, "%[^,], %[^,], %[^,], %[^,], %[^,], %[^,], %[^\n] ", p.firstName, p.lastName, p.phoneNumber, p.email, p.work, p.city, p.address ) == 7)
    {
        // Don't copying phone number of person that want to be deleted
        if (strcasecmp(tmp->data.phoneNumber, p.phoneNumber) == 0)
        {
            continue;
        }

        // Write to copy of csv file
        fprintf(new_file, "%s, %s, %s, %s, %s, %s, %s\n", p.firstName, p.lastName, p.phoneNumber, p.email, p.work, p.city, p.address);
    }

    // Close all files
    fclose(file);
    fclose(new_file);

    // Remove old csv
    remove(contact);

    // Rename copy csv as old csv name
    rename("copy_contact.csv", contact);

    printf("\t\t\t\t\t\t    DELETE CONTACT SUCCESSFUL!\n\n");
    
    // Draw end blocks
    draw_blocks();

    return true;
}

bool unload(void)
{
    node *head = NULL;
    node *tmp = NULL;

    for (int i = 0; i < N; i++)
    {
        head = table[i];

        while (head != NULL)
        {
            tmp = head;
            head = head->next;
            free(tmp);
        }
    }

    return true;
}

// Draw main menu, return user operation choices
int main_menu(void)
{
    system("clear");
    
    printf("\n\n");
    printf("\t\t\t\t\t\t\tCONTACT MANAGEMENT\n");
    draw_blocks();

    printf("\n");

    // Menu
    printf("\t\t\t\t\t\t\t1. Add New Contact\n");
    printf("\t\t\t\t\t\t\t2. Search Contact\n");
    printf("\t\t\t\t\t\t\t3. Edit Contact\n");
    printf("\t\t\t\t\t\t\t4. List Of Contact\n");
    printf("\t\t\t\t\t\t\t5. Delete Contact\n");
    printf("\t\t\t\t\t\t\t0. Exit\n");

    printf("\n");
    draw_blocks(); 

    // Ask for menu
    int status;
    do
    {
        printf("\33[2K\r");
        printf("\t\t\t\t\t\t\tEnter: ");
        scanf("%d", &status);
        getchar();
        
        if (status < 0 || status > 5)
        {
            printf("\33[A\33[J\r");
            printf("\t\t\t\t\t\t\tPlease enter valid menu!");
            sleep(5);
        }
    } 
    while (status < 0 || status > 5);
    

    return status;
}

void remove_trailing_newline(char *str)
{
    int len = strlen(str);
    
    if((len > 0 ) && (str[len - 1] == '\n'))
    {
        str[len - 1] = '\0';
    }
}

void draw_blocks(void)
{
    printf("==============================================================================================================================="); // 126 blocks
    printf("\n");
}

void loading()
{
    printf("\t\t\t\t\t\t");
    printf("      Please wait");
    for (int i = 0; i < 5; i++)
    {
        printf(".");
        fflush(stdout);
        sleep(3);
    }
    printf("\n");
}