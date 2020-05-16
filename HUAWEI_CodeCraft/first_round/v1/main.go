package main

import (
	"fmt"
	"io"
	"os"
)

type account struct {
	ID    int
	next  []*account //最多有9个分支
	visit bool
}

func main() {
	file, _ := os.OpenFile("./data/test_data.txt", os.O_RDONLY, 0666)
	m := make(map[int]*account)
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
	cnt := 0
	res := make(map[[7]int]bool)
	for id, node := range m {
		dfs(node, node, 1, &cnt, []int{id}, &res, node.ID)
	}
	fmt.Println(cnt)
}

func dfs(node, target *account, length int, cnt *int, temp []int, res *map[[7]int]bool, min int) { //转账长度为[3,7]
	if length > 7 || len(node.next) == 0 || node.visit {
		return
	}
	node.visit = true
	nextnodes := node.next
	for i := 0; i < len(nextnodes); i++ {
		if nextnodes[i].ID < min {
			continue
		}
		if nextnodes[i] == target {
			if length >= 3 && length <= 7 { //满足条件
				var tmp [7]int
				copy(tmp[:], temp)
				if (*res)[tmp] == false {
					(*res)[tmp] = true
					*cnt++
					fmt.Println(temp)
				}
			}
		} else {
			dfs(nextnodes[i], target, length+1, cnt, append(temp, nextnodes[i].ID), res, min)
		}
	}
	node.visit = false
}

func register(id1, id2 int, m *map[int]*account) {
	if _, ok := (*m)[id1]; !ok {
		(*m)[id1] = &account{ID: id1}
	}
	if _, ok := (*m)[id2]; !ok {
		(*m)[id2] = &account{ID: id2}
	}
	(*m)[id1].next = append((*m)[id1].next, (*m)[id2])
}
