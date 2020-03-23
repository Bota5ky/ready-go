package sort

// HeapSort 堆排序 不稳定
// 时间复杂度：平均 O(nlogn) 最好 O(nlogn) 最坏 O(nlogn)
// 空间复杂度：O(1)
func HeapSort(nums []int) {
	for i := len(nums); i > 0; i-- {
		heapify(nums[:i])
		nums[0], nums[i-1] = nums[i-1], nums[0]
	}
}

func heapify(nums []int) {
	last := len(nums)/2 - 1 //最后一个非叶子节点
	for i := last; i >= 0; i-- {
		max := i
		left := 2*i + 1
		right := 2*i + 2
		if left < len(nums) && nums[left] > nums[max] {
			max = left
		}
		if right < len(nums) && nums[right] > nums[max] {
			max = right
		}
		nums[max], nums[i] = nums[i], nums[max]
	}
}
