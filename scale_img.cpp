#include <stdio.h>
#include <stdlib.h>
/*
//Для суммы цветов точек используются числа с фиксированной запятой в формате 24.8.
unsigned MAX_FRACTION	=0xFF;

struct dst2src
{
	unsigned u;
	unsigned f0;
	unsigned f1;
};
void min_max_round(float x,dst2src *d2s)
{
	d2s->u=(unsigned)x;
	d2s->f1=(unsigned)((x-(d2s->u))*MAX_FRACTION);
	d2s->f0=(unsigned)((1.0f-x+(d2s->u))*MAX_FRACTION);
}

bool ScaleRGBWidth(unsigned src_width,unsigned height,RGB *src_pixels,
				   unsigned dst_width,RGB **dst_pixels)
{
	dst2src *dst2src_x;
	if (dst2src_x=(dst2src *)malloc((dst_width+1)*sizeof(dst2src)))
	{
		fprintf(stderr,"Could not get memory for resizing image.\n");
		return false;
	}
	if ((*dst_pixels=(RGB *)malloc(height*dst_width*sizeof(RGB)))==NULL)
	{
		free(dst2src_x);
		fprintf(stderr,"Could not get memory for resized image.\n");
		return false;
	}

	float x_scale=(float)src_width/(float)dst_width;

	for(unsigned dst_x=0;dst_x<dst_width+1;dst_x++)
		min_max_round(x_scale*dst_x,&(dst2src_x[dst_x]));

	for(unsigned adr_y=0;adr_y<height*dst_width;ard_y+=dst_width)
		for(unsigned dst_x=0;dst_x<dst_width;dst_x++)
		{
			unsigned red=0;
			unsigned green=0;
			unsigned blue=0;
			unsigned denom=0;

			unsigned ux0=dst2src_x[dst_x].u;
			unsigned ux1=dst2src_x[dst_x+1].u;
			unsigned fx0=dst2src_x[dst_x].f0;
			unsigned fx1=dst2src_x[dst_x+1].f1;
		
			red+=(unsigned)src_pixels[adr_dst_y+ux0].red*fx0+
				(unsigned)src_pixels[adr_dst_y+ux1].red*fx1;
			green+=(unsigned)src_pixels[adr_dst_y+ux0].green*fx0+
				(unsigned)src_pixels[adr_dst_y+ux1].green*fx1;
			blue+=(unsigned)src_pixels[adr_dst_y+ux0].blue*fx0+
				(unsigned)src_pixels[adr_dst_y+ux1].blue*fx1;

			denom+=fx0+fx1;

			for(unsigned x=ux0+1;x<ux1;x++)
			{
				red+=(unsigned)src_pixels[adr_dst_y+x].red*MAX_FRACTION;
				green+=(unsigned)src_pixels[adr_dst_y+x].green*MAX_FRACTION;
				blue+=(unsigned)src_pixels[adr_dst_y+x].blue*MAX_FRACTION;
				denom+=MAX_FRACTION;
			}

			(*dst_pixels)[adr_dst_y+dst_x].red=(unsigned char)(red/denom);
			(*dst_pixels)[adr_dst_y+dst_x].green=(unsigned char)(green/denom);
			(*dst_pixels)[adr_dst_y+dst_x].blue=(unsigned char)(blue/denom);
		}
	free(dst2src_x);
	return true;
}
*/