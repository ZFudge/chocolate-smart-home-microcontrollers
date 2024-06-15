test:
	@$(MAKE) -C tests all

test_neo_pixel:
	@$(MAKE) -C tests/neo_pixel_test clean || true
	@$(MAKE) -C tests/neo_pixel_test
	@$(PWD)/tests/neo_pixel_test/neo_pixel_test.out
	@$(MAKE) --silent -C tests/on_off_test clean

test_on_off:
	@$(MAKE) -C tests/neo_pixel_test clean || true
	@$(MAKE) -C tests/on_off_test
	@$(PWD)/tests/on_off_test/on_off_test.out
	@$(MAKE) --silent -C tests/on_off_test clean
