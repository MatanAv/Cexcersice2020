#pragma once

#define CHECK_RETURN_NULL(p, msg) if (!p) {\
									puts(msg);\
									return NULL;\
									}

#define CHECK_RETURN_0(p, msg) if (!p) {\
								puts(msg);\
								return 0;\
								}

#define MSG_RETURN_0(msg) {\
							puts(msg);\
							return 0;\
							}

#define MSG_RETURN_NULL(msg) {\
								puts(msg);\
								return NULL;\
								}

typedef unsigned char BYTE;

typedef struct {
	int	upperLeftX;	//the col index of the upper left corner
	int	upperLeftY;	//the row index of the upper left corner
	int width;		//the width of the rect
	int height;		//the height of the rect
}Rect;


void	q1();
int*	getImage(const char* fileName, int* pRow, int* pCol);

int		checkIfAlreadyRect(const int* image, const int col, const int i, const int j);
Rect*	createRectArr(const int* image, int row, int col, int* pCount);
void	getSizeOfRect(const int* image, const int row, const int col, int* height, int* width, const int i, const int j);

int		saveRectToFile(const char* fileName, Rect* rectArr, int count);
int		saveCompressImageToFile(const char* fileName, const int* image, int row, int col);
