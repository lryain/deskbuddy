package accounts

import (
	"github.com/lrya/sai-go-accounts/client/accounts"
	"github.com/lrya/sai-go-cli/apiutil"
	"github.com/lrya/sai-go-cli/cliutil"
	"github.com/lrya/sai-go-cli/config"
)

func newClient(cfg *config.Config) *accounts.AccountsClient {
	apicfg, err := apiutil.ApiClientCfg(cfg, config.Accounts)
	if err != nil {
		cliutil.Fail(err.Error())
	}
	client, err := accounts.NewAccountsClient("sai-go-cli", apicfg...)
	if err != nil {
		cliutil.Fail(err.Error())
	}
	return client
}
