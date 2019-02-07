#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct temp {
	int a;
	int b;
} something;

void swap(struct temp* s) {
	int temp = s->a;
	s->a = s->b;
	s->b = temp;
}
void swapIfFirstNegative(struct temp* array, struct temp* resultArray,
												int arrayLength, int* num) {
	resultArray = malloc(sizeof(array));
	int n = 0;
	for (int i=0;i<arrayLength;i++,resultArray++,array++) {
		*resultArray = *array;
				if (resultArray->a < 0) {
					swap(resultArray);
					n++;
				}
	}
	*num = n;
}

void swapIfSecondNegative(struct temp* array, struct temp* resultArray,
												int arrayLength, int* num) {
	resultArray = malloc(sizeof(array));
	int n = 0;
	for (int i=0;i<arrayLength;i++) {
				if (resultArray->b < 0) {
					swap(resultArray+i);
					n++;
				}
	}
	*num = n;
}

typedef struct node_t* Node;
struct node_t {
	int data;
	Node next;
};


void print_reverse_aux(Node s,Node f) {
	if (f == s) {
		printf("%d ",f->data);
		return;
	}
	print_reverse_aux(s,f->next);
	printf("%d ",f->data);
}
void print_reverse(Node s) {
	print_reverse_aux(s,s->next);
}

int main() {
	something a = { 2,3 };
	something b = { -2,5 };
	something c = { -2,3 };
	something d = { 2,-3 };
	something *arr = malloc(sizeof(something)*4);
	arr[0] = a;
	arr[1] = b;
	arr[2] = c;
	arr[3] = d;
	printf("\n**************************************\n");
	for (int i=0;i<4;i++)
		printf("a=%d b=%d \n",arr[i].a,arr[i].b);
	printf("\n**************************************\n");
	int num=0;
	something *resultArray = NULL;
	swapIfFirstNegative(arr,resultArray,4,&num);
//		printf("\n**************************************\n");
//		for (int i=0;i<4;i++)
//			printf("a=%d b=%d \n",resultArray[i].a,resultArray[i].b);
//		printf("\n**************************************\n");


//	Node head = NULL;
//	Node fixedHead = NULL;
//	Node current;
//
//	current = malloc(sizeof(Node));
//	current->data = 15;
//	current->next  = head;
//	head = current;
//	fixedHead = current;
//
//	for(int i=1;i<=3;i++) {
//		current = malloc(sizeof(Node));
//		current->data = i;
//		current->next  = head;
//	    head = current;
//	   }
//	fixedHead->next = head;
//
//	while (current != fixedHead) {
//		printf("%d -> ",current->data);
//		current = current->next;
//	}
//
//	printf("%d -> %d",fixedHead->data, fixedHead->next->data);
//	printf("\n Head Node: %d",fixedHead->data);
//
//	printf("\n******************\n");
//	print_reverse(fixedHead);
	return 0;
}
