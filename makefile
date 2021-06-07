INCLUDE := ./include

main: bdpt.cpp
	g++ -g -std=c++17 -I$(INCLUDE) bdpt.cpp -o main

# main: main.cpp
# 	g++ -g -std=c++17 -I$(INCLUDE) main.cpp -o main