#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define CONST_FILE_NAME "Data Tugas Pemrograman A.csv"

#define MAX_DATA 1000
#define DEGREE 2  // You can change this to 3 for cubic, etc.

void polynomial_fit(int n, int degree, const int x[], double y[], double coeffs[]) {
    double X[2 * DEGREE + 1] = {0}; // Sum of powers of x
    for (int i = 0; i < 2 * degree + 1; i++) {
        for (int j = 0; j < n; j++)
            X[i] += pow(x[j], i);
    }

    double B[DEGREE + 1] = {0};   
    for (int i = 0; i < degree + 1; i++) {
        for (int j = 0; j < n; j++)
            B[i] += y[j] * pow(x[j], i);
    }

    double matrix[DEGREE + 1][DEGREE + 2]; 
    for (int i = 0; i <= degree; i++) {
        for (int j = 0; j <= degree; j++) {
            matrix[i][j] = X[i + j];
        }
        matrix[i][degree + 1] = B[i];
    }

    for (int i = 0; i <= degree; i++) {
        for (int k = i + 1; k <= degree; k++) {
            double t = matrix[k][i] / matrix[i][i];
            for (int j = 0; j <= degree + 1; j++) {
                matrix[k][j] -= t * matrix[i][j];
            }
        }
    }
    for (int i = degree; i >= 0; i--) {
        coeffs[i] = matrix[i][degree + 1];
        for (int j = i + 1; j <= degree; j++) {
            coeffs[i] -= matrix[i][j] * coeffs[j];
        }
        coeffs[i] /= matrix[i][i];
    }
}

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

    fclose(file);

    double coeffs_percentage[DEGREE + 1] = {0};
    double coeffs_population[DEGREE + 1] = {0};

    polynomial_fit(i, DEGREE, year, percentage, coeffs_percentage);
    polynomial_fit(i, DEGREE, year, (double *)population, coeffs_population); // Cast needed

    printf("Persamaan a (Persentase Pengguna Internet): y = ");
    for (int d = DEGREE; d >= 0; d--) {
        printf("%+.6f*x^%d ", coeffs_percentage[d], d);
    }
    printf("\n");

    printf("Persamaan b (Pertumbuhan Populasi): y = ");
    for (int d = DEGREE; d >= 0; d--) {
        printf("%+.6f*x^%d ", coeffs_population[d], d);
    }
    printf("\n");
    return 0; 
}