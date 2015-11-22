#pragma once
#include "Global.h"
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>



class SoundClass
{
private:
	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

public:
	
	~SoundClass();

	void playWaveFile(IDSounds);
	bool initialize(HWND);
	void shutdown();
	static SoundClass* getInstance();
private:
	SoundClass();

	bool initializeDirectSound(HWND);
	void shutdownDirectSound();

	bool loadWaveFile(char*, IDirectSoundBuffer8**);
	void shutdownWaveFile(IDirectSoundBuffer8**);

	bool playWaveFile(IDirectSoundBuffer8*);
	IDirectSoundBuffer8*	getSound(IDSounds);
private:
	static SoundClass* m_instance;
	
	IDirectSound8* m_DirectSound;
	IDirectSoundBuffer* m_primaryBuffer;

	IDirectSoundBuffer8* smb_1_up;
	IDirectSoundBuffer8* smb_breakblock;
	IDirectSoundBuffer8* smb_bump;
	IDirectSoundBuffer8* smb_coin;
	IDirectSoundBuffer8* smb_fireball;
	IDirectSoundBuffer8* smb_fireworks;
	IDirectSoundBuffer8* smb_gameover;
	IDirectSoundBuffer8* smb_jump_small;
	IDirectSoundBuffer8* smb_jump_super;
	IDirectSoundBuffer8* smb_kick;
	IDirectSoundBuffer8* smb_mariodie;
	IDirectSoundBuffer8* smb_pause;
	IDirectSoundBuffer8* smb_pipe;
	IDirectSoundBuffer8* smb_powerup;
	IDirectSoundBuffer8* smb_powerup_appears;
	IDirectSoundBuffer8* smb_stage_clear;
	IDirectSoundBuffer8* smb_stomp;
	IDirectSoundBuffer8* smb_vine;
	IDirectSoundBuffer8* smb_warning;
	IDirectSoundBuffer8* smb_world_clear;
};
