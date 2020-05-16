package main

import (
	"io"
	"os"
	"sort"
)

type account struct {
	ID      int
	next    []*account //平均 < 10 个分支
	i, j    int        //i访问时间点 j可回溯到的最早时间点
	onStack bool
}
type node struct {
	ID    int
	refer int //表示Sccs中ID的位置
}

func main() {
	file, _ := os.OpenFile("./data/test_data.txt", os.O_RDONLY, 0666)
	m := make(map[int]*account)
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
		register(num1, num2, &m)
	}
	var stack []*account
	var Sccs [][]*account
	var nodes []*node
	visited := make(map[int]bool) //visited表示节点是否已组成SCCs
	for _, acc := range m {
		dfs(acc, &stack, 1, &m, &visited, &Sccs, &nodes)
	}
	sort.Slice(nodes, func(i, j int) bool {
		return nodes[i].ID < nodes[j].ID
	})

	// threeTo7 := make([][][]int, 5)
	// for len(nodes) > 0 {
	// 	index := nodes[0].refer
	// 	p := Sccs[index][0]
	// 	nodes = nodes[1:]
	// 	Scc := make(map[int]bool)
	// 	for i := 0; i < len(Sccs[index]); i++ {
	// 		Scc[Sccs[index][i].ID] = true
	// 		sort.Slice(Sccs[index][i].next, func(x, y int) bool {
	// 			return Sccs[index][i].next[x].ID < Sccs[index][i].next[y].ID
	// 		})
	// 	}
		//对p找环路
		//jonson(p)
	// 	Sccs[index] = Sccs[index][1:]
	// }
}

//jonson algorithm
func jonson(acc, start *account, Scc map[int]bool, stack *[]*account, blockedMap *map[*account]*account, res *[][][]int) {
	if len(Scc) < 3 || acc.onStack {
		return
	}
	//这里的onStack代表blockedSet，并不是表示是否在stack中
	acc.onStack = true
	(*stack) = append(*stack, acc)
	for i := 0; i < len(acc.next); i++ {
		if Scc[acc.next[i].ID] == false {
			continue
		}
		if acc.next[i] == start {
			length := len(*stack)
			if length >= 3 {
				temp := make([]int, length)
				for j := 0; j < length; j++ {
					temp[j] = (*stack)[j].ID
				}
				(*res)[length-3] = append((*res)[length-3], temp)
			}
		} else {
			jonson(acc.next[i], start, Scc, stack, blockedMap, res)
		}
	}
}

//tarjan algorithm
func dfs(acc *account, stack *[]*account, time int, m *map[int]*account, visited *map[int]bool, Sccs *[][]*account, nodes *[]*node) int {
	if acc.onStack {
		return acc.j
	}
	acc.i, acc.j = time, time
	acc.onStack = true
	*stack = append(*stack, acc)
	for i := 0; i < len(acc.next); i++ {
		if (*visited)[acc.next[i].ID] == false {
			acc.j = min(acc.j, dfs(acc.next[i], stack, time+1, m, visited, Sccs, nodes))
		}
	}
	if acc.i == acc.j {
		var temp []*account //单个SCC
		for len(*stack) > 0 {
			popNode := (*stack)[len(*stack)-1]
			if popNode.j != acc.j {
				break
			}
			*stack = (*stack)[:len(*stack)-1]
			popNode.onStack = false
			(*visited)[popNode.ID] = true
			newnode := &node{ID: popNode.ID, refer: len(*Sccs)}
			*nodes = append(*nodes, newnode)
			temp = append(temp, popNode)
			delete(*m, popNode.ID)
		}
		if len(temp) > 0 {
			sort.Slice(temp, func(i, j int) bool {
				return temp[i].ID < temp[j].ID
			})
			*Sccs = append(*Sccs, temp)
		}
	}
	return acc.j
}

//注册建立每行两个ID之间的连接，节点不存在就创建
func register(id1, id2 int, m *map[int]*account) {
	if _, ok := (*m)[id1]; !ok {
		(*m)[id1] = &account{ID: id1}
	}
	if _, ok := (*m)[id2]; !ok {
		(*m)[id2] = &account{ID: id2}
	}
	(*m)[id1].next = append((*m)[id1].next, (*m)[id2])
}

func min(a, b int) int {
	if a < b {
		return a
	}
	return b
}
