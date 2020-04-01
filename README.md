# Ready-Go
## 一、排序
- 插入排序
  - [直接插入](sort/insertionsort.go)&emsp;&emsp;[页内跳转](#1-InsertionSort-插入排序)
  - [Shell排序](sort/shellsort.go)&emsp;&emsp;[页内跳转](#2-ShellSort-希尔排序)
- 选择排序
  - [直接选择](sort/selectionsort.go)&emsp;&emsp;[页内跳转](#3-SelectionSort-选择排序)
  - [堆排序](sort/heapsort.go)&emsp;&emsp;&emsp;[页内跳转](#4-HeapSort-堆排序)
- 交换排序
  - [冒泡排序](sort/bubblesort.go)&emsp;&emsp;[页内跳转](#5-BubbleSort-冒泡排序)
  - [快速排序](sort/quicksort.go)&emsp;&emsp;[页内跳转](#6-QuickSort-快速排序)
- [归并排序](sort/mergesort.go)&emsp;&emsp;&emsp;&emsp;[页内跳转](#7-MergeSort-归并排序)
- [基数排序](sort/radixsort.go)&emsp;&emsp;&emsp;&emsp;[页内跳转](#8-RadixSort-基数排序)
### Time and space complexity of various sorting algorithms
<center><table>
  <tr>
    <th rowspan="2" align="center">类别</th>
    <th rowspan="2" align="center">排序方法</th>
    <th colspan="3" align="center">时间复杂度</th>
    <th align="center">空间复杂度</th>
    <th rowspan="2" align="center">稳定性</th>
  </tr>
  <tr>
    <td align="center">平均情况</td>
    <td align="center">最好情况</td>
    <td align="center">最坏情况</td>
    <td align="center">辅助存储</td>
  </tr>
  <tr>
    <td rowspan="2" align="center">插入排序</td>
    <td align="center">直接插入</td>
    <td align="center">O(n<sup>2</sup>)</td>
    <td align="center">O(n)</td>
    <td align="center">O(n<sup>2</sup>)</td>
    <td align="center">O(1)</td>
    <td align="center">稳定</td>
  </tr>
  <tr>
    <td align="center">Shell排序</td>
    <td align="center">O(n<sup>1.3</sup>)</td>
    <td align="center">O(n)</td>
    <td align="center">O(n<sup>2</sup>)</td>
    <td align="center">O(1)</td>
    <td align="center">不稳定</td>
  </tr>
  <tr>
    <td rowspan="2" align="center">选择排序</td>
    <td align="center">直接选择</td>
    <td align="center">O(n<sup>2</sup>)</td>
    <td align="center">O(n<sup>2</sup>)</td>
    <td align="center">O(n<sup>2</sup>)</td>
    <td align="center">O(1)</td>
    <td align="center">不稳定</td>
  </tr>
  <tr>
    <td align="center">堆排序</td>
    <td align="center">O(nlogn)</td>
    <td align="center">O(nlogn)</td>
    <td align="center">O(nlogn)</td>
    <td align="center">O(1)</td>
    <td align="center">不稳定</td>
  </tr>
  <tr>
    <td rowspan="2" align="center">交换排序</td>
    <td align="center">冒泡排序</td>
    <td align="center">O(n<sup>2</sup>)</td>
    <td align="center">O(n)</td>
    <td align="center">O(n<sup>2</sup>)</td>
    <td align="center">O(1)</td>
    <td align="center">稳定</td>
  </tr>
  <tr>
    <td align="center">快速排序</td>
    <td align="center">O(nlogn)</td>
    <td align="center">O(nlogn)</td>
    <td align="center">O(n<sup>2</sup>)</td>
    <td align="center">O(nlogn)</td>
    <td align="center">不稳定</td>
  </tr>
  <tr>
    <td colspan="2" align="center">归并排序</td>
    <td align="center">O(nlogn)</td>
    <td align="center">O(nlogn)</td>
    <td align="center">O(nlogn)</td>
    <td align="center">O(n)</td>
    <td align="center">稳定</td>
  </tr>
  <tr>
    <td colspan="2" align="center">基数排序</td>
    <td align="center">O(d(r+n))</td>
    <td align="center">O(d(n+rd))</td>
    <td align="center">O(d(r+n))</td>
    <td align="center">O(rd+n)</td>
    <td align="center">稳定</td>
  </tr>
</table></center>

说明: 希尔排序根据不同的增量序列得到的复杂度分析也不同，这里取N//2。
### 1. InsertionSort 插入排序
```golang
// InsertionSort 插入排序 稳定
// 时间复杂度：平均 O(n^2) 最好 O(n) 最坏 O(n^2) 
// 空间复杂度：O(1)
func InsertionSort(nums []int) {
	for i := 1; i < len(nums); i++ {
		temp := nums[i] /* 取出未排序序列中的第1个元素*/
		var j int
		for j = i; j > 0 && nums[j-1] > temp; j-- {
			nums[j] = nums[j-1] /*依次与已排序序列中元素比较并右移*/
		}
		nums[j] = temp /* 放进合适的位置 */
	}
}
```
### 2. ShellSort 希尔排序
```golang
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
```
### 3. SelectionSort 选择排序
```golang
// SelectionSort 直接选择排序 不稳定
// 时间复杂度：平均 O(n^2) 最好 O(n^2) 最坏 O(n^2) 
// 空间复杂度：O(1)
func SelectionSort(nums []int) {
	for i := 0; i < len(nums)-1; i++ {
		min := i
		for j := i + 1; j < len(nums); j++ {
			if nums[j] < nums[min] {
				min = j
			}
		}
		nums[min], nums[i] = nums[i], nums[min]
	}
}
```
### 4. HeapSort 堆排序
```golang
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
```
### 5. BubbleSort 冒泡排序
```golang
// BubbleSort 冒泡排序 稳定
// 时间复杂度：平均 O(n^2) 最好 O(n) 最坏 O(n^2) 
// 空间复杂度：O(1)
func BubbleSort(nums []int) {
	for i := len(nums) - 1; i > 0; i-- {
		for j := 0; j < i; j++ {
			if nums[j] > nums[j+1] {
				nums[j], nums[j+1] = nums[j+1], nums[j]
			}
		}
	}
}
```
### 6. QuickSort 快速排序
```golang
// QuickSort 快速排序 不稳定
// 时间复杂度：平均 O(nlogn) 最好 O(nlogn) 最坏 O(n^2) 
// 空间复杂度：O(nlogn)
func QuickSort(nums []int) {
	if len(nums) < 2 {
		return
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
}
```
### 7. MergeSort 归并排序
```golang
// MergeSort 归并排序 稳定
// 时间复杂度：平均 O(nlogn) 最好 O(nlogn) 最坏 O(nlogn)
// 空间复杂度：O(n)
func MergeSort(nums []int) {
	temp := make([]int, len(nums))
	merge(nums, temp, 0, len(nums)-1)
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
```
### 8. RadixSort 基数排序
```golang
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
```
## 二、二叉树的迭代遍历
- [前序](https://github.com/Bota5ky/leetcode/blob/master/144.go)
- [中序](https://github.com/Bota5ky/leetcode/blob/master/94.go)
- [后序](https://github.com/Bota5ky/leetcode/blob/master/145.go)
- 模板
```golang
for cur!=nil || len(stack)>0 {
        if cur!=nil {
            ...
        }else{
            ...
        }
    }
```
## 三、位运算
1. 绝对值的位运算，以int32为例：`(var ^ (var >> 31)) - (var >> 31)`。
2. 交换两个数字：`a = a^b` `b = a^b` `a = a^b`
3. 最右边的1变为0：`n &= (n - 1)`
## 字符串
- 常用的字符串拼接方法：`+`,`fmt.Sprintf()`,`strings.Join()`,以下还有2种
```golang
s1 := "字符串"
s2 := "拼接"
//定义Buffer类型
var bt bytes.Buffer
//向bt中写入字符串
bt.WriteString(s1)
bt.WriteString(s2)
//获得拼接后的字符串
s3 := bt.String()
```
```golang
s1 := "字符串"
s2 := "拼接"
var build strings.Builder
build.WriteString(s1)
build.WriteString(s2)
s3 := build.String()
```

