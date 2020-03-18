package sort

// BubbleSort 冒泡排序 稳定
// 时间复杂度：平均 O(n^2) 最好 O(n) 最坏 O(n^2) 
// 空间复杂度：O(1)
func BubbleSort(nums []int) []int {
	for i := len(nums) - 1; i > 0; i-- {
		for j := 0; j < i; j++ {
			if nums[j] > nums[j+1] {
				nums[j], nums[j+1] = nums[j+1], nums[j]
			}
		}
	}
	return nums
}
