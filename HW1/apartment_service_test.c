#include "aux_macros.h"
#include "apartment_service.h"
#include <stdlib.h>

/**
 * Prints the ApartmentService linked list (debugging purpose)
 */
void printList(ApartmentService list) {
	printf("\n\n****** Service Printing ******\n");
	aptNode currentNode=list->head;
	while (currentNode!=NULL) {
		printf("%d -> ",currentNode->id);
		currentNode = currentNode->next;
	}

	printf("NULL\n*****************************\n");
}
/**
 * Checks if two ApartmentService are basically an exact copy of each other
 */
bool isIdenticalService(ApartmentService service1,ApartmentService service2) {
	if (!service1 && !service2) {
		return true;
	}
	aptNode curr1 = service1->head;
	aptNode curr2 = service2->head;
	while (curr1 != NULL || curr2 != NULL) {
		if ( (curr1->id != curr2->id) ||
		     (!apartmentIsIdentical(curr1->apartment,curr2->apartment)) ) {
			return false;
		}
		curr1 = curr1->next;
		curr2 = curr2->next;
	}
	return true;
}

bool serviceCreate_test() {
	bool final = true;
	TEST_EQUALS(final, NULL, serviceCreate(-5));
	TEST_EQUALS(final, NULL, serviceCreate(0));
	return final;
}
bool serviceAddApartment_test() {
	bool final = true;
	ApartmentService service = serviceCreate(3);
	ApartmentService nullService = NULL;
	SquareType square1[1] = { WALL };
	SquareType square2[16] = {  WALL, WALL, WALL, WALL,
			WALL, EMPTY, EMPTY, WALL,
			WALL, EMPTY, EMPTY, WALL,
			WALL, WALL, WALL, WALL };
	SquareType square3[9] = { WALL, WALL, WALL,
			WALL, WALL, WALL,
			WALL, WALL, WALL };
	int id1 = 1000, id2 = 1006, id3 = 1049;
	SquareType* square1Ptr = square1;
	SquareType* square2Ptr = square2;
	SquareType* square3Ptr = square3;
	Apartment apartment1 = apartmentCreate(&square1Ptr, 1, 1, 100);
	Apartment apartment2 = apartmentCreate(&square2Ptr, 4, 4, 300);
	Apartment apartment3 = apartmentCreate(&square3Ptr, 3, 3, 451);
	Apartment nullApartment = NULL;
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS,serviceAddApartment(service, apartment1, id1));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS,serviceAddApartment(service, apartment2, id2));
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS,serviceAddApartment(service, apartment3, id3));
	TEST_EQUALS(final, APARTMENT_SERVICE_FULL,serviceAddApartment(service, apartment1, id1));
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG,serviceAddApartment(service, nullApartment, id1));
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG,serviceAddApartment(nullService, apartment1, id1));
	apartmentDestroy(apartment1);
	apartmentDestroy(apartment2);
	apartmentDestroy(apartment3);
	serviceDestroy(service);
	serviceDestroy(nullService);
	return final;
}
bool serviceNumberOfApatments_test() {
	bool final = true;
	ApartmentService service = serviceCreate(3);
	SquareType square1[1] = { WALL };
	SquareType square2[16] = {  WALL, WALL, WALL, WALL,
			WALL, EMPTY, EMPTY, WALL,
			WALL, EMPTY, EMPTY, WALL,
			WALL, WALL, WALL, WALL };
	SquareType square3[9] = { WALL, WALL, WALL,
			WALL, WALL, WALL,
			WALL, WALL, WALL };
	int id1 = 1, id2 = 6, id3 = 9;
	SquareType* square1Ptr = square1;
	SquareType* square2Ptr = square2;
	SquareType* square3Ptr = square3;
	Apartment apartment1 = apartmentCreate(&square1Ptr, 1, 1, 100);
	Apartment apartment2 = apartmentCreate(&square2Ptr, 4, 4, 300);
	Apartment apartment3 = apartmentCreate(&square3Ptr, 3, 3, 451);
	serviceAddApartment(service, apartment1, id1);
	serviceAddApartment(service, apartment2, id2);
	serviceAddApartment(service, apartment3, id3);
	TEST_EQUALS(final, 3, serviceNumberOfApatments(service));
	apartmentDestroy(apartment1);
	apartmentDestroy(apartment2);
	apartmentDestroy(apartment3);
	serviceDestroy(service);
	return final;
}
bool servicePriceMedian_test() {
	bool final = true;
	ApartmentService service = serviceCreate(4);
	ApartmentService singleAptService = serviceCreate(1);
	ApartmentService nullAptService = NULL;
	SquareType square1[1] = { WALL };
	SquareType square2[16] = { WALL, WALL, WALL, WALL, WALL, EMPTY, EMPTY, WALL,
			WALL, EMPTY, EMPTY, WALL, WALL, WALL, WALL, WALL };
	SquareType square3[9] = { WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL,
			WALL };
	SquareType square4[2] = { WALL, EMPTY };
	int id1 = 1000, id2 = 1006, id3 = 1049, id4 = 948;
	SquareType* square1Ptr = square1;
	SquareType* square2Ptr = square2;
	SquareType* square3Ptr = square3;
	SquareType* square4Ptr = square4;
	Apartment apartment1 = apartmentCreate(&square1Ptr, 1, 1, 100);
	Apartment apartment2 = apartmentCreate(&square2Ptr, 4, 4, 600);
	Apartment apartment3 = apartmentCreate(&square3Ptr, 3, 3, 451);
	Apartment apartment4 = apartmentCreate(&square4Ptr, 2, 1, 343451);
	serviceAddApartment(service, apartment3, id3);
	serviceAddApartment(service, apartment1, id1);
	serviceAddApartment(service, apartment2, id2);
	serviceAddApartment(service, apartment4, id4);
	serviceAddApartment(singleAptService, apartment1, id1);
	int outRes;
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS,
			servicePriceMedian(service, &outRes));
	TEST_EQUALS(final, 525, outRes);
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS,
			servicePriceMedian(singleAptService, &outRes));
	TEST_EQUALS(final, 100, outRes);
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG,
			servicePriceMedian(nullAptService, &outRes));
	apartmentDestroy(apartment1);
	apartmentDestroy(apartment2);
	apartmentDestroy(apartment3);
	apartmentDestroy(apartment4);
	serviceDestroy(service);
	serviceDestroy(singleAptService);
	serviceDestroy(nullAptService);
	return final;
}
bool serviceAreaMedian_test() {
	bool final = true;
	ApartmentService service1 = serviceCreate(4);
	ApartmentService service2 = serviceCreate(3);
	ApartmentService singleAptService = serviceCreate(1);
	ApartmentService nullAptService = NULL;
	SquareType square1[3] = { WALL, EMPTY, WALL }; // Area = 1
	SquareType square2[16] = { WALL, WALL, WALL, WALL, // Area = 4
			WALL, EMPTY, EMPTY, WALL,
			WALL, EMPTY, EMPTY, WALL,
			WALL, WALL, WALL, WALL };
	SquareType square3[9] = { WALL, EMPTY, WALL, // Area = 5
			WALL, EMPTY, EMPTY,
			EMPTY, WALL, WALL };
	SquareType square4[2] = { WALL, EMPTY }; // Area = 1
	int id1 = 1500, id2 = 1206, id3 = 1069, id4 = 928;
	SquareType* square1Ptr = square1;
	SquareType* square2Ptr = square2;
	SquareType* square3Ptr = square3;
	SquareType* square4Ptr = square4;
	Apartment apartment1 = apartmentCreate(&square1Ptr, 1, 3, 100);
	Apartment apartment2 = apartmentCreate(&square2Ptr, 4, 4, 600);
	Apartment apartment3 = apartmentCreate(&square3Ptr, 3, 3, 451);
	Apartment apartment4 = apartmentCreate(&square4Ptr, 2, 1, 5856);
	serviceAddApartment(service1, apartment3, id3);
	serviceAddApartment(service1, apartment1, id1);
	serviceAddApartment(service1, apartment2, id2);
	serviceAddApartment(service1, apartment4, id4);
	serviceAddApartment(service2, apartment3, id3);
	serviceAddApartment(service2, apartment2, id2);
	serviceAddApartment(service2, apartment1, id1);
	serviceAddApartment(singleAptService, apartment2, id2);
	int outRes = 0;
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS,
			serviceAreaMedian(service1, &outRes));
	TEST_EQUALS(final, 2, outRes);
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS,
			serviceAreaMedian(service2, &outRes));
	TEST_EQUALS(final, 4, outRes);
	TEST_EQUALS(final, APARTMENT_SERVICE_SUCCESS,
			serviceAreaMedian(singleAptService, &outRes));
	TEST_EQUALS(final, 4, outRes);
	TEST_EQUALS(final, APARTMENT_SERVICE_NULL_ARG,
			serviceAreaMedian(nullAptService, &outRes));
	apartmentDestroy(apartment1);
	apartmentDestroy(apartment2);
	apartmentDestroy(apartment3);
	apartmentDestroy(apartment4);
	serviceDestroy(service1);
	serviceDestroy(service2);
	serviceDestroy(singleAptService);
	serviceDestroy(nullAptService);
	return final;
}
bool serviceSearch_test() {
	bool final = true;
	ApartmentService service1 = serviceCreate(6);
	ApartmentService emptyService = serviceCreate(3);
	ApartmentService nullService = NULL;
	SquareType square1[1] = { WALL };
	SquareType square2[16] = { WALL, WALL, WALL, WALL, WALL, EMPTY, EMPTY, WALL,
			WALL, EMPTY, EMPTY, WALL, WALL, WALL, WALL, WALL };
	SquareType square3[9] = { WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL,
			WALL };
	SquareType square4[2] = { WALL, EMPTY };
	SquareType square5[6] = { WALL, WALL, EMPTY, WALL, WALL, WALL };
	SquareType square6[4] = { WALL, EMPTY, WALL, EMPTY };
	int id1 = 1000, id2 = 1006, id3 = 1049, id4 = 948, id5 = 941, id6 = 591;
	SquareType* square1Ptr = square1;
	SquareType* square2Ptr = square2;
	SquareType* square3Ptr = square3;
	SquareType* square4Ptr = square4;
	SquareType* square5Ptr = square5;
	SquareType* square6Ptr = square6;
	Apartment apartment1 = apartmentCreate(&square1Ptr, 1, 1, 100);
	Apartment apartment2 = apartmentCreate(&square2Ptr, 4, 4, 600);
	Apartment apartment3 = apartmentCreate(&square3Ptr, 3, 3, 451);
	Apartment apartment4 = apartmentCreate(&square4Ptr, 2, 1, 129309);
	Apartment apartment5 = apartmentCreate(&square5Ptr, 2, 3, 9999);
	Apartment apartment6 = apartmentCreate(&square6Ptr, 2, 2, 2500);
	serviceAddApartment(service1,apartment3, id3);
	serviceAddApartment(service1,apartment1, id1);
	serviceAddApartment(service1,apartment6, id6);
	serviceAddApartment(service1,apartment2, id2);
	serviceAddApartment(service1,apartment5, id5);
	serviceAddApartment(service1,apartment4, id4);
	Apartment outApt = NULL;
	TEST_EQUALS(final,APARTMENT_SERVICE_NULL_ARG,
			serviceSearch(nullService,300,43,2200,&outApt));
	TEST_EQUALS(final,APARTMENT_SERVICE_OUT_OF_BOUNDS,
			serviceSearch(service1,-500,3,1200,&outApt));
	TEST_EQUALS(final,APARTMENT_SERVICE_OUT_OF_BOUNDS,
			serviceSearch(service1,504,-43,1200,&outApt));
	TEST_EQUALS(final,APARTMENT_SERVICE_OUT_OF_BOUNDS,
			serviceSearch(service1,204,3,-1200,&outApt));
	TEST_EQUALS(final,APARTMENT_SERVICE_NO_FIT,
			serviceSearch(service1,3020,31,30,&outApt));
	TEST_EQUALS(final,APARTMENT_SERVICE_NO_FIT,
			serviceSearch(service1,1515151,1515151,1515151,&outApt));
	TEST_EQUALS(final,APARTMENT_SERVICE_NO_FIT,
			serviceSearch(service1,0,0,0,&outApt));
	TEST_EQUALS(final,APARTMENT_SERVICE_EMPTY,
			serviceSearch(emptyService,10,10,10,&outApt));
	TEST_EQUALS(final,APARTMENT_SERVICE_SUCCESS,
			serviceSearch(service1,1,1,129309,&outApt));
	TEST_EQUALS(final,APARTMENT_SERVICE_SUCCESS,
			serviceSearch(service1,4,1,6520,&outApt));
	TEST_EQUALS(final,APARTMENT_SERVICE_SUCCESS,
			serviceSearch(service1,0,0,210,&outApt));
	TEST_EQUALS(final,APARTMENT_SERVICE_SUCCESS,
			serviceSearch(service1,4,1,600,&outApt));
	apartmentDestroy(outApt);
	apartmentDestroy(apartment1);
	apartmentDestroy(apartment2);
	apartmentDestroy(apartment3);
	apartmentDestroy(apartment4);
	apartmentDestroy(apartment5);
	apartmentDestroy(apartment6);
	serviceDestroy(emptyService);
	serviceDestroy(service1);
	return final;
}
bool serviceGetById_test() {
	bool final = true;
	ApartmentService service1 = serviceCreate(6);
	ApartmentService emptyService = serviceCreate(3);
	ApartmentService nullService = NULL;
	SquareType square1[1] = { WALL };
	SquareType square2[16] = { WALL, WALL, WALL, WALL, WALL, EMPTY, EMPTY, WALL,
			WALL, EMPTY, EMPTY, WALL, WALL, WALL, WALL, WALL };
	SquareType square3[9] = { WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL,
			WALL };
	SquareType square4[2] = { WALL, EMPTY };
	SquareType square5[6] = { WALL, WALL, EMPTY, WALL, WALL, WALL };
	SquareType square6[4] = { WALL, EMPTY, WALL, EMPTY };
	int id1 = 435, id2 = 1006, id3 = 1049, id4 = 948, id5 = 941, id6 = 591;
	SquareType* square1Ptr = square1;
	SquareType* square2Ptr = square2;
	SquareType* square3Ptr = square3;
	SquareType* square4Ptr = square4;
	SquareType* square5Ptr = square5;
	SquareType* square6Ptr = square6;
	Apartment apartment1 = apartmentCreate(&square1Ptr, 1, 1, 100);
	Apartment apartment2 = apartmentCreate(&square2Ptr, 4, 4, 600);
	Apartment apartment3 = apartmentCreate(&square3Ptr, 3, 3, 451);
	Apartment apartment4 = apartmentCreate(&square4Ptr, 2, 1, 129309);
	Apartment apartment5 = apartmentCreate(&square5Ptr, 2, 3, 9999);
	Apartment apartment6 = apartmentCreate(&square6Ptr, 2, 2, 2500);
	serviceAddApartment(service1,apartment3, id3);
	serviceAddApartment(service1,apartment1, id1);
	serviceAddApartment(service1,apartment6, id6);
	serviceAddApartment(service1,apartment2, id2);
	serviceAddApartment(service1,apartment5, id5);
	serviceAddApartment(service1,apartment4, id4);
	Apartment outApt = NULL;
	TEST_EQUALS(final,APARTMENT_SERVICE_NULL_ARG,
			serviceSearch(nullService,300,43,2200,&outApt));
	TEST_EQUALS(final,APARTMENT_SERVICE_OUT_OF_BOUNDS,
			serviceGetById(service1,-5,&outApt));
	TEST_EQUALS(final,APARTMENT_SERVICE_EMPTY,
			serviceGetById(emptyService,530,&outApt));
	TEST_EQUALS(final,APARTMENT_SERVICE_NO_FIT,
			serviceGetById(service1,123456,&outApt));
	TEST_EQUALS(final,APARTMENT_SERVICE_SUCCESS,
			serviceGetById(service1,1006,&outApt));
	TEST_EQUALS(final,APARTMENT_SERVICE_SUCCESS,
			serviceGetById(service1,948,&outApt));
	TEST_EQUALS(final,APARTMENT_SERVICE_SUCCESS,
			serviceGetById(service1,435,&outApt));
	TEST_EQUALS(final,APARTMENT_SERVICE_SUCCESS,
			serviceGetById(service1,591,&outApt));
	TEST_EQUALS(final,APARTMENT_SERVICE_SUCCESS,
			serviceGetById(service1,1049,&outApt));
	apartmentDestroy(outApt);
	apartmentDestroy(apartment1);
	apartmentDestroy(apartment2);
	apartmentDestroy(apartment3);
	apartmentDestroy(apartment4);
	apartmentDestroy(apartment5);
	apartmentDestroy(apartment6);
	serviceDestroy(emptyService);
	serviceDestroy(service1);
	return final;
}

bool serviceDeleteApartment_test() {
	bool final = true;
	ApartmentService service1 = serviceCreate(6);
	ApartmentService emptyService = serviceCreate(1);
	ApartmentService nullService = NULL;
	SquareType square1[1] = { WALL };
	SquareType square2[16] = { WALL, WALL, WALL, WALL, WALL, EMPTY, EMPTY, WALL,
			WALL, EMPTY, EMPTY, WALL, WALL, WALL, WALL, WALL };
	SquareType square3[9] = { WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL,
			WALL };
	SquareType square4[2] = { WALL, EMPTY };
	SquareType square5[6] = { WALL, WALL, EMPTY, WALL, WALL, WALL };
	SquareType square6[4] = { WALL, EMPTY, WALL, EMPTY };
	int id1 = 435, id2 = 1006, id3 = 1049, id4 = 948, id5 = 941, id6 = 591;
	SquareType* square1Ptr = square1;
	SquareType* square2Ptr = square2;
	SquareType* square3Ptr = square3;
	SquareType* square4Ptr = square4;
	SquareType* square5Ptr = square5;
	SquareType* square6Ptr = square6;
	Apartment apartment1 = apartmentCreate(&square1Ptr, 1, 1, 100);
	Apartment apartment2 = apartmentCreate(&square2Ptr, 4, 4, 600);
	Apartment apartment3 = apartmentCreate(&square3Ptr, 3, 3, 451);
	Apartment apartment4 = apartmentCreate(&square4Ptr, 2, 1, 129309);
	Apartment apartment5 = apartmentCreate(&square5Ptr, 2, 3, 9999);
	Apartment apartment6 = apartmentCreate(&square6Ptr, 2, 2, 2500);
	serviceAddApartment(service1,apartment3, id3);
	serviceAddApartment(service1,apartment1, id1);
	serviceAddApartment(service1,apartment6, id6);
	serviceAddApartment(service1,apartment2, id2);
	serviceAddApartment(service1,apartment5, id5);
	serviceAddApartment(service1,apartment4, id4);
	Apartment delApt = apartmentCopy(apartment4);
	TEST_EQUALS(final,APARTMENT_SERVICE_NULL_ARG,
			serviceDeleteApartment(nullService,delApt));
	TEST_EQUALS(final,APARTMENT_SERVICE_EMPTY,
			serviceDeleteApartment(emptyService,delApt));
	TEST_EQUALS(final,APARTMENT_SERVICE_SUCCESS,
	serviceDeleteApartment(service1,delApt)); // Deletion of 1st
	apartmentDestroy(delApt);
	delApt = apartmentCopy(apartment6);
	TEST_EQUALS(final,APARTMENT_SERVICE_SUCCESS,
			serviceDeleteApartment(service1,delApt)); // Deletion of mid
	apartmentDestroy(delApt);
	delApt = apartmentCopy(apartment3);
	TEST_EQUALS(final,APARTMENT_SERVICE_SUCCESS,
			serviceDeleteApartment(service1,delApt)); // Deletion of last
	apartmentDestroy(delApt);
	delApt = apartmentCopy(apartment2);
	TEST_EQUALS(final,APARTMENT_SERVICE_SUCCESS,
			serviceDeleteApartment(service1,delApt))
	apartmentDestroy(delApt);
	delApt = apartmentCopy(apartment1);
	TEST_EQUALS(final,APARTMENT_SERVICE_SUCCESS,
			serviceDeleteApartment(service1,delApt))
	apartmentDestroy(delApt);
	delApt = apartmentCopy(apartment5);
	TEST_EQUALS(final,APARTMENT_SERVICE_SUCCESS,
			serviceDeleteApartment(service1,delApt))
	apartmentDestroy(delApt);
	apartmentDestroy(apartment1);
	apartmentDestroy(apartment2);
	apartmentDestroy(apartment3);
	apartmentDestroy(apartment4);
	apartmentDestroy(apartment5);
	apartmentDestroy(apartment6);
	serviceDestroy(emptyService);
	serviceDestroy(nullService);
	serviceDestroy(service1);
	return final;
}
bool serviceDeleteById_test() {
	bool final = true;
	ApartmentService service1 = serviceCreate(6);
	ApartmentService emptyService = serviceCreate(1);
	ApartmentService nullService = NULL;
	SquareType square1[1] = { WALL };
	SquareType square2[16] = { WALL, WALL, WALL, WALL, WALL, EMPTY, EMPTY, WALL,
			WALL, EMPTY, EMPTY, WALL, WALL, WALL, WALL, WALL };
	SquareType square3[9] = { WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL,
			WALL };
	SquareType square4[2] = { WALL, EMPTY };
	SquareType square5[6] = { WALL, WALL, EMPTY, WALL, WALL, WALL };
	SquareType square6[4] = { WALL, EMPTY, WALL, EMPTY };
	int id1 = 435, id2 = 1006, id3 = 1049, id4 = 948, id5 = 941, id6 = 591;
	SquareType* square1Ptr = square1;
	SquareType* square2Ptr = square2;
	SquareType* square3Ptr = square3;
	SquareType* square4Ptr = square4;
	SquareType* square5Ptr = square5;
	SquareType* square6Ptr = square6;
	Apartment apartment1 = apartmentCreate(&square1Ptr, 1, 1, 100);
	Apartment apartment2 = apartmentCreate(&square2Ptr, 4, 4, 600);
	Apartment apartment3 = apartmentCreate(&square3Ptr, 3, 3, 451);
	Apartment apartment4 = apartmentCreate(&square4Ptr, 2, 1, 129309);
	Apartment apartment5 = apartmentCreate(&square5Ptr, 2, 3, 9999);
	Apartment apartment6 = apartmentCreate(&square6Ptr, 2, 2, 2500);
	serviceAddApartment(service1,apartment3, id3);
	serviceAddApartment(service1,apartment1, id1);
	serviceAddApartment(service1,apartment6, id6);
	serviceAddApartment(service1,apartment2, id2);
	serviceAddApartment(service1,apartment5, id5);
	serviceAddApartment(service1,apartment4, id4);
	TEST_EQUALS(final,APARTMENT_SERVICE_OUT_OF_BOUNDS,
			serviceDeleteById(service1,-4425));
	TEST_EQUALS(final,APARTMENT_SERVICE_NULL_ARG,
			serviceDeleteById(nullService,5141));
	TEST_EQUALS(final,APARTMENT_SERVICE_EMPTY,
			serviceDeleteById(emptyService,1234));
	TEST_EQUALS(final,APARTMENT_SERVICE_SUCCESS,
			serviceDeleteById(service1,id4)); // Delete the first
	TEST_EQUALS(final,APARTMENT_SERVICE_SUCCESS,
			serviceDeleteById(service1,id2)); // Delete from mid
	TEST_EQUALS(final,APARTMENT_SERVICE_SUCCESS,
			serviceDeleteById(service1,id3)); // Delete the last
	TEST_EQUALS(final,APARTMENT_SERVICE_SUCCESS,
			serviceDeleteById(service1,id1));
	TEST_EQUALS(final,APARTMENT_SERVICE_SUCCESS,
			serviceDeleteById(service1,id6));
	TEST_EQUALS(final,APARTMENT_SERVICE_SUCCESS,
			serviceDeleteById(service1,id5));
	apartmentDestroy(apartment1);
	apartmentDestroy(apartment2);
	apartmentDestroy(apartment3);
	apartmentDestroy(apartment4);
	apartmentDestroy(apartment5);
	apartmentDestroy(apartment6);
	serviceDestroy(emptyService);
	serviceDestroy(service1);
	return final;
}
bool serviceCopy_test() {
	bool final = true;
	ApartmentService service1 = serviceCreate(6);
	ApartmentService singleAptService = serviceCreate(1);
	ApartmentService emptyService = serviceCreate(3);
	ApartmentService nullService = serviceCreate(-1);
	SquareType square1[1] = { WALL };
	SquareType square2[16] = { WALL, WALL, WALL, WALL, WALL, EMPTY, EMPTY, WALL,
			WALL, EMPTY, EMPTY, WALL, WALL, WALL, WALL, WALL };
	SquareType square3[9] = { WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL,
			WALL };
	SquareType square4[2] = { WALL, EMPTY };
	SquareType square5[6] = { WALL, WALL, EMPTY, WALL, WALL, WALL };
	SquareType square6[4] = { WALL, EMPTY, WALL, EMPTY };
	int id1 = 435, id2 = 1006, id3 = 1049, id4 = 948, id5 = 941, id6 = 591;
	SquareType* square1Ptr = square1;
	SquareType* square2Ptr = square2;
	SquareType* square3Ptr = square3;
	SquareType* square4Ptr = square4;
	SquareType* square5Ptr = square5;
	SquareType* square6Ptr = square6;
	Apartment apartment1 = apartmentCreate(&square1Ptr, 1, 1, 100);
	Apartment apartment2 = apartmentCreate(&square2Ptr, 4, 4, 600);
	Apartment apartment3 = apartmentCreate(&square3Ptr, 3, 3, 451);
	Apartment apartment4 = apartmentCreate(&square4Ptr, 2, 1, 129309);
	Apartment apartment5 = apartmentCreate(&square5Ptr, 2, 3, 9999);
	Apartment apartment6 = apartmentCreate(&square6Ptr, 2, 2, 2500);
	serviceAddApartment(service1,apartment3, id3);
	serviceAddApartment(service1,apartment1, id1);
	serviceAddApartment(service1,apartment6, id6);
	serviceAddApartment(service1,apartment2, id2);
	serviceAddApartment(service1,apartment5, id5);
	serviceAddApartment(service1,apartment4, id4);
	serviceAddApartment(singleAptService,apartment2, id2);
	ApartmentService copy = serviceCopy(service1);
	ApartmentService emptyCopy = serviceCopy(emptyService);
	ApartmentService nullCopy = serviceCopy(nullService);
	ApartmentService singleAptCopy = serviceCopy(singleAptService);
	TEST_EQUALS(final,true,isIdenticalService(copy,service1));
	TEST_EQUALS(final,true,isIdenticalService(singleAptCopy,singleAptService));
	TEST_EQUALS(final,true,isIdenticalService(emptyCopy,emptyService));
	TEST_EQUALS(final,true,isIdenticalService(nullCopy,nullService));
	apartmentDestroy(apartment1);
	apartmentDestroy(apartment2);
	apartmentDestroy(apartment3);
	apartmentDestroy(apartment4);
	apartmentDestroy(apartment5);
	apartmentDestroy(apartment6);
	serviceDestroy(emptyService);
	serviceDestroy(nullService);
	serviceDestroy(singleAptCopy);
	serviceDestroy(service1);
	serviceDestroy(copy);
	return final;
}
int main() {
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	RUN_TEST(serviceCreate_test);
	RUN_TEST(serviceAddApartment_test);
	RUN_TEST(serviceNumberOfApatments_test);
	RUN_TEST(servicePriceMedian_test);
	RUN_TEST(serviceAreaMedian_test);
	RUN_TEST(serviceSearch_test);
	RUN_TEST(serviceGetById_test);
	RUN_TEST(serviceDeleteApartment_test);
	RUN_TEST(serviceDeleteById_test);
	RUN_TEST(serviceCopy_test);
	return 0;
}
