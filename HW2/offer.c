#include "offer.h"
#include "auxFunctions.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

struct offer_t {
    char* customer_email;
    char* realtor_email;
    char* apartment_service;
    int apartment_id;
    int new_price;
};

Offer offerCreate(char* customer_email, char* realtor_email, char* apartment_service, int apartment_id, int new_price,
                  OfferResult* error)  {
    if (!customer_email || !realtor_email || !apartment_service) {
        *error = OFFER_NULL_ARG;
        return NULL;
    }
    if (!isValidMail(customer_email) || !isValidMail(realtor_email) || !isValidName(apartment_service) ||
        new_price <= 0 || apartment_id < 0) {
            *error = OFFER_BAD_ARG;
            return NULL; 
    }
    Offer offer = malloc(sizeof(*offer));
    if (!offer)  {
        *error = OFFER_OUT_OF_MEM;
        return NULL;
    }
    offer->customer_email = malloc(sizeof(char)*(strlen(customer_email)+1));
    if (!offer->customer_email)  {
        free(offer);
        *error = OFFER_OUT_OF_MEM;
        return NULL;
    }
    strcpy(offer->customer_email, customer_email);
    
    offer->realtor_email = malloc(sizeof(char)*(strlen(realtor_email)+1));
    if (!offer->realtor_email)  {
        offerDestroy(offer);
        *error = OFFER_OUT_OF_MEM;
        return NULL;
    }
    strcpy(offer->realtor_email, realtor_email);

    offer->apartment_service = malloc(sizeof(char)*(strlen(apartment_service)+1));
    if (!offer->apartment_service)  {
        offerDestroy(offer);
        *error = OFFER_OUT_OF_MEM;
        return NULL;
    }
    strcpy(offer->apartment_service, apartment_service);
    
    offer->apartment_id = apartment_id;
    offer->new_price = new_price;
    *error = OFFER_SUCCESS;
    return offer;
}

void offerDestroy(Offer offer)  {
	if (!offer)	{
		return;
	}
    free(offer->apartment_service);
    free(offer->realtor_email);
    free(offer->customer_email);
    free(offer);
}

Offer offerCopy(Offer offer, OfferResult* error)    {
    if (!offer) {
        *error = OFFER_NULL_ARG;
        return NULL;
    }
    Offer copy = offerCreate(offer->customer_email, offer->realtor_email,
                        offer->apartment_service, offer->apartment_id, offer->new_price, error);
    if (!copy) {
        *error = OFFER_OUT_OF_MEM;
        return NULL;
    }
    *error = OFFER_SUCCESS;
    return copy;
}

char* offerGetCustomerMail(Offer offer) {
    assert(offer!=NULL);
    return offer->customer_email;
}

OfferResult offerSetCostumerMail(Offer offer, char* new_mail) {
    if (!offer || !new_mail) {
        return OFFER_NULL_ARG;
    }
    if (isValidMail(new_mail)) {
        offer->customer_email = (char*)realloc(offer->customer_email, sizeof(char)*(strlen(new_mail)+1));
        if (!(offer->customer_email)) {
            return OFFER_OUT_OF_MEM;
        }
        strcpy(offer->customer_email,new_mail);
    } else {
        return OFFER_BAD_ARG;
    }
    return OFFER_SUCCESS;   
}

char* offerGetRealtorMail(Offer offer)  {
    assert(offer!=NULL);
    return offer->realtor_email;
}

OfferResult offerSetRealtorMail(Offer offer, char* new_mail) {
    if (!offer || !new_mail) {
        return OFFER_NULL_ARG;
    }
    if (isValidMail(new_mail)) {
        offer->realtor_email = (char*)realloc(offer->realtor_email, sizeof(char)*(strlen(new_mail)+1));
        if (!(offer->realtor_email)) {
            return OFFER_OUT_OF_MEM;
        }
        strcpy(offer->realtor_email,new_mail);
    } else {
        return OFFER_BAD_ARG;
    }
    return OFFER_SUCCESS;   
}

char* offerGetApartmentService(Offer offer) {
    assert(offer!=NULL);
    return offer->apartment_service;
}

int offerGetApartmentId(Offer offer) {
    assert(offer!=NULL);
    return offer->apartment_id;
}

int offerGetNewPrice(Offer offer) {
    assert(offer!=NULL);
    return offer->new_price;
}

OfferResult getOfferByMails(Offer offer, char* realtor_email, char* customer_email, Offer* outOffer) {
    assert(offer!=NULL);
    assert(realtor_email!=NULL);
    assert(customer_email!=NULL);
    if (strcmp(realtor_email, offerGetRealtorMail(offer))==0 && strcmp(customer_email, offerGetCustomerMail(offer))==0) {
        *outOffer = offer;
        return OFFER_SUCCESS;
    }
    return OFFER_NOT_FOUND;
}
