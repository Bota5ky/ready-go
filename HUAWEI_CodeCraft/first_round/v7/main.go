package main

import (
	"fmt"
	"os"
	"sort"
	"time"
)

type node2 struct {
	num1 int
	num2 int
}

func main() {
	beginTime := time.Now().UnixNano()
	file, _ := os.OpenFile("../data/56.txt", os.O_RDONLY, 0666) //1004812/test_data.txt
	defer file.Close()
	m := make(map[int][]int)
	preNode := make(map[[2]int]bool)
	inDegree := make(map[int]int)
	for { //读入数据，创建连接
		var num1, num2, num3 int
		_, err := fmt.Fscanf(file, "%d,%d,%d\n", &num1, &num2, &num3)
		if err != nil {
			break
		}
		//注册两个ID
		register(num1, num2, &m, &inDegree, &preNode)
	}
	endTime := time.Now().UnixNano()
	fmt.Println("注册耗时：            ", (endTime-beginTime)/1e6, "ms")
	startTime := time.Now().UnixNano()
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
	fmt.Println("0入度删除耗时：       ", (endTime-startTime)/1e6, "ms")
	startTime = time.Now().UnixNano()
	var ids []int
	for id := range m {
		sort.Ints(m[id])
		ids = append(ids, id)
	}
	sort.Ints(ids)
	redir := make(map[int]int) //重定向
	for i := 0; i < len(ids); i++ {
		redir[ids[i]] = i
	}
	endTime = time.Now().UnixNano()
	fmt.Println("重定向及预排序耗时：  ", (endTime-startTime)/1e6, "ms")
	startTime = time.Now().UnixNano()
	res := make([][][]int, 5)
	L4 := make([]map[int][]node2, len(ids))
	for i := 0; i < len(ids); i++ {
		L4[i] = make(map[int][]node2)
		dfs4(ids[i], 1, []int{}, &res, &L4, m, redir)
	}
	endTime = time.Now().UnixNano()
	fmt.Println("找3环耗时&构造L4链：  ", (endTime-startTime)/1e6, "ms")
	startTime = time.Now().UnixNano()
	//找4环
	for i := 0; i < len(L4); i++ {
		tempMap := L4[i]
		for last, nodeList := range tempMap {
			if last < ids[i] {
				continue
			}
			if preNode[[2]int{ids[i],last}] {
				for j := 0; j < len(nodeList); j++ {
					if nodeList[j].num1 < ids[i] || nodeList[j].num2 < ids[i] {
						continue
					}
					res[1] = append(res[1], []int{ids[i], nodeList[j].num1, nodeList[j].num2, last})
				}
			}
		}
	}
	endTime = time.Now().UnixNano()
	fmt.Println("找4环耗时：           ", (endTime-startTime)/1e6, "ms")
	startTime = time.Now().UnixNano()
	//找5环
	for i := 0; i < len(L4); i++ {
		tempMap := L4[i]
		for last, nodeList := range tempMap {
			if last<ids[i] {continue}
			lastList := m[last]
			for k := 0; k < len(lastList); k++ {
				if lastList[k]<ids[i] {continue}
				if preNode[[2]int{ids[i],lastList[k]}] {
					for j := 0; j < len(nodeList); j++ {
						if nodeList[j].num1<ids[i] || nodeList[j].num2<ids[i] {continue}
						//fmt.Println([]int{ids[i], nodeList[j].num1, nodeList[j].num2, last, lastList[k]})
						res[2] = append(res[2], []int{ids[i], nodeList[j].num1, nodeList[j].num2, last, lastList[k]})
					}
				}
			}
		}
	}
	endTime = time.Now().UnixNano()
	fmt.Println("找5环耗时：           ", (endTime-startTime)/1e6, "ms")
	startTime = time.Now().UnixNano()
	// //找6环
	// for i := 0; i < len(L4); i++ {
	// 	tempMap := L4[i]
	// 	for last, nodeList := range tempMap {
	// 		if _, ok := redir[last]; ok && redir[last] < len(L4) {
	// 			if nodeList2, ok := L4[redir[last]][ids[i]]; ok {
	// 				for j := 0; j < len(nodeList); j++ {
	// 					for k := 0; k < len(nodeList2); k++ {
	// 						res[3] = append(res[3], []int{ids[i], nodeList[j].num1, nodeList[j].num2, last, nodeList2[k].num1, nodeList2[k].num2})
	// 					}
	// 				}
	// 			}
	// 		}
	// 	}
	// }
	// endTime = time.Now().UnixNano()
	// fmt.Println("找6环耗时：           ", (endTime-startTime)/1e6, "ms")
	// startTime = time.Now().UnixNano()
	// fmt.Println("L4长度：", len(L4))
	// fmt.Println("id个数：", len(ids))
	// fmt.Println("3环个数：", len(res[0]))
	// fmt.Println("4环个数：", len(res[1]))
	// fmt.Println("5环个数：", len(res[2]))
	// fmt.Println("6环个数：", len(res[3]))
	// //找7环
	// for i := 0; i < len(L4); i++ {
	// 	tempMap := L4[i]
	// 	for last, nodeList := range tempMap {
	// 		if _, ok := redir[last]; ok && redir[last] < len(L4) {
	// 			tempMap2 := L4[redir[last]]
	// 			for last2, nodeList2 := range tempMap2 {
	// 				if preNode[ids[i]] == last2 {
	// 					for j := 0; j < len(nodeList); j++ {
	// 						for k := 0; k < len(nodeList2); k++ {
	// 							res[4] = append(res[4], []int{ids[i], nodeList[j].num1, nodeList[j].num2, last, nodeList2[k].num1, nodeList2[k].num2, last2})
	// 						}
	// 					}
	// 				}
	// 			}
	// 		}
	// 	}
	// }
	// endTime = time.Now().UnixNano()
	// fmt.Println("找7环耗时：           ", (endTime-startTime)/1e6, "ms")
	// fmt.Println("总耗时：              ", (endTime-beginTime)/1e6, "ms")
	fmt.Println("-----------------------------")
	fmt.Println("L4长度：", len(L4))
	fmt.Println("id个数：", len(ids))
	fmt.Println("3环个数：", len(res[0]))
	fmt.Println("4环个数：", len(res[1]))
	fmt.Println("5环个数：", len(res[2]))
	fmt.Println("6环个数：", len(res[3]))
	fmt.Println("7环个数：", len(res[4]))
}

//构建节点长度为4的链
func dfs4(num, length int, temp []int, res *[][][]int, L4 *[]map[int][]node2, m map[int][]int, redir map[int]int) { //转账长度为[3,7]
	temp = append(temp, num)
	if length == 3 && num == temp[0] {
		return
	}
	if length < 4 {
		list := m[num]
		for i := 0; i < len(list); i++ {
			dfs4(list[i], length+1, temp, res, L4, m, redir)
		}
	} else {
		if temp[3] == temp[0] { //形成3环
			if temp[1] > temp[0] && temp[2] > temp[0] {
				(*res)[0] = append((*res)[0], temp[:3])
			}
		} else {
			var newNode node2
			newNode.num1 = temp[1]
			newNode.num2 = temp[2]
			(*L4)[redir[temp[0]]][temp[3]] = append((*L4)[redir[temp[0]]][temp[3]], newNode)
		}
	}
}

//注册邻接表
func register(num1, num2 int, m *map[int][]int, inDegree *map[int]int, preNode *map[[2]int]bool) {
	(*m)[num1] = append((*m)[num1], num2)
	(*preNode)[[2]int{num2,num1}] = true
	(*inDegree)[num2]++
}

// func binarySearch(nums []int, target int) int {
// 	i, j := 0, len(nums)-1
// 	for i < j {
// 		mid := (i + j) / 2
// 		if nums[mid] == target {
// 			return mid
// 		}
// 		if nums[mid] < target {
// 			i = mid + 1
// 		} else {
// 			j = mid - 1
// 		}
// 	}
// 	return -1
// }
