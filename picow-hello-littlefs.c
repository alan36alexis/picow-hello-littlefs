#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/flash.h"
#include "hardware/sync.h"
//#include "lfs.h"
#include "lfs_pico_implementation.h"

int main() {
    stdio_init_all();
    sleep_ms(2000); // Esperar para estabilizar UART

    lfs_pico_config();

    // Montar el sistema de archivos
    int err = lfs_mount(&lfs, &cfg);
    if (err) {
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
    data_user_t data;

    // Leer los datos actuales
    lfs_file_t file;
    err = lfs_file_open(&lfs, &file, filename, LFS_O_RDONLY);
    if (err == LFS_ERR_OK) {
        lfs_file_read(&lfs, &file, &data, sizeof(data));
        lfs_file_close(&lfs, &file);
        printf("Datos leídos de %s: status=%u, value1=%.1f, value2=%.1f\n", filename, data.status, data.value1, data.value2);
    } else {
        // Inicializar datos si el archivo no existe
        data.value1 = 0.0;
        data.value2 = 1.0;
        data.status = (data.value1 == data.value2) ? 1 : 0;
        printf("No se encontró %s, inicializando: status=%u, value1=%.1f, value2=%.1f\n", filename, data.status, data.value1, data.value2);
    }

    // data.value1 = 0.0;
    // data.value2 = 1.0;
    // Actualizar datos
    data.value1 += 0.1;
    data.value2 -= 0.1;
    data.status = (data.value1 == data.value2) ? 1 : 0;

    // Guardar los datos actualizados
    err = lfs_file_open(&lfs, &file, filename, LFS_O_WRONLY | LFS_O_CREAT);
    if (err == LFS_ERR_OK) {
        lfs_file_write(&lfs, &file, &data, sizeof(data));
        lfs_file_close(&lfs, &file);
        printf("Datos guardados en %s: status=%u, value1=%.1f, value2=%.1f\n", filename, data.status, data.value1, data.value2);
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