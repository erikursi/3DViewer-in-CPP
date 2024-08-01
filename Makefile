VALGRIND_FLAGS				= --log-file="valgrind.txt" --track-origins=yes --trace-children=yes --leak-check=full --leak-resolution=med

QMAKE = qmake
QT_INSTALL_PREFIX = $(shell $(QMAKE) -query QT_INSTALL_PREFIX)


PROJECT_NAME=3DViewer
PROJECT_DIR=$(PROJECT_NAME)/
BUILD_DIR=build/
APP=$(BUILD_DIR)$(PROJECT_NAME)
CMAKELISTS=$(PROJECT_DIR)CMakeLists.txt

all: install

build: $(CMAKELISTS)
	mkdir -p $(BUILD_DIR)
	cmake -DCMAKE_PREFIX_PATH=$(QT_INSTALL_PREFIX) -S $(PROJECT_DIR) -B $(BUILD_DIR)


install: build
	echo "installing..."
	make -C $(BUILD_DIR) $(PROJECT_NAME)

uninstall:
	@rm -rf $(BUILD_DIR)
	@rm -rf build-3DViewer*

clean:
	make -C $(BUILD_DIR) clean
	rm -rf valgrind.txt
	rm -rf coverage*


# dvi:
# 	open README.md

# dist: install
# 	tar -czf $(APP_NAME).tar.gz --directory=build/ .

run: install
	$(APP)

tests: build
	make -C $(BUILD_DIR) tests
	./$(BUILD_DIR)tests

cppcheck: build
	make -C $(BUILD_DIR) cppcheck

check_style: build
	make -C $(BUILD_DIR) check_style

format_style: build
	make -C $(BUILD_DIR) format_style

check_coverage: build
	make -C $(BUILD_DIR) coverage

.PHONY: all build install uninstall clean dvi dist run tests valgrind cppcheck check_style format_syle check_coverage