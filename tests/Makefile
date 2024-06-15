tests:
	@set -e; \
	for i in *test/Makefile; do \
		echo '==== Making:' $$(dirname $$i); \
		$(MAKE) --silent -C $$(dirname $$i); \
	done

runtests:
	@set -e; \
	for i in *test/Makefile; do \
		echo '==== Running:' $$(dirname $$i); \
		$(MAKE) --silent -C $$(dirname $$i) run; \
	done

clean:
	@set -e; \
	for i in *test/Makefile; do \
		echo '==== Cleaning:' $$(dirname $$i); \
		$(MAKE) --silent -C $$(dirname $$i) clean; \
	done

teardown:
	@set -e; \
	for i in *test/*{.o,.out}; do \
		rm $$i; \
	done

run: clean tests runtests
all: run clean
