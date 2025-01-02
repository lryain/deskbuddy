// +build !shipping

package cloudproc

import (
	"lrya/cloudproc/dev"
	"lrya/token"
	"net/http"
)

var serveMux *http.ServeMux

func init() {
	serveMux = http.NewServeMux()
	devServer = launchServer
	addHandlerFunc = func(f func(*http.ServeMux), s *token.Server) {
		f(serveMux)
		token.TokenServer = s
	}
}

func launchServer() error {
	fs := http.FileServer(http.Dir("/lrya/data/assets/cozmo_resources/webserver/cloud"))
	serveMux.Handle("/", fs)
	dev.Init()
	dev.AddHandlers(serveMux)
	return http.ListenAndServe(":8890", serveMux)
}
