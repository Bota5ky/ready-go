package main

func main() {
	nums := []int{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}
	head := List(nums)
	newHead:=reveseAll(reveseK(reveseAll(head), 3))
	//newHead:=reveseK(head, 2)
	for newHead!=nil {
		print(newHead.Val,"->")
		newHead=newHead.Next
	}
}

func List(list []int) *ListNode {
	if len(list) == 0 {
		return nil
	}
	pre := &ListNode{}
	ret := pre
	for i := 0; i < len(list); i++ {
		node := &ListNode{list[i], nil}
		pre.Next = node
		pre = node
	}
	return ret.Next
}

func reveseK(head *ListNode, k int) *ListNode {
	temp := head
	for i := 0; i < k; i++ {
		if temp == nil {
			return head
		}
		temp = temp.Next
	}
	pre := reveseK(temp, k)
	rear := head.Next
	for i := 0; i < k; i++ {
		head.Next = pre
		pre = head
		head = rear
		if rear != nil {
			rear = rear.Next
		}
	}
	return pre
}

func reveseAll(head *ListNode) *ListNode {
	if head == nil || head.Next == nil {
		return head
	}
	var pre *ListNode
	rear := head.Next
	for head != nil {
		head.Next = pre
		pre = head
		head = rear
		if rear != nil {
			rear = rear.Next
		}
	}
	return pre
}

//ListNode 链表
type ListNode struct {
	Val  int
	Next *ListNode
}

func reveseList(p *ListNode) *ListNode {
	if p == nil || p.Next == nil {
		return p
	}
	var pre *ListNode
	rear := p.Next
	for p != nil {
		p.Next = pre
		pre = p
		p = rear
		if rear != nil {
			rear = rear.Next
		}
	}
	return pre
}

// 记一道字节跳动的算法面试题
// 题目
// 这其实是一道变形的链表反转题，大致描述如下 给定一个单链表的头节点 head,实现一个调整单链表的函数，
//使得每K个节点之间为一组进行逆序，并且从链表的尾部开始组起，头部剩余节点数量不够一组的不需要逆序。
//（不能使用队列或者栈作为辅助）

// 例如：

// 链表:1->2->3->4->5->6->7->8->null, K = 3。那么 6->7->8，3->4->5，1->2各位一组。调整后：1->2->5->4->3->8->7->6->null。其中 1，2不调整，因为不够一组。

// 解析

// 原文： https://juejin.im/post/5d4f76325188253b49244dd0

// 解析： 先全翻转，再每K段翻转，再全翻转