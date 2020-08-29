package main

//交替打印数值和字母
var chan1 = make(chan bool, 1)
var chan2 = make(chan bool, 1)
var end = make(chan bool, 1)

func f1() {
	for i := 1; i <= 26; i++ {
		<-chan1
		print(i)
		chan2 <- true
	}
}

func f2() {
	for i := 'A'; i <= 'Z'; i++ {
		<-chan2
		print(string(i))
		chan1 <- true
	}
	end <- true
}
func main() {
	chan1 <- true
	go f1()
	go f2()
	<-end
}
