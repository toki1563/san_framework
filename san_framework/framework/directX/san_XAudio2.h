//--------------------------------------------------------------//
//	"san_XAudio2.h"												//
//	     XAudio2管理												//
//													2024/11/04	//
//														sanda	//
//--------------------------------------------------------------//
#pragma once

#include <ks.h>
#include <ksmedia.h>

class sanXAudio2
{
private:
	static IXAudio2* pXAudio;                        //	XAudio2インターフェースへのポインタ
	static IXAudio2MasteringVoice* pMasteringVoice;	 // マスターボイスのインターフェースへのポインタ	
	static XAUDIO2_VOICE_DETAILS MasterVoiceDetails; // マスターボイスの詳細情報格納
	static DWORD dwChannelMask;

public:
	// システム関数
	static HRESULT initialize();
	static void terminate();

	static HRESULT CreateSourceVoice(IXAudio2SourceVoice **pSource, WAVEFORMATEX *wfex);

	static const XAUDIO2_VOICE_DETAILS* const masterVoiceDetails() { return &MasterVoiceDetails; }

	static DWORD channelMask() { return dwChannelMask; }
};