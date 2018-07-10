/**
 * @file main.c
 * @author Laiba Mustafa
 * @date May 2018
 * @brief File containing the function implementations for Htable
 */

#include "Htable.h"

HTable *createTable(size_t size,
  int (*hashFunction)(char *identifier, size_t table),
  void (*destroyData)(void *data),
  void (*printData)(void *toBePrinted))
{

  if(hashFunction == NULL || destroyData == NULL || printData == NULL || size == 0) {
    return NULL;
  }

  HTable *initial = malloc(sizeof(HTable));
  initial->table = malloc((sizeof(Node *))*size); //automatically initialized to NULL?
  initial->hashFunction = hashFunction;
  initial->destroyData = destroyData;
  initial->printData = printData;
  initial->size = size;

  return initial;
}

Node* createNode(char *key, void* data ) {
  Node *initial = malloc(sizeof(Node));
  initial->key = malloc(20);
  initial->next = NULL;
  strcpy(initial->key, key);
  initial->data = data;
  return initial;
}

void destroyTable(HTable *hashTable)  {
  int i;
  Node *tempNode, *tempDeleteNode;

  if(hashTable == NULL) {
    return;
  }

  for(i = 0; i < hashTable->size; i++) {
    tempNode = hashTable->table[i];

      while(tempNode) {
        tempDeleteNode = tempNode;
        tempNode = tempNode->next;
        hashTable->destroyData(tempDeleteNode->data);
        free(tempDeleteNode);
      }
  }
  free(hashTable->table);
  free(hashTable);
}

void insertData(HTable *hashTable, char *key, void *data) {

  int index;
  Node *node, *tempNode;

  if(hashTable == NULL) {
    return;
  }
  node = createNode(key, data);
  index = hashTable->hashFunction(key, hashTable->size);

  if(hashTable->table[index]) {

    if(strcmp((hashTable->table[index])->key, key) == 0) {
      hashTable->destroyData(hashTable->table[index]->data);
      (hashTable->table[index])->data = data;
      free(node);
    }
    else {
      // node = createNode(key, data);
      tempNode = hashTable->table[index];
      while(tempNode->next) {
        tempNode = tempNode->next;
        printf("%s\n",node->key);
      }
      tempNode->next = node;
    }
  }
  else {
    // node = createNode(key, data);
    hashTable->table[index] = node;
  }
}

void removeData(HTable *hashTable, char *key){

  int index;
  Node *node, *prev;

  if(hashTable == NULL) {
    return;
  }

    index = hashTable->hashFunction(key, hashTable->size);
    node = hashTable->table[index];
    prev = NULL;

    if(hashTable->table[index]) {

    while(strcmp(node->key,key) != 0)  {
      prev = node;
      node = node->next;
      if(node == NULL) { //key not in table
        return;
      }
    }

    if(strcmp(node->key,key) == 0) {
      if(prev == NULL) {
        (hashTable->table[index]) = node->next;
        hashTable->destroyData(node->data); // I added this
        node->data = NULL; // this too, necessary?
        free(node);
      }
      else {
        prev->next = node->next;
        hashTable->destroyData(node->data);
        free(node);
      }
    }
  }
}


void *lookupData(HTable *hashTable, char *key) {

  int index = 0;
  Node *tempNode;

  if(hashTable == NULL) {
    return NULL;
  }

  if(hashTable) {
    index = hashTable->hashFunction(key, hashTable->size);

    if(hashTable->table[index] == NULL) {
      return NULL;
    }
    tempNode = hashTable->table[index];
    while (tempNode) {
      if(strcmp(tempNode->key,key) == 0)  {
        return tempNode->data;
      }
      tempNode = tempNode->next;
    }
  }
  return NULL;
}
