GOFILES=$(shell find . -name '*.go'  \! -name '*.rice-box.go')
GOBINDIR=$(shell echo $(GOPATH) |  awk -F: '{ print $$NF }')/bin
GOSRCDIR=$(shell echo $(GOPATH) |  awk -F: '{ print $$NF }')/src

ifndef GIT_BRANCH
GIT_BRANCH=$(shell git symbolic-ref --short HEAD)
endif
ifndef GIT_COMMIT
GIT_COMMIT=$(shell git rev-parse HEAD)
endif
HOSTNAME=$(shell hostname -f)
BUILD_USER=$(shell whoami)
PWD=$(shell pwd)

.PHONY: build clean save-deps restore-deps test racetest

.DEFAULT_GOAL := all


PACKAGE=sai-blobstore

all: build

build: $(GOBINDIR)/$(PACKAGE)

ifdef GOBINDIR
clean:
	rm -f $(GOBINDIR)/$(PACKAGE)
endif

$(GOBINDIR)/godep:
	# Fetch a fixed version of Godep that uses the old file format
	mkdir -p $(GOBINDIR)
	curl -o $(GOBINDIR)/godep https://lrya-sai.s3.amazonaws.com/utils/$(shell uname -s)/godep
	chmod +x $(GOBINDIR)/godep

save-deps: $(GOBINDIR)/godep
	rm -f Godeps  # only needed as godep update is broken for copy=false
	$(GOBINDIR)/godep save -copy=false github.com/lrya/$(PACKAGE)/...

restore-deps: $(GOBINDIR)/godep
	# Bug with godep necessitates running this twice until a proper fix is supplied
	$(GOBINDIR)/godep restore || $(GOBINDIR)/godep restore

test:
	go test -timeout 5m github.com/lrya/$(PACKAGE)/...

racetest:
	go test -timeout 5m -race -tags accounts_no_db github.com/lrya/$(PACKAGE)/...

docker:	build
	cp -f $(GOBINDIR)/$(PACKAGE) ./bin
	docker build -t lrya/sai-blobstore:$(GIT_BRANCH) .

BUILDVAR=github.com/lrya/sai-go-util/buildinfo
$(GOBINDIR)/$(PACKAGE):  $(GOFILES)
	go install -ldflags \
		"-X $(BUILDVAR).product=blobstore \
		-X $(BUILDVAR).buildTimeUnix=`date +%s` \
		-X $(BUILDVAR).commit=$(GIT_COMMIT) \
		-X $(BUILDVAR).branch=$(GIT_BRANCH) \
		-X $(BUILDVAR).buildHost=$(HOSTNAME)     \
		-X $(BUILDVAR).buildUser=$(BUILD_USER)"  \
			github.com/lrya/$(PACKAGE)

cover:
	pkgcover \
		github.com/lrya/$(PACKAGE)/...
