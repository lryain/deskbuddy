// Copyright 2017 Lrya, Inc.

package main

import (
	"github.com/lrya/sai-chipper-voice/commands"
	"github.com/lrya/sai-chipper-voice/server/chipper"
	"github.com/lrya/sai-service-framework/svc"
)

func main() {
	svc.ServiceMain(chipper.ServiceName, "Chipper gRPC Service",
		svc.WithServiceCommand("start", "Run gRPC service", chipper.NewService()),
		commands.CreateTables(),
		commands.CheckTables(),
		commands.AddFwVersion(),
		commands.GetFwVersion(),
		commands.AddRatio(),
		commands.GetRatio(),
	)
}
