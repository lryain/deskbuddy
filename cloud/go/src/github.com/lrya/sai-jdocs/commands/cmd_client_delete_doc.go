package commands

import (
	"context"
	"fmt"

	"github.com/lrya/sai-go-util/log"
	"github.com/lrya/sai-jdocs/proto/jdocspb"
	"github.com/lrya/sai-service-framework/grpc/svc"
	"github.com/lrya/sai-service-framework/svc"

	"github.com/jawher/mow.cli"
	"google.golang.org/grpc"
)

func NewDeleteDocClientCommand() *grpcsvc.ClientCommand {
	var account, thing, name *string
	return grpcsvc.NewClientCommand(
		"jdocs", // prefix for args and env variables
		"[--account] [--thing] [--name]", // spec
		func(cmd *cli.Cmd) { // init
			account = svc.StringOpt(cmd, "account", "", "")
			thing = svc.StringOpt(cmd, "thing", "", "")
			name = svc.StringOpt(cmd, "name", "", "Document name")
		},
		func(conn *grpc.ClientConn) error {
			client := jdocspb.NewJdocsClient(conn)
			_, err := client.DeleteDoc(context.Background(),
				&jdocspb.DeleteDocReq{
					Account: *account,
					Thing:   *thing,
					DocName: *name,
				},
			)
			if err != nil {
				return err
			} else {
				alog.Info{
					"action": "delete-doc",
					"status": "success",
				}.Log()
				fmt.Printf("--------------------\n")
				fmt.Printf("Status = SUCCESS\n")
				fmt.Printf("--------------------\n")
			}
			return nil
		})
}
