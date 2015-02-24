compile: clean
	g++ src/main.cpp src/util.cpp -o jarfish -Isrc -Izlib -lz

clean:
	-@rm *.o jarfish 2>/dev/null || true
