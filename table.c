#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table.h"
#include "io.h"

int findParent(Table *table, KeyType value) {
    KeySpace *current = table->ks;
    while (current != NULL) {
        if (current->key == value) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

enum TableErrors free_table(Table *table) {

    if (table->ks == NULL || table == NULL) {
        free(table);
        return OK;
    }

    KeySpace *current = table->ks;
    KeySpace *next = NULL;

    while (current != NULL) {
        next = current->next;
        free(current->info);
        free(current);
        current = next;
    }
    free(table);
    return OK;
}


Table *createTable() {
    Table *table = (Table *) calloc(1, sizeof(Table));

    if (table == NULL) {
        return NULL;
    }
    table->ks = NULL;
    return table;
}


enum TableErrors insertElement(Table *table, KeyType key, KeyType par, InfoType *info) {  

    KeySpace *newElement = (KeySpace*) calloc(1, sizeof(KeySpace));

    if (newElement == NULL) {
        return MEMORY_LOCK;
    }

    if (table->ks == NULL) {

        if (par != 0 && par != key) {
            printf("Такого родительского элемента в таблице нет \n");
            free(newElement);
            return NO_PARENT;
        }
        if (par == 0) {
            newElement->par = 0;
        } else  {
            newElement->par = key;
        }
        table->ks = newElement;
        newElement->next = NULL;

    } else {
        KeySpace *current = table->ks;

        while (current != NULL) {
            if (current->key == key) {
                printf("Ключ со значением %u уже существует\n", current->key);
                free(newElement);
                return DUPLICATE_KEY;
            }
            current = current->next;
        }

        if (par == 0 || par == key) {
            if (par == 0) {
                newElement->par = 0;
            }
            if (par == key) {
                newElement->par = key;
            } 
        } else { 
            int res = findParent(table, par);
            if (res == 1) {
                newElement->par = par;
            } else {
                printf("Такого родительского элемента в таблице нет \n");
                free(newElement);
                return NO_PARENT;
            }
        }
        newElement->next = table->ks;
        table->ks = newElement;
    }

    newElement->key = key;
    newElement->info = (InfoType*) calloc(1, sizeof(InfoType));

    if (newElement->info == NULL) {
        free(newElement);
        return MEMORY_LOCK;
    }

    memcpy(newElement->info, info, sizeof(InfoType));
    return OK;
}

void search_by_parent(Table *table, Table *newTable, KeyType value) {
    KeySpace *current = table->ks;
    while (current != NULL) {
        if (current->par == value) {
            TableErrors k = insertElement(newTable, current->key, 0, current->info);
            if (k == MEMORY_LOCK) {
                free_table(table);
                return;
            }
        }
        current = current->next;
    }
}


enum TableErrors deleteElement(Table *table, KeyType key) {
    KeySpace *current = table->ks;
    KeySpace *prev = NULL;
    while (current != NULL) {
        if (current->key == key) {
            if (prev == NULL) {
                table->ks = current->next;
            } else {
                prev->next = current->next;
            }
            free(current->info);
            free(current);
            return OK;
        }
        prev = current;
        current = prev->next;
    }
    return KEY_NOT_FOUND;
}


void recursive_delete_helper(Table *table, KeyType key) {
    KeySpace *current = table->ks;
    KeySpace *prev = NULL;

    while (current != NULL) {
        if (current->par == key) {
            recursive_delete_helper(table, current->key);
            if (prev == NULL) {
                deleteElement(table, current->key);
                current = table->ks;
            } else {
                KeyType k = current->key;
                prev = current;
                current = current->next;
                deleteElement(table, k);
            }
        } else {
            prev = current;
            current = current->next;
        }
    }
}

void recursive_delete(Table *table, KeyType key) {
    recursive_delete_helper(table, key);
    deleteElement(table, key);
}


void printTable(Table *table) {
    if (table == NULL || table->ks == NULL) {
        printf("Table is empty\n");
        return;
    }
    KeySpace *current = table->ks;
    printf("key  par  info\n");
    while (current != NULL) {
        printf("%d  |  %d  |  %u ", current->key, current->par, *(current->info));
        printf("\n");
        current = current->next;
    }
    printf("\n");
}


enum TableErrors findElement(Table *table, KeyType key) {
    KeySpace *current = table->ks;
    while (current != NULL) {
        if (current->key == key) {
            printf("key  par  info\n");
            printf("%d  |  %d  |  %u", current->key, current->par, *(current->info));
            return OK;
        }
        current = current->next;
    }
    return KEY_NOT_FOUND;
}


enum TableErrors importDataFromFile(Table *table, const char *filename) {
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        return END;
    }

    KeyType key, par;
    InfoType info;
    char *line = fread_line(file);

    while (line != NULL) {
        char *token = strtok(line, " ");
        if (token == NULL) {
            printf("Отсутствует ключ!\n");
            free(line);
            line = fread_line(file);
            continue;
        }

        key = atoi(token);

        token = strtok(NULL, " ");
        if (token == NULL) {
            printf("Отсутствует родитель!\n");
            free(line);
            line = fread_line(file);
            continue;
        }

        par = atoi(token);

        token = strtok(NULL, " ");
        if (token == NULL) {
            printf("Отсутствует информация!\n");
            free(line);
            line = fread_line(file);
            continue;
        }

        info = atoi(token);

        TableErrors result = insertElement(table, key, par, &info);
        
        if (result == MEMORY_LOCK) {
            printf("Ошибка при выделении памяти\n");
            fclose(file); 
            free(line);
            return MEMORY_LOCK;
        } 

        if (result == DUPLICATE_KEY || result == NO_PARENT) {
            printf("Элемент не записан\n");
        }

        free(line);
        line = fread_line(file);
    }
    fclose(file);
    return OK;
}