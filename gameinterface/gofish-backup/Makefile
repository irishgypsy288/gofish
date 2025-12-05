# Go Fish Game Makefile
# Supports both graphical (X11) and console versions

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
DEBUGFLAGS = -g -O0 -DDEBUG

# Directories
SRC_DIR = src
BUILD_DIR = build
ASSETS_DIR = assets

# Libraries
X11_LIBS = -lX11
THREAD_LIBS = -lpthread
ALSA_LIBS = -lasound

# Source files
GUI_SOURCES = $(SRC_DIR)/main.cpp \
              $(SRC_DIR)/GameEngine.cpp \
              $(SRC_DIR)/UIManager.cpp \
              $(SRC_DIR)/AudioManager.cpp

CONSOLE_SOURCE = gofish.cpp

# Object files
GUI_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(GUI_SOURCES))

# Executables
GUI_TARGET = gofish-gui
CONSOLE_TARGET = gofish-console

# Default target
.PHONY: all
all: $(GUI_TARGET)

# Build graphical version
$(GUI_TARGET): $(GUI_OBJECTS) | $(BUILD_DIR)
	@echo "Linking $(GUI_TARGET)..."
	$(CXX) $(CXXFLAGS) -o $@ $^ $(X11_LIBS) $(THREAD_LIBS)
	@echo "Build complete: $(GUI_TARGET)"
	@echo "Run with: ./$(GUI_TARGET)"

# Build console version
.PHONY: console
console: $(CONSOLE_TARGET)

$(CONSOLE_TARGET): $(CONSOLE_SOURCE)
	@echo "Building console version..."
	$(CXX) $(CXXFLAGS) -o $@ $<
	@echo "Build complete: $(CONSOLE_TARGET)"
	@echo "Run with: ./$(CONSOLE_TARGET)"

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create build directory
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Debug build
.PHONY: debug
debug: CXXFLAGS = -std=c++11 -Wall -Wextra $(DEBUGFLAGS)
debug: clean $(GUI_TARGET)
	@echo "Debug build complete"

# Run the graphical version
.PHONY: run
run: $(GUI_TARGET)
	@echo "Starting Go Fish Game..."
	./$(GUI_TARGET)

# Run the console version
.PHONY: run-console
run-console: $(CONSOLE_TARGET)
	@echo "Starting console version..."
	./$(CONSOLE_TARGET)

# Create assets directory structure
.PHONY: setup-assets
setup-assets:
	@echo "Creating assets directory structure..."
	@mkdir -p $(ASSETS_DIR)/sounds
	@mkdir -p $(ASSETS_DIR)/fonts
	@echo "Assets directories created"
	@echo "Note: You need to add sound files to $(ASSETS_DIR)/sounds/"
	@echo "Required files: victory.wav, card_move.wav"

# Install (optional - copies to /usr/local/bin)
.PHONY: install
install: $(GUI_TARGET)
	@echo "Installing $(GUI_TARGET) to /usr/local/bin..."
	@sudo cp $(GUI_TARGET) /usr/local/bin/
	@sudo chmod 755 /usr/local/bin/$(GUI_TARGET)
	@echo "Installation complete"
	@echo "You can now run: gofish-gui"

# Uninstall
.PHONY: uninstall
uninstall:
	@echo "Uninstalling..."
	@sudo rm -f /usr/local/bin/$(GUI_TARGET)
	@echo "Uninstall complete"

# Clean build artifacts
.PHONY: clean
clean:
	@echo "Cleaning build artifacts..."
	@rm -rf $(BUILD_DIR)
	@rm -f $(GUI_TARGET) $(CONSOLE_TARGET)
	@echo "Clean complete"

# Clean everything including assets
.PHONY: distclean
distclean: clean
	@echo "Removing assets..."
	@rm -rf $(ASSETS_DIR)
	@echo "Distribution clean complete"

# Check dependencies
.PHONY: check-deps
check-deps:
	@echo "Checking dependencies..."
	@which $(CXX) > /dev/null || (echo "ERROR: g++ not found" && exit 1)
	@pkg-config --exists x11 || (echo "WARNING: X11 development libraries not found (install libx11-dev)" && exit 1)
	@which aplay > /dev/null || echo "WARNING: aplay not found (audio will be disabled)"
	@echo "Dependency check complete"

# Show help
.PHONY: help
help:
	@echo "Go Fish Game - Makefile Help"
	@echo "============================"
	@echo ""
	@echo "Available targets:"
	@echo "  all           - Build the graphical version (default)"
	@echo "  console       - Build the console version"
	@echo "  debug         - Build with debug symbols"
	@echo "  run           - Build and run the graphical version"
	@echo "  run-console   - Build and run the console version"
	@echo "  setup-assets  - Create assets directory structure"
	@echo "  install       - Install to /usr/local/bin (requires sudo)"
	@echo "  uninstall     - Remove from /usr/local/bin (requires sudo)"
	@echo "  clean         - Remove build artifacts"
	@echo "  distclean     - Remove build artifacts and assets"
	@echo "  check-deps    - Check for required dependencies"
	@echo "  help          - Show this help message"
	@echo ""
	@echo "Examples:"
	@echo "  make              # Build graphical version"
	@echo "  make run          # Build and run graphical version"
	@echo "  make console      # Build console version"
	@echo "  make debug        # Build with debug symbols"
	@echo "  make clean        # Clean build files"
	@echo ""
	@echo "Dependencies:"
	@echo "  - g++ with C++11 support"
	@echo "  - X11 development libraries (libx11-dev)"
	@echo "  - ALSA utilities (alsa-utils) for audio"
	@echo ""
	@echo "Install dependencies on Ubuntu/Debian:"
	@echo "  sudo apt-get install build-essential libx11-dev alsa-utils"

# Phony targets
.PHONY: all console debug run run-console setup-assets install uninstall clean distclean check-deps help