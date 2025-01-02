package tokenmetrics

import (
	"github.com/lrya/sai-go-util/metricsutil"
)

var (
	Registry = metricsutil.NewRegistry("service.sai_token_service")
)
