list_test: list_test.o
	g++ -g -Wall -std=c++11 -fopenmp list_test.o -o list_test

list_test.o: list_test.cpp Linked_list.h Linked_list.hpp List_node.h List_node.hpp
	g++ -g -Wall -std=c++11 -fopenmp -c list_test.cpp

clean: 	
	rm *.o list_test
	@echo Clean done.
