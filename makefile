compile: clean
	g++ src/main.cpp src/util.cpp -o jarfish -Isrc

clean:
	-@rm *.o jarfish 2>/dev/null || true
