package tokencmd

import (
	"context"
	"fmt"

	"github.com/lrya/sai-service-framework/grpc/grpcclient"
	"github.com/lrya/sai-service-framework/svc"
	"github.com/lrya/sai-token-service/proto/tokenpb"

	"google.golang.org/grpc"
)

// TODO: handle paging

func ListRevokedTokens() svc.ServiceConfig {
	return svc.WithCommandObject(
		"list-revoked-tokens",
		"Issue the ListRevokedTokens gRPC call",
		grpcclient.NewClientCommand(
			"token", "", nil,
			func(conn *grpc.ClientConn) error {
				client := tokenpb.NewTokenClient(conn)
				resp, err := client.ListRevokedTokens(context.Background(),
					&tokenpb.ListRevokedTokensRequest{})
				if err != nil {
					return err
				}
				fmt.Println(resp)
				return nil
			}),
	)
}
