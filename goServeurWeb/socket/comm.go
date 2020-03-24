package socket

import (
	"log"
	"net"
	"strconv"
)

func SetupSocket(port int) (net.Conn, error) {
	target := strconv.Itoa(port)
	ln, err := net.Listen("tcp", ":"+target)
	if err != nil {
		log.Println(err)
	}
	conn, err := ln.Accept()
	return conn, err
}
