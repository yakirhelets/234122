#include "test_utilities.h"
#include "list_mtm.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

static bool isLonger(ListElement element,ListFilterKey number) {
	char* string = element;
	return strlen(string) > *(int*)number;
}

static ListElement copyString(ListElement str){
	assert(str!=NULL);
	char* copy = malloc(strlen(str)+1);
	return copy!=NULL?strcpy(copy, str):NULL;
}

static int compareString(ListElement str1, ListElement str2){
	int res = strcmp(str1, str2);
	return res;
}

static void freeString(ListElement str){
	free(str);
}

static bool testListCreate1() {
	ASSERT_TEST(listCreate(NULL,NULL) == NULL);
	ASSERT_TEST(listCreate(copyString,NULL) == NULL);
	ASSERT_TEST(listCreate(NULL,freeString) == NULL);
	return true;
}

static bool testListCreate2() {
	List list = listCreate(copyString,freeString);
	listDestroy(list);
	return true;
}

static bool testListDestroy() {
	char* a[5] = {"1","2","3","4","5"};
	List list = listCreate(copyString,freeString);
	for (int i=0;i<5; i++){
		listInsertLast(list,a[i]);
	}
	listDestroy(list);
	listDestroy(NULL);
	return true;
}

static bool testListCopy() {
	char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
	List list = listCreate(copyString,freeString);
	for (int i=0;i<5;i++){
		listInsertFirst(list,a[i]);
	}
	List copy = listCopy(list);
	ASSERT_TEST(strcmp(listGetFirst(copy),listGetFirst(list))==0);
	for(int i=0;i<4;++i) {
		ASSERT_TEST(strcmp(listGetNext(copy),listGetNext(list))==0);
	}
	ASSERT_TEST(listGetNext(copy) == NULL);
	ASSERT_TEST(listGetNext(list) == NULL);
	ASSERT_TEST(listCopy(NULL) == NULL);
	listDestroy(list);
	listDestroy(copy);
	return true;
}

static bool testListFilter() {
	char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
	List list = listCreate(copyString,freeString);
	for (int i=0;i <5; ++i){
		listInsertFirst(list,a[i]);
	}
	int key = 7;
	List filtered_list = listFilter(list,isLonger, &key);
	ASSERT_TEST(listGetSize(filtered_list) == 1);
	ASSERT_TEST(strcmp(listGetFirst(filtered_list),a[3])==0);
	listDestroy(list);
	listDestroy(filtered_list);
	return true;
}



static bool testListGetSize() {
	char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
	List list = listCreate(copyString,freeString);
	ASSERT_TEST(listGetSize(list) == 0);
	for (int i=0;i <5; ++i){
		listInsertFirst(list,a[i]);
	}
	ASSERT_TEST(listGetSize(list) == 5);
	ASSERT_TEST(listGetSize(NULL) == -1);
	listDestroy(list);
	return true;
}
static bool testListGetFirst1() {
	List list = listCreate(copyString,freeString);
	ASSERT_TEST(listGetFirst(list) == NULL);
	listDestroy(list);
	return true;
}
static bool testListGetFirst2() {
	char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
	List list = listCreate(copyString,freeString);
	for (int i=0;i<5;i++){
		listInsertLast(list,a[i]);
	}
	ASSERT_TEST(strcmp(listGetFirst(list),a[0])==0);
	listDestroy(list);
	return true;
}

static bool testListGetNext() {
	char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
	List list = listCreate(copyString,freeString);
	for (int i=0;i<5; i++){
		listInsertLast(list,a[i]);
	}
	ASSERT_TEST(listGetNext(list) == NULL);
	ASSERT_TEST(strcmp(listGetFirst(list),a[0])==0);
	ASSERT_TEST(strcmp(listGetNext(list),a[1])==0);
	ASSERT_TEST(strcmp(listGetNext(list),a[2])==0);
	ASSERT_TEST(strcmp(listGetNext(list),a[3])==0);
	ASSERT_TEST(strcmp(listGetNext(list),a[4])==0);
	ASSERT_TEST(listGetNext(list) == NULL);
	listDestroy(list);
	return true;
}

static bool testListGetCurrent() {
	char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
	List list = listCreate(copyString,freeString);
	for (int i=0;i<5; i++){
		listInsertLast(list,a[i]);
	}
	ASSERT_TEST(strcmp(listGetFirst(list),a[0])==0);
	ASSERT_TEST(strcmp(listGetCurrent(list),a[0])==0);
	ASSERT_TEST(strcmp(listGetNext(list),a[1])==0);
	ASSERT_TEST(strcmp(listGetCurrent(list),a[1])==0);
	ASSERT_TEST(strcmp(listGetNext(list),a[2])==0);
	ASSERT_TEST(strcmp(listGetCurrent(list),a[2])==0);
	ASSERT_TEST(strcmp(listGetNext(list),a[3])==0);
	ASSERT_TEST(strcmp(listGetCurrent(list),a[3])==0);
	ASSERT_TEST(strcmp(listGetNext(list),a[4])==0);
	ASSERT_TEST(strcmp(listGetCurrent(list),a[4])==0);
	ASSERT_TEST(listGetNext(list) == NULL);
	listDestroy(list);
	return true;
}

static bool testListInsertFirst() {
	ASSERT_TEST(listInsertFirst(NULL,NULL) == LIST_NULL_ARGUMENT);
	char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
	List list = listCreate(copyString,freeString);
	ASSERT_TEST(listInsertFirst(NULL,a[0]) == LIST_NULL_ARGUMENT);
	ASSERT_TEST(listInsertFirst(list,a[0]) == LIST_SUCCESS);
	ASSERT_TEST(listInsertFirst(list,a[1]) == LIST_SUCCESS);
	listDestroy(list);
	return true;
}

static bool testListInsertLast() {
	ASSERT_TEST(listInsertLast(NULL,NULL) == LIST_NULL_ARGUMENT);
	char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
	List list = listCreate(copyString,freeString);
	ASSERT_TEST(listInsertLast(NULL,a[0]) == LIST_NULL_ARGUMENT);
	ASSERT_TEST(listInsertLast(list,a[0]) == LIST_SUCCESS);
	ASSERT_TEST(listInsertLast(list,a[1]) == LIST_SUCCESS);
	listDestroy(list);
	return true;
}
static bool testListInsertBeforeCurrent_1() {
	ASSERT_TEST(listInsertBeforeCurrent(NULL,NULL) == LIST_NULL_ARGUMENT);
	char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
	List list = listCreate(copyString,freeString);
	ASSERT_TEST(listInsertBeforeCurrent(NULL,a[0]) == LIST_NULL_ARGUMENT);
	listDestroy(list);
	return true;
}

static bool testListInsertBeforeCurrent_2() {
	char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
	List list = listCreate(copyString,freeString);
	ASSERT_TEST(listInsertLast(list,a[4]) == LIST_SUCCESS);
	ASSERT_TEST(strcmp(listGetFirst(list),a[4]) == 0);
	ASSERT_TEST(listInsertBeforeCurrent(list,a[3]) == LIST_SUCCESS);
	ASSERT_TEST(strcmp(listGetFirst(list),a[3]) == 0);
	ASSERT_TEST(listInsertBeforeCurrent(list,a[2]) == LIST_SUCCESS);
	ASSERT_TEST(strcmp(listGetFirst(list),a[2]) == 0);
	ASSERT_TEST(listInsertBeforeCurrent(list,a[1]) == LIST_SUCCESS);
	ASSERT_TEST(strcmp(listGetFirst(list),a[1]) == 0);
	ASSERT_TEST(listInsertBeforeCurrent(list,a[0]) == LIST_SUCCESS);
	List list2 = listCreate(copyString,freeString);
	for (int i=0;i <5; i++){
		listInsertLast(list2,a[i]);
	}
	ASSERT_TEST(strcmp(listGetFirst(list),listGetFirst(list2))==0);
	for(int i=0;i< 4 ;i++) {
		ASSERT_TEST(strcmp(listGetNext(list),listGetNext(list2))==0);
	}
	listDestroy(list);
	listDestroy(list2);
	return true;
}

static bool testListInsertAfterCurrent_1() {
	ASSERT_TEST(listInsertAfterCurrent(NULL,NULL) == LIST_NULL_ARGUMENT);
	char* str = {"some string"};
	List list = listCreate(copyString,freeString);
	ASSERT_TEST(listInsertAfterCurrent(NULL,str) == LIST_NULL_ARGUMENT);
	listDestroy(list);
	return true;
}
static bool testListInsertAfterCurrent_2() {
	ASSERT_TEST(listInsertAfterCurrent(NULL,NULL) == LIST_NULL_ARGUMENT);
	char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
	List list = listCreate(copyString,freeString);
	ASSERT_TEST(listInsertAfterCurrent(NULL,a[0]) == LIST_NULL_ARGUMENT);
	ASSERT_TEST(listInsertLast(list,a[4]) == LIST_SUCCESS);
	ASSERT_TEST(strcmp(listGetFirst(list),a[4]) == 0);
	ASSERT_TEST(listInsertAfterCurrent(list,a[3]) == LIST_SUCCESS);
	ASSERT_TEST(strcmp(listGetNext(list),a[3]) == 0);
	ASSERT_TEST(listInsertAfterCurrent(list,a[2]) == LIST_SUCCESS);
	ASSERT_TEST(strcmp(listGetNext(list),a[2]) == 0);
	ASSERT_TEST(listInsertAfterCurrent(list,a[1]) == LIST_SUCCESS);
	ASSERT_TEST(strcmp(listGetNext(list),a[1]) == 0);
	ASSERT_TEST(listInsertAfterCurrent(list,a[0]) == LIST_SUCCESS);
	ASSERT_TEST(strcmp(listGetNext(list),a[0]) == 0);
	List list2 = listCreate(copyString,freeString);
	for (int i=0;i <5; ++i){
		listInsertFirst(list2,a[i]);
	}
	ASSERT_TEST(strcmp(listGetFirst(list),listGetFirst(list2))==0);
	for(int i=0;i< 4 ;i++) {
		ASSERT_TEST(strcmp(listGetNext(list),listGetNext(list2))==0);
	}
	listDestroy(list);
	listDestroy(list2);
	return true;
}

static bool testListRemoveCurrent() {
	ASSERT_TEST(listRemoveCurrent(NULL) == LIST_NULL_ARGUMENT);
	char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
	List list2 = listCreate(copyString,freeString);
	for (int i=0;i<5; i++){
		listInsertLast(list2,a[i]);
	}
	ASSERT_TEST(strcmp(listGetFirst(list2),a[0])==0);
	ASSERT_TEST(listRemoveCurrent(list2) == LIST_SUCCESS);
	ASSERT_TEST(strcmp(listGetFirst(list2),a[1])==0);
	ASSERT_TEST(listGetSize(list2) == 4);
	ASSERT_TEST(strcmp(listGetNext(list2),a[2])==0);
	ASSERT_TEST(strcmp(listGetNext(list2),a[3])==0);
	ASSERT_TEST(strcmp(listGetNext(list2),a[4])==0);
	ASSERT_TEST(listRemoveCurrent(list2) == LIST_SUCCESS);
	ASSERT_TEST(strcmp(listGetFirst(list2),a[1])==0);
	ASSERT_TEST(strcmp(listGetNext(list2),a[2])==0);
	ASSERT_TEST(strcmp(listGetNext(list2),a[3])==0);
	ASSERT_TEST(listGetNext(list2) == NULL);
	ASSERT_TEST(listGetSize(list2) == 3);
	listDestroy(list2);
	return true;
}

static bool testListSort() {
	ASSERT_TEST(listSort(NULL,NULL) == LIST_NULL_ARGUMENT);
	char* a[5] = {"aa","AA","cow","X","BOLT"};
	List list = listCreate(copyString,freeString);
	for (int i=0;i<5; i++){
		listInsertLast(list,a[i]);
	}
	listSort(list,compareString);
	ASSERT_TEST(listGetSize(list) == 5);
	ASSERT_TEST(strcmp(listGetFirst(list),"AA") == 0);
	ASSERT_TEST(strcmp(listGetNext(list),"BOLT") == 0);
	ASSERT_TEST(strcmp(listGetNext(list),"X") == 0);
	ASSERT_TEST(strcmp(listGetNext(list),"aa") == 0);
	ASSERT_TEST(strcmp(listGetNext(list),"cow") == 0);
	listDestroy(list);
	return true;
}

static bool testListClear() {
	char* a[5] = {"aaa","bbb","NI","hello mister fish","I"};
	List list = listCreate(copyString,freeString);
	for (int i=0;i<5; i++){
		listInsertLast(list,a[i]);
	}
	ASSERT_TEST(listClear(NULL) == LIST_NULL_ARGUMENT);
	ASSERT_TEST(listClear(list) == LIST_SUCCESS);
	ASSERT_TEST(listGetNext(list) == NULL);
	ASSERT_TEST(listGetFirst(list) == NULL);
	ASSERT_TEST(listGetNext(list) == NULL);
	ASSERT_TEST(listGetSize(list) == 0);
	listDestroy(list);
	return true;
}

int main (){
	setvbuf(stdout,NULL, _IONBF,0);
	setvbuf(stderr,NULL, _IONBF,0);
	RUN_TEST(testListCreate1);
	RUN_TEST(testListCreate2);
	RUN_TEST(testListDestroy);
	RUN_TEST(testListCopy);
	RUN_TEST(testListFilter);
	RUN_TEST(testListGetSize);
	RUN_TEST(testListGetFirst1);
	RUN_TEST(testListGetFirst2);
	RUN_TEST(testListGetNext);
	RUN_TEST(testListGetCurrent);
	RUN_TEST(testListInsertFirst);
	RUN_TEST(testListInsertLast);
	RUN_TEST(testListInsertBeforeCurrent_1);
	RUN_TEST(testListInsertBeforeCurrent_2);
	RUN_TEST(testListInsertAfterCurrent_1);
	RUN_TEST(testListInsertAfterCurrent_2);
	RUN_TEST(testListRemoveCurrent);
	RUN_TEST(testListSort);
	RUN_TEST(testListClear);
	return 0;
}

