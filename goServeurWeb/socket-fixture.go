package main

import (
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
)

func main() {

	http.HandleFunc("/upload", func(w http.ResponseWriter, r *http.Request) {
		if r.Method == "POST" {
			err := r.ParseMultipartForm(20<<10)
			if err != nil {
				panic(err)
			}
			log.Println(r.Form)

			file ,h ,err:= r.FormFile("file_field")

			defer file.Close()
			fmt.Printf("Uploaded File: %+v\n", h.Filename)
			fmt.Printf("File Size: %+v\n", h.Size)
			fmt.Printf("MIME Header: %+v\n", h.Header)

			tempFile, err := ioutil.TempFile("goServeurWeb/temp-images", "upload-*.png")
			if err != nil {
				fmt.Println(err)
			}
			defer tempFile.Close()

			// read all of the contents of our uploaded file into a
			// byte array
			fileBytes, err := ioutil.ReadAll(file)
			if err != nil {
				fmt.Println(err)
			}
			// write this byte array to our temporary file
			tempFile.Write(fileBytes)
		}
	})
	http.ListenAndServe(":8080",nil)
}
