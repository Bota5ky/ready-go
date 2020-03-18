# Time and space complexity of various sorting algorithms
<table>
  <tr>
    <th rowspan="2">类别</th>
    <th rowspan="2">排序方法</th>
    <th colspan="3">时间复杂度</th>
    <th>空间复杂度</th>
    <th rowspan="2">稳定性</th>
  </tr>
  <tr>
    <td>平均情况</td>
    <td>最好情况</td>
    <td>最坏情况</td>
    <td>辅助存储</td>
  </tr>
  <tr>
    <td rowspan="2">插入排序</td>
    <td>直接插入</td>
    <td>O(n^2)</td>
    <td>O(n)</td>
    <td>O(n^2)</td>
    <td>O(1)</td>
    <td>稳定</td>
  </tr>
  <tr>
    <td>Shell排序</td>
    <td>O(n^1.3)</td>
    <td>O(n)</td>
    <td>O(n^2)</td>
    <td>O(1)</td>
    <td>不稳定</td>
  </tr>
  <tr>
    <td rowspan="2">选择排序</td>
    <td>直接选择</td>
    <td>O(n^2)</td>
    <td>O(n^2)</td>
    <td>O(n^2)</td>
    <td>O(1)</td>
    <td>不稳定</td>
  </tr>
  <tr>
    <td>堆排序</td>
    <td>O(nlogn)</td>
    <td>O(nlogn)</td>
    <td>O(nlogn)</td>
    <td>O(1)</td>
    <td>不稳定</td>
  </tr>
  <tr>
    <td rowspan="2">交换排序</td>
    <td>冒泡排序</td>
    <td>O(n^2)</td>
    <td>O(n)</td>
    <td>O(n^2)</td>
    <td>O(1)</td>
    <td>稳定</td>
  </tr>
  <tr>
    <td>快速排序</td>
    <td>O(nlogn)</td>
    <td>O(nlogn)</td>
    <td>O(n^2)</td>
    <td>O(nlogn)</td>
    <td>不稳定</td>
  </tr>
  <tr>
    <td colspan="2">归并排序</td>
    <td>O(nlogn)</td>
    <td>O(nlogn)</td>
    <td>O(nlogn)</td>
    <td>O(n)</td>
    <td>稳定</td>
  </tr>
  <tr>
    <td colspan="2">基数排序</td>
    <td>O(d(r+n))</td>
    <td>O(d(n+rd))</td>
    <td>O(d(r+n))</td>
    <td>O(rd+n)</td>
    <td>稳定</td>
  </tr>
</table>