package main

import (
	"net/http"

	"github.com/gin-gonic/gin"
)

func main() {
	r := gin.Default()
	r.LoadHTMLGlob("./*.html")
	r.GET("/form", func(c *gin.Context) {
		c.HTML(200, "login.html", nil)
	})
	r.POST("/form", func(c *gin.Context) {
		username := c.PostForm("username")
		password := c.PostForm("password")
		c.HTML(http.StatusOK, "index.html", gin.H{
			"Name":     username,
			"Password": password,
		})
	})
	r.Run(":8006")
}
