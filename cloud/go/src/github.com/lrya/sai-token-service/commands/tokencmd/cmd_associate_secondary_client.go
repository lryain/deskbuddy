package tokencmd

import (
	"context"
	"fmt"
	"os"

	"github.com/lrya/sai-service-framework/grpc/grpcclient"
	"github.com/lrya/sai-service-framework/grpc/grpcutil"
	"github.com/lrya/sai-service-framework/svc"
	"github.com/lrya/sai-token-service/proto/tokenpb"

	"github.com/jawher/mow.cli"
	"google.golang.org/grpc"
)

func AssociateSecondaryClient() svc.ServiceConfig {
	var userSession, clientName, appId *string

	return svc.WithCommandObject(
		"associate-secondary-client",
		"Issue the AssociateSecondaryClient gRPC call",
		grpcclient.NewClientCommand(
			"token",
			"--robot-id --user-session --client-name [--app-id]",
			func(cmd *cli.Cmd) {
				userSession = cmd.StringOpt("u user-session", "", "Accounts user session")
				appId = cmd.StringOpt("a app-id", "sai-token-service", "Human readable application name for identifying tokens")
				clientName = cmd.StringOpt("c client-name", "", "Human readable device name for identifying tokens")
			},
			func(conn *grpc.ClientConn) error {
				client := tokenpb.NewTokenClient(conn)
				clientNm := *clientName
				if clientNm == "" {
					hostname, _ := os.Hostname()
					clientNm = hostname
				}

				resp, err := client.AssociateSecondaryClient(grpcutil.WithUserSessionKey(context.Background(), *userSession),
					&tokenpb.AssociateSecondaryClientRequest{
						UserSession: *userSession,
						ClientName:  clientNm,
						AppId:       *appId,
					})
				if err != nil {
					return err
				}
				fmt.Println(resp)
				return nil
			}),
	)
}
