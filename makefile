# Target to build the game
game: build_dir ./build/board.o ./build/main.o ./build/game.o
	g++ ./build/main.o ./build/board.o ./build/game.o -o ./build/2048

# Ensure the build directory exists
build_dir:
	mkdir -p build

# Object files
./build/board.o: board.cpp
	g++ -c board.cpp -o ./build/board.o

./build/main.o: main.cpp
	g++ -c main.cpp -o ./build/main.o

./build/game.o: game.cpp
	g++ -c game.cpp -o ./build/game.o

# Valgrind target for debugging
val: game
	g++ -g ./build/main.o ./build/board.o ./build/game.o -o 2048
	valgrind --leak-check=full --track-origins=yes ./2048

# Preprocess target
preprocess: board.cpp game.cpp main.cpp
	g++ board.cpp -E > board.txt
	g++ game.cpp -E > game.txt
	g++ main.cpp -E > main.txt

# Clean up build artifacts
clean:
	rm -f ./build/*.o *.txt 2048
