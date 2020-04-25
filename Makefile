CURRENT_DIR:=$(PWD)

SOURCE_DIR:=$(CURRENT_DIR)
SOURCE_EXAMPLES_DIR:=$(SOURCE_DIR)/examples
THIRDPARTY_DIR:=$(SOURCE_DIR)/3rdparty

WORK_DIR:=$(CURRENT_DIR)/_work
BUILD_DIR:=$(WORK_DIR)/_build
INSTALL_DIR:=$(WORK_DIR)/_install
PACKAGE_DIR:=$(WORK_DIR)/_package
VERIFY_DIR=$(WORK_DIR)/_verify

BUILD_CPPCHECK_DIR:=$(BUILD_DIR)/cppcheck

CPUCOUNT=$(shell nproc)

CMAKE_OPTS=\
	-DCMAKE_BUILD_TYPE=Debug \
	-DRELEASE_VERSION=${RELEASE_VERSION} \
	-DCMAKE_INSTALL_PREFIX=$(INSTALL_DIR) \
	-DCPACK_PACKAGE_DIRECTORY=$(PACKAGE_DIR) \
	-DBUILD_TESTS=True \
	-DBUILD_EXAMPLES=True \
	-DENABLE_COVERAGE=True \
	-DCMAKE_EXPORT_COMPILE_COMMANDS=True

VERIFY_CMAKE_OPTS=\
	-DCMAKE_BUILD_TYPE=Release \
	-DOCTARGS_ROOT_DIR=$(INSTALL_DIR)

CPPCHECK_OPTS=\
	-i $(THIRDPARTY_DIR) \
	--library=googletest \
	--verbose \
	--enable=all \
	--std=c++11 \
	--inconclusive \
	--force \
	--inline-suppr

CMAKE_BUILD=cmake --build . -j $(CPUCOUNT)

all: install_verify test

$(BUILD_DIR) $(INSTALL_DIR) $(PACKAGE_DIR):
	mkdir -p $@

prepare: $(BUILD_DIR) $(PACKAGE_DIR) $(INSTALL_DIR)
	(cd $(BUILD_DIR) && cmake $(CMAKE_OPTS) $(SOURCE_DIR) )

build: prepare
	(cd $(BUILD_DIR) && $(CMAKE_BUILD) -- all)

install: build
	(cd $(BUILD_DIR) && $(CMAKE_BUILD) -- install)

install_verify: install
	rm -rf $(VERIFY_DIR)
	mkdir -p $(VERIFY_DIR)
	(cd $(VERIFY_DIR) && cmake $(VERIFY_CMAKE_OPTS) $(SOURCE_EXAMPLES_DIR) )
	(cd $(VERIFY_DIR) && $(CMAKE_BUILD))

doxygen: prepare
	(cd $(BUILD_DIR) && $(CMAKE_BUILD) -- doxygen)

doxygen_open: doxygen
	(cd $(BUILD_DIR) && pdetach xdg-open ./doxygen/html/index.html)

doxygen_deploy: doxygen
	rm -rf docs/html
	mkdir -p docs
	cp -r $(BUILD_DIR)/doxygen/html docs/

test: build
	(cd $(BUILD_DIR) && $(CMAKE_BUILD) -- test ARGS=--output-on-failure)

package: install
	(cd $(BUILD_DIR) && $(CMAKE_BUILD) -- package)

cppcheck: test
	mkdir -p $(BUILD_CPPCHECK_DIR)
	(cd $(BUILD_DIR) && cppcheck --project=compile_commands.json $(CPPCHECK_OPTS) --xml 2> cppcheck-report.xml)
	(cd $(BUILD_DIR) && cppcheck-htmlreport --file=cppcheck-report.xml --report-dir=$(BUILD_CPPCHECK_DIR) --source-dir=$(SOURCE_DIR))
	(cd $(BUILD_DIR) && pdetach xdg-open $(BUILD_CPPCHECK_DIR)/index.html)

run_examples_cat:
	@echo "----------------------------"
	@echo "CAT TEST 1"
	@echo "----------------------------"
	@(cd $(EXEDIR) && ./$(EXENAME) $(SOURCE_DIR)/LICENSE)
	@echo "----------------------------"

	@echo "----------------------------"
	@echo "CAT TEST 2"
	@echo "----------------------------"
	@(cd $(EXEDIR) && ./$(EXENAME) < $(SOURCE_DIR)/LICENSE)
	@echo "----------------------------"

	@echo "----------------------------"
	@echo "CAT TEST 3"
	@echo "----------------------------"
	@(cd $(EXEDIR) && ./$(EXENAME) - < $(SOURCE_DIR)/LICENSE)
	@echo "----------------------------"

	@echo "----------------------------"
	@echo "CAT TEST 4"
	@echo "----------------------------"
	@cat -n -E $(SOURCE_DIR)/LICENSE $(SOURCE_DIR)/README.md
	@echo "----------------------------"
	@(cd $(EXEDIR) && ./$(EXENAME) -n --show-ends $(SOURCE_DIR)/LICENSE $(SOURCE_DIR)/README.md)
	@echo "----------------------------"

	@echo "----------------------------"
	@echo "CAT HELP"
	@echo "----------------------------"
	@(cd $(EXEDIR) && ./$(EXENAME) --help)
	@echo "----------------------------"

examples_cat: install
	$(MAKE) run_examples_cat EXEDIR=$(INSTALL_DIR)/bin EXENAME=octargs_cat

run_examples_head:
	@echo "----------------------------"
	@echo "HEAD TEST 1"
	@echo "----------------------------"
	@head -n 5 -v $(SOURCE_DIR)/LICENSE $(SOURCE_DIR)/README.md - - < $(SOURCE_DIR)/LICENSE
	@echo "----------------------------"
	@(cd $(EXEDIR) && ./$(EXENAME) -n 5 -h $(SOURCE_DIR)/LICENSE $(SOURCE_DIR)/README.md - - < $(SOURCE_DIR)/LICENSE)
	@echo "----------------------------"
	@(cd $(EXEDIR) && ./$(EXENAME) -n=5 -h $(SOURCE_DIR)/LICENSE $(SOURCE_DIR)/README.md - - < $(SOURCE_DIR)/LICENSE)
	@echo "----------------------------"
	@echo "----------------------------"
	@echo "HEAD  HELP"
	@echo "----------------------------"
	@(cd $(EXEDIR) && ./$(EXENAME) --help)
	@echo "----------------------------"

examples_head: install
	$(MAKE) run_examples_head EXEDIR=$(INSTALL_DIR)/bin EXENAME=octargs_head

run_examples_calc:
	@echo "----------------------------"
	@echo "CALC TEST 1"
	@echo "----------------------------"
	(cd $(EXEDIR) && ./$(EXENAME) 1 2 3 4 5 )
	@echo "----------------------------"
	(cd $(EXEDIR) && ./$(EXENAME) 5 -5 5 -5 5 )
	@echo "----------------------------"
	@echo "CALC TEST 2"
	@echo "----------------------------"
	(cd $(EXEDIR) && ./$(EXENAME) -o mul -t double 1.2 1.2 )
	@echo "----------------------------"
	(cd $(EXEDIR) && ./$(EXENAME) --operation max -t float 1.1 1.3 1.0 0.7 1.25 )
	@echo "----------------------------"
	(cd $(EXEDIR) && ./$(EXENAME) --operation min -t float 1.1 1.3 1.0 0.7 1.25 )
	@echo "----------------------------"
	@echo "CALC STEPS"
	@echo "----------------------------"
	(cd $(EXEDIR) && ./$(EXENAME) -s -o sum 1 2 3 4 5 -3 )
	(cd $(EXEDIR) && ./$(EXENAME) -s -o mul 1 2 3 4 5 -3 )
	(cd $(EXEDIR) && ./$(EXENAME) -s -o min 1 2 3 4 5 -3 )
	(cd $(EXEDIR) && ./$(EXENAME) -s -o max 1 2 3 4 5 -3 )
	@echo "----------------------------"
	@echo "CALC HELP"
	@echo "----------------------------"
	@(cd $(EXEDIR) && ./$(EXENAME) --help)
	@echo "----------------------------"

examples_sum: install
	$(MAKE) run_examples_sum EXEDIR=$(INSTALL_DIR)/bin EXENAME=octargs_sum

run_examples_sum:
	@echo "----------------------------"
	@echo "SUM TEST 1"
	@echo "----------------------------"
	(cd $(EXEDIR) && ./$(EXENAME) 1 2 3 4 5 )
	@echo "----------------------------"
	(cd $(EXEDIR) && ./$(EXENAME) 5 -5 5 -5 5 )
	@echo "----------------------------"
	@echo "SUM STEPS"
	@echo "----------------------------"
	(cd $(EXEDIR) && ./$(EXENAME) 1 2 3 4 5 -3 )
	@echo "----------------------------"
	(cd $(EXEDIR) && LC_ALL=en_US.UTF-8 ./$(EXENAME) -s=tak 1 2 3 4 5 -3 || true)
	@echo "----------------------------"
	(cd $(EXEDIR) && LC_ALL=pl_PL.UTF-8 ./$(EXENAME) -s=tak 1 2 3 4 5 -3)
	@echo "----------------------------"
	@echo "SUM HELP"
	@echo "----------------------------"
	@(cd $(EXEDIR) && LC_ALL=en_US.UTF-8 ./$(EXENAME) --help)
	@echo "----------------------------"
	@(cd $(EXEDIR) && LC_ALL=pl_PL.UTF-8 ./$(EXENAME) --help)
	@echo "----------------------------"

examples_calc: install
	$(MAKE) run_examples_calc EXEDIR=$(INSTALL_DIR)/bin EXENAME=octargs_calc

coverage_prepare: build
	(cd $(BUILD_DIR) && find . -name "*.gcda" -exec rm -f {} \; )

ctest_coverage: coverage_prepare
	(cd $(BUILD_DIR) && cmake --build . -- ctest_coverage)

ctest_coverage_open: ctest_coverage
	(cd $(BUILD_DIR) && pdetach xdg-open ./ctest_coverage/index.html)

total_coverage: coverage_prepare
	$(MAKE) run_examples_cat  EXEDIR=$(BUILD_DIR)/examples/cat  EXENAME=octargs_cat
	$(MAKE) run_examples_head EXEDIR=$(BUILD_DIR)/examples/head EXENAME=octargs_head
	$(MAKE) run_examples_calc EXEDIR=$(BUILD_DIR)/examples/calc EXENAME=octargs_calc
	(cd $(BUILD_DIR) && cmake --build . -- ctest_coverage)

total_coverage_open: total_coverage
	(cd $(BUILD_DIR) && pdetach xdg-open ./ctest_coverage/index.html)

clean:
	rm -rf $(WORK_DIR)

.PHONY: doxygen
