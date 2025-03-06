# The build directory
BUILD_DIR = build

# Declare build as a phony target to ensure it's always executed
.PHONY: all build clean test

# Default target: configure and build the project
all: build

# Build the project inside the build directory
build:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake .. && cmake --build .

# Clean the build directory
clean:
	rm -rf $(BUILD_DIR)
	echo "Build directory cleaned."

# Run the tests (if you have tests set up with CTest)
test:
	cd $(BUILD_DIR)/tests && ctest --output-on-failure

# Run the executable generated from build
run:
	cd $(BUILD_DIR)/app && ./mgl
