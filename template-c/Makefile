default: build/PRJNAME

.PHONY: clean

clean:
	rm -rf build

build: src/*.* Makefile CMakeLists.txt
	cmake -B build .

build/PRJNAME: build
	cmake --build $<
