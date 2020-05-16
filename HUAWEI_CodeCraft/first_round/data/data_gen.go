package main

import (
	"fmt"
	"os"
)

//data generator
func main() {
	file, _ := os.OpenFile("56.txt", os.O_RDONLY, 0666) //1004812/test_data.txt
	file2, _ := os.OpenFile("56000.txt", os.O_CREATE|os.O_TRUNC|os.O_WRONLY, 0666)
	defer file.Close()
	defer file2.Close()
	for { //读入数据，创建连接
		var num1, num2, num3 int
		_, err := fmt.Fscanf(file, "%d,%d,%d\n", &num1, &num2, &num3)
		if err != nil {
			break
		}
		for i := 0; i < 1000; i++ {
			n1, n2 := num1+7000*i, num2+7000*i
			file2.WriteString(fmt.Sprintf("%d,%d,%d\n", n1, n2, num3))
		}
	}
}
