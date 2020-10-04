package taillog

import (
	"context"
	"fmt"
	"logAgent/kafka"

	"github.com/hpcloud/tail"
)

//TailTask 一个日志收集的任务
type TailTask struct {
	path     string
	topic    string
	instance *tail.Tail //tail包打开的那个文件
	//为了实现退出t.run
	ctx        context.Context
	cancelFunc context.CancelFunc
}

//NewTailTask 新建
func NewTailTask(path, topic string) (tailObj *TailTask) {
	ctx, cancel := context.WithCancel(context.Background())
	tailObj = &TailTask{
		path:       path,
		topic:      topic,
		ctx:        ctx,
		cancelFunc: cancel,
	}
	tailObj.init()
	return
}

//Init TailTask初始化方法
func (t *TailTask) init() {
	config := tail.Config{
		ReOpen:    true,                                 // 重新打开
		Follow:    true,                                 // 是否跟随
		Location:  &tail.SeekInfo{Offset: 0, Whence: 2}, // 从文件的哪个地方开始读
		MustExist: false,                                // 文件不存在不报错
		Poll:      true,
	}
	var err error
	t.instance, err = tail.TailFile(t.path, config)
	if err != nil {
		fmt.Println("tail file failed, err:", err)
	}
	go t.run()
}

func (t *TailTask) run() {
	for {
		select {
		case <-t.ctx.Done():
			fmt.Println("tail task:", t.path, "_", t.topic, " quit...")
			return
		case line := <-t.instance.Lines: //从tailObj通道中一行一行
			//3.2 发往kafka
			//kafka.SendToKafka(t.topic, line.Text)
			//先发到通道
			kafka.SendToChan(t.topic, line.Text)
			//kafka包中单独用goroutine去取日志数据发送到kafka
		}
	}
}

// //Init 初始化
// func Init(fileName string) (err error) {
// 	config := tail.Config{
// 		ReOpen:    true,                                 // 重新打开
// 		Follow:    true,                                 // 是否跟随
// 		Location:  &tail.SeekInfo{Offset: 0, Whence: 2}, // 从文件的哪个地方开始读
// 		MustExist: false,                                // 文件不存在不报错
// 		Poll:      true,
// 	}
// 	tailObj, err = tail.TailFile(fileName, config)
// 	if err != nil {
// 		fmt.Println("tail file failed, err:", err)
// 		return
// 	}
// 	return
// }

// //ReadChan chan收到数据
// func (t *TailTask) ReadChan() <-chan *tail.Line {
// 	return t.instance.Lines
// }
