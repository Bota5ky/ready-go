package main

import (
	"fmt"
	"logAgent/conf"
	"logAgent/kafka"
	"logAgent/taillog"
	"time"

	"gopkg.in/ini.v1"
)

var cfg = new(conf.AppConf)

//启动Kafka  在Kafka目录下
//zookeeper维护kafka集群列表，有多少台机器是由它来登记的
//bin\windows\zookeeper-server-start.bat config\zookeeper.properties
//启动kafka
//bin\windows\kafka-server-start.bat config\server.properties
//kafka终端做消费
//bin\windows\kafka-console-consumer.bat --bootstrap-server=127.0.0.1:9092 --topic=web_log --from beginning
func main() {
	// 0. 加载配置文件
	err := ini.MapTo(cfg, "./conf/config.ini")
	if err != nil {
		fmt.Printf("load ini failed, err:%v\n", err)
		return
	}
	// 1. 初始化kafka连接
	err = kafka.Init([]string{cfg.KafkaConf.Address})
	if err != nil {
		fmt.Printf("init Kafka failed,err:%v\n", err)
		return
	}
	fmt.Println("init kafka success.")
	// 2. 打开日志文件准备收集日志
	err = taillog.Init(cfg.TaillogConf.FileName)
	if err != nil {
		fmt.Printf("Init taillog failed,err:%v\n", err)
		return
	}
	fmt.Println("init taillog success.")
	run()
}

func run() {
	//1.read log
	for {
		select {
		case line := <-taillog.ReadChan():
			//2.send msg to kafka
			kafka.SendToKafka(cfg.KafkaConf.Topic, line.Text)
		default:
			time.Sleep(time.Second)
		}
	}
}
