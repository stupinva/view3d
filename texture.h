#ifndef __TEXTURE__
#define __TEXTURE__

////////////////////////////////////////////////////////////
// 05-07-2003                                             //
// Структура для хранения текстур и класс словаря текстур //
////////////////////////////////////////////////////////////

#ifndef __RGB__
#define __RGB__
struct RGB
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};
#endif

typedef enum texture_type_e
{
    TEXTURE_NOT_LOADED,
    TEXTURE_QUAKE,
    TEXTURE_QUAKE2,
    TEXTURE_INDEX_RGB,
    TEXTURE_RGB
} texture_type_t;

struct Texture
{
    char name[128];
    texture_type_t type;
    unsigned width;
    unsigned height;
    unsigned num_levels;
    void *data;
public:
    Texture(void);
    ~Texture(void);
    bool LoadPCXTexture(const char *file_name);
    bool ConvertTexture(void);
    bool DrawTexture(void);
    //bool SetTexture(void);
    void FreeTexture(void);
};

#endif
