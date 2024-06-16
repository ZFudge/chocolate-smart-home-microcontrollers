help:
	@echo "Usage: make TARGET"
	@echo ""
	@echo "Targets:"
	@echo "  help                         Print this help message"
	@echo "  test                         Run tests for all controllers"
	@echo "  neo_pixel, neopixel          Run tests for ./neo_pixel controller"
	@echo "  on_off, onoff                Run tests for ./on_off controller"
	@echo ""

test:
	@$(MAKE) -C tests all

neopixel: neo_pixel
neo_pixel:
	@$(MAKE) -C tests/neo_pixel_test clean || true
	@$(MAKE) -C tests/neo_pixel_test
	@$(PWD)/tests/neo_pixel_test/neo_pixel_test.out
	@$(MAKE) --silent -C tests/on_off_test clean

onoff: on_off
on_off:
	@$(MAKE) -C tests/neo_pixel_test clean || true
	@$(MAKE) -C tests/on_off_test
	@$(PWD)/tests/on_off_test/on_off_test.out
	@$(MAKE) --silent -C tests/on_off_test clean
