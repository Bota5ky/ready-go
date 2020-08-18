package main

import (
	"database/sql"
	"fmt"
	"time"

	"github.com/jinzhu/gorm"
	_ "github.com/jinzhu/gorm/dialects/mysql"
)

//UserInfo 用户信息 gorm.Model定义
type UserInfo struct {
	ID     uint
	Name   string
	Gender string
	Hobby  string
}

//User 定义模型
type User struct {
	gorm.Model
	Name         string
	Age          sql.NullInt64
	Birthday     *time.Time
	Email        string  `gorm:"type:varchar(100);unique_index"`
	Role         string  `gorm:"size:255"`        // 设置字段大小为255
	MemberNumber *string `gorm:"unique;not null"` // 设置会员号（member number）唯一并且不为空
	Num          int     `gorm:"AUTO_INCREMENT"`  // 设置 num 为自增类型
	Address      string  `gorm:"index:addr"`      // 给address字段创建名为addr的索引
	IgnoreMe     int     `gorm:"-"`               // 忽略本字段
}

func main() {
	//db, err := gorm.Open("mysql", "user:password@TCP(localhost)/dbname?charset=utf8mb4&parseTime=True&loc=Local")
	db, err := gorm.Open("mysql", "root:zjyzjy@(127.0.0.1:3306)/db1?charset=utf8mb4&parseTime=True&loc=Local")
	if err != nil {
		fmt.Printf("connect MYSQL failed, err:%v", err)
		return
	}
	defer db.Close()
	//创建表 自动迁移（把结构体和数据表进行对应）
	db.AutoMigrate(&UserInfo{})
	//创建数据行
	// u1:=UserInfo{1,"qimi","female","乒乓球"}
	// db.Create(&u1)
	//查询
	var u UserInfo
	db.First(&u)
	fmt.Printf("u:%#v", u)
	//更新
	db.Model(&u).Update("hobby", "双色球")
	//删除
	db.Delete(&u)
}
