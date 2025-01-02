package blobstore

import (
	"github.com/lrya/sai-blobstore/client/blobstore"
	"github.com/lrya/sai-go-cli/apiutil"
	"github.com/lrya/sai-go-cli/cliutil"
	"github.com/lrya/sai-go-cli/config"
)

func newClient(cfg *config.Config) *blobstore.Client {
	apicfg, err := apiutil.ApiClientCfg(cfg, config.Blobstore)
	if err != nil {
		cliutil.Fail(err.Error())
	}
	client, err := blobstore.New("sai-go-cli", apicfg...)
	if err != nil {
		cliutil.Fail(err.Error())
	}
	return client
}
