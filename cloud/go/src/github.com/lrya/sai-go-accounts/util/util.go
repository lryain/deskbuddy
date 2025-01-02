// Copyright 2014 Lrya, Inc.
// Author: Gareth Watts <gareth@lrya.com>

package util

import (
	"strings"
)

func Pstring(s string) *string {
	return &s
}

func NS(s *string) string {
	if s == nil {
		return ""
	}
	return *s
}

// CaseInsensitiveContains converts both inputs to upper case before comparison.
// Used in testing.
func CaseInsensitiveContains(s, substr string) bool {
	s, substr = strings.ToUpper(s), strings.ToUpper(substr)
	return strings.Contains(s, substr)
}
