#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "test_utilities.h"
#include "../offer.h"

static bool offerCreate_test()	{
	Offer offer = NULL;
	char* costumer_email = "customer@spendingmoney.gr";
	char* realtor_email = "realtor@realestate.gr";
	char* apartment_service = "AptsInAmuka";
	int apartment_id = 0;
	int new_price = 100000;
	OfferResult error;
	
	offer = offerCreate("@customer@spendingmoney.gr", realtor_email, apartment_service, apartment_id, new_price, &error);
	ASSERT_TEST(offer == NULL);
	ASSERT_TEST(error == OFFER_BAD_ARG);
	offer = offerCreate(costumer_email, "realtorrealestate.gr", apartment_service, apartment_id, new_price, &error);
	ASSERT_TEST(offer == NULL);
	ASSERT_TEST(error == OFFER_BAD_ARG);
	offer = offerCreate(costumer_email, "realtorrealestate.gr@@", apartment_service, apartment_id, new_price, &error);
	ASSERT_TEST(offer == NULL);
	ASSERT_TEST(error == OFFER_BAD_ARG);
	offer = offerCreate(costumer_email, "", apartment_service, apartment_id, new_price, &error);
	ASSERT_TEST(offer == NULL);
	ASSERT_TEST(error == OFFER_BAD_ARG);
	offer = offerCreate(NULL, realtor_email, apartment_service, apartment_id, new_price, &error);
	ASSERT_TEST(offer == NULL);
	ASSERT_TEST(error == OFFER_NULL_ARG);
	offer = offerCreate(costumer_email, realtor_email, "", apartment_id, new_price, &error);
	ASSERT_TEST(offer == NULL);	
	ASSERT_TEST(error == OFFER_BAD_ARG);
	offer = offerCreate(costumer_email, realtor_email, NULL, apartment_id, new_price, &error);
	ASSERT_TEST(offer == NULL);
	ASSERT_TEST(error == OFFER_NULL_ARG);
	offer = offerCreate(costumer_email, realtor_email, apartment_service, -1, new_price, &error);
	ASSERT_TEST(offer == NULL);
	ASSERT_TEST(error == OFFER_BAD_ARG);
	offer = offerCreate(costumer_email, realtor_email, apartment_service, apartment_id, 0, &error);
	ASSERT_TEST(offer == NULL);	
	ASSERT_TEST(error == OFFER_BAD_ARG);
	
	offer = offerCreate(costumer_email, realtor_email, apartment_service, apartment_id, new_price, &error);
	ASSERT_TEST(error == OFFER_SUCCESS);
	ASSERT_TEST(offer != NULL);
	ASSERT_TEST(strcmp(offerGetCustomerMail(offer), costumer_email) == 0);
	ASSERT_TEST(strcmp(offerGetRealtorMail(offer), realtor_email) == 0);
	ASSERT_TEST(strcmp(offerGetApartmentService(offer), apartment_service) == 0);
	ASSERT_TEST(offerGetApartmentId(offer) == apartment_id);
	ASSERT_TEST(offerGetNewPrice(offer) == new_price);
	offerDestroy(offer);

	return true;
}

static bool offerDestroy_test()	{
	
	Offer offer = NULL;
	offerDestroy(offer);
	OfferResult error;

	offer = offerCreate("customer@spendingmoney.gr", "realtor@realestate.gr", "AptsInAmuka", 0, 100000, &error);
	
	ASSERT_TEST(offer != NULL);
	offerDestroy(offer);
	
	return true;
}


static bool offerGetCustomerMail_test()	{
	
	OfferResult error;
	Offer offer = offerCreate("customer@spendingmoney.gr", "realtor@realestate.gr", "AptsInAmuka", 0, 100000, &error);
	ASSERT_TEST(strcmp(offerGetCustomerMail(offer),"customer@spendingmoney.gr") == 0);
	offerDestroy(offer);
	
	return true;
}

static bool offerGetRealtorMail_test()	{
	
	OfferResult error;
	Offer offer = offerCreate("customer@spendingmoney.gr", "realtor@realestate.gr", "AptsInAmuka", 0, 100000, &error);
	ASSERT_TEST(strcmp(offerGetRealtorMail(offer),"realtor@realestate.gr") == 0);
	offerDestroy(offer);
	
	return true;
}

static bool offerGetApartmentService_test()	{
	OfferResult error;	
	Offer offer = offerCreate("customer@spendingmoney.gr", "realtor@realestate.gr", "AptsInAmuka", 0, 100000, &error);
	ASSERT_TEST(strcmp(offerGetApartmentService(offer),"AptsInAmuka") == 0);
	offerDestroy(offer);
	
	return true;
}

static bool offerGetApartmentId_test()	{
	
	OfferResult error;	
	Offer offer = offerCreate("customer@spendingmoney.gr", "realtor@realestate.gr", "AptsInAmuka", 0, 100000, &error);
	ASSERT_TEST(offerGetApartmentId(offer) == 0);
	ASSERT_TEST(offerGetApartmentId(offer) != 100000);
	offerDestroy(offer);
	
	return true;
}

static bool offerGetNewPrice_test()	{

	OfferResult error;
	Offer offer = offerCreate("customer@spendingmoney.gr", "realtor@realestate.gr", "AptsInAmuka", 0, 100000, &error);
	ASSERT_TEST(offerGetNewPrice(offer) != 0);
	ASSERT_TEST(offerGetNewPrice(offer) == 100000);
	offerDestroy(offer);
	
	return true;
}

static bool offerSetCostumerMail_test()	{

	OfferResult error;
	ASSERT_TEST(offerSetCostumerMail(NULL, "awesomeCustomer@yad3.com") == OFFER_NULL_ARG);

	Offer offer = offerCreate("customer@spendingmoney.gr", "realtor@realestate.gr", "AptsInAmuka", 0, 100000, &error);
	ASSERT_TEST(strcmp(offerGetCustomerMail(offer), "customer@spendingmoney.gr") == 0);

	ASSERT_TEST(offerSetCostumerMail(offer, "@customer@spendingmoney.gr") == OFFER_BAD_ARG);
	ASSERT_TEST(offerSetCostumerMail(offer, "customer@spendingmoney.gr@") == OFFER_BAD_ARG);
	ASSERT_TEST(offerSetCostumerMail(offer, "customer@@spendingmoney.gr") == OFFER_BAD_ARG);
	ASSERT_TEST(offerSetCostumerMail(offer, "customerspendingmoney.gr") == OFFER_BAD_ARG);

	ASSERT_TEST(strcmp(offerGetCustomerMail(offer), "customer@spendingmoney.gr") == 0);

	ASSERT_TEST(offerSetCostumerMail(offer, "great-customer@spendingmoney.gr") == OFFER_SUCCESS);
	ASSERT_TEST(strcmp(offerGetCustomerMail(offer), "great-customer@spendingmoney.gr") == 0);

	offerDestroy(offer);

	return true;
}

static bool offerSetRealtorMail_test()	{
	
	OfferResult error;
	ASSERT_TEST(offerSetRealtorMail(NULL, "awesomeRealtor@yad3.com") == OFFER_NULL_ARG);

	Offer offer = offerCreate("customer@spendingmoney.gr", "realtor@realestate.gr", "AptsInAmuka", 0, 100000, &error);
	ASSERT_TEST(strcmp(offerGetRealtorMail(offer), "realtor@realestate.gr") == 0);

	ASSERT_TEST(offerSetRealtorMail(offer, "realtor@rea@lestate.gr") == OFFER_BAD_ARG);
	ASSERT_TEST(offerSetRealtorMail(offer, "@@realtorrealestate.gr") == OFFER_BAD_ARG);
	ASSERT_TEST(offerSetRealtorMail(offer, "realtor@realestate.gr@") == OFFER_BAD_ARG);
	ASSERT_TEST(offerSetRealtorMail(offer, "realtorrealestate.gr") == OFFER_BAD_ARG);

	ASSERT_TEST(strcmp(offerGetRealtorMail(offer), "realtor@realestate.gr") == 0);

	ASSERT_TEST(offerSetRealtorMail(offer, "@realtor-is-real.realestate.gr") == OFFER_SUCCESS);
	ASSERT_TEST(strcmp(offerGetRealtorMail(offer), "@realtor-is-real.realestate.gr") == 0);

	offerDestroy(offer);

	return true;
}

static bool getOfferByMails_test()	{
	
	OfferResult error;
	ASSERT_TEST(offerSetRealtorMail(NULL, "awesomeRealtor@yad3.com") == OFFER_NULL_ARG);
	Offer outOffer;
	
	Offer offer = offerCreate("customer@spendingmoney.gr", "realtor@realestate.gr", "AptsInAmuka", 0, 100000, &error);

	ASSERT_TEST(getOfferByMails(offer, "realtor@realestate.gr", "customer@spendingmoney.ggr", &outOffer) == OFFER_NOT_FOUND);
	ASSERT_TEST(getOfferByMails(offer, "realtor@4real.gr", "customer@spendingmoney.gr", &outOffer) == OFFER_NOT_FOUND);


	ASSERT_TEST(getOfferByMails(offer, "realtor@realestate.gr", "customer@spendingmoney.gr", &outOffer) == OFFER_SUCCESS);
	ASSERT_TEST(outOffer!=NULL);
	
	ASSERT_TEST(strcmp(offerGetRealtorMail(outOffer), "realtor@realestate.gr") == 0);
	ASSERT_TEST(strcmp(offerGetCustomerMail(outOffer), "customer@spendingmoney.gr") == 0);
	ASSERT_TEST(strcmp(offerGetApartmentService(outOffer), "AptsInAmuka") == 0);
	ASSERT_TEST(offerGetApartmentId(outOffer) == 0);
	ASSERT_TEST(offerGetNewPrice(outOffer) == 100000);

	offerDestroy(offer);

	return true;
}
// static bool offerCompare_test()	{
	
// 	OfferResult error;
// 	Offer offer1 = NULL;
// 	Offer offer2 = NULL;

// 	ASSERT_TEST(offerCompare(offer1, offer2) == 0);
	
// 	offer1 = offerCreate("customer@spendingmoney.gr", "realtor@realestate.gr", "AptsInAmuka", 0, 100000, &error);
// 	ASSERT_TEST(offerCompare(offer1, offer2) == 1);
	
// 	offerDestroy(offer1);
// 	offer1 = NULL;
	
// 	offer2 = offerCreate("customer@spendingmoney.gr", "realtor@realestate.gr", "AptsInAmuka", 0, 100000, &error);
// 	ASSERT_TEST(offerCompare(offer1, offer2) == -1);
	
// 	offer1 = offerCreate("customer@spendingmoney.gr", "realtor@realestate.gr", "AptsInAmuka", 0, 100000, &error);
// 	ASSERT_TEST(offerCompare(offer1, offer2) == 0);
	
// 	ASSERT_TEST(offerSetCostumerMail(offer1, "great-customer@spendingmoney.gr") == OFFER_SUCCESS);
// 	ASSERT_TEST(offerCompare(offer1, offer2) == 1);
// 	ASSERT_TEST(offerSetCostumerMail(offer2, "greater-customer@spendingmoney.gr") == OFFER_SUCCESS);
// 	ASSERT_TEST(offerCompare(offer1, offer2) == 1);
	
// 	offerDestroy(offer1);
// 	offerDestroy(offer2);

// 	return true;
// }

static bool offerCopy_test()	{
	
	Offer offer = NULL;
	Offer copy = NULL;
	OfferResult error;
	
	ASSERT_TEST(offerCopy(offer,&error) == NULL);
	ASSERT_TEST(error == OFFER_NULL_ARG);
	
	offer = offerCreate("customer@spendingmoney.gr", "realtor@realestate.gr", "AptsInAmuka", 0, 100000, &error);
	copy = offerCopy(offer,&error);
	ASSERT_TEST(error == OFFER_SUCCESS);
	ASSERT_TEST(strcmp(offerGetCustomerMail(offer), offerGetCustomerMail(copy)) == 0);
	ASSERT_TEST(strcmp(offerGetRealtorMail(offer), offerGetRealtorMail(copy)) == 0);
	ASSERT_TEST(strcmp(offerGetApartmentService(offer), offerGetApartmentService(copy)) == 0);
	ASSERT_TEST(offerGetApartmentId(offer) == offerGetApartmentId(copy));
	ASSERT_TEST(offerGetNewPrice(offer) == offerGetNewPrice(copy));
	
	offerDestroy(offer);
	offerDestroy(copy);

	return true;
}

int main() {
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	RUN_TEST(offerCreate_test);
	RUN_TEST(offerDestroy_test);
	RUN_TEST(offerGetApartmentId_test);
	RUN_TEST(offerGetNewPrice_test);
	RUN_TEST(offerGetCustomerMail_test);
	RUN_TEST(offerGetRealtorMail_test);
	RUN_TEST(offerGetApartmentService_test);
	RUN_TEST(offerSetCostumerMail_test);
	RUN_TEST(offerSetRealtorMail_test);
	// RUN_TEST(offerCompare_test);
	RUN_TEST(getOfferByMails_test);	
	RUN_TEST(offerCopy_test);
	return 0;
}

