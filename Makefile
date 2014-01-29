test : time_tool.o main.o
	g++ -o test main.o time_tool.o
time_tool.o : time_tool.h time_tool.cpp
	g++ -c -std=c++0x time_tool.cpp
main.o : unittest/main.cpp
	g++ -c -std=c++0x unittest/main.cpp
clean : 
	rm *.o test
