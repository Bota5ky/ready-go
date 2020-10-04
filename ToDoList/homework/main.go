package main

import (
	"net/http"

	"github.com/gin-gonic/gin"
)

func main() {
	r := gin.Default()
	r.Static("components/", "./components/")
	r.Static("images/", "./images/")
	r.LoadHTMLGlob("./*.html")
	r.GET("/index.html", func(c *gin.Context) {
		c.HTML(http.StatusOK, "index.html", nil)
	})
	r.GET("/cart.html", func(c *gin.Context) {
		c.HTML(http.StatusOK, "cart.html", nil)
	})
	r.Run(":9090")
}
