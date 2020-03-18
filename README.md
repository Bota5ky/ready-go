## Ready-Go

- 排序
  - 插入排序
    - [直接插入](sort/insertionsort.go)
    - [Shell排序](sort/shellsort.go)
  - 选择排序
    - [直接选择](sort/selectionsort.go)
    - [堆排序](sort/heapsort.go)
  - 交换排序
    - [冒泡排序](sort/bubblesort.go)
    - [快速排序](sort/quicksort.go)
  - [归并排序](sort/mergesort.go)
  - [基数排序](sort/radixsort.go)

# Time and space complexity of various sorting algorithms
<table align="center">
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
    <td align="center">O(n^2)</td>
    <td align="center">O(n)</td>
    <td align="center">O(n^2)</td>
    <td align="center">O(1)</td>
    <td align="center">稳定</td>
  </tr>
  <tr>
    <td align="center">Shell排序</td>
    <td align="center">O(n^1.3)</td>
    <td align="center">O(n)</td>
    <td align="center">O(n^2)</td>
    <td align="center">O(1)</td>
    <td align="center">不稳定</td>
  </tr>
  <tr>
    <td rowspan="2" align="center">选择排序</td>
    <td align="center">直接选择</td>
    <td align="center">O(n^2)</td>
    <td align="center">O(n^2)</td>
    <td align="center">O(n^2)</td>
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
    <td align="center">O(n^2)</td>
    <td align="center">O(n)</td>
    <td align="center">O(n^2)</td>
    <td align="center">O(1)</td>
    <td align="center">稳定</td>
  </tr>
  <tr>
    <td align="center">快速排序</td>
    <td align="center">O(nlogn)</td>
    <td align="center">O(nlogn)</td>
    <td align="center">O(n^2)</td>
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
</table>