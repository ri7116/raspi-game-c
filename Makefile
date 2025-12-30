CC = gcc
CFLAGS = -Wall -O2 -I. -I./src -I./driver -I/usr/local/include
LDFLAGS = -L/usr/local/lib -lbcm2835

# 모든 소스 파일 경로 명시
SRCS = main.c \
       driver/st7789.c \
       src/common/global_variable.c \
       src/common/input_handler.c \
       src/common/system_init.c \
       src/logic/logic.c \
       src/rendering/randering.c

# 오브젝트 파일 경로 (소스와 동일한 위치)
OBJS = main.o \
       driver/st7789.o \
       src/common/global_variable.o \
       src/common/input_handler.o \
       src/common/system_init.o \
       src/logic/logic.o \
       src/rendering/randering.o

TARGET = main

.PHONY: all clean rebuild

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# 각 폴더별 컴파일 규칙
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET)
	rm -f *.o
	rm -f driver/*.o
	rm -f src/common/*.o
	rm -f src/logic/*.o
	rm -f src/rendering/*.o

rebuild:
	$(MAKE) clean
	$(MAKE) all
