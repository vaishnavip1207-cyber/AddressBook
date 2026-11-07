#include <stdio.h>
#include "file.h"
#include<string.h>

void saveContactsToFile(AddressBook *addressBook) {
    FILE *fp = fopen("contacts.csv", "w");

    if(fp == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    /* Header */
    fprintf(fp, "Name,Phone,Email\n");

    /* Write all contacts */
    for(int i = 0; i < addressBook->contactCount; i++)
    {
        fprintf(fp, "%s,%s,%s\n",
                addressBook->contacts[i].name,
                addressBook->contacts[i].phone,
                addressBook->contacts[i].email);
    }

    fclose(fp);

    printf("Contacts saved successfully.\n");
  
}

void loadContactsFromFile(AddressBook *addressBook) {
    FILE *fp = fopen("contacts.csv", "r");

    if(fp == NULL)
    {
        printf("contacts.csv not found.\n");
        return;
    }

    char line[200];

    
    fgets(line, sizeof(line), fp);

    while(fgets(line, sizeof(line), fp))
    {
        line[strcspn(line, "\n")] = '\0';

        char *name = strtok(line, ",");
        char *phone = strtok(NULL, ",");
        char *email = strtok(NULL, ",");

        if(name && phone && email)
        {
            strcpy(addressBook->contacts[addressBook->contactCount].name, name);
            strcpy(addressBook->contacts[addressBook->contactCount].phone, phone);
            strcpy(addressBook->contacts[addressBook->contactCount].email, email);

            addressBook->contactCount++;
        }
    }

    fclose(fp);

    printf("Contacts loaded successfully.\n");
    
}


