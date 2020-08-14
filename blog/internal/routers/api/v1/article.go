package v1

import "github.com/gin-gonic/gin"

//Article  文章
type Article struct{}

//NewArticle 新文章
func NewArticle() Article {
	return Article{}
}

//Get Get
func (a Article) Get(c *gin.Context) {}

//List List
func (a Article) List(c *gin.Context) {}

//Create Create
func (a Article) Create(c *gin.Context) {}

//Update Update
func (a Article) Update(c *gin.Context) {}

//Delete Delete
func (a Article) Delete(c *gin.Context) {}
