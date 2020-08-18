package main

import (
	"github.com/jinzhu/gorm"
	_ "github.com/jinzhu/gorm/dialects/mysql"
)

//User User
type User struct {
	gorm.Model
	Name string
	Age  int64
}

func main() {
	db, err := gorm.Open("mysql", "root:zjyzjy@(127.0.0.1:3306)/db1?charset=utf8mb4&timeParse=true&loc=Local")
	if err != nil {
		panic(err)
	}
	defer db.Close()
	db.AutoMigrate(&User{})

	var user User
	// 根据主键查询第一条记录
	db.First(&user)
	//// SELECT * FROM users ORDER BY id LIMIT 1;

	// 随机获取一条记录
	db.Take(&user)
	//// SELECT * FROM users LIMIT 1;

	// 根据主键查询最后一条记录
	db.Last(&user)
	//// SELECT * FROM users ORDER BY id DESC LIMIT 1;

	// 查询所有的记录
	var users []User
	db.Find(&users)
	//// SELECT * FROM users WHERE users.deleted_at IS NULL;
	
	// 查询指定的某条记录(仅当主键为整型时可用)
	db.First(&user, 10)
	//// SELECT * FROM users WHERE id = 10;
}
