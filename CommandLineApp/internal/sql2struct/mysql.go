package sql2struct

import (
	"database/sql"
	"fmt"

	_ "github.com/go-sql-driver/mysql" //init
)

//DBModel 整个数据库连接的核心对象
type DBModel struct {
	DBEngine *sql.DB
	DBInfo   *DBInfo
}

//DBInfo 储存链接MYSQL的基本信息
type DBInfo struct {
	DBType   string
	Host     string
	UserName string
	Password string
	Charset  string
}

//TableColumn 储存字段
type TableColumn struct {
	ColumnName    string
	DataType      string
	IsNullable    string
	ColumnKey     string
	ColumnType    string
	ColumnComment string
}

//NewDBModel 初始化方法
func NewDBModel(info *DBInfo) *DBModel {
	return &DBModel{DBInfo: info}
}

//Connect 连接
func (m *DBModel) Connect() error {
	var err error
	dsn := fmt.Sprintf("%s:%s@tcp(%s)/information_schema?charset=%s&parseTime=True&loc=Local",
		m.DBInfo.UserName,
		m.DBInfo.Password,
		m.DBInfo.Host,
		m.DBInfo.Charset,
	)
	m.DBEngine, err = sql.Open(m.DBInfo.DBType, dsn)
	if err!=nil {
		return err
	}
	return nil
}
