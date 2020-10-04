package conf

//AppConf 应用配置
type AppConf struct {
	KafkaConf   `ini:"kafka"`
	TaillogConf `ini:"taillog"`
}

//KafkaConf kafka配置
type KafkaConf struct {
	Address string `ini:"address"`
	Topic   string `ini:"topic"`
}

//TaillogConf tail配置
type TaillogConf struct {
	FileName string `ini:"filename"`
}
