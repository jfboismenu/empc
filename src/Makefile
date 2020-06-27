BUILD_DIR=../build

all:
	mkdir -p $(BUILD_DIR)
	g++ empc/*/*.cpp empc/*.cpp -I . -o $(BUILD_DIR)/libempc.so -Wall -Werror -std=c++2a -shared -fPIC
