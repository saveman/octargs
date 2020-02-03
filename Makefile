CURRENT_DIR:=$(PWD)

SOURCE_DIR:=$(CURRENT_DIR)
BUILD_DIR:=$(CURRENT_DIR)/_build
INSTALL_DIR:=$(CURRENT_DIR)/_install

CMAKE_OPTS=\
	-DCMAKE_INSTALL_PREFIX=$(INSTALL_DIR) \
	-DCMAKE_BUILD_TYPE=Debug \
	-DBUILD_TESTS=True \
	-DBUILD_EXAMPLES=True

all:
	mkdir -p $(BUILD_DIR)
	(cd $(BUILD_DIR) && cmake $(CMAKE_OPTS) $(SOURCE_DIR) )
	(cd $(BUILD_DIR) && cmake --build .)
	(cd $(BUILD_DIR) && cmake --build . -- install)
	(cd $(BUILD_DIR) && ctest --output-on-failure)
#	ls -lR $(INSTALL_DIR

examples: all
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

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(INSTALL_DIR)
