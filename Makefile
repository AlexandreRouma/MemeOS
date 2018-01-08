INCLUDES=libs
SRCFILES=$(wildcard libs/*.cpp)
OBJFILES=$(patsubst libs/%.cpp,build/%.o,$(SRCFILES))
CC=i686-elf-g++
CFLAGS=
CPPFLAGS=-ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -Wwrite-strings -I$(INCLUDES)

all: bin/MemeOS.bin

bin/MemeOS.bin: build/isr.o build/asm_gdt.o $(OBJFILES) build/loader.o build/kernel.o
	i686-elf-g++ -T linker.ld -o bin/MemeOS.bin -ffreestanding -O2 -nostdlib build/isr.o build/asm_gdt.o $(OBJFILES) build/loader.o build/kernel.o -lgcc

build/loader.o: loader.asm
	i686-elf-as loader.asm -o build/loader.o

build/kernel.o: kernel.cpp
	i686-elf-g++ $(CPPFLAGS) -c kernel.cpp -o build/kernel.o

build/isr.o: libs/isr.asm
	i686-elf-as libs/isr.asm -o build/isr.o

build/asm_gdt.o: libs/asm_gdt.asm
	i686-elf-as libs/asm_gdt.asm -o build/asm_gdt.o

build/%.o : libs/%.cpp
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@