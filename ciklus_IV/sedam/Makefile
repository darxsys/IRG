CC = g++
LD = g++

CC_FLAGS = -c -Wall -Wno-sign-compare
LD_FLAGS = -lglut -lGLU

NAME = sjencanje

SRC_DIR = src
OBJ_DIR = obj

SRC = $(shell find src -type f -regex ".*\.cpp")
OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

all: $(NAME)

$(NAME): $(OBJ)
	@echo [LD]
	$(LD) $^ -o $@ $(LD_FLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo [CC]
	$(CC) $< -o $@ $(CC_FLAGS)

clean:
	@rm -f $(NAME) $(OBJ_DIR)/*