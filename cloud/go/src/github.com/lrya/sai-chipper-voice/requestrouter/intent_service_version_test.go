package requestrouter

import (
	"github.com/stretchr/testify/assert"
	"testing"

	pb "github.com/lrya/sai-chipper-voice/proto/lrya/chipperpb"
)

type testVersion struct {
	intentSvc  pb.IntentService
	dfProj     string
	dfVersion  string
	lexName    string
	lexVersion string
	err        error
}

func TestVersionValidation(t *testing.T) {
	testCases := []testVersion{
		{df, "deskmate-dev-en-us", "df-dev-0_12", "", "", nil},
		{lex, "", "", "deskmate-dev", "lex_dev_a_bc", nil},
		{df, "deskmate-dev-en-us-bad", "", "", "", ErrorVersionEmpty},
		{df, "", "df-dev-0_12-bad-1", "", "", ErrorVersionEmpty},
		{df, "", "df-dev-0_12-bad-2", "deskmate-dev", "", ErrorVersionEmpty},
		{lex, "deskmate-dev-en-us-bad-3", "", "", "lex_dev_a_cd", ErrorVersionEmpty},
	}

	for _, tc := range testCases {
		version := NewIntentServiceVersion(
			WithIntentService(&tc.intentSvc),
			WithProjectName(&tc.dfProj),
			WithDFVersion(&tc.dfVersion),
			WithLexBot(&tc.lexName, &tc.lexVersion),
		)
		err := version.Validate()
		assert.Equal(t, tc.err, err)
		if err == nil {
			if tc.intentSvc == df {
				assert.Equal(t, tc.dfVersion, version.DFVersion)
				assert.Equal(t, tc.dfProj, version.DFProjectName)
			} else {
				assert.Equal(t, tc.lexVersion, version.LexBotAlias)
				assert.Equal(t, tc.lexName, version.LexBotName)
			}
		}
	}
}
