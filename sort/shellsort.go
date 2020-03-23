package sort

// ShellSort 希尔排序 不稳定
// 时间复杂度：平均 O(n^1.3) 最好 O(n) 最坏 O(n^2)
// 空间复杂度：O(1)
func ShellSort(nums []int) {
	for k := len(nums) / 2; k > 0; k /= 2 {
		for i := k; i < len(nums); i++ {
			temp := nums[i] /* 取出未排序序列中的第k个元素*/
			var j int
			for j = i; j >= k && nums[j-k] > temp; j -= k { /* 注意界限 j >= k */
				nums[j] = nums[j-k] /*依次与已排序序列中元素比较并右移*/
			}
			nums[j] = temp /* 放进合适的位置 */
		}
	}
}
