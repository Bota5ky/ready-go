package sort

// InsertionSort 插入排序 稳定
// 时间复杂度：平均 O(n^2) 最好 O(n) 最坏 O(n^2) 
// 空间复杂度：O(1)
func InsertionSort(nums []int) []int {
	for i := 1; i < len(nums); i++ {
		temp := nums[i] /* 取出未排序序列中的第1个元素*/
		var j int
		for j = i; j > 0 && nums[j-1] > temp; j-- {
			nums[j] = nums[j-1] /*依次与已排序序列中元素比较并右移*/
		}
		nums[j] = temp /* 放进合适的位置 */
	}
	return nums
}
