package main

import (
	"net/http"

	"github.com/gin-gonic/gin"
	"github.com/jinzhu/gorm"
	_ "github.com/jinzhu/gorm/dialects/mysql"
)

//Todo 待办事项
type Todo struct {
	ID     int    `json:"id"`
	Title  string `json:"title"`
	Status bool   `json:"status"`
}

//TableName 表名
func (Todo) TableName() string {
	return "todo"
}

var (
	//DB 数据库
	DB *gorm.DB
)

func initMySQL() (err error) {
	dsn := "root:zjyzjy@tcp(127.0.0.1:3306)/bubble?charset=utf8mb4&parseTime=true&loc=Local"
	DB, err = gorm.Open("mysql", dsn)
	if err != nil {
		return
	}
	err = DB.DB().Ping() //ping不通会返回错误，通返回nil
	return
}

func main() {
	//创建数据库
	//sql: CREATE DATABASE bubble;
	//连接数据库
	err := initMySQL()
	if err != nil {
		panic(err)
	}
	defer DB.Close() //程序退出关闭数据库连接
	//模型绑定
	DB.AutoMigrate(&Todo{}) //todos

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
		v1Group.POST("/todo", func(c *gin.Context) {
			//前端页面填写待办事项 点击提交 会发请求到这里
			//1.从请求中把数据拿出来
			var todo Todo
			c.BindJSON(&todo)
			//2.存入数据库
			//3.返回响应
			if err = DB.Create(&todo).Error; err != nil {
				c.JSON(http.StatusOK, gin.H{"error": err.Error()})
			} else {
				c.JSON(http.StatusOK, todo)
			}
		})
		//删
		v1Group.DELETE("/todo/:id", func(c *gin.Context) {
			id, ok := c.Params.Get("id")
			if !ok {
				c.JSON(http.StatusOK, gin.H{"error": "invalid ID"})
				return
			} else {
				if err=DB.Where("id=?", id).Delete(Todo{}).Error;err!=nil {
					c.JSON(http.StatusOK, gin.H{"error":err.Error()})
				}else{
					c.JSON(http.StatusOK, gin.H{id:"deleted!"})
				}
			}
		})
		//改
		v1Group.PUT("/todo/:id", func(c *gin.Context) {
			id, ok := c.Params.Get("id")
			if !ok {
				c.JSON(http.StatusOK, gin.H{"error": "invalid ID"})
				return
			}
			var todo Todo
			if err = DB.Where("id=?", id).First(&todo).Error; err != nil {
				c.JSON(http.StatusOK, gin.H{"error": err.Error()})
				return
			}
			c.BindJSON(&todo)
			if err = DB.Save(&todo).Error; err != nil {
				c.JSON(http.StatusOK, gin.H{"error": err.Error()})
			} else {
				c.JSON(http.StatusOK, todo)
			}
		})
		//查所有
		v1Group.GET("/todo", func(c *gin.Context) {
			//查询todo这个表里所有的数据
			var todoList []Todo
			if err = DB.Find(&todoList).Error; err != nil {
				c.JSON(http.StatusOK, gin.H{"error": err.Error()})
				return 
			} else {
				c.JSON(http.StatusOK, todoList)
			}
		})
		//查看一个
		v1Group.GET("/todo/:id", func(c *gin.Context) {})
	}
	//待办事项

	r.Run(":9090")
}
