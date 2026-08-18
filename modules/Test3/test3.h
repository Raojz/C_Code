#ifndef __TEST3_H__
#define __TEST3_H__


#include <stdio.h>
#include <stdlib.h>

typedef struct  DListNode {
	int data;
	struct DListNode* prev;
	struct DListNode* next;
}DListNode;

void test3_main_01(void);
void test3_main_02(void);
#endif