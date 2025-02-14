#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct BMPHeader {
    uint16_t header; // Decimal Value -> 19778
    uint32_t size_of_bmp_file;
    uint16_t app_specific0;
    uint16_t app_specific1;
    uint32_t pixel_array_offset;
} BMPHeader;

typedef struct DIBHeader {
    uint32_t number_of_bytes;
    uint32_t width;
    uint32_t height;
    uint16_t number_of_color_planes;
    uint16_t bits_per_pixel;
    uint32_t used_compression;
    uint32_t size_of_raw_bitmap;
    uint32_t horizontal_print_resolution;
    uint32_t vertical_print_resolution;
    uint32_t number_of_colors_in_palette;
    uint32_t number_of_important_colors;
} DIBHeader;


void saveAsBMP(const char *FILEPATH, uint8_t *Canvas, int32_t Channels, int32_t Width, int32_t Height)
{
    uint8_t padding = (4 - (Width * Channels) % 4) % 4;

    BMPHeader BMPheader = {
        19778,
        54 + ((Width * Channels) + padding) * Height,
        0,
        0,
        54
    };

    DIBHeader DIBheader = {
        40,
        Width,
        Height,
        1,
        Channels * 8,
        0,
        ((Width * Channels) + padding) *Height,
        2835,
        2835,
        0,
        0
    };

    FILE *file;
    fopen_s(&file, FILEPATH, "wb");
    if (NULL == file) {
        printf("Error saving bmp file!\n");
        return;
    }

    fwrite(&BMPheader.header, sizeof(BMPheader.header), 1, file);
    fwrite(&BMPheader.size_of_bmp_file, sizeof(BMPheader.size_of_bmp_file), 1, file);
    fwrite(&BMPheader.app_specific0, sizeof(BMPheader.app_specific0), 1, file);
    fwrite(&BMPheader.app_specific1, sizeof(BMPheader.app_specific1), 1, file);
    fwrite(&BMPheader.pixel_array_offset, sizeof(BMPheader.pixel_array_offset), 1, file);

    fwrite(&DIBheader.number_of_bytes, sizeof(DIBheader.number_of_bytes), 1, file);
    fwrite(&DIBheader.width, sizeof(DIBheader.width), 1, file);
    fwrite(&DIBheader.height, sizeof(DIBheader.height), 1, file);
    fwrite(&DIBheader.number_of_color_planes, sizeof(DIBheader.number_of_color_planes), 1, file);
    fwrite(&DIBheader.bits_per_pixel, sizeof(DIBheader.bits_per_pixel), 1, file);
    fwrite(&DIBheader.used_compression, sizeof(DIBheader.used_compression), 1, file);
    fwrite(&DIBheader.size_of_raw_bitmap, sizeof(DIBheader.size_of_raw_bitmap), 1, file);
    fwrite(&DIBheader.horizontal_print_resolution, sizeof(DIBheader.horizontal_print_resolution), 1, file);
    fwrite(&DIBheader.vertical_print_resolution, sizeof(DIBheader.vertical_print_resolution), 1, file);
    fwrite(&DIBheader.number_of_colors_in_palette, sizeof(DIBheader.number_of_colors_in_palette), 1, file);
    fwrite(&DIBheader.number_of_important_colors, sizeof(DIBheader.number_of_important_colors), 1, file);

    uint8_t zero = 0;
    for (int y = 0; Height > y; ++y) {
        for (int x = 0; Width > x; ++x) {
            fwrite((uint32_t *) Canvas + x + (y * Width), Channels, 1, file);
        }
        for (int p = 0; padding > p; ++p) {
            fwrite(&zero, sizeof(uint8_t), 1, file);
        }
    }

    fclose(file);
}