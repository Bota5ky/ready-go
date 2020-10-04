package main

import (
	"fmt"
	"time"

	"github.com/hpcloud/tail"
)

// tail demo
func main() {
	fileName := "./my.log"
	config := tail.Config{
		ReOpen:    true,                                 //重新打开
		Follow:    true,                                 //是否跟随
		Location:  &tail.SeekInfo{Offset: 0, Whence: 2}, //从文件的哪个地方开始读。0为相对文件开头，1为相对当前位置，2为相对文件结尾
		MustExist: false,                                //文件不存在不报错
		Poll:      true,                                 //轮询
	}
	tails, err := tail.TailFile(fileName, config)
	if err != nil {
		fmt.Println("tail file failed, err:", err)
		return
	}
	var (
		msg *tail.Line
		ok  bool
	)
	for {
		msg, ok = <-tails.Lines
		if !ok {
			fmt.Printf("tail file close reopen, filename:%s\n", tails.Filename)
			time.Sleep(time.Second)
			continue
		}
		fmt.Println("msg:", msg.Text)
	}
}

