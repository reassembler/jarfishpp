compile: miniz.o
	g++ miniz.o src/main.cpp src/util.cpp -o jarfish -Isrc -Izlib -lz

miniz.o:
	g++ -c src-ext/miniz.c


clean:
	-@rm *.o jarfish 2>/dev/null || true
