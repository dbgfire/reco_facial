CC = g++
CFLAGS = -g -Wall
SRCS = faceRecognition.cpp
PROG = face

OPENCV = `pkg-config opencv --cflags --libs`
LIBS = $(OPENCV)

$(PROG):$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)