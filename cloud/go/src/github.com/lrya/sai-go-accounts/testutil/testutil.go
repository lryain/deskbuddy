// Copyright 2014 Lrya, Inc.
// Author: Gareth Watts <gareth@lrya.com>

// This should be imported into all unit tests
package testutil

import (
	"github.com/lrya/sai-go-accounts/email"
	"github.com/lrya/sai-go-util/log"
	"github.com/lrya/sai-go-util/postmarkapp"
)

func init() {
	alog.ToStdout()
}

// CaptureEmails runs a function and captures all of the messages it generates
func CaptureEmails(f func() error) (msgs []*postmarkapp.Message, err error) {
	email.InitLryaEmailer()
	defer email.InitLryaEmailer()
	pmc := email.Emailer.PostmarkC.(*postmarkapp.Client)
	pmc.LastEmail = make(chan *postmarkapp.Message)
	pmc.LastBatchEmail = make(chan []*postmarkapp.Message)
	done := make(chan error)
	go func() {
		done <- f()
	}()
	for {
		select {
		case msg := <-pmc.LastEmail:
			msgs = append(msgs, msg)
		case batch := <-pmc.LastBatchEmail:
			msgs = append(msgs, batch...)
		case err = <-done:
			return msgs, err
		}
	}
}
