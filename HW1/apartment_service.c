#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include "apartment_service.h"

//***Aid functions***//

/**
 * changes the direction of a linked list if a given service.
 */
static void reverseList(ApartmentService service) {
    aptNode prev   = NULL;
    aptNode curr = service->head;
    aptNode next;
    while (curr != NULL)
    {
        next  = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    service->head = prev;
}

/**
 * A simple swap function between two integers
 */
static void swap(int *a, int *b) {
	int temp = *b;
	*b = *a;
	*a = temp;
}

/**
 * Bubble-sort algorithm for an int array
 */
static void sortArray(int *priceArray, int len) {
	for (int i=0;i<len-1;i++) {
		for (int j=0;j<len-i-1;j++) {
			if (priceArray[j] > priceArray[j+1]) {
				swap(&priceArray[j],&priceArray[j+1]);
			}
		}
	}
}

/**
 * Calculates the median price or area of a given service. Result will be
 * held in outResult.
 */
static ApartmentServiceResult genericMedianCalc(ApartmentService service,
										 int* outResult,bool byArea) {
	if (!service || !outResult) {
			return APARTMENT_SERVICE_NULL_ARG;
	}
	int numOfApts = serviceNumberOfApatments(service);
	if (numOfApts == 0) {
		return APARTMENT_SERVICE_EMPTY;
	}
	int *targetArray = malloc(sizeof(int)*numOfApts);
		if (!targetArray) {
			return APARTMENT_SERVICE_OUT_OF_MEM;
	}
	int c = 0;
	aptNode curr = service->head;
	while (curr != NULL) {
		targetArray[c++] = byArea?(apartmentTotalArea(curr->apartment)):
						   (apartmentGetPrice(curr->apartment));
		curr = curr->next;
	}
	sortArray(targetArray,numOfApts);
	if (numOfApts%2 != 0) {
		*outResult = targetArray[numOfApts/2];
	}
	else {
		*outResult = (targetArray[numOfApts/2]+targetArray[(numOfApts/2)-1])/2;
	}
	free(targetArray);
	return APARTMENT_SERVICE_SUCCESS;
}

/**
* Frees all memory allocated for the next of a given node.
* This function can receive NULL.
*/
static void deleteCurrentApartment(ApartmentService service, aptNode prev)
{
    aptNode curr;
    if(prev == NULL) {
        curr = service->head;
        service->head = service->head->next;
    }
    else {
        aptNode prevNode = prev;
        curr = prev->next;
        prevNode->next = curr->next;
    }
    (service->currentNumOfApartments)--;
    apartmentDestroy(curr->apartment);
    free(curr);
}

/**
 * Checks if a given id exists in the given service.
 */
static bool idExist(ApartmentService service,int id) {
	aptNode curr = service->head;
	while (curr != NULL) {
		if (curr->id == id) {
			return true;	
	    }
	curr = curr->next;
	}
	return false;
}

/**
 * Gets a service and an id, and deletes or copies the apartment that holds the 
 * same id.
 */
static ApartmentServiceResult handleID(ApartmentService service, int id,
							    Apartment* outApartment, bool del) {
	if (!service) {
		return APARTMENT_SERVICE_NULL_ARG;
	}
	if (id < 0) {
		return APARTMENT_SERVICE_OUT_OF_BOUNDS;
	}
	if (serviceNumberOfApatments(service) == 0) {
		return APARTMENT_SERVICE_EMPTY;
	}
	aptNode curr = service->head;
	aptNode prev = NULL;
		while (curr != NULL) {
			if (curr->id == id) {
				if (del) {
					deleteCurrentApartment(service,prev);
					return APARTMENT_SERVICE_SUCCESS;
				}
				else {
					*outApartment = apartmentCopy(curr->apartment);
					return APARTMENT_SERVICE_SUCCESS;
				}
			}
			prev = curr;
			curr = curr->next;
		}
	return APARTMENT_SERVICE_NO_FIT;
}
//***End of aid functions end***//

ApartmentService serviceCreate(int maxNumOfApartments) {
	if (maxNumOfApartments<=0) {
		return NULL;
	}
	ApartmentService newList = malloc(sizeof(*newList));
	if (!newList) {
		return NULL;
	}
	newList->maxNumOfApartments = maxNumOfApartments;
	newList->currentNumOfApartments = 0;
	newList->head = NULL;

	return newList;
}

ApartmentServiceResult serviceAddApartment(ApartmentService service,
										   Apartment apartment, int id) {
	if (!service || !apartment) {
		return APARTMENT_SERVICE_NULL_ARG;
	}
	if (id < 0)	{
		return APARTMENT_SERVICE_OUT_OF_BOUNDS;
	}
	if (service->currentNumOfApartments >= service->maxNumOfApartments) {
		return APARTMENT_SERVICE_FULL;
	}
	if (idExist(service,id)) {
		return APARTMENT_SERVICE_ALREADY_EXISTS;
	}
	aptNode newNode = malloc(sizeof(*newNode));
	if (!newNode) {
		return APARTMENT_SERVICE_OUT_OF_MEM;
	}
	newNode->id = id;
	newNode->apartment = apartmentCopy(apartment);
	if (!(newNode->apartment))	{
		free(newNode);
		return APARTMENT_SERVICE_OUT_OF_MEM;
	}
	if (service->head == NULL) {
		service->head = newNode;
		service->head->next = NULL;
	} 
	else {
		newNode->next = service->head;
		service->head = newNode;
	}
	(service->currentNumOfApartments)++;

	return APARTMENT_SERVICE_SUCCESS;
}

int serviceNumberOfApatments(ApartmentService service) {
	assert(service != NULL);
	return service->currentNumOfApartments;
}

ApartmentServiceResult servicePriceMedian(ApartmentService service,
															int* outResult) {
	return genericMedianCalc(service,outResult,false);
}

ApartmentServiceResult serviceAreaMedian(ApartmentService service,
										 int* outResult) {
	return genericMedianCalc(service,outResult,true);
}

ApartmentServiceResult serviceSearch(ApartmentService service, int area,
									 int rooms, int price,
									 Apartment* outApartment) {
	if (!service) {
		return APARTMENT_SERVICE_NULL_ARG;
	}
	if (area < 0 || rooms < 0 || price < 0) {
		return APARTMENT_SERVICE_OUT_OF_BOUNDS;
	}
	if (serviceNumberOfApatments(service) == 0) {
		return APARTMENT_SERVICE_EMPTY;
	}
	aptNode curr = service->head;
	while (curr != NULL) {
		if ( (apartmentTotalArea(curr->apartment) >= area) &&
			 (apartmentNumOfRooms(curr->apartment) >= rooms) &&
			 (apartmentGetPrice(curr->apartment) <= price) )
		{
			*outApartment = apartmentCopy(curr->apartment);
			return APARTMENT_SERVICE_SUCCESS;
		}
		curr = curr->next;
	}
	return APARTMENT_SERVICE_NO_FIT;
}

ApartmentServiceResult serviceDeleteById(ApartmentService service, int id) {
	Apartment outApartment = NULL;
	return handleID(service,id,&outApartment,true);
}

ApartmentServiceResult serviceGetById(ApartmentService service, int id,
									  Apartment* outApartment) {
	if (!outApartment) {
		return APARTMENT_SERVICE_NULL_ARG;
	}
	return handleID(service,id,outApartment,false);
}

ApartmentServiceResult serviceDeleteApartment(ApartmentService service,
										      Apartment apartment) {
	if (!service || !apartment) {
		return APARTMENT_SERVICE_NULL_ARG;
	}
	if (serviceNumberOfApatments(service) == 0) {
		return APARTMENT_SERVICE_EMPTY;
	}
	aptNode curr = service->head;
	aptNode prev = NULL;
	while (curr != NULL) {
		if (apartmentIsIdentical(curr->apartment,apartment)) {
			deleteCurrentApartment(service,prev);
			return APARTMENT_SERVICE_SUCCESS;
		}
		prev = curr;
	    curr = curr->next;
	}
	return APARTMENT_SERVICE_NO_FIT;
}

void serviceDestroy(ApartmentService service) {
	if (!service) {
		return;
	}
	aptNode curr = service->head;
	while(curr != NULL) {
		aptNode toDelete = curr;
		curr = curr->next;
		apartmentDestroy(toDelete->apartment);
		free(toDelete);
	}
	free(service);
}

ApartmentService serviceCopy(ApartmentService service) {
	if (!service) {
		return NULL;
	}
	ApartmentService copy = malloc(sizeof(*copy));
	if (!copy) {
		return NULL;
	}
	copy->currentNumOfApartments = service->currentNumOfApartments;
	copy->maxNumOfApartments = service->maxNumOfApartments;
	if (service->currentNumOfApartments == 0) {
		copy->head = NULL;
		return copy;
	}
	aptNode headCopy = malloc(sizeof(*headCopy));
	if (!headCopy) {
		serviceDestroy(copy);
		return NULL;
	}
	headCopy->id = service->head->id;
	headCopy->apartment = apartmentCopy(service->head->apartment);
	headCopy->next = NULL;
	copy->head = headCopy;
	aptNode curr = service->head->next;
	while (curr != NULL) {
		aptNode newNode = malloc(sizeof(*newNode));
		if (!newNode) {
			return NULL;
		}
		newNode->id = curr->id;
		newNode->apartment = apartmentCopy(curr->apartment);
		if (copy->head == NULL) {
			copy->head = newNode;
			copy->head->next = NULL;
		}
		else {
			newNode->next = copy->head;
			copy->head = newNode;
		}
		curr = curr->next;
	}
	reverseList(copy);
	return copy;
}
