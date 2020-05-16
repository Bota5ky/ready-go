package main

import (
	"fmt"
	"time"
)

var (
	ids      []int                 //ID表，从小到大
	m        = make(map[int][]int) //邻接表
	inDegree = make(map[int]int)   //入度
	//redir    = make(map[int]int)               //重映射
	fileRoad = "../data/1004812/test_data.txt" //测试文件路径1004812/test_data.txt
	res      = make([][][]int, 5)              //结果
	nodeLink []map[int][]nodes2
	redir    = make(map[int]int)
	preNodes = make(map[int][]int)
)

func main() {
	rigister(fileRoad, m, inDegree, preNodes) //---1.读入文件注册
	topSort(m, inDegree)                      //---2.拓扑排序，删除零入度节点
	redirect(m, &ids /* , redir */)           //---3.获得有序ID表，和 ID-->序号 的 Map
	startTime := time.Now().UnixNano()
	for i := 0; i < len(ids); i++ {
		dfs(ids[i], []int{}, m, &res, &nodeLink, redir, preNodes)
	}
	endTime := time.Now().UnixNano()
	fmt.Println("dfs耗时：  ", (endTime-startTime)/1e6, "ms")
	fmt.Println(len(res[0]))
	fmt.Println(len(res[1]))
	fmt.Println(len(res[2]))
	fmt.Println(len(res[3]))
	fmt.Println(len(res[4]))
}

type nodes2 struct {
	num1 int
	num2 int
}

func dfs(curID int, temp []int, m map[int][]int, res *[][][]int, nodeLink *[]map[int][]nodes2, redir map[int]int, preNodes map[int][]int) {
	temp = append(temp, curID)
	if len(temp) < 4 {
		if len(temp) == 3 {
			if temp[2] == temp[0] { //2环无效
				return
			} else if temp[2] < temp[0] && temp[2] < temp[1] { //找5环
				if _, ok := redir[temp[2]]; ok {
					if list, ok := (*nodeLink)[redir[temp[2]]][temp[0]]; ok {
						for i := 0; i < len(list); i++ {
							if list[i].num1 == temp[1] || list[i].num2 == temp[1] {
								continue
							}
							(*res)[2] = append((*res)[2], []int{temp[2], list[i].num1, list[i].num2, temp[0], temp[1]}) //5环
						}
					}
				}
			}
		}
		if len(temp) == 2 && temp[1] < temp[0] {
			if _, ok := redir[temp[1]]; ok {
				if list, ok := (*nodeLink)[redir[temp[1]]][temp[0]]; ok {
					for i := 0; i < len(list); i++ {
						(*res)[1] = append((*res)[1], []int{temp[1], list[i].num1, list[i].num2, temp[0]}) //4环
					}
				}
			}
		}
		nextIDs := m[curID]
		for i := 0; i < len(nextIDs); i++ {
			temp2 := make([]int, len(temp))
			copy(temp2, temp)
			dfs(nextIDs[i], temp2, m, res, nodeLink, redir, preNodes)
		}
	} else { //长度为4时
		if temp[0] < temp[1] && temp[0] < temp[2] {
			if temp[3] == temp[0] { //3环有效
				(*res)[0] = append((*res)[0], []int{temp[0], temp[1], temp[2]}) //3环
			} else if temp[0] < temp[3] && temp[1] != temp[3] { //正向4链
				if key, ok := redir[temp[0]]; ok {
					(*nodeLink)[key][temp[3]] = append((*nodeLink)[key][temp[3]], nodes2{num1: temp[1], num2: temp[2]})
				} else {
					redir[temp[0]] = len(*nodeLink)
					temp3 := make(map[int][]nodes2)
					temp3[temp[3]] = append(temp3[temp[3]], nodes2{num1: temp[1], num2: temp[2]})
					*nodeLink = append(*nodeLink, temp3)
				}
			}
		}
		if temp[3] < temp[0] && temp[3] < temp[1] && temp[3] < temp[2] { //反向4链
			if _, ok := redir[temp[3]]; !ok {
				return
			}
			if list, ok := (*nodeLink)[redir[temp[3]]][temp[0]]; ok { //找6环
				for i := 0; i < len(list); i++ {
					if list[i].num1 == temp[1] || list[i].num1 == temp[2] || list[i].num2 == temp[1] || list[i].num2 == temp[2] {
						continue
					}
					(*res)[3] = append((*res)[3], []int{temp[3], list[i].num1, list[i].num2, temp[0], temp[1], temp[2]}) //6环
				}
			}
			preList := preNodes[temp[0]]
			for i := 0; i < len(preList); i++ {
				if preList[i] == temp[1] || preList[i] == temp[2] {
					continue
				}
				if list, ok := (*nodeLink)[redir[temp[3]]][preList[i]]; ok { //找7环
					for j := 0; j < len(list); j++ {
						if list[j].num1 == temp[0] || list[j].num1 == temp[1] || list[j].num1 == temp[2] {
							continue
						}
						if list[j].num2 == temp[0] || list[j].num2 == temp[1] || list[j].num2 == temp[2] {
							continue
						}
						(*res)[4] = append((*res)[4], []int{temp[3], list[j].num1, list[j].num2, preList[i], temp[0], temp[1], temp[2]}) //7环
					}
				}
			}
		}
	}
}
