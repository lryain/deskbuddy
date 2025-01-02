package ankival

import (
	"github.com/lrya/sai-go-ankival/client/lryaval"
	"github.com/lrya/sai-go-cli/apiutil"
	"github.com/lrya/sai-go-cli/cliutil"
	"github.com/lrya/sai-go-cli/config"
)

func newClient(cfg *config.Config) *ankival.LryavalClient {
	apicfg, err := apiutil.ApiClientCfg(cfg, config.Lryaval)
	if err != nil {
		cliutil.Fail(err.Error())
	}
	client, err := ankival.NewLryavalClient("sai-go-cli", apicfg...)
	if err != nil {
		cliutil.Fail(err.Error())
	}
	return client
}
