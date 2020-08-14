package model

//ArticleTag 文章标签关联表
type ArticleTag struct {
	*Model
	TagID     uint32 `json:"tag_id"`
	ArticleID uint32 `json:"article_id"`
}

//TableName 返回文章标签关联表名
func (a ArticleTag) TableName() string {
	return "blog_article_tag"
}
