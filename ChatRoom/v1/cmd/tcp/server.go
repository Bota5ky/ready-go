package main

import (
	"bufio"
	"fmt"
	"log"
	"net"
	"strconv"
	"sync"
	"time"
)

var (
	// 新用户到来，通过该channel进行登记
	enteringChannel = make(chan *User)
	// 用户离开，通过该channel进行登记
	leavingChannel = make(chan *User)
	// 广播专用的用户普通消息 channel，缓冲是尽可能避免出现异常情况阻塞，这里简单给了8，
	// 具体根据情况调整
	messageChannel = make(chan string, 8)
)

//User 用户信息
type User struct {
	ID             int
	Addr           string
	EnterAt        time.Time
	MessageChannel chan string
}

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

// broadcaster 用于记录聊天室用户，并进行消息广播：
// 1.新用户进来; 2.用户普通消息; 3.用户离开
func broadcaster() {
	users := make(map[*User]struct{})
	for {
		select {
		case user := <-enteringChannel:
			//新用户进入
			users[user] = struct{}{}
		case user := <-leavingChannel:
			//用户离开
			delete(users, user)
			//避免gorotine泄露
			close(user.MessageChannel)
		case msg := <-messageChannel:
			//给所有在线用户发送消息
			for user := range users {
				user.MessageChannel <- msg
			}
		}
	}
}

func handleConn(conn net.Conn) {
	defer conn.Close()
	//1.新用户进来，构建该用户的实例
	user := &User{
		ID:             GenUserID(),
		Addr:           conn.RemoteAddr().String(),
		EnterAt:        time.Now(),
		MessageChannel: make(chan string, 8),
	}
	//2.由于当前是在一个新的gorutine中进行读操作的，所以需要开一个gorutine用于写操作。
	//读写gorutine之间可以通过channel进行通信
	go sendMessage(conn, user.MessageChannel)
	//3.给当前用户发送欢迎信息，向所有用户告知新用户到来
	user.MessageChannel <- "Welcome," + user.String()
	messageChannel <- "user:`" + strconv.Itoa(user.ID) + "` has enter"
	//4.记录到全局用户列表中，避免用锁
	enteringChannel <- user
	//5.循环读入用户输入
	input := bufio.NewScanner(conn)
	for input.Scan() {
		messageChannel <- strconv.Itoa(user.ID) + ":" + input.Text()
	}
	if err := input.Err(); err != nil {
		log.Println("读取错误：", err)
	}
	//6.用户离开
	leavingChannel <- user
	messageChannel <- "user:`" + strconv.Itoa(user.ID) + "` has left"
}

func sendMessage(conn net.Conn, ch <-chan string) {
	for msg := range ch {
		fmt.Fprintln(conn, msg)
	}
}

func (u *User) String() string {
	return u.Addr + ", UID:" + strconv.Itoa(u.ID) + ", Enter At:" +
		u.EnterAt.Format("2006-01-02 15:04:05+8000")
}

// 生成用户 ID
var (
	globalID int
	idLocker sync.Mutex
)

//GenUserID 生成用户ID
func GenUserID() int {
	idLocker.Lock()
	defer idLocker.Unlock()

	globalID++
	return globalID
}