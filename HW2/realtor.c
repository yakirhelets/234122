#include "realtor.h"
#include <string.h>
#include <assert.h>
#include <stdlib.h>

struct realtor_t {
    char* email;
    char* company_name;
    int tax_percentage;
    Map apartment_service_map;
};

Realtor realtorCreate(char* email, char* company_name, int tax_percentage, RealtorResult* error)    {
    if (!email || !company_name) {
        *error = REALTOR_NULL_ARG;
        return NULL;
    }
    if (tax_percentage <= 0 || tax_percentage > 100 || !isValidMail(email) || !isValidName(company_name)) {
        *error = REALTOR_BAD_ARG;
        return NULL;
    }
    Realtor realtor = malloc(sizeof(*realtor));
    if (!realtor)  {
        *error = REALTOR_OUT_OF_MEM;
        return NULL;
    }
    realtor->email = malloc(sizeof(char)*(strlen(email)+1));
    if (!(realtor->email))  {
        free(realtor);
        *error = REALTOR_OUT_OF_MEM;
        return NULL;
    }
    strcpy(realtor->email, email);
    realtor->company_name = malloc(sizeof(char)*(strlen(company_name)+1));
    if (!(realtor->company_name))  {
		realtorDestroy(realtor);
		*error = REALTOR_OUT_OF_MEM;
        return NULL;
    }
    strcpy(realtor->company_name, company_name);
    realtor->tax_percentage = tax_percentage;
    realtor->apartment_service_map = mapCreate(serviceCopyAux,copyString,serviceFree,
			freeString,compareString);
	if (!(realtor->apartment_service_map)) {
		realtorDestroy(realtor);
	}
	*error = REALTOR_SUCCESS;
    return realtor;
}

void realtorDestroy(Realtor realtor) {
    if (!realtor)  {
        return;
    }
    mapDestroy(realtor->apartment_service_map);
    free(realtor->company_name);
    free(realtor->email);
    free(realtor);
}

Realtor realtorCopy(Realtor realtor, RealtorResult* error) {
    if (!realtor) {
        *error = REALTOR_NULL_ARG;
        return NULL;
    }
    Realtor copy = realtorCreate(realtor->email,realtor->company_name,
                    			 realtor->tax_percentage, error);
    if (!copy) {
        *error = REALTOR_OUT_OF_MEM;
        return NULL;
    }
    copy->apartment_service_map =  mapCopy(realtor->apartment_service_map);
    *error = REALTOR_SUCCESS;
    return copy;
}

char* realtorGetMail(Realtor realtor) {
    assert(realtor!=NULL);
    return realtor->email;
}

char* realtorGetCompanyName(Realtor realtor) {
	assert(realtor!=NULL);
	return realtor->company_name;
}

int realtorGetTaxPercentage(Realtor realtor) {
	assert(realtor!=NULL);
	return realtor->tax_percentage;
}

RealtorResult realtorSetTaxPercentage(Realtor realtor, int new_tax) {
	if (!realtor) {
		return REALTOR_NULL_ARG;
	}
	if (new_tax <= 0 || new_tax > 100) {
		return REALTOR_BAD_ARG;
	}
	realtor->tax_percentage = new_tax;
	return REALTOR_SUCCESS;
}

RealtorResult realtorSetMail(Realtor realtor, char* new_mail) {
    if (!realtor || !new_mail) {
        return REALTOR_NULL_ARG;
    }
    if (isValidMail(new_mail)) {
        realtor->email = (char*)realloc(realtor->email, sizeof(char)*(strlen(new_mail)+1));
        if (!(realtor->email)) {
            return REALTOR_OUT_OF_MEM;
        }
        strcpy(realtor->email,new_mail);
    } else {
        return REALTOR_BAD_ARG;
    }
    return REALTOR_SUCCESS;
}

RealtorResult realtorSetCompanyName(Realtor realtor, char* new_company_name) {
    if (!realtor || !new_company_name) {
        return REALTOR_NULL_ARG;
    }
    if (!isValidName(new_company_name)) {
        return REALTOR_BAD_ARG;
    }
    realtor->company_name = (char*)realloc(realtor->company_name,
        				                   sizeof(char)*(strlen(new_company_name)+1));
    if (!(realtor->company_name)) {
    //should destroy the realtor in other function
    	return REALTOR_OUT_OF_MEM;
    }
    strcpy(realtor->company_name,new_company_name);
    return REALTOR_SUCCESS;
}

Map realtorGetServices(Realtor realtor) {
    assert(realtor!=NULL);
    return realtor->apartment_service_map;
}
