.PHONY: build, run, build-and-run

build:
	rm -rf build 
	mkdir build 
	cd build && cmake .. && make && mv laba .. 
	rm -rf build 

run:
	mkdir -p output
	./laba
	rm laba

build-and-run:
	make build
	make run