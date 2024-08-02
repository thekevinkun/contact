#include <stdbool.h>
#include <stdio.h>

#include "contact.h"

#define CONTACT "./contacts.csv"

int main(int argc, char *argv[])
{
    // Determine contact to use based on user command arguments
    char *contact = (argc == 2) ? argv[1] : CONTACT;
    
    // Load contact to memory
    bool loaded = load(contact);

    // Exit if contact not loaded
    if (!loaded)
    {
        printf("Could not load %s.\n", contact);
        return -1;
    }

    // Draw menu and ask for menu choices
    int status = main_menu(); 

    switch (status)
    {
        case 1:
            if (!add(contact))
            {
                printf("\t\t\t\t\t\t   Failed to add new contact.\n");
                printf("\n");
                draw_blocks();
            }
            break;
        case 2:
            if (!search())
            {
                printf("\t\t\t\t\t\t    Failed to search contact.\n");
                printf("\n");
                draw_blocks();
            }
            break;
        case 3:
            if(!edit(contact))
            {
                printf("\t\t\t\t\t\t    Failed to edit contact.\n");
                printf("\n");
                draw_blocks();
            }
            break;
        case 4:
            list(contact);
            break;
        case 5:
            if(!deleted(contact))
            {
                printf("\t\t\t\t\t\t    Failed to delete contact.\n");
                printf("\n");
                draw_blocks();
            }
            break;
        default:
            printf("You exit!\n");
            break;
    }

    bool unloaded = unload();
    if (!unloaded)
    {
        printf("Could not unload memory.\n");
        return -1;
    }

    return 0;
}