help:
	@echo "Usage: make TARGET"
	@echo ""
	@echo "Targets:"
	@echo "  help                         Print this help message"
	@echo "  test                         Run tests for all controllers"
	@echo "  neo_pixel, neopixel          Run tests for ./neo_pixel controller"
	@echo "  on_off, onoff                Run tests for ./on_off controller"
	@echo "  teardown                     Remove all .o and .out files"
	@echo ""

teardown:
	@set -e; \
	for i in $$(find . -type f -name "*.o" -o -name '*.out'); do \
		rm $$i; \
	done

testall:
	@$(MAKE) -C tests all

test: testall teardown

neopixel:
	$(MAKE) -C tests/neo_pixel_tests all
neo_pixel: neopixel

onoff:
	@$(MAKE) -C tests/on_off_test clean || true
	@$(MAKE) -C tests/on_off_test
	@$(PWD)/tests/on_off_test/on_off_test.out
	@$(MAKE) --silent -C tests/on_off_test clean
on_off: onoff
