#include "test3.h"

/*
### 2.4 双向链表实现（进阶）

* *【难度】 * *⭐⭐ 进阶

* *【问题描述】 * *
用 C 语言实现一个带头节点的双向链表，支持头插、尾插、按位置删除、按值查找、正向遍历和反向遍历操作。

* *【实现要求】 * *
- `DListNode* dlistCreate()` —— 创建带头节点的空双向链表
- `DListNode* dlistInsertHead(DListNode * head, int val)` —— 头插
- `DListNode* dlistInsertTail(DListNode * head, int val)` —— 尾插
- `DListNode* dlistErase(DListNode * head, int val)` —— 删除首个值为 val 的节点
- `DListNode* dlistFind(DListNode * head, int val)` —— 查找
- `void dlistPrintForward(DListNode * head)` —— 正向遍历
- `void dlistPrintBackward(DListNode * head)` —— 反向遍历
- `DListNode* dlistDestroy(DListNode * head)` —— 销毁
*/

DListNode* dlistCreate()//创建带头节点的双向循环链表
{
	DListNode* head = (DListNode*)malloc(sizeof(DListNode));
	head->data = 0;
	head->prev = head;
	head->next = head;
	return head;
}

DListNode* dlistInsertHead(DListNode* head, int val)//头插位置在头节点之后
{
	DListNode* new_node = (DListNode*)malloc(sizeof(DListNode));
	if (new_node == NULL)
	{
		printf("新节点创建失败\n");
		return head;
	}
	new_node->data = val;
	new_node->next = head->next;
	new_node->prev = head;
	head->next->prev = new_node;
	head->next = new_node;
	printf("头插入值为%d的新节点\n",val);
	return head;
}

DListNode* dlistInsertTail(DListNode* head, int val)//尾插
{
	DListNode* node = (DListNode*)malloc(sizeof(DListNode));
	if (node == NULL)
	{
		printf("新节点创建失败\n");
		return head;
	}
	node->data = val;
	node->next = head;
	node->prev = head->prev;
	head->prev->next = node;
	head->prev = node;
	printf("尾插入值为%d的新节点\n", val);
	return head;
}

DListNode* dlistErase(DListNode* head, int val)
{
	if (head == NULL)
	{
		printf("链表为空\n");
		return NULL;
	}

	// 情况1：删除头节点
	if (head->data == val)
	{
		DListNode* new_head = head->next;
		if (new_head != NULL)
		{
			new_head->prev = NULL;   // 新头节点的 prev 置空
		}
		free(head);                  // 释放旧头节点
		return new_head;             // 返回新头节点
	}

	// 情况2：在非头节点中查找
	DListNode* temp = head->next;    // 头节点已排除，从第二个开始
	while (temp != NULL && temp->data != val)  // ← 先判空，再访问 data
	{
		temp = temp->next;
	}

	// 未找到
	if (temp == NULL)
	{
		printf("链表中没找到该值节点\n");
		return head;
	}

	// 找到：摘除节点（temp 必非头节点，故 temp->prev 必非 NULL）
	temp->prev->next = temp->next;
	if (temp->next != NULL)          // ← 尾节点删除时需判空
	{
		temp->next->prev = temp->prev;
	}
	free(temp);
	printf("已删除值为%d的节点\n",val);
	return head;                     // 非头节点删除，头指针不变
}

DListNode* dlistFind(DListNode* head, int val)//查找
{
	if (head == NULL)
	{
		printf("链表为空\n");
		return NULL;
	}
	if (head->data == val)
	{
		return head;
	}
	DListNode* temp = head->next;    // 头节点已排除，从第二个开始
	while (temp != NULL && temp->data != val)  // ← 先判空，再访问 data
	{
		temp = temp->next;
	}
	if (temp == NULL)	// 未找到
	{
		printf("链表中没找到该值节点\n");
		return NULL;
	}
	return temp;
}
void dlistPrintForward(DListNode* head)//正向遍历
{
	if (head == NULL || head->next == head)
	{
		printf("链表为空\n");
		return;
	}
	printf("链表数据正向遍历为:\n");
	DListNode* cur = head->next;
	while (cur != head)
	{
		printf(" %d", cur->data);
		cur = cur->next;
	}
	printf("\n");
}
void dlistPrintBackward(DListNode* head)//反向遍历
{
	if (head == NULL || head->prev == head)
	{
		printf("链表为空\n");
		return;
	}
	printf("链表数据反向遍历为:\n");
	DListNode* cur = head->prev;
	while (cur != head)
	{
		printf(" %d", cur->data);
		cur = cur->prev;
	}
	printf("\n");
}
DListNode* dlistDestroy(DListNode* head)//销毁
{
	if (head == NULL)
	{
		printf("链表为空\n");
		return NULL;
	}
	DListNode* cur = head->next;
	while (cur != head)
	{
		DListNode* next = cur->next;
		free(cur);
		cur = next;
	}
	free(head);
	return NULL;
}

void test3_main_01(void)
{
	DListNode* Test_list = dlistCreate();
	Test_list = dlistInsertHead(Test_list, 147);
	Test_list = dlistInsertHead(Test_list, 25);
	Test_list = dlistInsertTail(Test_list, 96);
	Test_list = dlistInsertTail(Test_list, 58);
	Test_list = dlistInsertHead(Test_list, 35);

	dlistPrintForward(Test_list);
	Test_list = dlistErase(Test_list, 96);
	dlistPrintBackward(Test_list);

	Test_list = dlistInsertHead(Test_list, 87);
	Test_list = dlistInsertTail(Test_list, 103);
	dlistPrintForward(Test_list);
	dlistPrintBackward(Test_list);

	Test_list = dlistDestroy(Test_list);
}
void test3_main_02(void) {

}