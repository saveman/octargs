CURRENT_DIR:=$(PWD)

SOURCE_DIR:=$(CURRENT_DIR)
BUILD_DIR:=$(CURRENT_DIR)/_build
INSTALL_DIR:=$(CURRENT_DIR)/_install

CMAKE_OPTS=\
	-DCMAKE_INSTALL_PREFIX=$(INSTALL_DIR) \
	-DCMAKE_BUILD_TYPE=Debug \
	-DBUILD_TESTS=True \
	-DBUILD_EXAMPLES=True \
	-DENABLE_COVERAGE=True

all: install runctest

build:
	mkdir -p $(BUILD_DIR)
	(cd $(BUILD_DIR) && cmake $(CMAKE_OPTS) $(SOURCE_DIR) )
	(cd $(BUILD_DIR) && cmake --build .)

install: build
	(cd $(BUILD_DIR) && cmake --build . -- install)

runctest: build
	(cd $(BUILD_DIR) && ctest --output-on-failure)

examples: install
	@echo "----------------------------"
	@echo "CAT TEST 1"
	@echo "----------------------------"
	@(cd ${INSTALL_DIR}/bin && ./octargs_cat ${SOURCE_DIR}/LICENSE)
	@echo "----------------------------"

	@echo "----------------------------"
	@echo "CAT TEST 2"
	@echo "----------------------------"
	@(cd ${INSTALL_DIR}/bin && ./octargs_cat < ${SOURCE_DIR}/LICENSE)
	@echo "----------------------------"

	@echo "----------------------------"
	@echo "CAT TEST 3"
	@echo "----------------------------"
	@(cd ${INSTALL_DIR}/bin && ./octargs_cat - < ${SOURCE_DIR}/LICENSE)
	@echo "----------------------------"

	@echo "----------------------------"
	@echo "CAT TEST 4"
	@echo "----------------------------"
	@(cd ${INSTALL_DIR}/bin && cat -n -E ${SOURCE_DIR}/LICENSE ${SOURCE_DIR}/README.md)
	@echo "----------------------------"
	@(cd ${INSTALL_DIR}/bin && ./octargs_cat -n --show-ends ${SOURCE_DIR}/LICENSE ${SOURCE_DIR}/README.md)
	@echo "----------------------------"

ctest_coverage: build
	(cd $(BUILD_DIR) && find . -name "*.gcda" -exec rm -f {} \; )
	(cd $(BUILD_DIR) && cmake --build . -- ctest_coverage)

ctest_coverage_open: ctest_coverage
	(cd $(BUILD_DIR) && pdetach xdg-open ./ctest_coverage/index.html)

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(INSTALL_DIR)
