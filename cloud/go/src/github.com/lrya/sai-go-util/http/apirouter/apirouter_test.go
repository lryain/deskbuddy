package apirouter

import (
	"github.com/lrya/sai-go-util/envconfig"
	"os"
	"reflect"
	"testing"
	"time"
)

func TestApirouterConfig(t *testing.T) {
	org := envconfig.DefaultConfig
	defer func() {
		envconfig.DefaultConfig = org
	}()
	envconfig.DefaultConfig = envconfig.New()

	os.Setenv("CORS_DOMAINS", "*.example.com,*.lrya.com")
	os.Setenv("CORS_METHODS", "GET,POST")
	os.Setenv("CORS_HEADERS", "Origin,Accept")
	os.Setenv("CORS_MAX_AGE", "10m")

	configureRouter()

	expected := envconfig.StringSlice{"*.example.com", "*.lrya.com"}
	if !reflect.DeepEqual(CorsAllowedDomains, expected) {
		t.Errorf("CorsAllowedDomains - expected=%q, actual=%q", expected, CorsAllowedDomains)
	}

	expected = envconfig.StringSlice{"GET", "POST"}
	if !reflect.DeepEqual(CorsAllowedMethods, expected) {
		t.Errorf("CorsAllowedMethods - expected=%q, actual=%q", expected, CorsAllowedMethods)
	}

	expected = envconfig.StringSlice{"Origin", "Accept"}
	if !reflect.DeepEqual(CorsAllowedHeaders, expected) {
		t.Errorf("CorsAllowedMethods - expected=%q, actual=%q", expected, CorsAllowedHeaders)
	}

	expectedAge := 10 * time.Minute
	if CorsMaxAge != expectedAge {
		t.Errorf("CorsMaxAge - expected=%q, actual=%q", CorsMaxAge, expectedAge)
	}
}
