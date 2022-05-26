// Copyright Vladut Alexandru-Nicolae 312CAa 2021-2022

#include <stdio.h>
#include <stdlib.h>	
#define M 10007

void eliberare_matrice(int **a, int linii)
{
	for (int i = 0; i < linii; i++)
		free(a[i]);

	free(a);
}

int **alocare_matrice(int linii, int coloane)
{
	int **a = malloc(linii * sizeof(int *));
	if (!a) {
		free(a);
		return NULL;
	}

	for (int i = 0; i < linii; i++) {
		a[i] = malloc(coloane * sizeof(int));
		if (!a[i]) {
			eliberare_matrice(a, linii);
			return NULL;
		}
	}

	return a;
}

void citire_matrice(int **a, int linii, int coloane)
{
	for (int i = 0; i < linii; i++)
		for (int j = 0; j < coloane; j++)
			scanf("%d", &a[i][j]);
}

void afisare_matrice(int **a, int linii, int coloane)
{
	for (int i = 0; i < linii; i++) {
		for (int j = 0; j < coloane; j++)
			printf("%d ", a[i][j]);
		printf("\n");
	}
}

int suma_elemente(int **a, int linii, int coloane)
{
	int sum = 0;
	for (int i = 0; i < linii; i++) {
		for (int j = 0; j < coloane; j++) {
			sum = (sum + a[i][j]) % M;
			if (sum < 0)
				sum += M;
		}
	}
	return sum;
}

void realocare(int ****col, int **linii, int **coloane, int nr)
{
	*col = realloc(*col, (nr + 1) * sizeof(int **));
	*linii = realloc(*linii, (nr + 1) * sizeof(int));
	*coloane = realloc(*coloane, (nr + 1) * sizeof(int));
}

// caz D
void dimensiuni(int *linii, int *coloane, int nr)
{
	int indice;
	scanf("%d", &indice);
	if (indice < 0 || indice > nr - 1)
		printf("No matrix with the given index\n");
	else
		printf("%d %d\n", linii[indice], coloane[indice]);
}

// caz P
void afisare(int ***col, int *linii, int *coloane, int nr)
{
	int indice;
	scanf("%d", &indice);
	if (indice < 0 || indice > nr - 1)
		printf("No matrix with the given index\n");
	else
		afisare_matrice(col[indice], linii[indice], coloane[indice]);
}

// caz C
void redimensionare(int ****col, int **linii, int **coloane, int nr)
{
	int indice, l, c;

	scanf("%d", &indice);

	scanf("%d", &l);
	int *indici_l = malloc(l * sizeof(int));
	for (int i = 0; i < l; i++)
		scanf("%d", &indici_l[i]);

	scanf("%d", &c);
	int *indici_c = malloc(c * sizeof(int));
	for (int i = 0; i < c; i++)
		scanf("%d", &indici_c[i]);

	if (indice < 0 || indice > nr - 1) {
		printf("No matrix with the given index\n");
	} else {
		int **redim = alocare_matrice(l, c);
		for (int i = 0; i < l; i++)
			for (int j = 0; j < c; j++)
				redim[i][j] = (*col)[indice][indici_l[i]][indici_c[j]];

		eliberare_matrice((*col)[indice], (*linii)[indice]);
		(*col)[indice] = redim;
		(*linii)[indice] = l;
		(*coloane)[indice] = c;
	}

	free(indici_l);
	free(indici_c);
}

// caz M
void inmultire(int ****col, int **linii, int **coloane, int *nr)
{
	int indice1, indice2;
	scanf("%d%d", &indice1, &indice2);
	if (indice1 < 0 || indice1 > *nr - 1 || indice2 < 0 || indice2 > *nr - 1) {
		printf("No matrix with the given index\n");
	} else if ((*coloane)[indice1] != (*linii)[indice2]) {
		printf("Cannot perform matrix multiplication\n");
	} else {
		int l1 = (*linii)[indice1];
		int c1 = (*coloane)[indice1];
		int c2 = (*coloane)[indice2];
		int **a = (*col)[indice1];
		int **b = (*col)[indice2];

		realocare(&(*col), &(*linii), &(*coloane), *nr);
		(*col)[*nr] = alocare_matrice(l1, c2);
		for (int i = 0; i < l1; i++) {
			for (int j = 0; j < c2; j++) {
				(*col)[*nr][i][j] = 0;
				for (int k = 0; k < c1; k++) {
					int tmp = (*col)[*nr][i][j];
					tmp = (tmp + a[i][k] * b[k][j]) % M;
					(*col)[*nr][i][j] = tmp;
					if ((*col)[*nr][i][j] < 0)
						(*col)[*nr][i][j] += M;
				}
			}
		}
		(*linii)[*nr] = l1;
		(*coloane)[*nr] = c2;
		(*nr)++;
	}
}

// caz O
void sortare(int ****col, int **linii, int **coloane, int nr)
{
	int *sume = malloc(nr * sizeof(int));

	for (int i = 0; i < nr; i++)
		sume[i] = suma_elemente((*col)[i], (*linii)[i], (*coloane)[i]);

	for (int i = 0; i < nr - 1; i++) {
		for (int j = i + 1; j < nr; j++) {
			if (sume[i] > sume[j]) {
				int aux;
				aux = sume[i], sume[i] = sume[j], sume[j] = aux;
				aux = (*linii)[i], (*linii)[i] = (*linii)[j], (*linii)[j] = aux;
				aux = (*coloane)[i];
				(*coloane)[i] = (*coloane)[j];
				(*coloane)[j] = aux;
				int **aux_m;
				aux_m = (*col)[i], (*col)[i] = (*col)[j], (*col)[j] = aux_m;
			}
		}
	}

	free(sume);
}

// caz T
void transpusa(int ****col, int **linii, int **coloane, int nr)
{
	int indice;
	scanf("%d", &indice);
	if (indice < 0 || indice > nr - 1) {
		printf("No matrix with the given index\n");
	} else {
		int tl, tc;
		tl = (*coloane)[indice];
		tc = (*linii)[indice];
		int **t = alocare_matrice(tl, tc);
		for (int i = 0; i < tl; i++)
			for (int j = 0; j < tc; j++)
				t[i][j] = (*col)[indice][j][i];
		eliberare_matrice((*col)[indice], (*linii)[indice]);
		(*col)[indice] = t;
		int aux = (*linii)[indice];
		(*linii)[indice] = (*coloane)[indice];
		(*coloane)[indice] = aux;
	}
}

// caz F
void eliberare_indice(int ****col, int **linii, int **coloane, int *nr)
{
	int indice;
	scanf("%d", &indice);
	if (indice < 0 || indice > *nr - 1) {
		printf("No matrix with the given index\n");
	} else {
		eliberare_matrice((*col)[indice], (*linii)[indice]);
		for (int i = indice; i < *nr - 1; i++) {
			(*col)[i] = (*col)[i + 1];
			(*linii)[i] = (*linii)[i + 1];
			(*coloane)[i] = (*coloane)[i + 1];
		}
		(*nr)--;
		*linii = realloc(*linii, (*nr) * sizeof(int));
		*coloane = realloc(*coloane, (*nr) * sizeof(int));
		*col = realloc(*col, (*nr) * sizeof(int **));
	}
}

// caz Q
void eliberare_resurse(int ***col, int *linii, int *coloane, int nr)
{
	for (int i = 0; i < nr; i++)
		eliberare_matrice(col[i], linii[i]);
	free(col);

	free(linii);
	free(coloane);
}

// caz A
void adunare(int ****col, int **linii, int **coloane, int *nr) 
{
	int indice1, indice2;
	scanf("%d%d", &indice1, &indice2);
	if (indice1 < 0 || indice1 > *nr - 1 || indice2 < 0 || indice2 > *nr - 1) {
		printf("No matrix with the given index\n");
	} else if (((*linii)[indice1] != (*linii)[indice2]) || ((*linii)[indice1] != (*linii)[indice2])) {
		printf("Cannot perform matrix addition\n");
	} else {
		int l = (*linii)[indice1];
		int c = (*coloane)[indice1];

		int **a = (*col)[indice1];
		int **b = (*col)[indice2];

		realocare(&(*col), &(*linii), &(*coloane), *nr);
		(*col)[*nr] = alocare_matrice(l, c);
		for (int i = 0; i < l; i++) {
			for (int j = 0; j < c; j++) {
				(*col)[*nr][i][j] = a[i][j] + b[i][j];
			}
		}
		(*linii)[*nr] = l;
		(*coloane)[*nr] = c;
		(*nr)++;
	}
}

int main(void)
{
	// ***col -> colectia de matrici
	// *linii -> vectorul care retine numarul de linii al fiecarei matrice
	// *coloane -> vectorul care retine numarul de coloane al fiecarei matrice
	int ***col = NULL, *linii = NULL, *coloane = NULL, nr = 0;
	int l, c;

	char litera = 'X';
	while (litera != 'Q') {
		scanf("%c", &litera);
		switch (litera) {
		case 'L':
			realocare(&col, &linii, &coloane, nr);
			scanf("%d%d", &l, &c);
			col[nr] = alocare_matrice(l, c);
			citire_matrice(col[nr], l, c);
			linii[nr] = l; coloane[nr] = c;
			nr++;
			break;
		case 'D':
			dimensiuni(linii, coloane, nr);
			break;
		case 'P':
			afisare(col, linii, coloane, nr);
			break;
		case 'C':
			redimensionare(&col, &linii, &coloane, nr);
			break;
		case 'M':
			inmultire(&col, &linii, &coloane, &nr);
			break;
		case 'O':
			sortare(&col, &linii, &coloane, nr);
			break;
		case 'T':
			transpusa(&col, &linii, &coloane, nr);
			break;
		case 'F':
			eliberare_indice(&col, &linii, &coloane, &nr);
			break;
		case 'Q':
			eliberare_resurse(col, linii, coloane, nr);
			break;
		case 'A':
			adunare(&col, &linii, &coloane, &nr);
			break;
		case '\n':
			break;
		default:
			printf("Unrecognized command\n");
			break;
		}
	}

	return 0;
}
