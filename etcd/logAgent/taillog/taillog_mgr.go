package taillog

import (
	"fmt"
	"logAgent/etcd"
	"time"
)

var tskMgr *tailLogMgr

//tailLogMgr 管理者
type tailLogMgr struct {
	logEntry    []*etcd.LogEntry
	tskMap      map[string]*TailTask
	newConfChan chan []*etcd.LogEntry
}

//Init 初始化
func Init(logEntryConf []*etcd.LogEntry) {
	tskMgr = &tailLogMgr{
		logEntry:    logEntryConf, //把当前的日志收集项配置信息保存起来
		tskMap:      make(map[string]*TailTask, 16),
		newConfChan: make(chan []*etcd.LogEntry), //无缓冲区的通道
	}
	//3.1 循环每个日志收集项，创建tailobj
	for _, logEntry := range logEntryConf {
		// conf是 json格式的 address和 topic  即 *etcd.LogEntry
		// logentry  要收集日志的路径
		// 初始化的时候起了多少个tailtask都要记下来，为了后续判断方便
		tailObj := NewTailTask(logEntry.Path, logEntry.Topic)
		//路径和 topic作为 key
		mk := fmt.Sprintf("%s_%s", logEntry.Path, logEntry.Topic)
		tskMgr.tskMap[mk] = tailObj //路径不一样
	}
	go tskMgr.run()
}

// 监听自己的newConfChan ,有新的配置过来就做对应的处
func (t *tailLogMgr) run() {
	for {
		select {
		case newConf := <-t.newConfChan:
			//1.配置新增
			//2.配置删除
			//3.配置变更
			for _, conf := range newConf {
				mk := fmt.Sprintf("%s_%s", conf.Path, conf.Topic)
				if _, ok := t.tskMap[mk]; !ok {
					tailObj := NewTailTask(conf.Path, conf.Topic)
					t.tskMap[mk] = tailObj
				}
			}
			// 找出原来 t.logEntry有，newConf中没有的,删除
			for _, c1 := range t.logEntry { //从原来的配置中依次拿出配置项
				isDelete := true
				for _, c2 := range newConf { //从新的配置中依次拿出配置项
					if c2.Path == c1.Path && c2.Topic == c1.Topic {
						isDelete = false
						continue
					}
				}
				if isDelete {
					//把c1对应的tailObj给停掉
					mk := fmt.Sprintf("%s_%s", c1.Path, c1.Topic)
					//t.tskMap[mk] ==> tailObj
					t.tskMap[mk].cancelFunc()
				}
			}
			fmt.Println("新配置：", newConf)
		default:
			time.Sleep(time.Second)
		}
	}
}

//NewConfChan 一个函数，向外暴露向外暴露tskMgr的newConfChan
func NewConfChan() chan<- []*etcd.LogEntry {
	return tskMgr.newConfChan
}
