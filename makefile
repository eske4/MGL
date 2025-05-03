# # The build directory
BUILD_DIR = build
DEMO_DIR = demo
ASSEMBLY_DIR = assembly
MAKEFLAGS += --no-print-directory

# Declare build as a phony target to ensure it's always executed
.PHONY: all build clean test run

# Default target: configure and build the project
all: build test generate

# Build the project inside the build directory
build:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake .. && cmake --build .
	@cd $(DEMO_DIR) && $(MAKE) build
	@ln -sf "build/compile_commands.json"

# Generates the assembly files through compiler
generate:
	@cd $(BUILD_DIR)/app && ./mgl

# Run assembly binary
run: 
	@cd $(ASSEMBLY_DIR) && $(MAKE) --no-print-directory run

# Clean the build directory
clean:
	@rm -rf $(BUILD_DIR)
	@echo "Build directory cleaned."
	@cd $(DEMO_DIR) && $(MAKE) clean
	@echo "Clean Usage artifacts"

# Run the tests (if you have tests set up with CTest)
test:
	@cd $(BUILD_DIR)/tests && ctest --output-on-failure

c_demo:
	@cd $(USAGE_DIR) && $(MAKE) c_demo

py_demo:
	@cd $(USAGE_DIR) && $(MAKE) py_demo

