package main

import (
	"log"
	"time"
)

func main() {
	location,_:=time.LoadLocation("America/New_York")
	input := "2029-09-04 12:02:33"
	layout := "2006-01-02 15:04:05"
	t, _ := time.ParseInLocation(layout, input, location)
	log.Printf("%v",time.Unix(t.Unix(), 0).Format(layout))
}
