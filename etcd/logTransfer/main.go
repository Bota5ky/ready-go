package main

import (
	"fmt"
	"logTransfer/conf"
	"logTransfer/es"
	"logTransfer/kafka"

	"gopkg.in/ini.v1"
)

// log transfer
// 将日志从kafka取出来发往ES

func main() {
	//0.加载配置文件
	var cfg conf.LogTransferCfg
	err := ini.MapTo(&cfg, "conf/cfg.ini")
	if err != nil {
		fmt.Println("init config, err:", err)
		return
	}
	//fmt.Println(cfg)
	//1.初始化 es
	err = es.Init(cfg.ESCfg.Address, cfg.ESCfg.ChanSize, cfg.ESCfg.Nums)
	if err != nil {
		fmt.Println("init elastic search failed, err:", err)
		return
	}
	fmt.Println("init es client success")
	//2.初始化 kafka
	// 连接kafka，创建分区的消费者
	// 每个分区的消费者分别取出数据，通过sentoES发往elasticsearch
	err = kafka.Init([]string{cfg.KafkaCfg.Address}, cfg.KafkaCfg.Topic)
	if err != nil {
		fmt.Println("kafka consumer init failed, err:", err)
		return 
	}
	fmt.Println("init kafka consumer success")
	select{}
	//3.从kafka取日志数据
	//4.发往ES
}
