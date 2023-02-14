#ifndef _AUDIO_H_H___H_
#define _AUDIO_H_H___H_


void AudioInit(u8 volume);
int AudioSingleVoice(unsigned char dat);
int AudioNumericVoice(double num);
int AudioSingleVoiceNoWait(unsigned char dat);
int AudioNumericVoiceDigit(double num, int digit);

#endif
