CURRENT_DIR:=$(PWD)

SOURCE_DIR:=$(CURRENT_DIR)
BUILD_DIR:=$(CURRENT_DIR)/build
INSTALL_DIR:=$(CURRENT_DIR)/install

all:
	mkdir -p $(BUILD_DIR)
	(cd $(BUILD_DIR) && cmake -DCMAKE_INSTALL_PREFIX=$(INSTALL_DIR) -DCMAKE_BUILD_TYPE=Debug $(SOURCE_DIR) )
	(cd $(BUILD_DIR) && cmake --build .)
	(cd $(BUILD_DIR) && cmake --build . -- install)

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(INSTALL_DIR)
