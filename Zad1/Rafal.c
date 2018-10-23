#define _CRT_SECURE_NO_DEPRECATE
//#include <stdafx.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <conio.h>
#define n 100


int main()
{
	int i;
	//const int n = 100;
	float y[n]; //wektor danych wejsciowych
	float alfa, beta; //parametry modelu
	float S[n - 1]; //wygladzone wartosci przyrostu trendu na moment t-1
	float F[n - 1]; //wygladzone wartosci zmiennej prognozowanej na moment t-1

	float predykcja[n - 2]; //predykcje od t=3 do t=100 -->(100-3)+1 wyników
	float predykcja101;

	float blad[n - 2]; //blad bezwzgledny
	float blad_sq[n - 2]; //kwadrat bledow
	float suma_sq = 0;
	float RMSE;


	// podanie parametrow modelu alfa i beta z przedzialu [0,1]

	printf("Podaj parametr alfa z przedzialu [0,1] : ");
	scanf_s("%f", &alfa);
	printf("Alfa wynosi: %0.2f \n", alfa);

	printf("Podaj parametr beta z przedzialu [0,1] : ");
	scanf_s("%f", &beta);
	printf("Beta wynosi: %0.2f \n", beta);


	//tworzenie macierzy danych wejsciowych
	float p = 0;//przedzial dla danych wejsciowych
	float q = 10;//przedzial
	printf("Dane wejsciowe :\n");
	srand(12345);
	for (int i = 0; i < n; i++)
	{
		y[i] = p + rand() / (double)RAND_MAX*(q - p);
		printf("t= %d : %.3f\n", i + 1, y[i]);

	}


	//obliczenie wartosci przyrostu trendu i zmiennej prognozowanej
	for (int i = 2; i < n; i++)
	{
		if (i == 2)
		{
			F[i - 2] = y[i - 1];
			S[i - 2] = y[i - 1] - y[i - 2];
		}
		else {
			F[i - 2] = alfa * y[i - 1] + (1 - alfa)*(F[i - 3] + S[i - 3]);
			S[i - 2] = beta * (F[i - 2] - F[i - 3]) + (1 - beta)*S[i - 3];
		}

	}
	//obliczenie prognoz od t=3 do t=100

	printf("Prognoza dla:\n ");
	for (int i = 0; i < n - 2; i++)
	{
		predykcja[i] = F[i] + S[i];
		printf("t= %d : %f\n", i + 3, predykcja[i]);
	}

	//predykcja dla t=101 (t>n)

	F[n - 2] = alfa * y[n - 1] + (1 - alfa)*(F[n - 3] + S[n - 3]);
	S[n - 2] = beta * (F[n - 2] - F[n - 3]) + (1 - beta)*S[n - 3];

	predykcja101 = F[n - 2] + S[n - 2];
	printf("Predykcja na okres t=101: %f\n", predykcja101);

	//blad prognozy RMSE

	for (int i = 0; i < n - 2; i++)
	{
		blad[i] = predykcja[i] - y[i + 2];
		printf(" t=%d. %f\n", i + 3, blad[i]);
		blad_sq[i] = blad[i] * blad[i];
		suma_sq += blad_sq[i];

	}
	RMSE = sqrt(suma_sq);

	//printf("RMSE: %f\n", RMSE);


	//wyswietlenie wynikow
	printf("t \ty \tF \tS \ty* \tblad \n");
	printf("t=%d \t%0.2f \t \t \t \t \n", 1, y[0]);
	printf("t=%d \t%0.2f \t%0.2f \t%0.2f \t \t \n", 2, y[1], F[0], S[0]);
	for (i = 0; i < n - 2; i++) {
		printf("t=%d \t%0.2f \t%0.2f \t%0.2f \t%0.2f \t%0.2f \n", i + 3, y[i + 2], F[i + 1], S[i + 1], predykcja[i], blad[i]);
	}

	printf("t=101 \t \t \t \t%0.2f \t \n", predykcja101);




	//zapis do pliku
	FILE *fp = NULL;

	fp = fopen("zadanie2_wyniki.txt", "a");

	fprintf(fp, "t \ty \tF \tS \ty* \tblad \n");
	fprintf(fp, "t=%d \t%0.2f \t \t \t \t \n", 1, y[0]);
	fprintf(fp, "t=%d \t%0.2f \t%0.2f \t%0.2f \t \t \n", 2, y[1], F[0], S[0]);
	for (i = 0; i < n - 2; i++) {
		fprintf(fp, "t=%d \t%0.2f \t%0.2f \t%0.2f \t%0.2f \t%0.2f \n", i + 3, y[i + 2], F[i + 1], S[i + 1], predykcja[i], blad[i]);
	}

	fprintf(fp, "t=101 \t \t \t \t%0.2f \t \n", predykcja101);
	_getch();
}