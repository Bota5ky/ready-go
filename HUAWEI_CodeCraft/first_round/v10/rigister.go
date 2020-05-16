package main

import (
	"fmt"
	"io"
	"os"
	"time"
)

//Rigister 读入数据
func rigister(fileRoad string, m map[int][]int, inDegree map[int]int, preNodes map[int][]int) {
	startTime := time.Now().UnixNano()
	file, _ := os.OpenFile(fileRoad, os.O_RDONLY, 0666) //1004812/test_data.txt
	defer file.Close()
	for { //读入数据，创建连接
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
		for temp[0] != '\n' { //读到换行或者文件末尾
			_, err = file.Read(temp[:])
		}
		//注册两个ID
		r(num1, num2, m, inDegree, preNodes)
	}
	endTime := time.Now().UnixNano()
	fmt.Println("注册耗时：            ", (endTime-startTime)/1e6, "ms")
}

//Rigister 注册邻接表
func r(num1, num2 int, m map[int][]int, inDegree map[int]int, preNodes map[int][]int) {
	m[num1] = append(m[num1], num2)
	preNodes[num2] = append(preNodes[num2], num1)
	inDegree[num2]++
}
