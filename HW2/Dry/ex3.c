#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

typedef struct node_t* Node;
struct node_t {
	int n;
	Node next;
};

void printList(Node head) {
	while (head!=NULL) {
		printf("%d -> ",head->n);
		head = head->next;
	}
	printf("||\n");
}

Node addNode(int n,Node nextNode) {
	Node node = malloc(sizeof(*node));
	if (!node) {
		return NULL;
	}
	node->n = n;
	node->next = nextNode;
	return node;
}

int getNum(Node node) {
	return node->n;
}

void addRemainingNodes(Node* head, Node* joined) {
	while (*head != NULL) {
		(*joined)->next = addNode(getNum(*head),NULL);
		*joined = (*joined)->next;
		*head= (*head)->next;
	}
}

void progressToNextNode(Node* head1, Node* head2) {
	if (getNum(*head1) <= getNum(*head2)) {
		*head1 = (*head1)->next;
	} else {
		*head2 = (*head2)->next;
	}
}

int minValNode(Node node1, Node node2) {
	int a = getNum(node1);
	int b = getNum(node2);
	return (a<=b)?a:b;
}

Node joinSorted(Node head1, Node head2) {
	Node joined = NULL;
	Node joined_head = NULL;
	while (head1!=NULL && head2!=NULL) {
		Node newNode = addNode(minValNode(head1,head2),NULL);
		if (joined!=NULL) {
			joined->next = newNode;
			joined = newNode;
		} else {
			joined = newNode;
			joined_head = joined;
		}
		progressToNextNode(&head1,&head2);
	}
	addRemainingNodes((!head1?&head2:&head1),&joined);
	return joined_head;
}

void destroyList (Node ptr) {
    if (!ptr) {
        return;
    }
    destroyList (ptr->next);
    free (ptr);
}

Node copyList (Node node)	{
    if(!node)	{
    	return NULL;
    }
    Node copy= malloc(sizeof(*copy));
    copy->n=node->n;
    copy->next=copyList(node->next);
    return copy;
}
Node joinSortedArray (Node* array, int length)	{
	Node first = copyList(array[0]);
	Node joined_arr = NULL;
	for (int i=1 ; i<length ; i++)	{
		Node second = array[i];
		joined_arr = joinSorted(first, second);
		destroyList(first);
		first = joined_arr;
	}
	return joined_arr;
}

int main() {
	Node l1 = addNode(9,NULL);
	l1 = addNode(8,l1);
	l1 = addNode(7,l1);
	l1 = addNode(6,l1);
	Node head1 = l1;
	Node l2 = addNode(3,NULL);
	l2 = addNode(3,l2);
	l2 = addNode(2,l2);
	l2 = addNode(2,l2);
	l2 = addNode(1,l2);
	Node head2 = l2;
	Node l3 = addNode(5,NULL);
	l3 = addNode(4,l3);
	l3 = addNode(3,l3);
	l3 = addNode(2,l3);
	l3 = addNode(1,l3);
	Node head3 = l3;
	Node arr[3] = {head1, head2, head3};
	printList(arr[0]);
	printList(arr[1]);
	printList(arr[2]);
	Node joined = joinSortedArray(arr,3);
	printList(joined);
	destroyList(head1);
	destroyList(head2);
	destroyList(head3);
	destroyList(joined);
	return 0;
}
