package model

//Tag 标签表
type Tag struct {
	*Model
	Name  string `json:"name"`
	State uint8  `json:"state"`
}

//TableName 返回标签表名
func (a Tag) TableName() string {
	return "blog_tag"
}
