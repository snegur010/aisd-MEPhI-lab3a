#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io.h"
#include "table.h"

int getInteger(KeyType *d) {
    int k = 0;
    while (k != 1) {
        k = scanf("%u", d);

        if (k == EOF) {
            printf("End of file\n");
            return 1;
        }
        if (k != 1) {
            printf("Incorrect input! Try again: ");
            scanf("%*[^\n]"); // Очистка буфера до новой строки
            scanf("%*c"); // Очистка символа новой строки
        } else {
            int nextChar = getchar(); 
            if (nextChar != '\n' && nextChar != EOF) {
                printf("Extra characters found! Try again: ");
                while ((getchar()) != '\n'); 
                k = 0; 
            }
        }
    }
    return 0;
}

int getinteger(int *d) {
    int k = 0;
    while (k != 1) {
        k = scanf("%d", d);

        if (k == EOF) {
            printf("End of file\n");
            return 1;
        }
        if (k != 1) {
            printf("Incorrect input! Try again: ");
            scanf("%*[^\n]"); // Очистка буфера до новой строки
            scanf("%*c"); // Очистка символа новой строки
        } else {
            int nextChar = getchar(); 
            if (nextChar != '\n' && nextChar != EOF) {
                printf("Extra characters found! Try again: ");
                while ((getchar()) != '\n'); 
                k = 0; 
            }
        }
    }
    return 0;
}

char* readline(const char *str) {
    printf("%s", str);
    int b_size = 50, f = 0;
    char *res = (char *) calloc (b_size, sizeof(char));
    if (res == NULL) {
        return NULL;
    }
    while ((f = scanf("%49[^\n]", res + b_size - 50)) == 1) { 
        b_size += 49;
        res = realloc(res, b_size * sizeof(char));
    }
    if (f == -1) {
        free(res);
        return NULL;
    }
    int clen = strlen(res);
    res = realloc(res, (clen + 1) * sizeof(char));
    scanf("%*c");
    return res;
}


char* fread_line(FILE *fin) {
    int b_size = 50, f = 0;
    char *res = (char *) calloc(b_size, sizeof(char));
    if (res == NULL) {
        return NULL;
    }
    while ((f = fscanf(fin, "%49[^\n]", res + b_size - 50)) == 1) {
        b_size += 49;
        res = realloc(res, b_size * sizeof(char));
    }

    if (f == -1) {
        free(res);
        return NULL;
    }
    int clen = strlen(res);
    res = realloc(res, (clen + 1) * sizeof(char));
    fscanf(fin, "%*c");
    return res;
}