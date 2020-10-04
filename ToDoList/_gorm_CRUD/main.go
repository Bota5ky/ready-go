package main

import (
	"fmt"

	"github.com/jinzhu/gorm"
	_ "github.com/jinzhu/gorm/dialects/mysql"
)

//User User 1.定义模型
type User struct {
	ID   int64
	Name string
	Age  int64
}

func main() {
	// 连接MYSQL数据库
	db, err := gorm.Open("mysql", "root:zjyzjy@(127.0.0.1:3306)/db1?charset=utf8mb4&parseTime=true&loc=Local")
	if err != nil {
		panic(err)
	}
	defer db.Close()
	// 2.把模型与数据库中的表对应起来
	db.AutoMigrate(&User{})
	// 3.创建
	u := User{Name: "qimi", Age: 18}
	fmt.Println(db.NewRecord(&u)) //判断主键是否为空  true
	db.Create(&u)
	fmt.Println(db.NewRecord(&u)) //判断主键是否为空  false
	
}
