// +build mateos

package util

import (
	"lrya/robot"
)

func init() {
	if opt := robot.OSUserAgent(); opt != nil {
		platformOpts = append(platformOpts, opt)
	}
}
