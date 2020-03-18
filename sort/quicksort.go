package sort

// QuickSort 快速排序 不稳定
// 时间复杂度：平均 O(nlogn) 最好 O(nlogn) 最坏 O(n^2) 
// 空间复杂度：O(nlogn)
func QuickSort(nums []int) []int {
	if len(nums) < 2 {
		return nums
	}
	pivot := nums[0]
	i, j := 0, len(nums)-1
	for i < j {
		//基准pivot设置为最左边，就从最右边开始
		for ; i < j && nums[j] >= pivot; j-- {
		}
		nums[i] = nums[j]
		for ; i < j && nums[i] <= pivot; i++ {
		}
		nums[j] = nums[i]
	}
	nums[i] = pivot
	QuickSort(nums[:i])
	QuickSort(nums[i+1:])
	return nums
}
