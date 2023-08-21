all:
	g++ -o main main.cpp -I/opt/homebrew/include -stdlib=libc++ -std=c++20 -arch arm64 -L/opt/homebrew/lib -lSDL2 