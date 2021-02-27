main: main.o index.o
	g++ main.o -o main

main.o: main.cpp
	g++ -std=c++0x main.cpp

################ Pesquisa Sequencial Indexada ################

index.o: ./sequencial_indexado/index.cpp
	g++ -std=c++0x ./sequencial_indexado/index.cpp

clean:
	del -f *.o main.exe

remove:
	rm -f *.o main.exe

################ Árvore B* ################
b_star.o: ./arvore_b_estrela/b_star.cpp
	g++ -std=c++0x -Wall ./arvore_b_estrela/b_star.cpp

clean:
	del -f *.o b_star.exe

remove:
	rm -f *.o b_star.exe

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