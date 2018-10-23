#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define SIZE 560
#define NCOL 8
#define DATA_COL_NUMBER 4
#define WINDOW_SIZE 5
#define DRAWS_NUMBER 10

//Function used to generate random float numbers from range
float float_rand(float min, float max)
{
	float scale = rand() / (float)RAND_MAX; /* [0, 1.0] */
	return min + scale * (max - min);      /* [min, max] */
}

int main() {
	//Variable declaration
	FILE *ifp;
	char firstLine[256], help[100], *row, *columns[DATA_COL_NUMBER];
	int i, j, k, selection;
	float data[SIZE][DATA_COL_NUMBER], Yt[SIZE], bestAlphaBetas[SIZE-4][3];
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
		Yt[i] = data[i][selection - 1];
	}
	//Loop iterating through window of size 5 and doing 10 prognosis
	for (i = 0; i < SIZE-4; i++) {
		float rmseArr[DRAWS_NUMBER], rmseMin, alphas[DRAWS_NUMBER], betas[DRAWS_NUMBER];
		int locationBestRmse;
		//Alfa and Beta parameters randomly generated
		for (k = 0; k < 10; k++) {
			alphas[k] = float_rand(0.00, 1.00);
			betas[k] = float_rand(0.00, 1.00);
		}
		for (j = 0; j < DRAWS_NUMBER; j++) {
			float  Yttemp[WINDOW_SIZE], Ft[WINDOW_SIZE], St[WINDOW_SIZE], Ytprog[WINDOW_SIZE], Mod_Yt_minus_Ytprog[WINDOW_SIZE - 1],
				Yt_minus_Ytprog[WINDOW_SIZE - 1], Yt_minus_Ytprog_squared[WINDOW_SIZE - 1], Me, Mse, Rmse, tempSum;
			//Fullfilling the temporary Yt array
			for (k = 0; k < WINDOW_SIZE; k++) {
				Yttemp[k] = Yt[i + k];
			}
			//Initial Ft and St values
			Ft[0] = Yttemp[0];
			St[0] = Yttemp[1] - Yttemp[0];
			//Fullfilling Ft and St arrays
			for (k = 0; k < WINDOW_SIZE - 1; k++) {
				Ft[k + 1] = alphas[j] * Yt[k + 1] + (1 - alphas[j])*(Ft[k] + St[k]);
				St[k + 1] = betas[j] * (Ft[k + 1] - Ft[k]) + (1 - betas[j])*St[k];
			}
			//Making the predictions for the past
			for (k = 0; k < WINDOW_SIZE - 1; k++) {
				Ytprog[k] = Ft[k] + St[k];
			}
			//Making the prediction for next T
			Ytprog[WINDOW_SIZE - 1] = Ft[WINDOW_SIZE - 1] + ((WINDOW_SIZE + 1) - WINDOW_SIZE)*St[WINDOW_SIZE - 1];
			//Calculating errors
			for (k = 0; k < WINDOW_SIZE - 1; k++) {
				Mod_Yt_minus_Ytprog[k] = fabs(Yt[k + 1] - Ytprog[k]);
				Yt_minus_Ytprog[k] = Yt[k + 1] - Ytprog[k];
				Yt_minus_Ytprog_squared[k] = Yt_minus_Ytprog[k] * Yt_minus_Ytprog[k];
			}
			//Calculating ME
			tempSum = 0;
			for (k = 0; k < WINDOW_SIZE - 1; k++) {
				tempSum += Yt_minus_Ytprog[k];
			}
			Me = tempSum / (WINDOW_SIZE - 1);
			//Calculating MSE
			tempSum = 0;
			for (k = 0; k < WINDOW_SIZE - 1; k++) {
				tempSum += Yt_minus_Ytprog_squared[k];
			}
			Mse = tempSum / (WINDOW_SIZE - 1);
			//Calculating RMSE
			Rmse = sqrt(Mse);
			rmseArr[j] = Rmse;
		}
		rmseMin = rmseArr[0];
		locationBestRmse = 0;
		for (j = 0; j < DRAWS_NUMBER; j++) {
			if (rmseArr[j] < rmseMin) {
				rmseMin = rmseArr[j];
				locationBestRmse = j;
			}
		}
		bestAlphaBetas[i][0] = alphas[locationBestRmse];
		bestAlphaBetas[i][1] = betas[locationBestRmse];
		bestAlphaBetas[i][2] = rmseMin;
	}
	//Printing the result and saving to file
	FILE *ofp;
	ofp = fopen("Results_MCS.txt", "w");
	if (ofp == NULL) {
		printf("Error in writing to a file!");
		system("PAUSE");
		exit(1);
	}
	fprintf(ofp, "Results of simulation:\nt\tAlpha\tBeta\tMin RMSE\n---------------------------------------\n");
	printf("Results of simulation:\nt\tAlpha\tBeta\tMin RMSE\n---------------------------------------\n");
	for (i = 0; i < SIZE - 5; i++) {
		fprintf(ofp, "%d\t%.2f\t%.2f\t%.2f\n", i + 6, bestAlphaBetas[i][0], bestAlphaBetas[i][1], bestAlphaBetas[i][2]);
		printf("%d\t%.2f\t%.2f\t%.2f\n", i + 6, bestAlphaBetas[i][0], bestAlphaBetas[i][1], bestAlphaBetas[i][2]);
	}
	//Variable declaration for final prediction
	float Ft[SIZE-5], St[SIZE-5], Ytprog[SIZE-5], Mod_Yt_minus_Ytprog[SIZE-6], Yt_minus_Ytprog[SIZE-6], 
		Yt_minus_Ytprog_squared[SIZE-6], tempSum, Me, Mse, Rmse, result[SIZE - 5 + 1][NCOL];
	//Initial values for Ft and St
	Ft[0] = Yt[0];
	St[0] = Yt[1] - Yt[0];
	//Fullfilling the Ft and St arrays
	for (i = 0; i < SIZE - 6; i++) {
		Ft[i + 1] = bestAlphaBetas[i][0] * Yt[i + 1] + (1 - bestAlphaBetas[i][0])*(Ft[i] + St[i]);
		St[i + 1] = bestAlphaBetas[i][1] * (Ft[i + 1] - Ft[i]) + (1 - bestAlphaBetas[i][1])*St[i];
	}
	//Making the predictions for the past
	for (i = 0; i < SIZE - 6; i++) {
		Ytprog[i] = Ft[i] + St[i];
	}
	//Making the prediction for next T
	Ytprog[SIZE - 6] = Ft[SIZE - 6] + ((SIZE - 5 + 1) - SIZE - 5)*St[SIZE - 6];
	//Calculating errors
	for (i = 0; i < SIZE - 6; i++) {
		Mod_Yt_minus_Ytprog[i] = fabs(Yt[i + 1] - Ytprog[i]);
		Yt_minus_Ytprog[i] = Yt[i + 1] - Ytprog[i];
		Yt_minus_Ytprog_squared[i] = Yt_minus_Ytprog[i] * Yt_minus_Ytprog[i];
	}
	//Calculating ME
	tempSum = 0;
	for (i = 0; i < SIZE - 6; i++) {
		tempSum += Yt_minus_Ytprog[i];
	}
	Me = tempSum / (SIZE - 6);
	//Calculating MSE
	tempSum = 0;
	for (i = 0; i < SIZE - 6; i++) {
		tempSum += Yt_minus_Ytprog_squared[i];
	}
	Mse = tempSum / (SIZE - 6);
	//Calculating RMSE
	Rmse = sqrt(Mse);
	//Binding the result to a data
	for (i = 0; i < SIZE - 5 + 1; i++) {
		result[i][0] = i + 1; //t
		if (i < SIZE - 5) {
			result[i][1] = Yt[i];
			result[i][2] = Ft[i];
			result[i][3] = St[i];
			result[i + 1][4] = Ytprog[i];
		}
		if (i < SIZE - 6) {
			result[i + 1][5] = Mod_Yt_minus_Ytprog[i];
			result[i + 1][6] = Yt_minus_Ytprog[i];
			result[i + 1][7] = Yt_minus_Ytprog_squared[i];
		}
	}
	//Printing the result data
	printf("\nFinal Simulation:\nSelected Data: %s\nt\tYt\tFt\tSt\tYt*\t|Yt-Yt*|\tYt-Yt*\t(Yt-Yt*)^2\tAlpha\tBeta\n---------------------------------------------------------------\n", columns[selection-1]);
	for (i = 0; i < SIZE - 5 + 1; i++) {
		for (j = 0; j < NCOL; j++) {
			if (fabs(result[i][j]) < 107374175) { //not print empty default values
				if (j == 0) { //print T as integer
					printf("%d \t", (int)result[i][j]+5);
				}
				else {
					printf("%.2f \t", result[i][j]);
				}
				if (j == 7) {
					printf("%.2f \t %.2f \t", bestAlphaBetas[i][0], bestAlphaBetas[i][1]);
				}
			}
			else {
				printf(" \t ");
			}
		}
		printf("\n");
	}
	printf("---------------------------------------------------------------\nME Error: %.2f\nMSE Error: %.2f\nRMSE Error: %.2f\n", Me, Mse, Rmse);
	//Print results to file
	if (ofp == NULL) {
		printf("Error in writing to a file!");
		system("PAUSE");
		exit(1);
	}
	else {
		fprintf(ofp, "\nFinal Simulation:\nSelected Data: %s\nt\tYt\tFt\tSt\tYt*\t|Yt-Yt*|\tYt-Yt*\t(Yt-Yt*)^2\tAlpha\tBeta\n---------------------------------------------------------------\n", columns[selection-1]);
		for (i = 0; i < SIZE - 5 + 1; i++) {
			for (j = 0; j < NCOL; j++) {
				if (fabs(result[i][j]) < 107374175) { //not print empty default values
					if (j == 0) { //print T as integer
						fprintf(ofp, "%d \t", (int)result[i][j]+5);
					}
					else {
						fprintf(ofp, "%.2f \t", result[i][j]);
					}
					if (j == 7) {
						fprintf(ofp, "%.2f \t %.2f \t", bestAlphaBetas[i][0], bestAlphaBetas[i][1]);
					}
				}
				else {
					fprintf(ofp, " \t ");
				}
			}
			fprintf(ofp, "\n");
		}
		fprintf(ofp, "---------------------------------------------------------------\nME Error: %.2f\nMSE Error: %.2f\nRMSE Error: %.2f\n", Me, Mse, Rmse);
	}
	fclose(ofp);
	system("PAUSE");
	return 0;
}