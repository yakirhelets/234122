

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "list_mtm1.h"



typedef struct node_t* Node;


struct node_t {
ListElement data;
Node next;
};




struct List_t {

	Node first;
	Node iterator;
	CopyListElement copyElement;
	FreeListElement freeElement;

};


//________________________________________________________________
//node functions

static void destroyNode(FreeListElement freeElement, Node node) {
	if (!node){
		return;
	}
	freeElement(node->data);
	free (node);
}


static Node createNode(Node next, ListElement data){

	Node node=malloc(sizeof(*node));
	if (!node){
		return NULL;
	}
	node->next = next;
	node->data = data;
	return node;
}

static Node listGetLast(List list) {

	if (!list){
		return NULL;
	}
	if (!list->first){
		return NULL;
	}
	Node index = list->first;
	while(index->next){
		index = index->next;
	}
	return index;
}






//___________________________________________________________________
// list functions




//1
List listCreate(CopyListElement copyElement, FreeListElement freeElement){

	if( !copyElement  || !freeElement){
		return NULL;
	}
	List newList=malloc(sizeof(*newList));
	if (!newList){
		return NULL;
	}
	newList->first = NULL;
	newList->iterator = NULL;
	newList->freeElement=freeElement;
	newList->copyElement=copyElement;
	return newList;
}


//2
void listDestroy(List list){
	if (!list){
		return;
	}
	listClear(list);
	free(list);
	list = NULL;
}


//3
List listCopy(List list){
	if(!list) {
		return NULL;
	}
	List copy=listCreate(list->copyElement, list->freeElement);
	if (!copy){
		return NULL;
	}
	Node temp=list->iterator;
	LIST_FOREACH(ListElement,iterator,list){
		if(listInsertLast(copy,iterator)!=LIST_SUCCESS) {
			listDestroy(copy);
			return NULL;
		}
	}
	list->iterator=temp;
	copy->iterator=temp;
	return copy;
}


//4
List listFilter(List list, FilterListElement filterElement, ListFilterKey key) {

	if (!list || !filterElement) {
		return NULL;
	}
	List filteredList = listCreate(list->copyElement,list->freeElement);
	if (!filteredList){
		return NULL;
	}
	Node temp=list->iterator;
	LIST_FOREACH(ListElement,iterator,list){
		if(filterElement(iterator,key)){
			if(listInsertLast(filteredList,iterator)==LIST_OUT_OF_MEMORY){
				listDestroy(filteredList);
				return NULL;
			}
		}
	}
	list->iterator=temp;
	filteredList->iterator=filteredList->first;
	return filteredList;
}


//5
int listGetSize(List list){
	if(!list){
		return -1;
	}
	int size=0;
	Node iterator=list->first;
	while (iterator){
		size++;
		iterator=iterator->next;
	}
	return size;
}


//6
ListElement listGetFirst(List list){

	if (!list){
		return NULL;
	}
	if (!list->first){
		return NULL;
	}
	list->iterator=list->first;
	return list->first->data;
}


//7
ListElement listGetNext(List list){
	if (!list || !list->iterator || !list->iterator->next){
		return NULL;
	}
	list->iterator=list->iterator->next;
	return list->iterator->data;
}


//8
ListElement listGetCurrent(List list){
	if (!list || !list->iterator){
		return NULL;
	}
	return list->iterator->data;
}


//9
ListResult listInsertFirst(List list, ListElement element){
	if (!list ){
		return LIST_NULL_ARGUMENT;
	}
	ListElement copy = list->copyElement(element);
	if (!copy){
		return LIST_OUT_OF_MEMORY;
	}
	Node node=createNode(list->first,copy);
	if (!node){
		return LIST_OUT_OF_MEMORY;
	}
	list->first=node;
	return LIST_SUCCESS;
}


//10
ListResult listInsertLast(List list, ListElement element){
	if (!list){
		return LIST_NULL_ARGUMENT;
	}
	ListElement copy = list->copyElement(element);
	if (!copy){
		return LIST_OUT_OF_MEMORY;
	}
	Node last = listGetLast(list);
	if (!last) {
		last = createNode(NULL,copy);
		list->first = last;
		return LIST_SUCCESS;
	}
	Node node = createNode(NULL, copy);
	if(!node){
		return LIST_OUT_OF_MEMORY;
	}
	last->next = node;
	return LIST_SUCCESS;
}


//11
ListResult listInsertBeforeCurrent(List list, ListElement element){
	if (!list){
		return LIST_NULL_ARGUMENT;
	}
	if (!list->iterator){
		return LIST_INVALID_CURRENT;
	}
	ListElement copy = list->copyElement(element);
	if (!copy){
		return LIST_OUT_OF_MEMORY;
	}
	if(list->iterator == list->first){
		return listInsertFirst(list,element);
	}
	Node place=list->iterator;
	list->iterator=list->first;
	while(list->iterator->next != place){
		list->iterator=list->iterator->next;
	}
	Node node = createNode(place,copy);
	if(!node){
		return LIST_OUT_OF_MEMORY;
	}
	list->iterator->next = node;
	list->iterator = place;
	return LIST_SUCCESS;
}



//12
ListResult listInsertAfterCurrent(List list, ListElement element){
	if (!list){
			return LIST_NULL_ARGUMENT;
		}
		if (!list->iterator){
			return LIST_INVALID_CURRENT;
		}
		Node place = list->iterator;
		if(list->iterator == listGetLast(list)){
			return listInsertLast(list,element);
		}
		ListElement copy = list->copyElement(element);
		if (!copy){
			return LIST_OUT_OF_MEMORY;
		}
		Node node = createNode(list->iterator->next,copy);
		if (!node){
			return LIST_OUT_OF_MEMORY;
		}
		list->iterator->next = node;
		list->iterator = place;
		return LIST_SUCCESS;
}


//13
ListResult listRemoveCurrent(List list) {
	if (!list){
		return LIST_NULL_ARGUMENT;
	}
	if (!list->iterator) {
		return LIST_INVALID_CURRENT;
	}
	Node place=list->first;
	if (list->iterator==list->first){
		if(!list->iterator->next){
			destroyNode(list->freeElement,list->iterator);
			list->first=NULL;
			return LIST_SUCCESS;
		}
		list->first=list->first->next;
		destroyNode(list->freeElement,list->iterator);
		list->iterator = NULL;
		return LIST_SUCCESS;
	}
	while(place->next != list->iterator){
		place=place->next;
	}
	if (!list->iterator->next){
		place->next=NULL;
		destroyNode(list->freeElement,list->iterator);
		list->iterator = NULL;
		return LIST_SUCCESS;
	}
	place->next=list->iterator->next;
	destroyNode(list->freeElement,list->iterator);
	list->iterator = NULL;
	return LIST_SUCCESS;
}


//14
ListResult listSort(List list, CompareListElements compareElement){
	if(!compareElement || !list) {
		return LIST_NULL_ARGUMENT;
	}
	int size = listGetSize(list);
	int iteratorPlace=0;
	while (list->iterator!=NULL && iteratorPlace < size && list->iterator != list->first ){
		iteratorPlace++;
		list->iterator = list->iterator->next;
	}
	bool swap = true;
	Node before = NULL;
	list->iterator = list->first;
	while (swap){
		swap = false;
		for(int i=0;i<size-1;i++) {
			int cmp = compareElement(list->iterator->data,list->iterator->next->data);
			if (cmp < 0){
				Node tmp = list->iterator->next;
				list->iterator->next = list->iterator->next->next;
				tmp->next = list->iterator;
				if(before){
					before->next = tmp;
				}else {
					before = tmp;
					list->first = tmp;
				}
				swap = true;
			} else {
				before = list->iterator;
				list->iterator = list->iterator->next;
			}
		}
		list->iterator = list->first;
	}
	for(int i=0;i<iteratorPlace;i++){
		list->iterator = list->iterator->next;
	}
	return LIST_SUCCESS;
}



//15
ListResult listClear(List list){
	if (!list){
		return LIST_NULL_ARGUMENT;
	}
	list->iterator = list->first;
	ListResult result;
	while(list->iterator) {
		Node next = list->iterator->next;
		result = listRemoveCurrent(list);
		list->iterator = next;
	}
	return result;
}












