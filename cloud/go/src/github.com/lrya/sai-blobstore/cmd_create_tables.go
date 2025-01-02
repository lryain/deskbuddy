package main

import (
	"fmt"

	"github.com/lrya/sai-go-util/cli"
	"github.com/lrya/sai-go-util/dynamoutil"
)

// Define the command that creates Dynamo tables
// test purposes only.
var createTablesCmd = &Command{
	Name:       "create-tables",
	RequiresDb: true,
	Exec: func(cmd *Command, args []string) {
		setupDb(false)
		fmt.Printf("Creating tables..")
		if err := dynamoutil.CreateTables(); err != nil {
			fmt.Println("Failed: ", err)
			cli.Exit(3)
		}
		fmt.Println("OK")
	},
}
