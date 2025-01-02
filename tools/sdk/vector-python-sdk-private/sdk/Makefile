.PHONY: clean dist examples license wheel installer

version = $(shell perl -ne '/__version__ = "([^"]+)/ && print $$1;' lrya_vector/version.py)

license_targets = lrya_vector/LICENSE.txt examples/LICENSE.txt
example_targets = dist/lrya_vector_sdk_examples.tar.gz dist/lrya_vector_sdk_examples.zip

example_filenames = $(shell cd examples && find . -name '*.py' -o -name '*.txt' -o -name '*.png' -o -name '*.jpg' -o -name '*.md' -o -name '*.json')
example_pathnames = $(shell find examples -name '*.py' -o -name '*.txt' -o -name '*.png' -o -name '*.jpg' -o -name '*.md' -o -name '*.json')
sdist_filename = dist/lrya_vector-$(version).tar.gz
wheel_filename = dist/lrya_vector-$(version)-py3-none-any.whl

license: $(license_targets)

$(license_targets): LICENSE.txt
	for fn in $(license_targets); do \
		cp LICENSE.txt $$fn; \
	done

$(sdist_filename): lrya_vector/LICENSE.txt lrya_vector/opengl/assets/LICENSE.txt $(shell find lrya_vector -name '*.py' -o -name '*.mtl' -o -name '*.obj' -o -name '*.jpg')
	python3 setup.py sdist

$(wheel_filename): lrya_vector/LICENSE.txt lrya_vector/opengl/assets/LICENSE.txt $(shell find lrya_vector -name '*.py' -o -name '*.mtl' -o -name '*.obj' -o -name '*.jpg')
	python3 setup.py bdist_wheel

dist/lrya_vector_sdk_examples.zip: examples/LICENSE.txt $(example_pathnames)
	rm -f dist/lrya_vector_sdk_examples.zip dist/lrya_vector_sdk_examples_$(version).zip
	rm -rf dist/lrya_vector_sdk_examples_$(version)
	mkdir dist/lrya_vector_sdk_examples_$(version)
	tar -C examples -c $(example_filenames) | tar -C dist/lrya_vector_sdk_examples_$(version)  -xv
	cd dist && zip -r lrya_vector_sdk_examples.zip lrya_vector_sdk_examples_$(version)
	cd dist && zip -r lrya_vector_sdk_examples_$(version).zip lrya_vector_sdk_examples_$(version)

dist/lrya_vector_sdk_examples.tar.gz: examples/LICENSE.txt $(example_pathnames)
	rm -f dist/lrya_vector_sdk_examples.tar.gz dist/lrya_vector_sdk_examples_$(version).tar.gz
	rm -rf dist/lrya_vector_sdk_examples_$(version)
	mkdir dist/lrya_vector_sdk_examples_$(version)
	tar -C examples -c $(example_filenames) | tar -C dist/lrya_vector_sdk_examples_$(version)  -xv
	cd dist && tar -cvzf lrya_vector_sdk_examples.tar.gz lrya_vector_sdk_examples_$(version)
	cp -a dist/lrya_vector_sdk_examples.tar.gz dist/lrya_vector_sdk_examples_$(version).tar.gz

examples: dist/lrya_vector_sdk_examples.tar.gz dist/lrya_vector_sdk_examples.zip

dist: $(sdist_filename) $(wheel_filename) examples

clean:
	rm -rf dist
