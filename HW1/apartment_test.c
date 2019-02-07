#include "aux_macros.h"
#include "apartment.h"
#include <stdlib.h>

/**
 * Prints a given apartment layout (for debbuging...)
 */
void printSquares(Apartment apartment) {
	printf("\n***********\n");
	SquareType value = EMPTY;
	for (int i = 0; i < apartmentGetLength(apartment); i++) {
		for (int j = 0; j < apartmentGetWidth(apartment); j++) {
			apartmentGetSquare(apartment, i, j, &value);
			printf(" %c", value == EMPTY ? 'E' : 'W');
		}
		printf("\n");
	}
	printf("***********\n");
}

bool apartmentCreate_test1() {
	bool final = true;

	SquareType square[6] = { WALL, WALL, EMPTY, WALL, EMPTY, EMPTY };
	SquareType* squarePtr = square;
	//negative length test
	Apartment apartment = apartmentCreate(&squarePtr, -3, 2, 1000000);
	TEST_EQUALS(final, NULL, apartment);
	//not positive width test
	apartment = apartmentCreate(&squarePtr, 3, 0, 1000000);
	TEST_EQUALS(final, NULL, apartment);
	//NULL squares array test
	apartment = apartmentCreate(NULL, 3, 2, 1000000);
	TEST_EQUALS(final, NULL, apartment);
	//negative price test
	apartment = apartmentCreate(&squarePtr, 3, 2, -20);
	TEST_EQUALS(final, NULL, apartment);
	//not positive price test - should work!
	apartment = apartmentCreate(&squarePtr, 3, 2, 0);
	TEST_DIFFERENT(final, NULL, apartment);
	apartmentDestroy(apartment);
	return final;
}

bool apartmentCreate_test2() {
	bool final = true;

	SquareType square[6] = { WALL, WALL, EMPTY, WALL, EMPTY, EMPTY };
	SquareType* squarePtr = square;

	Apartment apartment = apartmentCreate(&squarePtr, 3, 2, 1000001);

	TEST_EQUALS(final, 3, apartmentGetLength(apartment));
	TEST_EQUALS(final, 2, apartmentGetWidth(apartment));
	TEST_EQUALS(final, 1000001, apartmentGetPrice(apartment));
	apartmentDestroy(apartment);

	return final;
}

bool apartmentDestroy_test1() {
	bool final = true;
	//trying to destroy a NULL pointer
	Apartment apartment = NULL;
	apartmentDestroy(apartment);

	return final;
}

bool apartmentDestroy_test2() {
	bool final = true;

	SquareType square[2] = { WALL, EMPTY };
	SquareType* squarePtr = square;

	Apartment apartment = apartmentCreate(&squarePtr, 1, 2, 100);
	apartmentDestroy(apartment);

	return final;
}

bool apartmentGetLength_test() {
	bool final = true;

	SquareType square[6] = { WALL, WALL, EMPTY, WALL, EMPTY, EMPTY };
	SquareType* squarePtr = square;

	Apartment apartment = apartmentCreate(&squarePtr, 3, 2, 1000001);

	TEST_EQUALS(final, 3, apartmentGetLength(apartment));

	apartmentDestroy(apartment);

	return final;
}

bool apartmentGetWidth_test() {
	bool final = true;

	SquareType square[6] = { WALL, WALL, EMPTY, WALL, EMPTY, EMPTY };
	SquareType* squarePtr = square;

	Apartment apartment = apartmentCreate(&squarePtr, 3, 2, 1000001);

	TEST_EQUALS(final, 2, apartmentGetWidth(apartment));

	apartmentDestroy(apartment);

	return final;
}

bool apartmentGetPrice_test() {
	bool final = true;

	SquareType square[6] = { WALL, WALL, EMPTY, WALL, EMPTY, EMPTY };
	SquareType* squarePtr = square;

	Apartment apartment = apartmentCreate(&squarePtr, 3, 2, 123456);

	TEST_EQUALS(final, 123456, apartmentGetPrice(apartment));

	apartmentDestroy(apartment);

	return final;
}

bool apartmentChangePrice_test() {
	bool final = true;

	SquareType square[6] = { WALL, WALL, EMPTY, WALL, EMPTY, EMPTY };
	SquareType* squarePtr = square;

	//null argument test
	TEST_EQUALS(final, APARTMENT_NULL_ARG, apartmentChangePrice(NULL, 50));

	Apartment apartment = apartmentCreate(&squarePtr, 3, 2, 35);

	//percent < -100 test
	TEST_EQUALS(final, APARTMENT_PRICE_NOT_IN_RANGE,
			apartmentChangePrice(apartment, -101));

	//add 50% test. should be 52 after rounding
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, 50));
	TEST_EQUALS(final, 52, apartmentGetPrice(apartment));

	//Subtract 33% test. should be 35 after rounding
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, -33));
	TEST_EQUALS(final, 35, apartmentGetPrice(apartment));

	//Subtract 50% test. should be 35 after rounding
	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, -50));
	TEST_EQUALS(final, 18, apartmentGetPrice(apartment));

	apartmentDestroy(apartment);

	return final;
}

bool apartmentTotalArea_test() {
	bool final = true;

	SquareType square1[9] = { WALL, WALL, WALL, WALL, EMPTY, EMPTY, WALL, EMPTY,
			EMPTY };
	SquareType* squarePtr1 = square1;
	Apartment apartment1 = apartmentCreate(&squarePtr1, 3, 3, 1000001);

	TEST_EQUALS(final, 4, apartmentTotalArea(apartment1));

	SquareType square2[9] = { WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL,
			WALL };
	SquareType* squarePtr2 = square2;
	Apartment apartment2 = apartmentCreate(&squarePtr2, 3, 3, 1000001);

	TEST_EQUALS(final, 0, apartmentTotalArea(apartment2));

	apartmentDestroy(apartment1);
	apartmentDestroy(apartment2);

	return final;
}

bool apartmentGetSquare_test() {
	bool final = true;

	SquareType square[6] = { WALL, WALL, EMPTY, WALL, EMPTY, EMPTY };
	SquareType* squarePtr = square;
	SquareType value = EMPTY;

	Apartment apartment = NULL;
	TEST_EQUALS(final, APARTMENT_NULL_ARG,
			apartmentGetSquare(apartment, 0, 0, &value));

	apartment = apartmentCreate(&squarePtr, 3, 2, 1000001);

	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentGetSquare(apartment, -1, 0, &value));
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentGetSquare(apartment, 3, 1, &value));
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentGetSquare(apartment, 1, -1, &value));
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentGetSquare(apartment, 2, 2, &value));

	TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentGetSquare(apartment, 0, 0, &value));
	TEST_EQUALS(final, WALL, value);

	TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentGetSquare(apartment, 2, 1, &value));
	TEST_EQUALS(final, EMPTY, value);

	apartmentDestroy(apartment);

	return final;
}

bool apartmentSetSquare_test() {
	bool final = true;

	SquareType square[6] = { WALL, WALL, EMPTY, WALL, EMPTY, EMPTY };
	SquareType* squarePtr = square;
	SquareType value = EMPTY;

	Apartment apartment = NULL;
	TEST_EQUALS(final, APARTMENT_NULL_ARG,
			apartmentSetSquare(apartment, 0, 0, value));

	apartment = apartmentCreate(&squarePtr, 3, 2, 1000001);

	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentSetSquare(apartment, -1, 0, value));
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentSetSquare(apartment, 3, 1, value));
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentSetSquare(apartment, 1, -1, value));
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentSetSquare(apartment, 2, 2, value));

	value = WALL;
	TEST_EQUALS(final, APARTMENT_OLD_VALUE,
			apartmentSetSquare(apartment, 0, 0, value));

	value = EMPTY;
	TEST_EQUALS(final, APARTMENT_OLD_VALUE,
			apartmentSetSquare(apartment, 2, 1, value));

	TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentSetSquare(apartment, 0, 0, value));
	TEST_EQUALS(final, EMPTY, value);

	value = WALL;
	TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentSetSquare(apartment, 2, 1, value));
	TEST_EQUALS(final, WALL, value);

	apartmentDestroy(apartment);

	return final;
}

bool apartmentIsSameRoom_test() {
	bool final = true;

	SquareType square[24] = { EMPTY, EMPTY, WALL, EMPTY, WALL, EMPTY, EMPTY,
			EMPTY, WALL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL, WALL, WALL,
			EMPTY, EMPTY, EMPTY, WALL, EMPTY, EMPTY, EMPTY };
	SquareType* squarePtr = square;
	bool OutRes = false;

// * 	APARTMENT_NULL_ARG if apartment is NULL or outResult is NULL
	Apartment apartment = NULL;
	TEST_EQUALS(final, APARTMENT_NULL_ARG,
			apartmentIsSameRoom(apartment, 1, 0, 3, 1, &OutRes));
	apartment = apartmentCreate(&squarePtr, 4, 6, 1000001);
	TEST_EQUALS(final, APARTMENT_NULL_ARG,
			apartmentIsSameRoom(apartment, 1, 0, 3, 1, NULL));
// * 	APARTMENT_OUT_OF_BOUNDS if one the coordinates is illegal.
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentIsSameRoom(apartment, -1, 0, 3, 1, &OutRes));
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentIsSameRoom(apartment, 1, 0, 3, -1, &OutRes));
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentIsSameRoom(apartment, 1, 0, 4, 1, &OutRes));
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentIsSameRoom(apartment, 1, 6, 2, 1, &OutRes));
// *	APARTMENT_NO_ROOM if one if the requested squares is a wall.	
	TEST_EQUALS(final, APARTMENT_NO_ROOM,
			apartmentIsSameRoom(apartment, 0, 2, 1, 3, &OutRes));
	TEST_EQUALS(final, APARTMENT_NO_ROOM,
			apartmentIsSameRoom(apartment, 0, 0, 2, 2, &OutRes));
// * 	MESSAGE_SUCCESS otherwise
	TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentIsSameRoom(apartment, 3, 1, 1, 0, &OutRes));
	TEST_EQUALS(final, true, OutRes);
	TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentIsSameRoom(apartment, 1, 3, 3, 3, &OutRes));
	TEST_EQUALS(final, true, OutRes);
	TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentIsSameRoom(apartment, 1, 1, 1, 1, &OutRes));
	TEST_EQUALS(final, true, OutRes);
	TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentIsSameRoom(apartment, 1, 0, 1, 3, &OutRes));
	TEST_EQUALS(final, false, OutRes);
	TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentIsSameRoom(apartment, 3, 1, 1, 3, &OutRes));
	TEST_EQUALS(final, false, OutRes);
	TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentIsSameRoom(apartment, 3, 3, 1, 3, &OutRes));
	TEST_EQUALS(final, true, OutRes);
	apartmentDestroy(apartment);

	return final;
}

bool apartmentRoomArea_test() {
	bool final = true;

	SquareType square[24] = { EMPTY, EMPTY, WALL, EMPTY, WALL, EMPTY, EMPTY,
			EMPTY, WALL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL, WALL, WALL,
			EMPTY, EMPTY, EMPTY, WALL, EMPTY, EMPTY, EMPTY };
	SquareType* squarePtr = square;
	int outArea = 0;

// * 	APARTMENT_NULL_ARG if apartment is NULL or outArea is NULL
	Apartment apartment = NULL;
	TEST_EQUALS(final, APARTMENT_NULL_ARG,
			apartmentRoomArea(apartment, 1, 1, &outArea));
	apartment = apartmentCreate(&squarePtr, 4, 6, 1000001);
	TEST_EQUALS(final, APARTMENT_NULL_ARG,
			apartmentRoomArea(apartment, 1, 1, NULL));
// * 	APARTMENT_OUT_OF_BOUNDS if the coordinates are illegal.
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentRoomArea(apartment, -1, 1, &outArea));
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentRoomArea(apartment, 1, -1, &outArea));
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentRoomArea(apartment, 4, 1, &outArea));
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentRoomArea(apartment, 1, 6, &outArea));
// *	APARTMENT_NO_ROOM if one if the requested squares is a wall.	
	TEST_EQUALS(final, APARTMENT_NO_ROOM,
			apartmentRoomArea(apartment, 2, 2, &outArea));
// * 	MESSAGE_SUCCESS otherwise
	TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentRoomArea(apartment, 1, 0, &outArea));
	TEST_EQUALS(final, 8, outArea);
	TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentRoomArea(apartment, 2, 1, &outArea));
	TEST_EQUALS(final, 8, outArea);
	TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentRoomArea(apartment, 0, 0, &outArea));
	TEST_EQUALS(final, 8, outArea);
	TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentRoomArea(apartment, 1, 3, &outArea));
	TEST_EQUALS(final, 9, outArea);
	TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentRoomArea(apartment, 3, 5, &outArea));
	TEST_EQUALS(final, 9, outArea);

	apartmentDestroy(apartment);

	return final;
}

bool apartmentNumOfRooms_test() {
	bool final = true;

	SquareType square1[24] = { EMPTY, EMPTY, WALL, EMPTY, WALL, EMPTY, EMPTY,
			EMPTY, WALL, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WALL, WALL, WALL,
			EMPTY, EMPTY, EMPTY, WALL, EMPTY, EMPTY, EMPTY };
	SquareType* squarePtr1 = square1;
	Apartment apartment1 = apartmentCreate(&squarePtr1, 4, 6, 1000001);
	TEST_EQUALS(final, 2, apartmentNumOfRooms(apartment1));

	SquareType square2[1] = { WALL };
	SquareType* squarePtr2 = square2;

	Apartment apartment2 = apartmentCreate(&squarePtr2, 1, 1, 100);
	TEST_EQUALS(final, 0, apartmentNumOfRooms(apartment2));
	SquareType square3[1] = { EMPTY };
	SquareType* squarePtr3 = square3;

	Apartment apartment3 = apartmentCreate(&squarePtr3, 1, 1, 100);
	TEST_EQUALS(final, 1, apartmentNumOfRooms(apartment3));
	SquareType square4[24] = { EMPTY, EMPTY, WALL, EMPTY, WALL, EMPTY, EMPTY,
			EMPTY, WALL, EMPTY, EMPTY, EMPTY, WALL, WALL, WALL, WALL, WALL,
			WALL, EMPTY, EMPTY, WALL, EMPTY, EMPTY, EMPTY };
	SquareType* squarePtr4 = square4;

	Apartment apartment4 = apartmentCreate(&squarePtr4, 4, 6, 1000001);
	TEST_EQUALS(final, 4, apartmentNumOfRooms(apartment4));
	apartmentDestroy(apartment1);
	apartmentDestroy(apartment2);
	apartmentDestroy(apartment3);
	apartmentDestroy(apartment4);

	return final;
}

bool apartmentSplit_test1() {
	bool final = true;

	SquareType square[24] = { EMPTY, EMPTY, WALL, EMPTY, WALL, EMPTY, EMPTY,
			EMPTY, WALL, EMPTY, EMPTY, EMPTY, WALL, WALL, WALL, WALL, WALL,
			WALL, EMPTY, EMPTY, WALL, EMPTY, EMPTY, EMPTY };
	SquareType* squarePtr = square;

// * 	APARTMENT_NULL_ARG if apartment, first or second are NULL.
	Apartment apartment = apartmentCreate(&squarePtr, 4, 6, 50);
	Apartment first = NULL;
	Apartment second = NULL;
	TEST_EQUALS(final, APARTMENT_NULL_ARG,
			apartmentSplit(NULL, true, 2, &first, &second));
// * 	APARTMENT_OUT_OF_BOUNDS if the given row or colomn index exceeds the bounds
// *	of the apartment. 
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentSplit(apartment, true, -1, &first, &second));
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentSplit(apartment, false, -1, &first, &second));
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentSplit(apartment, true, 4, &first, &second));
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentSplit(apartment, false, 6, &first, &second));
	//should return APARTMENT_BAD_SPLIT
	TEST_DIFFERENT(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentSplit(apartment, false, 4, &first, &second));
// APARTMENT_BAD_SPLIT if the requested split will result with at least one
// empty apartment, or the splitting row or column contains an empty sqaure.
	TEST_EQUALS(final, APARTMENT_BAD_SPLIT,
			apartmentSplit(apartment, true, 1, &first, &second));
	TEST_EQUALS(final, APARTMENT_BAD_SPLIT,
			apartmentSplit(apartment, false, 3, &first, &second));
	TEST_EQUALS(final, APARTMENT_BAD_SPLIT,
			apartmentSplit(apartment, false, 4, &first, &second));
// * 	MESSAGE_SUCCESS otherwise
	TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentSplit(apartment, true, 2, &first, &second));
	//first
	TEST_EQUALS(final, 2, apartmentGetLength(first));
	TEST_EQUALS(final, 6, apartmentGetWidth(first));
	TEST_EQUALS(final, 37, apartmentGetPrice(first));
	TEST_EQUALS(final, 9, apartmentTotalArea(first));

	//second
	TEST_EQUALS(final, 1, apartmentGetLength(second));
	TEST_EQUALS(final, 6, apartmentGetWidth(second));
	TEST_EQUALS(final, 25, apartmentGetPrice(second));
	TEST_EQUALS(final, 5, apartmentTotalArea(second));

	apartmentDestroy(first);
	apartmentDestroy(second);

	TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentSplit(apartment, false, 2, &first, &second));
	//first
	TEST_EQUALS(final, 4, apartmentGetLength(first));
	TEST_EQUALS(final, 2, apartmentGetWidth(first));
	TEST_EQUALS(final, 25, apartmentGetPrice(first));
	TEST_EQUALS(final, 6, apartmentTotalArea(first));
	//second
	TEST_EQUALS(final, 4, apartmentGetLength(second));
	TEST_EQUALS(final, 3, apartmentGetWidth(second));
	TEST_EQUALS(final, 33, apartmentGetPrice(second));
	TEST_EQUALS(final, 8, apartmentTotalArea(second));

	apartmentDestroy(first);
	apartmentDestroy(second);
	apartmentDestroy(apartment);

	return final;
}

bool apartmentSplit_test2() {
	bool final = true;

	SquareType square1[16] = { WALL, WALL, WALL, WALL, WALL, EMPTY, EMPTY, WALL,
			WALL, EMPTY, EMPTY, WALL, WALL, WALL, WALL, WALL, };
	SquareType* squarePtr1 = square1;
	Apartment apartment = NULL;
	Apartment first = NULL;
	Apartment second = NULL;
	apartment = apartmentCreate(&squarePtr1, 4, 4, 50);
// APARTMENT_OUT_OF_BOUNDS if the given row or column index exceeds the bounds
// of the apartment.
	TEST_EQUALS(final, APARTMENT_BAD_SPLIT,
			apartmentSplit(apartment, true, 0, &first, &second));
	TEST_EQUALS(final, APARTMENT_BAD_SPLIT,
			apartmentSplit(apartment, false, 0, &first, &second));
	TEST_EQUALS(final, APARTMENT_BAD_SPLIT,
			apartmentSplit(apartment, true, 3, &first, &second));
	TEST_DIFFERENT(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentSplit(apartment, false, 3, &first, &second));

	apartmentDestroy(apartment);
	apartmentDestroy(first);
	apartmentDestroy(second);

	SquareType square2[9] = { WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL,
			WALL };
	SquareType* squarePtr2 = square2;
	apartment = apartmentCreate(&squarePtr2, 3, 3, 50);
	TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentSplit(apartment, false, 1, &first, &second));
	//first
	TEST_EQUALS(final, 3, apartmentGetLength(first));
	TEST_EQUALS(final, 1, apartmentGetWidth(first));
	TEST_EQUALS(final, 33, apartmentGetPrice(first));
	TEST_EQUALS(final, 0, apartmentTotalArea(first));
	//second
	TEST_EQUALS(final, 3, apartmentGetLength(second));
	TEST_EQUALS(final, 1, apartmentGetWidth(second));
	TEST_EQUALS(final, 33, apartmentGetPrice(second));
	TEST_EQUALS(final, 0, apartmentTotalArea(second));

	apartmentDestroy(apartment);
	apartmentDestroy(first);
	apartmentDestroy(second);

	return final;
}

bool apartmentIsIdentical_test() {
	bool final = true;

	SquareType square[9] = { WALL, WALL, WALL, WALL, EMPTY, EMPTY, WALL, EMPTY,
			EMPTY };
	SquareType* squarePtr = square;
	Apartment apartment1 = NULL;
	Apartment apartment2 = NULL;
	//two NULLs test
	TEST_EQUALS(final, true, apartmentIsIdentical(apartment1, apartment2));
	apartment1 = apartmentCreate(&squarePtr, 3, 3, 1000001);
	//one NULL and the other ain't test
	TEST_EQUALS(final, false, apartmentIsIdentical(apartment1, apartment2));
	apartment2 = apartmentCreate(&squarePtr, 3, 3, 1000001);
	//two identical apartments test
	TEST_EQUALS(final, true, apartmentIsIdentical(apartment1, apartment2));
	apartmentDestroy(apartment1);
	apartmentDestroy(apartment2);

	return final;
}

bool apartmentCopy_test() {
	bool final = true;

	SquareType square[12] = { WALL, WALL, WALL, WALL, EMPTY, WALL, WALL, WALL,
			EMPTY, WALL, WALL, EMPTY };
	SquareType* squarePtr1 = square;
	Apartment apartment1 = apartmentCreate(&squarePtr1, 3, 4, 1000001);
	Apartment copy = apartmentCopy(apartment1);
	TEST_EQUALS(final, true, apartmentIsIdentical(apartment1, copy));

	apartmentDestroy(apartment1);
	apartmentDestroy(copy);

	return final;
}

bool apartment_combo_test() {
	bool final = true;

	SquareType square[2] = { WALL, EMPTY };
	SquareType* squarePtr = square;

	Apartment apartment = apartmentCreate(&squarePtr, 1, 2, 100);

	bool result;
	TEST_EQUALS(final, APARTMENT_NULL_ARG,
			apartmentIsSameRoom(NULL, 0, 1, 0, 1, &result));
	SquareType squareVal;
	TEST_EQUALS(final, APARTMENT_OUT_OF_BOUNDS,
			apartmentGetSquare(apartment, 0, 2, &squareVal));
	TEST_EQUALS(final, 1, apartmentTotalArea(apartment));
	int area;
	TEST_EQUALS(final, APARTMENT_NO_ROOM,
			apartmentRoomArea(apartment, 0, 0, &area));
	TEST_EQUALS(final, APARTMENT_SUCCESS,
			apartmentRoomArea(apartment, 0, 1, &area));
	TEST_EQUALS(final, 1, area);
	Apartment split1 = NULL, split2 = NULL;
	TEST_EQUALS(final, APARTMENT_BAD_SPLIT,
			apartmentSplit(apartment, false, 1, &split1, &split2));

	TEST_EQUALS(final, 1, apartmentNumOfRooms(apartment));
	TEST_EQUALS(final, APARTMENT_OLD_VALUE,
			apartmentSetSquare(apartment, 0, 0, WALL));
	TEST_EQUALS(final, 2, apartmentGetWidth(apartment));

	TEST_EQUALS(final, APARTMENT_SUCCESS, apartmentChangePrice(apartment, 10));
	TEST_EQUALS(final, 110, apartmentGetPrice(apartment));

	Apartment copy = apartmentCopy(apartment);
	TEST_EQUALS(final, true, apartmentIsIdentical(apartment, copy));

	apartmentDestroy(apartment);
	apartmentDestroy(copy);

	return final;
}

int main() {
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	RUN_TEST(apartmentCreate_test1);
	RUN_TEST(apartmentCreate_test2);
	RUN_TEST(apartmentDestroy_test1);
	RUN_TEST(apartmentDestroy_test2);
	RUN_TEST(apartmentGetLength_test);
	RUN_TEST(apartmentGetWidth_test);
	RUN_TEST(apartmentGetPrice_test);
	RUN_TEST(apartmentChangePrice_test);
	RUN_TEST(apartmentTotalArea_test);
	RUN_TEST(apartmentGetSquare_test);
	RUN_TEST(apartmentSetSquare_test);
	RUN_TEST(apartmentIsIdentical_test);
	RUN_TEST(apartmentIsSameRoom_test);
	RUN_TEST(apartmentRoomArea_test);
	RUN_TEST(apartmentNumOfRooms_test);
	RUN_TEST(apartmentSplit_test1);
	RUN_TEST(apartmentSplit_test2);
	RUN_TEST(apartmentCopy_test);
	RUN_TEST(apartment_combo_test);
	return  0;
}
