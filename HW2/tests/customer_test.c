#include <assert.h>
#include <string.h>
#include "test_utilities.h"
#include "../customer.h"


static bool customerCreate_test()	{
	Customer customer = NULL;
	CustomerResult error;
	customer = customerCreate("customer@@nadlan.com", 4, 2, 1000, &error);
	ASSERT_TEST(customer == NULL);
	ASSERT_TEST(error == CUSTOMER_BAD_ARG);
	customer = customerCreate("customernadlan.com", 4, 2, 1000, &error);
	ASSERT_TEST(customer == NULL);
	ASSERT_TEST(error == CUSTOMER_BAD_ARG);
	customer = customerCreate("customer@nadlan.com@", 4, 2, 1000, &error);
	ASSERT_TEST(customer == NULL);
	ASSERT_TEST(error == CUSTOMER_BAD_ARG);
	customer = customerCreate("@customer@nadlan.com@", 4, 2, 1000, &error);
	ASSERT_TEST(customer == NULL);
	ASSERT_TEST(error == CUSTOMER_BAD_ARG);
	customer = customerCreate("", 4, 2, 1000, &error);
	ASSERT_TEST(customer == NULL);
	ASSERT_TEST(error == CUSTOMER_BAD_ARG);
	customer = customerCreate(NULL, 4, 2, 1000, &error);
	ASSERT_TEST(customer == NULL);
	ASSERT_TEST(error == CUSTOMER_NULL_ARG);
	customer = customerCreate("customer@nadlan.com", 0, 2, 1000, &error);
	ASSERT_TEST(customer == NULL);
	ASSERT_TEST(error == CUSTOMER_BAD_ARG);
	customer = customerCreate("customer@nadlan.com", 4, 0, 1000, &error);
	ASSERT_TEST(customer == NULL);
	ASSERT_TEST(error == CUSTOMER_BAD_ARG);
	customer = customerCreate("customer@nadlan.com", 4, 2, 0, &error);
	ASSERT_TEST(customer == NULL);
	ASSERT_TEST(error == CUSTOMER_BAD_ARG);
	
	customer = customerCreate("customer@nadlan.com", 4, 2, 1000, &error);
	ASSERT_TEST(customer != NULL);
	ASSERT_TEST(error == CUSTOMER_SUCCESS);
	
	ASSERT_TEST(strcmp(customerGetMail(customer), "customer@nadlan.com") == 0);
	ASSERT_TEST(customerGetMinArea(customer) == 4);
	ASSERT_TEST(customerGetMinRooms(customer) == 2);
	ASSERT_TEST(customerGetMaxPrice(customer) == 1000);
	customerDestroy(customer);

	return true;
}

static bool customerDestroy_test()	{
	CustomerResult error;
	Customer customer = NULL;
	customerDestroy(customer);
	
	customer = customerCreate("customer@nadlan.com", 4, 2, 1000, &error);
	ASSERT_TEST(customer != NULL);
	ASSERT_TEST(error == CUSTOMER_SUCCESS);
	
	customerDestroy(customer);

	return true;
}

static bool customerGetMinArea_test()	{
	CustomerResult error;
	Customer customer = customerCreate("customer@nadlan.com", 4, 2, 1000, &error);

	ASSERT_TEST(customerGetMinArea(customer) == 4);
	ASSERT_TEST(customerGetMinArea(customer) != 3);
	customerDestroy(customer);

	return true;
}

static bool customerSetMinArea_test()	{
	CustomerResult error;
	ASSERT_TEST(customerSetMinArea(NULL, 3) == CUSTOMER_NULL_ARG);
	
	Customer customer = customerCreate("customer@nadlan.com", 4, 2, 1000, &error);
	ASSERT_TEST(customerGetMinArea(customer) == 4);
	
	ASSERT_TEST(customerSetMinArea(customer, -1) == CUSTOMER_BAD_ARG);
	ASSERT_TEST(customerSetMinArea(customer, 0) == CUSTOMER_BAD_ARG);
	ASSERT_TEST(customerGetMinArea(customer) == 4);
	
	ASSERT_TEST(customerSetMinArea(customer, 3) == CUSTOMER_SUCCESS);
	ASSERT_TEST(customerGetMinArea(customer) == 3);
	
	customerDestroy(customer);

	return true;
}

static bool customerGetMinRooms_test()	{
	CustomerResult error;
	Customer customer = customerCreate("customer@nadlan.com", 4, 2, 1000, &error);

	ASSERT_TEST(customerGetMinRooms(customer) == 2);
	ASSERT_TEST(customerGetMinRooms(customer) != 3);
	customerDestroy(customer);

	return true;
}

static bool customerSetMinRooms_test()	{
	CustomerResult error;
	ASSERT_TEST(customerSetMinRooms(NULL, 3) == CUSTOMER_NULL_ARG);
	
	Customer customer = customerCreate("customer@nadlan.com", 4, 2, 1000, &error);
	ASSERT_TEST(customerGetMinRooms(customer) == 2);
	
	ASSERT_TEST(customerSetMinRooms(customer, -1) == CUSTOMER_BAD_ARG);
	ASSERT_TEST(customerSetMinRooms(customer, 0) == CUSTOMER_BAD_ARG);
	ASSERT_TEST(customerGetMinRooms(customer) == 2);
	
	ASSERT_TEST(customerSetMinRooms(customer, 3) == CUSTOMER_SUCCESS);
	ASSERT_TEST(customerGetMinRooms(customer) == 3);
	
	customerDestroy(customer);

	return true;
}

static bool customerGetMaxPrice_test()	{
	CustomerResult error;
	Customer customer = customerCreate("customer@nadlan.com", 4, 2, 1000, &error);
	ASSERT_TEST(customerGetMaxPrice(customer) == 1000);
	ASSERT_TEST(customerGetMaxPrice(customer) != 3);
	customerDestroy(customer);

	return true;
}

static bool customerSetMaxPrice_test()	{
	CustomerResult error;
	ASSERT_TEST(customerSetMaxPrice(NULL, 3) == CUSTOMER_NULL_ARG);
	
	Customer customer = customerCreate("customer@nadlan.com", 4, 2, 1000, &error);
	ASSERT_TEST(customerGetMaxPrice(customer) == 1000);
	
	ASSERT_TEST(customerSetMaxPrice(customer, -1) == CUSTOMER_BAD_ARG);
	ASSERT_TEST(customerSetMaxPrice(customer, 0) == CUSTOMER_BAD_ARG);
	ASSERT_TEST(customerGetMaxPrice(customer) == 1000);
	
	ASSERT_TEST(customerSetMaxPrice(customer, 100000) == CUSTOMER_SUCCESS);
	ASSERT_TEST(customerGetMaxPrice(customer) == 100000);
	
	customerDestroy(customer);

	return true;
}

static bool customerGetMail_test()	{
	CustomerResult error;
	Customer customer = customerCreate("customer@nadlan.com", 4, 2, 1000, &error);
	ASSERT_TEST(strcmp(customerGetMail(customer), "customer@nadlan.com") == 0);
	ASSERT_TEST(strcmp(customerGetMail(customer), "customer@nadlan.co") != 0);
	ASSERT_TEST(strcmp(customerGetMail(customer), "ustomer@nadlan.com") != 0);
	customerDestroy(customer);

	return true;
}

static bool customerSetMail_test()	{
	CustomerResult error;
	ASSERT_TEST(customerSetMail(NULL, "awesomeCustomer@nadlan.com") == CUSTOMER_NULL_ARG);
	
	Customer customer = customerCreate("customer@nadlan.com", 4, 2, 1000, &error);
	ASSERT_TEST(strcmp(customerGetMail(customer), "customer@nadlan.com") == 0);

	ASSERT_TEST(customerSetMail(customer, "awesomeCustomer@nadlan@.com") == CUSTOMER_BAD_ARG);
	ASSERT_TEST(customerSetMail(customer, "@awesomeCustomer@nadlan.com") == CUSTOMER_BAD_ARG);
	ASSERT_TEST(customerSetMail(customer, "awesomeCustomer@nadlan.com@") == CUSTOMER_BAD_ARG);
	ASSERT_TEST(customerSetMail(customer, "awesomeCustomernadlan.com") == CUSTOMER_BAD_ARG);

	ASSERT_TEST(strcmp(customerGetMail(customer), "customer@nadlan.com") == 0);
	
	ASSERT_TEST(customerSetMail(customer, "awesomeCustomer@nadlan.com") == CUSTOMER_SUCCESS);
	ASSERT_TEST(strcmp(customerGetMail(customer), "awesomeCustomer@nadlan.com") == 0);
	
	customerDestroy(customer);

	return true;
}

static bool customerGetSpentByFar_test()	{
	CustomerResult error;
	
	Customer customer = customerCreate("customer@nadlan.com", 4, 2, 1000000, &error);
	ASSERT_TEST(customerGetSpentByFar(customer) == 0);
	
	customerDestroy(customer);

	return true;
}

static bool customerSetSpentByFar_test()	{
	CustomerResult error;
	ASSERT_TEST(customerSetSpentByFar(NULL, 10000) == CUSTOMER_NULL_ARG);
	
	Customer customer = customerCreate("customer@nadlan.com", 4, 2, 1000000, &error);
	ASSERT_TEST(customerSetSpentByFar(customer, 0) == CUSTOMER_BAD_ARG);
	ASSERT_TEST(customerSetSpentByFar(customer, -1000) == CUSTOMER_BAD_ARG);

	ASSERT_TEST(customerGetSpentByFar(customer) == 0);
	ASSERT_TEST(customerSetSpentByFar(customer, 10000) == CUSTOMER_SUCCESS);
	ASSERT_TEST(customerGetSpentByFar(customer) == 10000);
	ASSERT_TEST(customerSetSpentByFar(customer, 10001) == CUSTOMER_SUCCESS);
	ASSERT_TEST(customerGetSpentByFar(customer) == 20001);
	
	customerDestroy(customer);

	return true;
}

static bool customerCopy_test()	{
	CustomerResult error;
	Customer customer = NULL;
	Customer copy = NULL;
	
	copy = customerCopy(customer, &error);
	ASSERT_TEST(copy == NULL);
	ASSERT_TEST(error == CUSTOMER_NULL_ARG);
	
	customer = customerCreate("customer@nadlan.com", 4, 2, 1000, &error);
	ASSERT_TEST(error == CUSTOMER_SUCCESS);
	copy = customerCopy (customer, &error);
	ASSERT_TEST(error == CUSTOMER_SUCCESS);
	
	ASSERT_TEST(strcmp(customerGetMail(customer), customerGetMail(copy)) == 0);
	ASSERT_TEST(customerGetMaxPrice(customer) == customerGetMaxPrice(copy));
	ASSERT_TEST(customerGetMinArea(customer) == customerGetMinArea(copy));
	ASSERT_TEST(customerGetMinRooms(customer) == customerGetMinRooms(copy));
	
	customerDestroy(customer);
	customerDestroy(copy);

	return true;
}

int main() {
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	RUN_TEST(customerCreate_test);
	RUN_TEST(customerDestroy_test);
	RUN_TEST(customerGetMinArea_test);
	RUN_TEST(customerSetMinArea_test);
	RUN_TEST(customerGetMinRooms_test);
	RUN_TEST(customerSetMinRooms_test);
	RUN_TEST(customerGetMaxPrice_test);
	RUN_TEST(customerSetMaxPrice_test);
	RUN_TEST(customerGetMail_test);
	RUN_TEST(customerSetMail_test);
	RUN_TEST(customerGetSpentByFar_test);
	RUN_TEST(customerSetSpentByFar_test);
	RUN_TEST(customerCopy_test);
	return 0;
}
