TARGET=demo

CC=gcc
INCLUDE=-I/home/aia/opt/opencv-3.0.0/include -Isrc
CFLAGS=-Wall $(INCLUDE) -O3 -flto -ffast-math
# CFLAGS=-Wall `pkg-config --cflags opencv` -O3 -flto -ffast-math
# CFLAGS=-Wall `pkg-config --cflags opencv` -O0 -g
OPENCV=-lopencv_highgui -lopencv_imgproc -lopencv_core -lopencv_imgcodecs
# LDFLAGS= `pkg-config --libs opencv` -lm 
LDFLAGS=-L/home/aia/opt/opencv-3.0.0/lib
LDFLAGS+= $(OPENCV) -lm
VPATH=./src/

C_SRC = $(wildcard src/*.c)

OBJ=$(patsubst %.c, %.o, $(C_SRC))


all: $(TARGET)

$(TARGET): $(OBJ)
	echo $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -rf $(OBJ) $(TARGET)