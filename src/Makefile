all: install run

install:
	mkdir build
	cmake CMakeLists -S ./3D_Viewer_QT -B ./build
	
run:
	cd build && make 3D_Viewer && open 3D_Viewer.app/Contents/MacOS/3D_Viewer

uninstall:
	rm -rf build

clean:
	cd build && make clean && cd ..
	cd tests && make clean && cd ..