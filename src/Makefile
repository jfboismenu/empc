BUILD_DIR=../build

all:
	mkdir -p $(BUILD_DIR)
	g++ *.cpp cpu/*.cpp -I . -o $(BUILD_DIR)/libempc.so -Wall -Werror -std=c++2a -shared -fPIC
