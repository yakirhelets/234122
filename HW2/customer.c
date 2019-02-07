#include "customer.h"
#include "auxFunctions.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

struct customer_t {
    char* email;
    int min_area;
    int min_rooms;
    int max_price;
    int spent_by_far;
};

Customer customerCreate(char* email, int min_area, int min_rooms, int max_price, CustomerResult* error) {
    if (!email) {
       *error = CUSTOMER_NULL_ARG; 
       return NULL;
    }
    if (min_area <= 0 || min_rooms <= 0 ||max_price <= 0 || !isValidMail(email))  {
        *error = CUSTOMER_BAD_ARG;
        return NULL;
    }
    Customer customer = malloc(sizeof(*customer));
    if (!customer)  {
        *error = CUSTOMER_OUT_OF_MEM;
        return NULL;
    }
    customer->email = malloc(sizeof(char)*(strlen(email)+1));
    if (!customer->email)  {
        *error = CUSTOMER_OUT_OF_MEM;
        free(customer);
        return NULL;
    }
    strcpy(customer->email, email);
    customer->min_area = min_area;
    customer->min_rooms = min_rooms;
    customer->max_price = max_price;
    customer->spent_by_far = 0;
    *error = CUSTOMER_SUCCESS;
    return customer;
}

void customerDestroy(Customer customer) {
    if (!customer)  {
        return;
    }
    free(customer->email);
    free(customer);
}

Customer customerCopy(Customer customer, CustomerResult* error) {
    if (!customer) {
        *error = CUSTOMER_NULL_ARG;
        return NULL;
    }
    Customer copy = customerCreate(customer->email,customer->min_area,
                    customer->min_rooms,customer->max_price, error);
    if (!copy) {
        *error = CUSTOMER_OUT_OF_MEM;
        return NULL;
    }
    *error = CUSTOMER_SUCCESS;
    return copy;
}

int customerGetMinArea(Customer customer)   {
    assert(customer!=NULL);
    return (customer->min_area);
}

CustomerResult customerSetMinArea(Customer customer, int min_area)    {
    if (!customer) {
        return CUSTOMER_NULL_ARG;
    }
    if (min_area <= 0) {
        return CUSTOMER_BAD_ARG;
    }
    customer->min_area = min_area;
    return CUSTOMER_SUCCESS;
}

int customerGetMinRooms(Customer customer)  {
    assert(customer!=NULL);
    return (customer->min_rooms);
}

CustomerResult customerSetMinRooms(Customer customer, int min_rooms) {
    if (!customer) {
        return CUSTOMER_NULL_ARG;
    }
    if (min_rooms <= 0) {
        return CUSTOMER_BAD_ARG;
    }
    customer->min_rooms = min_rooms;
    return CUSTOMER_SUCCESS;
}

int customerGetMaxPrice(Customer customer)  {
    assert(customer!=NULL);
    return (customer->max_price);
}

CustomerResult customerSetMaxPrice(Customer customer, int max_price) {
    if (!customer) {
        return CUSTOMER_NULL_ARG;
    }
    if (max_price <= 0) {
        return CUSTOMER_BAD_ARG;
    }
    customer->max_price = max_price;
    return CUSTOMER_SUCCESS;
}

char* customerGetMail(Customer customer) {
    assert(customer!=NULL);
    return customer->email;
}

CustomerResult customerSetMail(Customer customer, char* new_mail) {
    if (!customer || !new_mail) {
        return CUSTOMER_NULL_ARG;
    }
    if (isValidMail(new_mail)) {
        customer->email = (char*)realloc(customer->email, sizeof(char)*(strlen(new_mail)+1));
        if (!(customer->email)) {
            return CUSTOMER_OUT_OF_MEM;
        }
        strcpy(customer->email,new_mail);
    } else {
        return CUSTOMER_BAD_ARG;
    }
    return CUSTOMER_SUCCESS;
}

int customerGetSpentByFar(Customer customer) {
    assert(customer != NULL);
    return customer->spent_by_far;
}

CustomerResult customerSetSpentByFar(Customer customer, int added_price) {
    if (!customer) {
        return CUSTOMER_NULL_ARG;
    }
    if (added_price<=0) {
        return CUSTOMER_BAD_ARG;
    } else {
        customer->spent_by_far += added_price;
        return CUSTOMER_SUCCESS;
    }
}
