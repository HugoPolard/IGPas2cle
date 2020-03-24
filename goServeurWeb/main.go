package main

import (
	"IGPas2cle/goServeurWeb/webServer"
	"sync"
)

func main() {
	wg :=sync.WaitGroup{}
	wg.Add(1)
	webServer.SetUpServer()
	go webServer.Listen()

	wg.Wait()


}
