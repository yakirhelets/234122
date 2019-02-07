#include "yad3.h"

struct yad3_t {
	Map realtors_map;
	Map customers_map;
	Set offers_set;
};

	////////////////////////////////////////////////////////////////////////
	// *~*~*~*~*~*~*~*~*~*~*~ AUXILIARY FUNCTIONS *~*~*~*~*~*~*~*~*~*~*~  //
	////////////////////////////////////////////////////////////////////////

/******************************************************************************/
static int sortByMail(ListElement realtor1, ListElement realtor2);
static Yad3Result ApartmentServiceResultToYad3Result(ApartmentServiceResult error);
static Yad3Result ListResultToYad3Result(ListResult error);
static Yad3Result isMailInRealtor(Yad3 yad3, char* email, bool checkRealtor);
static bool isValidString(char* str, int width, int height);
static SquareType** stringToMatrix(char* string, int height, int width);
static int calcAverage(Realtor realtor, bool isArea);
static int compareCustomer(MapDataElement customer1,MapDataElement customer2);
static bool isPositive(ListElement customer, ListFilterKey key);
static bool isPositiveApts(ListElement realtor, ListFilterKey key);
static int realtorCalcRank(int realtor_total_num_of_apts, int price_average, int area_average);
static int compareRealtor(MapDataElement realtor1,MapDataElement realtor2);
static int realtorTotalNumberOfApartments(Realtor realtor);
static int calcFinalPrice(Apartment apartment, Realtor realtor);
static bool doesMatchProperties(Customer customer, Apartment apartment);
static void matrixDestroy(SquareType** matrix, int height);
static ListElement listCustomerCopy (ListElement customer);
static void listCustomerDestroy (ListElement customer);
static ListElement listRealtorCopy (ListElement realtor);
static void listRealtorDestroy (ListElement realtor);
static MapDataElement mapRealtorCopy(constMapDataElement realtor);
static void mapRealtorDestroy (MapDataElement realtor);
static MapDataElement mapCustomerCopy(constMapDataElement customer);
static void mapCustomerDestroy (MapDataElement customer);
static SetElement setOfferCopy(SetElement offer);
static void setOfferDestroy (SetElement offer);
static int offerCompare (SetElement offer1, SetElement offer2);
/******************************************************************************/

/** Compares two given strings (email addresses) by lexicographic order
 * @return
if return value < 0 then it indicates s1 is less than s2.
if return value > 0 then it indicates s2 is less than s1.
if return value= 0 then it indicates s1 is equal to s2.
 */
static int sortByMail(ListElement realtor1, ListElement realtor2) {
	char* realtor1_email = realtorGetMail(realtor1);
	char* realtor2_email = realtorGetMail(realtor2);
	return strcmp(realtor1_email,realtor2_email);
}

/** Error handling function: returns a proper error type accordingly
 * If no error was found, return YAD3_SUCCESS.
 */
static Yad3Result ApartmentServiceResultToYad3Result(ApartmentServiceResult error) {
	switch (error) {
	case APARTMENT_SERVICE_NULL_ARG: return YAD3_APARTMENT_SERVICE_DOES_NOT_EXIST; break;
	case APARTMENT_SERVICE_FULL: return YAD3_APARTMENT_SERVICE_FULL; break;
	case APARTMENT_SERVICE_OUT_OF_BOUNDS: return YAD3_BAD_ARG; break;
	case APARTMENT_SERVICE_EMPTY: return YAD3_APARTMENT_DOES_NOT_EXIST; break;
	case APARTMENT_SERVICE_NO_FIT: return YAD3_APARTMENT_DOES_NOT_EXIST; break;
	case APARTMENT_SERVICE_ALREADY_EXISTS: return YAD3_APARTMENT_ALREADY_EXISTS; break;
	case APARTMENT_SERVICE_OUT_OF_MEM: return YAD3_OUT_OF_MEM; break;
	case APARTMENT_SERVICE_SUCCESS: break;
	}
	return YAD3_SUCCESS;
}

/** List Error handling function: returns a proper error type accordingly
 * If no error was found, return YAD3_SUCCESS.
 */
static Yad3Result ListResultToYad3Result(ListResult error) {
	switch (error) {
	case LIST_INVALID_CURRENT: return YAD3_BAD_ARG; break;
	case LIST_NULL_ARGUMENT: return YAD3_NULL_ARG; break;
	case LIST_OUT_OF_MEMORY: return YAD3_OUT_OF_MEM; break;
	case LIST_SUCCESS: break;
	}
	return YAD3_SUCCESS;
}

/**
 * Checks whether a given email address belongs to a realtor.
 * This function asserts(email!=NULL).
 * @return
 * 	YAD3_EMAIL_DOES_NOT_EXIST: given mail address doesn't belong to any realtor.
 * 	YAD3_EMAIL_WRONG_ACCOUNT_TYPE: given mail belongs to a customer and not a realtor.
 *	YAD3_SUCCESS: given mail exists and belongs to a realtor.
 */
static Yad3Result isMailInRealtor(Yad3 yad3, char* email, bool checkRealtor) {
	assert(yad3!=NULL && email!=NULL);
		if (mapContains(yad3->realtors_map, email) && !mapContains(yad3->customers_map,email)) { // a realtor's mail
			return checkRealtor?YAD3_SUCCESS:YAD3_EMAIL_WRONG_ACCOUNT_TYPE;
		} else {
			if (mapContains(yad3->customers_map, email) && !mapContains(yad3->realtors_map,email)) { // customer's mail
				return checkRealtor?YAD3_EMAIL_WRONG_ACCOUNT_TYPE:YAD3_SUCCESS;
			}
		}
	return YAD3_EMAIL_DOES_NOT_EXIST;
}

/** Returns true if the given string is a valid representation of an apartment.
 * A valid representation of an apartment is a string made of 'w','e' chars only.
 * @return
 * 		true if the given string is a valid representation of an apartment,
 * 		false otherwise.
 */
static bool isValidString(char* str, int width, int height) {
	int len = strlen(str);
	if (len == 0) return false;
	if (len != height*width) return false;
	while (*str) {
		if (*str != 'e' && *str != 'w') return false;
		str++;
	}
	return true;
}
/** frees all allocated memory of a given SquareType matrix.
 *  This function can receive NULL.
 */
static void matrixDestroy(SquareType** matrix, int height) {
	if (!matrix) {
		return;
	}
	for (int i=0 ; i<height ; i++)  {
		free(matrix[i]);
	}
	free(matrix);
}

/** Returns a 2D Array from a given string representing an apartment.
 * The string given should be consisted of 'w','e' (representing WALL and EMPTY
 * squares respectively).
 * The array's number of columns is width and number of rows in height.
 * @return
		A SquareType matrix which represents the apartment's structure.
		NULL is returned in case of memory allocation failure.
 */
static SquareType** stringToMatrix(char* string, int height, int width)    {
	SquareType** matrix = malloc(sizeof(SquareType*)*height);
	if (!matrix) {
		return NULL;
	}
	for (int i=0 ; i<height ; i++)  {
		matrix[i] = malloc(sizeof(SquareType)*width);
		if (!matrix[i]) {
			for (int j=0 ; j<i ; j++)   {
				free(matrix[j]);
			}
			free(matrix);
			return NULL;
		}
	}
	int c = 0;
	for (int i=0; i<height; i++) {
		for (int j=0; j<width; j++) {
			string[c++]=='e'?(matrix[i][j]=EMPTY):(matrix[i][j]=WALL);
		}
	}
	return matrix;
}

/** Calculates and returns the medians' average prices/areas of all of the
 * services that belongs to a given realtor.
 * A boolean flag is given representing which average (prices/areas)
 * should be calculated. (true = calculate by areas / false = by prices)
 * This function asserts (realtor!=NULL).
 * @return
 * The medians' average prices/areas of all of the
 * services that belongs to a given realtor.
 */
static int calcAverage(Realtor realtor, bool isArea) {
	assert(realtor!=NULL);
	Map apartment_service_map = realtorGetServices(realtor);
	int sum=0;
	int out_result=0;
	int count=0;
	MAP_FOREACH(ApartmentService,iterator,apartment_service_map)   {
		ApartmentService current = mapGet(apartment_service_map, iterator);
		isArea?(serviceAreaMedian(current, &out_result)):(servicePriceMedian(current, &out_result));
		sum+=out_result;
		count++;
	}
	return (sum/count);
}

/** Compares two customers by the money the spent by far and their email addresses. (lexicographically)
 * @return
 * 	-1: in case that customer1 has spent lower amount than customer2.
 * 	1: in case that customer1 has spent more than customer2.
 */
static int compareCustomer(MapDataElement customer1,MapDataElement customer2) {
	int spent_by_customer1 = customerGetSpentByFar(customer1);
	int spent_by_customer2 = customerGetSpentByFar(customer2);

	if (spent_by_customer1 == spent_by_customer2) {
		if (strcmp(customerGetMail(customer1), customerGetMail(customer2)) < 0 ) {
			return -1;
		} else {
			return 1;
		}
	}
	if (spent_by_customer1 > spent_by_customer2) {
		return -1;
	} else {
		return 1;
	}
}

/** Returns true if the amount of money spent my a customer is positive.
 * @return
 * 		TRUE if the amount of money spent my a customer is positive, FALSE otherwise.
 */
static bool isPositive(ListElement customer, ListFilterKey key) {
	return (customerGetSpentByFar(customer) > *(int*)key);
}

/** Returns true if the total number of apartments of a realtor is positive.
 * @return
 * 		TRUE if the total number of apartments of a given realtor is positive, FALSE otherwise.
 */
static bool isPositiveApts(ListElement realtor, ListFilterKey key) {
	assert(realtor!=NULL);
	int num_of_apartments = realtorTotalNumberOfApartments(realtor);
	return num_of_apartments >= *(int*)key;
}

/** Returns true if the apartment matches the requirements of the customer.
 * @return
 * 		TRUE if the apartment matches the requirements of the customer, FALSE otherwise.
 */
static bool doesMatchProperties(Customer customer,Apartment apartment) {
	if (customerGetMinArea(customer)>apartmentTotalArea(apartment)
		|| customerGetMinRooms(customer)>apartmentNumOfRooms(apartment)
		|| customerGetMaxPrice(customer)<apartmentGetPrice(apartment)) {
		return false;
	} else {
		return true;
	}
}

/** Calculates and returns the a rank (realtor's rank) which
 * is calculated by the formula: 1,000,000*Number of realtor's apartments)+Realtor's apartments' price average+(100,000*Realtor's apartments' area average);
 * @return
 * 		The realtor's rank.
 */
static int realtorCalcRank(int realtor_total_num_of_apts, int price_average, int area_average) {
	return (1000000*realtor_total_num_of_apts)+price_average+(100000*area_average);
}

/** Compares two realtors by their rank and then by their email addresses. (lexicographically)
 * @return
 * 	-1: in case that realtor1 has lower rank than customer2.
 * 	 1: in case that realtor1 has higher rank than customer2.
 */
static int compareRealtor(MapDataElement realtor1,MapDataElement realtor2) {

	int realtor1_rank = realtorCalcRank(realtorTotalNumberOfApartments(realtor1), calcAverage(realtor1, false), calcAverage(realtor1, true));
	int realtor2_rank = realtorCalcRank(realtorTotalNumberOfApartments(realtor2), calcAverage(realtor2, false), calcAverage(realtor2, true));

	if (realtor1_rank == realtor2_rank) {
		if (strcmp(realtorGetMail(realtor1), realtorGetMail(realtor2)) < 0 ) {
			return -1;
		} else {
			return 1;
		}
	}
	if (realtor1_rank > realtor2_rank) {
		return -1;
	} else {
		return 1;
	}
}

/** Calculates and returns the total number of apartments which are being
 *  sold by a given realtor.
 * This function asserts (realtor!=NULL).
 * @return
 *  The total number of apartments which are being sold by a given realtor.
 */
static int realtorTotalNumberOfApartments(Realtor realtor) {
	assert(realtor!=NULL);
	int num_of_apartments = 0;
	Map apartment_service_map = realtorGetServices(realtor);
	MAP_FOREACH(ApartmentService,iterator,apartment_service_map) {
		num_of_apartments += serviceNumberOfApatments(iterator);
	}
	return num_of_apartments;
}

/** Calculates and returns the final apartment price after adding the
 *  realtor's commission to the customer's offered price.
 * @return
 * 	The final apartment's price including fees.
 */
static int calcFinalPrice(Apartment apartment, Realtor realtor) {
	return apartmentGetPrice(apartment)+(apartmentGetPrice(apartment)*realtorGetTaxPercentage(realtor)/100);
}

/**
 * A copying function of a customer in a List.
 * @return
 * A newly allocated copy of a given customer.
 * In case of any memory allocation, return NULL.
 */
static ListElement listCustomerCopy (MapDataElement customer) {
	CustomerResult error;
	Customer copy = customerCopy(customer, &error);
	if (!copy) {
		return NULL;
	}
	return copy;
}

/* Frees all memory allocated for a given customer in a List.
* This function can receive NULL.
*/
static void listCustomerDestroy (ListElement customer) {
	customerDestroy(customer);
}

/**
 * A copying function of a realtor in a List.
 * @return
 * A newly allocated copy of a given realtor.
 * In case of any memory allocation, return NULL.
 */
static ListElement listRealtorCopy (MapDataElement realtor) {
	RealtorResult error;
	Realtor copy = realtorCopy(realtor, &error);
	if (!copy) {
		return NULL;
	}
	return copy;
}

/* Frees all memory allocated for a given realtor in a List.
* This function can receive NULL.
*/
static void listRealtorDestroy (ListElement realtor) {
	realtorDestroy(realtor);
}

/**
 * A copying function of a realtor in a Map.
 * @return
 * A newly allocated copy of a given realtor.
 * In case of any memory allocation, return NULL.
 */
static MapDataElement mapRealtorCopy(constMapDataElement realtor) {
	RealtorResult error;
	Realtor copy = realtorCopy((Realtor)realtor, &error);
	if (!copy) {
		return NULL;
	}
	return copy;
}

/* Frees all memory allocated for a given realtor in a Map.
* This function can receive NULL.
*/
static void mapRealtorDestroy (MapDataElement realtor) {
	realtorDestroy(realtor);
}

/**
 * A copying function of a customer in a Map.
 * @return
 * A newly allocated copy of a given realtor.
 * In case of any memory allocation, return NULL.
 */
static MapDataElement mapCustomerCopy(constMapDataElement customer) {
	CustomerResult error;
	Customer copy = customerCopy((Customer)customer, &error);
	if (!copy) {
		return NULL;
	}
	return copy;
}

/* Frees all memory allocated for a given customer in a Map.
* This function can receive NULL.
*/
static void mapCustomerDestroy (MapDataElement customer) {
	customerDestroy(customer);
}

/**
 * A copying function of an offer in a Set.
 * @return
 * A newly allocated copy of a given offer.
 * In case of any memory allocation, return NULL.
 */
static SetElement setOfferCopy(SetElement offer) {
	OfferResult error;
	Offer copy = offerCopy((Offer)offer, &error);
	if (!copy) {
		return NULL;
	}
	return copy;
}

/* Frees all memory allocated for a given offer in a Set.
* This function can receive NULL.
*/
static void setOfferDestroy (SetElement offer) {
	offerDestroy(offer);
}

/**
 * Compares between two given offer by customer_mail and realtor_mail fields.
 *
 * @return
 * 1 if offer2 is NULL or one of the fields is un-even.
 * -1 if offer1 is NULL
 * 0 if offer1 and offer2 are NULL, or their mail fields are equal.
 */
static int offerCompare (SetElement offer1, SetElement offer2)    {
	if (!offer1 && !offer2) {
		return 0;
	}
	if (offer1 && !offer2)  {
		return 1;
	}
	if (!offer1 && offer2)  {
		return -1;
	}
	int realtor_cmp = strcmp(offerGetRealtorMail(offer1), offerGetRealtorMail(offer2));
	int customer_cmp = strcmp(offerGetCustomerMail(offer1), offerGetCustomerMail(offer2));
	if (realtor_cmp==0 && customer_cmp==0)  {
		return 0;
	}
	return 1;
}

	////////////////////////////////////////////////////////////////////////
	// *~*~*~*~*~*~~*~*~*~ END OF AUXILIARY FUNCTIONS *~*~*~*~*~~*~*~ //////
	////////////////////////////////////////////////////////////////////////

Yad3 yad3Create(Yad3Result* error) {
	*error = YAD3_OUT_OF_MEM;
	Yad3 yad3 = malloc(sizeof(*yad3));
	if (!yad3) {
		return NULL;
	}
	yad3->realtors_map = mapCreate(mapRealtorCopy,copyString, mapRealtorDestroy, freeString, compareString);
	if (!yad3->realtors_map)    {
		yad3Destroy(yad3);
		return NULL;
	}
	yad3->customers_map = mapCreate(mapCustomerCopy, copyString, mapCustomerDestroy, freeString, compareString);
	if (!yad3->customers_map)    {
		yad3Destroy(yad3);
		return NULL;
	}
	yad3->offers_set = setCreate(setOfferCopy, setOfferDestroy, offerCompare);
	if (!yad3->offers_set)    {
		yad3Destroy(yad3);
		return NULL;
	}
	*error = YAD3_SUCCESS;
	return yad3;
}

void yad3Destroy(Yad3 yad3) {
	if (!yad3) {
		return;
	}
	mapDestroy(yad3->realtors_map);
	mapDestroy(yad3->customers_map);
	setDestroy(yad3->offers_set);
	free(yad3);
}

Yad3Result addRealtor(Yad3 yad3, char* email, char* company_name, int tax_percentage) {
	if (!yad3 || !email || !company_name) return YAD3_NULL_ARG;
	MapResult result;
	RealtorResult error;
	if (isMailInRealtor(yad3,email,true) != YAD3_EMAIL_DOES_NOT_EXIST) {
		return YAD3_EMAIL_ALREADY_EXISTS;
	}
	Realtor realtor = realtorCreate(email, company_name, tax_percentage, &error);
	if (!realtor)   {
		switch (error) {
		case REALTOR_NULL_ARG: return YAD3_NULL_ARG; break;
		case REALTOR_BAD_ARG: return YAD3_BAD_ARG; break;
		case REALTOR_OUT_OF_MEM: return YAD3_OUT_OF_MEM; break;
		default: break;
		}
	}
	result = mapPut(yad3->realtors_map, email, realtor);
	if(result == MAP_OUT_OF_MEMORY)   {
		realtorDestroy(realtor);
		return YAD3_OUT_OF_MEM;
	}
	if(result == MAP_NULL_ARGUMENT)   {
		realtorDestroy(realtor);
		return YAD3_NULL_ARG;
	}
	realtorDestroy(realtor);
	return YAD3_SUCCESS;
}

Yad3Result addCustomer(Yad3 yad3, char* email, int min_area, int min_rooms, int max_price) {
	if (!yad3 || !email) return YAD3_NULL_ARG;
	MapResult result;
	if (isMailInRealtor(yad3,email,false) != YAD3_EMAIL_DOES_NOT_EXIST) {
		return YAD3_EMAIL_ALREADY_EXISTS;
	}
	CustomerResult error;
	Customer customer = customerCreate(email, min_area, min_rooms, max_price, &error);
	if (!customer)   {
		switch (error) {
		case CUSTOMER_NULL_ARG: return YAD3_NULL_ARG; break;
		case CUSTOMER_BAD_ARG: return YAD3_BAD_ARG; break;
		case CUSTOMER_OUT_OF_MEM: return YAD3_OUT_OF_MEM; break;
		default: break;
		}
	}
	result = mapPut(yad3->customers_map, email, customer);
	if(result == MAP_OUT_OF_MEMORY)   {
		customerDestroy(customer);
		return YAD3_OUT_OF_MEM;
	}
	if(result == MAP_NULL_ARGUMENT)   {
		customerDestroy(customer);
		return YAD3_NULL_ARG;
	}
	customerDestroy(customer);
	return YAD3_SUCCESS;
}

Yad3Result removeRealtor(Yad3 yad3, char* email) {
	MapResult result;
	if (!yad3 || !email) {
		return YAD3_NULL_ARG;
	}
	if (!isValidMail(email)) {
		return YAD3_BAD_ARG;
	}
	result = mapRemove(yad3->realtors_map, email);
	if (result == MAP_SUCCESS)   {
		return YAD3_SUCCESS;
	}
	return isMailInRealtor(yad3,email,true);
}

Yad3Result removeCustomer(Yad3 yad3, char* email) {
	if (!yad3 || !email) return YAD3_NULL_ARG;
	MapResult result;
	if (!isValidMail(email)) {
		return YAD3_BAD_ARG;
	}
	result = mapRemove(yad3->customers_map, email);
	if (result == MAP_SUCCESS)   {
		return YAD3_SUCCESS;
	}
	result = isMailInRealtor(yad3,email,false);
	return result;
}

Yad3Result addApartmentService(Yad3 yad3, char* email, char* service_name, int max_apartments) {
	if (!yad3 || !email || !service_name) {
		return YAD3_NULL_ARG;
	}
	if (!isValidMail(email) || max_apartments <= 0 || !isValidName(service_name)) {
		return YAD3_BAD_ARG;
	}
	Yad3Result result = isMailInRealtor(yad3,email,true);
	if (result != YAD3_SUCCESS) {
		return result;
	}
	Realtor realtor = mapGet(yad3->realtors_map, email);
	if(mapContains(realtorGetServices(realtor),service_name)) {
		return YAD3_APARTMENT_SERVICE_ALREADY_EXISTS;
	} else {
		ApartmentService service = serviceCreate(max_apartments);
		if (!service)   {
			return YAD3_OUT_OF_MEM;
		}
		MapResult map_result = mapPut(realtorGetServices(realtor), service_name, service);
		if (map_result == MAP_OUT_OF_MEMORY) {
			serviceDestroy(service);
			return YAD3_OUT_OF_MEM;
		}
		serviceDestroy(service);
	}
	return YAD3_SUCCESS;
}

Yad3Result removeApartmentService(Yad3 yad3, char* email, char* service_name) {
	if (!yad3 || !email || !service_name) {
		return YAD3_NULL_ARG;
	}
	if (!isValidMail(email) || !isValidName(service_name)) {
		return YAD3_BAD_ARG;
	}
	Yad3Result result = isMailInRealtor(yad3,email,true);
	if (result != YAD3_SUCCESS) {
		return result;
	}
	Realtor realtor = mapGet(yad3->realtors_map, email);
	if(mapRemove(realtorGetServices(realtor),service_name) ==  MAP_ITEM_DOES_NOT_EXIST) {
		return YAD3_APARTMENT_SERVICE_DOES_NOT_EXIST;
	}
	return YAD3_SUCCESS;
}

Yad3Result reportRelevantRealtors(Yad3 yad3, FILE* output_channel, char* customer_mail) {
	if (!yad3 || !output_channel || !customer_mail) {
		return YAD3_NULL_ARG;
	}
	if (!isValidMail(customer_mail)) {
		return YAD3_BAD_ARG;
	}
	Yad3Result mail_result = isMailInRealtor(yad3, customer_mail,false);
	if (mail_result != YAD3_SUCCESS) {
		return mail_result;
	}

	Customer customer = mapGet(yad3->customers_map, customer_mail);
	Apartment outApartment;
	List relevant_realtors = listCreate(listRealtorCopy,listRealtorDestroy);
	ListResult list_result;
	MAP_FOREACH(Realtor,realtor_iterator,yad3->realtors_map) {
		Realtor realtor = mapGet(yad3->realtors_map, realtor_iterator);
		MAP_FOREACH(ApartmentService,service_iterator,realtorGetServices(realtor)) {
			ApartmentService service = mapGet(realtorGetServices(realtor), service_iterator);
			if (serviceSearch(service, customerGetMinArea(customer),
					customerGetMinRooms(customer), customerGetMaxPrice(customer),
					&outApartment) == APARTMENT_SERVICE_SUCCESS) {
				list_result = listInsertFirst(relevant_realtors,realtor);
				if (list_result == LIST_OUT_OF_MEMORY) {
					listDestroy(relevant_realtors);
					return YAD3_OUT_OF_MEM;
				}
				if (list_result == LIST_SUCCESS)  {
					break;
				}
			}
		}
	}
	list_result = listSort(relevant_realtors,sortByMail);
	if (list_result == LIST_OUT_OF_MEMORY) {
		listDestroy(relevant_realtors);
		return YAD3_OUT_OF_MEM;
	} else {
		LIST_FOREACH(Realtor,realtor_iterator,relevant_realtors) {
			mtmPrintRealtor(output_channel,realtorGetMail(realtor_iterator),realtorGetCompanyName(realtor_iterator));
		}
	}
	listDestroy(relevant_realtors);
	return YAD3_SUCCESS;
}

Yad3Result realtorAddApartment(Yad3 yad3, char* realtor_mail, char* service_name, int id,
		int price, int width, int height, char* matrix) {
	if (!yad3 || !realtor_mail || !service_name || !matrix) {
		return YAD3_NULL_ARG;
	}
	if (!isValidMail(realtor_mail)  || !isValidString(matrix,width,height) ||
			!isValidName(service_name) || id < 0 || width <= 0 || height <= 0 ||
			price <= 0 || price % 100 != 0) {
		return YAD3_BAD_ARG;
	}
	Yad3Result result = isMailInRealtor(yad3,realtor_mail,true);
	if (result != YAD3_SUCCESS) {
		return result;
	}
	Realtor realtor = mapGet(yad3->realtors_map, realtor_mail);
	if(!mapContains(realtorGetServices(realtor),service_name)) {
		return YAD3_APARTMENT_SERVICE_DOES_NOT_EXIST;
	} else {
		ApartmentService service = mapGet(realtorGetServices(realtor), service_name);
		SquareType** squares = stringToMatrix(matrix,height,width);
		Apartment new_apartment = apartmentCreate(squares,height,width,price);
		if (!new_apartment) {
			return YAD3_OUT_OF_MEM;
		}
		ApartmentServiceResult error = serviceAddApartment(service,new_apartment,id);
		Yad3Result result = ApartmentServiceResultToYad3Result(error);
		apartmentDestroy(new_apartment);
		matrixDestroy(squares, height);
		return result;
	}
	return result;
}

Yad3Result realtorRemoveApartment(Yad3 yad3, char* realtor_mail, char* service_name, int id) {
	if (!yad3 || !realtor_mail || !service_name) {
		return YAD3_NULL_ARG;
	}
	if (!isValidMail(realtor_mail) || !isValidName(service_name) || id < 0) {
		return YAD3_BAD_ARG;
	}
	Yad3Result result = isMailInRealtor(yad3,realtor_mail,true);
	if (result != YAD3_SUCCESS) {
		return result;
	}
	Realtor realtor = mapGet(yad3->realtors_map, realtor_mail);
	if(!mapContains(realtorGetServices(realtor),service_name)) {
		return YAD3_APARTMENT_SERVICE_DOES_NOT_EXIST;
	} else {
		Map map = realtorGetServices(realtor);
		ApartmentService service = mapGet(map,service_name);
		ApartmentServiceResult error = serviceDeleteById(service, id);
		result = ApartmentServiceResultToYad3Result(error);
	}
	return result;
}

Yad3Result customerPurchase(Yad3 yad3, char* customer_mail, char* realtor_mail, char* service_name, int id) {
	if (!yad3 || !realtor_mail || !customer_mail || !service_name) {
		return YAD3_NULL_ARG;
	}
	if (!isValidMail(customer_mail) || !isValidMail(realtor_mail) || !isValidName(service_name) || id < 0) {
		return YAD3_BAD_ARG;
	}
	//customer mail check
	Yad3Result result = isMailInRealtor(yad3,customer_mail,false);
	if (result != YAD3_SUCCESS) {
		return result;
	}
	//realtor mail check
	result = isMailInRealtor(yad3,realtor_mail,true);
	if (result != YAD3_SUCCESS) {
		return result;
	}
	Realtor realtor = mapGet(yad3->realtors_map, realtor_mail);
	if(!mapContains(realtorGetServices(realtor),service_name)) {
		return YAD3_APARTMENT_SERVICE_DOES_NOT_EXIST;
	}
	Customer customer = mapGet(yad3->customers_map, customer_mail);
	ApartmentService service = mapGet(realtorGetServices(realtor), service_name);
	Apartment apartment_to_purchase;
	ApartmentServiceResult search_result = serviceGetById(service, id, &apartment_to_purchase);
	switch (search_result) {
		case APARTMENT_SERVICE_NULL_ARG:apartmentDestroy(apartment_to_purchase); return YAD3_APARTMENT_SERVICE_DOES_NOT_EXIST; break;
		case APARTMENT_SERVICE_FULL:apartmentDestroy(apartment_to_purchase); return YAD3_APARTMENT_SERVICE_FULL; break;
		case APARTMENT_SERVICE_OUT_OF_BOUNDS:apartmentDestroy(apartment_to_purchase); return YAD3_BAD_ARG; break;
		case APARTMENT_SERVICE_EMPTY: return YAD3_APARTMENT_DOES_NOT_EXIST; break;
		case APARTMENT_SERVICE_NO_FIT:apartmentDestroy(apartment_to_purchase); return YAD3_APARTMENT_DOES_NOT_EXIST; break;
		case APARTMENT_SERVICE_ALREADY_EXISTS:apartmentDestroy(apartment_to_purchase); return YAD3_APARTMENT_ALREADY_EXISTS; break;
		case APARTMENT_SERVICE_OUT_OF_MEM:apartmentDestroy(apartment_to_purchase); return YAD3_OUT_OF_MEM; break;
		case APARTMENT_SERVICE_SUCCESS: break;
	}
	if(!doesMatchProperties(customer,apartment_to_purchase)) {
		apartmentDestroy(apartment_to_purchase);
		return YAD3_PURCHASE_WRONG_PROPERTIES;
	}
	int added_price = calcFinalPrice(apartment_to_purchase,realtor);
	customerSetSpentByFar(customer,added_price);
	serviceDeleteApartment(service,apartment_to_purchase);
	apartmentDestroy(apartment_to_purchase);
	return YAD3_SUCCESS;
}

Yad3Result reportSignificantRealtors(Yad3 yad3,FILE* output_channel, int count) {
	if (!yad3) return YAD3_NULL_ARG;
	if (count <= 0) return YAD3_BAD_ARG;
	List list;
	MAP_TO_LIST(Realtor,yad3->realtors_map,list,listRealtorCopy,listRealtorDestroy);
	int key = 0;
	List filtered = listFilter(list, isPositiveApts, &key);
	if (list)   {
		listDestroy(list);
	}
	ListResult result = listSort(filtered, compareRealtor);
	if (result == LIST_OUT_OF_MEMORY) {
		return YAD3_OUT_OF_MEM;
	}
	int list_size = listGetSize(filtered);
	if (list_size == 0) {
		if (filtered) {
			listDestroy(filtered);
		}
		return YAD3_SUCCESS;
	}
	Realtor iterator = listGetFirst(filtered);
	for (int i=0 ; i<((count<=list_size)?count:list_size) ; i++)   {
		mtmPrintRealtor(output_channel, realtorGetMail(iterator),realtorGetCompanyName(iterator));
		iterator = listGetNext(filtered);
	}
	if (filtered)   {
		listDestroy(filtered);
	}
	return YAD3_SUCCESS;
}

Yad3Result customerMakeOffer(Yad3 yad3, char* customer_mail, char* realtor_mail, char* service_name,
		int apartment_id, int new_price) {
	if (!yad3 || !customer_mail || !realtor_mail || !service_name) {
		return YAD3_NULL_ARG;
	}
	if (!isValidMail(customer_mail) || !isValidMail(realtor_mail) || !isValidName(service_name) || apartment_id < 0 || new_price < 0) {
		return YAD3_BAD_ARG;
	}
	Yad3Result result = isMailInRealtor(yad3,realtor_mail,true);
	if (result != YAD3_SUCCESS) {
		return result;
	}
	result = isMailInRealtor(yad3,customer_mail,false);
	if (result != YAD3_SUCCESS) {
		return result;
	}
	Realtor realtor = mapGet(yad3->realtors_map, realtor_mail);
	if(!mapContains(realtorGetServices(realtor),service_name)) {
		return YAD3_APARTMENT_SERVICE_DOES_NOT_EXIST;
	}
	Map map = realtorGetServices(realtor);
	ApartmentService service = mapGet(map, service_name);
	Apartment outApartment;
	ApartmentServiceResult service_result = serviceGetById(service,apartment_id, &outApartment);
	if (service_result != APARTMENT_SERVICE_SUCCESS) {
		apartmentDestroy(outApartment);
		return YAD3_APARTMENT_DOES_NOT_EXIST;
	}
	Customer customer = mapGet(yad3->customers_map,customer_mail);
	if (new_price > calcFinalPrice(outApartment, realtor))  {
		apartmentDestroy(outApartment);
		return YAD3_REQUEST_ILLOGICAL_PRICE;
	}
	if (customerGetMinArea(customer)>apartmentTotalArea(outApartment) ||
			customerGetMinRooms(customer)>apartmentNumOfRooms(outApartment) ||
			customerGetMaxPrice(customer)<new_price)  {
		apartmentDestroy(outApartment);
		return YAD3_REQUEST_WRONG_PROPERTIES;
	}
	OfferResult offer_result;
	Offer offer = offerCreate(customer_mail,realtor_mail,service_name,apartment_id,new_price, &offer_result);
	switch (offer_result) {
	case OFFER_NULL_ARG: apartmentDestroy(outApartment); return YAD3_NULL_ARG; break;
	case OFFER_BAD_ARG: apartmentDestroy(outApartment); return YAD3_BAD_ARG; break;
	case OFFER_OUT_OF_MEM: apartmentDestroy(outApartment); return YAD3_OUT_OF_MEM; break;
	default: break;
	}
	SetResult set_result = setAdd(yad3->offers_set,offer); // add the new offer to the offers_set
	switch (set_result) {
	case SET_OUT_OF_MEMORY: apartmentDestroy(outApartment); offerDestroy(offer); return YAD3_OUT_OF_MEM; break;
	case SET_ITEM_ALREADY_EXISTS: apartmentDestroy(outApartment); offerDestroy(offer); return YAD3_ALREADY_REQUESTED; break; // offer already exists in the set
	default: break;
	}
	offerDestroy(offer);
	apartmentDestroy(outApartment);
	return YAD3_SUCCESS;
}

Yad3Result realtorRespondToOffer(Yad3 yad3, char* realtor_mail, char* customer_mail, char* choice) {
	if (!yad3 || !realtor_mail || !customer_mail || !choice) {
		return YAD3_NULL_ARG;
	}
	if (!isValidMail(realtor_mail) || !isValidMail(customer_mail) || (strcmp(choice,"accept")!=0 && strcmp(choice,"decline")!=0)) {
		return YAD3_BAD_ARG;
	}
	Yad3Result result = isMailInRealtor(yad3,realtor_mail,true);
	if (result != YAD3_SUCCESS) {
		return result;
	}
	result = isMailInRealtor(yad3,customer_mail,false);
	if (result != YAD3_SUCCESS) {
		return result;
	}
	Offer offer = NULL;
	Customer customer = mapGet(yad3->customers_map,customer_mail);
	Realtor realtor = mapGet(yad3->realtors_map,realtor_mail);
	SET_FOREACH(Offer,iterator,yad3->offers_set)  {
		if(getOfferByMails(iterator, realtor_mail, customer_mail, &offer) == OFFER_SUCCESS)   {
			if (strcmp(choice,"accept") == 0) {
				//increase spentByFar for customer
				customerSetSpentByFar(customer,offerGetNewPrice(offer));
				//delete the apartment from the realtor's service
				Map map = realtorGetServices(realtor);
				ApartmentService service = mapGet(map, offerGetApartmentService(offer));
				serviceDeleteById(service, offerGetApartmentId(offer));
			}
			setRemove(yad3->offers_set,offer);  // remove the offer anyway
			return YAD3_SUCCESS;
		}
	}
	return YAD3_NOT_REQUESTED;
}

Yad3Result mostPayingCustomers(Yad3 yad3, FILE* output_channel, int count) {
	if (!yad3 || !output_channel) return YAD3_NULL_ARG;
	if (count <= 0) return YAD3_BAD_ARG;
	List list;
	MAP_TO_LIST(Customer,yad3->customers_map,list,listCustomerCopy,listCustomerDestroy);
	int key = 0;
	List filtered = listFilter(list, isPositive, &key);
	if (list) {
		listDestroy(list);
	}
	ListResult list_result = listSort(filtered, compareCustomer);
	Yad3Result result = ListResultToYad3Result (list_result);
	if (result != YAD3_SUCCESS) {
		return result;
	}
	int list_size = listGetSize(filtered);
	if (list_size == 0) {
		if (filtered) {
			listDestroy(filtered);
		return YAD3_SUCCESS;
		}
	}
	Customer iterator = listGetFirst(filtered);
	for (int i=0 ; i<((count<=list_size)?count:list_size) ; i++)   {
		mtmPrintCustomer(output_channel, customerGetMail(iterator), customerGetSpentByFar(iterator));
		iterator = listGetNext(filtered);
	}
	if (filtered) {
		listDestroy(filtered);
	}
	return YAD3_SUCCESS;
}
