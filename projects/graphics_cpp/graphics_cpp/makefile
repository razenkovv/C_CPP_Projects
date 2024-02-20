OBJ = main.o functions.o
TARGET = task
EIGEN_PATH = ~/eigen/eigen-3.4.0/

all: $(OBJ) link

main.o: main.cpp
	g++ -c main.cpp -o main.o -I $(EIGEN_PATH)

functions.o: functions.cpp
	g++ -c functions.cpp -o functions.o -I $(EIGEN_PATH)

run:
	./$(TARGET)

link: main.o
	g++ main.o functions.o -I eigen -o $(TARGET) -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm -rf $(TARGET) *.o

# make all
# make run