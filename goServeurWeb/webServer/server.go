package webServer

import (
	"bytes"
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"mime/multipart"
	"net/http"
	"os"
	"time"
)

func SetUpServer() {
	// gestion de la requête de verrouillage
	http.HandleFunc("/lock", func(w http.ResponseWriter, r *http.Request) {
		(w).Header().Set("Access-Control-Allow-Origin", "*")
		// lancer le script d'éxecution du verrouillage;
		fmt.Println(r.RemoteAddr) // traitement de l'adresse
		w.Header().Set("Content-type", "application/json")
		w.WriteHeader(http.StatusOK) //200
		_, _ = w.Write([]byte("{\"porte\":{\"locked\":true}}"))
	})
	// gestion de la requête de déverrouillage
	http.HandleFunc("/unlock", func(w http.ResponseWriter, r *http.Request) {
		(w).Header().Set("Access-Control-Allow-Origin", "*")

		// lancer le script d'éxecution du déverrouillage;
		fmt.Println(r.RemoteAddr)
		w.Header().Set("Content-type", "application/json")
		w.WriteHeader(http.StatusOK) //200
		_, _ = w.Write([]byte("{\"porte\":{\"locked\":false}}"))
	})

	http.HandleFunc("/card", func(w http.ResponseWriter, r *http.Request) {
		// on obtient une map[string][]string,
		strings := r.URL.Query()["uuid"]
		if len(strings) > 0 {
			uuid := strings[0]
			//image := "path/vers/photo/a/définir.png"
			image := "hugo.jpg"
			ts := time.Now()
			format := ts.Format("2006-01-02 15:04:05 MST")
			//fmt.Println(format, uuid, image)
			uploadFile(uuid, format, image)
		}
		//fmt.Println(strings)
	})
}

func Listen() {
	err := http.ListenAndServe(":80", nil)
	if err != nil {
		log.Fatal(err)
	}
}

func uploadFile(uuid, time, filename string) {
	fmt.Println(os.Getwd())
	file, err := os.Open(filename)
	if err != nil {
		log.Println(err)
		return
	}
	// On ferme le fichier plus tard
	defer file.Close()

	var requestBody bytes.Buffer

	mpw := multipart.NewWriter(&requestBody)
	// initialisation du foi
	fw, err := mpw.CreateFormFile("file_field", filename)
	if err != nil {
		log.Println(err)
		return
	}

	// Copy the actual file content to the field field's writer
	_, err = io.Copy(fw, file)
	if err != nil {
		log.Println(err)
		return
	}

	mpw.WriteField("uuid",uuid)
	mpw.WriteField("timestamp",time)

	// On ferme pour avoir le caratère de fermeture
	mpw.Close()

	req, err := http.NewRequest("POST", "http://localhost:8080/upload", &requestBody)
	if err != nil {
		log.Println(err)
		return
	}

	req.Header.Set("Content-Type", mpw.FormDataContentType())

	// Do the request
	client := &http.Client{}
	res, err := client.Do(req)
	if err != nil {
		log.Println(err)
		return
	}

	body, err := ioutil.ReadAll(res.Body)
	if err != nil {
		log.Println(err)
		return
	}
	fmt.Println(string(body))
	return
}
