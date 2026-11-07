#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"
#include "file.h"
#include "populate.h"
#include <ctype.h>

void listContacts(AddressBook *addressBook, int sortCriteria) 
{
    // Sort contacts based on the chosen criteria
    // Repeatedly compares adjacent contacts and swaps them
    // until all names are arranged in ascending alphabetical order.
    switch(sortCriteria){
        case 1:printf("Sorting by name ; \n");
        for(int i=0;i<addressBook->contactCount-1;i++){
            for(int j=0;j<addressBook->contactCount-i-1;j++){
                if(strcmp(addressBook->contacts[j].name,addressBook->contacts[j+1].name)>0){
                    Contact temp=addressBook->contacts[j];
                    addressBook->contacts[j]=addressBook->contacts[j+1];
                    addressBook->contacts[j+1]=temp;
                }
            }

        }
        
        printf("Index no\t\tName\t\tPhone\t\tEmail\n");
        for(int i=0;i<addressBook->contactCount;i++){
            printf("%d\t\t%s\t\t%s\t\t%s\n",i+1,addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
        }
        break;
        case 2:printf("Sorting by phone ; \n");
        for(int i=0;i<addressBook->contactCount-1;i++){
            for(int j=0;j<addressBook->contactCount-i-1;j++){
                if(strcmp(addressBook->contacts[j].phone,addressBook->contacts[j+1].phone)>0){
                    Contact temp=addressBook->contacts[j];
                    addressBook->contacts[j]=addressBook->contacts[j+1];
                    addressBook->contacts[j+1]=temp;
                }
            }

        }
        
        printf("Index no\t\tName\t\tPhone\t\tEmail\n");
        for(int i=0;i<addressBook->contactCount;i++){
            printf("%d\t\t%s\t\t%s\t\t%s\n",i+1,addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
        }
        break;
        
        case 3:printf("Sorting by email ; \n");
        for(int i=0;i<addressBook->contactCount-1;i++){
            for(int j=0;j<addressBook->contactCount-i-1;j++){
                if(strcmp(addressBook->contacts[j].email,addressBook->contacts[j+1].email)>0){
                    Contact temp=addressBook->contacts[j];
                    addressBook->contacts[j]=addressBook->contacts[j+1];
                    addressBook->contacts[j+1]=temp;
                }
            }

        }
        
        
        printf("Index no\t\tName\t\tPhone\t\tEmail\n");
        for(int i=0;i<addressBook->contactCount;i++){
            printf("%d\t\t%s\t\t%s\t\t%s\n",i+1,addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
        }
        break;
        default:
        printf("Invalid choice!\n");
        return;

    }
    
}

void initialize(AddressBook *addressBook) {
    // addressBook->contactCount = 0;
    // populateAddressBook(addressBook);
    
    // Load contacts from file during initialization (After files)
    //loadContactsFromFile(addressBook);
    addressBook->contactCount = 0;

    FILE *fp = fopen("contacts.csv", "r");

    if(fp == NULL)
    {
        populateAddressBook(addressBook);
        saveContactsToFile(addressBook);
    }
    else
    {
        fclose(fp);
        loadContactsFromFile(addressBook);
    }
}

void saveAndExit(AddressBook *addressBook) {
    saveContactsToFile(addressBook); // Save contacts to file
    exit(EXIT_SUCCESS); // Exit the program
}

int validName(char name[])
{
    int i;

    if(strlen(name) < 4)
    {
        printf("Name must contain at least 4 characters.\n");
        return 0;
    }

    for(i = 0; name[i] != '\0'; i++)
    {
        if(name[i] != ' ' && !isalpha(name[i]))
        {
            printf("Name should contain only alphabets.\n");
            return 0;
        }
    }

    return 1;
}

int validPhone(char phone[], AddressBook *addressBook, int currentIndex)
{
    int i;

    if(strlen(phone) != 10)
    {
        printf("Phone number must contain exactly 10 digits.\n");
        return 0;
    }

    if(phone[0] < '6' || phone[0] > '9')
    {
        printf("First digit must be between 6 and 9.\n");
        return 0;
    }

    for(i = 0; phone[i] != '\0'; i++)
    {
        if(isalpha(phone[i]))
        {
            printf("Alphabets are not allowed in phone number.\n");
            return 0;
        }

        if(!isdigit(phone[i]))
        {
            printf("Symbols are not allowed in phone number.\n");
            return 0;
        }
    }

    //duplicate phone numbers
    for(i = 0; i < addressBook->contactCount; i++)
    {
        // Skip the current contact while editing
        if(i == currentIndex)
            continue;

        if(strcmp(phone, addressBook->contacts[i].phone) == 0)
        {
            printf("Phone number already exists.\n");
            return 0;
        }
    }

    return 1;
}
int validEmail(char email[], AddressBook *addressBook, int currentIndex)
{
    int i;
    int at = 0, dot = 0;
    int atPos = -1, dotPos = -1;

    // Email should not start with a digit
    if(isdigit(email[0]))
    {
        printf("Email should not start with a digit.\n");
        return 0;
    }

    // Check characters and find @ and .
    for(i = 0; email[i] != '\0'; i++)
    {
        if(email[i] == '@')
        {
            at++;
            atPos = i;
        }
        else if(email[i] == '.')
        {
            dot++;
            dotPos = i;
        }
        else if(!isalnum(email[i]))
        {
            printf("Invalid symbol '%c' in email.\n", email[i]);
            return 0;
        }
    }

    if(at != 1)
    {
        printf("Email must contain exactly one '@'.\n");
        return 0;
    }

    if(dot != 1)
    {
        printf("Email must contain exactly one '.'.\n");
        return 0;
    }

    if(dotPos < atPos)
    {
        printf("'.' must appear after '@'.\n");
        return 0;
    }

    if(dotPos == atPos + 1)
    {
        printf("There must be at least one character between '@' and '.'.\n");
        return 0;
    }

    // Check domain name (after @ and before )
    for(i = atPos + 1; i < dotPos; i++)
    {
        if(!isalpha(email[i]))
        {
            printf("Only alphabets are allowed after '@'.\n");
            return 0;
        }
    }

    // Check extension
    if(email[dotPos + 1] == '\0')
    {
        printf("Domain extension is missing.\n");
        return 0;
    }

    for(i = dotPos + 1; email[i] != '\0'; i++)
    {
        if(!isalpha(email[i]))
        {
            printf("Only alphabets are allowed after '.'.\n");
            return 0;
        }
    }

    // Check duplicate email IDs
    for(i = 0; i < addressBook->contactCount; i++)
    {
        // Skip the current contact while editing
        if(i == currentIndex)
            continue;

        if(strcmp(email, addressBook->contacts[i].email) == 0)
        {
            printf("Email ID already exists.\n");
            return 0;
        }
    }

    return 1;
}
void createContact(AddressBook *addressBook)
{
    Contact newContact;
    int attempt;

    attempt = 0;
    while(1)
    {
        printf("Enter Name : ");
        scanf(" %[^\n]", newContact.name);

        if(validName(newContact.name))
            break;

        attempt++;

        if(attempt == 3)
        {
            printf("Name validation failed.\n");
            return;
        }

        printf("Attempts left : %d\n", 3 - attempt);
    }

    // Phone
    attempt = 0;
    while(1)
    {
        printf("Enter Phone Number : ");
        scanf("%s", newContact.phone);

        if(validPhone(newContact.phone, addressBook, -1))
            break;

        attempt++;

        if(attempt == 3)
        {
            printf("Phone validation failed.\n");
            return;
        }

        printf("Attempts left : %d\n", 3 - attempt);
    }

    // Email
    attempt = 0;
    while(1)
    {
        printf("Enter Email ID : ");
        scanf("%s", newContact.email);

        if(validEmail(newContact.email, addressBook, -1))
            break;

        attempt++;

        if(attempt == 3)
        {
            printf("Email validation failed.\n");
            return;
        }

        printf("Attempts left : %d\n", 3 - attempt);
    }

    addressBook->contacts[addressBook->contactCount] = newContact;
    addressBook->contactCount++;

    printf("\nContact Added Successfully.\n");
}


int searchContact(AddressBook *addressBook)
{
    int choice, i, j, count, select, attempt;
    char key[100], name[100];
    int index[100];
    
    // Keep displaying the search menu until the user exits.
    while(1)
    {
        printf("\n----- Search Contact -----\n");
        printf("1. Search by Name\n");
        printf("2. Search by Phone\n");
        printf("3. Search by Email\n");
        printf("4. Exit\n");
        printf("Enter your choice : ");
        scanf("%d", &choice);

        if(choice == 4)
            return -1;

        switch(choice)
        {
            case 1:

                for(attempt = 1; attempt <= 3; attempt++)
                {
                    printf("Enter Name (or few letters): ");
                    scanf(" %[^\n]", key);

                    /* Convert input to lowercase */
                    for(i = 0; key[i] != '\0'; i++)
                        key[i] = tolower(key[i]);

                    count = 0;

                    printf("\n----------------------------------------------------------\n");
                    printf("No\tName\t\tPhone\t\tEmail\n");
                    printf("----------------------------------------------------------\n");

                    for(i = 0; i < addressBook->contactCount; i++)
                    {
                        strcpy(name, addressBook->contacts[i].name);

                        /* Convert contact name to lowercase */
                        for(j = 0; name[j] != '\0'; j++)
                            name[j] = tolower(name[j]);

                        if(strstr(name, key) != NULL)
                        {
                            printf("%d\t%s\t%s\t%s\n",
                                   count + 1,
                                   addressBook->contacts[i].name,
                                   addressBook->contacts[i].phone,
                                   addressBook->contacts[i].email);
                            // Required because displayed serial numbers (1,2,3...) differ from array indices.
                            index[count] = i;
                            count++;
                        }
                    }

                    if(count > 0)
                    {
                        printf("\nSelect Contact : ");
                        scanf("%d", &select);

                        if(select >= 1 && select <= count)
                            return index[select - 1];
                            // Return the actual index of the selected contact.
                        printf("Invalid Selection.\n");
                        return -1;
                    }

                    printf("Contact Not Found!\n");

                    if(attempt < 3)
                        printf("Attempts Left : %d\n", 3 - attempt);
                }

                printf("Search Failed.\n");
                return -1;

            case 2:

                for(attempt = 1; attempt <= 3; attempt++)
                
                {
                    printf("Enter Phone Number : ");
                    scanf("%s", key);

                    count = 0;

                    for(i = 0; i < addressBook->contactCount; i++)
                    {
                        if(strcmp(addressBook->contacts[i].phone, key) == 0)
                        {
                            printf("\n1. %s\t%s\t%s\n",
                                   addressBook->contacts[i].name,
                                   addressBook->contacts[i].phone,
                                   addressBook->contacts[i].email);

                            return i;
                        }
                    }

                    printf("Contact Not Found!\n");

                    if(attempt < 3)
                        printf("Attempts Left : %d\n", 3 - attempt);
                }

                printf("Search Failed.\n");
                return -1;

            case 3:

                for(attempt = 1; attempt <= 3; attempt++)
                {
                    printf("Enter Email : ");
                    scanf("%s", key);

                    count = 0;

                    for(i = 0; i < addressBook->contactCount; i++)
                    {
                        if(strcmp(addressBook->contacts[i].email, key) == 0)
                        {
                            printf("\n1. %s\t%s\t%s\n",
                                   addressBook->contacts[i].name,
                                   addressBook->contacts[i].phone,
                                   addressBook->contacts[i].email);

                            return i;
                        }
                    }

                    printf("Contact Not Found!\n");

                    if(attempt < 3)
                        printf("Attempts Left : %d\n", 3 - attempt);
                }

                printf("Search Failed.\n");
                return -1;

            default:
                printf("Invalid Choice!\n");
        }
    }
}
void editContact(AddressBook *addressBook)
{
    int index, choice, attempt;
    char name[50];
    char phone[20];
    char email[50];

    index = searchContact(addressBook);

    if(index == -1)
        return;

    while(1)
    {
        printf("\n------ Edit Contact ------\n");
        printf("1. Edit Name\n");
        printf("2. Edit Phone Number\n");
        printf("3. Edit Email ID\n");
        printf("4. Exit\n");
        printf("Enter your choice : ");

        scanf("%d", &choice);

        switch(choice)
        {
            case 1:

                attempt = 0;

                while(1)
                {
                    printf("Enter New Name : ");
                    scanf(" %[^\n]", name);

                    if(validName(name))
                    {
                        strcpy(addressBook->contacts[index].name, name);
                        printf("Name Updated Successfully.\n");
                        return;
                    }

                    attempt++;

                    if(attempt == 3)
                    {
                        printf("Name validation failed.\n");
                        return;
                    }

                    printf("Attempts Left : %d\n", 3 - attempt);
                }

            case 2:

                attempt = 0;

                while(1)
                {
                    printf("Enter New Phone Number : ");
                    scanf("%s", phone);

                    if(validPhone(phone, addressBook, index))
                    {
                        strcpy(addressBook->contacts[index].phone, phone);
                        printf("Phone Number Updated Successfully.\n");
                        return;
                    }

                    attempt++;

                    if(attempt == 3)
                    {
                        printf("Phone validation failed.\n");
                        return;
                    }

                    printf("Attempts Left : %d\n", 3 - attempt);
                }

            case 3:

                attempt = 0;

                while(1)
                {
                    printf("Enter New Email ID : ");
                    scanf("%s", email);

                    if(validEmail(email, addressBook, index))
                    {
                        strcpy(addressBook->contacts[index].email, email);
                        printf("Email ID Updated Successfully.\n");
                        return;
                    }

                    attempt++;

                    if(attempt == 3)
                    {
                        printf("Email validation failed.\n");
                        return;
                    }

                    printf("Attempts Left : %d\n", 3 - attempt);
                }

            case 4:
                printf("Edit Cancelled.\n");
                return;

            default:
                printf("Invalid Choice! Please enter between 1 and 4.\n");
        }
    }
}
void deleteContact(AddressBook *addressBook)
{
    int index, i;
    char ch;

    index = searchContact(addressBook);

    if(index == -1)
        return;

    printf("\nSelected Contact\n");
    printf("Name  : %s\n", addressBook->contacts[index].name);
    printf("Phone : %s\n", addressBook->contacts[index].phone);
    printf("Email : %s\n", addressBook->contacts[index].email);

    printf("\nDo you want to delete this contact? (Y/N) : ");
    scanf(" %c", &ch);

    if(ch == 'Y' || ch == 'y')
    {
        for(i = index; i < addressBook->contactCount - 1; i++)
        {
            addressBook->contacts[i] = addressBook->contacts[i + 1];
        }

        addressBook->contactCount--;

        printf("Contact Deleted Successfully.\n");
    }
    else if(ch == 'N' || ch == 'n')
    {
        printf("Deletion Cancelled.\n");
    }
    else
    {
        printf("Invalid Choice.\n");
    }
}
