MAKE_FILES := {.,neo_pixel_tests}/*test/Makefile

tests:
	@set -e; \
	for i in $(MAKE_FILES); do \
		echo '==== Making:' $$(dirname $$i); \
		$(MAKE) --silent -C $$(dirname $$i); \
	done

runtests:
	@set -e; \
	for i in $(MAKE_FILES); do \
		echo '==== Running:' $$(dirname $$i); \
		$(MAKE) --silent -C $$(dirname $$i) run; \
	done

clean:
	@set -e; \
	for i in $(MAKE_FILES); do \
		echo '==== Cleaning:' $$(dirname $$i); \
		$(MAKE) --silent -C $$(dirname $$i) clean; \
	done

all: clean tests runtests
