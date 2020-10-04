package conf

//AppConf 应用配置
type AppConf struct {
	KafkaConf `ini:"kafka"`
	EtcdConf  `ini:"etcd"`
}

//EtcdConf etcd配置
type EtcdConf struct {
	Address string `ini:"address"`
	Key     string `ini:"key"`
	Timeout int    `ini:"timeout"`
}

//KafkaConf kafka配置
type KafkaConf struct {
	Address string `ini:"address"`
	MaxSize int    `ini:"max_chan_size"`
	//Topic   string `ini:"topic"`
}

//-------------unused---------------

//TaillogConf tail配置
type TaillogConf struct {
	FileName string `ini:"filename"`
}
