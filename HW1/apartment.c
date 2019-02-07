#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include "apartment.h"

/**---AUX FUNCTION---*/
/**
 * Checks if there is a a given square (row,col) is EMPTY
 * @return
 * TRUE if EMPTY. FALSE otherwise.
 */
static bool isEmpty(Apartment apartment, int row, int col) {
	SquareType squareVal = EMPTY;
	apartmentGetSquare(apartment, row, col, &squareVal);
	return (squareVal == EMPTY) ? true : false;
}

/**
 * Checks if there is a a given square (row,col) is in the apartment bound
 * @return
 * TRUE if in bound. FALSE otherwise.
 */
static bool isInBounds(Apartment apartment, int row, int col) {
	if (row < 0 || row >= apartmentGetLength(apartment) || col < 0
			|| col >= apartmentGetWidth(apartment)) {
		return false;
	}
	return true;
}

/**
 * Frees all memory allocated for the given SquareType matrix.
 * This function can receive NULL.
 */
static void squareDestroy(SquareType** square) {
	if (!square) {
		return;
	}
	free(square[0]);
	free(square);
}

/**
 * Creates a newly allocated copy of a given SquareType array.
 * @return
 * A newly allocated copy of the original SquareType array.
 * NULL if the array is NULL or in any case of memory allocation failure.
 */
static SquareType** copySquares(SquareType** originalSquares, int length, int width) {
	if (!originalSquares) {
		return NULL;
	}
	SquareType** newSquare = malloc(sizeof(SquareType**) * length);
	if (!newSquare) {
		return NULL;
	}
	newSquare[0] = malloc(sizeof(SquareType) * width * length);
	if (!newSquare[0]) {
		free(newSquare);
		return NULL;
	}
	for (int i = 0; i < length; i++) {
		newSquare[i] = (*newSquare + width * i);
	}
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < width; j++) {
			newSquare[i][j] = *(*originalSquares + (i * width) + j);
		}
	}
	return newSquare;
}
/**
 * Creates a newly allocated transposed version of given SquareType array.
 * @return
 * A newly allocated transposed version of given SquareType array.
 * NULL if the array is NULL or in any case of memory allocation failure.
 */
static SquareType** transpose(SquareType** squares, int length, int width) {
	if (!squares) {
		return NULL;
	}
	SquareType** newSquare = malloc(sizeof(SquareType**) * width);
	if (!newSquare) {
		return NULL;
	}
	newSquare[0] = malloc(sizeof(SquareType) * width * length);
	if (!newSquare[0]) {
		free(newSquare);
		return NULL;
	}
	for (int i = 0; i < width; i++) {
		newSquare[i] = (*newSquare + length * i);
	}
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < length; j++) {
			newSquare[i][j] = *((*squares) + (j * width) + i); //squares[j][i];
		}
	}
	return newSquare;
}

/**
 * Checks if there is a course of EMPTY spaces between (row1,col1) to
 * (row2,col2) in a SquareType matrix.
 * @return
 * TRUE if such a course exists. FALSE else.
 */
static bool isSameRoomAux(SquareType** square, int length, int width, int row1,
		int col1, int row2, int col2) {
	if (row1 == row2 && col1 == col2) {
		return true;
	}
	if (square[row1][col1] == WALL) {
		return false;
	}
	square[row1][col1] = WALL;
	if (((row1 < length - 1)
			&& isSameRoomAux(square, length, width, row1 + 1, col1, row2, col2))
			|| ((col1 < width - 1)
					&& isSameRoomAux(square, length, width, row1, col1 + 1,
							row2, col2))
			|| ((row1 > 0)
					&& isSameRoomAux(square, length, width, row1 - 1, col1,
							row2, col2))
			|| ((col1 > 0)
					&& isSameRoomAux(square, length, width, row1, col1 - 1,
							row2, col2))) {
		square[row1][col1] = EMPTY;
		return true;
	}
	square[row1][col1] = EMPTY;
	return false;
}

/**
 * Gets a square coordinates in a given SquareType matrix. if the square is a
 * part of a room (EMPTY), the entire room will be marked as walls.
 * @return
 * Marked room (as WALLs) for the given coordinates of an EMPTY square.
 * if the given square is a WALL, return.
 */
static int numOfRoomsAux(SquareType** square, int length, int width, int row,
																	int col) {
	if (square[row][col] == WALL) {
		return 0;
	}
	square[row][col] = WALL;
	return (((row < length - 1)
			&& numOfRoomsAux(square, length, width, row + 1, col))
			|| ((col < width - 1)
					&& numOfRoomsAux(square, length, width, row, col + 1))
			|| ((row > 0) && numOfRoomsAux(square, length, width, row - 1, col))
			|| ((col > 0) && numOfRoomsAux(square, length, width, row, col-1)));
}

/**
 * Calculates the price of a new allocated sub-apartment by:
 * old_price * (#rows in new apartment + 1) / #rows in original apartment.
 * @return
 * The price of the new apartment.
 */
static int calculatePrice(int oldPrice, int oldLength, int oldWidth, int newLength,
		int newWidth, bool splitByRow) {
	return splitByRow ?
			(oldPrice * (newLength + 1) / oldLength) :
			(oldPrice * (newWidth + 1) / oldWidth);
}

/**
 * Auxiliary function that transpose a SquareType matrix and handle the memory
 *  allocation of a given apartment
 * @return
 * APARTMENT_NULL_ARG in any case of an allocation failure,
 * APARTMENT_SUCCESS otherwise.
 */
static ApartmentResult handleSplitByCol(Apartment apartment, int length, int width) {
	if (!apartment) {
		return APARTMENT_NULL_ARG;
	}
	SquareType** ptr = transpose((apartment->squares), length, width);
	if (!ptr) {
		return APARTMENT_NULL_ARG;
	}
	apartment->width = length;
	apartment->length = width;
	squareDestroy(apartment->squares);
	apartment->squares = ptr;

	return APARTMENT_SUCCESS;
}
/* ---AUX FUNCTION END---*/
Apartment apartmentCreate(SquareType** squares, int length, int width,
		int price) {
	if (!squares || length <= 0 || width <= 0 || price < 0) {
		return NULL;
	}
	Apartment newApartment = malloc(sizeof(*newApartment));
	if (!newApartment) {
		return NULL;
	}
	newApartment->length = length;
	newApartment->width = width;
	newApartment->price = price;
	newApartment->squares = copySquares(squares, length, width);
	if (!squares) {
		free(newApartment);
		return NULL;
	}
	return newApartment;
}

void apartmentDestroy(Apartment apartment) {
	if (!apartment) {
		return;
	}
	squareDestroy(apartment->squares);
	free(apartment);
}

Apartment apartmentCopy(Apartment apartment) {
	if (!apartment) {
		return NULL;
	}
	Apartment copy = apartmentCreate(apartment->squares, apartment->length,
			apartment->width, apartment->price);
	if (!copy) {
		return NULL;
	}
	return copy;
}

ApartmentResult apartmentIsSameRoom(Apartment apartment, int row1, int col1,
		int row2, int col2, bool* outResult) {
	if (!apartment || !outResult) {
		return APARTMENT_NULL_ARG;
	}
	if (!isInBounds(apartment, row1, col1)
			|| !isInBounds(apartment, row2, col2)) {
		return APARTMENT_OUT_OF_BOUNDS;
	}
	if (!isEmpty(apartment, row1, col1) || !isEmpty(apartment, row2, col2)) {
		return APARTMENT_NO_ROOM;
	}
	*outResult = isSameRoomAux(apartment->squares,
			apartmentGetLength(apartment), apartmentGetWidth(apartment), row1,
			col1, row2, col2);
	return APARTMENT_SUCCESS;
}

int apartmentTotalArea(Apartment apartment) {
	assert(apartment != NULL);
	int emptyCells = 0;
	SquareType squareVal = EMPTY;
	int length = apartmentGetLength(apartment);
	int width = apartmentGetWidth(apartment);
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < width; j++) {
			apartmentGetSquare(apartment, i, j, &squareVal);
			if (squareVal == EMPTY) {
				emptyCells++;
			}
		}
	}
	return emptyCells;
}

ApartmentResult apartmentRoomArea(Apartment apartment, int row, int col,
		int* outArea) {
	if (!apartment || !outArea) {
		return APARTMENT_NULL_ARG;
	}
	if (!isInBounds(apartment, row, col)) {
		return APARTMENT_OUT_OF_BOUNDS;
	}
	if (!isEmpty(apartment, row, col)) {
		return APARTMENT_NO_ROOM;
	}
	int length = apartmentGetLength(apartment);
	int width = apartmentGetWidth(apartment);
	int count = 0;
	bool outResult;
	SquareType outValue;
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < width; j++) {
			apartmentGetSquare(apartment, i, j, &outValue);
			apartmentIsSameRoom(apartment, i, j, row, col, &outResult);
			if (outValue == EMPTY && outResult) {
				count++;
			}
		}
	}
	*outArea = count;
	return APARTMENT_SUCCESS;
}

ApartmentResult apartmentSplit(Apartment apartment, bool splitByRow, int index,
		Apartment* first, Apartment* second) {
	if (!apartment) {
		return APARTMENT_NULL_ARG;
	}
	int oldLength = apartmentGetLength(apartment);
	int oldWidth = apartmentGetWidth(apartment);
	if (index < 0 || index > (splitByRow ? (oldLength - 1) : (oldWidth - 1))) {
		return APARTMENT_OUT_OF_BOUNDS;
	}
	if (index == 0
			|| index == (splitByRow ? (oldLength - 1) : (oldWidth - 1))) {
		return APARTMENT_BAD_SPLIT;
	}
	SquareType outVal = EMPTY;
	for (int k = 0; k < (splitByRow ? oldWidth : oldLength); k++) {
		apartmentGetSquare(apartment, splitByRow ? index : k,
				splitByRow ? k : index, &outVal);
		if (outVal == EMPTY) {
			return APARTMENT_BAD_SPLIT;
		}
	}
	SquareType** trans = transpose(apartment->squares, oldLength, oldWidth);

	int newLength = (splitByRow ? index : oldLength);
	int newWidth = (splitByRow ? oldWidth : index);
	int apartmentPrice = calculatePrice(apartmentGetPrice(apartment), oldLength,
			oldWidth, newLength, newWidth, splitByRow);
	newLength = (splitByRow ? index : index);
	newWidth = (splitByRow ? oldWidth : oldLength);
	*first = apartmentCreate((splitByRow ? (apartment->squares) : trans),
			newLength, newWidth, apartmentPrice);
	if (!first) {
		return APARTMENT_NULL_ARG;
	}
	if (!splitByRow) {
		if ((handleSplitByCol((*first), newLength, newWidth))
				== APARTMENT_NULL_ARG) {
			apartmentDestroy(*first);
			squareDestroy(trans);
			return APARTMENT_NULL_ARG;
		}
	}

	newLength = (splitByRow ? (oldLength - index - 1) : oldLength);
	newWidth = (splitByRow ? oldWidth : (oldWidth - index - 1));
	apartmentPrice = calculatePrice(apartmentGetPrice(apartment), oldLength,
			oldWidth, newLength, newWidth, splitByRow);
	newLength = (splitByRow ? (oldLength - index - 1) : (oldWidth - index - 1));
	newWidth = (splitByRow ? oldWidth : oldLength);
	*second =
			apartmentCreate(
					(splitByRow ?
							(apartment->squares + index + 1) :
							(trans + index + 1)), newLength, newWidth,
					apartmentPrice);

	if (!second) {
		apartmentDestroy(*first);
		return APARTMENT_NULL_ARG;
	}
	if (!splitByRow) {
		if ((handleSplitByCol((*second), newLength, newWidth))
				== APARTMENT_NULL_ARG) {
			apartmentDestroy(*first);
			apartmentDestroy(*second);
			squareDestroy(trans);
			return APARTMENT_NULL_ARG;
		}
	}
	squareDestroy(trans);
	return APARTMENT_SUCCESS;
}

int apartmentNumOfRooms(Apartment apartment) {
	assert(apartment != NULL);
	int length = apartmentGetLength(apartment);
	int width = apartmentGetWidth(apartment);
	SquareType** copy = copySquares(apartment->squares, length, width);
	int roomNum = 0;
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < width; j++) {
			if (copy[i][j] == EMPTY) {
				roomNum++;
			}
			numOfRoomsAux(copy, length, width, i, j);
		}
	}
	squareDestroy(copy);
	return roomNum;
}

ApartmentResult apartmentSetSquare(Apartment apartment, int row, int col,
		SquareType value) {
	if (!apartment) {
		return APARTMENT_NULL_ARG;
	}
	if (!isInBounds(apartment, row, col)) {
		return APARTMENT_OUT_OF_BOUNDS;
	}
	if (apartment->squares[row][col] == value) {
		return APARTMENT_OLD_VALUE;
	}
	apartment->squares[row][col] = value;
	return APARTMENT_SUCCESS;
}

ApartmentResult apartmentGetSquare(Apartment apartment, int row, int col,
		SquareType* outValue) {
	if (!apartment) {
		return APARTMENT_NULL_ARG;
	}
	if (!isInBounds(apartment, row, col)) {
		return APARTMENT_OUT_OF_BOUNDS;
	}
	*outValue = apartment->squares[row][col];
	return APARTMENT_SUCCESS;
}

ApartmentResult apartmentChangePrice(Apartment apartment, int percent) {
	if (!apartment) {
		return APARTMENT_NULL_ARG;
	}
	if (percent < -100) {
		return APARTMENT_PRICE_NOT_IN_RANGE;
	}
	int currentPrice = apartmentGetPrice(apartment);
	int priceDiff = (currentPrice * percent) / 100;
	apartment->price = currentPrice + priceDiff;
	return APARTMENT_SUCCESS;
}

int apartmentGetPrice(Apartment apartment) {
	assert(apartment != NULL);
	return apartment->price;
}

int apartmentGetLength(Apartment apartment) {
	assert(apartment != NULL);
	return apartment->length;
}

int apartmentGetWidth(Apartment apartment) {
	assert(apartment != NULL);
	return apartment->width;
}

bool apartmentIsIdentical(Apartment apartment1, Apartment apartment2) {
	if (!apartment1 && !apartment2) {
		return true;
	}
	if ((!apartment1 && apartment2) || (apartment1 && !apartment2)) {
		return false;
	}
	if ((apartmentGetLength(apartment1) != apartmentGetLength(apartment2))
			|| (apartmentGetWidth(apartment1) != apartmentGetWidth(apartment2))
			|| (apartmentGetPrice(apartment1) != apartmentGetPrice(apartment2))) {
		return false;
	}
	SquareType squareVal1 = EMPTY;
	SquareType squareVal2 = EMPTY;
	for (int i = 0; i < apartmentGetLength(apartment1); i++) {
		for (int j = 0; j < apartmentGetWidth(apartment1); j++) {
			apartmentGetSquare(apartment1, i, j, &squareVal1);
			apartmentGetSquare(apartment2, i, j, &squareVal2);
			if (squareVal1 != squareVal2) {
				return false;
			}
		}
	}
	return true;
}

