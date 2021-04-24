INCLUDE := ./include

main: main.cpp
	g++ -g -std=c++17 -I$(INCLUDE) main.cpp -o main