#ifndef _BEEP_H_
#define _BEEP_H_

void BeepInit();
void BeepEnable(u8 enabled);
u8 IsBeepEnabled();

void ModelBeep(u32 freq, u32 ms); //Beep and delay, xxx ms. 
void Beep(u32 freq, u32 ms);      //Beep without blocking.
void PlayStartMusic(s16* music,int duration);
void PlayMusicMultiThreadInit(int music_id,u32 duration);
void PlayMusicMultiThread();

#endif