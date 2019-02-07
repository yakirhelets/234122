#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "list_mtm.h"

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

static void destroyNode(FreeListElement freeElement, Node node) {
	if (!node || !freeElement) return;
	freeElement(node->data);
	free(node);
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

List listCreate(CopyListElement copyElement, FreeListElement freeElement){
	if( !copyElement  || !freeElement){
		return NULL;
	}
	List new_list=malloc(sizeof(*new_list));
	if (!new_list){
		return NULL;
	}
	new_list->first = NULL;
	new_list->iterator = NULL;
	new_list->freeElement=freeElement;
	new_list->copyElement=copyElement;
	return new_list;
}

void listDestroy(List list){
	if (!list){
		return;
	}
	listClear(list);
	free(list);
}

List listCopy(List list){
	if(!list) {
		return NULL;
	}
	List copy=listCreate(list->copyElement, list->freeElement);
	if (!copy){
		return NULL;
	}
	Node temp_node=list->iterator;
	LIST_FOREACH(ListElement,iterator,list){
		if(listInsertLast(copy,iterator)!=LIST_SUCCESS) {
			listDestroy(copy);
			return NULL;
		}
	}
	list->iterator=temp_node;
	copy->iterator=temp_node;
	return copy;
}

List listFilter(List list, FilterListElement filterElement, ListFilterKey key) {

	if (!list || !filterElement) {
		return NULL;
	}
	List filtered_list = listCreate(list->copyElement,list->freeElement);
	if (!filtered_list){
		return NULL;
	}
	Node temp_node=list->iterator;
	LIST_FOREACH(ListElement,iterator,list){
		if(filterElement(iterator,key)){
			if(listInsertLast(filtered_list,iterator)!=LIST_SUCCESS){
				listDestroy(filtered_list);
				return NULL;
			}
		}
	}
	list->iterator=temp_node;
	filtered_list->iterator=filtered_list->first;
	return filtered_list;
}

int listGetSize(List list){
	if(!list){
		return -1;
	}
	int size_count = 0;
	Node iterator=list->first;
	while (iterator){
		size_count++;
		iterator=iterator->next;
	}
	return size_count;
}

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

ListElement listGetNext(List list){
	if (!list || !list->iterator) {
		return NULL;
	}
	if (!(list->iterator->next)) {
		list->iterator = NULL;
		return NULL;
	}
	list->iterator=list->iterator->next;
	return list->iterator->data;
}

ListElement listGetCurrent(List list){
	if (!list || !list->iterator){
		return NULL;
	}
	return list->iterator->data;
}

ListResult listInsertFirst(List list, ListElement element){
	if (!list ) {
		return LIST_NULL_ARGUMENT;
	}
	ListElement copy = list->copyElement(element);
	if (!copy) {
		return LIST_OUT_OF_MEMORY;
	}
	Node node=createNode(list->first,copy);
	if (!node) {
		list->freeElement(copy);
		return LIST_OUT_OF_MEMORY;
	}
	list->first=node;
	return LIST_SUCCESS;
}

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
		list->freeElement(copy);
		return LIST_OUT_OF_MEMORY;
	}
	last->next = node;
	return LIST_SUCCESS;
}

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
	Node position=list->iterator;
	list->iterator=list->first;
	while(list->iterator->next != position){
		list->iterator=list->iterator->next;
	}
	Node node = createNode(position,copy);
	if(!node){
		list->freeElement(copy);
		return LIST_OUT_OF_MEMORY;
	}
	list->iterator->next = node;
	list->iterator = position;
	return LIST_SUCCESS;
}

ListResult listInsertAfterCurrent(List list, ListElement element){
	if (!list){
		return LIST_NULL_ARGUMENT;
	}
	if (!list->iterator){
		return LIST_INVALID_CURRENT;
	}
	Node position = list->iterator;
	if(list->iterator == listGetLast(list)){
		return listInsertLast(list,element);
	}
	ListElement copy = list->copyElement(element);
	if (!copy){
		return LIST_OUT_OF_MEMORY;
	}
	Node node = createNode(list->iterator->next,copy);
	if (!node){
		list->freeElement(copy);
		return LIST_OUT_OF_MEMORY;
	}
	list->iterator->next = node;
	list->iterator = position;
	return LIST_SUCCESS;
}

ListResult listRemoveCurrent(List list) {
	if (!list){
		return LIST_NULL_ARGUMENT;
	}
	if (!list->iterator) {
		return LIST_INVALID_CURRENT;
	}
	Node position=list->first;
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
	while(position->next != list->iterator){
		position=position->next;
	}
	if (!list->iterator->next){
		position->next=NULL;
		destroyNode(list->freeElement,list->iterator);
		list->iterator = NULL;
		return LIST_SUCCESS;
	}
	position->next=list->iterator->next;
	destroyNode(list->freeElement,list->iterator);
	list->iterator = NULL;
	return LIST_SUCCESS;
}

ListResult listSort(List list, CompareListElements compareElement) {
	if(!compareElement || !list) {
		return LIST_NULL_ARGUMENT;
	}
	int list_size = listGetSize(list);
	int iterator_pos = 0;
	while (list->iterator!=NULL && iterator_pos < list_size && list->iterator != list->first ) {
		iterator_pos++;
		list->iterator = list->iterator->next;
	}
	bool done = false;
	while (!done) {
		list->iterator = list->first;
		done = true;
		for (int k=0;k<list_size-1;k++) {
			if (compareElement(list->iterator->data,list->iterator->next->data) > 0) {
				// nodes data-swap
				ListElement copy = list->copyElement(list->iterator->data);
				if (!copy){
					return LIST_OUT_OF_MEMORY;
				}
				Node temp_node = createNode(NULL,copy);
				if (!temp_node) {
					list->freeElement(copy);
					return LIST_OUT_OF_MEMORY;
				}
				list->iterator->data = list->iterator->next->data;
				list->iterator->next->data = temp_node->data;
				free(temp_node);
				done = false;
			}
			list->iterator = list->iterator->next;
		}
	}
	list->iterator = list->first;
	for(int k=0;k<iterator_pos;k++) { // return the iterator to its the original position
		list->iterator = list->iterator->next;
	}
	return LIST_SUCCESS;
}

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
