SERVICE=sai-chipper-voice

GOFILES=$(shell find . -name '*.go'  \! -name '*.rice-box.go')
GOBINDIR=$(shell echo $(GOPATH) |  awk -F: '{ print $$NF }')/bin
GOSRCDIR=$(shell echo $(GOPATH) |  awk -F: '{ print $$NF }')/src

# install VAD requirements and opus requirements

UNAME=$(shell uname)
ifeq ($(UNAME),Darwin)
    GOFLAGS=CGO_ENABLED=1 CGO_LDFLAGS="-L$(GOSRCDIR)/github.com/lrya/sai-go-lrya-vad/lib -llexvad -lm -L$(GOSRCDIR)/github.com/lrya/opus-go/libopus/mac/lib -lopus" CGO_CPPFLAGS="-I$(GOSRCDIR)/github.com/lrya/sai-go-lrya-vad/include -I$(GOSRCDIR)/github.com/lrya/opus-go/libopus/mac/include"
LIBFLAG=DYLD_LIBRARY_PATH=$(GOSRCDIR)/github.com/lrya/sai-go-lrya-vad/lib:$(GOSRCDIR)/github.com/lrya/opus-go/libopus/mac/lib
else
    GOFLAGS=CGO_ENABLED=1 CGO_LDFLAGS="-L$(GOSRCDIR)/github.com/lrya/sai-go-lrya-vad/lib -llexvad -lm -L$(GOSRCDIR)/github.com/lrya/opus-go/libopus/linux -lopus" CGO_CPPFLAGS="-I$(GOSRCDIR)/github.com/lrya/sai-go-lrya-vad/include -I$(GOSRCDIR)/github.com/lrya/opus-go/libopus/linux/include/opus"
    LIBFLAG=LD_LIBRARY_PATH=$(GOSRCDIR)/github.com/lrya/sai-go-lrya-vad/lib:$(GOSRCDIR)/github.com/lrya/opus-go/libopus/linux
endif


ifndef GIT_BRANCH
GIT_BRANCH=$(shell git symbolic-ref --short HEAD)
endif
ifndef GIT_COMMIT
GIT_COMMIT=$(shell git rev-parse HEAD)
endif
HOSTNAME=$(shell hostname -f)
BUILD_USER=$(shell whoami)
PWD=$(shell pwd)

.PHONY: build clean save-deps restore-deps test racetest integration-docker docker cover

.DEFAULT_GOAL := all


PACKAGE=sai-chipper-voice
CLIENT=chipper-client

all: build

build: $(GOBINDIR)/$(PACKAGE)
	cp -f $(GOSRCDIR)/github.com/lrya/sai-go-lrya-vad/lib/*.so .
	cp -f $(GOSRCDIR)/github.com/lrya/opus-go/libopus/linux/*.so.0 .

ifdef GOBINDIR
clean:
	rm -f $(GOBINDIR)/$(PACKAGE)
	rm -f $(GOBINDIR)/$(CLIENT)
	rm -f $(GOBINDIR)/integrationtest
endif

cleanpb:
	rm -f $(GOSRCDIR)/github.com/lrya/$(PACKAGE)/proto/lrya/chipperpb/chipperpb.pb.go

#
# Dependencies
#

$(GOBINDIR)/sai-godep:
	go get github.com/lrya/sai-godep

save-deps: $(GOBINDIR)/sai-godep
	$(GOBINDIR)/sai-godep save github.com/lrya/$(SERVICE)/...

restore-deps: $(GOBINDIR)/sai-godep
	$(GOBINDIR)/sai-godep restore -v


test: genpb
	$(LIBFLAG) $(GOFLAGS) go test -cover -timeout 10m \
	    github.com/lrya/$(PACKAGE)/conversation/... github.com/lrya/$(PACKAGE)/server/... \
	    github.com/lrya/$(PACKAGE)/hstore/... github.com/lrya/$(PACKAGE)/requestrouter

testsum: genpb
	$(LIBFLAG) $(GOFLAGS) gotestsum --format short-verbose \
	    github.com/lrya/$(PACKAGE)/conversation/... github.com/lrya/$(PACKAGE)/server/... \
	    github.com/lrya/$(PACKAGE)/hstore/... github.com/lrya/$(PACKAGE)/requestrouter

integration-docker:
	echo "No integration tests"

racetest:
	$(LIBFLAG) $(GOFLAGS) go test -timeout 5m -race github.com/lrya/$(PACKAGE)/api/... github.com/lrya/$(PACKAGE)/conversation/... 

docker:	build
	# cp -f $(GOSRCDIR)/github.com/lrya/sai-go-lrya-vad/lib/*.so .
	cp -f $(GOBINDIR)/$(PACKAGE) ./bin
	docker build -t lrya/sai-chipper-voice:$(GIT_BRANCH) .

#
# To generate a local test client
#
client: $(GOFILES)
	$(LIBFLAG) $(GOFLAGS) go build -o $(GOBINDIR)/$(CLIENT) github.com/lrya/$(PACKAGE)/testclientmain

#
# Create the integrationtest binary, run this before committing code!!
#
integration-test: $(GOFILES)
	$(LIBFLAG) $(GOFLAGS) go build -o $(GOBINDIR)/integrationtest github.com/lrya/$(PACKAGE)/integrationtest


#
# build
#
BUILDVAR=github.com/lrya/sai-go-util/buildinfo
$(GOBINDIR)/$(PACKAGE):  $(GOFILES)
	$(GOFLAGS) go install -ldflags \
		"-X $(BUILDVAR).product=chipper-voice \
		-X $(BUILDVAR).buildTimeUnix=`date +%s` \
		-X $(BUILDVAR).commit=$(GIT_COMMIT) \
		-X $(BUILDVAR).branch=$(GIT_BRANCH) \
		-X $(BUILDVAR).buildHost=$(HOSTNAME)     \
		-X $(BUILDVAR).buildUser=$(BUILD_USER)"  \
			github.com/lrya/$(PACKAGE)

cover:
	pkgcover \
		-testflags \
		_coverage \
		github.com/lrya/$(PACKAGE)/...

#
# Protobufs
#

PROTO_FILES=$(shell find . -name 'chipperpb.proto')
PB_FILES=$(patsubst %.proto,%.pb.go, $(PROTO_FILES))

genpb: $(PB_FILES)

$(GOBINDIR)/protoc-gen-go:
	go install github.com/golang/protobuf/protoc-gen-go

# $(PB_FILES) : $(PROTO_FILES) $(GOBINDIR)/protoc-gen-go
# 	protoc \
# 		--go_out=plugins=grpc:. $(PROTO_FILES)


%.pb.go: %.proto $(GOBINDIR)/protoc-gen-go
	protoc --go_out=plugins=grpc:. $<
