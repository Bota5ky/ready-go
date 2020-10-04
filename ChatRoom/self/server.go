package main

import (
	"log"
	"net"
)

func main() {
	listener, err := net.Listen("tcp", ":2020")
	if err != nil {
		panic(err)
	}
	go broadcaster()
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Println(err)
			continue
		}
		go handleConn(conn)
	}
}

//1.new user 2.send message 3.user quit
func broadcaster(){

}

func handleConn(conn net.Conn){
	
}