CC=clang++
CFLAGS=-c -g -std=c++11 -I /usr/local/include
SOURCES=main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
LIBS=-pthread -lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videostab -lboost_filesystem -lboost_system
LDFLAGS=-L /usr/local/lib /usr/local/lib/libiomp5.dylib

all: spectrumTest

spectrumTest: $(OBJECTS)
	$(CC) -o spectrumTest $(OBJECTS) $(LDFLAGS) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $<

clean:
	rm -f spectrumTest $(OBJECTS)
