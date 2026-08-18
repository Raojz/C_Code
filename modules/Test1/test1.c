//1.1 动态数组实现（基础）

/** 【问题描述】**
请用C语言实现一个支持动态扩容的整型数组结构，需支持尾部插入、指定位置插入、指定位置删除、
		按值查找和遍历打印操作。当数组容量不足时，应自动以2倍策略扩容。

* *【实现要求】 * *
1. 定义结构体 `DynamicArray`，包含数据指针 `data`、当前元素个数 `size`、当前容量 `capacity`。
2. 实现以下函数：
- `void arrayInit(DynamicArray * arr, int initCapacity)` —— 初始化
- `void arrayPushBack(DynamicArray * arr, int value)` —— 尾部插入
- `void arrayInsert(DynamicArray * arr, int index, int value)` —— 指定位置插入
- `void arrayErase(DynamicArray * arr, int index)` —— 删除指定位置元素
- `int arrayFind(DynamicArray * arr, int value)` —— 查找元素，返回下标，未找到返回 - 1
- `void arrayPrint(DynamicArray * arr)` —— 打印数组
- `void arrayFree(DynamicArray * arr)` —— 释放内存
3. 扩容使用 `realloc`，新容量为旧容量的 2 倍。

* *【约束条件】 * *
-初始容量 `initCapacity` ≥ 1。
- `index` 必须在 `[0, size]` 范围内（插入允许等于 `size`）。
- 删除时 `index` 必须在 `[0, size - 1]` 范围内。
- 所有动态分配的内存必须释放，禁止内存泄漏。
*/

#include "test1.h"

void arrayInit(DynamicArray * arr, int initCapacity)//初始化
{
	arr->data = (int*)malloc(sizeof(int) * initCapacity);
	if (arr->data == NULL)
	{
		printf("初始化失败\n");
		return;
	}
	arr->size = 0;
	arr->capacity = initCapacity;
}
/* 扩容：容量翻倍 */
static void arrayExpand(DynamicArray* arr) {
	int newCapacity = arr->capacity * 2;
	int* newData = (int*)realloc(arr->data, sizeof(int) * newCapacity);
	if (newData == NULL) {
		printf("realloc failed\n");
	}
	arr->data = newData;
	arr->capacity = newCapacity;
}
void arrayPushBack(DynamicArray* arr, int value)//尾部插入
{
	if (arr->size >= arr->capacity)
	{
		arrayExpand(arr);
	}
	arr->data[arr->size] = value;
	arr->size++;
}
void arrayInsert(DynamicArray* arr, int index, int value)//指定位置插入
														//index就是你要插入的下标位置
{
	if (index < 0 || index > arr->size) {
		printf("插入位置无效\n");
		return;
	}
	if (arr->size >= arr->capacity)//数组不够大
	{
		arrayExpand(arr);//扩容
	}

	for (int i = arr->size; i > index; i--) 
	{
		arr->data[i] = arr->data[i - 1];
	}
	arr->data[index] = value;
	arr->size++;
}
/* 删除指定位置元素 */
void arrayErase(DynamicArray* arr, int index) {
	if (index < 0 || index >= arr->size) {
		printf("位置信息错误\n");
		return;
	}
	/* 从前向后移动元素覆盖被删除位置 */
	for (int i = index; i < arr->size - 1; i++)//arr->size-1是数组下标的值 
	{
		arr->data[i] = arr->data[i + 1];
	}
	arr->size--;
}

/* 查找元素，返回下标，未找到返回 -1 */
int arrayFind(DynamicArray* arr, int value) {
	for (int i = 0; i < arr->size; i++) {
		if (arr->data[i] == value) {
			return i;
		}
	}
	return -1;
}

/* 打印数组 */
void arrayPrint(DynamicArray* arr) {
	printf("Array:");
	for (int i = 0; i < arr->size; i++) {
		printf(" %d", arr->data[i]);
	}
	printf("\n");
}

/* 释放内存 */
void arrayFree(DynamicArray* arr) 
{
	free(arr->data);
	arr->data = NULL;
	arr->size = 0;
	arr->capacity = 0;
}

void test1_main_01(void) {
	printf("动态数组演示\n");
	printf("****************************\n");
	DynamicArray arr;//结构体变量在栈上
	arrayInit(&arr, 2);  // 初始容量 2，测试扩容

	arrayPushBack(&arr, 1);
	arrayPushBack(&arr, 2);
	arrayPushBack(&arr, 3);  // 触发扩容
	arrayPushBack(&arr, 4);
	arrayPushBack(&arr, 5);
	arrayPrint(&arr);

	arrayInsert(&arr, 2, 99);  // 在下标 2 处插入 99
	arrayPrint(&arr);

	arrayErase(&arr, 3);       // 删除下标 3 的元素（值为 3）
	arrayPrint(&arr);

	printf("Find 99 at index: %d\n", arrayFind(&arr, 99));
	printf("Find 100 at index: %d\n", arrayFind(&arr, 100));

	arrayFree(&arr);
	printf("****************************\n");
}


/***********************************************************************************************/
/*## 1.2 数组原地反转（基础）

* *【实现要求】 * *
1. 实现函数 `void arrayReverse(int* arr, int size)`。
2. 使用双指针法：左指针从 0 开始，右指针从 `size-1` 开始，交换后向中间靠拢。
3. 不允许使用辅助数组。

* *【约束条件】 * *
-`size ≥ 0`；当 `size ≤ 1` 时无需操作。
- 时间复杂度 O(n)，空间复杂度 O(1)。
*/

void arrayReverse(int* arr, int size)
{
	if (size <= 1)return;
	int left = 0;
	int right = size - 1;
	int temp = 0;
	while (left < right)
	{
		temp = arr[right];
		arr[right] = arr[left];
		arr[left] = temp;
		left++;
		right--;
	}
	printf("翻转后数组数据为:");
	for (int i = 0; i < size; i++)
	{
		printf(" %d", arr[i]);
	}
	printf("\n");
}

void test1_main_02()
{
	printf("\n");
	printf("数组翻转演示\n");
	printf("****************************\n");
	int num[] = { 1,10,5,8,7,59,124,55 };
	printf("翻转前数组数据为:");
	for (int i = 0; i < sizeof(num)/sizeof(num[0]); i++)
	{
		printf(" %d", num[i]);
	}
	printf("\n");
	arrayReverse(num, sizeof(num)/sizeof(num[0]));
	printf("****************************\n");
}

/*### 3.1 顺序栈实现（基础）

* *【难度】 * *⭐ 基础

* *【问题描述】 * *
用 C 语言基于数组实现一个固定容量的整型顺序栈，支持入栈、出栈、取栈顶元素、判空、判满操作。
- `SeqStack* stackCreate(int capacity)`
- `int stackPush(SeqStack * s, int val)` —— 成功返回 1，满栈返回 0
- `int stackPop(SeqStack * s, int* val)` —— 成功返回 1，空栈返回 0
- `int stackTop(SeqStack * s, int* val)` —— 取栈顶但不弹出
- `int stackIsEmpty(SeqStack * s)`
- `int stackIsFull(SeqStack * s)`
- `void stackDestroy(SeqStack * s)`
* *【约束条件】 * *

-`capacity ≥ 1`。
- `top == 0` 表示空栈，`top == capacity` 表示满栈。
- 出栈 / 取栈顶时需先判空。

*/
SeqStack* stackCreate(int capacity)
{
	SeqStack* sta = (SeqStack*)malloc(sizeof(SeqStack));
	if (sta == NULL)
	{
		printf("栈创建失败\n");
		return NULL;
	}

	sta->data = (int*)malloc(sizeof(int) * capacity);
	if (sta->data == NULL)  // data 也要判空
	{
		printf("栈数据空间创建失败\n");
		free(sta);          // 记得释放已分配的 sta
		return NULL;
	}
	sta->top = 0;
	sta->capacity = capacity;
	return sta;
}
int stackIsEmpty(SeqStack* s) {
	return s->top == 0;
}

int stackIsFull(SeqStack* s) {
	return s->top == s->capacity;
}

void stackDestroy(SeqStack* s)
{
	if (s != NULL) {
		free(s->data);
		free(s);
	}
}
int stackPush(SeqStack* s, int val)//入栈成功返回 1，满栈返回 0
{
	if (s == NULL)
	{
		return -1;
	}
	if (stackIsFull(s))
	{
		printf("入栈失败栈已满\n");
		return 0;
	}
	s->data[s->top++] = val;
	return 1;
}

int stackPop(SeqStack* s, int* val)//出栈成功返回 1，空栈返回 0
{
	if (s == NULL)
	{
		return -1;
	}
	if (stackIsEmpty(s))
	{
		return 0;
	}
	*val = s->data[--s->top];  // top 先减，再取值
	return 1;
}
void test1_main_03()
{
	printf("\n");
	printf("数组栈演示\n");
	printf("****************************\n");
	SeqStack* test_sta = stackCreate(5);

	printf("栈内数据为:");
	for (int i = 1; i <= 5; i++)
	{
		stackPush(test_sta, i);
		printf(" %d", i);
	}
	printf("\n");

	printf("出栈数据为:");
	int val;
	while (stackPop(test_sta, &val))
	{
		printf(" %d", val);
	}
	printf("\n");

	stackDestroy(test_sta);
	test_sta = NULL;
	printf("****************************\n");
}

/*
### 循环队列实现（基础）

** 【问题描述】**
用 C 语言基于数组实现一个固定容量的循环队列，支持入队、出队、取队头、判空、判满操作。
需解决"队空与队满判断条件冲突"的问题。

* *【实现要求】 * *
2. 采用"浪费一个空间"的策略：
- 队空条件：`front == rear`
- 队满条件：`(rear + 1) % capacity == front`
3. 实现以下函数：
- `CircularQueue* cQueueCreate(int k)` —— 实际容量为 k，数组长度为 k + 1
- `int cQueueEnqueue(CircularQueue * q, int val)` —— 入队
- `int cQueueDequeue(CircularQueue * q, int* val)` —— 出队
- `int cQueueFront(CircularQueue * q, int* val)`
- `int cQueueIsEmpty(CircularQueue * q)`
- `int cQueueIsFull(CircularQueue * q)`
- `void cQueueFree(CircularQueue * q)`

* *【约束条件】 * *

-实际可存储 `k` 个元素，数组长度为 `k + 1`。
- 所有下标操作需取模。
- 入队判满，出队判空。
*/

CircularQueue* cQueueCreate(int k)//实际容量为 k，数组长度为 k + 1
{
	CircularQueue* que = (CircularQueue*)malloc(sizeof(CircularQueue));
	if (que == NULL)
	{
		printf("队列创建失败\n");
		return NULL;
	}
	que->data = (int*)malloc(sizeof(int) * (k+1));
	if (que->data == NULL)  // data 也要判空
	{
		printf("队列数据空间创建失败\n");
		free(que);
		return NULL;
	}
	que->front = que->rear = 0;
	que->capacity = k+1;
	return que;
}

int cQueueIsEmpty(CircularQueue* q)//队列是否为空
{
	if (q == NULL) {
		return -1;
	}
	if (q->front == q->rear)
	{
		return 1;
	}
	else {
		return 0;
	}
}
int cQueueIsFull(CircularQueue* q)//队列是否满
{
	if (q == NULL) {
		return -1;
	}
	if ((q->rear + 1) % q->capacity == q->front)
	{
		return 1;
	}
	else {
		return 0;
	}
}
int cQueueEnqueue(CircularQueue* q, int val)//入队
{
	if (q == NULL) {
		return -1;
	}
	if (cQueueIsFull(q))
	{
		return 0;
	}
	q->data[q->rear] = val;
	q->rear = (q->rear+1) % q->capacity;
	return 1;
}

int cQueueDequeue(CircularQueue* q, int* val)//出队
{
	if (q == NULL) {
		return -1;
	}
	if (cQueueIsEmpty(q))
	{
		return 0;
	}
	*val = q->data[q->front];
	q->front = (q->front + 1) % q->capacity;
	return 1;
}
void cQueueFree(CircularQueue* q)
{
	if (q != NULL)
	{
		free(q->data);
		free(q);
	}
}

void test1_main_04()
{
	printf("\n");
	printf("数组队列演示\n");
	printf("****************************\n");
	CircularQueue* test_q = cQueueCreate(5);

	printf("队列内数据为:");
	for (int i = 1; i <= 5; i++)
	{
		cQueueEnqueue(test_q, i);
		printf(" %d", i);
	}
	printf("\n");

	printf("出队数据为:");
	int val;
	while (cQueueDequeue(test_q, &val))
	{
		printf(" %d", val);
	}
	printf("\n");

	cQueueFree(test_q);
	test_q = NULL;
	printf("****************************\n");
}






