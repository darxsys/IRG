CC = g++
LD = g++

NAME = zadatak1
CCFLAGS = -c
LDFLAGS = -lglut -lGLU

SRC_DIR = src
OBJ_DIR = obj

SRC = $(shell find src -type f -regex ".*\.cpp")
OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

all: $(NAME)

$(NAME): $(OBJ)
	@echo [LD];
	$(LD) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo [CC];
	$(CC) $< -o $@ $(CCFLAGS)

clean:
	rm $(OBJ_DIR)/%.o $(NAME) -rf