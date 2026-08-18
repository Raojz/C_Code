#ifndef __TEST2_H__
#define __TEST2_H__


#include <stdio.h>


typedef struct  ListNode {
	int data; //数据
	struct ListNode*next;
}ListNode;

void test2_main_01(void);
void test2_main_02(void);
#endif