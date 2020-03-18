package create

//ListNode a singly-linked list node.
type ListNode struct {
	Val  int
	Next *ListNode
}

//List create a singly-linked list.
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
