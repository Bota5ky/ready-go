package create

//TreeNode Definition for a binary tree node.
type TreeNode struct {
	Val   int
	Left  *TreeNode
	Right *TreeNode
}

//Tree create binary tree nodes.
func Tree(nums []int) *TreeNode {
	//-1代表null节点
	node := make([]*TreeNode, len(nums))
	for i := 0; i < len(nums); i++ {
		if node[i].Val == -1 {
			node[i] = nil
		}
		node[i].Val = nums[i]
		node[i].Left = node[2*i+1]
		node[i].Right = node[2*i+2]
	}
	return node[0]
}
