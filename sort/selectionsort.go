package sort

// SelectionSort 直接选择排序 不稳定
// 时间复杂度：平均 O(n^2) 最好 O(n^2) 最坏 O(n^2) 
// 空间复杂度：O(1)
func SelectionSort(nums []int) []int {
	for i := 0; i < len(nums)-1; i++ {
		min := i
		for j := i + 1; j < len(nums); j++ {
			if nums[j] < nums[min] {
				min = j
			}
		}
		nums[min], nums[i] = nums[i], nums[min]
	}
	return nums
}
