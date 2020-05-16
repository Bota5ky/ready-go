package main

import (
	"fmt"
	"io"
	"os"
	"sort"
	"time"
)

type account struct {
	ID    int
	visit bool
	in    int
	next  []*account //最多有9个分支
}

func main() {
	startTime := time.Now().UnixNano()
	file, _ := os.OpenFile("test_data.txt", os.O_RDONLY, 0666)
	m := make(map[int]*account)
	var IDs []*account
	for {
		var temp [1]byte
		num1, num2 := 0, 0
		_, err := file.Read(temp[:])
		if err == io.EOF { //文件读完了退出
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
		for err != io.EOF && temp[0] != '\n' { //读到换行或者文件末尾
			_, err = file.Read(temp[:])
		}
		//注册两个ID
		register(num1, num2, &m)
	}
	for {
		count := 0
		for key, node := range m {
			if node.in == 0 {
				for i := 0; i < len(node.next); i++ {
					node.next[i].in--
				}
				delete(m, key)
				count++
			}
		}
		if count == 0 {
			break
		}
	}
	for _, node := range m {
		IDs = append(IDs, node)
	}
	/***************************************/
	endTime := time.Now().UnixNano()
	milliseconds := float64((endTime - startTime) / 1e6)
	fmt.Println("注册耗时:", milliseconds)
	/***************************************/
	sort.Slice(IDs, func(i, j int) bool {
		return IDs[i].ID < IDs[j].ID
	})
	/***************************************/
	endTime = time.Now().UnixNano()
	milliseconds = float64((endTime - startTime) / 1e6)
	fmt.Println("ID记录表耗时:", milliseconds)
	/***************************************/
	for i := 0; i < len(IDs); i++ {
		sort.Slice(IDs[i].next, func(x, y int) bool {
			return IDs[i].next[x].ID < IDs[i].next[y].ID
		})
	}
	/***************************************/
	endTime = time.Now().UnixNano()
	milliseconds = float64((endTime - startTime) / 1e6)
	fmt.Println("Next顺序表耗时:", milliseconds)
	/***************************************/

	res := make([][][]int, 5) //长度从3-7
	cnt := 0
	for i := 0; i < len(IDs); i++ {
		dfs(IDs[i], IDs[i], 1, &cnt, []int{IDs[i].ID}, &res)
	}
	fmt.Println(cnt)
	// for i := 0; i < 5; i++ {
	// 	for j := 0; j < len(res[i]); j++ {
	// 		fmt.Println(res[i][j])
	// 	}
	// }
	/***************************************/
	endTime = time.Now().UnixNano()
	milliseconds = float64((endTime - startTime) / 1e6)
	fmt.Println("DFS耗时:", milliseconds)
	/***************************************/
}

func dfs(node, target *account, length int, cnt *int, temp []int, res *[][][]int) { //转账长度为[3,7]
	if node.visit {
		return
	}
	node.visit = true
	for i := 0; i < len(node.next); i++ {
		nextNode := node.next[i]
		if nextNode.ID < target.ID {
			continue
		}
		if nextNode == target {
			if length >= 3 && length <= 7 { //满足条件
				*cnt++
				(*res)[length-3] = append((*res)[length-3], temp)
			}
		} else if length < 7 && len(nextNode.next) > 0 {
			dfs(nextNode, target, length+1, cnt, append(temp, nextNode.ID), res)
		}
	}
	node.visit = false
}

//注册节点，不存在就创建
func register(id1, id2 int, m *map[int]*account) {
	if _, ok := (*m)[id1]; !ok {
		(*m)[id1] = &account{ID: id1}
	}
	if _, ok := (*m)[id2]; !ok {
		(*m)[id2] = &account{ID: id2}
	}
	(*m)[id2].in++
	(*m)[id1].next = append((*m)[id1].next, (*m)[id2])
}
