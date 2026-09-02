CXX ?= clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -pthread -Iinclude -O2

all: build/thread_pool_demo build/test_thread_pool

build:
	mkdir -p build

build/thread_pool_demo: src/main.cpp include/thread_pool.hpp | build
	$(CXX) $(CXXFLAGS) src/main.cpp -o build/thread_pool_demo

build/test_thread_pool: tests/test_thread_pool.cpp include/thread_pool.hpp | build
	$(CXX) $(CXXFLAGS) tests/test_thread_pool.cpp -o build/test_thread_pool

test: build/test_thread_pool
	./build/test_thread_pool

run: build/thread_pool_demo
	./build/thread_pool_demo

clean:
	rm -rf build

.PHONY: all test run clean
