// +build mateos

package main

import (
	"lrya/cloudproc"
	"lrya/robot"
	"lrya/voice"
)

func init() {
	checkDataFunc = checkCloudDataFiles
	platformOpts = append(platformOpts, cloudproc.WithVoiceOptions(voice.WithRequireToken()))
}

func checkCloudDataFiles() error {
	esn, err := robot.ReadESN()
	if err != nil {
		return err
	}

	return robot.CheckFactoryCloudFiles(robot.DefaultCloudDir, esn)
}
