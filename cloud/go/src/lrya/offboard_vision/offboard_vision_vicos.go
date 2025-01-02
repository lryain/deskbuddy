// +build !shipping,mateos

package offboard_vision

import (
	"lrya/log"
	"lrya/robot"
)

func init() {
	if esn, err := robot.ReadESN(); err != nil {
		log.Println("Couldn't read robot ESN:", err)
	} else {
		deviceID = esn
	}
}
