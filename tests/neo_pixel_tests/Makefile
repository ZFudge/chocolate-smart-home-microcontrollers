MAKEFILES := *_test/Makefile

tests:
	@set -e; \
	for i in $(MAKEFILES); do \
		echo '==== Making:' $$(dirname $$i); \
		$(MAKE) --silent -C $$(dirname $$i); \
	done

runtests:
	@set -e; \
	for i in $(MAKEFILES); do \
		echo '==== Running:' $$(dirname $$i); \
		$(MAKE) --silent -C $$(dirname $$i) run; \
	done

clean:
	@set -e; \
	for i in $(MAKEFILES); do \
		echo '==== Cleaning:' $$(dirname $$i); \
		$(MAKE) --silent -C $$(dirname $$i) clean; \
	done

teardown:
	@set -e; \
	for i in $$(find ../ -type f -name "*.o" -o -name '*.out'); do \
		rm $$i; \
	done

all: clean tests runtests teardown
