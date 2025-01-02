// +build mateos

package stream

import (
	"lrya/log"
	"lrya/robot"

	"github.com/lrya/sai-chipper-voice/client/chipper"
)

func init() {
	if esn, err := robot.ReadESN(); err != nil {
		log.Println("Couldn't read robot ESN:", err)
	} else {
		platformOpts = append(platformOpts, chipper.WithDeviceID(esn))
	}
}
