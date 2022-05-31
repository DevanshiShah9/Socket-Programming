# Makefile 

#     make all
#        Makes everything executable
# 

# compile options to be used in all compile commands
#     NOTE: $(CXXFLAGS) in the compile commands below will be replaced by the options after the =
TARGET = clientA clientB serverM serverA serverB serverC
LIBS = miscellaneous
CC = g++
CFLAGS = -g -Wall -std=c++11


all: $(LIBS) $(TARGET)

$(LIBS): %:%.cpp
	$(CC) -c $(CFLAGS) $< -o $@

$(TARGET): %:%.cpp
	$(CC) $(CFLAGS) $< $(LIBS) -o $@

clean:
	$(RM) $(TARGET) $(LIBS)

remake:clean all