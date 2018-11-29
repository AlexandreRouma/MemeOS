cmake_minimum_required(VERSION 3.9)
project(MemeOS)

enable_language(ASM)

set(CMAKE_ASM_COMPILER "i686-elf-as")
set(CMAKE_C_COMPILER "i686-elf-gcc")
set(CMAKE_CXX_COMPILER "i686-elf-g++")

set(CMAKE_CXX_FLAGS  "-ffreestanding -O2 -fno-rtti -Wno-write-strings -Wno-multichar -Wno-unused-parameter -Wno-overflow -Wno-narrowing -fno-exceptions -Wno-trigraphs ${CMAKE_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS  "-T ../linker.ld -ffreestanding -O2 -nostdlib -lgcc -Wwrite-strings ${CMAKE_EXE_LINKER_FLAGS}")

include_directories(MemeOS "./")

# Kernel source
file(GLOB KERN_SRC      "*.cpp" "*.asm")

if(${ARCH} STREQUAL "x86")
    # Configure code

    # Driver source
    file(GLOB DRV_FS_EXT2   "drivers/filesystems/ext2/src/*.cpp" "drivers/filesystems/ext2/src/*.asm")
    file(GLOB DRV_KEYBOARD  "drivers/keyboard/src/*.cpp" "drivers/keyboard/src/*.asm")
    file(GLOB DRV_PC_SPKR   "drivers/pc_speaker/src/*.cpp" "drivers/pc_speaker/src/*.asm")
    file(GLOB DRV_PCI       "drivers/pci/src/*.cpp" "drivers/pci/src/*.asm")
    file(GLOB DRV_RTC       "drivers/rtc/src/*.cpp" "drivers/rtc/src/*.asm")
    file(GLOB DRV_SERIAL    "drivers/serial/src/*.cpp" "drivers/serial/src/*.asm")
    file(GLOB DRV_ST_AHCI   "drivers/storage/ahci/src/*.cpp" "drivers/storage/ahci/src/*.asm")
    file(GLOB DRV_ST_ATAPIO "drivers/storage/atapio/src/*.cpp" "drivers/storage/atapio/src/*.asm")
    file(GLOB DRV_ST_FLOPPY "drivers/storage/floppy/src/*.cpp" "drivers/storage/floppy/src/*.asm")
    file(GLOB DRV_TEXT_TERM "drivers/text_term/src/*.cpp" "drivers/text_term/src/*.asm")
    file(GLOB DRV_TIMER     "drivers/timer/src/*.cpp" "drivers/timer/src/*.asm")

    # Library source
    file(GLOB LIB_KERNEL    "libs/kernel/src/*.cpp" "libs/kernel/src/*.asm")
    file(GLOB LIB_STD       "libs/std/src/*.cpp" "libs/std/src/*.asm")
    file(GLOB LIB_SYS       "libs/sys/src/*.cpp" "libs/sys/src/*.asm")

    # Shell source
    file(GLOB SHELL_SRC     "shell/*.cpp" "shell/*.asm")

    set(SRCS    ${KERN_SRC} ${DRV_FS_EXT2} ${DRV_KEYBOARD} ${DRV_PC_SPKR} ${DRV_PCI}
                ${DRV_RTC} ${DRV_SERIAL} ${DRV_ST_AHCI} ${DRV_ST_ATAPIO} ${DRV_ST_FLOPPY}
                ${DRV_TEXT_TERM} ${DRV_TIMER} ${LIB_KERNEL} ${LIB_STD} ${LIB_SYS}
                ${SHELL_SRC})
elseif(${ARCH} STREQUAL "arm")
    set(SRCS    ${KERN_SRC})
elseif(${ARCH} STREQUAL "nspire")
    set(SRCS    ${KERN_SRC})
endif(${ARCH} STREQUAL "arm")



add_executable(MemeOS ${SRCS})


set_target_properties(MemeOS PROPERTIES OUTPUT_NAME "MemeOS.bin")
#set_target_properties(MemeOS PROPERTIES LINK_DEPENDS "../linker.ld")

#add_custom_command(TARGET MemeOS POST_BUILD COMMAND cp MemeOS.bin ../ISO/boot/MemeOS && grub-mkrescue -o MemeOS.iso ../ISO)
add_custom_target(iso COMMAND cp MemeOS.bin ../ISO/boot/MemeOS && grub-mkrescue -o MemeOS.iso ../ISO)
add_dependencies(iso MemeOS)

#add_custom_command(TARGET MemeOS POST_BUILD COMMAND genzehn --input tiapp.elf --output tiapp.tns --name "helloworld-cpp")