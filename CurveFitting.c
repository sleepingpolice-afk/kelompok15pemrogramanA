#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define CONST_FILE_NAME "Data Tugas Pemrograman A.csv"

#define MAX_DATA 100
#define DEGREE 3  // You can change this to 3 for cubic, etc.

void polynomial_fit(int n, int degree, const int x[], const double y[], double coeffs[]) {
    double X[2 * degree + 1];  // menyimpan jumlah pangkat x
    for (int i = 0; i < 2 * degree + 1; i++) {
        X[i] = 0;
        for (int j = 0; j < n; j++) {
            X[i] += pow(x[j], i);
        }
    }

    double B[degree + 1];  // sisi kanan
    for (int i = 0; i < degree + 1; i++) {
        B[i] = 0;
        for (int j = 0; j < n; j++) {
            B[i] += y[j] * pow(x[j], i);
        }
    }

    double matrix[degree + 1][degree + 2];  // matriks augmented
    for (int i = 0; i <= degree; i++) {
        for (int j = 0; j <= degree; j++) {
            matrix[i][j] = X[i + j];
        }
        matrix[i][degree + 1] = B[i];
    }

    // Eliminasi Gauss
    for (int i = 0; i <= degree; i++) {
        for (int k = i + 1; k <= degree; k++) {
            double factor = matrix[k][i] / matrix[i][i];
            for (int j = 0; j <= degree + 1; j++) {
                matrix[k][j] -= factor * matrix[i][j];
            }
        }
    }

    // Substitusi mundur
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
    int year[100] = {0};
    double percentage[100] = {0};
    long population[100] = {0};
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

    // for(int j = 0; j < i; j++) {
    //     printf("%.2f\n", percentage[j]);
    // }

    double coeffs_percentage[DEGREE + 1] = {0};
    double coeffs_population[DEGREE + 1] = {0};

    polynomial_fit(i, DEGREE, year, percentage, coeffs_percentage);
    double population_double[100];
    for (int j = 0; j < i; j++) {
        population_double[j] = (double)population[j];
    }
    polynomial_fit(i, DEGREE, year, population_double, coeffs_population);

    printf("Persamaan a (Persentase Pengguna Internet): y = ");
    for (int d = DEGREE; d >= 0; d--) {
        printf("%+.6f*x^%d ", coeffs_percentage[d], d);
    }
    printf("\n");

    printf("Persamaan b (Pertumbuhan Populasi): y = ");
    for (int d = DEGREE; d >= 0; d--) {
        printf("%+.6f*x^%d ", coeffs_population[d], d);
    }

    
    printf("\nEnter year to predict: ");
    fflush(stdout);
    int year_to_predict;
    scanf("%d", &year_to_predict);
    double predicted_percentage = 0;
    double predicted_population = 0;
    for (int d = DEGREE; d >= 0; d--) {
        predicted_percentage += coeffs_percentage[d] * pow(year_to_predict, d);
        predicted_population += coeffs_population[d] * pow(year_to_predict, d);
    }
    printf("Predicted percentage for year %d: %.6f\n", year_to_predict, predicted_percentage);
    printf("Predicted population for year %d: %.6f\n", year_to_predict, predicted_population);

    printf("\n");
    return 0; 
}