package kafka

import (
	"fmt"
	"time"

	"github.com/Shopify/sarama"
)

type logData struct {
	topic string
	data  string
}

var (
	client      sarama.SyncProducer
	logDataChan chan *logData
)

//Init 初始化client
func Init(addrs []string, maxSize int) (err error) {
	config := sarama.NewConfig()
	config.Producer.RequiredAcks = sarama.WaitForAll          // 发送完数据需要leader和follow都确认
	config.Producer.Partitioner = sarama.NewRandomPartitioner // 新选出⼀个 partition
	config.Producer.Return.Successes = true                   // 成功交付的消息将在success channel返回

	// 连接Kafka
	client, err = sarama.NewSyncProducer(addrs, config)
	if err != nil {
		fmt.Println("producer closed,err:", err)
		return
	}
	//初始化全局的那个logData chan
	logDataChan = make(chan *logData, maxSize)
	//开启后台的goroutine从通道中取数据发往kafka
	go sendToKafka(logDataChan)
	return
}

//SendToKafka 发送到Kafka
func sendToKafka(logDataChan <-chan *logData) {
	for {
		select {
		case ld := <-logDataChan:
			//构造一个消息
			msg := &sarama.ProducerMessage{}
			msg.Topic = ld.topic
			msg.Value = sarama.StringEncoder(ld.data)
			//发送到Kafka
			_, _, err := client.SendMessage(msg)
			if err != nil {
				fmt.Println("send msg failed, err:", err)
				return
			}
			//fmt.Printf("pid:%v offset:%v\n", pid, offset)
			fmt.Printf("get log from d:/Seminar/LearnGo/ready-go/tmp/, log:%s\n", ld.data)
		default:
			time.Sleep(time.Millisecond * 50)
		}

	}
}

//SendToChan 先发送到通道等待发送到kafka
func SendToChan(topic, data string) {
	msg := &logData{
		topic: topic,
		data:  data,
	}
	logDataChan <- msg
}
