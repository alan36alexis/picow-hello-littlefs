#ifndef LFS_PICO_IMPLEMENTATION_H
#define LFS_PICO_IMPLEMENTATION_H

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

// Inicializar la configuración de LittleFS
void lfs_pico_init(void);

// Montar el sistema de archivos
int lfs_mount_filesystem(lfs_t *lfs);

// Desmontar el sistema de archivos
void lfs_unmount_filesystem(lfs_t *lfs);

#endif // LFS_PICO_IMPLEMENTATION_H