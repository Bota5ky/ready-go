package v1

import "github.com/gin-gonic/gin"

//Tag 标签
type Tag struct{}

//NewTag 新标签
func NewTag() Tag {
	return Tag{}
}

//Get Get
func (t Tag) Get(c *gin.Context) {}

//List List
func (t Tag) List(c *gin.Context) {}

//Create Create
func (t Tag) Create(c *gin.Context) {}

//Update Update
func (t Tag) Update(c *gin.Context) {}

//Delete Delete
func (t Tag) Delete(c *gin.Context) {}
