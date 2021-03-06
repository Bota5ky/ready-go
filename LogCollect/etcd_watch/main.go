package main

import (
	"context"
	"fmt"
	"time"

	"go.etcd.io/etcd/clientv3"
)

// etcd client put/get demo
// use etcd/clientv3
func main() {
	cli, err := clientv3.New(clientv3.Config{
		Endpoints:   []string{"127.0.0.1:2379"},
		DialTimeout: 5 * time.Second,
	})
	if err != nil {
		// handle error!
		fmt.Printf("connect to etcd failed, err:%v\n", err)
		return
	}
	fmt.Println("connect to etcd success")
	defer cli.Close()
	ch := cli.Watch(context.Background(), "name")
	for wresp := range ch { //watch response
		for _, ev := range wresp.Events {
			fmt.Println("type:", ev.Type, "key:", string(ev.Kv.Key), "val:", string(ev.Kv.Value))
		}
	}
}
