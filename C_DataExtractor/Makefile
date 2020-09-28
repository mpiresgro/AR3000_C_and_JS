# https://www.embarcados.com.br/introducao-ao-makefile/

# Name of the project
PROJ_NAME=extractor

# .c files
C_SOURCE=$(shell find $(./source/) -name '*.c')

# .h files
H_SOURCE=$(shell find $(./source/) -name '*.h')

# Object files
OBJ=$(subst .c,.o,$(subst source,objects,$(C_SOURCE)))

# Compiler and linker
CC=gcc

# Flags for compiler
CC_FLAGS=-c

# Command used at clean target
RM = rm -rf

#
# Compilation and linking
#
all: objFolder $(PROJ_NAME)

$(PROJ_NAME): $(OBJ)
	@ echo 'Building binary using GCC linker: $@'
	$(CC) $^ -o $@
	@ echo 'Finished building binary: $@'
	@ echo ' '

./objects/%.o: ./source/%.c ./source/%.h
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(CC_FLAGS) -o $@
	@ echo ' '

./objects/main.o: ./source/main.c $(H_SOURCE)
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(CC_FLAGS) -o $@
	@ echo ' '

objFolder:
	@ mkdir -p objects
	@ mkdir -p objects/TCP_Client
	@ mkdir -p objects/PK_Extractor

clean:
	@ $(RM) ./objects/*.o $(PROJ_NAME) *~
	@ rm -rf objects/TCP_Client
	@ rm -rf objects/PK_Extractor
	@ rm -rf objects

.PHONY: all clean