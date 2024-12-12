//--------------------------------------------------------------//
//	"san_XAudio2.cpp"											//
//	     XAudio2管理												//
//													2024/11/04	//
//														sanda	//
//--------------------------------------------------------------//
#include "../../framework.h"
#include "../san_environment.h"

//#define sanXAUDIO2_OUTPUT_INFO (1)

IXAudio2* sanXAudio2::pXAudio = NULL;
IXAudio2MasteringVoice* sanXAudio2::pMasteringVoice = NULL;
XAUDIO2_VOICE_DETAILS sanXAudio2::MasterVoiceDetails = {};
DWORD sanXAudio2::dwChannelMask = 0;

HRESULT sanXAudio2::initialize()
{
	HRESULT hr;

	// COM初期化
	hr = CoInitializeEx(0, COINIT_MULTITHREADED);
	assert(hr == S_OK);

	// XAudio2のインスタンス作成
	hr = XAudio2Create(&pXAudio);
	assert(hr == S_OK);

	// マスターボイス作成
	hr = pXAudio->CreateMasteringVoice(&pMasteringVoice);
	assert(hr == S_OK);

	// スピーカーチャンネルマスクを取得
	hr = pMasteringVoice->GetChannelMask(&dwChannelMask);

	// スピーカーの配置情報
#ifdef sanXAUDIO2_OUTPUT_INFO
	//Speaker Positions:
	sanFont::output(L"Active Speaker Positions\n");
	if (dwChannelMask & SPEAKER_FRONT_LEFT)				sanFont::output(L"\tSPEAKER_FRONT_LEFT\n");
	if (dwChannelMask & SPEAKER_FRONT_RIGHT)			sanFont::output(L"\tSPEAKER_FRONT_RIGHT\n");
	if (dwChannelMask & SPEAKER_FRONT_CENTER)			sanFont::output(L"\tSPEAKER_FRONT_CENTER\n");
	if (dwChannelMask & SPEAKER_LOW_FREQUENCY)			sanFont::output(L"\tSPEAKER_LOW_FREQUENCY\n");
	if (dwChannelMask & SPEAKER_BACK_LEFT)				sanFont::output(L"\tSPEAKER_BACK_LEFT\n");
	if (dwChannelMask & SPEAKER_BACK_RIGHT)				sanFont::output(L"\tSPEAKER_BACK_RIGHT\n");
	if (dwChannelMask & SPEAKER_FRONT_LEFT_OF_CENTER)	sanFont::output(L"\tSPEAKER_FRONT_LEFT_OF_CENTER\n");
	if (dwChannelMask & SPEAKER_FRONT_RIGHT_OF_CENTER)	sanFont::output(L"\tSPEAKER_FRONT_RIGHT_OF_CENTER\n");
	if (dwChannelMask & SPEAKER_BACK_CENTER)			sanFont::output(L"\tSPEAKER_BACK_CENTER\n");
	if (dwChannelMask & SPEAKER_SIDE_LEFT)				sanFont::output(L"\tSPEAKER_SIDE_LEFT\n");
	if (dwChannelMask & SPEAKER_SIDE_RIGHT)				sanFont::output(L"\tSPEAKER_SIDE_RIGHT\n");
	if (dwChannelMask & SPEAKER_TOP_CENTER)				sanFont::output(L"\tSPEAKER_TOP_CENTER\n");
	if (dwChannelMask & SPEAKER_TOP_FRONT_LEFT)			sanFont::output(L"\tSPEAKER_TOP_FRONT_LEFT\n");
	if (dwChannelMask & SPEAKER_TOP_FRONT_CENTER)		sanFont::output(L"\tSPEAKER_TOP_FRONT_CENTER\n");
	if (dwChannelMask & SPEAKER_TOP_FRONT_RIGHT)		sanFont::output(L"\tSPEAKER_TOP_FRONT_RIGHT\n");
	if (dwChannelMask & SPEAKER_TOP_BACK_LEFT)			sanFont::output(L"\tSPEAKER_TOP_BACK_LEFT\n");
	if (dwChannelMask & SPEAKER_TOP_BACK_CENTER)		sanFont::output(L"\tSPEAKER_TOP_BACK_CENTER\n");
	if (dwChannelMask & SPEAKER_TOP_BACK_RIGHT)			sanFont::output(L"\tSPEAKER_TOP_BACK_RIGHT\n");

	//DirectSound Speaker Config
	sanFont::output(L"DirectSound Speaker Config\n");
	switch (dwChannelMask)
	{
	case KSAUDIO_SPEAKER_MONO:		sanFont::output(L"\tKSAUDIO_SPEAKER_MONO\n");	break;
	case KSAUDIO_SPEAKER_1POINT1:	sanFont::output(L"\tKSAUDIO_SPEAKER_1POINT1\n"); break;
	case KSAUDIO_SPEAKER_STEREO:	sanFont::output(L"\tKSAUDIO_SPEAKER_STEREO\n");	break;
	case KSAUDIO_SPEAKER_2POINT1:	sanFont::output(L"\tKSAUDIO_SPEAKER_2POINT1\n"); break;
	case KSAUDIO_SPEAKER_3POINT0:	sanFont::output(L"\tKSAUDIO_SPEAKER_3POINT0\n"); break;
	case KSAUDIO_SPEAKER_3POINT1:	sanFont::output(L"\tKSAUDIO_SPEAKER_3POINT1\n"); break;
	case KSAUDIO_SPEAKER_QUAD:		sanFont::output(L"\tKSAUDIO_SPEAKER_QUAD\n");	break;
	case KSAUDIO_SPEAKER_SURROUND:	sanFont::output(L"\tKSAUDIO_SPEAKER_SURROUND\n"); break;
	case KSAUDIO_SPEAKER_5POINT0:	sanFont::output(L"\tKSAUDIO_SPEAKER_5POINT0\n"); break;
	case KSAUDIO_SPEAKER_5POINT1:	sanFont::output(L"\tKSAUDIO_SPEAKER_5POINT1\n"); break;
	case KSAUDIO_SPEAKER_7POINT0:	sanFont::output(L"\tKSAUDIO_SPEAKER_7POINT0\n"); break;
	case KSAUDIO_SPEAKER_7POINT1:	sanFont::output(L"\tKSAUDIO_SPEAKER_7POINT1\n"); break;
	case KSAUDIO_SPEAKER_5POINT1_SURROUND:	sanFont::output(L"\tKSAUDIO_SPEAKER_5POINT1_SURROUND\n"); break;
	case KSAUDIO_SPEAKER_7POINT1_SURROUND:	sanFont::output(L"\tKSAUDIO_SPEAKER_7POINT1_SURROUND\n"); break;
	}
#endif

	pMasteringVoice->GetVoiceDetails(&MasterVoiceDetails);

	return S_OK;
}

void sanXAudio2::terminate()
{
	if (pMasteringVoice)
	{
		pMasteringVoice->DestroyVoice();
		pMasteringVoice = NULL;
	}
	SAFE_RELEASE(pXAudio);

	CoUninitialize();
}

// ソースボイスの作成
HRESULT sanXAudio2::CreateSourceVoice(IXAudio2SourceVoice** pSource, WAVEFORMATEX* wfex)
{
	return pXAudio->CreateSourceVoice(pSource, wfex);
}
