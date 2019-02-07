#ifndef CUSTOMER_H
#define CUSTOMER_H

typedef struct customer_t* Customer;

typedef enum {
	CUSTOMER_SUCCESS, CUSTOMER_NULL_ARG, CUSTOMER_OUT_OF_MEM,
	CUSTOMER_BAD_ARG
} CustomerResult;

/**
* Creates a new customer type:
* The structure is given by email, and the customer's demand for
* minimal apartment area and number of rooms.
* The maximum price which the customer demands is given by max_price.
* @return
* 	A new allocated customer type.
* 	In case of invalid input or any kind of memory allocation failure - return NULL, and a specific error
*   result via pointer
*/
Customer customerCreate(char* email, int min_area, int min_rooms, int max_price, CustomerResult* error);

/**
* Frees all memory allocated for the given customer.
* This function can receive NULL.
*/
void customerDestroy(Customer customer);

/**
* Creates a newly allocated copy of a given customer.
*
* @return
* A newly allocated copy of the original customer.
* NULL if the customer is NULL or in any case of memory allocation failure, and a specific error
* result via pointer
*/
Customer customerCopy(Customer customer, CustomerResult* error);

/**
* @return
* The minmimum area which the customer demands.
* This function asserts (customer != NULL).
*/
int customerGetMinArea(Customer customer);


/**
* Sets the apartment's minimum area which the customer demands.
*
* @return
* 	CUSTOMER_NULL_ARG if customer is NULL.
* 	CUSTOMER_BAD_ARG if the minimum area given is not positive.
* 	CUSTOMER_SUCCESS otherwise.
*/
CustomerResult customerSetMinArea(Customer customer, int min_area);

/**
* @return
* The minmimum rooms which are the customer demands.
* This function asserts (customer != NULL).
*/
int customerGetMinRooms(Customer customer);

/**
* Sets the apartment's minimum number of rooms which the customer demands.
*
* @return
* 	CUSTOMER_NULL_ARG if customer is NULL.
* 	CUSTOMER_BAD_ARG if the minimum number of rooms given is not positive.
* 	CUSTOMER_SUCCESS otherwise.
*/
CustomerResult customerSetMinRooms(Customer customer, int min_rooms);

/**
* @return
* The maximum price which the customer demands.
* This function asserts (customer != NULL).
*/
int customerGetMaxPrice(Customer customer);

/**
* Sets the apartment's maximum price which the customer demands.
*
* @return
* 	CUSTOMER_NULL_ARG if customer is NULL.
* 	CUSTOMER_BAD_ARG if the maximum price given is not positive.
* 	CUSTOMER_SUCCESS otherwise.
*/
CustomerResult customerSetMaxPrice(Customer customer, int max_price);

/**
* @return
* The email address of the customer.
* This function asserts (customer != NULL).
*/
char* customerGetMail(Customer customer);

/**
* Sets the customer email address.
*
* @return
* 	CUSTOMER_NULL_ARG if customer or new_mail is NULL.
* 	CUSTOMER_BAD_ARG if the given email address format is invalid.
*   CUSTOMER_OUT_OF_MEM in any case of memory allocation failure.
* 	CUSTOMER_SUCCESS otherwise.
*/
CustomerResult customerSetMail(Customer customer, char* new_mail);

/**
* Adds the amount of money that the customer spent on an apartment.
*
* @return
*	CUSTOMER_NULL_ARG if customer is NULL.
* 	CUSTOMER_BAD_ARG if the given amount is not positive.
* 	CUSTOMER_SUCCESS otherwise.
*/
CustomerResult customerSetSpentByFar(Customer customer, int added_price);

/**
* @return
* The amount of money that the customer spent by far.
* This function asserts (customer != NULL).
*/
int customerGetSpentByFar(Customer customer);

#endif /* CUSTOMER_H */
