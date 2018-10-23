#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <conio.h>
#define SIZE 560
#define NCOL 8
#define DATA_COL_NUMBER 4

int main() {
	//Variable declaration
	int i, j, line = 0, selection;
	float Yt[SIZE], Ft[SIZE], St[SIZE], Ytprog[SIZE], Mod_Yt_minus_Ytprog[SIZE - 1], Yt_minus_Ytprog[SIZE - 1],
		Yt_minus_Ytprog_squared[SIZE - 1], Me, Mse, Rmse, alpha, beta, tempSum, result[SIZE + 1][NCOL], data[SIZE][DATA_COL_NUMBER];
	char help[100], *row, *columns[DATA_COL_NUMBER], firstLine[256];
	FILE *ofp;
	FILE *ifp;
	//Read data about first line from a file
	ifp = fopen("eurpln_d.csv", "r");
	if (ifp == NULL) {
		printf("Error in reading from a file!\n");
		system("PAUSE");
		exit(1);
	}
	i = 0;
	while (!feof(ifp)) {
		if (i == 0) {
			fscanf(ifp, "%s", firstLine);
			row = strtok(firstLine, ",");
			j = 0;
			while (row != NULL) {
				if (j > 0) {
					columns[j - 1] = row;
					row = strtok(NULL, ",");
					j++;
				}
				else {
					row = strtok(NULL, ",");
					j++;
				}
			}
			i++;
			break;
		}
	}
	fclose(ifp);
	//Read data about other lines than first from a file
	ifp = fopen("eurpln_d.csv", "r");
	if (ifp == NULL) {
		printf("Error in reading from a file!\n");
		system("PAUSE");
		exit(1);
	}
	i = 0;
	while (!feof(ifp))
	{
		j = 0;
		fscanf(ifp, "%s", help);
		row = strtok(help, ",");
		while (row != NULL)
		{
			if (i == 0) {
				row = strtok(NULL, ",");
				j++;
			}
			else {
				if (j != 0) {
					data[i - 1][j - 1] = (float)atof(row);
					row = strtok(NULL, ",");
					j++;
				}
				else {
					row = strtok(NULL, ",");
					j++;
				}
			}
		}
		i++;
	}
	fclose(ifp);
	//Get selected column from the user
	printf("Please select column with data:\n");
	for (i = 0; i < sizeof(columns) / sizeof(columns[0]); i++) {
		printf("%d. %s\n", i + 1, columns[i]);
	}
	if (scanf("%d", &selection) != 1) {
		printf("Error! Please select proper column number!\n");
		system("PAUSE");
		exit(1);
	}
	if (selection < 1 || selection > DATA_COL_NUMBER) {
		printf("Error! Please enter a valid number between 1 and 4!\n");
		system("PAUSE");
		exit(1);
	}
	for (i = 0; i < SIZE; i++) {
		Yt[i] = data[i][selection-1];
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
	if (scanf("%f", &beta) != 1) {
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
		Ft[i + 1] = alpha * Yt[i + 1] + (1 - alpha)*(Ft[i] + St[i]);
		St[i + 1] = beta * (Ft[i + 1] - Ft[i]) + (1 - beta)*St[i];
	}
	//Making the predictions for the past
	for (i = 0; i < SIZE - 1; i++) {
		Ytprog[i] = Ft[i] + St[i];
	}
	//Making the prediction for next T
	Ytprog[SIZE - 1] = Ft[SIZE - 1] + ((SIZE + 1) - SIZE)*St[SIZE - 1];
	//Calculating errors
	for (i = 0; i < SIZE - 1; i++) {
		Mod_Yt_minus_Ytprog[i] = (float)fabs(Yt[i + 1] - Ytprog[i]);
		Yt_minus_Ytprog[i] = Yt[i + 1] - Ytprog[i];
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
	//Binding the result to a data
	for (i = 0; i < SIZE + 1; i++) {
		result[i][0] = i + 1; //t
		if (i < SIZE) {
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
	//Printing the result data
	printf("Selected Data: %s\nt\tYt\tFt\tSt\tYt*\t|Yt-Yt*|\tYt-Yt*\t(Yt-Yt*)^2\n---------------------------------------------------------------\n", columns[selection-1]);
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
			else {
				printf(" \t ");
			}
		}
		printf("\n");
	}
	printf("\n---------------------------------------\nAlpha: %.2f\nBeta: %.2f\nME Error: %.2f\nMSE Error: %.2f\nRMSE Error: %.2f\n", alpha, beta, Me, Mse, Rmse);
	//Print results to file
	ofp = fopen("MiTP_zad_1.3_wyniki.txt", "w");
	if (ofp == NULL) {
		printf("Error in writing to a file!");
		system("PAUSE");
		exit(1);
	}
	else {
		fprintf(ofp, "Selected Data: %s\nt\tYt\tFt\tSt\tYt*\t|Yt-Yt*|\tYt-Yt*\t(Yt-Yt*)^2\n---------------------------------------------------------------\n", columns[selection-1]);
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
		fprintf(ofp, "---------------------------------------\nAlpha: %.2f\nBeta: %.2f\nME Error: %.2f\nMSE Error: %.2f\nRMSE Error: %.2f\n", alpha, beta, Me, Mse, Rmse);
	}
	fclose(ofp);
	system("PAUSE");
	return 0;
}