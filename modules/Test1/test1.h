#ifndef __TEST1_H__
#define __TEST1_H__


#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int size;//当前元素个数
	int* data; //数据指针
	int capacity;//当前容量
}DynamicArray;

typedef struct {
	int* data;
	int top;       /* 栈顶指针，指向下一个可写入位置 */
	int capacity;
} SeqStack;

typedef struct {
	int* data;
	int front;     /* 队头下标 */
	int rear;      /* 队尾下一写入位置 */
	int capacity;  /* 数组实际长度（含一个预留空位） */
} CircularQueue;


void test1_main_01(void);
void test1_main_02(void);
void test1_main_03(void);
void test1_main_04(void);
#endif