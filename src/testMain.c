/**
 * @file test.h
 * @author Laiba Mustafa
 * @date May 2018
 * @brief File containing the function implementations for testing
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include"test.h"
#include "Htable.h"
#include "assert.h"
#define BASE (256)

typedef struct information {
  char name[50];
  int num;
}Info;

int hashFunction(char *identifier, size_t table);
void destroyData(void *data);
void printData(void *toBePrinted);
void printTable(HTable *table);
void *new_data(char*name, int num);
void received_false();
void received_true();
void set_table_to_null(HTable *table);
Info* cast_this(Node *node);
int hashValue(char*string);

int testNumber = 1;
int testsPassed = 0;

int main () {
  /****************************** Testing section ******************************/
  printStatement("Start Testing Section");

  printf("\nTesting Hash Function Using Most Commom Keys:\n");

  int values[20] = {0};
  char *strings[20] = {"ProgramPassword", "hotmail", "gmail",
                       "youtube", "facebook", "reddit", "stack overflow",
                       "twitter", "yahoo", "courselink", "scotia bank",
                       "rbc", "pinterest", "yelp", "quora", "snapchat",
                       "instagram", "whatsapp", "tumblr", "paypal"};

  int i = 0;
  int j = 0;
  int k = 0;
  int l = 0;
  int sum = 0;

  while(i < 20) {
    values[i] = hashValue(strings[i]);
    i++;
  }
  printf("HASHED VALUES: ");

  while(l < 20) {
    printf("%d ",values[l]);
    l++;
  }


  for(k = 0; k < 20; k++)
    {
        for(j = k + 1; j < 20; j++)
        {
            if(values[k] == values[j])
            {
                sum++;
            }
        }
    }

    printf("\n");
    printf("Number of items that match: %d\n",sum);
    printf("Entered 20 different keys and only two match.\n\n");
    printStatement("*");

    /*TEST #1: Testing createTable with an invalid parameter*/
     HTable *table = createTable(37, NULL,destroyData,printData);
     set_table_to_null(table);
     printTestIntroInfo(testNumber++, "Testing createTable with NULL parameter");
     printf("Expected:%d\n",1);
     if(table == NULL) {
       received_true();
     } else {received_false();}
     printStatement("*");

     /*TEST #2: Testing createTable with an invalid parameter*/
    table = createTable(0, hashFunction,destroyData,printData);
    set_table_to_null(table);
    printTestIntroInfo(testNumber++, "Testing createTable with size 0");
    printf("Expected:%d\n",1);
    if(table == NULL) {
        received_true();
    } else {received_false();}
    destroyTable(table);
    printStatement("*");

    /*TEST #3: Testing insertData with a key whose hash value is known*/
    table = createTable(37, hashFunction,destroyData,printData);
    set_table_to_null(table);
    printTestIntroInfo(testNumber++, "Testing insertData with data at table[0]");
    insertData(table,"bmo", new_data("name1", 10)); //index 0
    printf("Expected:%d\n",1);
    if(((Info *)((table->table[0])->data))->num == 10) {
       received_true();
    }else{received_false();}
    printStatement("*");

    /*TEST #4: Testing insertData by creating a chain*/
    printTestIntroInfo(testNumber++, "Creating a chain at table[12]");
    insertData(table,"abc", new_data("name2", 20)); //index 12
    insertData(table,"vvv", new_data("name3", 30)); //index 12
    printf("Expected:%d\n",1);
    Info* info1 = cast_this((table->table[12]));
    Info* info2 = cast_this((table->table[12])->next);
    assert((table->table[12])->next->next == NULL);
    if(info1->num == 20 && info2->num == 30) {
        received_true();
    }else{received_false();}
    printStatement("*");

    /*TEST #5: "Testing lookupData by looking up data"*/
    printTestIntroInfo(testNumber++, "Testing lookupData by looking up data");
    insertData(table,"hello", new_data("name2", 220));
    Info *look1 = (Info *)lookupData(table,"hello");
    printf("Expected:%d\n",1);
    if(look1->num == 220) {
        received_true();
    }else{received_false();}
    printStatement("*");

    /*TEST #6: "Testing lookupData by looking up data in the middle of chain"*/
    printTestIntroInfo(testNumber++, "Testing lookupData by looking up data in the middle of chain");
    insertData(table,"wwwabaaa", new_data("name1", 10)); //index 15
    insertData(table,"wwwabaa", new_data("name2", 20)); //index 15
    insertData(table,"wwwaba", new_data("name3", 30)); //index 15
    Info *look2 = (Info *)lookupData(table,"wwwabaa");
    printf("Expected:%d\n",1);
    if(look2->num == 20) {
      received_true();
    }else{received_false();}
    printStatement("*");

    /*TEST #7: "Testing lookupData by looking up data that DNE"*/
    printTestIntroInfo(testNumber++, "Testing lookupData by looking up data that DNE");
    look2 = (Info *)lookupData(table,"DNE"); // index 3
    printf("Expected:%d\n",1);
    if(look2 == NULL) {
      received_true();
    }else{received_false();}
    printStatement("*");

    /*TEST #8: "Removing data from the middle of the chain"*/
    printTestIntroInfo(testNumber++, "Removing data from the middle of the chain");
    printf("Expected:%d\n",1);
    removeData(table, "wwwabaa");
    info1 = cast_this((table->table[15]));
    info2 = cast_this((table->table[15])->next);
    if(info1->num == 10 && info2->num == 30) {
       received_true();
    }else{received_false();}
    printStatement("*");

    /*TEST #9: "Removing data from the front of the chain"*/
    printTestIntroInfo(testNumber++, "Removing data from the front of the chain");
    printf("Expected:%d\n",1);
    removeData(table, "wwwabaaa");
    info1 = cast_this((table->table[15]));
    if(info1->num == 30) {
      received_true();
    }else{received_false();}
    printStatement("*");

    /*TEST #10: "Replacing data at the front"*/
    printTestIntroInfo(testNumber++, "Replacing data at the front");
    printf("Expected:%d\n",1);
    insertData(table, "wwwaba", new_data("hello",99));
    info1 = cast_this((table->table[15]));
    if(info1->num == 99) {
      received_true();
    }else{received_false();}
    printStatement("*");

    /*TEST #11: "Testing removeData by removing data that DNE"*/
    printTestIntroInfo(testNumber++, "Testing removeData by removing data that DNE");
    removeData(table, "this"); //index
    printf("Expected:%d\n",1);
    if(table->table[9] == NULL) {
      received_true();
    }else{received_false();}
    destroyTable(table); // table using destoyed
    printStatement("*");


  //Note: destroyTable was checked with valgrind

  /****************************** End Testing ******************************/
  printf("\n");
	printStatement("Ending Testing Section");
  printf("Number of tests passed: %d\n",testsPassed);


 return 0;
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
  Info *info = (Info *)data;
  free(info);
  return;
}

void printData(void *toBePrinted) {
  return;
}

void received_true () {
  printf("Received:%d\n", 1);
  testsPassed += printPassFail(1);
}

void received_false() {
  printf("Received:%d\n", 0);
  testsPassed += printPassFail(0);
}

void set_table_to_null(HTable *table) {
  if(table == NULL) {return;}

  for(int i = 0; i < table->size; i++) {
    table->table[i] = NULL;
  }
}

int hashValue(char*string) {
  int index = hashFunction(string,37);
  return index;
}

Info* cast_this(Node *node) {
  Info *info = (Info *)node->data;
  return info;
}

void *new_data(char*name, int num) {
  Info *new = malloc(sizeof(Info));
  strcpy(new->name, name);
  new->num = num;
  void *data = (void *)new;
  return data;
}
