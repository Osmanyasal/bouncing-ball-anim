all: bin 
	g++ -O0 -Wall ./src/*.cc -o ./bin/main.out -lGL -lglfw -lGLEW -I./include && ./bin/main.out
	
bin:
	mkdir ./bin
	echo "bin created, files moved"
 
