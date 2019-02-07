#include <stdio.h>
#include <stdlib.h>
#include "list_mtm1.h"

typedef struct node {
	ListElement data;
	struct node* next;
} *Node;

struct List_t {
	Node head;
	Node iterator;
	CopyListElement copyElement;
	FreeListElement freeElement;
};

/*node functions*/
static Node listGetPrev(List list);
static void destroyNode(List list, Node node);
static Node getPosition (List list, Node node);

//creates a new node
static Node createNode(List list, ListElement data, Node next){
	if (!data || !list){
		return NULL;
	}
	Node newNode=malloc(sizeof(*newNode));
	if (!newNode){
		return NULL;
	}
	newNode->data=list->copyElement(data);
	if (!(newNode->data)){
		destroyNode(list,newNode);
		return NULL;
	}
	newNode->next=next;
	return newNode;
}

static void destroyNode(List list, Node node){
	if (!node){
		return;
	}
	list->freeElement(node->data);
	free(node);
}
/*end node fuctions*/

List listCreate(CopyListElement copyElement, FreeListElement freeElement){
	if (!copyElement || !freeElement) {
		return NULL;
	}
	List list = malloc(sizeof(*list));
	if(!list) {
		return NULL;
	}
	list->head = NULL;
	list->iterator = NULL;
	list->copyElement = copyElement;
	list->freeElement = freeElement;
	return list;

}
/*more static functions*/
static Node listGetPrev(List list){
	if (!list || !(list->iterator)){
		return NULL;
	}
	Node prev=list->head;
	if (!(list->head)){
		return NULL;
	}
	while (prev->next != NULL){
		if (prev == list->iterator){
			return NULL;
		}
		if (prev->next == list->iterator){
			return prev;
		}
		prev = prev->next;
	}
	return NULL;
}


static Node getPosition (List list, Node node){
	Node current = list->head;
	while (current)
	{
		if (current == node) {
			break;
		}
		current=current->next;
	}
	return current;
}

List listCopy(List list){
	if(!list) {
		return NULL;
	}
	List newList=listCreate(list->copyElement,list->freeElement);
	if(!newList) {
		return NULL;
	}
	int size = listGetSize(list);
	listGetFirst(list);
	Node mark = list->iterator;
	for (int i=0;i < size; i++){
		if(listInsertLast(newList,list->iterator->data)!=LIST_SUCCESS) {
			listDestroy(newList);
			return NULL;
		}
		listGetNext(list);
	}
	list->iterator=mark;
	newList->iterator=getPosition(newList, mark);
	return newList;
}

int listGetSize(List list){
	if(!list){
		return -1;
	}
	Node curr=list->iterator;
	int i=0;
	LIST_FOREACH(ListElement,iterator,list){
		i++;
	}
	list->iterator = curr;
	return i;
}

List listFilter(List list, FilterListElement filterElement, ListFilterKey key){
	if(!list) {
		return NULL;
	}
	List newList=listCreate(list->copyElement,list->freeElement);
	if(!newList) {
		return NULL;
	}
	Node current= list->head;
	while(current){
		if (filterElement(current->data,key)){
			if(listInsertLast(newList,current->data)!=LIST_SUCCESS) {
				listDestroy(newList);
				return NULL;
			}
		}
		current = current->next;
	}
	newList->iterator = getPosition(newList,getPosition(list,list->head));
	return newList;
}

ListElement listGetFirst(List list){
	if (!list) {
		return NULL;
	}
	list->iterator=list->head;
	if (!(list->head)) {// || list->iterator==NULL){
		return NULL;
	}
	return list->iterator->data;
}

ListElement listGetNext(List list){
	if (!list || !(list->iterator) || !(list->iterator->next)){
		return NULL;
	}
	list->iterator=list->iterator->next;
	return list->iterator->data;
}

ListElement listGetCurrent(List list){
	if (!list || !(list->iterator)) {
		return NULL;
	}
	return list->iterator->data;
}

ListResult listInsertFirst(List list, ListElement element){
	if (!list){
		return LIST_NULL_ARGUMENT;
	}
	Node curr = list->iterator;
	Node new_head = createNode(list, element, list->head);
	if (!new_head) {
		return LIST_OUT_OF_MEMORY;
	}
	//	if (list->head==NULL){
	//		list->head=new_head;
	//		list->iterator=curr;
	//		return LIST_SUCCESS;
	//	}
	new_head->next = list->head;
	list->head = new_head;
	list->iterator = curr;
	return LIST_SUCCESS;
}

ListResult listInsertLast(List list, ListElement element){
	if (!list) {
		return LIST_NULL_ARGUMENT;
	}
	Node curr = list->iterator;
	Node new_last_node = createNode(list, element, NULL);
	if (!new_last_node) {
		destroyNode(list, new_last_node);
		return LIST_OUT_OF_MEMORY;
	}
	if(!(list->head)) {
		list->head = new_last_node;
		list->iterator = curr;
		return LIST_SUCCESS;
	}
	else {
		list->iterator = list->head;
		while (!(list->iterator)) {
			if(!(list->iterator->next)){
				list->iterator->next = new_last_node;
				break;
			}
			list->iterator = list->iterator->next;
		}
	}
	list->iterator = curr;
	return LIST_SUCCESS;
}

ListResult listInsertBeforeCurrent(List list, ListElement element){
	if (!list) {
		return LIST_NULL_ARGUMENT;
	}
	Node curr = list->iterator;
	Node old_prev_node = listGetPrev(list);
	Node new_prev_node = createNode(list, element, curr);
	if (!new_prev_node){
		return LIST_OUT_OF_MEMORY;
	}
	if (!old_prev_node){
		listInsertFirst(list, element);
		destroyNode(list, new_prev_node);
		list->iterator = curr;
		return LIST_SUCCESS;
	}
	old_prev_node->next = new_prev_node;
	list->iterator = curr;
	return LIST_SUCCESS;
}

ListResult listInsertAfterCurrent(List list, ListElement element){
	if (!list) {
		return LIST_NULL_ARGUMENT;
	}
	Node curr = list->iterator;
	Node oldNext = listGetNext(list);
	list->iterator=curr;
	Node new_next_node = createNode(list, element, oldNext);
	if (!new_next_node){
		destroyNode(list, new_next_node);
		//destroyNode(list, oldNext);
		//destroyNode(list, curr);
		return LIST_OUT_OF_MEMORY;
	}
	if (!(list->iterator)) {
		destroyNode(list, new_next_node);
		//destroyNode(list, oldNext);
		//destroyNode(list, curr);
		return LIST_INVALID_CURRENT;
	}
	list->iterator = curr;
	list->iterator->next = new_next_node;
	return LIST_SUCCESS;
}

ListResult listRemoveCurrent(List list){
	if (!list){
		return LIST_NULL_ARGUMENT;
	}
	if(!(list->iterator)) {
		return LIST_INVALID_CURRENT;
	}
	Node to_delete = list->iterator;
	Node next = list->iterator->next;
	if (list->iterator==list->head){
		list->head=list->head->next;
		destroyNode(list, to_delete);
	} else {
		Node prev=listGetPrev(list);
		destroyNode(list, to_delete);
		prev->next=next;
	}
	list->iterator = NULL;
	return LIST_SUCCESS;
}

ListResult listSort(List list, CompareListElements compareElement){
	if (!list || !compareElement){
		return LIST_NULL_ARGUMENT;
	}
	Node curr = list->iterator;
	List sorted = listCreate(list->copyElement,list->freeElement);
	if (!sorted){
		destroyNode(list,curr);
		listDestroy(sorted);
		return LIST_OUT_OF_MEMORY;
	}
	int size = listGetSize(list);
	listGetFirst(list);
	sorted->iterator = sorted->head;
	for (int i=0; i<size; i++){
		while(!(sorted->iterator)){
			if (compareElement(sorted->iterator->data, list->iterator->data)<0){
				listInsertBeforeCurrent(sorted,list->iterator->data);
				break;
			} else {
				sorted->iterator=sorted->iterator->next;
			}
		}
		if(!(sorted->iterator)){
			listInsertLast(sorted, list->iterator->data);
		}
		sorted->iterator = sorted->head;
		listGetNext(list);
	}
	listClear(list);
	sorted->iterator = curr;
	list->head = sorted->head;
	list->iterator = sorted->iterator;
	sorted->copyElement = NULL;
	sorted->freeElement = NULL;
	free(sorted);
	return LIST_SUCCESS;
}


ListResult listClear(List list){
	if (!list){
		return LIST_NULL_ARGUMENT;
	}
	list->iterator = list->head;
	while(list->iterator) {
		Node next = list->iterator->next;
		listRemoveCurrent(list);
		list->iterator = next;
	}
	return LIST_SUCCESS;
}

void listDestroy(List list){
	if (!list){
		return;
	}
	listClear(list);
	free(list);
	list = NULL;
}

