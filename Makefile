CURRENT_DIR:=$(PWD)

SOURCE_DIR:=$(CURRENT_DIR)
BUILD_DIR:=$(CURRENT_DIR)/_build
INSTALL_DIR:=$(CURRENT_DIR)/_install
PACKAGE_DIR:=$(CURRENT_DIR)/_package
CPPCHECK_DIR:=$(BUILD_DIR)/cppcheck

CPUCOUNT=$(shell nproc)

CMAKE_OPTS=\
	-DRELEASE_VERSION=${RELEASE_VERSION} \
	-DCMAKE_INSTALL_PREFIX=$(INSTALL_DIR) \
	-DCPACK_PACKAGE_DIRECTORY=$(PACKAGE_DIR) \
	-DCMAKE_BUILD_TYPE=Debug \
	-DBUILD_TESTS=True \
	-DBUILD_EXAMPLES=True \
	-DENABLE_COVERAGE=True \
	-DCMAKE_EXPORT_COMPILE_COMMANDS=True

CPPCHECK_OPTS=\
	--enable=all \
	--std=c++11 \
	--inconclusive \
	--force \
	--inline-suppr

all: install test

build:
	mkdir -p $(BUILD_DIR)
	mkdir -p ${PACKAGE_DIR}
	(cd $(BUILD_DIR) && cmake $(CMAKE_OPTS) $(SOURCE_DIR) )
	(cd $(BUILD_DIR) && cmake --build . -j $(CPUCOUNT))

install: build
	(cd $(BUILD_DIR) && cmake --build . -- install)

test: build
	(cd $(BUILD_DIR) && ctest --output-on-failure)

package: install
	(cd $(BUILD_DIR) && cmake --build . -- package)

cppcheck: test
	mkdir -p $(CPPCHECK_DIR)
	(cd $(BUILD_DIR) && cppcheck --project=compile_commands.json $(CPPCHECK_OPTS) --xml 2> cppcheck-report.xml)
	(cd $(BUILD_DIR) && cppcheck-htmlreport --file=cppcheck-report.xml --report-dir=$(CPPCHECK_DIR) --source-dir=$(SOURCE_DIR))
	(cd $(BUILD_DIR) && pdetach xdg-open $(CPPCHECK_DIR)/index.html)

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

examples_head: install
	$(MAKE) run_examples_head EXEDIR=$(INSTALL_DIR)/bin EXENAME=octargs_head

coverage_prepare: build
	(cd $(BUILD_DIR) && find . -name "*.gcda" -exec rm -f {} \; )

ctest_coverage: coverage_prepare
	(cd $(BUILD_DIR) && cmake --build . -- ctest_coverage)

ctest_coverage_open: ctest_coverage
	(cd $(BUILD_DIR) && pdetach xdg-open ./ctest_coverage/index.html)

total_coverage: coverage_prepare
	$(MAKE) run_examples_cat  EXEDIR=$(BUILD_DIR)/examples/cat  EXENAME=octargs_cat
	$(MAKE) run_examples_head EXEDIR=$(BUILD_DIR)/examples/head EXENAME=octargs_head
	(cd $(BUILD_DIR) && cmake --build . -- ctest_coverage)

total_coverage_open: total_coverage
	(cd $(BUILD_DIR) && pdetach xdg-open ./ctest_coverage/index.html)

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(INSTALL_DIR)
