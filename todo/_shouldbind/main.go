package main

import (
	"fmt"
	"net/http"

	"github.com/gin-gonic/gin"
)

//UserInfo 用户信息
type UserInfo struct {
	Name     string `form:"username"`
	Password string `form:"password"`
}

func main() {
	r := gin.Default()
	var u UserInfo
	r.GET("/user", func(c *gin.Context) {
		err := c.ShouldBind(&u)
		if err != nil {
			c.JSON(http.StatusBadRequest, gin.H{
				"error": err.Error(),
			})
		} else {
			fmt.Printf("%#v", u)
			c.JSON(http.StatusOK, gin.H{
				"status": "OK",
			})
		}
	})
	r.Run()
}
