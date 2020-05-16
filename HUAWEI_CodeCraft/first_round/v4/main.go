package main

import (
	"fmt"
	"io"
	"os"
	"sort"
	"time"
)

type node struct {
	ID   int
	next *node
}

//邻接表版本
func main() {
	startTime := time.Now().UnixNano()
	file, _ := os.OpenFile("./data/1004812/test_data.txt", os.O_RDONLY, 0666)
	m := make(map[int]*node)
	for {
		var temp [1]byte
		num1, num2 := 0, 0
		//每次读一个字符
		_, err := file.Read(temp[:])
		//文件读完了退出
		if err == io.EOF {
			break
		}
		//不是EOF就传入整数num1，读到[，]退出
		for temp[0] != ',' {
			num1 = num1*10 + int(temp[0]-'0')
			_, err = file.Read(temp[:])
		}
		//再读下一个num2
		file.Read(temp[:])
		for temp[0] != ',' {
			num2 = num2*10 + int(temp[0]-'0')
			_, err = file.Read(temp[:])
		}
		//读到换行，下次开始读就在下一行开头
		for temp[0] != '\n' {
			_, err = file.Read(temp[:])
		}
		//注册两个ID
		register(num1, num2, &m)
	}
	endTime := time.Now().UnixNano()
	fmt.Println("注册耗时：", (endTime-startTime)/1e6)
	var IDs []int
	for id := range m {
		IDs = append(IDs, id)
	}
	sort.Ints(IDs)
	endTime = time.Now().UnixNano()
	fmt.Println("预排序耗时：", (endTime-startTime)/1e6)
	//栈操作
	var stack []*node
	visit := make(map[int]bool)
	res := make([][][]int, 5)
	cnt := 0
	for i := 0; i < len(IDs)-2; {
		if len(stack) < 7 { //增加节点
			if len(stack) == 0 {
				stack = append(stack, m[IDs[i]]) //可能会没有子节点
			} else {
				lastNode := stack[len(stack)-1]       //根据上一个节点取next[]
				stack = append(stack, m[lastNode.ID]) //可能会没有子节点
			}
		} else { //栈满了
			delete(visit, stack[len(stack)-1].ID)
			stack[len(stack)-1] = stack[len(stack)-1].next
		}
		//新节点不满足要求的处理
		//1.访问过的节点 2.m[]中找不到的也就是没子节点的 3.小于目标值的节点 4.遍历完，空节点
		for len(stack) > 0 {
			lastNode := stack[len(stack)-1]
			if lastNode == nil {
				stack = stack[:len(stack)-1]
				if len(stack) > 0 {
					delete(visit, stack[len(stack)-1].ID)
					stack[len(stack)-1] = stack[len(stack)-1].next
				}
			} else if _, ok := m[lastNode.ID]; !ok || visit[lastNode.ID] || lastNode.ID <= IDs[i] {
				if lastNode.ID == IDs[i] && len(stack) >= 3 {
					var temp []int
					temp = append(temp, IDs[i])
					for j := 0; j < len(stack)-1; j++ {
						temp = append(temp, stack[j].ID)
					}
					cnt++
					res[len(temp)-3] = append(res[len(temp)-3], temp)
				}
				stack[len(stack)-1] = stack[len(stack)-1].next
			} else {
				break
			}
		}
		if len(stack) == 0 {
			i++
		} else {
			lastNode := stack[len(stack)-1]
			visit[lastNode.ID] = true
		}
	}
	fmt.Println(cnt)
	// for i := 0; i < 5; i++ {
	// 	for j := 0; j < len(res[i]); j++ {
	// 		fmt.Println(res[i][j])
	// 	}
	// }
	endTime = time.Now().UnixNano()
	fmt.Println("总耗时：", (endTime-startTime)/1e6)
}

func register(num1, num2 int, m *map[int]*node) {
	if _, ok := (*m)[num1]; !ok {
		(*m)[num1] = &node{ID: num2}
	} else {
		if num2 < (*m)[num1].ID {
			newNode := &node{ID: num2, next: (*m)[num1]}
			(*m)[num1] = newNode
		} else {
			pre := (*m)[num1]
			head := (*m)[num1].next
			for head != nil && num2 > head.ID {
				pre = pre.next
				head = head.next
			}
			newNode := &node{ID: num2}
			pre.next = newNode
			newNode.next = head
		}
	}
}
