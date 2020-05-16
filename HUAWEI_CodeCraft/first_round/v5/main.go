package main

import (
	"io"
	"os"
)

func main() {
	file, _ := os.OpenFile("../data/56.txt", os.O_RDONLY, 0666)
	m := make(map[int][]int)
	inDegree := make(map[int]int)
	for { //读入数据，创建连接
		var temp [1]byte
		num1, num2 := 0, 0
		_, err := file.Read(temp[:])
		//每行开始判断是否文件已读完
		if err == io.EOF {
			break
		}
		for temp[0] != ',' {
			num1 = num1*10 + int(temp[0]-'0')
			_, err = file.Read(temp[:])
		}
		file.Read(temp[:])
		for temp[0] != ',' {
			num2 = num2*10 + int(temp[0]-'0')
			_, err = file.Read(temp[:])
		}
		for temp[0] != '\n' {
			_, err = file.Read(temp[:])
		}
		//注册两个ID
		register(num1, num2, &m, &inDegree)
	}
	//删除入度为0的点
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
	//tarjan algorithm
	var SCCS [][]int
	for acc := range m {
		visitTime := make(map[int]int)
		earlyTime := make(map[int]int)
		onStack := make(map[int]bool)
		var stack []int
		tarjan(acc, 1, &visitTime, &earlyTime, &m, &onStack, &stack, &SCCS)
	}
}

func register(num1, num2 int, m *map[int][]int, inDegree *map[int]int) {
	(*m)[num1] = append((*m)[num1], num2)
	(*inDegree)[num2]++
}

//tarjan algorithm
func tarjan(num, time int, visitTime, earlyTime *map[int]int, m *map[int][]int, onStack *map[int]bool, stack *[]int, SCCS *[][]int) int {
	if (*onStack)[num] {
		return (*earlyTime)[num]
	}
	(*visitTime)[num], (*earlyTime)[num] = time, time
	(*onStack)[num] = true
	list := (*m)[num]
	for i := 0; i < len(list); i++ {
		if _, ok := (*m)[list[i]]; ok {
			(*earlyTime)[num] = min((*earlyTime)[num], tarjan(list[i], time+1, visitTime, earlyTime, m, onStack, stack, SCCS))
		}
	}
	if (*visitTime)[num] == (*earlyTime)[num] {
		var temp []int //单个SCC
		for len(*stack) > 0 {
			popNode := (*stack)[len(*stack)-1]
			if (*earlyTime)[popNode] != (*earlyTime)[num] {
				break
			}
			*stack = (*stack)[:len(*stack)-1]
			(*onStack)[popNode] = false
			temp = append(temp, popNode)
			delete(*m, popNode)
		}
		*SCCS = append(*SCCS, temp)
	}
	return (*earlyTime)[num]
}

func min(a, b int) int {
	if a < b {
		return a
	}
	return b
}
