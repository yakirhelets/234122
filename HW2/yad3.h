#ifndef YAD3_H
#define YAD3_H
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "realtor.h"
#include "customer.h"
#include "offer.h"
#include "auxFunctions.h"
#include "mtm_ex2.h"
#include "map.h"
#include "set.h"
#include "list.h"

typedef struct yad3_t* Yad3;

typedef enum {
    YAD3_SUCCESS,
	YAD3_OUT_OF_MEM,
	YAD3_INVALID_COMMAND_LINE_PARAMETERS,
	YAD3_NULL_ARG,
	YAD3_CANNOT_OPEN_FILE,
	YAD3_BAD_ARG,
	YAD3_EMAIL_ALREADY_EXISTS,
	YAD3_EMAIL_DOES_NOT_EXIST,
	YAD3_EMAIL_WRONG_ACCOUNT_TYPE,
	YAD3_ALREADY_REQUESTED,
	YAD3_NOT_REQUESTED,
	YAD3_APARTMENT_SERVICE_ALREADY_EXISTS,
	YAD3_APARTMENT_SERVICE_DOES_NOT_EXIST,
	YAD3_APARTMENT_SERVICE_FULL,
	YAD3_APARTMENT_ALREADY_EXISTS,
	YAD3_APARTMENT_DOES_NOT_EXIST,
	YAD3_PURCHASE_WRONG_PROPERTIES,
	YAD3_REQUEST_WRONG_PROPERTIES,
	YAD3_REQUEST_ILLOGICAL_PRICE,
} Yad3Result;

/**
 * a macro for converting any type of map to a list.
 */
#define MAP_TO_LIST(type,map,list,copy_element,free_element) \
		list = listCreate(copy_element, free_element) ;\
		for (type iterator = (type) mapGetFirst(map) ; \
			iterator ;\
			iterator = (type) mapGetNext(map)) {\
				type to_insert = mapGet(map, iterator);\
				listInsertFirst(list,to_insert);\
			}

/**
 * Creates a new Yad3 service.
 * @param
* 		A pointer to an error message: In case of any allocation failure,
* 		pointer is set to YAD3_OUT_OF_MEM. Else: pointer is set to YAD3_SUCCESS.
 * @return
 * 		A new allocated Yad3 service.
*/
Yad3 yad3Create(Yad3Result* error);

/**
* Frees all memory allocated for a given Yad3 service.
* This function can receive NULL.
*/
void yad3Destroy(Yad3 yad3);

/**
* Adds a new realtor to the Yad3 service.
* Realtor details are given by email address, company name and his commission percentage. 
* @return
*   YAD3_NULL_ARG: given realtor is NULL.
*   YAD3_BAD_ARG: one (or more) of the parameters is invalid (email address must contain a single @, company name shouldn't be empty and comission percentage should be between 1 to 100)
* 	YAD3_EMAIL_ALREADY_EXISTS: given email address already exists in Yad3 realtors.
* 	YAD3_OUT_OF_MEM: memory allocation for the realtor failed.
*	YAD3_SUCCESS: realtor was added to Yad3 service.
*/
Yad3Result addRealtor(Yad3 yad3, char* email, char* company_name, int tax_percentage);

/**
* Adds a new customer to the Yad3 service.
* Customer details are given by email address, minimum desired apartment area,
* minimum apartment's number of rooms and the maximum price he is willing to pay
* for an apartment.
* @return
* 	YAD3_NULL_ARG: given customer is NULL.
* 	YAD3_BAD_ARG: one (or more) of the parameters is invalid (email address must contain a single '@'', minimum area/min rooms/max price should be positive)
* 	YAD3_EMAIL_ALREADY_EXISTS: given email address already exists in Yad3 customers.
* 	YAD3_OUT_OF_MEM: memory allocation for the realtor failed.
*	YAD3_SUCCESS: realtor was added to Yad3 service.
*/
Yad3Result addCustomer(Yad3 yad3, char* email, int min_area, int min_rooms, int max_price);

/**
* Removes a realtor from the Yad3 service by a given email address.
* @return
* 	YAD3_NULL_ARG: given email address is NULL.
* 	YAD3_BAD_ARG: given email address is invalid (should contain a single '@')
* 	YAD3_EMAIL_WRONG_ACCOUNT_TYPE: given email address belongs to a customer.
* 	YAD3_EMAIL_DOES_NOT_EXIST: realtor's email was not found in the system.
*	YAD3_SUCCESS: realtor was successfully removed from Yad3.
*/
Yad3Result removeRealtor(Yad3 yad3, char* email);

/**
* Removes a customer from the Yad3 service by a given email address.
* @return
* 	YAD3_NULL_ARG: given email address is NULL.
* 	YAD3_BAD_ARG: given email address is invalid (should contain a single '@')
* 	YAD3_EMAIL_WRONG_ACCOUNT_TYPE: given email address belongs to a realtor.
* 	YAD3_EMAIL_DOES_NOT_EXIST: customer's email was not found in the system.
*	YAD3_SUCCESS: customer was successfully removed from Yad3.
*/
Yad3Result removeCustomer(Yad3 yad3, char* email);

/**
* Creates a newly allocated ApartmentService for a given realtor.
* Maximum number of apartments and service name are given as parameters.
* A realtor can have an unlimited amount of Apartment Service.
* @return
* 	YAD3_NULL_ARG: given email address/apartment service name is NULL.
* 	YAD3_BAD_ARG: given email address is invalid (should contain a single '@') / Maximum numberf of apartments is not positive.
*	YAD3_EMAIL_WRONG_ACCOUNT_TYPE: given realtor's email address belongs to a customer.
* 	YAD3_EMAIL_DOES_NOT_EXIST: realtors's email was not found in the system.
*   YAD3_APARTMENT_SERVICE_ALREADY_EXISTS: an existing ApartmentService already exists (same service name)
*	YAD3_OUT_OF_MEM: memory allocation for the ApartmentService failed.
*   YAD3_SUCCESS: A newly allocated ApartmentService was successfully added to the realtor.
*/
Yad3Result addApartmentService(Yad3 yad3, char* email, char* service_name, int max_apartments);

/**
* Removes an existing ApartmentService from a given realtor.
* @return
* 	YAD3_NULL_ARG: given email address/apartment service name is NULL.
* 	YAD3_BAD_ARG: given realtor's email address is invalid (should contain a single '@') / Service name is invalid (empty)
* 	YAD3_EMAIL_WRONG_ACCOUNT_TYPE: given realtor's email address belongs to a customer.
* 	YAD3_EMAIL_DOES_NOT_EXIST: realtors's email was not found in the system.
*   YAD3_APARTMENT_SERVICE_DOES_NOT_EXIST: given service name was not found in the realtor's database.
*   YAD3_SUCCESS: given Apartment Service was successfully deleted.
*/
Yad3Result removeApartmentService(Yad3 yad3, char* email, char* service_name);

/** Prints all the realtors who sell apartments that meet the requirements
 * of a given customer.
 * @return
 *   	YAD3_NULL_ARG: given email address of a customer is NULL.
 * 		YAD3_BAD_ARG: given customer's email is invalid (should contain a single '@'). 
 *		YAD3_EMAIL_WRONG_ACCOUNT_TYPE:  given customer's email address
 * 	    belongs to a realtor.
 *		YAD3_EMAIL_DOES_NOT_EXIST: given customer's email doesn't exist.
 *		YAD3_OUT_OF_MEM: in case of any memory allocation failure during the
 *      proccess.
 * 		YAD3_SUCCESS: printing operation was successfully done.
*/
Yad3Result reportRelevantRealtors(Yad3 yad3,FILE* output_channel, char* customer_mail);

/** Sends a purchase offer by a customer to a realtor regarding a specific apartment.
 * @return
 *   	YAD3_NULL_ARG: given customer's email/realtor's email address/service name is NULL.
 * 		YAD3_BAD_ARG: given realtor's/cutomer's email are invalid (should contain a single '@')/service name is empty
 *		YAD3_EMAIL_WRONG_ACCOUNT_TYPE:  given customer's email address
 * 	    belongs to a realtor or vice versa.
 *		YAD3_EMAIL_DOES_NOT_EXIST: given customer's/realtor's email doesn't exist.
 *		YAD3_APARTMENT_SERVICE_DOES_NOT_EXIST: given apartment service was not found in the realtor's services.
 * 		YAD3_APARTMENT_DOES_NOT_EXIST: given apartment id was not found in the realtor's apartments.
 * 		YAD3_REQUEST_WRONG_PROPERTIES: the offer made doesn't fit the customer's demands (area,number of rooms, price).
 * 		YAD3_REQUEST_ILLOGICAL_PRICE: illogical offer price - in case that the customer will pay less/the same without brokerage.
 *      YAD3_ALREADY_REQUESTED: offer already exists.
 *		YAD3_OUT_OF_MEM: in case of any memory allocation failure during offer creation.
 * 		YAD3_SUCCESS: otherwise
*/
Yad3Result customerMakeOffer(Yad3 yad3, char* customer_mail, char* realtor_mail, char* service_name,
                             int apartment_id, int new_price);
 
/** Send a respond (choice="accept"/"decline") to an offer made by a customer to the realtor.
 * @return
 *   	YAD3_NULL_ARG: given customer's email/realtor's email address/choice e is NULL.
 * 		YAD3_BAD_ARG: given realtor's/cutomer's email are invalid (should contain a single '@') OR choice is invalid (must be either "accept" or "decline").
 *		YAD3_EMAIL_WRONG_ACCOUNT_TYPE:  given customer's email address
 * 	    belongs to a realtor or vice versa.
 *		YAD3_EMAIL_DOES_NOT_EXIST: given customer's/realtor's email doesn't exist.
 * 		YAD3_NOT_REQUESTED: such offer to be respoded doesn't exist.
*/                             
Yad3Result realtorRespondToOffer(Yad3 yad3, char* realtor_mail, char* customer_mail, char* choice);

/** Prints the customers who paid the most for their apartments in a decending order.
 * In case of multiple customers who spent the same amount of money in total,
 * print the customer by email-lexicographic order.
 * Customers who haven't paid at all won't be printed.
 * @param
 * 		count - The number of customers to print.
 * 				If the number of customers who paid is smaller than count,
 * 				print the suitable ones.
 * @return
 * 		YAD3_BAD_ARG: if count is not a positive number.
 * 		YAD3_SUCCESS: printing operation was successfully done.
*/
Yad3Result mostPayingCustomers(Yad3 yad3,FILE* output_channel, int count);

/**
* Removes an existing apartment from a realtor's apartment service.
* The apartment is given by a unique ID (positive number).
* @return
* 	YAD3_NULL_ARG: given email address/apartment service name is NULL.
* 	YAD3_BAD_ARG: given realtor's email address is invalid (should contain a single '@') / Service name is invalid (empty) / apartment's ID is negative
* 	YAD3_EMAIL_DOES_NOT_EXIST: realtors's email was not found in the system.
* 	YAD3_EMAIL_WRONG_ACCOUNT_TYPE: given realtor's email address belongs to a customer.
*   YAD3_APARTMENT_SERVICE_DOES_NOT_EXIST: given service name was not found in the realtor's database.
* 	YAD3_APARTMENT_DOES_NOT_EXIST: given apartment id was not found in the realtor's database.
*   YAD3_SUCCESS: given apartment was successfully deleted.
*/
Yad3Result realtorRemoveApartment(Yad3 yad3, char* realtor_mail, char* service_name, int id);

/**
* Adds a new apartment into a given apartment service of a realtor.
* The apartment is given by a string consists of the chars 'w','e'.
* A valid string is made only of 'w' and 'e's.
* ('w' stands for a WALL, 'e' stands for an empty square).
* Additionally, the apartment's price and dimensions are given (height, width).
* @return
* 	YAD3_NULL_ARG: given email address/apartment service name/apartment string is NULL.
* 	YAD3_BAD_ARG: given realtor's email address is invalid (should contain a single '@') / Service name is invalid (empty) / apartment string is invalid (contains other chars other than 'w','e') / apartment price isn't divisable by 100 / height and width are not positive / id is negative.
* 	YAD3_EMAIL_DOES_NOT_EXIST: realtors's email was not found in the system.
* 	YAD3_EMAIL_WRONG_ACCOUNT_TYPE: given realtor's email address belongs to a customer.
*   YAD3_APARTMENT_SERVICE_DOES_NOT_EXIST: given service name was not found in the realtor's database.
* 	MTM_APARTMENT_ALREADY_EXISTS: given apartment id already exists the realtor's database.
*   YAD3_SUCCESS: given Apartment Service was successfully deleted.
*/
Yad3Result realtorAddApartment(Yad3 yad3, char* realtor_mail, char* service_name, int id, int price, int width, int height, char* matrix);


/**
* Purchase a specific apartment by a customer.
* After purchase, the apartment is removed from the realtor's service.
* @return
* 		YAD3_NULL_ARG: customer's/realtor's mail/service name is NULL.
*		YAD3_BAD_ARG: given realtor's/cutomer's email are invalid (should contain a single '@')/service name is empty/apartment id is not positive
*		YAD3_EMAIL_WRONG_ACCOUNT_TYPE: given customer's email address
 * 	    belongs to a realtor or vice versa.
* 		YAD3_EMAIL_DOES_NOT_EXIST: given customer's/realtor's email doesn't exist.
*		YAD3_APARTMENT_SERVICE_DOES_NOT_EXIST: given service name was not found in the realtor's database.
* 		YAD3_APARTMENT_DOES_NOT_EXIST: no such apartment with its ID exists.
*		YAD3_PURCHASE_WRONG_PROPERTIES: purchase doesn't fit the customer's needs
* 										(regarding the apartment's price, rooms and area).
*		YAD3_SUCCESS: purchase proccess was successfully done.
*/
Yad3Result customerPurchase(Yad3 yad3, char* customer_mail, char* realtor_mail, char* service_name, int id);

/**
* Prints realtors by their rank in a descending order. A realtor's rank is 
* a number consists the number of apartments he has for sale , their price and
* their area.
* @param
* 	count - The number of realtors to print.
* 			If the total number of realtors is smaller than count,
* 			print the suitable ones.
* @return
* 	YAD3_BAD_ARG: count is not positive.
*   YAD3_SUCCESS: print action was successfully done.
*/
Yad3Result reportSignificantRealtors(Yad3 yad3,FILE* output_channel, int count);

#endif /* YAD3_H */
