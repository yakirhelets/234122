#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "test_utilities.h"
#include "../realtor.h"

static bool realtorCreate_test()	{

	char* email = "realtor@realestate.gr";
	char* company_name = "Best Builders Inc";
	int tax_percentage = 33;
	RealtorResult error1,error2,error3,error4,error5;
	
	Realtor realtor1 = realtorCreate(email, company_name, tax_percentage, &error1);
	Realtor realtor2 = realtorCreate("183@hotmail.com","",54, &error2);
	Realtor realtor3 = realtorCreate("a@a.com","OAS",-1, &error3);
	Realtor realtor4 = realtorCreate("!","!",5, &error4);
	Realtor realtor5 = realtorCreate(NULL,NULL,5000, &error5);
	
	ASSERT_TEST(error1 == REALTOR_SUCCESS);
	ASSERT_TEST(realtor1 != NULL);
	ASSERT_TEST(error2 == REALTOR_BAD_ARG);
	ASSERT_TEST(realtor2 == NULL);
	ASSERT_TEST(error3 == REALTOR_BAD_ARG);
	ASSERT_TEST(realtor3 == NULL);
	ASSERT_TEST(error4 == REALTOR_BAD_ARG);
	ASSERT_TEST(realtor4 == NULL);
	ASSERT_TEST(error5 == REALTOR_NULL_ARG);
	ASSERT_TEST(realtor5 == NULL);
	ASSERT_TEST(strcmp(realtorGetMail(realtor1), email) == 0);
	ASSERT_TEST(strcmp(realtorGetCompanyName(realtor1), company_name) == 0);
	ASSERT_TEST(realtorGetTaxPercentage(realtor1) == tax_percentage);
	realtorDestroy(realtor1);
	realtorDestroy(realtor2);
	realtorDestroy(realtor3);
	realtorDestroy(realtor4);
	realtorDestroy(realtor5);
	return true;
}

static bool realtorDestroy_test()	{
	
	Realtor realtor = NULL;
	realtorDestroy(realtor);
	RealtorResult error;
	
	realtor = realtorCreate("realtor@realestate.gr","Best Builders Inc", 33, &error);
	ASSERT_TEST(error == REALTOR_SUCCESS);
	ASSERT_TEST(realtor != NULL);
	realtorDestroy(realtor);

	return true;
}

static bool realtorGetMail_test()	{
	
	RealtorResult error;
	char* email = "realtor@realestate.gr";
	char* company_name = "Best Builders Inc";
	Realtor realtor = realtorCreate(email,company_name,1, &error);
	ASSERT_TEST(error == REALTOR_SUCCESS);
	ASSERT_TEST(strcmp(realtorGetMail(realtor),email) == 0);
	realtorDestroy(realtor);
	
	return true;
}

static bool realtorGetCompanyName_test()	{
	
	RealtorResult error;
	char* email = "realtor@realestate.gr";
	char* company_name = "Best Builders Inc";
	Realtor realtor = realtorCreate(email,company_name,100, &error);
	ASSERT_TEST(error == REALTOR_SUCCESS);
	ASSERT_TEST(strcmp(realtorGetCompanyName(realtor),company_name) == 0);
	realtorDestroy(realtor);

	return true;
}

static bool realtorGetTaxPercentage_test()	{

	RealtorResult error;
	char* email = "realtor@realestate.gr";
	char* company_name = "Best Builders Inc";
	int tax_percentage = 17;
	Realtor realtor = realtorCreate(email,company_name,17, &error);
	ASSERT_TEST(error == REALTOR_SUCCESS);
	ASSERT_TEST(realtorGetTaxPercentage(realtor) == tax_percentage);
	realtorDestroy(realtor);

	return true;
}

static bool realtorSetMail_test()	{
	
	RealtorResult error;

	ASSERT_TEST(realtorSetMail(NULL, "awesomeRealtor@yad3.com") == REALTOR_NULL_ARG);

	Realtor realtor = realtorCreate("realtor@awesomeNadlan.com", "awesomeNadlan",15, &error);
	ASSERT_TEST(error == REALTOR_SUCCESS);
	ASSERT_TEST(strcmp(realtorGetMail(realtor), "realtor@awesomeNadlan.com") == 0);

	ASSERT_TEST(realtorSetMail(realtor, "awesomeRealtor@nadlan@.com") == REALTOR_BAD_ARG);
	ASSERT_TEST(realtorSetMail(realtor, "@awesomeRealtor@nadlan.com") == REALTOR_BAD_ARG);
	ASSERT_TEST(realtorSetMail(realtor, "awesomeRealtor@nadlan.com@") == REALTOR_BAD_ARG);
	ASSERT_TEST(realtorSetMail(realtor, "awesomeRealtornadlan.com") == REALTOR_BAD_ARG);

	ASSERT_TEST(strcmp(realtorGetMail(realtor), "realtor@awesomeNadlan.com") == 0);

	ASSERT_TEST(realtorSetMail(realtor, "realtor@awesomeNadlan.com") == REALTOR_SUCCESS);
	ASSERT_TEST(strcmp(realtorGetMail(realtor), "realtor@awesomeNadlan.com") == 0);

	realtorDestroy(realtor);

	return true;
}

static bool realtorSetCompanyName_test()	{
	
	RealtorResult error;
	
	ASSERT_TEST(realtorSetCompanyName(NULL, "IBI") == REALTOR_NULL_ARG);
	
	Realtor realtor = realtorCreate("realtor@awesomeNadlan.com", "awesomeNadlan",15, &error);
	ASSERT_TEST(error == REALTOR_SUCCESS);
	ASSERT_TEST(strcmp(realtorGetCompanyName(realtor), "awesomeNadlan") == 0);
	ASSERT_TEST(realtorSetCompanyName(realtor, "") == REALTOR_BAD_ARG);
	ASSERT_TEST(realtorSetCompanyName(realtor, "XFX") == REALTOR_SUCCESS);
	ASSERT_TEST(strcmp(realtorGetCompanyName(realtor), "XFX") == 0);

	realtorDestroy(realtor);

	return true;
}

static bool realtorSetTaxPercentage_test()	{
	
	RealtorResult error;
	
	int tax_percentage = 15;
	Realtor realtor = NULL;
	ASSERT_TEST(realtorSetTaxPercentage(realtor,53) == REALTOR_NULL_ARG);
	realtor = realtorCreate("realtor@awesomeNadlan.com", "awesomeNadlan",tax_percentage, &error);
	ASSERT_TEST(error == REALTOR_SUCCESS);
	ASSERT_TEST(realtorGetTaxPercentage(realtor) == tax_percentage);
	ASSERT_TEST(realtorSetTaxPercentage(realtor, -1) == REALTOR_BAD_ARG);
	ASSERT_TEST(realtorSetTaxPercentage(realtor, 53) == REALTOR_SUCCESS);

	realtorDestroy(realtor);

	return true;
}

static bool realtorGetServices_test()	{
	
	Map map;
	RealtorResult error;
	char* email = "realtor@realestate.gr";
	char* company_name = "Best Builders Inc";
	int tax_percentage = 17;
	Realtor realtor = realtorCreate(email,company_name,tax_percentage, &error);
	
	map = realtorGetServices(realtor);
	ASSERT_TEST(map != NULL);
	
	realtorDestroy(realtor);

	return true;
}

static bool realtorCopy_test()	{
	
	Realtor realtor = NULL;
	Realtor copy = NULL;
	RealtorResult error;
	char* email = "realtor@realestate.gr";
	char* company_name = "Best Builders Inc";
	
	ASSERT_TEST(realtorCopy(realtor,&error) == NULL);
	ASSERT_TEST(error == REALTOR_NULL_ARG);
	
	realtor = realtorCreate(email,company_name,26, &error);
	ASSERT_TEST(error == REALTOR_SUCCESS);
	copy = realtorCopy (realtor,&error);
	ASSERT_TEST(error == REALTOR_SUCCESS);
	ASSERT_TEST(strcmp(realtorGetMail(realtor), realtorGetMail(copy)) == 0);
	ASSERT_TEST(strcmp(realtorGetCompanyName(realtor), realtorGetCompanyName(copy)) == 0);
	ASSERT_TEST(realtorGetTaxPercentage(realtor) == realtorGetTaxPercentage(copy));
	
	// * * MUST ADD A CHECK OF THE LISTS THEMSELVES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! * * //
	
	realtorDestroy(realtor);
	realtorDestroy(copy);

	return true;
}

int main() {
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	RUN_TEST(realtorCreate_test);
	RUN_TEST(realtorDestroy_test);
	RUN_TEST(realtorGetMail_test);
	RUN_TEST(realtorGetCompanyName_test);
	RUN_TEST(realtorGetTaxPercentage_test);
	RUN_TEST(realtorSetMail_test);
	RUN_TEST(realtorSetCompanyName_test);
	RUN_TEST(realtorSetTaxPercentage_test);
	RUN_TEST(realtorGetServices_test);
	RUN_TEST(realtorCopy_test);
	return 0;
}
