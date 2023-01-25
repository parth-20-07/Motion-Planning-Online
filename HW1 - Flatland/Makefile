output: main.o
	g++ main.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system
	./sfml-app

main.o: main.cpp
	g++ -c main.cpp -I/home/me/sfml/include


clean:
	rm *.o sfml-app