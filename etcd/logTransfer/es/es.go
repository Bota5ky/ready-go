package es

import (
	"context"
	"fmt"
	"strings"
	"time"

	"github.com/olivere/elastic/v7"
)

//LogData LogData
type LogData struct {
	Topic string `json:"topic"`
	Data  string `json:"data"`
}

var (
	client *elastic.Client
	ch     chan *LogData
)

//Init 初始化es
func Init(addr string, chanSize, nums int) (err error) {
	if !strings.HasPrefix(addr, "http://") {
		addr = "http://" + addr
	}
	client, err = elastic.NewClient(elastic.SetURL(addr))
	if err != nil {
		// Handle error
		return
	}
	fmt.Println("connect to es success")
	ch = make(chan *LogData, chanSize)
	for i := 0; i < nums; i++ {
		go sendToES()
	}
	return
}

//SendToES 发送数据到ES
func SendToES(msg *LogData) {
	ch <- msg
}

func sendToES() {
	for {
		select {
		case msg := <-ch:
			put1, err := client.Index().Index(msg.Topic).Type("zjy").BodyJson(msg).Do(context.Background()) //json格式序列化
			if err != nil {
				// Handle error
				fmt.Println("Handle error:", err)
				continue
			}
			fmt.Printf("Indexed user %s to index %s, type %s\n", put1.Id, put1.Index, put1.Type)
		default:
			time.Sleep(time.Second)
		}
	}
}
