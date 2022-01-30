default: build/PRJNAME compile_commands.json

.PHONY: clean

clean:
	rm -rf build compile_commands.json

build/PRJNAME: build
	cmake --build $<

compile_commands.json: build/compile_commands.json
	ln -s $<

build/compile_commands.json: build

build: src/*.* Makefile CMakeLists.txt
	cmake -B build .

