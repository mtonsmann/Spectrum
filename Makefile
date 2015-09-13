CC=clang++
CFLAGS=-c -g -std=c++11 -I /usr/local/include
SOURCES=main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
LIBS= -lopencv_core -lopencv_highgui -lopencv_imgproc  -lopencv_video  -lboost_filesystem -lboost_system
LDFLAGS=-L /usr/local/lib

all: spectrumTest

spectrumTest: $(OBJECTS)
	$(CC) -o spectrumTest $(OBJECTS) $(LDFLAGS) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $<

clean:
	rm -f spectrumTest $(OBJECTS)
