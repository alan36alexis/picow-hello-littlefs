#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/flash.h"
#include "hardware/sync.h"
#include "lfs.h"

// Definir el offset y tamaño para el sistema de archivos LittleFS
#define FLASH_OFFSET (1 * 1024 * 1024 + 768 * 1024) // 1.75 MB desde el inicio
#define FLASH_SIZE (256 * 1024) // 256 KB para LittleFS
#define FLASH_SECTOR_SIZE (1u << 12) // 4 KB por sector
#define FLASH_PAGE_SIZE (1u << 8) // 256 bytes por página

// Configuración de LittleFS
#define READ_SIZE 32
#define PROG_SIZE 256
#define BLOCK_SIZE FLASH_SECTOR_SIZE // 4 KB
#define BLOCK_COUNT (FLASH_SIZE / BLOCK_SIZE) // 256 KB / 4 KB = 64 bloques
#define LOOKAHEAD_SIZE 64 // Tamaño del búfer de lookahead (en bits)

// Prototipos
int lfs_pico_config (void);

// Definición de la estructura
typedef struct {
    uint8_t status;
    float value1;
    float value2;
} data_user_t;

// Búfer estático para el lookahead de LittleFS
static uint8_t lookahead_buffer[LOOKAHEAD_SIZE / 8]; // 64 bits = 8 bytes

// Estructura para la configuración de LittleFS
lfs_t lfs;
struct lfs_config cfg;