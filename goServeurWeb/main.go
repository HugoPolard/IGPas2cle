package main

import (
	"IGPas2cle/goServeurWeb/webServer"
	"bytes"
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"mime/multipart"
	"net/http"
	"os"
	"sync"
)

func main() {
	wg := sync.WaitGroup{}
	wg.Add(1)
	// serveur de gestion du blocage de la porte
	webServer.SetUpServer()
	go webServer.Listen()

	uploadFile()
	//return
	wg.Wait()

}

func uploadFile() {
	filename := "goServeurWeb/img.jpg"

	file, err := os.Open(filename)
	if err != nil {
		log.Fatalln(err)
	}
	// Close the file later
	defer file.Close()

	// Buffer to store our request body as bytes
	var requestBody bytes.Buffer

	// Create a multipart writer
	multiPartWriter := multipart.NewWriter(&requestBody)

	// Initialize the file field
	fileWriter, err := multiPartWriter.CreateFormFile("file_field", filename)
	if err != nil {
		log.Fatalln(err)
	}

	// Copy the actual file content to the field field's writer
	_, err = io.Copy(fileWriter, file)
	if err != nil {
		log.Fatalln(err)
	}

	// Populate other fields
	fieldWriter, err := multiPartWriter.CreateFormField("normal_field")
	if err != nil {
		log.Fatalln(err)
	}

	_, err = fieldWriter.Write([]byte("Value"))
	if err != nil {
		log.Fatalln(err)
	}

	// We completed adding the file and the fields, let's close the multipart writer
	// So it writes the ending boundary
	multiPartWriter.Close()

	// By now our original request body should have been populated, so let's just use it with our custom request
	req, err := http.NewRequest("POST", "http://localhost:8080/upload", &requestBody)
	if err != nil {
		log.Fatalln(err)
	}
	// We need to set the content type from the writer, it includes necessary boundary as well
	req.Header.Set("Content-Type", multiPartWriter.FormDataContentType())

	// Do the request
	client := &http.Client{}
	res, err := client.Do(req)
	if err != nil {
		log.Fatalln(err)
	}


	/*f, err := os.Open(file)
	if err != nil {
		log.Println(err)
		return
	}
	defer f.Close()

	fw, err := w.CreateFormFile("file", file)
	if err != nil {
		log.Println(err)
		return
	}
	_, err = io.Copy(fw, f)
	if err != nil {
		log.Println(err)
		return
	}

	if fw, err = w.CreateFormField("key"); err != nil {
		log.Println(err)
		return
	}
	if _, err = fw.Write([]byte("KEY")); err != nil {
		log.Println(err)
		return
	}

	w.Close()

	req, err := http.NewRequest("POST", "http://localhost/testfile", &b)
	if err != nil {
		log.Println(err)
		return
	}

	// Don't forget to set the content type, this will contain the boundary.
	req.Header.Set("Content-Type", w.FormDataContentType())

	// Submit the request
	client := &http.Client{}
	res, err := client.Do(req)
	if err != nil {
		log.Println(err)
		return
	}
	if res.StatusCode != http.StatusOK {
		err = fmt.Errorf("bad status: %s", res.Status)
	}*/
	body, err := ioutil.ReadAll(res.Body)
	if err != nil {
		panic(err)
	}
	fmt.Println(string(body))
	return
}
