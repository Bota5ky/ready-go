package main

import (
	"fmt"
	"net/http"
)

func main() {
	http.HandleFunc("/hello", f1)
	err := http.ListenAndServe(":8080", nil)
	if err != nil {
		fmt.Println("start http serve failed,err:", err)
		return
	}
}

func f1(w http.ResponseWriter, r *http.Request) {
	
}
