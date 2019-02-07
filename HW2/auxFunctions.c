#include "auxFunctions.h"

bool isValidMail(char* email) {
    int counter = 0;
	while (*email) {
	    if (*email == '@') {
	        counter++;
	    }
	    email++;
	}
	return (counter==1);
}

bool isValidName(char* name) {
	return (strlen(name) != 0);
}

MapKeyElement copyString(constMapKeyElement src) {
	char* dest = malloc(sizeof(char)*(strlen((char*)src)+1));
    if (!dest) {
    	return NULL;
    }
    strcpy((char*)dest,(char*)src);
    return (char*)dest;
}

void freeString(MapKeyElement str) {
    free(str);
}

int compareString(constMapKeyElement dest, constMapKeyElement src) {
	return strcmp(dest,src);
}

MapDataElement serviceCopyAux(constMapDataElement service) {
	return serviceCopy((ApartmentService)service);
}

void serviceFree(MapDataElement service) {
	serviceDestroy(service);
}
