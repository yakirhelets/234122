#include "test_utilities.h"
#include "../yad3.h"

static Yad3 setUp() {
	Yad3Result error;
	Yad3 yad3 = yad3Create(&error);
	error = addRealtor(yad3, "glorious@realtor.com", "company_name", 100);
	error = addRealtor(yad3, "magnificent@realtor.com", "company_name", 50);
	error = addRealtor(yad3, "noservice@realtor.com", "easy_company", 33);

	//realtor 1 services + apartments
	error = addApartmentService(yad3,"glorious@realtor.com","service1",3);
	error = addApartmentService(yad3,"glorious@realtor.com","service2",1);
	error = addApartmentService(yad3,"glorious@realtor.com","service3",2);

	error = realtorAddApartment(yad3, "glorious@realtor.com", "service1", 0, 100000, 2, 2, "eeee");
	error = realtorAddApartment(yad3, "glorious@realtor.com", "service1", 1, 1000000, 3, 2, "eeeeww");
	error = realtorAddApartment(yad3, "glorious@realtor.com", "service1", 7, 10000000, 3, 3, "ewewewewe");

	error = realtorAddApartment(yad3, "glorious@realtor.com", "service2", 7, 10000000, 1, 1, "w");

	//realtor 2 services + apartments
	error = addApartmentService(yad3, "magnificent@realtor.com","the_service",3);

	error = realtorAddApartment(yad3, "magnificent@realtor.com", "the_service", 0, 10000000, 3, 3, "eeewwweee");

	//customers
	error = addCustomer(yad3, "walter.white@gmail.com", 2, 1, 110000);
	error = addCustomer(yad3, "jesse.pinkman@gmail.com", 3, 2, 909999999);
	error = addCustomer(yad3, "hector.salamanca@gmail.com", 5, 5, 2000);
	error = addCustomer(yad3, "more.area@gmail.com", 7, 1, 20000000);
	error = addCustomer(yad3, "more.rooms@gmail.com", 5, 3, 200000000);
	error = addCustomer(yad3, "poor.customer@gmail.com", 5, 5, 2000);

	return yad3;
}

//static void tearDown(Yad3 examples) {
//	yad3Destroy(examples);
//}

static bool yad3Create_test()	{
	Yad3Result error;
	Yad3 yad3 = yad3Create(&error);
	ASSERT_TEST(error == YAD3_SUCCESS);
	yad3Destroy(yad3);
	return true;
}

static bool destroyYad3_test()	{
	Yad3Result error;
	Yad3 yad3 = yad3Create(&error);
	ASSERT_TEST(error == YAD3_SUCCESS);
	yad3Destroy(yad3);
	return true;
}

static bool addRealtor_test() {
	Yad3Result error;
	Yad3 yad3 = yad3Create(&error);
	ASSERT_TEST(error == YAD3_SUCCESS);
	error = addRealtor(NULL, "das@mail.com", "great_company_name", 100);
	ASSERT_TEST(error == YAD3_NULL_ARG);
	error = addRealtor(yad3, NULL, "great_company_name", 100);
	ASSERT_TEST(error == YAD3_NULL_ARG);
	error = addRealtor(yad3, "das@mail.com", NULL, 100);
	ASSERT_TEST(error == YAD3_NULL_ARG);

	error = addRealtor(yad3, "", "great_company_name", 100);
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = addRealtor(yad3, "das@@mail.com", "great_company_name", 100);
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = addRealtor(yad3, "dasmail.com", "great_company_name", 100);
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = addRealtor(yad3, "das@mail.com", "", 100);
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = addRealtor(yad3, "das@mail.com", "great_company_name", 101);
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = addRealtor(yad3, "das@mail.com", "great_company_name", 0);
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = addRealtor(yad3, "das@mail.com", "great_company_name", -1);
	ASSERT_TEST(error == YAD3_BAD_ARG);

	error = addRealtor(yad3, "das@mail.com", "great_company_name", 50);
	ASSERT_TEST(error == YAD3_SUCCESS);
	error = addRealtor(yad3, "das@mail.com", "not_so_great_company_name", 100);
	ASSERT_TEST(error == YAD3_EMAIL_ALREADY_EXISTS);

	yad3Destroy(yad3);
	return true;
}

static bool addCustomer_test() {
	Yad3Result error;
	Yad3 yad3 = yad3Create(&error);
	ASSERT_TEST(error == YAD3_SUCCESS);
	error = addCustomer(NULL, "das@mail.com", 30, 3, 110000);
	ASSERT_TEST(error == YAD3_NULL_ARG);
	error = addCustomer(yad3, NULL, 30, 3, 110000);
	ASSERT_TEST(error == YAD3_NULL_ARG);

	error = addCustomer(yad3, "dasmail.com", 30, 3, 110000);
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = addCustomer(yad3, "", 30, 3, 110000);
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = addCustomer(yad3, "das@mail.@com", 30, 3, 110000);
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = addCustomer(yad3, "das@mail.com", 0, 3, 110000);
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = addCustomer(yad3, "das@mail.com", 30, 0, 110000);
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = addCustomer(yad3, "das@mail.com", 30, 3, 0);
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = addCustomer(yad3, "das@mail.com", -30, 3, 110000);
	ASSERT_TEST(error == YAD3_BAD_ARG);

	error = addCustomer(yad3, "das@mail.com", 30, 3, 110000);
	ASSERT_TEST(error == YAD3_SUCCESS);
	error = addCustomer(yad3, "das@mail.com", 20, 2, 1110000);
	ASSERT_TEST(error == YAD3_EMAIL_ALREADY_EXISTS);

	yad3Destroy(yad3);
	return true;
}

static bool removeRealtor_test() {
	Yad3Result error;
	Yad3 yad3 = yad3Create(&error);
	ASSERT_TEST(error == YAD3_SUCCESS);

	error = addRealtor(yad3, "das@mail.com", "great_company_name", 50);
	ASSERT_TEST(error == YAD3_SUCCESS);

	error = removeRealtor(yad3,"toRemove@rem.de");
	ASSERT_TEST(error == YAD3_EMAIL_DOES_NOT_EXIST);

	error = removeRealtor(yad3,NULL);
	ASSERT_TEST(error == YAD3_NULL_ARG);
	error = removeRealtor(NULL,"toRemove@rem.de");
	ASSERT_TEST(error == YAD3_NULL_ARG);


	error = addCustomer(yad3, "customer@aol.com", 30, 3, 125000);
	ASSERT_TEST(error == YAD3_SUCCESS);
	error = removeRealtor(yad3,"customer@aol.com");
	ASSERT_TEST(error == YAD3_EMAIL_WRONG_ACCOUNT_TYPE);

	error = removeRealtor(yad3,"dasmail.com");
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = removeRealtor(yad3,"das@mai@l.com");
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = removeRealtor(yad3,"");
	ASSERT_TEST(error == YAD3_BAD_ARG);

	error = removeRealtor(yad3,"das@mail.com");
	ASSERT_TEST(error == YAD3_SUCCESS);
	yad3Destroy(yad3);
	return true;
}

static bool removeCustomer_test() {
	Yad3Result error;
	Yad3 yad3 = yad3Create(&error);
	ASSERT_TEST(error == YAD3_SUCCESS);

	error = addCustomer(yad3, "hezi@yes.com", 30, 3, 125000);
	ASSERT_TEST(error == YAD3_SUCCESS);

	error = removeCustomer(yad3,"toRemove@rem.de");
	ASSERT_TEST(error == YAD3_EMAIL_DOES_NOT_EXIST);
	error = removeCustomer(yad3,NULL);
	ASSERT_TEST(error == YAD3_NULL_ARG);
	error = removeCustomer(NULL,"toRemove@rem.de");
	ASSERT_TEST(error == YAD3_NULL_ARG);

	error = addRealtor(yad3, "das@mail.com", "great_company_name", 50);
	ASSERT_TEST(error == YAD3_SUCCESS);
	error = removeCustomer(yad3,"das@mail.com");
	ASSERT_TEST(error == YAD3_EMAIL_WRONG_ACCOUNT_TYPE);

	error = removeCustomer(yad3,"dasmail.com");
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = removeCustomer(yad3,"das@mai@l.com");
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = removeCustomer(yad3,"");
	ASSERT_TEST(error == YAD3_BAD_ARG);

	error = removeCustomer(yad3,"hezi@yes.com");
	ASSERT_TEST(error == YAD3_SUCCESS);

	yad3Destroy(yad3);
	return true;
}

static bool realtorAddApartmentService_test() {
	Yad3Result error;
	Yad3 yad3 = yad3Create(&error);

	error = addApartmentService(yad3,"kookilida@gmail.com","service1",5);
	ASSERT_TEST(error == YAD3_EMAIL_DOES_NOT_EXIST);

	error = addRealtor(yad3,"kookilida@gmail.com","kookilida",15);
	ASSERT_TEST(error == YAD3_SUCCESS);

	error = addApartmentService(yad3,"realtor@gmail.com","service1",5);
	ASSERT_TEST(error == YAD3_EMAIL_DOES_NOT_EXIST);

	error = addApartmentService(yad3,"kookilida@gmail.com","service1",5);
	ASSERT_TEST(error == YAD3_SUCCESS);

	error = addApartmentService(yad3,"kookilida@gmail.co@m","service1",5);
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = addApartmentService(yad3,"kookilidagmail.com","service1",5);
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = addApartmentService(yad3,"kookilidagmail.com@","service1",0);
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = addApartmentService(yad3,"a@a.com","",5);
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = addApartmentService(yad3,"","service1",5);
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = addApartmentService(yad3,"a@@a.com","service1",-1);
	ASSERT_TEST(error == YAD3_BAD_ARG);

	error = addApartmentService(yad3,NULL,"service1",5);
	ASSERT_TEST(error == YAD3_NULL_ARG);
	error = addApartmentService(yad3,"a@a.com",NULL,5);
	ASSERT_TEST(error == YAD3_NULL_ARG);
	error = addApartmentService(NULL,"a@a.com","service1",5);
	ASSERT_TEST(error == YAD3_NULL_ARG);

	error = addApartmentService(yad3,"kookilida@gmail.com","service1",5);
	ASSERT_TEST(error == YAD3_APARTMENT_SERVICE_ALREADY_EXISTS);

	error = addCustomer(yad3, "fake@realtor.com", 30, 3, 125000);
	ASSERT_TEST(error == YAD3_SUCCESS);
	error = addApartmentService(yad3,"fake@realtor.com","i'm_not_a_realtor",5);
	ASSERT_TEST(error == YAD3_EMAIL_WRONG_ACCOUNT_TYPE);

	yad3Destroy(yad3);
	return true;
}

static bool realtorRemoveApartmentService_test() {
	Yad3Result error;
	Yad3 yad3 = yad3Create(&error);

	error = addRealtor(yad3,"kookilida@gmail.com","kookilida",15);
	ASSERT_TEST(error == YAD3_SUCCESS);

	error = addApartmentService(yad3,"kookilida@gmail.com","service1",5);
	ASSERT_TEST(error == YAD3_SUCCESS);

	error = removeApartmentService(yad3,NULL,"service1");
	ASSERT_TEST(error == YAD3_NULL_ARG);
	error = removeApartmentService(yad3,"a@a.com",NULL);
	ASSERT_TEST(error == YAD3_NULL_ARG);
	error = removeApartmentService(NULL,"a@a.com","service1");
	ASSERT_TEST(error == YAD3_NULL_ARG);

	error = removeApartmentService(yad3,"kookilida@gmail.co@m","service1");
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = removeApartmentService(yad3,"kookilidagmail.com","service1");
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = removeApartmentService(yad3,"a@a.com","");
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = removeApartmentService(yad3,"","service1");
	ASSERT_TEST(error == YAD3_BAD_ARG);

	error = addCustomer(yad3, "fake@realtor.com", 30, 3, 125000);
	ASSERT_TEST(error == YAD3_SUCCESS);
	error = removeApartmentService(yad3,"fake@realtor.com","i'm_not_a_realtor");
	ASSERT_TEST(error == YAD3_EMAIL_WRONG_ACCOUNT_TYPE);


	error = removeApartmentService(yad3,"kookilidag@mail.com","service1");
	ASSERT_TEST(error == YAD3_EMAIL_DOES_NOT_EXIST);

	error = removeApartmentService(yad3,"kookilida@gmail.com","service11");
	ASSERT_TEST(error == YAD3_APARTMENT_SERVICE_DOES_NOT_EXIST);

	error = removeApartmentService(yad3,"kookilida@gmail.com","service1");
	ASSERT_TEST(error == YAD3_SUCCESS);
	error = removeApartmentService(yad3,"kookilida@gmail.com","service1");
	ASSERT_TEST(error == YAD3_APARTMENT_SERVICE_DOES_NOT_EXIST);

	yad3Destroy(yad3);
	return true;
}

static bool realtorAddApartment_test() {
	Yad3Result error;
	Yad3 yad3 = yad3Create(&error);
	error = addRealtor(yad3, "glorious@realtor.com", "company_name", 100);
	ASSERT_TEST(error == YAD3_SUCCESS);
	error = addCustomer(yad3, "walter.white@gmail.com", 2, 1, 110000);
	ASSERT_TEST(error == YAD3_SUCCESS);

	error = addApartmentService(yad3,"glorious@realtor.com","service1",3);
	ASSERT_TEST(error == YAD3_SUCCESS);

	error = realtorAddApartment(yad3, "glorious@realtor.com", "service1", 0, 100000, 2, 2, "eeee");
	ASSERT_TEST(error == YAD3_SUCCESS);
	error = realtorAddApartment(yad3, "glorious@realtor.com", "service1", 1, 1000000, 3, 2, "eeeeww");
	ASSERT_TEST(error == YAD3_SUCCESS);

	error = realtorAddApartment(yad3, "glorious@realtor.com", "service1", 1, 1000000, 3, 2, "eeeeww");
	ASSERT_TEST(error == YAD3_APARTMENT_ALREADY_EXISTS);

	error = realtorAddApartment(yad3, "glorious@realtor.com", "service1", 7, 10000000, 3, 3, "ewewewewe");
	ASSERT_TEST(error == YAD3_SUCCESS);

	error = realtorAddApartment(yad3, "glorious@realtor.com", "service1", 7, 10000000, 3, 3, "ewewewewe");
	ASSERT_TEST(error == YAD3_APARTMENT_SERVICE_FULL);

	error = realtorAddApartment(yad3, "glorious@realtor.com", "service1", 8, 10000000, 3, 3, "ewewewewe");
	ASSERT_TEST(error == YAD3_APARTMENT_SERVICE_FULL);

	error = realtorAddApartment(NULL, "glorious@realtor.com", "service1", 7, 10000000, 3, 3, "ewewewewe");
	ASSERT_TEST(error == YAD3_NULL_ARG);
	error = realtorAddApartment(yad3, NULL, "service1", 7, 10000000, 3, 3, "ewewewewe");
	ASSERT_TEST(error == YAD3_NULL_ARG);
	error = realtorAddApartment(yad3, "glorious@realtor.com", NULL, 7, 10000000, 3, 3, "ewewewewe");
	ASSERT_TEST(error == YAD3_NULL_ARG);
	error = realtorAddApartment(yad3, "glorious@realtor.com", "service1", 7, 10000000, 3, 3, NULL);
	ASSERT_TEST(error == YAD3_NULL_ARG);

	//should return BAD_ARG, not ALREADY_EXISTS
	error = realtorAddApartment(yad3, "gloriousrealtor.com", "service1", 7, 10000000, 3, 3, "ewewewewe");
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = realtorAddApartment(yad3, "glorious@@realtor.com", "service1", 7, 10000000, 3, 3, "ewewewewe");
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = realtorAddApartment(yad3, "", "service1", 7, 10000000, 3, 3, "ewewewewe");
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = realtorAddApartment(yad3, "glorious@realtor.com", "", 7, 10000000, 3, 3, "ewewewewe");
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = realtorAddApartment(yad3, "glorious@realtor.com", "service1", 7, 10000000, 3, 3, "");
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = realtorAddApartment(yad3, "glorious@realtor.com", "service1", 7, 10000000, 3, 3, "ewnananananana");
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = realtorAddApartment(yad3, "glorious@realtor.com", "service1", 7, 10000000, 3, 3, "ewewewjewe");
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = realtorAddApartment(yad3, "glorious@realtor.com", "service1", 7, 10000000, 0, 3, "ewewewewe");
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = realtorAddApartment(yad3, "glorious@realtor.com", "service1", 7, 10000000, 3, 0, "ewewewewe");
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = realtorAddApartment(yad3, "glorious@realtor.com", "service1", 7, 10000000, 3, -1, "ewewewewe");
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = realtorAddApartment(yad3, "glorious@realtor.com", "service1", 7, 10000010, 3, 3, "ewewewewe");
	ASSERT_TEST(error == YAD3_BAD_ARG);

	error = realtorAddApartment(yad3, "gloriouss@realtor.com", "service1", 7, 10000000, 3, 3, "ewewewewe");
	ASSERT_TEST(error == YAD3_EMAIL_DOES_NOT_EXIST);

	error = realtorAddApartment(yad3, "glorious@realtor.com", "service12", 7, 10000000, 3, 3, "ewewewewe");
	ASSERT_TEST(error == YAD3_APARTMENT_SERVICE_DOES_NOT_EXIST);

	error = realtorAddApartment(yad3, "walter.white@gmail.com", "service1", 7, 10000000, 3, 3, "ewewewewe");
	ASSERT_TEST(error == YAD3_EMAIL_WRONG_ACCOUNT_TYPE);

	yad3Destroy(yad3);
	return true;
}

static bool realtorRemoveApartment_test() {
	Yad3Result error;
	SET_UP(Yad3);

	error = realtorRemoveApartment(NULL, "glorious@realtor.com", "service1", 0);
	ASSERT_TEST(error == YAD3_NULL_ARG);
	error = realtorRemoveApartment(examples, NULL, "service1", 0);
	ASSERT_TEST(error == YAD3_NULL_ARG);
	error = realtorRemoveApartment(examples, "glorious@realtor.com", NULL, 0);
	ASSERT_TEST(error == YAD3_NULL_ARG);

	error = realtorRemoveApartment(examples, "gloriousrealtor.com", "service1", 0);
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = realtorRemoveApartment(examples, "@glorious@realtor.com", "service1", 0);
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = realtorRemoveApartment(examples, "", "service1", 0);
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = realtorRemoveApartment(examples, "glorious@realtor.com", "", 0);
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = realtorRemoveApartment(examples, "glorious@realtor.com", "service1", -1);
	ASSERT_TEST(error == YAD3_BAD_ARG);

	error = realtorRemoveApartment(examples, "glorioussss@realtor.com", "service1", 0);
	ASSERT_TEST(error == YAD3_EMAIL_DOES_NOT_EXIST);

	error = realtorRemoveApartment(examples, "jesse.pinkman@gmail.com", "service1", 0);
	ASSERT_TEST(error == YAD3_EMAIL_WRONG_ACCOUNT_TYPE);

	error = realtorRemoveApartment(examples, "glorious@realtor.com", "service4", 0);
	ASSERT_TEST(error == YAD3_APARTMENT_SERVICE_DOES_NOT_EXIST);

	error = realtorRemoveApartment(examples, "glorious@realtor.com", "service1", 0);
	ASSERT_TEST(error == YAD3_SUCCESS);
	error = realtorRemoveApartment(examples, "glorious@realtor.com", "service1", 0);
	ASSERT_TEST(error == YAD3_APARTMENT_DOES_NOT_EXIST);

	error = realtorRemoveApartment(examples, "magnificent@realtor.com", "the_service", 0);
	ASSERT_TEST(error == YAD3_SUCCESS);
	error = realtorRemoveApartment(examples, "magnificent@realtor.com", "the_service", 0);
	ASSERT_TEST(error == YAD3_APARTMENT_DOES_NOT_EXIST);

	error = realtorRemoveApartment(examples, "magnificent@realtor.com", "the_service", 270);
	ASSERT_TEST(error == YAD3_APARTMENT_DOES_NOT_EXIST);

	yad3Destroy(examples);

	return true;
}

static bool customerMakeOffer_test() {
	Yad3Result error;
	SET_UP(Yad3);

	error = customerMakeOffer(NULL, "glorious@realtor.com", "jesse.pinkman@gmail.com", "service1", 7, 1000);
	ASSERT_TEST(error == YAD3_NULL_ARG);
	error = customerMakeOffer(examples, NULL, "jesse.pinkman@gmail.com", "service1", 7, 1000);
	ASSERT_TEST(error == YAD3_NULL_ARG);
	error = customerMakeOffer(examples, "glorious@realtor.com", NULL, "service1", 7, 1000);
	ASSERT_TEST(error == YAD3_NULL_ARG);
	error = customerMakeOffer(examples, "glorious@realtor.com", "jesse.pinkman@gmail.com", NULL, 7, 1000);
	ASSERT_TEST(error == YAD3_NULL_ARG);

	error = customerMakeOffer(examples, "g@lorious@realtor.com", "jesse.pinkman@gmail.com", "service1", 7, 1000);
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = customerMakeOffer(examples, "gloriousrealtor.com", "jesse.pinkman@gmail.com", "service1", 7, 1000);
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = customerMakeOffer(examples, "glorious@realtor.com", "jesse@.pinkman@gmail.com", "service1", 7, 1000);
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = customerMakeOffer(examples, "glorious@realtor.com", "jesse.pinkmangmail.com", "service1", 7, 1000);
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = customerMakeOffer(examples, "", "jesse.pinkman@gmail.com", "service1", 7, 1000);
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = customerMakeOffer(examples, "glorious@realtor.com", "", "service1", 7, 1000);
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = customerMakeOffer(examples, "glorious@realtor.com", "jesse.pinkman@gmail.com", "", 7, 1000);
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = customerMakeOffer(examples, "glorious@realtor.com", "jesse.pinkman@gmail.com", "service1", -7, 1000);
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = customerMakeOffer(examples, "glorious@realtor.com", "jesse.pinkman@gmail.com", "service1", 7, -1000);
	ASSERT_TEST(error == YAD3_BAD_ARG);

	error = customerMakeOffer(examples, "jesse.pinkman@gmail.com", "gloriousrealt@or.com", "service1", 7, 1000);
	ASSERT_TEST(error == YAD3_EMAIL_DOES_NOT_EXIST);
	error = customerMakeOffer(examples, "glorious@realtor.com", "jese.@pinkmangmail.com", "service1", 7, 1000);
	ASSERT_TEST(error == YAD3_EMAIL_DOES_NOT_EXIST);

	error = customerMakeOffer(examples, "magnificent@realtor.com", "glorious@realtor.com", "service1", 7, 1000);
	ASSERT_TEST(error == YAD3_EMAIL_WRONG_ACCOUNT_TYPE);
	error = customerMakeOffer(examples, "jesse.pinkman@gmail.com", "walter.white@gmail.com", "service1", 7, 1000);
	ASSERT_TEST(error == YAD3_EMAIL_WRONG_ACCOUNT_TYPE);

	error = customerMakeOffer(examples, "jesse.pinkman@gmail.com", "glorious@realtor.com", "service567", 7, 1000);
	ASSERT_TEST(error == YAD3_APARTMENT_SERVICE_DOES_NOT_EXIST);

	error = customerMakeOffer(examples, "jesse.pinkman@gmail.com", "glorious@realtor.com", "service1", 70, 1000);
	ASSERT_TEST(error == YAD3_APARTMENT_DOES_NOT_EXIST);

	error = customerMakeOffer(examples, "more.area@gmail.com", "magnificent@realtor.com", "the_service", 0, 1000);
	ASSERT_TEST(error == YAD3_REQUEST_WRONG_PROPERTIES);
	error = customerMakeOffer(examples, "more.rooms@gmail.com", "magnificent@realtor.com", "the_service", 0, 1000);
	ASSERT_TEST(error == YAD3_REQUEST_WRONG_PROPERTIES);
	error = customerMakeOffer(examples, "poor.customer@gmail.com", "magnificent@realtor.com", "the_service", 0, 1000);
	ASSERT_TEST(error == YAD3_REQUEST_WRONG_PROPERTIES);

	error = customerMakeOffer(examples, "jesse.pinkman@gmail.com", "glorious@realtor.com", "service1", 7, 90999999);
	ASSERT_TEST(error == YAD3_REQUEST_ILLOGICAL_PRICE);

	error = customerMakeOffer(examples, "jesse.pinkman@gmail.com", "glorious@realtor.com", "service1", 7, 1000);
	ASSERT_TEST(error == YAD3_SUCCESS);

	error = customerMakeOffer(examples, "jesse.pinkman@gmail.com", "glorious@realtor.com", "service1", 7, 1000);
	ASSERT_TEST(error == YAD3_ALREADY_REQUESTED);


	yad3Destroy(examples);

	return true;
}

static bool realtorRespondToOffer_test() {
	Yad3Result error;
	SET_UP(Yad3);

	error = customerMakeOffer(examples, "jesse.pinkman@gmail.com", "glorious@realtor.com", "service1", 7, 1000);
	ASSERT_TEST(error == YAD3_SUCCESS);

	error = realtorRespondToOffer(NULL, "jesse.pinkman@gmail.com", "glorious@realtor.com", "accept");
	ASSERT_TEST(error == YAD3_NULL_ARG);
	error = realtorRespondToOffer(examples, "jesse.pinkman@gmail.com", NULL, "accept");
	ASSERT_TEST(error == YAD3_NULL_ARG);
	error = realtorRespondToOffer(examples, NULL, "glorious@realtor.com", "accept");
	ASSERT_TEST(error == YAD3_NULL_ARG);
	error = realtorRespondToOffer(examples, "jesse.pinkman@gmail.com", "glorious@realtor.com", NULL);
	ASSERT_TEST(error == YAD3_NULL_ARG);

	error = realtorRespondToOffer(examples, "glorious@@realtor.com", "jesse.pinkman@gmail.com", "accept");
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = realtorRespondToOffer(examples, "gloriousrealtor.com", "jesse.pinkman@gmail.com", "accept");
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = realtorRespondToOffer(examples, "", "jesse.pinkman@gmail.com", "accept");
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = realtorRespondToOffer(examples, "glorious@realtor.com", "jes@se.pinkman@gmail.com", "accept");
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = realtorRespondToOffer(examples, "glorious@realtor.com", "jesse.pinkmangmail.com", "accept");
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = realtorRespondToOffer(examples, "glorious@realtor.com", "", "accept");
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = realtorRespondToOffer(examples, "glorious@realtor.com", "jesse.pinkman@gmail.com", "");
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = realtorRespondToOffer(examples, "glorious@realtor.com", "jesse.pinkman@gmail.com", "blablanotaccept");
	ASSERT_TEST(error == YAD3_BAD_ARG);

	error = realtorRespondToOffer(examples, "jesse.pinkman@gmail.com", "jesse.pinkman@gmail.com", "accept");
	ASSERT_TEST(error == YAD3_EMAIL_WRONG_ACCOUNT_TYPE);
	error = realtorRespondToOffer(examples, "glorious@realtor.com", "glorious@realtor.com", "accept");
	ASSERT_TEST(error == YAD3_EMAIL_WRONG_ACCOUNT_TYPE);
	error = realtorRespondToOffer(examples, "jesse.pinkman@gmail.com", "glorious@realtor.com", "decline");
	ASSERT_TEST(error == YAD3_EMAIL_WRONG_ACCOUNT_TYPE);


	error = realtorRespondToOffer(examples, "glorious@realtorz.com", "jesse.pinkman@gmail.com", "accept");
	ASSERT_TEST(error == YAD3_EMAIL_DOES_NOT_EXIST);
	error = realtorRespondToOffer(examples, "glorious@realtor.com", "jesse.pinkmans@gmail.com", "accept");
	ASSERT_TEST(error == YAD3_EMAIL_DOES_NOT_EXIST);

	error = realtorRespondToOffer(examples, "glorious@realtor.com", "walter.white@gmail.com", "accept");
	ASSERT_TEST(error == YAD3_NOT_REQUESTED);

	error = realtorRespondToOffer(examples, "glorious@realtor.com", "jesse.pinkman@gmail.com", "decline");
	ASSERT_TEST(error == YAD3_SUCCESS);
	error = realtorRemoveApartment(examples, "glorious@realtor.com", "the_service", 0);
	ASSERT_TEST(error == YAD3_APARTMENT_SERVICE_DOES_NOT_EXIST);

	error = customerMakeOffer(examples, "jesse.pinkman@gmail.com", "glorious@realtor.com", "service1", 7, 1000);
	ASSERT_TEST(error == YAD3_SUCCESS);

	error = realtorRespondToOffer(examples, "glorious@realtor.com", "jesse.pinkman@gmail.com", "accept");
	ASSERT_TEST(error == YAD3_SUCCESS);
	error = realtorRemoveApartment(examples, "glorious@realtor.com", "the_service", 0);
	ASSERT_TEST(error == YAD3_APARTMENT_SERVICE_DOES_NOT_EXIST);
	// ASSERT_TEST(customerGetSpentByFar("jesse.pinkman@gmail.com")==1000);

	yad3Destroy(examples);

	return true;
}

static bool customerPurchase_test() {
	Yad3Result error;
	SET_UP(Yad3);

	error = customerPurchase(NULL , "jesse.pinkman@gmail.com", "glorious@realtor.com", "service1", 1);
	ASSERT_TEST(error == YAD3_NULL_ARG);
	error = customerPurchase(examples , NULL, "glorious@realtor.com", "service1", 1);
	ASSERT_TEST(error == YAD3_NULL_ARG);
	error = customerPurchase(examples , "jesse.pinkman@gmail.com", NULL, "service1", 1);
	ASSERT_TEST(error == YAD3_NULL_ARG);
	error = customerPurchase(examples , "jesse.pinkman@gmail.com", "glorious@realtor.com", NULL, 1);
	ASSERT_TEST(error == YAD3_NULL_ARG);

	error = customerPurchase(examples , "jesse.@pinkman@gmail.com", "glorious@realtor.com", "service1", 1);
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = customerPurchase(examples , "jesse.pinkmangmail.com", "glorious@realtor.com", "service1", 1);
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = customerPurchase(examples , "", "glorious@realtor.com", "service1", 1);
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = customerPurchase(examples , "jesse.pinkman@gmail.com", "", "service1", 1);
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = customerPurchase(examples , "jesse.pinkman@gmail.com", "gloriousrealtor.com", "service1", 1);
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = customerPurchase(examples , "jesse.pinkman@gmail.com", "glorious@realtor.com@", "service1", 1);
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = customerPurchase(examples , "jesse.pinkman@gmail.com", "glorious@realtor.com", "", 1);
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = customerPurchase(examples , "jesse.pinkman@gm@ail.com", "glorious@realtor.com", "service1", 0);
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = customerPurchase(examples , "jesse.pinkman@gmail.com", "glorious@realtor.com", "service1", -1);
	ASSERT_TEST(error == YAD3_BAD_ARG);

	error = customerPurchase(examples , "jesse.pinkman@gmail.com", "jesse.pinkman@gmail.com", "service1", 1);
	ASSERT_TEST(error == YAD3_EMAIL_WRONG_ACCOUNT_TYPE);
	error = customerPurchase(examples , "glorious@realtor.com", "glorious@realtor.com", "service1", 1);
	ASSERT_TEST(error == YAD3_EMAIL_WRONG_ACCOUNT_TYPE);
	error = customerPurchase(examples , "glorious@realtor.com", "jesse.pinkman@gmail.com", "service1", 1);
	ASSERT_TEST(error == YAD3_EMAIL_WRONG_ACCOUNT_TYPE);

	error = customerPurchase(examples , "jesse.pinkman@gmail.co.il", "glorious@realtor.com", "service1", 1);
	ASSERT_TEST(error == YAD3_EMAIL_DOES_NOT_EXIST);
	error = customerPurchase(examples , "jesse.pinkman@gmail.com", "glorious@realtor.co.il", "service1", 1);
	ASSERT_TEST(error == YAD3_EMAIL_DOES_NOT_EXIST);

	error = customerPurchase(examples , "jesse.pinkman@gmail.com", "glorious@realtor.com", "service6", 1);
	ASSERT_TEST(error == YAD3_APARTMENT_SERVICE_DOES_NOT_EXIST);

	error = customerPurchase(examples , "jesse.pinkman@gmail.com", "glorious@realtor.com", "service1", 11);
	ASSERT_TEST(error == YAD3_APARTMENT_DOES_NOT_EXIST);

	error = customerPurchase(examples, "more.area@gmail.com", "magnificent@realtor.com", "the_service", 0);
	ASSERT_TEST(error == YAD3_PURCHASE_WRONG_PROPERTIES);
	error = customerPurchase(examples, "more.rooms@gmail.com", "magnificent@realtor.com", "the_service", 0);
	ASSERT_TEST(error == YAD3_PURCHASE_WRONG_PROPERTIES);
	error = customerPurchase(examples, "poor.customer@gmail.com", "magnificent@realtor.com", "the_service", 0);
	ASSERT_TEST(error == YAD3_PURCHASE_WRONG_PROPERTIES);

	error = customerPurchase(examples , "jesse.pinkman@gmail.com", "magnificent@realtor.com", "the_service", 0);
	ASSERT_TEST(error == YAD3_SUCCESS);

	error = customerPurchase(examples , "jesse.pinkman@gmail.com", "magnificent@realtor.com", "the_service", 0);
	ASSERT_TEST(error == YAD3_APARTMENT_DOES_NOT_EXIST);


	yad3Destroy(examples);

	return true;
}

static bool mostPayingCustomers_test() {
	Yad3Result error;
	SET_UP(Yad3);

	error = mostPayingCustomers(examples, stdout, 0);
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = mostPayingCustomers(examples, stdout, -1);
	ASSERT_TEST(error == YAD3_BAD_ARG);

	error = mostPayingCustomers(examples, stdout, 2);
	ASSERT_TEST(error == YAD3_SUCCESS);
	
	error = customerPurchase(examples , "jesse.pinkman@gmail.com", "magnificent@realtor.com", "the_service", 0);
	ASSERT_TEST(error == YAD3_SUCCESS);
	
	error = mostPayingCustomers(examples, stdout, 2);
	ASSERT_TEST(error == YAD3_SUCCESS);

	yad3Destroy(examples);

	return true;
}

static bool reportRelevantRealtors_test() {
	Yad3Result error;
	SET_UP(Yad3);

	error = reportRelevantRealtors(NULL, stdout, "jesse.pinkman@gmail.com");
	ASSERT_TEST(error == YAD3_NULL_ARG);
	error = reportRelevantRealtors(examples, NULL, "jesse.pinkman@gmail.com");
	ASSERT_TEST(error == YAD3_NULL_ARG);
	error = reportRelevantRealtors(examples, stdout, NULL);
	ASSERT_TEST(error == YAD3_NULL_ARG);

	error = reportRelevantRealtors(examples, stdout, "@jesse.pinkman@gmail.com");
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = reportRelevantRealtors(examples, stdout, "jesse.pinkmangmail.com");
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = reportRelevantRealtors(examples, stdout, "");
	ASSERT_TEST(error == YAD3_BAD_ARG);

	error = reportRelevantRealtors(examples, stdout, "glorious@realtor.com");
	ASSERT_TEST(error == YAD3_EMAIL_WRONG_ACCOUNT_TYPE);

	error = reportRelevantRealtors(examples, stdout, "glorious@realtorkjhkjh.com");
	ASSERT_TEST(error == YAD3_EMAIL_DOES_NOT_EXIST);

	error = reportRelevantRealtors(examples, stdout, "jesse.pinkman@gmail.com");
	ASSERT_TEST(error == YAD3_SUCCESS);

	yad3Destroy(examples);

	return true;
}

static bool reportSignificantRealtors_test() {
	Yad3Result error;
	SET_UP(Yad3);

	error = reportSignificantRealtors(examples, stdout, 0);
	ASSERT_TEST(error == YAD3_BAD_ARG);
	error = reportSignificantRealtors(examples, stdout, -1);
	ASSERT_TEST(error == YAD3_BAD_ARG);

	error = reportSignificantRealtors(examples, stdout, 2);
	ASSERT_TEST(error == YAD3_SUCCESS);

	yad3Destroy(examples);

	return true;
}

int main() {
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	RUN_TEST(yad3Create_test);
	RUN_TEST(destroyYad3_test);
	RUN_TEST(addRealtor_test);
	RUN_TEST(addCustomer_test);
	RUN_TEST(removeRealtor_test);
	RUN_TEST(removeCustomer_test);
	RUN_TEST(realtorAddApartmentService_test);
	RUN_TEST(realtorRemoveApartmentService_test);
	RUN_TEST(realtorAddApartment_test);
	RUN_TEST(realtorRemoveApartment_test);
	RUN_TEST(customerMakeOffer_test);
	RUN_TEST(realtorRespondToOffer_test);
	RUN_TEST(customerPurchase_test);
	RUN_TEST(mostPayingCustomers_test);
	RUN_TEST(reportSignificantRealtors_test);
	RUN_TEST(reportRelevantRealtors_test);
	return 0;
}
