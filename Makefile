TARGET=demo
CC=gcc
# CFLAGS=-Wall `pkg-config --cflags opencv` -O3 -flto -ffast-math
INCLUDE=-Isrc/
CFLAGS=$(INCLUDE) -Wall -O0 -g

OPENCV_LIB=-lopencv_imgproc -lopencv_imgcodecs -lopencv_highgui -lopencv_core
LDFLAGS=-L/usr/local/lib -L/home/chujie/anaconda3/lib
LDFLAGS+=$(OPENCV_LIB) -lm -licui18n -licuuc -licudata
# VPATH=./src/
C_SRC=$(wildcard src/*.c)
OBJ=$(patsubst %.c, %.o, $(C_SRC))

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -rf $(OBJ) $(TARGET)