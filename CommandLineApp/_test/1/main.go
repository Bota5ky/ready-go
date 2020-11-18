package main

import (
	"errors"
	"flag"
	"fmt"
	"log"
)

//Name 姓名
type Name string

func (n *Name) String() string {
	return fmt.Sprint(*n)
}

//Set Set
func (n *Name) Set(value string) error {
	if len(*n) > 0 {
		return errors.New("name flag already set")
	}
	*n = Name("header:" + value)
	return nil
}

func main() {
	var name Name
	flag.Var(&name, "name", "帮助信息")
	flag.Parse()
	log.Printf("name:%s", name)
}
