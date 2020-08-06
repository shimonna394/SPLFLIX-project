all: splflix

# Tool invocation
# Executable splflix
splflix: \
	Action \
	bin/Session.o \
	bin/User.o \
	bin/Watchable.o \
	bin/main.o \
	doneComp \
	Linking \
	doneLinking \



Action: src/Action.cpp
	g++ -g -Wall -std=c++11 -c -Include -o bin/Action.o src/Action.cpp

bin/Session.o: src/Session.cpp
	g++ -g -Wall -std=c++11 -c -Include -o bin/Session.o src/Session.cpp

bin/User.o: src/User.cpp
	g++ -g -Wall -std=c++11 -c -Include -o bin/User.o src/User.cpp

bin/Watchable.o: src/Watchable.cpp
	g++ -g -Wall -std=c++11 -c -Include -o bin/Watchable.o src/Watchable.cpp

bin/main.o: src/main.cpp
	g++ -g -Wall -std=c++11 -c -Include -o bin/main.o src/main.cpp
	
doneComp:
	@echo ' '
	@echo ' splflix is compiled and ready to LINKING!'
	@echo ' '
	
Linking:
	g++ -o bin/splflix bin/main.o bin/Action.o bin/Session.o bin/User.o bin/Watchable.o

doneLinking:
	@echo ' '
	@echo ' splflix is LINKED and ready to go!'
	@echo ' '

#Clean the build directory
clean:
	rm -rf bin/*
