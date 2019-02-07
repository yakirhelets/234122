#ifndef OFFER_H
#define OFFER_H

typedef struct offer_t* Offer;

typedef enum {
	OFFER_SUCCESS, OFFER_NULL_ARG, OFFER_OUT_OF_MEM,
	OFFER_BAD_ARG, OFFER_NOT_FOUND
} OfferResult;

/**
* Creates a new Offer type:
* The structure is given by costumer email, realtor email, apartment service that belongs to the realtor,
* id of an apartment that belongs to the apartment service, and the wnted price.
* @return
* A new allocated Offer type.
* In case of invalid input or any kind of memory allocation failure - return NULL and a specific error result (via a pointer).
*/
Offer offerCreate(char* costumer_email, char* realtor_email, char* apartment_service, int apartment_id, int new_price, OfferResult* error);

/**
* Frees all memory allocated for the given Offer.
* This function can receive NULL.
*/
void offerDestroy(Offer offer);

/**
* Creates a newly allocated copy of a given Offer.
*
* @return
* A newly allocated copy of the original Offer.
* NULL if the Offer is NULL or in any case of memory allocation failure and a specific error result (via a pointer).
*
*/
Offer offerCopy(Offer offer, OfferResult* error);

/**
* @return
* The email address of the costumer.
* This function asserts (Offer).
*/
char* offerGetCustomerMail(Offer offer);

/**
* Sets the customer email address.
*
* @return
* 	OFFER_NULL_ARG if Offer is NULL.
* 	OFFER_BAD_ARG if the given email address format is invalid.
*   OFFER_OUT_OF_MEM in any case of memory allocation failure.
* 	OFFER_SUCCESS otherwise.
*/
OfferResult offerSetCostumerMail(Offer Offer, char* new_mail);

/**
* @return
* The email address of the realtor.
* This function asserts (Offer).
*/
char* offerGetRealtorMail(Offer offer);

/**
* Sets the realtor email address.
*
* @return
* 	OFFER_NULL_ARG if Offer is NULL.
* 	OFFER_BAD_ARG if the given email address format is invalid.
*   OFFER_OUT_OF_MEM in any case of memory allocation failure.
* 	OFFER_SUCCESS otherwise.
*/
OfferResult offerSetRealtorMail(Offer Offer, char* new_mail);


/**
* @return
* The Offer's apartment service name.
* This function asserts (Offer).
*/
char* offerGetApartmentService(Offer Offer);


/**
* @return
* The apartment id number of the Offer.
* This function asserts (Offer).
*/
int offerGetApartmentId(Offer Offer);

/**
* @return
* The wanted price of the Offer.
* This function asserts (Offer).
*/
int offerGetNewPrice(Offer Offer);

/**
* Gets an offer indicated by two given emails (realtor & customer emails)
* @return
* OFFER_NOT_FOUND if there is no such offer.
* OFFER_SUCCESS otherwise and the relevant offer via outOffer.
* This function asserts (offer,realtor_email,customer_email != NULL).
*/
OfferResult getOfferByMails(Offer offer, char* realtor_email, char* customer_email, Offer* outOffer);

#endif /* OFFER_H */
