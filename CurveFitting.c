#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONST_FILE_NAME "Data Tugas Pemrograman A.csv"

int main(void){
    FILE *file = fopen(CONST_FILE_NAME, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", CONST_FILE_NAME);
        return 1;
    }

    char buffer[1024];
    int year[1000] = {0};
    double percentage[1000] = {0};
    long population[1000] = {0};
    int i = 0;

    char *token;

    // Skip header
    fgets(buffer, sizeof(buffer), file);

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        token = strtok(buffer, ",");
        if (token != NULL) {
            year[i] = atoi(token);
        } else {
            continue;
        }

        token = strtok(NULL, ",");
        if (token != NULL) {
            percentage[i] = atof(token);
        } else {
            continue;
        }

        token = strtok(NULL, ",");
        if (token != NULL) {
            population[i] = atol(token);
        } else {
            continue;
        }

        i++;
    }

    for (int j = 0; j < i; j++) {
        printf("Year: %d\n", year[j]);
    }

    fclose(file);

}