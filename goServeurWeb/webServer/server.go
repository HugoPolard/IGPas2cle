package webServer

import (
	"fmt"
	"log"
	"net/http"
)

func SetUpServer() {
	http.HandleFunc("/lock", func(w http.ResponseWriter, r *http.Request) {
		// lancer le script d'éxecution du verrouillage;
		fmt.Println(r.RemoteAddr) // traitement de l'adresse
		w.Header().Set("Content-type", "application/json")
		w.WriteHeader(http.StatusOK) //200
		_, _ = w.Write([]byte("{\"porte\":{\"locked\":true}}"))
	})
	http.HandleFunc("/unlock", func(w http.ResponseWriter, r *http.Request) {
		// lancer le script d'éxecution du déverrouillage;
		fmt.Println(r.RemoteAddr)
		w.Header().Set("Content-type", "application/json")
		w.WriteHeader(http.StatusOK) //200
		_, _ = w.Write([]byte("{\"porte\":{\"locked\":false}}"))
	})
}

func Listen(){
	err := http.ListenAndServe(":80", nil)
	if err != nil {
		log.Fatal(err)
	}
}