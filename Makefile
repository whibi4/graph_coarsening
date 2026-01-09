TARGET = graph_coarsening
BUILD_DIR = build
all: $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake .. && cmake --build . -j && cd ..

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
run:
	./$(BUILD_DIR)/$(TARGET)
clean:
	rm -rf $(BUILD_DIR)
