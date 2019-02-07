#ifndef APARTMENT_SERVICE_H
#define APARTMENT_SERVICE_H

#include "apartment.h"

typedef struct node_t {
	int id;
	Apartment apartment;
	struct node_t* next;
} *aptNode;

typedef struct apartment_service_t* ApartmentService;
struct apartment_service_t {
	int currentNumOfApartments;
	int maxNumOfApartments;
	aptNode head;
};

typedef enum {
	APARTMENT_SERVICE_SUCCESS,
	APARTMENT_SERVICE_OUT_OF_MEM,
	APARTMENT_SERVICE_NULL_ARG,
	APARTMENT_SERVICE_FULL,
	APARTMENT_SERVICE_NO_FIT,
	APARTMENT_SERVICE_OUT_OF_BOUNDS,
	APARTMENT_SERVICE_EMPTY,
	APARTMENT_SERVICE_ALREADY_EXISTS
} ApartmentServiceResult;

/**
* Creates a new service:
* The structure is given by linked list of the apartments a their Id's.
* The the structure holds a limited number of apartments, given by
* maxNumOfApartments. Also, the structure holds the current number of apartments
* in the linked list
*
* @return
* 	A new allocated service.
* 	if maxNumOfApartments is not positive, return NULL
* 	or in case of a memory allocation failure - return NULL.
**/
ApartmentService serviceCreate(int maxNumOfApartments);

/**
* creates a newly allocated copy of a given service.
* All apartments in the service will be copied as well.
*
* @return
* A newly allocated copy of the original service
* NULL if the service is NULL or in any case of memory allocation failure
*/
ApartmentService serviceCopy(ApartmentService service);

/**
* Frees all memory allocated for the given service.
* This function can receive NULL.
*/
void serviceDestroy(ApartmentService service);

/**
 * Gets the number of apartment that are currently held in service
 * @return
 * Number of current apartments
 */
int serviceNumberOfApatments(ApartmentService service);

/**
 * Adds a newly allocated copy of a given apartment with a unique given ID in
 * the given service.
 * @return
 * APARTMENT_SERVICE_NULL_ARG in case that apartment or the service points to
 * NULL.
 * APARTMENT_SERVICE_OUT_OF_MEM in any case of memory allocation failure.
 * APARTMENT_SERVICE_OUT_OF_BOUNDS if id<0.
 * APARTMENT_SERVICE_FULL if the current number of apartments exceeds the max
 * number of apartments.
 * APARTMENT_SERVICE_ALREADY_EXISTS if the id is already exist.
 * APARTMENT_SERVICE_SUCCESS otherwise.
 */
ApartmentServiceResult serviceAddApartment(ApartmentService service, 									   
										   Apartment apartment, int id);
/**
 * Calculates the median price of all the apartments currently held in the
 * service, and puts it in outResult.
 * @return
 * APARTMENT_SERVICE_NULL_ARG in case that apartment or the service points to
 * NULL.
 * APARTMENT_SERVICE_EMPTY if there are no apartments held by the given service.
 * APARTMENT_SERVICE_SUCCESS otherwise.
 */
ApartmentServiceResult servicePriceMedian(ApartmentService service,
										  int* outResult);

/**
 * Calculates the median area of all the apartments currently held in the
 * service, and puts it in outResult.
 * @return
 * APARTMENT_SERVICE_NULL_ARG in case that apartment or the service points to
 * NULL.
 * APARTMENT_SERVICE_EMPTY if there are no apartments held by the given service.
 * APARTMENT_SERVICE_SUCCESS otherwise.
 */
ApartmentServiceResult serviceAreaMedian(ApartmentService service, 
										 int* outResult);

/**
 * Gets a service and an apartment, and deletes the latest apartment that is
 * being held by the service, which is completely identical to the
 * given apartment.
 * @return
 * APARTMENT_SERVICE_NULL_ARG in case that apartment or the service points to
 * NULL.
 * APARTMENT_SERVICE_EMPTY if there are no apartments held by the given service.
 * APARTMENT_SERVICE_NO_FIT if none of the apartments in service matches the
 * given apartment.
 * APARTMENT_SERVICE_SUCCESS otherwise.
 */
ApartmentServiceResult serviceDeleteApartment(ApartmentService service,
											  Apartment apartment);
/**
 * Gets a service and an id, and deletes the apartment that holds the same id.
 * @return
 * APARTMENT_SERVICE_NULL_ARG in case that the service points to NULL.
 * APARTMENT_SERVICE_OUT_OF_BOUNDS if id<0.
 * APARTMENT_SERVICE_EMPTY if there are no apartments held by the given service.
 * APARTMENT_SERVICE_NO_FIT if none of the apartments in service matches the
 * given apartment.
 * APARTMENT_SERVICE_SUCCESS otherwise.
 */
ApartmentServiceResult serviceDeleteById(ApartmentService service, int id);

/**
 * Gets a service and an id, and creates a newly allocated copy of the apartment
 * that holds the same id in outApartment.
 * @return
 * APARTMENT_SERVICE_NULL_ARG in case that the service points to NULL.
 * APARTMENT_SERVICE_OUT_OF_BOUNDS if id<0.
 * APARTMENT_SERVICE_EMPTY if there are no apartments held by the given service.
 * APARTMENT_SERVICE_NO_FIT if none of the apartments in service matches the
 * given id.
 * APARTMENT_SERVICE_SUCCESS otherwise.
 */
ApartmentServiceResult serviceGetById(ApartmentService service, int id,
									  Apartment* outApartment);
									  
/**
 * Gets a service and 3 criterions (area, rooms, price), and creates a newly
 * allocated copy of the latest apartment that has at least 'area' area, at
 * least 'rooms' number of rooms, and costs 'price' at most. The copy will be
 * assigned to outApartment.
 * @return
 * APARTMENT_SERVICE_NULL_ARG in case that  the service points to NULL.
 * APARTMENT_SERVICE_OUT_OF_BOUNDS if rooms or price or area is NEGATIVE.
 * APARTMENT_SERVICE_EMPTY if there are no apartments held by the given service.
 * APARTMENT_SERVICE_NO_FIT if none of the apartments in service matches the
 * given criterions.
 * APARTMENT_SERVICE_SUCCESS otherwise.
 */
ApartmentServiceResult serviceSearch(ApartmentService service, int area,
									 int rooms, int price, 
									 Apartment* outApartment);

#endif /* APARTMENT_SERVICE_H */
