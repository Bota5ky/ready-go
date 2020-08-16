package main

import (
	"html/template"
	"net/http"

	"github.com/gin-gonic/gin"
)

func main() {
	r := gin.Default()
	// r.SetFuncMap(template.FuncMap{
	// 	"safe": func(str string) template.HTML {
	// 		return template.HTML(str)
	// 	},
	// })
	//加载静态文件
	r.Static("/xxx", "./statics")
	r.LoadHTMLGlob("templates/**/*")
	r.GET("/index", func(c *gin.Context) {
		c.HTML(http.StatusOK, "posts/index.html", gin.H{
			"title": template.HTML("<a href='https://liwenzhou.com'>李文周的博客</a>"),
		})
	})
	r.Run(":9090")
}