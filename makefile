compile: 
	g++ -s src/main.cpp src/util.cpp src/classparser.cpp -o jarfish -Isrc -Isrc-ext

compiledebug: 
	g++ src/main.cpp src/util.cpp src/classparser.cpp -o jarfish -Isrc -Isrc-ext

timecompile: 
	g++ -DTIMER=1 src/main.cpp src/util.cpp src/classparser.cpp -o jarfish -Isrc -Isrc-ext

clean:
	-@rm *.o jarfish 2>/dev/null || true

parser:
	g++ src/parser.cpp src/classparser.cpp -o classfish -Isrc -Isrc-ext
