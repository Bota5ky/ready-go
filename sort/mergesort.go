package sort

// MergeSort 归并排序 稳定
// 时间复杂度：平均 O(nlogn) 最好 O(nlogn) 最坏 O(nlogn)
// 空间复杂度：O(n)
func MergeSort(nums []int) []int {
	temp := make([]int, len(nums))
	merge(nums, temp, 0, len(nums)-1)
	return nums
}

func merge(nums, temp []int, l, r int) {
	if l >= r {
		return
	}
	mid := (l + r) / 2
	merge(nums, temp, l, mid)
	merge(nums, temp, mid+1, r)
	i, j := l, mid+1 //  l <= i <=mid     mid < j <=r
	for k := l; k <= r; k++ {
		temp[k] = nums[k]
	}
	for k := l; k <= r; k++ {
		if i > mid || j <= r && temp[j] < temp[i] {
			nums[k] = temp[j]
			j++
		} else {
			nums[k] = temp[i]
			i++
		}
	}
}
