#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define MAX_PASSWORD_LENGTH 50
#define FILENAME "userdata.dat"
#define TRANSFER_FILE "transferdata.dat"

struct User {
    char username[50];
    char password[MAX_PASSWORD_LENGTH];
    int date, month, year;
    char pnumber[15];
    char adharnum[20];
    char fname[20];
    char lname[20];
    char fathname[20];
    char mothname[20];
    char address[50];
    char typeaccount[20];
};

struct Money {
    char usernameto[50];
    char userpersonfrom[50];
    long int amount;
};

void getPassword(char *password);
void createAccount();
void accountCreated();
void login();
void loggedIn(char username[]);
void displayUserInfo(char username[]);
void checkBalance(char username[]);
void transferMoney(char username[]);
void logout();

int main() {

    int choice;

    system("clear");
    printf("WELCOME TO BANK ACCOUNT SYSTEM\n");
    printf("1. Create a bank account\n");
    printf("2. Already a user? Sign in\n");
    printf("3. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch(choice) {
        case 1:
            createAccount();
            break;
        case 2:
            char ch = '\n';
            while (ch == '\n') {
                login();
                printf("Press enter to continue...");
                ch = getchar();
                ch = getchar();
            }
            break;
        case 3:
            exit(0);
        default:
            printf("Invalid choice!\n");
            break;
    }

    return 0;
}

void getPassword(char *password) {

	struct termios old, new;
	tcgetattr(fileno(stdin), &old);

	new = old;
	new.c_lflag &= ~(ECHO | ICANON);

	printf("Enter password: ");
	fflush(stdout);

	tcsetattr(fileno(stdin), TCSANOW, &new);

	int i = 0;
	char ch;
	ch = getchar();
	while (1) {
        ch = getchar();

		if (ch == '\n' || ch == '\r') {
			password[i] = '\0';
			printf("\n");
			break;
		}

		if (i < MAX_PASSWORD_LENGTH - 1) {
			password[i++] = ch;
			putchar('*');
			fflush(stdout);
		}
	}

	tcsetattr(fileno(stdin), TCSANOW, &old);
}

void createAccount() {
    struct User user;
    FILE *file = fopen(FILENAME, "ab");
    system("clear");
    
    printf("CREATE ACCOUNT\n");
    printf("==================\n");
    printf("Enter your username: ");
    scanf("%s", user.username);

    getPassword(user.password);
    printf("%s\n", user.password);

    printf("Enter your first name: ");
    scanf("%s", user.fname);

    printf("Enter your last name: ");
    scanf("%s", user.lname);

    printf("Enter your phone number: ");
    scanf("%s", user.pnumber);

    printf("Enter your adhar number: ");
    scanf("%s", user.adharnum);

    printf("Enter your Birth date: ");
    printf("Enter date: ");
    scanf("%d", &user.date);
    printf("Enter month: ");
    scanf("%d", &user.month);
    printf("Enter year: ");
    scanf("%d", &user.year);

    printf("Enter your father name: ");
    scanf("%s", user.fathname);

    printf("Enter your mother name: ");
    scanf("%s", user.mothname);

    printf("Enter your address: ");
    scanf("%s", user.address);

    printf("Enter your account type: ");
    scanf("%s", user.typeaccount);

    fwrite(&user, sizeof(struct User), 1, file);
    fclose(file);

    accountCreated();
}

void accountCreated() {
    int i;
	char ch;
	system("clear");
	printf(
		"PLEASE WAIT....\n\nYOUR DATA IS PROCESSING....");
	for (i = 0; i < 200000000; i++) {
		i++;
		i--;
	}
    printf("Account created successfully.\n\n\n\n\n");
    main();
}

void login() {
    char username[50];
    struct User user;
    FILE *file = fopen(FILENAME, "rb");

    system("clear");
    printf("USER LOGIN\n");
    printf("================\n");
    printf("Enter your username: ");
    scanf("%s", username);
    
    char pwd[50];
    // Check if username exists in the file
    while (fread(&user, sizeof(struct User), 1, file)) {
        if (strcmp(username, user.username) == 0) {
            do {
                getPassword(pwd);
            } while (strcmp(pwd, user.password) != 0);
            fclose(file);
            char ch = '\n';
            while (1) {
                if (ch == '\n') {
                    loggedIn(username);
                    printf("Press enter to continue...");
                    ch = getchar();
                    ch = getchar();
                }
            }
            return;
        }
    }

    fclose(file);
    printf("User not found!\n");
}

void loggedIn(char username[]) {
    int choice;
    system("clear");
    printf("\n\n\n\n");
    printf("================\n");
    printf("Logged in as %s\n", username);
    printf("1. Display user info\n");
    printf("2. Check balance\n");
    printf("3. Transfer money\n");
    printf("4. Logout\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        displayUserInfo(username);
        break;
    case 2:
        checkBalance(username);
        break;
    case 3:
        transferMoney(username);
        break;
    case 4:
        logout(username);
        break;
    default:
        printf("Invalid choice!\n");
        break;
    }
}

void displayUserInfo(char username[]) {
    struct User user;
    FILE *file = fopen(FILENAME, "rb");

    system("clear");
    printf("USER INFORMATION\n");
    printf("================\n");

    while (fread(&user, sizeof(struct User), 1, file)) {
        if (strcmp(username, user.username) == 0) {
            printf("Username: %s\n", user.username);
            printf("Holder's name: %s %s\n", user.fname, user.lname);
            printf("Phone number: %s\n", user.pnumber);
            printf("Adhar number: %s\n", user.adharnum);
            printf("Birth date: %2d-%2d-%4d", user.date, user.month, user.year);
            printf("Father name: %s\n", user.fathname);
            printf("Mother name: %s\n", user.mothname);
            printf("Address: %s\n", user.address);
            printf("Account type: %s\n", user.typeaccount);
            printf("====================================\n");
            fclose(file);
            return;
        }
    }

    fclose(file);
    printf("User not found!\n");
}

void checkBalance(char username[]) {
    FILE  *transferFile = fopen(TRANSFER_FILE, "rb");
    struct Money transfer;
    long int balance = 0;

    system("clear");
    printf("CHECK BALANCE\n");
    printf("================\n");

    if (transferFile == NULL) {
        printf("Error opening transfer file.\n");
        return;
    }

    printf("Transactions for user %s:\n", username);
    printf("--------------------------------\n");

    while (fread(&transfer, sizeof(struct Money), 1, transferFile)) {
        if (strcmp(username, transfer.usernameto) == 0) {
            balance += transfer.amount;
        }
        if (strcmp(username, transfer.userpersonfrom) == 0) {
            balance -= transfer.amount;
        }

    }

    fclose(transferFile);

    printf("Current balance for user %s: %ld\n", username, balance);

}

void transferMoney(char sender[]) {
    struct Money transfer;
    FILE *transferFile = fopen(TRANSFER_FILE, "ab");

    system("clear");
    printf("TRANSFER MONEY\n");
    printf("================\n");

    if (transferFile == NULL) {
        printf("Error opening transfer file.\n");
        return;
    }
    
    printf("Enter receiver's username: " );
    scanf("%s", transfer.usernameto);

    strcpy(transfer.userpersonfrom, sender);

    printf("Enter the amount to transfer: ");
    scanf("%ld", &transfer.amount);

    fwrite(&transfer, sizeof(struct Money), 1, transferFile);
    fclose(transferFile);

    printf("Amount transferred successfully from %s to %s.\n", sender, transfer.usernameto);
}

void logout() {
    system("clear");
    printf("LOGOUT\n");
    printf("================\n");
    printf("Logged out successfully!\n");
    main(); // Go back to the main menu
}
