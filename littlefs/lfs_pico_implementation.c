#include "lfs_pico_implementation.h"

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

int lfs_pico_config (void) {
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

    return LFS_ERR_OK;
}