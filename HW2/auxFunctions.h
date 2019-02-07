#ifndef AUXFUNCTIONS_H_
#define AUXFUNCTIONS_H_
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "map.h"
#include "apartment_service.h"

/*
 * Checks whether a given string is in a correct email address format.
 * A valid email format is a non-empty string which contains at single '@' only.
 * @return
 * 		TRUE - if the given string is a valid mail address, FALSE otherwise.
 */
bool isValidMail(char* email);

/* Creates a copy of a given string.
* @return
*    A newly allocated exact copy of a given string.
*/
MapKeyElement copyString(constMapKeyElement src);

/* Frees all memory allocated for a given string.
* This function can receive NULL.
*/
void freeString(MapKeyElement str);

/*
 *
This function starts comparing the first character of each string.
If they are equal to each other, it continues with the following pairs until
 the characters differ or until a terminating null-character is reached.
 @return
 	 <0 : the first character that does not match has a lower value
 	      in dest than in src.
 	  0 : the contents of both strings are equal.
 	 >0 : the first character that does not match has a greater value
 	      in dest than in src.  
 */
int compareString(constMapKeyElement dest, constMapKeyElement src);

/* Copy function: Creates a copy of an apartment service in a Map.
* @return
*    A copy of an apartment service.
*/
MapDataElement serviceCopyAux(constMapDataElement service);

/* Frees all memory allocated for a given service.
* This function can receive NULL.
*/
void serviceFree(MapDataElement service);

/*
 * Checks whether a given string has is valid.
 * A valid string is a non-empty string.
 */
bool isValidName(char* name);

#endif /* AUXFUNCTIONS_H_ */
