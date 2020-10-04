package conf

//LogTransferCfg LogTransfer配置信息
type LogTransferCfg struct {
	KafkaCfg `ini:"kafka"`
	ESCfg    `ini:"es"`
}

//KafkaCfg Kafka配置信息
type KafkaCfg struct {
	Address string `ini:"address"`
	Topic   string `ini:"topic"`
}

//ESCfg ElasticSearch配置信息
type ESCfg struct {
	Address  string `ini:"address"`
	ChanSize int    `ini:"chan_size"`
	Nums     int    `ini:"nums"`
}
