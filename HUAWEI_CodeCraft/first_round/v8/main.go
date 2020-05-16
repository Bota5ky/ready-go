package main

var (
	ids      []int                             //ID表，从小到大
	m        = make(map[int][]int)             //邻接表
	preNode  = make(map[[2]int]bool)           //父节点
	inDegree = make(map[int]int)               //入度
	redir    = make(map[int]int)               //重映射
	fileRoad = "../data/1004812/test_data.txt" //测试文件路径1004812/test_data.txt
)

func main() {
	rigister(fileRoad, m, inDegree, preNode) //---1.读入文件注册
	topSort(m, inDegree)                     //---2.拓扑排序，删除零入度节点
	redirect(m, &ids, redir)                 //---3.获得有序ID表，和 ID-->序号 的 Map
}
