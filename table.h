#ifndef TABLE_H
#define TABLE_H

typedef enum TableErrors {
    OK,
    DUPLICATE_KEY,
    KEY_NOT_FOUND,
    MEMORY_LOCK,
    NO_PARENT,
    END
} TableErrors;

typedef unsigned int InfoType;

typedef unsigned int KeyType;

typedef unsigned int IndexType;

typedef struct KeySpace {
    KeyType key;
    KeyType par;
    InfoType *info;
    struct KeySpace *next;
} KeySpace;


typedef struct Table {
    KeySpace *ks;
    // IndexType msize;
    // IndexType csize;
} Table;

//individual task
int findParent(Table *table, KeyType value);

void search_by_parent(Table *table, Table *newTable, KeyType value);

void recursive_delete_helper(Table *table, KeyType key);

void recursive_delete(Table *table, KeyType key);

//table struct functions
enum TableErrors free_table(Table *table);

Table *createTable();

enum TableErrors insertElement(Table *table, KeyType key, KeyType par, InfoType *info);

enum TableErrors deleteElement(Table *table, KeyType key);

void printTable(Table *table);

enum TableErrors findElement(Table *table, KeyType key);

//import from file
enum TableErrors importDataFromFile(Table *table, const char *filename);

#endif