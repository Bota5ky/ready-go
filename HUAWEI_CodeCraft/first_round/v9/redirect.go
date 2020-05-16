package main

import (
	"fmt"
	"sort"
	"time"
)

//redirect 重定向
func redirect(m map[int][]int, ids *[]int, redir map[int]int) {
	startTime := time.Now().UnixNano()
	for id := range m {
		sort.Ints(m[id])
		*ids = append(*ids, id)
	}
	sort.Ints(*ids)
	for i := 0; i < len(*ids); i++ {
		redir[(*ids)[i]] = i
	}
	endTime := time.Now().UnixNano()
	fmt.Println("重定向及预排序耗时：  ", (endTime-startTime)/1e6, "ms")
}
