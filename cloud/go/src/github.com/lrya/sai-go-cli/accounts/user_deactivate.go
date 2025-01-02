package accounts

import (
	"github.com/lrya/sai-go-cli/apiutil"
	"github.com/lrya/sai-go-cli/config"
	"github.com/jawher/mow.cli"
)

func DeactivateUsercommand(parent *cli.Cmd, cfg **config.Config) {
	parent.Command("deactivate-user", "Deactivate a single user", func(cmd *cli.Cmd) {
		cmd.Spec = "-u"

		userid := cmd.StringOpt("u userid", "", "User ID to deactivate")

		cmd.Action = func() {
			c := newClient(*cfg)

			apiutil.DefaultHandler(c.DeleteUser(*userid))
		}
	})
}
