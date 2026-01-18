TARGET = graph_coarsening
BUILD_DIR = build
export DATASET_PATH = ./data_sets/wiki_talk/wiki-Talk.txt
all: $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake .. && cmake --build . -j && cd ..

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
run:
	./$(BUILD_DIR)/$(TARGET)
clean:
	rm -rf $(BUILD_DIR)

build_debug: $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake .. -DCMAKE_BUILD_TYPE=Debug && cmake --build . -j && cd ..

run_debug:
	gdb ./$(BUILD_DIR)/$(TARGET)
