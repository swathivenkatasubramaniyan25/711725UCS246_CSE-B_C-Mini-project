#include <stdio.h>
#include <stdlib.h>

struct clientData
{
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
};

// prototypes
unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
void initializeFile(FILE *fPtr);
void displayAll(FILE *fPtr);     // NEW
void searchAccount(FILE *fPtr);  // NEW

int main()
{
    FILE *cfPtr;

    // open or create file
    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL)
    {
        cfPtr = fopen("credit.dat", "wb+");

        if (cfPtr == NULL)
        {
            printf("File could not be created.\n");
            exit(1);
        }

        printf("New file created.\n");
        initializeFile(cfPtr);
    }

    unsigned int choice;

    while ((choice = enterChoice()) != 7)
    {
        switch (choice)
        {
        case 1:
            textFile(cfPtr);
            break;
        case 2:
            updateRecord(cfPtr);
            break;
        case 3:
            newRecord(cfPtr);
            break;
        case 4:
            deleteRecord(cfPtr);
            break;
        case 5:
            displayAll(cfPtr);
            break;
        case 6:
            searchAccount(cfPtr);
            break;
        default:
            printf("Invalid choice. Try again.\n");
        }
    }

    fclose(cfPtr);
    return 0;
}

// initialize file
void initializeFile(FILE *fPtr)
{
    struct clientData blank = {0, "", "", 0.0};

    for (int i = 0; i < 100; i++)
    {
        fwrite(&blank, sizeof(struct clientData), 1, fPtr);
    }
}

// create text file
void textFile(FILE *readPtr)
{
    FILE *writePtr;
    struct clientData client;

    if ((writePtr = fopen("accounts.txt", "w")) == NULL)
    {
        printf("File could not be opened.\n");
        return;
    }

    rewind(readPtr);

    fprintf(writePtr, "%-6s%-16s%-11s%10s\n",
            "Acct", "Last Name", "First Name", "Balance");

    while (fread(&client, sizeof(struct clientData), 1, readPtr) == 1)
    {
        if (client.acctNum != 0)
        {
            fprintf(writePtr, "%-6u%-16s%-11s%10.2f\n",
                    client.acctNum,
                    client.lastName,
                    client.firstName,
                    client.balance);
        }
    }

    fclose(writePtr);
    printf("accounts.txt created successfully.\n");
}

// update record
void updateRecord(FILE *fPtr)
{
    unsigned int account;
    double transaction;
    struct clientData client;

    printf("Enter account number (1-100): ");
    scanf("%u", &account);

    if (account < 1 || account > 100)
    {
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
        return;
    }

    printf("Current Balance: %.2f\n", client.balance);
    printf("Enter amount (+ deposit / - withdraw): ");
    scanf("%lf", &transaction);

    client.balance += transaction;

    fseek(fPtr, -(long)sizeof(struct clientData), SEEK_CUR);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Balance updated successfully.\n");
}

// delete record
void deleteRecord(FILE *fPtr)
{
    struct clientData client;
    struct clientData blank = {0, "", "", 0.0};
    unsigned int account;

    printf("Enter account number to delete (1-100): ");
    scanf("%u", &account);

    if (account < 1 || account > 100)
    {
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account does not exist.\n");
        return;
    }

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&blank, sizeof(struct clientData), 1, fPtr);

    printf("Account deleted successfully.\n");
}

// create record
void newRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};
    unsigned int account;

    printf("Enter new account number (1-100): ");
    scanf("%u", &account);

    if (account < 1 || account > 100)
    {
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0)
    {
        printf("Account already exists.\n");
        return;
    }

    printf("Enter last name, first name, balance:\n");
    scanf("%14s %9s %lf",
          client.lastName,
          client.firstName,
          &client.balance);

    client.acctNum = account;

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Account created successfully.\n");
}

// NEW: display all accounts
void displayAll(FILE *fPtr)
{
    struct clientData client;

    rewind(fPtr);

    printf("\n--- All Accounts ---\n");

    while (fread(&client, sizeof(struct clientData), 1, fPtr) == 1)
    {
        if (client.acctNum != 0)
        {
            printf("Acc: %u | %s %s | Balance: %.2f\n",
                   client.acctNum,
                   client.firstName,
                   client.lastName,
                   client.balance);
        }
    }
}

// NEW: search account
void searchAccount(FILE *fPtr)
{
    struct clientData client;
    unsigned int account;

    printf("Enter account number to search: ");
    scanf("%u", &account);

    if (account < 1 || account > 100)
    {
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
    }
    else
    {
        printf("\nAccount Found:\n");
        printf("Account No: %u\n", client.acctNum);
        printf("Name: %s %s\n", client.firstName, client.lastName);
        printf("Balance: %.2f\n", client.balance);
    }
}

// menu
unsigned int enterChoice(void)
{
    unsigned int choice;

    printf("\n===== BANK MENU =====\n");
    printf("1 - Export to text file\n");
    printf("2 - Update account\n");
    printf("3 - Add account\n");
    printf("4 - Delete account\n");
    printf("5 - View all accounts\n");
    printf("6 - Search account\n");
    printf("7 - Exit\n");
    printf("Enter choice: ");

    scanf("%u", &choice);
    return choice;
}
