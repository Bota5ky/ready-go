package sort

// RadixSort 基数排序 稳定 r代表基数 d代表长度
// 时间复杂度：平均 O(d(r+n)) 最好 O(d(n+rd)) 最坏 O(d(r+n))
// 空间复杂度：O(rd+n)
func RadixSort(nums []int) {
	MSD(nums)
}

//LSD Least Significant Digit 次位优先 	/* BucketSort */
func LSD(nums []int) {
	bucket := make([][]int, 10) // 0 ~ 9 尾数
	val := 1
	for len(bucket[0]) < len(nums) {
		bucket = make([][]int, 10)
		for _, v := range nums {
			last := (v / val) % 10
			bucket[last] = append(bucket[last], v)
		}
		val *= 10
		for i, k := 0, 0; i < 10; i++ {
			for j := 0; j < len(bucket[i]); j++ {
				nums[k] = bucket[i][j]
				k++
			}
		}
	}
}

//MSD Most Significant Digit 主位优先
func MSD(nums []int) {
	//先按其主位排好序 --> 然后对每一个主位桶再进行内部的次位优先基数排序 --> 最后统一收集
}
