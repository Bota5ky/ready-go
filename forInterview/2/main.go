package main

import (
	"fmt"
	"math/rand"
	"sync"
)

//在 golang 协程和channel配合使用
//写代码实现两个 goroutine，其中一个产生随机数并写入到 go channel 中，
//另外一个从 channel 中读取数字并打印到标准输出。最终输出五个随机数。
var chan1 = make(chan int)
var wg sync.WaitGroup

func main() {
	wg.Add(2)
	go func() {
		defer wg.Done()
		for i := 0; i < 5; i++ {
			chan1 <- rand.Intn(100)
		}
	}()
	go func() {
		defer wg.Done()
		for i := 0; i < 5; i++ {
			fmt.Print(<-chan1, " ")
		}
	}()
	wg.Wait()
}
