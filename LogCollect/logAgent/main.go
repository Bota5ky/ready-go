package main

import (
	"fmt"
	"logAgent/conf"
	"logAgent/etcd"
	"logAgent/kafka"
	"logAgent/taillog"
	"logAgent/utils"
	"sync"
	"time"

	"gopkg.in/ini.v1"
)

var cfg = new(conf.AppConf)

//启动Kafka  在Kafka目录下
//zookeeper维护kafka集群列表，有多少台机器是由它来登记的
//	bin\windows\zookeeper-server-start.bat config\zookeeper.properties
//启动kafka
//	bin\windows\kafka-server-start.bat config\server.properties
//kafka终端做消费
//	bin\windows\kafka-console-consumer.bat --bootstrap-server=127.0.0.1:9092 --topic=web_log --from-beginning
//etcd 设置环境变量 SET ETCDCTL_API=3 // Windows
//	etcdctl.exe --endpoints=http://127.0.0.1:2379 put key value
func main() {
	// 0. 加载配置文件
	err := ini.MapTo(cfg, "./conf/config.ini")
	if err != nil {
		fmt.Printf("load ini failed, err:%v\n", err)
		return
	}

	// 1. 初始化kafka连接
	err = kafka.Init([]string{cfg.KafkaConf.Address}, cfg.KafkaConf.MaxSize)
	if err != nil {
		fmt.Printf("init Kafka failed,err:%v\n", err)
		return
	}
	fmt.Println("init kafka success.")

	// 2. 初始化etcd连接
	err = etcd.Init(cfg.EtcdConf.Address, time.Duration(cfg.EtcdConf.Timeout)*time.Second)
	if err != nil {
		fmt.Printf("init Etcd failed,err:%v\n", err)
		return
	}
	fmt.Println("init etcd success.")
	//为了实现每个logAgent都拉取自己独有的配置，所以要以自己的IP地址作为区分
	ipStr, err := utils.GetOutboundIP()
	if err != nil {
		panic(err)
	}
	etcdConfKey := fmt.Sprintf(cfg.EtcdConf.Key, ipStr)
	//2.1 从etcd根据key获取配置项
	logEntryConf, err := etcd.GetConf(etcdConfKey)
	if err != nil {
		fmt.Println("get logEntryConf failed, err:", err)
		return
	}
	fmt.Println("get logEntryConf success")
	//2.2派一个哨兵去监视日志收集项（有变化及时通知logAgent实现热加载配置）
	for k, v := range logEntryConf {
		fmt.Println("key:", k, "val:", v)
	}

	//3.根据以上的 v中的日志地址和 topic收集日志发往 kafka
	taillog.Init(logEntryConf)
	//因为NewConfChan访问了tskMgr的newConfChan,这个channel是在taillog。Init(logEntryConf)执行的初始化
	newConfChan := taillog.NewConfChan() //获取对外暴露的通道
	var wg sync.WaitGroup
	wg.Add(1)
	go etcd.WatchConf(etcdConfKey, newConfChan) //哨兵发现最新的配置信息会通知上面的通道
	wg.Wait()
	//4.具体的业务
}
