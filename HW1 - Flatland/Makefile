output: main.o
	g++ main.o -o a.out -lsfml-graphics -lsfml-window -lsfml-system
	./a.out

debug: 
	g++ -g main.cpp -o a.out -lsfml-graphics -lsfml-window -lsfml-system

main.o: main.cpp
	g++ -c main.cpp -I/home/me/sfml/include


clean:
	rm *.o a.out