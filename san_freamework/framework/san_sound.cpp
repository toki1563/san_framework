//--------------------------------------------------------------//
//	"san_sound.cpp"												//
//	     WAVEファイル再生クラス(XAudio2)							//
//													2024/11/12	//
//														sanda	//
//--------------------------------------------------------------//
#include "../framework.h"
#include "san_environment.h"

sanSound::sanSound(const WCHAR* path)
{
	HRESULT hr;
	MMCKINFO mSrcWaveFile;
	MMCKINFO mSrcWaveFmt;
	MMCKINFO mSrcWaveData;
	LPWAVEFORMATEX wf;

	pSourceVoice = NULL;

	// WAVファイルをロード
	HMMIO hSrc;
	hSrc = mmioOpen((LPWSTR)path, NULL, MMIO_ALLOCBUF | MMIO_READ | MMIO_COMPAT);
	if (!hSrc)
	{
		return; // WAVファイルがNULLの時
	}

	// 'WAVE'チャンクチェック
	ZeroMemory(&mSrcWaveFile, sizeof(mSrcWaveFile));
	hr = mmioDescend(hSrc, &mSrcWaveFile, NULL, MMIO_FINDRIFF);
	if (mSrcWaveFile.fccType != mmioFOURCC('W', 'A', 'V', 'E'))
	{
		mmioClose(hSrc, 0);
		return;
	}

	// 'fmt'チェンクチェック
	ZeroMemory(&mSrcWaveFmt, sizeof(mSrcWaveFmt));
	hr = mmioDescend(hSrc, &mSrcWaveFmt, &mSrcWaveFile, MMIO_FINDCHUNK);
	if (mSrcWaveFmt.ckid != mmioFOURCC('f', 'm', 't', ' '))
	{
		mmioClose(hSrc, 0);
		return;
	}

	// ヘッダサイズの計算
	int iSrcHeaderSize = mSrcWaveFmt.cksize;
	if (iSrcHeaderSize < sizeof(WAVEFORMATEX))
	{
		iSrcHeaderSize = sizeof(WAVEFORMATEX);
	}

	// ヘッダメモリ確保
	wf = (LPWAVEFORMATEX)new BYTE[iSrcHeaderSize];
	if (!wf)
	{
		mmioClose(hSrc, 0);
		return;
	}
	ZeroMemory(wf, iSrcHeaderSize);

	// WAVEフォーマットロード
	hr = mmioRead(hSrc, (char*)wf, mSrcWaveFmt.cksize);
	if (FAILED(hr))
	{
		delete[] wf;
		mmioClose(hSrc, 0);
		return;
	}

	// fmtチャンクに戻る
	mmioAscend(hSrc, &mSrcWaveFmt, 0);

	// fataチャンクを探す
	while (true)
	{
		// 検索
		hr = mmioDescend(hSrc, &mSrcWaveData, &mSrcWaveFile, 0);
		if (FAILED(hr))
		{
			delete[] wf;
			mmioClose(hSrc, 0);
			return;
		}
		if (mSrcWaveData.ckid == mmioStringToFOURCC(L"data", 0))
		{
			break;
		}
		// 次のチャンクへ
		hr = mmioAscend(hSrc, &mSrcWaveData, 0);
	}

	sanFont::output(L"%s\n", path);
	sanFont::output(L"チャンネル数		= %d\n", wf->nChannels);
	sanFont::output(L"サンプリングレート	= %d\n", wf->nSamplesPerSec);
	sanFont::output(L"ビットレート		= %d\n", wf->wBitsPerSample);
	sanFont::output(L"データサイズ		= %d\n", mSrcWaveData.cksize);

	const BYTE* buffer = new BYTE[mSrcWaveData.cksize];
	mmioRead(hSrc, (char*)buffer, mSrcWaveData.cksize);

	// ソースボイスの作成
	hr = sanXAudio2::CreateSourceVoice(&pSourceVoice, wf);
	assert(hr == S_OK);

	// 再生する波形データの設定
	xbuf.pAudioData = (const BYTE*)buffer;
	xbuf.Flags = XAUDIO2_END_OF_STREAM;
	xbuf.AudioBytes = mSrcWaveData.cksize;

	hr = pSourceVoice->SubmitSourceBuffer(&xbuf);
	assert(hr == S_OK);

	lastSamplePlayed = 0;
	lastSampleStopped = 0;
	memcpy(&wfex, wf, sizeof(WAVEFORMATEX));

	// ヘッダ用メモリを解放
	delete[] wf;

	// WAVを閉じる
	mmioClose(hSrc, 0);
}

sanSound::~sanSound()
{
	pSourceVoice->DestroyVoice();
	if (xbuf.pAudioData)delete[] xbuf.pAudioData;
}

bool sanSound::play(bool start)
{
	if (pSourceVoice)
	{
		HRESULT hr;
		if (start)
		{
			hr = pSourceVoice->Stop(0, 0);
			hr = pSourceVoice->Discontinuity();
			hr = pSourceVoice->FlushSourceBuffers();
			hr = pSourceVoice->SubmitSourceBuffer(&xbuf);
		}
		XAUDIO2_VOICE_STATE stat;
		pSourceVoice->GetState(&stat);
		lastSamplePlayed = stat.SamplesPlayed;

		hr = pSourceVoice->Start(0, 0);
		return hr == S_OK;
	}
	return false;
}

bool sanSound::stop()
{
	if (pSourceVoice)
	{
		HRESULT hr = pSourceVoice->Stop(0, 0);

		XAUDIO2_VOICE_STATE stat;
		pSourceVoice->GetState(&stat);
		lastSampleStopped = stat.SamplesPlayed;

		return hr == S_OK;
	}
	return false;
}

bool sanSound::isPlaying(void)
{
	if (pSourceVoice)
	{
		XAUDIO2_VOICE_STATE stat;
		pSourceVoice->GetState(&stat);
		return stat.SamplesPlayed > 0 && stat.SamplesPlayed != lastSampleStopped;
	}
	return false;
}

bool sanSound::isStopped()
{
	return !isPlaying;
}

bool sanSound::setVolume(float volume)
{
	if (pSourceVoice)
	{
		if (volume > XAUDIO2_MAX_VOLUME_LEVEL)
		{
			volume = XAUDIO2_MAX_VOLUME_LEVEL;
		}
		else if (volume < 0.0f)
		{
			volume = 0.0f;
		}
		HRESULT hr = pSourceVoice->SetVolume(volume);
		return hr == S_OK;
	}
	return false;
}

bool sanSound::setFrequencyRatio(float frequency)
{
	if (pSourceVoice)
	{
		HRESULT hr = pSourceVoice->SetFrequencyRatio(frequency);
		return hr == S_OK;
	}
	return false;
}

float sanSound::getVolume()
{
	float ret = 0.0f;
	if (pSourceVoice)
	{
		pSourceVoice->GetVolume(&ret);
	}
	return ret;
}

float sanSound::getFrequencyRatio()
{
	if (pSourceVoice)
	{
		float ret = 0.0f;
		pSourceVoice->GetFrequencyRatio(&ret);
		return ret;
	}
	return 0.0f;
}

float sanSound::getPlayingTime()
{
	if (pSourceVoice)
	{
		return convertSecond(xbuf.AudioBytes, wfex.nSamplesPerSec, wfex.wBitsPerSample, wfex.nChannels);
	}
	return 0.0f;
}

float sanSound::getCurrentPlayingPosition()
{
	if (pSourceVoice)
	{
		XAUDIO2_VOICE_STATE stat;
		pSourceVoice->GetState(&stat);
		UINT64 data = stat.SamplesPlayed >= lastSamplePlayed ? (stat.SamplesPlayed - lastSamplePlayed) : 0;
		return (float)data / (float)(wfex.nSamplesPerSec);
	}
	return 0.0f;
}

float sanSound::convertSecond(UINT64 data, int samplesPerSec, int bitsPerSample, int channels)
{
	return (float)data / (float)(samplesPerSec * (bitsPerSample / 8) * channels);
}