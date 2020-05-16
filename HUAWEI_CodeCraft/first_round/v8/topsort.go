package main

import (
	"fmt"
	"time"
)

//topSort 删除入度为0的点
func topSort(m map[int][]int, inDegree map[int]int) {
	startTime := time.Now().UnixNano()
	var stack []int
	for num := range m {
		if inDegree[num] == 0 {
			stack = append(stack, num)
		}
	}
	for len(stack) > 0 {
		var temp []int
		for i := 0; i < len(stack); i++ {
			list := m[stack[i]]
			for j := 0; j < len(list); j++ {
				if _, ok := m[list[j]]; ok {
					inDegree[list[j]]--
					if inDegree[list[j]] == 0 {
						temp = append(temp, list[j])
					}
				}
			}
			delete(m, stack[i])
		}
		stack = temp
	}
	endTime := time.Now().UnixNano()
	fmt.Println("零入度删除耗时：      ", (endTime-startTime)/1e3, "ns")
}
