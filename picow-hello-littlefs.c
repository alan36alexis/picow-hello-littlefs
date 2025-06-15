#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/flash.h"
#include "hardware/sync.h"
#include "littlefs/lfs.h"

// Definir el offset y tamaño para el sistema de archivos LittleFS
#define FLASH_OFFSET (1 * 1024 * 1024 + 768 * 1024) // 1.75 MB desde el inicio
#define FLASH_SIZE (256 * 1024) // 256 KB para LittleFS
#define FLASH_SECTOR_SIZE (1u << 12) // 4 KB por sector
#define FLASH_PAGE_SIZE 256 // 256 bytes por página

// Configuración de LittleFS
#define READ_SIZE 32
#define PROG_SIZE 256
#define BLOCK_SIZE FLASH_SECTOR_SIZE // 4 KB
#define BLOCK_COUNT (FLASH_SIZE / BLOCK_SIZE) // 256 KB / 4 KB = 64 bloques
#define LOOKAHEAD_SIZE 64 // Tamaño del búfer de lookahead (en bits)

// Estructura para la configuración de LittleFS
lfs_t lfs;
struct lfs_config cfg;

// Funciones de interfaz con la Flash
int flash_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size) {
    uint32_t addr = FLASH_OFFSET + (block * BLOCK_SIZE) + off;
    memcpy(buffer, (const void *)(XIP_BASE + addr), size);
    return LFS_ERR_OK;
}

int flash_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size) {
    uint32_t addr = FLASH_OFFSET + (block * BLOCK_SIZE) + off;
    uint32_t interrupts = save_and_disable_interrupts();
    flash_range_program(addr, buffer, size);
    restore_interrupts(interrupts);
    return LFS_ERR_OK;
}

int flash_erase(const struct lfs_config *c, lfs_block_t block) {
    uint32_t addr = FLASH_OFFSET + (block * BLOCK_SIZE);
    uint32_t interrupts = save_and_disable_interrupts();
    flash_range_erase(addr, BLOCK_SIZE);
    restore_interrupts(interrupts);
    return LFS_ERR_OK;
}

int flash_sync(const struct lfs_config *c) {
    return LFS_ERR_OK; // No se necesita sincronización adicional
}

// Búfer estático para el lookahead de LittleFS
static uint8_t lookahead_buffer[LOOKAHEAD_SIZE / 8]; // 64 bits = 8 bytes

int main() {
    stdio_init_all();
    sleep_ms(2000); // Esperar para estabilizar USB

    // Configurar LittleFS
    cfg.read = flash_read;
    cfg.prog = flash_prog;
    cfg.erase = flash_erase;
    cfg.sync = flash_sync;
    cfg.read_size = READ_SIZE;
    cfg.prog_size = PROG_SIZE;
    cfg.block_size = BLOCK_SIZE;
    cfg.block_count = BLOCK_COUNT;
    cfg.block_cycles = 100; // Ciclos para wear leveling
    cfg.cache_size = 256;
    cfg.lookahead_size = LOOKAHEAD_SIZE;
    cfg.read_buffer = NULL; // Usar búfer interno
    cfg.prog_buffer = NULL; // Usar búfer interno
    cfg.lookahead_buffer = lookahead_buffer;
    cfg.name_max = 0; // Usar máximo predeterminado
    cfg.file_max = 0; // Usar máximo predeterminado
    cfg.attr_max = 0; // Usar máximo predeterminado

    // Montar el sistema de archivos
    int err = lfs_mount(&lfs, &cfg);
    if (err) {
        // Si falla, formatear e intentar montar de nuevo
        printf("Formateando sistema de archivos...\n");
        lfs_format(&lfs, &cfg);
        err = lfs_mount(&lfs, &cfg);
        if (err) {
            printf("Error al montar LittleFS: %d\n", err);
            while (true) sleep_ms(1000);
        }
    }

    // Nombre del archivo
    const char *filename = "data.bin";
    uint32_t value;

    // Leer el valor actual
    lfs_file_t file;
    err = lfs_file_open(&lfs, &file, filename, LFS_O_RDONLY);
    if (err == LFS_ERR_OK) {
        lfs_file_read(&lfs, &file, &value, sizeof(value));
        lfs_file_close(&lfs, &file);
        printf("Valor leído de %s: %u\n", filename, value);
    } else {
        value = 0; // Valor inicial si el archivo no existe
        printf("No se encontró %s, inicializando en %u\n", filename, value);
    }

    // Incrementar y guardar el valor
    value++;
    err = lfs_file_open(&lfs, &file, filename, LFS_O_WRONLY | LFS_O_CREAT);
    if (err == LFS_ERR_OK) {
        lfs_file_write(&lfs, &file, &value, sizeof(value));
        lfs_file_close(&lfs, &file);
        printf("Valor guardado en %s: %u\n", filename, value);
    } else {
        printf("Error al escribir en %s: %d\n", filename, err);
    }

    // Desmontar el sistema de archivos
    lfs_unmount(&lfs);

    // Bucle infinito
    while (true) {
        sleep_ms(1000);
    }
}