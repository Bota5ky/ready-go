package main

import (
	"net/http"
	"path"

	"github.com/gin-gonic/gin"
)

func main() {
	r := gin.Default()
	r.LoadHTMLFiles("./index.html")
	r.GET("/index", func(c *gin.Context) {
		c.HTML(http.StatusOK, "index.html", nil)
	})
	r.POST("/upload", func(c *gin.Context) {
		//从请求中读取文件
		if f, err := c.FormFile("f1"); err != nil {
			//上传文件的文件名可以由用户自定义，所以可能包含非法字符串，为了安全起见，应该由服务端统一文件名规则
			c.JSON(http.StatusInternalServerError, gin.H{
				"error": err.Error(),
			})
		} else { //将读取到的文件保存到服务端本地
			//dst:=fmt.Sprintf("./%v", f.Filename)
			dst := path.Join("./", f.Filename)
			c.SaveUploadedFile(f, dst)
			c.JSON(http.StatusOK, gin.H{
				"status": "ok",
			})
		}
	})
	r.Run()
}
