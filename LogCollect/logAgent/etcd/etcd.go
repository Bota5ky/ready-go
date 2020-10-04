package etcd

import (
	"context"
	"encoding/json"
	"fmt"
	"time"

	"go.etcd.io/etcd/clientv3"
)

var (
	cli *clientv3.Client
)

//LogEntry 多日志配置信息
type LogEntry struct {
	Path  string `json:"path"`  //日志存放的路径
	Topic string `json:"topic"` //日志要发往kafka的哪个topic
}

//Init etcd初始化
func Init(addr string, timeout time.Duration) (err error) {
	cli, err = clientv3.New(clientv3.Config{
		Endpoints:   []string{addr},
		DialTimeout: timeout,
	})
	if err != nil {
		// handle error!
		fmt.Printf("connect to etcd failed, err:%v\n", err)
		return
	}
	return
}

//GetConf 从etcd根据key获取配置项
func GetConf(key string) (logEntryConf []*LogEntry, err error) {
	//get
	ctx, cancel := context.WithTimeout(context.Background(), time.Second)
	resp, err := cli.Get(ctx, key)
	cancel()
	if err != nil {
		fmt.Println("get from etcd failed,err:", err)
		return
	}
	for _, ev := range resp.Kvs {
		//fmt.Println("key:", ev.Key, "val:", ev.Value)
		err = json.Unmarshal(ev.Value, &logEntryConf)
		if err != nil {
			fmt.Println("Unmarshal json failed,err:", err)
			return
		}
	}
	return
}

//WatchConf 监视Conf变化
func WatchConf(key string, newConfCh chan<- []*LogEntry) {
	ch := cli.Watch(context.Background(), key)
	for wresp := range ch { //watch response
		for _, ev := range wresp.Events {
			fmt.Println("type:", ev.Type, "key:", string(ev.Kv.Key), "val:", string(ev.Kv.Value))
			//有变化通知别人 通知taillog.tskMgr
			//1.先判断操作的类型
			var newConf []*LogEntry
			if ev.Type != clientv3.EventTypeDelete {
				err := json.Unmarshal(ev.Kv.Value, &newConf)
				if err != nil {
					fmt.Println("unmarshal failed,err:", err)
					continue
				}
			}
			fmt.Println("get new conf:", newConf)
			newConfCh <- newConf //若是删除操作，手动传递一个空的配置项
		}
	}
}
