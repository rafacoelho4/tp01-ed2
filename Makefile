main: main.o index.o
	gcc main.o -o main

main.o: main.c
	gcc -c main.c

################ Pesquisa Sequencial Indexada ################

index.o: ./sequencial_indexado/index.c
	gcc -c ./sequencial_indexado/index.c

clean:
	del -f *.o main.exe

remove:
	rm -f *.o main.exe

# Em windows, usar comando make clean
# Em linux, comando make remove

#### EXEMPLO MAKEFILE ####

# output: main.o garage.o
#         g++ -std=c++0x -Wall main.o garage.o -o output

#  main.o: main.cpp
#         g++ -c main.cpp

#  garage.o: garage.cpp garage.hpp
#         g++ -c garage.cpp

#  clean:
#         rm *.o output