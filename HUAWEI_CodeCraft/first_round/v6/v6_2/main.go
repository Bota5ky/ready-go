package main

import (
	"fmt"
	"io"
	"os"
	"sort"
	"time"
)

func main() {
	startTime := time.Now().UnixNano()
	file, _ := os.OpenFile("../data/56C.txt", os.O_RDONLY, 0666)
	m := make(map[int][]int)
	var ids []int
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
		register(num1, num2, &m, &ids)
	}
	endTime := time.Now().UnixNano()
	fmt.Println("注册时间：", (endTime-startTime)/1e6, "ms")
	sort.Ints(ids)
	redirect := make(map[int]int) //重映射ID-->ids中的序号
	for i := 0; i < len(ids); i++ {
		redirect[ids[i]] = i
	}
	L3 := make([]map[int][]int, len(ids)-2)//L3[head][rear]-->[mid]
	for i := 0; i < len(ids)-2; i++ {
		L3[i]=make(map[int][]int)
		mids := m[ids[i]]
		for j := 0; j < len(mids); j++ {
			lasts := m[mids[j]]
			for k := 0; k < len(lasts); k++ {
				L3[i][redirect[lasts[k]]] = append(L3[i][redirect[lasts[k]]], redirect[mids[j]])
			}
		}
	}
	endTime = time.Now().UnixNano()
	fmt.Println("建立L3", (endTime-startTime)/1e6, "ms")
	for i:=0;i<len(ids);i++ {
		
	}
}

//注册邻接表
func register(num1, num2 int, m *map[int][]int, ids *[]int) {
	if _, ok := (*m)[num1]; !ok {
		*ids = append(*ids, num1)
	}
	(*m)[num1] = append((*m)[num1], num2)
}
