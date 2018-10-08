# <-- MemeOS Makefile v2.0 -->
# Copyright xX_WhatsTheGeek_Xx (c) 2018 - 2019

# CONFIGURATION
DELETE = rm -f 

# DIRECTORIES
SRC_DIR = ./
OBJ_DIR = ./obj
INC_DIR = ./

# C++
CPP_FILES = $(shell find -type f -name '*.cpp' -not -path "./libs/_old/*")

# ASSEMBLY
ASM_FILES = $(shell find -type f -name '*.asm' -not -path "./libs/_old/*")

# COMPILER STUFF
# unused: -Wall -Wextra
GPP_CMD = i686-elf-g++
GAS_CMD = i686-elf-as
LDFLAGS = -T linker.ld -ffreestanding -O2 -nostdlib -lgcc -Wwrite-strings
CPPFLAGS = -I $(INC_DIR) -ffreestanding -O2 -fno-rtti -Wno-write-strings -Wno-multichar -Wno-unused-parameter -Wno-overflow -Wno-narrowing -fno-exceptions -Wno-trigraphs
CXXFLAGS = 
GASFLAGS = 

OBJ_ASM = $(addsuffix .o, $(basename $(ASM_FILES)))
OBJ_CPP = $(addsuffix .o, $(basename $(CPP_FILES)))

OBJ_FILES = $(OBJ_ASM) $(OBJ_CPP)
OUTPUT = ./bin/MemeOS.bin
ISO_OUTPUT = ./bin/MemeOS.iso

all: $(OUTPUT)

clean:
	$(DELETE) ./obj/*
	$(DELETE) ./bin/*

iso: all
	cp $(OUTPUT) ./ISO/boot/MemeOS
	grub-mkrescue -o $(ISO_OUTPUT) ./ISO

folders:
	mkdir -p ./bin
	mkdir -p ./obj

$(OUTPUT): $(OBJ_FILES)
	$(GPP_CMD) $(LDFLAGS) -o $@ $(addprefix $(OBJ_DIR)/, $(notdir $^))

$(OBJ_CPP): $(CPP_FILES)
	$(GPP_CMD) $(CPPFLAGS) $(CXXFLAGS) -c -o $(OBJ_DIR)/$(notdir $@) $(basename $@).cpp

$(OBJ_ASM): $(ASM_FILES)
	$(GAS_CMD) $(GASFLAGS) -o $(OBJ_DIR)/$(notdir $@) $(basename $@).asm