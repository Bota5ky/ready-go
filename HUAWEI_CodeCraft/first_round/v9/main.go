package main

import (
	"bufio"
	"fmt"
	"io"
	"os"
	"sync"
	"time"
)

var (
	ids      []int                             //ID表，从小到大
	fileRoad = "../data/1004812/test_data.txt" //测试文件路径1004812/test_data.txt
)

//M 邻接表
type M struct {
	sync.RWMutex
	Map map[int][]int
}

//InDegree 入度
type InDegree struct {
	sync.RWMutex
	Map map[int]int
}

//PreNode 父节点
type PreNode struct {
	sync.RWMutex
	Map map[[2]int]bool
}

func newM(size int) *M {
	sm := new(M)
	sm.Map = make(map[int][]int)
	return sm
}

func (sm *M) writeM(num1 int, num2 int) {
	sm.Lock()
	sm.Map[num1] = append(sm.Map[num1], num2)
	sm.Unlock()
}

//InDegree
func newIndegree(size int) *InDegree {
	sm := new(InDegree)
	sm.Map = make(map[int]int)
	return sm
}

func (sm *InDegree) writeIndegree(num2 int) {
	sm.Lock()
	sm.Map[num2]++
	sm.Unlock()
}

//PreNode
func newPreNode(size int) *PreNode {
	sm := new(PreNode)
	sm.Map = make(map[[2]int]bool)
	return sm
}

func (sm *PreNode) writePreNode(num1, num2 int) {
	sm.Lock()
	sm.Map[[2]int{num1, num2}] = true
	sm.Unlock()
}

var wg sync.WaitGroup

func main() {
	startTime := time.Now().UnixNano()
	file, _ := os.OpenFile(fileRoad, os.O_RDONLY, 0666) //1004812/test_data.txt
	f := bufio.NewReader(file)
	defer file.Close()
	m := newM(10)
	inDegree := newIndegree(10)
	preNode := newPreNode(10)
	lines := make(chan string, 100000)
	for { //读入数据，创建连接
		line, err := f.ReadString('\n')
		if err == io.EOF {
			break
		}
		lines <- line
		//注册两个ID
		wg.Add(1)
		go rig(m, preNode, inDegree, extract(<-lines))
	}
	wg.Wait()
	endTime := time.Now().UnixNano()
	fmt.Println("注册耗时：            ", (endTime-startTime)/1e6, "ms")
	// for key,value:=range m.Map {
	// 	fmt.Println(key,value)
	// }
}

func rig(m *M, preNode *PreNode, inDegree *InDegree, num [2]int) {
	defer wg.Done()
	go m.writeM(num[0], num[1])
	go preNode.writePreNode(num[0], num[1])
	go inDegree.writeIndegree(num[1])
}

func extract(line string) [2]int {
	i, num1, num2 := 0, 0, 0
	for ; i < len(line) && line[i] != ','; i++ {
		num1 = num1*10 + int(line[i]-'0')
	}
	i++
	for ; i < len(line) && line[i] != ','; i++ {
		num2 = num2*10 + int(line[i]-'0')
	}
	return [2]int{num1, num2}
}
