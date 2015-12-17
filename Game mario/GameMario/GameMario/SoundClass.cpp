#include "SoundClass.h"


///////////////////////////////////////////////////////////////////////////////
// Filename: soundclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "soundclass.h"

//Use the class constructor to initialize the private member variables that are used inside the sound class.

SoundClass* SoundClass::m_instance = NULL;

SoundClass::SoundClass()
{
	m_DirectSound = 0;
	m_primaryBuffer = 0;
	m_instance = 0;
}


SoundClass::~SoundClass()
{
}


bool SoundClass::initialize(HWND hwnd)
{
	bool result;
	//First initialize the DirectSound API as well as the primary buffer.Once that is initialized then the LoadWaveFile function can be called which will load in the.wav audio file and initialize the secondary buffer with the audio information from the.wav file.After loading is complete then PlayWaveFile is called which then plays the.wav file once.

	// Initialize direct sound and the primary sound buffer.
	result = initializeDirectSound(hwnd);
	if (!result)
	{
		return false;
	}

	// Load a wave audio file onto a secondary buffer.
	loadWaveFile("Sounds/smb_1-up.wav", &smb_1_up);
	loadWaveFile("Sounds/smb_breakblock.wav", &smb_breakblock);
	loadWaveFile("Sounds/smb_world_clear.wav", &smb_world_clear);
	loadWaveFile("Sounds/smb_stage_clear.wav", &smb_stage_clear);
	loadWaveFile("Sounds/smb_bump.wav", &smb_bump);
	loadWaveFile("Sounds/smb_coin.wav", &smb_coin);
	loadWaveFile("Sounds/smb_fireball.wav", &smb_fireball);
	loadWaveFile("Sounds/smb_fireworks.wav", &smb_fireworks);
	loadWaveFile("Sounds/smb_gameover.wav", &smb_gameover);
	loadWaveFile("Sounds/smb_jump-small.wav", &smb_jump_small);
	loadWaveFile("Sounds/smb_jump-super.wav", &smb_jump_super);
	loadWaveFile("Sounds/smb_kick.wav", &smb_kick);
	loadWaveFile("Sounds/smb_mariodie.wav", &smb_mariodie);
	loadWaveFile("Sounds/smb_pause.wav", &smb_pause);
	loadWaveFile("Sounds/smb_pipe.wav", &smb_pipe);
	loadWaveFile("Sounds/smb_powerup.wav", &smb_powerup);
	loadWaveFile("Sounds/smb_powerup_appears.wav", &smb_powerup_appears);
	loadWaveFile("Sounds/smb_stomp.wav", &smb_stomp);
	loadWaveFile("Sounds/smb_vine.wav", &smb_vine);
	loadWaveFile("Sounds/smb_warning.wav", &smb_warning);
	loadWaveFile("Sounds/smb_flagpole.wav", &smb_flagpole);

	return true;
}

//The Shutdown function first releases the secondary buffer which held the.wav file audio data using the ShutdownWaveFile function.Once that completes this function then called ShutdownDirectSound which releases the primary buffer and the DirectSound interface.
void SoundClass::shutdown()
{
	// Release the secondary buffer.
	shutdownWaveFile(&smb_1_up);
	shutdownWaveFile(&smb_breakblock);
	shutdownWaveFile(&smb_bump);
	shutdownWaveFile(&smb_coin);
	shutdownWaveFile(&smb_fireball);
	shutdownWaveFile(&smb_fireworks);
	shutdownWaveFile(&smb_gameover);
	shutdownWaveFile(&smb_jump_small);
	shutdownWaveFile(&smb_jump_super);
	shutdownWaveFile(&smb_kick);
	shutdownWaveFile(&smb_mariodie);
	shutdownWaveFile(&smb_pause);
	shutdownWaveFile(&smb_pipe);
	shutdownWaveFile(&smb_powerup);
	shutdownWaveFile(&smb_powerup_appears);
	shutdownWaveFile(&smb_stage_clear);
	shutdownWaveFile(&smb_stomp);
	shutdownWaveFile(&smb_vine);
	shutdownWaveFile(&smb_warning);
	shutdownWaveFile(&smb_world_clear);
	// Shutdown the Direct Sound API.
	shutdownDirectSound();

	return;
}

//InitializeDirectSound handles getting an interface pointer to DirectSound and the default primary sound buffer.Note that you can query the system for all the sound devices and then grab the pointer to the primary sound buffer for a specific device, however I've kept this tutorial simple and just grabbed the pointer to the primary buffer of the default sound device.
bool SoundClass::initializeDirectSound(HWND hwnd)
{
	HRESULT result;
	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;


	// Initialize the direct sound interface pointer for the default sound device.
	result = DirectSoundCreate8(NULL, &m_DirectSound, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level to priority so the format of the primary sound buffer can be modified.
	result = m_DirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
	if (FAILED(result))
	{
		return false;
	}

	// We have to setup the description of how we want to access the primary buffer.The dwFlags are the important part of this structure.In this case we just want to setup a primary buffer description with the capability of adjusting its volume.There are other capabilities you can grab but we are keeping it simple for now.
	// Setup the primary buffer description.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// Get control of the primary sound buffer on the default sound device.
	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &m_primaryBuffer, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Now that we have control of the primary buffer on the default sound device we want to change its format to our desired audio file format.Here I have decided we want high quality sound so we will set it to uncompressed CD audio quality.
	// Setup the format of the primary sound bufffer.
	// In this case it is a .WAV file recorded at 44,100 samples per second in 16-bit stereo (cd audio format).
	ZeroMemory(&waveFormat, sizeof(WAVEFORMATEX));
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;

	// Set the primary buffer to be the wave format specified.
	result = m_primaryBuffer->SetFormat(&waveFormat);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

//The ShutdownDirectSound function handles releasing the primary buffer and DirectSound interfaces.
void SoundClass::shutdownDirectSound()
{
	// Release the primary sound buffer pointer.
	if (m_primaryBuffer)
	{
		m_primaryBuffer->Release();
		m_primaryBuffer = 0;
	}

	// Release the direct sound interface pointer.
	if (m_DirectSound)
	{
		m_DirectSound->Release();
		m_DirectSound = 0;
	}

	return;
}

//The LoadWaveFile function is what handles loading in a.wav audio file and then copies the data onto a new secondary buffer.If you are looking to do different formats you would replace this function or write a similar one.
bool SoundClass::loadWaveFile(char* filename, IDirectSoundBuffer8** secondaryBuffer)
{
	int error;
	FILE* filePtr;
	unsigned int count;
	WaveHeaderType waveFileHeader;
	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferDesc;
	HRESULT result;
	IDirectSoundBuffer* tempBuffer;
	unsigned char* waveData;
	unsigned char *bufferPtr;
	unsigned long bufferSize;
	//To start first open the.wav file and read in the header of the file.The header will contain all the information about the audio file so we can use that to create a secondary buffer to accommodate the audio data.The audio file header also tells us where the data begins and how big it is.You will notice I check for all the needed tags to ensure the audio file is not corrupt and is the proper wave file format containing RIFF, WAVE, fmt, data, and WAVE_FORMAT_PCM tags.I also do a couple other checks to ensure it is a 44.1KHz stereo 16bit audio file.If it is mono, 22.1 KHZ, 8bit, or anything else then it will fail ensuring we are only loading the exact format we want.

	// Open the wave file in binary.
	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
	{
		return false;
	}

	// Read in the wave file header.
	count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Check that the chunk ID is the RIFF format.
	if ((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') ||
		(waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F'))
	{
		return false;
	}

	// Check that the file format is the WAVE format.
	if ((waveFileHeader.format[0] != 'W') || (waveFileHeader.format[1] != 'A') ||
		(waveFileHeader.format[2] != 'V') || (waveFileHeader.format[3] != 'E'))
	{
		return false;
	}

	// Check that the sub chunk ID is the fmt format.
	if ((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') ||
		(waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' '))
	{
		return false;
	}

	// Check that the audio format is WAVE_FORMAT_PCM.
	if (waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
	{
		return false;
	}

	// Check for the data chunk header.
	if ((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') ||
		(waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a'))
	{
		return false;
	}

	//Now that the wave header file has been verified we can setup the secondary buffer we will load the audio data onto.We have to first set the wave format and buffer description of the secondary buffer similar to how we did for the primary buffer.There are some changes though since this is secondary and not primary in terms of the dwFlags and dwBufferBytes.		
	// Set the wave format of secondary buffer that this wave file will be loaded onto.
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 22050 / 2;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// Set the buffer description of the secondary sound buffer that the wave file will be loaded onto.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// Now the way to create a secondary buffer is fairly strange.First step is that you create a temporary IDirectSoundBuffer with the sound buffer description you setup for the secondary buffer.If this succeeds then you can use that temporary buffer to create a IDirectSoundBuffer8 secondary buffer by calling QueryInterface with the IID_IDirectSoundBuffer8 parameter.If this succeeds then you can release the temporary buffer and the secondary buffer is ready for use.
	// Create a temporary sound buffer with the specific buffer settings.
	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Test the buffer format against the direct sound 8 interface and create the secondary buffer.
	result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the temporary buffer.
	tempBuffer->Release();
	tempBuffer = 0;
	// Now that the secondary buffer is ready we can load in the wave data from the audio file.First I load it into a memory buffer so I can check and modify the data if I need to.Once the data is in memory you then lock the secondary buffer, copy the data to it using a memcpy, and then unlock it.This secondary buffer is now ready for use.Note that locking the secondary buffer can actually take in two pointers and two positions to write to.This is because it is a circular buffer and if you start by writing to the middle of it you will need the size of the buffer from that point so that you don't write outside the bounds of it. This is useful for streaming audio and such. In this tutorial we create a buffer that is the same size as the audio file and write from the beginning to make things simple.
	// Move to the beginning of the wave data which starts at the end of the data chunk header.
	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	// Create a temporary buffer to hold the wave file data.
	waveData = new unsigned char[waveFileHeader.dataSize];
	if (!waveData)
	{
		return false;
	}

	// Read in the wave file data into the newly created buffer.
	count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
	if (count != waveFileHeader.dataSize)
	{
		return false;
	}

	// Close the file once done reading.
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	// Lock the secondary buffer to write wave data into it.
	result = (*secondaryBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
	if (FAILED(result))
	{
		return false;
	}

	// Copy the wave data into the buffer.
	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

	// Unlock the secondary buffer after the data has been written to it.
	result = (*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
	if (FAILED(result))
	{
		return false;
	}

	// Release the wave data since it was copied into the secondary buffer.
	delete[] waveData;
	waveData = 0;

	return true;
}

//ShutdownWaveFile just does a release of the secondary buffer.
void SoundClass::shutdownWaveFile(IDirectSoundBuffer8** secondaryBuffer)
{
	// Release the secondary sound buffer.
	if (*secondaryBuffer)
	{
		(*secondaryBuffer)->Release();
		*secondaryBuffer = 0;
	}
	return;
}

//The PlayWaveFile function will play the audio file stored in the secondary buffer.The moment you use the Play function it will automatically mix the audio into the primary buffer and start it playing if it wasn't already. Also note that we set the position to start playing at the beginning of the secondary sound buffer otherwise it will continue from where it last stopped playing. And since we set the capabilities of the buffer to allow us to control the sound we set the volume to maximum here.
bool SoundClass::playWaveFile(IDirectSoundBuffer8* soundBuffer)
{
	// if soundBuffer is playing before, it will be stop
	soundBuffer->Stop();

	HRESULT result;
	// Set position at the beginning of the sound buffer.
	result = soundBuffer->SetCurrentPosition(0);
	if (FAILED(result))
	{
		return false;
	}

	// Set volume of the buffer to 100%.
	result = soundBuffer->SetVolume(DSBVOLUME_MAX);
	if (FAILED(result))
	{
		return false;
	}

	// Play the contents of the secondary sound buffer.
	result = soundBuffer->Play(0, 0, 0);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void SoundClass::playWaveFile(IDSounds idSound)
{
	// Play the wave file now that it has been loaded.
	playWaveFile(getSound(idSound));
}

IDirectSoundBuffer8* SoundClass::getSound(IDSounds idSound)
{
	switch (idSound)
	{
	case IDSounds::Sound_1_Up:
		return smb_1_up;
		break;
	case IDSounds::Sound_BreakBlock:
		return smb_breakblock;
		break;
	case IDSounds::Sound_Coin:
		return smb_coin;
		break;
	case IDSounds::Sound_FireBall:
		return smb_fireball;
		break;
	case IDSounds::Sound_FireWorks:
		return smb_fireworks;
		break;
	case IDSounds::Sound_GameOver:
		return smb_gameover;
		break;
	case IDSounds::Sound_Jump_Small:
		return smb_jump_small;
		break;
	case IDSounds::Sound_Jump_Super:
		return smb_jump_super;
		break;
	case IDSounds::Sound_Kick:
		return smb_kick;
		break;
	case IDSounds::Sound_MarioDie:
		return smb_mariodie;
		break;
	case IDSounds::Sound_Pause:
		return smb_pause;
		break;
	case IDSounds::Sound_Pipe:
		return smb_pipe;
		break;
	case IDSounds::Sound_PowerUp:
		return smb_powerup;
		break;
	case IDSounds::Sound_PowerUp_Appears:
		return smb_powerup_appears;
		break;
	case IDSounds::Sound_StageCLear:
		return smb_stage_clear;
		break;
	case IDSounds::Sound_Stomp:
		return smb_stomp;
		break;
	case IDSounds::Sound_Vine:
		return smb_vine;
		break;
	case IDSounds::Sound_Warning:
		return smb_warning;
		break;
	case IDSounds::Sound_WorldCLear:
		return smb_world_clear;
		break;
	case IDSounds::Sound_FlagPole:
		return smb_flagpole;
		break;
	}
}

SoundClass* SoundClass::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new SoundClass();
	}

	return m_instance;
}