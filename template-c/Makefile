default: build/PRJNAME compile_commands.json

.PHONY: clean

clean:
	rm -rf build compile_commands.json .cache tests.tap tests.xml

build/PRJNAME: build
	cmake --build $<

build/PRJNAMEtest: build
	cmake --build $<

compile_commands.json: build/compile_commands.json
	ln -s $<

build/compile_commands.json: build

build: src/*.* Makefile CMakeLists.txt
	cmake -B build .

test: build/PRJNAMEtest
	cmake --build build -t test

format:
	clang-format -i src/**/*.c src/**/*.h
	cmake-format -i CMakeLists.txt
