package main

import (
	"net/http"

	"github.com/gin-gonic/gin"
	"github.com/jinzhu/gorm"
)

//Todo 待办事项
type Todo struct {
	ID     int    `json:"id"`
	Title  string `json:"title"`
	Status bool   `json:"status"`
}

func main() {
	//创建数据库
	//sql: CREATE DATABASE bubble;
	//连接数据库
	db, err := gorm.Open("mysql", "root:zjyzjy@(127.0.0.1:3306)/bubble?charset=utf8mb4&parseTime=true&loc=Local")
	if err != nil {
		panic(err)
	}
	defer db.Close()
	r := gin.Default()
	r.Static("/static", "./static")
	r.LoadHTMLFiles("./index.html")
	r.GET("/", func(c *gin.Context) {
		c.HTML(http.StatusOK, "index.html", nil)
	})
	//v1
	v1Group := r.Group("v1")
	{
		//增
		v1Group.POST("/todo", func(c *gin.Context){})
		//删
		v1Group.DELETE("/todo", func(c *gin.Context){})
		//改
		v1Group.PUT("/todo", func(c *gin.Context){})
		//查所有
		v1Group.GET("/todo", func(c *gin.Context){})
		//查看一个
		v1Group.GET("/todo/:id", func(c *gin.Context){})
	}
	//待办事项

	r.Run(":9090")
}
