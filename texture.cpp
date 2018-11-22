#include <GL/gl.h>
#include <stdlib.h>
#include <stdio.h>
#include "misc.h"
#include "texture.h"

Texture::Texture(void)
{
    name[0] = 0;
    type = TEXTURE_NOT_LOADED;
}

Texture::~Texture(void)
{
    FreeTexture();
}

void Texture::FreeTexture(void)
{
    if (type != TEXTURE_NOT_LOADED)
    {
        name[0] = 0;
        free(data);
        type = TEXTURE_NOT_LOADED;
    }
}

bool Texture::ConvertTexture(void)
{
    RGB *src_palette;
    unsigned char *src_pixels;
    RGB *dst_pixels;
    
    if (type == TEXTURE_QUAKE)
    {
        //src_palette = (RGB *)quake_palette;
        src_pixels = (unsigned char *)data;
    }
    else if (type == TEXTURE_QUAKE2)
    {
        //src_palette = (RGB *)quake2_palette;
        src_pixels = (unsigned char *)data;
    }
    else if (type == TEXTURE_INDEX_RGB)
    {
        src_palette = (RGB *)data;
        src_pixels = (unsigned char *)data + 256 * sizeof(RGB);
    }
    else if (type == TEXTURE_RGB)
    {
        return true;
    }
    else
    {
        return false;
    }

    dst_pixels = (RGB *)malloc(width * height * sizeof(*dst_pixels));
    if (dst_pixels == NULL)
    {
        fprintf(stderr, "Could not get memory for pixels.\n");
        return false;
    }

    for(unsigned i = 0; i < width * height; i++)
    {
        dst_pixels[i] = src_palette[src_pixels[i]];
    }

    free(data);
    data = dst_pixels;
    type = TEXTURE_RGB;

    return true;
}

bool Texture::DrawTexture(void)
{

    if (type != TEXTURE_RGB)
    {
        ConvertTexture();
    }

    if (type == TEXTURE_RGB)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        //glRasterPos2f(0.0f, 0.0f);
        glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
        return true;
    }
    return false;
}
