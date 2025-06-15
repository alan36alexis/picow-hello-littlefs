LittleFS Example for Raspberry Pi Pico W
This project demonstrates how to use the LittleFS filesystem to store and retrieve a 32-bit integer value in the non-volatile Flash memory of a Raspberry Pi Pico W using the Pico SDK. The program reads a value from a file (data.bin), increments it, and writes it back, persisting the data across power cycles.
Features

Initializes a LittleFS filesystem on the Pico W's 2 MB Flash memory.
Reserves 256 KB of Flash at offset 1.75 MB for the filesystem.
Reads and writes a uint32_t value to a file named data.bin.
Implements wear leveling and power-loss resilience via LittleFS.
Outputs debug messages over UART for monitoring.

Requirements

Hardware:
Raspberry Pi Pico W
USB cable or UART interface for debugging


Software:
Pico SDK (version 2.1.1 or later recommended)
LittleFS (version 2.8.1 recommended)
CMake (version 3.13 or later)
ARM GCC toolchain (arm-none-eabi-gcc)
Terminal emulator (e.g., minicom or PuTTY) for UART output


Environment:
PICO_SDK_PATH environment variable set to the Pico SDK directory



Project Structure
picow-hello-littlefs/
├── picow-hello-littlefs.c  # Main source file
├── CMakeLists.txt         # CMake build configuration
└── littlefs/              # LittleFS library
    ├── lfs.c
    ├── lfs.h
    ├── lfs_util.c
    ├── lfs_util.h

Setup Instructions

Clone the Pico SDK (if not already installed):
git clone https://github.com/raspberrypi/pico-sdk.git
cd pico-sdk
git submodule update --init

Set the PICO_SDK_PATH environment variable:
export PICO_SDK_PATH=/path/to/pico-sdk


Download LittleFS:Clone the LittleFS repository and copy the required files to the littlefs directory:
git clone --branch v2.8.1 https://github.com/littlefs-project/littlefs.git
mkdir -p picow-hello-littlefs/littlefs
cp littlefs/lfs.c littlefs/lfs.h littlefs/lfs_util.c littlefs/lfs_util.h picow-hello-littlefs/littlefs/


Prepare the Project:Ensure the project directory contains picow-hello-littlefs.c, CMakeLists.txt, and the littlefs folder with the above files.

Build the Project:
cd picow-hello-littlefs
mkdir build
cd build
cmake ..
make

This generates picow-hello-littlefs.uf2 in the build directory.

Flash the Pico W:

Hold the BOOTSEL button on the Pico W while connecting it via USB.
Copy picow-hello-littlefs.uf2 to the USB mass storage device that appears.
The Pico W will restart and run the program.


Monitor Output:

Connect a terminal emulator to the Pico W's UART (115200 baud rate).
Example using minicom:minicom -b 115200 -o -D /dev/ttyACM0


Expected output on first run:Formateando sistema de archivos...
No se encontró data.bin, inicializando en 0
Valor guardado en data.bin: 1


On subsequent runs:Valor leído de data.bin: 1
Valor guardado en data.bin: 2





Program Description

Flash Configuration:
Uses 256 KB of Flash starting at 1.75 MB offset (out of 2 MB total).
Flash is divided into 64 blocks of 4 KB each.


LittleFS Setup:
Initializes LittleFS with a block size of 4 KB, read size of 32 bytes, and program size of 256 bytes.
Implements wear leveling with 100 cycles.


Operation:
Mounts the LittleFS filesystem, formatting it if necessary.
Reads a 32-bit value from data.bin. If the file doesn't exist, initializes the value to 0.
Increments the value and writes it back to data.bin.
Outputs status messages via UART.


Dependencies:
Pico SDK: pico_stdlib, hardware_flash, hardware_sync
LittleFS: lfs.c, lfs_util.c



Notes

Flash Usage: The program reserves 256 KB for LittleFS, leaving ~1.75 MB for the program and bootloader. Adjust FLASH_OFFSET and FLASH_SIZE in picow-hello-littlefs.c to use more or less space.
UART Output: Configured for UART (not USB). Modify CMakeLists.txt to enable USB output if needed.
Extending the Program: To store more data, modify the file operations to write structures or multiple files. Example:struct config { uint32_t value1; uint32_t value2; };
struct config data = {1234, 5678};
lfs_file_write(&lfs, &file, &data, sizeof(data));


Wear Leveling: LittleFS automatically distributes writes to prevent Flash wear, unlike direct Flash access.

Troubleshooting

"lfs_config undefined" Error:
Ensure lfs.h is in littlefs/ and contains struct lfs_config.
Verify target_include_directories in CMakeLists.txt includes littlefs.
Clean the build directory: rm -rf build/* and re-run cmake .. && make.


No UART Output:
Check the terminal is connected to the correct UART port and baud rate.
Ensure pico_enable_stdio_uart is enabled in CMakeLists.txt.


Build Errors:
Confirm PICO_SDK_PATH is set.
Run make VERBOSE=1 to inspect compiler commands.



License
This project is licensed under the MIT License. See the LittleFS repository for its license details.
Acknowledgments

Raspberry Pi Foundation for the Pico SDK
LittleFS Project for the filesystem library