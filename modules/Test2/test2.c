/* 2.1 单链表基本操作（基础）
* *【问题描述】 * *
用C语言实现一个带头节点的单链表，支持头插法建表、尾插法建表、
按位置插入、按位置删除、按值查找、求长度、遍历打印和销毁操作。
*/

#include "test2.h"

ListNode* listCreate()//创建带头节点的空链表
{
	ListNode* head_node = (ListNode*)malloc(sizeof(ListNode));
	if (head_node == NULL)
	{
		printf("创建失败\n");
		return NULL;
	}
	head_node->next = NULL;
	head_node->data = 0;
	return head_node;
}

void listSize(ListNode* head)//求长度
{
	int list_line = 0;
	while (head != NULL)
	{
		list_line++;
		head = head->next;
	}
	printf("链表长度为%d\n", list_line);
}

ListNode* listInsertHead(ListNode* head, int val)//头插法:新插入的节点为头节点
{
	ListNode* head_node = (ListNode*)malloc(sizeof(ListNode));
	if (head_node == NULL)return;
	head_node->data = val;
	head_node->next = head;
	return head_node;
}

ListNode* listInsertTail(ListNode* head, int val)//尾插法
{
	ListNode* tail_node = (ListNode*)malloc(sizeof(ListNode));
	if (tail_node == NULL)return head;
	tail_node->data = val;
	tail_node->next = NULL;

	if (head == NULL)return tail_node;

	ListNode* cur = head;
	while (cur->next != NULL)//找到最后节点
	{
		cur = cur->next;
	}
	cur->next = tail_node;
	return head;
}

ListNode* listFind_del(ListNode* head, int val)//按值查找删除
{
	while ((head != NULL) && (head->data == val))//删除头节点
	{
		ListNode* temp = head;
		head = head->next;
		free(temp);
	}

	if (head == NULL)return NULL;

	ListNode* cur = head;//处理后续节点

	while (cur->next != NULL)//遍历链表
	{
		if (cur->next->data == val)//找到删除节点的前一个节点把它和删除节点的后一个节点相连
		{
			ListNode* temp = cur->next;//要删除的节点
			cur->next = cur->next->next;
			free(temp);
		}
		else
		{
			cur = cur->next;//更新节点指针
		}
	}
	return head;
}

void listPrint(ListNode* head)//遍历打印
{
	if (head == NULL)
	{
		printf("链表为空\n");
		return;
	}
	printf("链表数据为:");
	while (head != NULL)
	{
		printf(" %d", head->data);
		head = head->next;
	}
	printf("\n");
}

ListNode* listDestroy(ListNode* head)//销毁链表
{
	while (head != NULL)
	{
		ListNode* cur = head;
		head = cur->next;
		free(cur);
	}
	return NULL;
}

//函数里涉及到头节点的操作，函数必须要返回指针，告诉外部头节点变了
//不然调用函数也只是进行无效操作，test_list根本没变
void test2_main_01(void)
{
	ListNode* Test_list  = listCreate();

	Test_list = listInsertHead(Test_list, 15);
	Test_list = listInsertHead(Test_list, 99);
	Test_list = listInsertHead(Test_list, 54);
	Test_list = listInsertHead(Test_list, 74);

	listSize(Test_list);
	listPrint(Test_list);

	Test_list = listInsertTail(Test_list, 12);
	Test_list = listInsertTail(Test_list, 19);
	Test_list = listInsertTail(Test_list, 28);

	listSize(Test_list);
	listPrint(Test_list);

	Test_list = listFind_del(Test_list, 15);
	Test_list = listFind_del(Test_list, 12);

	listSize(Test_list);
	listPrint(Test_list);

	Test_list = listDestroy(Test_list);
}

/* 2.2 单链表反转（基础）
* *【问题描述】 * *
给定一个不带头节点的单链表头指针，将其反转并返回新的头指针。要求使用 * *迭代法 * *实现，空间复杂度 O(1)。

* *【实现要求】 * *
1. 实现函数 `ListNode* reverseList(ListNode * head)`。
2. 使用三指针法：`prev`、`curr`、`next`。
3. 不可使用递归（本题），不可借助数组。
*/

ListNode* reverseList(ListNode* head)
{
	ListNode* pre = NULL;
	ListNode* cur = head;
	ListNode* next = NULL;

	while (cur != NULL)
	{
		next = cur->next;//记好下个节点
		cur->next = pre;//把当前节点指向上个节点
		pre = cur;//指针指向下个节点
		cur = next;//指向下个节点再次循环
	}
	return pre;//新的头节点
}

void test2_main_02()
{
	ListNode* list = NULL;
	list = listInsertHead(list, 157);
	list = listInsertHead(list, 14);
	list = listInsertHead(list, 52);
	list = listInsertHead(list, 98);
	list = listInsertHead(list, 74);

	listPrint(list);

	list = reverseList(list);
	listPrint(list);
	list = listDestroy(list);
}