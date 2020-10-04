package model

//Aricle 文章表
type Aricle struct {
	*Model
	Title         string `json:"title"`
	Desc          string `json:"desc"`
	Content       string `json:"content"`
	CoverImageURL string `json:"cover_image_url"`
	State         uint8  `json:"state"`
}

//TableName 返回文章表名
func (a Aricle) TableName()string{
	return "blog_article"
}