package main

import (
	"fmt"
	"io"
	"os"
	"time"
)

func main() {
	startTime := time.Now().UnixNano()
	file, _ := os.OpenFile("../data/1004812C.txt", os.O_RDONLY, 0666)
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
	endTime := time.Now().UnixNano()
	fmt.Println("注册时间：", (endTime-startTime)/1e6, "ms")
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
	endTime = time.Now().UnixNano()
	fmt.Println("删除入度为0：", (endTime-startTime)/1e6, "ms")
	//tarjan algorithm
	var SCCS [][]int              //强连通子图集合
	dfn := make(map[int]int)      //访问的时间点
	low := make(map[int]int)      //可回溯到的最早时间点
	inStack := make(map[int]bool) //是否在栈中
	t := 1                        //时间戳
	var stack []int               //栈
	for num := range m {
		if dfn[num] == 0 {
			dfs(num, &t, &stack, &inStack, &dfn, &low, &m, &SCCS)
		}
	}
	endTime = time.Now().UnixNano()
	fmt.Println("划分SCC：", (endTime-startTime)/1e6, "ms")
	// for i := 0; i < len(SCCS); i++ {
	// 	fmt.Println(SCCS[i])
	// }
	//
	
}

//注册邻接表
func register(num1, num2 int, m *map[int][]int, inDegree *map[int]int) {
	(*m)[num1] = append((*m)[num1], num2)
	(*inDegree)[num2]++
}

//tarjan algorithm
func dfs(num int, t *int, stack *[]int, inStack *map[int]bool, dfn, low *map[int]int, m *map[int][]int, SCCS *[][]int) {
	//压入栈
	*stack = append(*stack, num)
	(*inStack)[num] = true
	(*dfn)[num], (*low)[num] = *t, *t
	(*t)++
	list := (*m)[num]
	for i := 0; i < len(list); i++ {
		if (*dfn)[list[i]] == 0 {
			dfs(list[i], t, stack, inStack, dfn, low, m, SCCS)
			(*low)[num] = min((*low)[num], (*low)[list[i]])
		} else if (*inStack)[list[i]] {
			(*low)[num] = min((*low)[num], (*low)[list[i]])
		}
	}
	if (*dfn)[num] == (*low)[num] { //stack.pop一直到num
		var temp []int
		for {
			lastNum := (*stack)[len(*stack)-1]
			delete(*m, lastNum)
			delete(*inStack, lastNum)
			temp = append(temp, lastNum)
			*stack = (*stack)[:len(*stack)-1]
			if lastNum == num {
				break
			}
		}
		*SCCS = append(*SCCS, temp)
	}
}

func min(a, b int) int {
	if a < b {
		return a
	}
	return b
}
