# Makefile文件
.PHONY: clean all rebuild check


BUILD 	:= build
SRC		:= $(wildcard ./source/*.cpp)
CC		:= g++
CFALGS	:= -I include/
TARGET	:= $(BUILD)/C_ZERO


all:$(BUILD) $(TARGET)
	@echo "Build Success ==> C_ZERO"


$(BUILD):
	@mkdir $(BUILD)


$(TARGET): $(SRC)
	@echo "Compile"
	@$(CC) $(CFALGS) -o $@ $^


clean:
	rm -rf $(BUILD) 

rebuild: clean all

check: $(TARGET)
