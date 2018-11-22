#ifndef __EVENTS__
#define __EVENTS__

void Draw(void);
void SwitchLighting(void);
void SwitchDrawNormals(void);
void DistUp(void);
void DistDown(void);
void PrevFrame(void);
void NextFrame(void);
void YawUp(void);
void YawDown(void);
void AngleRight(void);
void AngleLeft(void);
int Init(const char *filename);
void Reshape(unsigned width, unsigned height);

#endif
