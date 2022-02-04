default: build/realize compile_commands.json

.PHONY: clean

clean:
	rm -rf build compile_commands.json .cache tests.tap tests.xml

build/realize: build
	cmake --build $<

build/realizetest: build
	cmake --build $<

compile_commands.json: build/compile_commands.json
	ln -s $<

build/compile_commands.json: build

build: src/*.* Makefile CMakeLists.txt
	cmake -B build .

install: build/realize
	sudo cmake --build build -t install

test: build/realizetest
	cmake --build build -t test

format:
	clang-format -i src/*.c src/*.h
	cmake-format -i CMakeLists.txt
