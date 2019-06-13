# Dedicated to the public domain under CC0: https://creativecommons.org/publicdomain/zero/1.0/.

# $@: The file name of the target of the rule.
# $<: The name of the first prerequisite.
# $^: The names of all the prerequisites, with spaces between them.
# $*: The matching string in a pattern rule.

.SUFFIXES: # Disable implicit rules.

.PHONY: _default _phony clean help pip-dev pip-uninstall pypi-dist pypi-upload test typecheck

# First target of a makefile is the default.
_default: test typecheck

_phony:

build:
	py setup.py build

clean:
	rm -rf _build/*
	rm -rf hashing/*.so

help: # Summarize the targets of this makefile.
	@GREP_COLOR="1;32" egrep --color=always '^\w[^ :]+:' makefile | sort

pip-dev:
	pip3 install -e .

pip-uninstall:
	pip3 uninstall --yes pithy

pypi-dist:
	python3 setup.py sdist bdist

pypi-upload-test: pypi-dist
	twine upload --repository-url https://test.pypi.org/legacy/ _build/dist/*

pypi-upload:
	twine upload --repository-url https://upload.pypi.org/legacy/ _build/dist/*

test:
	iotest -fail-fast

test/%: _phony
	iotest -fail-fast $@

typecheck:
	craft-py-check hashing
