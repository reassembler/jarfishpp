compile: 
	g++ src/main.cpp src/util.cpp -o jarfish -Isrc -Isrc-ext

clean:
	-@rm *.o jarfish 2>/dev/null || true
