all:
	g++ -std=c++17 -Isrc/include -Lsrc/lib -o main.exe main.cpp -lmingw32 -lSDL2main -lSDL2
