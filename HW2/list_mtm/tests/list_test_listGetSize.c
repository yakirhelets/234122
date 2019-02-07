#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "test_utilities.h"
#include "../list_mtm/list_mtm1.h"
//#include "list_mtm1.h"

/**
 * We're going to be doing this a lot, so we define a macro for this specific
 * test. You can define many such macros for repetitive assertions in your
 * unit-tests.
 */
#define ASSERT_SAME_PERSON(a,b) ASSERT_TEST(personCompare(a,b) == 0)

/** A simple datatype of Person to be used in testing the list
 * Note that this is not an ADT because the struct is exposed.
 **/
typedef struct Person_t {
	char* name;
	int id;
} *Person;

static Person personCreate(const char* name, int id);
static Person personCopy(Person person);
static void personDestroy(Person person);

static char* stringDuplicate(const char* str);

static char* stringDuplicate(const char* str) {
	assert(str);
	char* result = malloc(strlen(str)+1);
	return result ? strcpy(result,str) : NULL;
}

static void personDestroy(Person person) {
	if (!person) {
		return;
	}
	free(person->name);
	free(person);
}

static Person personCreate(const char* name, int id) {
	assert(name);
	Person person = malloc(sizeof(struct Person_t));
	if (!person) {
		return NULL;
	}
	person->name = stringDuplicate(name);
	if (!name) {
		personDestroy(person);
		return NULL;
	}
	person->id = id;
	return person;
}

static Person personCopy(Person person) {
	assert(person);
	return personCreate(person->name, person->id);
}


/*!
 * Helper functions for storing Person objects in a list
 */
static ListElement personCopyHelper(ListElement element) {
	return personCopy(element);
}

static void personDestroyHelper(ListElement element) {
	personDestroy(element);
}



/**
 * This struct is used to hold a set of examples
 * This allows us to save code on reinitializing big examples in the start of
 * each test
 */
typedef struct {
	Person haimMoshe;
	Person oferLevi;
	Person eyalGolan;
	Person aviBitter;
	Person idanTal;
	List list;
} Examples;

/**
 * This function is called by the SET_UP macro in testing_utilities. This
 * function should initialize an examples struct that can be used for testing
 * the ADT
 * @return a new fully initialize examples sturct
 */
static Examples setUp() {
	Examples examples;
	examples.haimMoshe = personCreate("Haim Moshe", 111111111);
	examples.oferLevi = personCreate("Ofer Levi", 222222222);
	examples.eyalGolan = personCreate("Eyal Golan", 333333333);
	examples.aviBitter = personCreate("Avi Bitter", 444444444);
	examples.idanTal = personCreate("Idan Tal", 555555555);
	examples.list = listCreate(personCopyHelper, personDestroyHelper);
	listInsertLast(examples.list, examples.eyalGolan);
	listInsertLast(examples.list, examples.oferLevi);
	listInsertLast(examples.list, examples.aviBitter);
	listInsertLast(examples.list, examples.haimMoshe);
	return examples;
}

/**
 * Called by the TEAR_DOWN macro from testing_utilities.h. This function should
 * deallocate and clear all the code
 * @param examples
 */
void tearDown(Examples examples) {
	personDestroy(examples.haimMoshe);
	personDestroy(examples.oferLevi);
	personDestroy(examples.eyalGolan);
	personDestroy(examples.aviBitter);
	personDestroy(examples.idanTal);
	listDestroy(examples.list);
}


static bool testListGetSize() {
	SET_UP(Examples);
	List list = listCreate(personCopyHelper, personDestroyHelper);
	ASSERT_TEST(0 == listGetSize(list));
	listInsertFirst(list, examples.aviBitter);
	ASSERT_TEST(1 == listGetSize(list));
	listInsertFirst(list, examples.aviBitter);
	ASSERT_TEST(2 == listGetSize(list));
	listGetFirst(list);
	listRemoveCurrent(list);
	ASSERT_TEST(1 == listGetSize(list));
	listDestroy(list);
	TEAR_DOWN();
	return true;
}

int main() {

	RUN_TEST(testListGetSize);

	return 0;
}
