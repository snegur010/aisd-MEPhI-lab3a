#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table.h"
#include "io.h"


typedef TableErrors (*MenuFunction)(Table *);

TableErrors insertElementWrapper(Table *table); //1
TableErrors deleteElementWrapper(Table *table); //2
TableErrors findElementWrapper(Table *table); //3
TableErrors printTableWrapper(Table *table); //4
TableErrors importDataWrapper(Table *table); //5
TableErrors recursiveDeleteWrapper(Table *table); //6
TableErrors searchByParentWrapper(Table *table); //7
TableErrors exitProgram(Table *table); //8


int main() {

    int choice;
    Table *table = createTable();

    MenuFunction menuFunctions[] = {
        insertElementWrapper,
        deleteElementWrapper,
        findElementWrapper,
        printTableWrapper,
        importDataWrapper,
        recursiveDeleteWrapper,
        searchByParentWrapper,
        exitProgram
    };

    const char *msgs[] = {"1. Вставка нового элемента",
                          "2. Удаление элемента",
                          "3. Поиск элемента",
                          "4. Вывод содержимого таблицы",
                          "5. Импорт данных из файла",
                          "6. Рекурсивное удаление элементов",
                          "7. Поиск по родительскому ключу",
                          "8. Выход"};

    const int numMsgs = sizeof(msgs) / sizeof(msgs[0]);

    do {
        printf("\n______________MENU_______________\n");

        for (int i = 0; i < numMsgs; i++) {
            printf("%s\n", msgs[i]);
        } 

        printf("\n");

        printf("Введите номер операции: ");
        int curr = getinteger(&choice);
        if (curr == 1) {
            free_table(table);
            return 1;
        }

        if (choice >= 1 && choice <= 8) {
            if (menuFunctions[choice - 1] != NULL) {
                TableErrors result = menuFunctions[choice - 1](table);
                if (result == END) {
                    return 1;
                }
            } else {
                printf("Опция пока не доступна. Попробуйте еще раз.\n");
            }
        } else {
            printf("Неверный номер операции. Попробуйте еще раз.\n");
        }
    } while (choice != 8);
    return 0;
}


TableErrors insertElementWrapper(Table *table) {
    KeyType key, par;
    InfoType info;
    printf("Введите ключ элемента: ");
    int curr = getInteger(&key);
    if (curr == 1) {
        free_table(table);
        return END;
    }
    printf("Введите родительский ключ (если его нет, введите 0): ");
    curr = getInteger(&par);
    if (curr == 1) {
        free_table(table);
        return END;
    }
    printf("Введите значение для ключа: ");
    curr = getInteger(&info);
    if (curr == 1) {
        free_table(table);
        return END;
    }

    TableErrors k = insertElement(table, key, par, &info);
    if (k == MEMORY_LOCK) {
        free_table(table);
        return MEMORY_LOCK;
    }
    return OK;
}


TableErrors deleteElementWrapper(Table *table) {
    KeyType key;
    printf("Введите ключ элемента, который хотите удалить: ");
    int curr = getInteger(&key);
    if (curr == 1) {
        free_table(table);
        return END;
    }
    TableErrors k = deleteElement(table, key);
    if (k == KEY_NOT_FOUND) {
        printf("\nКлюча с таким значением нет\n");
    }
    return OK;
}


TableErrors findElementWrapper(Table *table) {
    KeyType key;
    printf("Введите ключ элемента, который хотите найти: ");
    int curr = getInteger(&key);
    if (curr == 1) {
        free_table(table);
        return END;
    }
    TableErrors k = findElement(table, key);
    if (k == KEY_NOT_FOUND) {
        printf("\nНе удалось найти элемент с таким ключом\n");
    }
    return OK;
}


TableErrors printTableWrapper(Table *table) {
    printf("\n    ТАБЛИЦА    \n");
    printTable(table);
    return OK;
}


TableErrors importDataWrapper(Table *table) {
    char *filename = readline("Введите имя файла для импорта данных >>>  ");
    if (filename == NULL) {
        printf("Не удалось считать файл\n");
        free(filename);
        return END;
    }

    TableErrors importResult = importDataFromFile(table, filename);

    if (importResult == MEMORY_LOCK) {
        free(filename);
        free_table(table);
    } 

    if (importResult == END) {
        free(filename);
        printf("Не удалось открыть такой файл\n");
        return OK;
    }
    printf("Данные успешно записаны");
    free(filename);
    return OK;
}


TableErrors recursiveDeleteWrapper(Table *table) {
    KeyType key;
    printf("Введите ключ элемента для рекурсивного удаления: ");
    getInteger(&key);
    recursive_delete(table, key);
    return OK;
}


TableErrors searchByParentWrapper(Table *table) {
    Table *newTable = createTable();
    KeyType key;
    printf("Введите значение родителя: ");
    int curr = getInteger(&key);
    if (curr == 1) {
        free_table(newTable);
        return END;
    }
    search_by_parent(table, newTable, key);
    printf("\n    ТАБЛИЦА    \n");
    printTable(table);
    printf("\n НОВАЯ ТАБЛИЦА  \n");
    printTable(newTable);
    free_table(newTable);
    return OK;
}


TableErrors exitProgram(Table *table) {
    free_table(table);
    return OK;
}