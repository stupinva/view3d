#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "texture.h"

// Заголовок PCX-файлов
#pragma pack(push,1)
struct PCXHeader
{
    unsigned char manufacturer;    // - изготовитель
                                   // всегда 10
    unsigned char version;         // - версия
                                   // 0 - 2.5 без палитры (монохромная)
                                   // 2 - 2.8 с палитрой (4-битная)
                                   // 3    - 2.8/3.0 без палитры (8-битная)
                                   // 4 - нет палитры?
                                   // 5 - 3.0 с палитрой.
    unsigned char encoding;        // - тип кодирования
                                   // всегда 1
    unsigned char bits_per_plane;  // - число цветовых битов на плоскость
                                   // 1, 2, 4, 8
    unsigned short x_min;
    unsigned short y_min;
    unsigned short x_max;
    unsigned short y_max;
    unsigned short h_res;          // - разрешение дисплея по горизонтали (единицы неизвестны)
    unsigned short v_res;          // - разрешение дисплея по вертикали (единицы неизвестны)
    RGB palette[16];               // - RGB-палитра для 4-битных изображений
    unsigned char video_mode;      // - номер видеорежима (список номеров режимов неизвестен)
    unsigned char num_planes;      // - число плоскостей
    unsigned short bytes_per_line; // - размер буфера строки в байтах
    unsigned short palette_type;   // - тип палитры: серая или цветная
                                   // крайне противоречивые сведения
    unsigned short sh_res;         // - разрешение сканера по горизонтали (единицы неизвестны)
    unsigned short sv_res;         // - разрешение сканера по вертикали (единицы неизвестны)
    char reserved[54];             // - отведено для будущих версий формата (не дай Бог!)
};
#pragma pack(pop)

// Перечисляются все возможные сочетания полей bits_per_plane и num_planes
const int PCX_NUM_TYPES = 6;
const unsigned char pcx_bits_per_plane[PCX_NUM_TYPES] = {1, 2, 1, 1, 8, 8};
const unsigned char pcx_num_planes[PCX_NUM_TYPES] = {1, 1, 3, 4, 1, 3};

// Таблица для масштабирования 6-битных элементов палитры в 8-битные
const unsigned char scale63to255[] =
{
    0x00, 0x04, 0x08, 0x0C, 0x10, 0x14, 0x18, 0x1C, 0x20, 0x24, 0x28, 0x2D, 0x31, 0x35, 0x39, 0x3D,
    0x41, 0x45, 0x49, 0x4D, 0x51, 0x55, 0x59, 0x5D, 0x61, 0x65, 0x69, 0x6D, 0x71, 0x75, 0x79, 0x7D,
    0x82, 0x86, 0x8A, 0x8E, 0x92, 0x96, 0x9A, 0x9E, 0xA2, 0xA6, 0xAA, 0xAE, 0xB2, 0xB6, 0xBA, 0xBE,
    0xC2, 0xC6, 0xCA, 0xCE, 0xD2, 0xD7, 0xDB, 0xDF, 0xE3, 0xE7, 0xEB, 0xEF, 0xF3, 0xF7, 0xFB, 0xFF
};

// Процедура для декодирования и загрузки строки в буфер из файла
bool loadLinePCX(FILE *file, unsigned char *line_buffer, unsigned bytes_per_line)
{
    // Заполняем буфер строки
    unsigned j = 0;
    while (j < bytes_per_line)
    {
        int byte = fgetc(file);
        if (byte == EOF)
        {
            fprintf(stderr, "Unexpected en of PCX-file.\n");
            return false;
        }

        if ((byte & 0xC0) == 0xC0)
        {
            int number = byte & 0x3F;
            int pixel = fgetc(file);
            if (pixel == EOF)
            {
                fprintf(stderr, "Unexpected en of PCX-file.\n");
                return false;
            }

            if (j + number > bytes_per_line)
            {
                fprintf(stderr, "Detected wrong RLE-code of PCX-file.\n");
                return false;
            }

            while (number > 0)
            {
                line_buffer[j++] = pixel;
                number--;
            }
        }
        else
            line_buffer[j++] = byte;
    }
    return true;
}

// Метод для загрузки изображений
bool Texture::LoadPCXTexture(const char *file_name)
{
    FreeTexture();

    FILE *file;
    if ((file = fopen(file_name, "rb")) == NULL)
    {
        fprintf(stderr, "Could not open PCX-file \"%s\".\n", file_name);
        return false;
    }

    PCXHeader pcx_header;
    if (fread(&pcx_header, sizeof(pcx_header), 1, file) != 1)
    {
        fclose(file);
        fprintf(stderr, "Could not read PCX-file header.\n");
        return false;
    }
    if (pcx_header.manufacturer != 10)
    {
        fclose(file);
        fprintf(stderr, "Supports only manufacturer #10 of PCX-file.\n");
        return false;
    }
    if (pcx_header.encoding != 1)
    {
        fclose(file);
        fprintf(stderr, "Supports only encoding #1 of PCX-file.\n\n");
        return false;
    }

    for(int pcx_type = 0; pcx_type < PCX_NUM_TYPES; pcx_type++)
        if ((pcx_header.bits_per_plane == pcx_bits_per_plane[pcx_type]) &&
            (pcx_header.num_planes == pcx_num_planes[pcx_type]))
            switch (pcx_type)
            {
                unsigned char *line_buffer;
                unsigned j;

                case 0:
                case 1:
                case 2:
                case 3:
                case 5:
                    fclose(file);
                    fprintf(stderr, "bits_per_plane == %d and num_planes == %d "
                                    "- unsupported type of PCX-file.\n",
                                    pcx_header.bits_per_plane, pcx_header.num_planes);
                    return false;

                case 4:
                    // Здесь грузятся 8-битные картинки, в которых не происходит переноса
                    // RLE-кодирования через строки.
                    // Если ПЕРЕНОС МЕЖДУ СТРОК есть, изображение НЕ ЗАГРУЗИТСЯ!
                    //fprintf(stderr, "loading 8-bit pcx-file.\n");
                    if ((line_buffer = (unsigned char *)malloc(pcx_header.bytes_per_line)) == NULL)
                    {
                        fclose(file);
                        fprintf(stderr, "Could not get memory for line of PCX-file.\n");
                        return false;
                    }
                    width = pcx_header.x_max - pcx_header.x_min + 1;
                    height = pcx_header.y_max - pcx_header.y_min + 1;

                    if ((data = malloc(256 * sizeof(RGB) + width * height)) == NULL)
                    {
                        free(line_buffer);
                        fclose(file);
                        fprintf(stderr, "Could not get memory for pixels of PCX-file.\n");
                        return false;
                    }

                    // Цикл по строкам
                    for(j = 0; j < height; j++)
                    {
                        if (!loadLinePCX(file, line_buffer, pcx_header.bytes_per_line))
                        {
                            free(data);
                            free(line_buffer);
                            fclose(file);
                            return false;
                        }
                        // Копирование считанной строки, выравнивающие байты не копируются
                        memcpy(((unsigned char *)data) + j * width + 256 * sizeof(RGB), line_buffer, width);
                    }
                    free(line_buffer);

                    // Осталось прочесть палитру
                    int byte = fgetc(file);
                    if (byte == EOF)
                    {
                        free(data);
                        fclose(file);
                        fprintf(stderr, "Unexpected end of PCX-file.\n");
                        return false;
                    }

                    if ((byte == 0x0C) || (byte == 0x0A))
                        for(j = 0; j < 256; j++)
                        {
                            int red, green, blue;
                            if ((red = fgetc(file)) != EOF)
                            {
                                ((RGB *)data)[j].red = (unsigned char)red;
                                if ((green = fgetc(file)) != EOF)
                                {
                                    ((RGB *)data)[j].green = (unsigned char)green;
                                    if ((blue = fgetc(file)) != EOF)
                                    {
                                        ((RGB *)data)[j].blue = (unsigned char)blue;
                                        continue;
                                    }
                                }
                            }

                            free(data);
                            fclose(file);
                            fprintf(stderr, "Unexpected end of PCX-file.\n");
                            return false;
                        }
                    else
                    {
                        free(data);
                        fclose(file);
                        fprintf(stderr, "PCX-file without palette.\n");
                        return false;
                    }

                    if (byte == 0xC0)
                        for(j = 0; j < 256; j++)
                        {
                            ((RGB *)data)[j].red = scale63to255[((RGB *)data)[j].red];
                            ((RGB *)data)[j].green = scale63to255[((RGB *)data)[j].green];
                            ((RGB *)data)[j].blue = scale63to255[((RGB *)data)[j].blue];
                        }
                    fclose(file);

                    type = TEXTURE_INDEX_RGB;
                    strcpy(name, file_name);
                    return true;
            }
    fprintf(stderr, "File \"%s\" - not a PCX-file.\n", file_name);
    return false;
}
