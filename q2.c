#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>

#include "q2.h"

void q2()
{
	//Unremark to check
	
	printf("\n---------- 1 ----------- \n");
	if (!createAndPrintPoly(6, 2, 4, -5, 1, 0, 6, 6, 4, -8, 0, 7, 3))
		puts("Error");
	printf("\n---------- 2 ----------- \n");
	if (!createAndPrintPoly(7, 1, 4, -5, 4, 2, 3, 6, 6, -8, 0, 7, 3,9,2))
		puts("Error");
}

int createAndPrintPoly(int length, float coef, int pw, ...)
{
	Polynom poly;
	poly.size = length;
	poly.monoms = NULL;
	Monom* monomArr = NULL;
	int monomCount = 0;

	float coefficient = coef;
	int pow = pw;

	va_list monomList;
	va_start(monomList, pw);

	for (int i = 0; i < length; i++)
	{
		if (coefficient != 0)
			if (!insertMonomToMonomArr(monomArr, &monomCount, coefficient, pow))
				return 0;

		coefficient = va_arg(monomList, float);
		pow = va_arg(monomList, int);
	}

	if (monomArr != NULL)		// Sort monoms from low to high
		qsort(monomArr, monomCount, sizeof(Monom), compareByPower);

	for (int i = monomCount; i > 0; i--)		// Chain them from high to low
		monomArr[i].next = &monomArr[i - 1];

	poly.monoms = monomArr+monomCount;		// List starts from the highest

	printPoly(&poly);

	va_end(monomList);
	//freePolynom(&poly);
	return 1;
}

int insertMonomToMonomArr(Monom* monomArr, int* monomCount, const float coefficient, const int pow)
{

	if (*monomCount == 0) {			// Polinom is empty
		monomArr = (Monom*)malloc(sizeof(Monom));
		if (!monomArr)
			return 0;
		monomArr[0].coefficient = coefficient;
		monomArr[0].power = pow;
		monomArr[0].next = NULL;
		(*monomCount)++;
	}

	else {
		int equalPow = 0;
		int i = 0;

		while (!equalPow && i < *monomCount) {		// Power already in the polynom
			if (monomArr[i].power == pow) {
				monomArr[i].coefficient += coefficient;
				equalPow = 1;
			}
			i++;
		}

		if (!equalPow) {		// Power not in the polynom
			(*monomCount)++;
			monomArr = (Monom*)realloc(monomArr, ((*monomCount) * sizeof(Monom)));
			if (!monomArr)
				return 0;
			monomArr[*monomCount].coefficient = coefficient;
			monomArr[*monomCount].power = pow;
			monomArr[*monomCount].next = NULL;
		}

	}

	return 1;
}

int compareByPower(const void* pow1, const void* pow2)
{
	int p1 = (*(int*)pow1);
	int p2 = (*(int*)pow2);
	return p1 - p2;
}

void printPoly(const Polynom* poly)
{
	if (poly->monoms == NULL)
		return;

	Monom* temp = poly->monoms;
	if (!temp)
		puts("Empty polynom");

	while (temp->next != NULL)
	{
		if (temp->power == 1)
			if (temp->coefficient > 0)
				printf("+%fX", temp->coefficient);
			else
				printf("%fX", temp->coefficient);

		else if (temp->power == 0)
			if (temp->coefficient > 0)
				printf("+%f", temp->coefficient);
			else
				printf("%f", temp->coefficient);

		else
			if (temp->coefficient > 0)
				printf("+%fX^%d", temp->coefficient, temp->power);
			else
				printf("%fX^%d", temp->coefficient, temp->power);

		temp = temp->next;
	}

}