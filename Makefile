# Makefile for the bank program
CCC = g++
CXXFLAGS = -g -Wall -std=c++11
CXXLINK = $(CCC)
OBJS = main.o bank.o Account.o ATM.o
TARGET = Bank
RM = rm -f
# Creating the  executable
$(TARGET): $(OBJS)
	$(CXXLINK) -o $(TARGET) $(OBJS) -lpthread
# Creating the object files
account.o: Account.cpp Account.h
bank.o: bank.cpp bank.h Account.h
atm.o: ATM.cpp ATM.h bank.h Account.h
main.o: main.cpp bank.h Account.h ATM.h
# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ "#"* core.*

