#ifndef REALTOR_H
#define REALTOR_H

#include "map.h"
#include "apartment_service.h"
#include "auxFunctions.h"

typedef struct realtor_t* Realtor;

typedef enum {
	REALTOR_SUCCESS, REALTOR_NULL_ARG, REALTOR_OUT_OF_MEM,
	REALTOR_BAD_ARG
} RealtorResult;

/**
* Creates a new realtor type:
* The structure is given by email, company name and tax percentage.
* Tax percentage is valid value is between 0-100 inclusive.
* The structure also contains ApartmentService structure managed by the realtor.
* @return
* A new allocated realtor type.
* In case of invalid input or any kind of memory allocation failure - return NULL, and a specific error
* result via pointer.
*/
Realtor realtorCreate(char* email, char* company_name, int tax_percentage, RealtorResult* error);

/**
* Frees all memory allocated for the given realtor.
* This function can receive NULL.
*/
void realtorDestroy(Realtor realtor);

/**
* Creates a newly allocated copy of a given realtor.
*
* @return
* A newly allocated copy of the original realtor.
* NULL if the realtor is NULL or in any case of memory allocation failure, and a specific error
* result via pointer.
*/
Realtor realtorCopy(Realtor realtor, RealtorResult* error);

/**
* @return
* The email address of the realtor.
* This function asserts (realtor).
*/
char* realtorGetMail(Realtor realtor);

/**
* Sets the customer email address.
*
* @return
* 	REALTOR_NULL_ARG if realtor is NULL.
* 	REALTOR_BAD_ARG if the given email address format is invalid.
*   REALTOR_OUT_OF_MEM in any case of memory allocation failure.
* 	REALTOR_SUCCESS otherwise.
*/
RealtorResult realtorSetMail(Realtor realtor, char* new_mail);

/**
* @return
* The realtor's company name.
* This function asserts (realtor).
*/
char* realtorGetCompanyName(Realtor realtor);

/**
* Sets the realtor's company name.
*
* @return
* 	REALTOR_NULL_ARG if realtor is NULL.
* 	REALTOR_BAD_ARG if the given company name is invalid (empty).
*   REALTOR_OUT_OF_MEM in any case of memory allocation failure.
* 	REALTOR_SUCCESS otherwise.
*/
RealtorResult realtorSetCompanyName(Realtor realtor, char* new_company_name);


/**
* @return
* The tax percentage rate of the realtor.
* This function asserts (realtor).
*/
int realtorGetTaxPercentage(Realtor realtor);

/**
* Sets the realtor's tax percentage
*
* @return
* 	REALTOR_NULL_ARG if realtor is NULL.
* 	REALTOR_BAD_ARG if the given tax percentage is not valid (not between 1-100)
* 	REALTOR_SUCCESS otherwise.
*/
RealtorResult realtorSetTaxPercentage(Realtor realtor, int new_tax);

/**
* @return
* The realtor's services structure.
* This function asserts (realtor).
*/
Map realtorGetServices(Realtor realtor);
#endif /* REALTOR_H */
