#Nhan Cao
controller: controller.o
	g++ -pthread -std=c++11 -o controller controller.o 

controller.o: controller.cpp buffer.h
	g++ -std=c++11 -g -pthread -c controller.cpp 

clean:
	rm -f *.o controller *~



