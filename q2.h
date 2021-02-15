#pragma once

typedef struct {
	float coefficient;
	int power;
	struct Monom* next;		// Pointer to the next monom in the list
} Monom;

typedef struct {
	Monom* monoms;		// Array of Monoms
	int size;			// Number of Monoms in Polygon
} Polynom;

void	q2();

int createAndPrintPoly(int length, float coef, int pw, ...);
int insertMonomToMonomArr(Monom* monomArr, int* monomCount, const float coefficient, const int pow);
int compareByPower(const void* pow1, const void* pow2);
void printPoly(const Polynom* poly);
//void freePolynom(Polynom* poly);