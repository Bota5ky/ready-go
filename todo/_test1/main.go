package main

import (
	"fmt"
	"net/http"
	"text/template"
)

func main() {
	http.HandleFunc("/", hello)
	//ListenAndServe使用指定的监听地址和处理器启动一个HTTP服务端。
	//处理器参数通常是nil，这表示采用包变量DefaultServeMux作为处理器。
	//Handle和HandleFunc函数可以向DefaultServeMux添加处理器。
	err := http.ListenAndServe(":8080", nil)
	if err != nil {
		fmt.Println("HTTP serve start failed,err:", err)
		return
	}
}

func hello(w http.ResponseWriter, r *http.Request) {
	t, err := template.ParseFiles("./hello.tmpl")
	if err != nil {
		fmt.Println("parse tmpl files failed,err:", err)
		return
	}
	m:=map[string]interface{}{
		"name":"王睿",
		"gender":"male",
		"done":"吃饭",
		"age":18,
	}
	n:=map[string]interface{}{
		"name":"一名",
		"gender":"female",
		"done":"吃屎",
		"age":28,
	}
	t.Execute(w, map[string]interface{}{
		"u1":m,
		"u2":n,
	})
}
