package main

import (
	"fmt"
	"io"
	"os"
)

func main() {
	file, _ := os.OpenFile("../data/test_data_online.txt", os.O_RDONLY, 0666)
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
	var sta []int
	for num := range m {
		if inDegree[num] == 0 {
			sta = append(sta, num)
		}
	}
	for len(sta) > 0 {
		var temp []int
		for i := 0; i < len(sta); i++ {
			list := m[sta[i]]
			for j := 0; j < len(list); j++ {
				if _, ok := m[list[j]]; ok {
					inDegree[list[j]]--
					if inDegree[list[j]] == 0 {
						temp = append(temp, list[j])
					}
				}
			}
			delete(m, sta[i])
		}
		sta = temp
	}
	//tarjan algorithm
	var SCCS [][]int              //强连通子图集合
	dfn := make(map[int]int)      //访问的时间点
	low := make(map[int]int)      //可回溯到的最早时间点
	inStack := make(map[int]bool) //是否在栈中
	time := 1                     //时间戳
	var stack []int               //栈
	for num := range m {
		if dfn[num] == 0 {
			dfs(num, &time, &stack, &inStack, &dfn, &low, &m, &SCCS)
		}
	}
	fmt.Println(len(SCCS))
	// for i := 0; i < len(SCCS); i++ {
	// 	fmt.Println(SCCS[i])
	// }
}

//注册邻接表
func register(num1, num2 int, m *map[int][]int, inDegree *map[int]int) {
	(*m)[num1] = append((*m)[num1], num2)
	(*inDegree)[num2]++
}

//tarjan algorithm
func dfs(num int, time *int, stack *[]int, inStack *map[int]bool, dfn, low *map[int]int, m *map[int][]int, SCCS *[][]int) {
	//压入栈
	*stack = append(*stack, num)
	(*inStack)[num] = true
	(*dfn)[num], (*low)[num] = *time, *time
	(*time)++
	list := (*m)[num]
	for i := 0; i < len(list); i++ {
		if (*dfn)[list[i]] == 0 {
			dfs(list[i], time, stack, inStack, dfn, low, m, SCCS)
			(*low)[num] = min((*low)[num], (*low)[list[i]])
		} else if (*inStack)[list[i]] {
			(*low)[num] = min((*low)[num], (*low)[list[i]])
		}
	}
	if (*dfn)[num] == (*low)[num] { //stack.pop一直到num
		i := len(*stack) - 1
		for {
			//delete(*m, (*stack)[i])
			delete(*inStack, (*stack)[i])
			if (*stack)[i] == num {
				break
			}
			i--
		}
		if len((*stack)[i:]) > 2 {
			temp := make([]int, len((*stack)[i:]))
			copy(temp, (*stack)[i:])
			*SCCS = append(*SCCS, temp)
		}
		*stack = (*stack)[:i]
	}
}

func min(a, b int) int {
	if a < b {
		return a
	}
	return b
}
