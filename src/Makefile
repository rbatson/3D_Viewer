all: install run

install:
	mkdir build
	cmake CMakeLists -S ./3D -B ./build
	
run:
	cd build && make 3D && open 3D.app/Contents/MacOS/3D

uninstall:
	rm -rf build

clean:
	cd build && make clean && cd ..
	cd tests && make clean && cd ..