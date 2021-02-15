#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#include "q1.h"


void q1()
{
	int* image;
	Rect* rectArr;
	int row, col;
	int count;
	
	image = getImage("mat1.bin", &row, &col);
	if (!image)
		return;

	rectArr = createRectArr(image, row, col, &count);
	if (!rectArr)
		return;

	if (!saveRectToFile("rects.txt", rectArr, count))
		return;

	if (!saveCompressImageToFile("mat1_comp.bin", image, row, col))
		return;

}

int*	getImage(const char* fileName, int* pRow, int* pCol)
{
	FILE* fp = fopen(fileName,"rb");
	char msg[] = "Error reading file";
	int* image;
	int size;

	if (fread(pRow, sizeof(int), 1, fp) != 1)
		CHECK_RETURN_NULL(pRow, msg);
	if (fread(pCol, sizeof(int), 1, fp) != 1)
		CHECK_RETURN_NULL(pCol, msg);

	size = (*pRow) * (*pCol);

	image = (int*)calloc(size, sizeof(int));
	if (!image)
		CHECK_RETURN_NULL(pCol, msg);

	if (fread(image, sizeof(int), size, fp) != size)	// TODO: Check
		MSG_RETURN_NULL(msg);

	fclose(fp);
	return image;
}

Rect*	createRectArr(const int* image, int row, int col, int* pCount)
{
	Rect* rectArr = NULL;
	char msg[] = "Allocation Error";
	int countRect = 0;
	int width, height;

	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++)
			if (image[col * i + j] == 1)
				if (checkIfAlreadyRect(image, col, i, j))
				{
					countRect++;
					rectArr = (Rect*)realloc(rectArr, sizeof(Rect) * countRect);
					CHECK_RETURN_0(rectArr, msg);

					getSizeOfRect(image, row, col, &height, &width, i, j);
					rectArr[countRect - 1].upperLeftX = i;
					rectArr[countRect - 1].upperLeftY = j;
					rectArr[countRect - 1].height = height;
					rectArr[countRect - 1].width = width;
				}

	*pCount = countRect;
	return rectArr;
}


int		checkIfAlreadyRect(const int* image, const int col, const int i, const int j)	// Return == 1 means this is a rect
{																						// Return == 0 means to move towards the next cell
	// Upper border
	if (i == 0)
		if (j == 0)
			return 1;
		else if (image[col * i + (j - 1)] == 1)
			return 0;
		else
			return 1;

	// Left border
	else if (j == 0)
		if (image[col * (i - 1) + j] == 1)
			return 0;
		else
			return 1;

	// Not in the border
	else
		if (image[col * (i - 1) + j] == 1)
			return 0;
		if (image[col * i + (j - 1)] == 1)
			return 0;

	return 1;
}

void	getSizeOfRect(const int* image, const int row, const int col, int* height, int* width, const int i, const int j)
{
	int w = 1, h = 1;
	while ((j + w) < col && image[col * i + (j + w)] == 1)
		w++;
	while (((i + h) < row) && image[col * (i + h) + j] == 1)
		h++;
	*height = h;
	*width = w;
}


int		saveRectToFile(const char* fileName, Rect* rectArr, int count)
{
	char msg[] = "Error reading file";
	FILE* fp = fopen(fileName, "w");
	CHECK_RETURN_0(fp, msg);

	fprintf(fp, "%d\n", count);

	for (int i = 0; i < count; i++)
		fprintf(fp, "%d %d %d %d\n", rectArr[i].upperLeftX, rectArr[i].upperLeftY,
											rectArr[i].width, rectArr[i].height);

	fclose(fp);
	return 1;
}

int		saveCompressImageToFile(const char* fileName, const int* image, int row, int col)
{
	char msg[] = "Error writing file";
	char msgAlloc[] = "Allocation Error";
	FILE* fp = fopen(fileName, "wb");
	CHECK_RETURN_0(fp, msg);

	if (fwrite(&row, sizeof(int), 1, fp) != 1)
		MSG_RETURN_0(msg);
	if (fwrite(&col, sizeof(int), 1, fp) != 1)
		MSG_RETURN_0(msg);

	BYTE* binImage;
	int byteSize;

	if ((row * col) / 8 == 0) {
		byteSize = (row * col) / 8;
		binImage = (BYTE*)calloc(byteSize, sizeof(BYTE));
		CHECK_RETURN_0(binImage, msgAlloc);
	}
	else {
		byteSize = ((row * col) / 8) + 1;
		binImage = (BYTE*)calloc(byteSize, sizeof(BYTE));
		CHECK_RETURN_0(binImage, msgAlloc);
	}

	for (int i = 0; i < row*col; i++)
	{
		int steps = i % 8;

		if (image[i] == 1)
			binImage[i / 8] = (0x1 << steps) | binImage[i / 8];

		if (i > row * col - 1)
			break;
	}

	if (fread(binImage,sizeof(char), byteSize,fp) != byteSize)
		MSG_RETURN_0(msg);

	free(binImage);
	return 0;
}


