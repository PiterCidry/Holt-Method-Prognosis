#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <conio.h>
#define SIZE 100
#define NCOL 8

//Function used to generate random float numbers from range
float float_rand(float min, float max)
{
	float scale = rand() / (float)RAND_MAX; /* [0, 1.0] */
	return min + scale * (max - min);      /* [min, max] */
}

int main() {
	//Variable declaration
	int i, j;
	float Yt[SIZE], Ft[SIZE], St[SIZE], Ytprog[SIZE], Mod_Yt_minus_Ytprog[SIZE-1], Yt_minus_Ytprog[SIZE-1],
		Yt_minus_Ytprog_squared[SIZE-1], Me, Mse, Rmse, alpha, beta, tempSum, result[SIZE+1][NCOL];
	//Fullfilling the table with random data
	srand(12345);
	for (i = 0; i < SIZE; i++) {
		Yt[i] = float_rand(0.00, 10.00);
	}
	//Initial values for Ft and St
	Ft[0] = Yt[0];
	St[0] = Yt[1] - Yt[0];
	//Alpha and beta parameters get from user
	printf("Please provide alpha parameter from range [0,1]: ");
	if (scanf("%f", &alpha) != 1) {
		printf("Error! Please provide a number from range [0,1]!\n");
		system("PAUSE");
		exit(1);
	}
	if (alpha < 0 || alpha > 1) {
		printf("Error! Alpha parameter must be from range [0,1]!\n");
		system("PAUSE");
		exit(1);
	}
	printf("Please provide beta parameter from range [0,1]: ");
	if(scanf("%f", &beta) != 1) {
		printf("Error! Please provide a number from range [0,1]!\n");
		system("PAUSE");
		exit(1);
	}
	if (beta < 0 || beta > 1) {
		printf("Error! Beta parameter must be from range [0,1]!\n");
		system("PAUSE");
		exit(1);
	}
	//Fullfilling the Ft and St arrays
	for (i = 0; i < SIZE - 1; i++) {
		Ft[i + 1] = alpha*Yt[i + 1] + (1 - alpha)*(Ft[i] + St[i]);
		St[i + 1] = beta*(Ft[i + 1] - Ft[i]) + (1 - beta)*St[i];
	}
	//Making the predictions for the past
	for (i = 0; i < SIZE - 1; i++) {
		Ytprog[i] = Ft[i] + St[i];
	}
	//Making the prediction for next T
	Ytprog[SIZE - 1] = Ft[SIZE - 1] + ((SIZE + 1) - SIZE)*St[SIZE - 1];
	//Calculating errors
	for (i = 0; i < SIZE - 1; i++) {
		Mod_Yt_minus_Ytprog[i] = fabs(Yt[i+1] - Ytprog[i]);
		Yt_minus_Ytprog[i] = Yt[i+1] - Ytprog[i];
		Yt_minus_Ytprog_squared[i] = Yt_minus_Ytprog[i] * Yt_minus_Ytprog[i];
	}
	//Calculating ME
	tempSum = 0;
	for (i = 0; i < SIZE - 1; i++) {
		tempSum += Yt_minus_Ytprog[i];
	}
	Me = tempSum / (SIZE - 1);
	//Calculating MSE
	tempSum = 0;
	for (i = 0; i < SIZE - 1; i++) {
		tempSum += Yt_minus_Ytprog_squared[i];
	}
	Mse = tempSum / (SIZE - 1);
	//Calculating RMSE
	Rmse = sqrt(Mse);
	//Binding the result to a matrix
	for (i = 0; i < SIZE + 1; i++) {
		result[i][0] = i + 1; //t
		if (i < SIZE){
			result[i][1] = Yt[i];
			result[i][2] = Ft[i];
			result[i][3] = St[i];
			result[i + 1][4] = Ytprog[i];
		}
		if (i < SIZE - 1) {
			result[i + 1][5] = Mod_Yt_minus_Ytprog[i];
			result[i + 1][6] = Yt_minus_Ytprog[i];
			result[i + 1][7] = Yt_minus_Ytprog_squared[i];
		}
	}
	//Printing the result matrix
	printf("t\tYt\tFt\tSt\tYt*\t|Yt-Yt*|\tYt-Yt*\t(Yt-Yt*)^2\n---------------------------------------------------------------\n");
	for (i = 0; i < SIZE + 1; i++) {
		for (j = 0; j < NCOL; j++) {
			if (fabs(result[i][j]) < 107374175) { //not print empty default values
				if (j == 0) { //print T as integer
					printf("%d \t", (int)result[i][j]);
				}
				else {
					printf("%.2f \t", result[i][j]);
				}
			}
			else{
				printf(" \t ");
			}
		}
		printf("\n");
	}
	printf("\n---------------------------------------\nAlpha: %.2f\nBeta: %.2f\nME Error: %.2f\nMSE Error: %.2f\nRMSE Error: %.2f\n", alpha, beta, Me, Mse, Rmse);
	//Print results to file
	FILE *ofp;
	ofp = fopen("Results_DRG.txt", "w");
	if (ofp == NULL) {
		printf("Error in writing to a file!");
		exit(1);
	}
	else {
		fprintf(ofp, "t\tYt\tFt\tSt\tYt*\t|Yt-Yt*|\tYt-Yt*\t(Yt-Yt*)^2\n---------------------------------------------------------------\n");
		for (i = 0; i < SIZE + 1; i++) {
			for (j = 0; j < NCOL; j++) {
				if (fabs(result[i][j]) < 107374175) { //not print empty default values
					if (j == 0) { //print T as integer
						fprintf(ofp, "%d \t", (int)result[i][j]);
					}
					else {
						fprintf(ofp, "%.2f \t", result[i][j]);
					}
				}
				else {
					fprintf(ofp, " \t ");
				}
			}
			fprintf(ofp, "\n");
		}
		fprintf(ofp, "\n---------------------------------------\nAlpha: %.2f\nBeta: %.2f\nME Error: %.2f\nMSE Error: %.2f\nRMSE Error: %.2f\n", alpha, beta, Me, Mse, Rmse);
	}
	fclose(ofp);
	system("PAUSE");
	return 0;
}