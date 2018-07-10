/**
 * @file main.c
 * @author Laiba Mustafa
 * @date May 2018
 * @brief File containing the program
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Htable.h"
#define BASE (256)

/*creates a file and using contents in the table*/
int createFile(HTable *table);

/*adds and element to the table, and uploads it to the file*/
int writeToVault(HTable *table);

/*prints all the content in the file*/
int printVault();

/*displays a list of options and allows user to choose next operation*/
void whatToDo(HTable *table);

/*sets the program password*/
void setMasterPassword(char *string, HTable *table);

/*changes the program password*/
void changeMasterPassword(HTable *table);

/*hashes the key(identifier)*/
int hashFunction(char *identifier, size_t table);

/*frees all allocated memory in the table*/
void destroyData(void *data);

/*prints data*/
void printData(void *toBePrinted);

/*initializes the table to null - prevents a lot of warnings*/
void setTableNull(HTable *table);

/*prints the list of keys that the user can choose from*/
void printTable( HTable *table);

/*puts the contents of the file into the table*/
void populateTable(HTable *table);

/*allows the user to change a password*/
void changePassword(HTable *table);

/*puts the contents of the table into the file*/
void populateFile(HTable *table);

/*allows user to delete a password*/
void deletePassword(HTable *table);

/*prints the list of keys that the user can choose from including the program password*/
void printTableWithoutMP(HTable *table);

/*prints all the content in the table*/
void printT(HTable *table);

/*checks whether user has access to program*/
int authorize(HTable *table);

/*checks whether a file exists*/
int fileExist(char *file);

/*checks user input and whether it is valid or not*/
int userCont();

/*allows user to retrieve a passoword*/
void retrievePassword(HTable *table);

typedef struct vault {
  char key[200];
  char password[200];
} Vault;


int main () {

  char *choice = malloc(200);
  HTable *table = createTable(37, hashFunction, destroyData, printData);
  setTableNull(table);

  do
  {
   printf("\nMenu\n");
   printf("1. Create new vault\n");
   printf("2. Manage existing vault\n");
   printf("q. Exit\n");
   printf("Option selected: ");
   fgets(choice,200, stdin);

   if(choice[0] == '1' && choice[1] == '\n') {
     createFile(table);
   }
   else if(choice[0] == '2' && choice[1] == '\n') {
     whatToDo(table);
   }
   else if(choice[0]== 'q' && choice[1] == '\n'){
     free(choice);
     destroyTable(table);
     return 0;
   }
   else {
     printf("\nInvalid Input. Try Again.\n");
   }
 } while(choice[0] != 'q');

}

int createFile (HTable *table) {

 int check = fileExist("vault.bin");
 int access = 0;
 int response = 0;

 if(check == 1) {
   printf("\nA password vault already exists.\n");
   access = authorize(table);
   if(access == 1) {
     printf("\nIf you create a new file, the contents of the old file will be lost\n");
     printf("Do you wish to continue?\n");
     printf("YES - 'y'\n");
     printf("NO  - 'n'\n");
     response = userCont();
     if(response == 0) {
       return 0;
     }
     else {
       printf("Overwriting old file.\n");
     }
   }
   else {
     printf("\nIncorrect Password. You do not have access to this file\n");
     return 0;
   }
 }

  FILE *fp;

  if((fp = fopen("vault.bin", "wb")) == (FILE*)NULL)
  {
    printf("File was not created!\n");
    exit(1);
  }

  setTableNull(table);
  // OR
  // destroyTable(table);
  // table = createTable(37, hashFunction, destroyData, printData);

  printf("\nSet Program Password\n");

  setMasterPassword("ProgramPassword", table);

  fclose(fp);

  return 0;
}

int writeToVault(HTable *table) {
  Vault *vault;

  char key[200];
  printf("\nKey: ");
  fgets(key,200, stdin);

  if (key[strlen(key)-1] == '\n')
  {
    key[strlen(key)-1] = '\0';
  }

  void *data = lookupData(table, key);

  if(data != NULL) {
    printf("\nA password already exists for this key.\n");
    printf("If you wish to change a password press '2'.\n");
    return 0;
  }

  vault = malloc(sizeof(Vault));

  strcpy(vault->key,key);
  printf("Password: ");
  fgets(vault->password,200, stdin);

  if (vault->password[strlen(vault->password)-1] == '\n')
  {
    vault->password[strlen(vault->password)-1] = '\0';
  }

  insertData(table, vault->key, (void *)vault); //or key?
  populateFile(table);

  printf("\nKey Added.\n");

  return 0;
}

int printVault() {
  FILE *fp;

  Vault *vault;

  vault = malloc(sizeof(Vault));

  fp = fopen("vault.bin", "rb+");

  while(fread(vault,sizeof(Vault),1,fp))
  {
    printf("Key: %s\n", vault->key);
    printf("Password: %s\n", vault->password);
  }
  fclose(fp);

  return 0;
}

void whatToDo(HTable *table) {

  int check = fileExist("vault.bin");

  if(check == 0) {
    printf("\nA password vault does not exist.\n");
    printf("To create a vault select '1'\n");
    return;
  }

  printf("\n");
  populateTable(table);

  int allowed = authorize(table);

  if(allowed == 0) {
    printf("\nIncorrect Password. You do not have access to this file\n");
    return;
  }

  char *choice = malloc(200);

  do
  {
    printf("\nWhat would you like to do: \n");
    printf("1. Add new password\n");
    printf("2. Change existing password\n");
    printf("3. Change Program Password\n");
    printf("4. Retrieve a password\n");
    printf("5. Delete a password\n");
    printf("q. Exit\n");
    printf("Option selected: ");
    fgets(choice,200, stdin);

   if(choice[0] == '1' && choice[1] == '\n') {
     writeToVault(table);
   }
   else if(choice[0] == '2' && choice[1] == '\n') {
     changePassword(table);
   }
   else if(choice[0]== '3' && choice[1] == '\n'){
     printf("\nChange Program Password\n");
     changeMasterPassword(table); //DNW
   }
   else if(choice[0]== '4' && choice[1] == '\n'){
     retrievePassword(table);
   }
   else if(choice[0]== '5' && choice[1] == '\n'){
     deletePassword(table);
   }
   else if(choice[0]== 'q' && choice[1] == '\n'){
     free(choice);
     return;
   }
   else {
     printf("\nInvalid Input. Try Again.\n");
   }
  } while(choice[0] != 'q');

}

void setMasterPassword(char *string, HTable *table) {

  printf("Enter password: ");

  Vault *vault = malloc(sizeof(Vault));
  fgets(vault->password,200, stdin);

  if (vault->password[strlen(vault->password)-1] == '\n')
  {
    vault->password[strlen(vault->password)-1] = '\0';
  }

  strcpy(vault->key,string);

  insertData(table, string, (void *)vault); //puts MP in table

  populateFile(table);

  return;
}

void changeMasterPassword(HTable *table) {

  removeData(table, "ProgramPassword");
  populateFile(table);

  printf("Enter new password: ");

  Vault *vault = malloc(sizeof(Vault));
  fgets(vault->password,200, stdin);

  if (vault->password[strlen(vault->password)-1] == '\n')
  {
    vault->password[strlen(vault->password)-1] = '\0';
  }

  strcpy(vault->key,"ProgramPassword");

  insertData(table, "ProgramPassword", (void *)vault);

  populateFile(table);

  return;
}

int hashFunction(char *identifier, size_t table) {
    size_t hash;
    unsigned const char *copy;

    /* cast s to unsigned const char * */
    /* this ensures that elements of s will be treated as having values >= 0 */
    copy = (unsigned const char *) identifier;

    hash = 0;
    while(*copy != '\0') { // need !=?
        hash = (hash * BASE + *copy) % table;
        copy++;
    }
    return hash;
}

void destroyData(void *data) {
  Vault *vault = (Vault *)data;
  free(vault);
  return;
}

void printData(void *toBePrinted) {
  return;
}

void setTableNull(HTable *table) {
  if(table == NULL) {return;}

  for(int i = 0; i < table->size; i++) {
    table->table[i] = NULL;
  }
}

void printTable( HTable *table) {
  Node *node = NULL;
  int i = 0;
  Vault *vault;

  while(i < 37) {
    node = table->table[i];
    //if(node) {
      while(node) {
        vault = (Vault *)node->data;
        printf("KEY: %s\n",vault->key);
        node = node->next;
      }
    //}
    i++;
  }
  printf("\n");
}

void populateTable(HTable *table) {
  FILE *fp;

  Vault vault;
  Vault *v2 = malloc(sizeof(Vault));

  fp = fopen("vault.bin","rb");

    while(fread(&vault,sizeof(Vault),1,fp)) {
      memcpy(v2, &vault,sizeof(Vault));
      insertData(table, vault.key, (void *)v2);
      v2 = malloc(sizeof(Vault));
    }

  fclose(fp);

}

void changePassword(HTable *table) {
  printf("\nList of keys currently in vault:\n");
  printTableWithoutMP(table);
  printf("Enter key you want to change: ");

  char toChange[200];
  fgets(toChange,200, stdin);

  if (toChange[strlen(toChange)-1] == '\n')
  {
    toChange[strlen(toChange)-1] = '\0';
  }

  if(strcmp(toChange,"ProgramPassword") == 0) { //Just in case
    printf("Cannot change Program Password\n");
    return;
  }
  void *data = lookupData(table, toChange);

  if(data == NULL) {
    printf("\nKey does not exist.\n");
    return;
  }

  removeData(table, toChange);

  printf("Enter new password: ");

  Vault *vault = malloc(sizeof(Vault));
  fgets(vault->password,200, stdin);

  if (vault->password[strlen(vault->password)-1] == '\n')
  {
    vault->password[strlen(vault->password)-1] = '\0';
  }

  strcpy(vault->key,toChange);

  insertData(table, toChange, (void *)vault);

  populateFile(table);

}

void populateFile(HTable *table) {
  FILE *fp;

  fp = fopen("vault.bin", "wb");

  Node *node = NULL;
  int i = 0;
  Vault *vault;

  while(i < 37) {
    node = table->table[i];
    if(node) { //necessary
      while(node) {
        vault = (Vault *)node->data;
        node = node->next;
        fwrite(vault,sizeof(Vault),1,fp);
      }
    }
    i++;
  }
  fclose(fp);
}

void deletePassword(HTable *table) {
  printf("\nList of keys currently in vault:\n");
  printTableWithoutMP(table);
  printf("Enter key you want to delete: ");

  char toChange[200];
  fgets(toChange,200, stdin);

  if (toChange[strlen(toChange)-1] == '\n')
  {
    toChange[strlen(toChange)-1] = '\0';
  }

  if(strcmp(toChange,"ProgramPassword") == 0) {
    printf("Cannot delete Program Password\n");
    return;
  }

  Vault *check = (Vault *)lookupData(table, toChange);
  if(check == NULL) {
    printf("\nThis key does not exist in vault.\n");
  } else {
    printf("\nDeleted.\n");
    removeData(table, toChange);
  }

  populateFile(table);
}

void printTableWithoutMP(HTable *table) {
  Node *node = NULL;
  int i = 0;
  Vault *vault;

  while(i < 37) { //for loop did not work with valgrind why??
    node = table->table[i];
      while(node) {
        vault = (Vault *)node->data;
        if(strcmp(vault->key, "ProgramPassword") != 0) {
          printf("KEY: %s\n",vault->key);
        }
        node = node->next;
      }
    i++;
  }
  printf("\n");
}

void printT( HTable *table) {
  populateTable(table);
  Node *node = NULL;
  int i = 0;
  Vault *vault;

  while(i < 37) {
    node = table->table[i];
    if(node) {
      while(node) {
        vault = (Vault *)node->data;
        printf("KEY: %s\n",vault->key);
        printf("PASSWORD: %s\n",vault->password);
        node = node->next;
      }
    }
    i++;
  }
}

int authorize(HTable *table) {

  populateTable(table);

  printf("Enter Program Password: ");

  char password[200];
  fgets(password,200, stdin);

  if (password[strlen(password)-1] == '\n')
  {
    password[strlen(password)-1] = '\0';
  }

  Vault *vault = (Vault *)lookupData(table, "ProgramPassword");

  if(strcmp(vault->password, password) == 0) {
    return 1;
  }

  return 0;
}

int fileExist(char *file) {
  FILE *fp;

  if((fp = fopen(file, "rb"))) {
    fclose(fp);
    return 1;
  }
  return 0;
}

int userCont() {
  printf("Input: ");

  char ans[200];
  fgets(ans,200, stdin);

 do {
    if(ans[0] == 'y' && ans[1] == '\n') {
      return 1;
    }
    else if(ans[0] == 'n' && ans[1] == '\n') {
      return 0;
    }
    else {
      printf("\nInvalid Input. Try Again.\n");
      printf("Input: ");
      fgets(ans,200, stdin);
    }
  } while(ans[0] != 'n');

  return 0;
}

void retrievePassword(HTable *table) {
  printf("\nList of keys currently in vault:\n");
  printTable(table); //
  printf("Enter key whose password you want to retrieve: ");

  char toRetrieve[200];
  fgets(toRetrieve,200, stdin);

  if (toRetrieve[strlen(toRetrieve)-1] == '\n')
  {
    toRetrieve[strlen(toRetrieve)-1] = '\0';
  }

  void *data = lookupData(table, toRetrieve);

  if(data == NULL) {
    printf("\nKey does not exist.\n");
    return;
  }

  Vault *save = (Vault*)data;
  printf("Password: %s\n",save->password);

}
