.PHONY: build clean
default: build

build:
	make -C realize
	make -C template-c

clean:
	make -C realize $@
	make -C template-c $@

install:
	sudo make -C realize $@

