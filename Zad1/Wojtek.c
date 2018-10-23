float Matrix[1000][1000]; //Macierz na dane z pliku

int main() {
	float *tab; //tablica przechowujaca wybrana przez uzytkownika kolumne
	int j, i;
	int rows; //liczba wierszy w pliku
	int col; //liczba kolumn
	float alfa, beta, MSE, RMSE;
	float *Ftab; //tablica do przechowywania wartosci F
	float *Stab; //tablica do przechowywania wartosci S
	float *y; //prognozy
	float *odchyleniekw; //odchylenie kw prognoz
	float sum = 0; //suma odchylen^2 prognoz
	int ncol; //numer kolumny wybranej przez uzytkownika



//Otwarcie i pokazanie zawartosci pliku

	FILE *data;
	//DLA PRAWIDLOWEGO DZIALANIA NALEZY PONIZEJ WCZYTAC ODPOWIEDNIA SCIEZKE DO PLIKU !!!
	data = fopen("/Users/wojciechdragula/Desktop/Projekt_1.3/Projekt_1.3/dane.csv", "r");
	//WAZNE!!! ^
	char help[100], *row;
	i = 0;
	while (!feof(data))
	{
		j = 0;
		fscanf(data, "%s", help);
		row = strtok(help, ",");

		while (row != NULL)
		{
			Matrix[i][j] = atof(row);
			row = strtok(NULL, ",");
			j++;
			if (i == 0) {
				col = j;
			}
		}
		i++;
	}