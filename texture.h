#ifndef __TEXTURE__
#define __TEXTURE__
/////////////////////////////////////////////////////////////
//05-07-2003                                               //
//Структура для хранения текстур и класс словаря текстур.  //
/////////////////////////////////////////////////////////////

#ifndef __RGB__
#define __RGB__
struct RGB
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};
#endif

const unsigned TEXTURE_NOT_LOADED	=0;
const unsigned TEXTURE_QUAKE		=1;
const unsigned TEXTURE_QUAKE2		=2;
const unsigned TEXTURE_INDEX_RGB	=3;
const unsigned TEXTURE_RGB			=4;

struct Texture
{
	char name[128];
	unsigned type;
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