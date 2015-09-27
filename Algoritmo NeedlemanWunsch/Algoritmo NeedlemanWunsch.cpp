// Algoritmo NeedlemanWunsch.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
using namespace std;

int A = 0, G = 1, C = 2, T = 3;
int esp = 5;

// A    G   C   T
int simularity[] = { 
10, -1, -3, -4, //A
-1,  7, -5, -3, //G
-3, -5,  9,  0, //C
-4, -3,  0,  8 };//T   

int similitud(int first, int second)
{
	return simularity[first * 4 + second];
}

vector<int> convertirStringToVector(string str)
{
	vector<int> l;
	for (int i = 0; i < str.size(); ++i)
	{
		int n = -1;
		if (str[i] == 'A')
			n = 0;
		if (str[i] == 'G')
			n = 1;
		if (str[i] == 'C')
			n = 2;
		if (str[i] == 'T')
			n = 3;
		l.push_back(n);
	}
	return l;
}

void imprimir_matriz(int** M,int size1,int size2) {
	for (int i = 0;i < size1;++i) {
		for (int j = 0;j < size2;++j) {
			cout << M[i][j] << '\t';
		}
		cout << endl;
	}

}

int main()
{
	/// abrimos las secuencias
	ifstream file1("secuencia1.txt");
	ifstream file2("secuencia2.txt");
	string buffer;
	string sec1, sec2;

	getline(file1, buffer);
	while (file1.good()) {
		getline(file1, buffer);
		sec1.append(buffer);
	}

	getline(file2, buffer);
	while (file2.good()) {
		getline(file2, buffer);
		sec2.append(buffer);
	}

	int size1 = sec1.size();
	int size2 = sec2.size();

	int ** M = new int *[size1 + 1];
	for (int i = 0; i <= size1; i++)
		M[i] = new int[size2 + 1];

	M[0][0] = 0;

	for (int j = 1; j <= size2; j++)
		M[0][j] = -j * esp;
	for (int i = 1; i <= size1; i++)
		M[i][0] = -i * esp;

	vector<int> sec_1 = convertirStringToVector(sec1);
	vector<int> sec_2 = convertirStringToVector(sec2);
	/// generamos la matriz
	for (int i = 0; i < size1;++i) {
		for (int j = 0; j < size2;++j) {
			int k = M[i][j] + similitud(sec_1[i], sec_2[j]);
			int l = M[i][j+1] - esp;
			int m = M[i+1][j] - esp;

			/// escogemos al mayor y lo guardamos en k
			if (k >= l)
				k = k;
			else
				k = l;
			
			if (k >= m)
				k = k;
			else
				k = m;

			M[i + 1][j + 1] = k;
			
		}
	}

	imprimir_matriz(M, size1 + 1, size2 + 1);

	/// obtenemos las alineaciones

	string alin_1 = ""; 
	string alin_2 = "";
	int i = size1; int j = size2;
	while (i > 0 && j > 0) {
		int score = M[i][j];
		int diag = M[i-1][j-1];
		int up = M[i][j-1];
		int izq = M[i-1][j];
		if (score == diag + similitud(sec_1[i - 1], sec_2[j - 1])) {
			alin_1 = sec1[i - 1] + alin_1;
			alin_2 = sec1[i - 1] + alin_2;
			--i;--j;
		}
		else if (score == izq - esp) {
			alin_1 = sec1[i - 1] + alin_1;
			alin_2 = "-" + alin_2;
			--i;
		}
		else if (score == up - esp) {
			alin_1 = "-" + alin_1;
			alin_2 = sec2[j - 1] + alin_2;
			--j;
		}
	}
	while (i > 0) {
		alin_1 = sec1[i - 1] + alin_1;
		alin_2 = "-" + alin_2;
		--i;
	}

	while (j > 0) {
		alin_1 = "-" + alin_1;
		alin_2 = sec2[j - 1] + alin_2;
		--j;
	}

	cout << "Alineacion 1: " << alin_1 << endl;
	cout << "Alineacion 2: " << alin_2 << endl;

	for (int i = 0; i <= size1; ++i) {
		delete M[i];
	}
	delete M;

    return 0;
}

