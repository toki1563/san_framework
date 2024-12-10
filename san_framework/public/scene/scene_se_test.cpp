//--------------------------------------------------------------//
//	"san_se._test.cpp"  						    			//
//	     サウンド												//
//													2024/11/25	//
//														sanda	//
//--------------------------------------------------------------//
#include "../../framework.h"
#include "../../framework/san_environment.h"

#define FILE_PATH_MAX (256)

WCHAR seFile[][FILE_PATH_MAX] = 
{
	L"data/sound/decision1.wav",
	L"data/sound/decision2.wav",
	L"data/sound/decision3.wav",
	L"data/sound/decision7.wav",
	L"data/sound/decision17.wav",
	L"data/sound/decision21.wav",
	L"data/sound/decision22.wav",
	L"data/sound/decision23.wav",
	L"data/sound/cursor1.wav",
	L"data/sound/cursor2.wav",
	L"data/sound/cursor3.wav",
	L"data/sound/cursor4.wav",
	L"data/sound/cursor10.wav",
	L"data/sound/cancel1.wav",
	L"data/sound/Aqua_Blue.wav",
	L"data/sound/心の中の小さな宇宙_2.wav",
	L"data/sound/朧月夜.wav",
	L"data/sound/yattaze_1.wav",
};

// 初期化関数
bool SceneSeTest::initialize()
{
	fileNum = sizeof(seFile) / (sizeof(WCHAR)*FILE_PATH_MAX);

	pSound = new sanSound*[fileNum];
	for (int i = 0; i < fileNum; i++)
	{
		pSound[i] = new sanSound(seFile[i]);
	}
	return true;
}

// 終了関数
void SceneSeTest::terminate()
{
	if (pSound != NULL)
	{
		for (int i = 0; i < fileNum; i++)
		{
			if(pSound[i] == NULL) continue;
			delete pSound[i];
			pSound[i] = NULL;
		}
		delete[] pSound;
		pSound = NULL;
	}
}

// 処理関数
void SceneSeTest::execute()
{
	if (sanMouse::trg())
	{
		int i = (sanMouse::getY() - 100) / 32;
		if (i >= 0 && i < fileNum)
		{
			if (sanMouse::getX() > 400 && sanMouse::getX() < 450)
			{
				if (pSound[i]->isPlaying() == false)
				{
					pSound[i]->play();
				}
				else
				{
					pSound[i]->stop();
				}
			}
			else if (sanMouse::getX() > 450 && sanMouse::getX() < 500)
			{
				pSound[i]->stop();
			}
			else if (sanMouse::getX() > 660 && sanMouse::getX() < 680)
			{
				pSound[i]->setVolume(pSound[i]->getVolume() - 0.1f);
			}
			else if (sanMouse::getX() > 800 && sanMouse::getX() < 820)
			{
				pSound[i]->setVolume(pSound[i]->getVolume() + 0.1f);
			}
			else if (sanMouse::getX() > 960 && sanMouse::getX() < 980)
			{
				pSound[i]->setFrequencyRatio(pSound[i]->getFrequencyRatio() - 0.1f);
			}
			else if (sanMouse::getX() > 1100 && sanMouse::getX() < 1120)
			{
				pSound[i]->setFrequencyRatio(pSound[i]->getFrequencyRatio() + 0.1f);
			}
		}
	}

	sanFont::print(100.0f, 68.0f, L"File");
	sanFont::print(400.0f, 68.0f, L"Control");
	sanFont::print(700.0f, 68.0f, L"Volume");
	sanFont::print(1000.0f, 68.0f, L"FreqRatio");

	for (int i = 0; i < fileNum; i++)
	{
		float y = 100.0f + 32.0f * (float)i;

		sanFont::setColor(pSound[i]->isPlaying() ? 0xffffff00 : 0xffffffff);

		WCHAR* extension = wcsrchr(seFile[i], L'/');
		sanFont::print(100.0f, y, L"%s", extension != NULL ? ++extension : seFile[i]);

		sanFont::print(400.0f, y, L"Play");
		sanFont::print(450.0f, y, L"Stop");

		sanFont::print(660.0f, y, L"－");
		sanFont::print(700.0f, y, L"%.3f", pSound[i]->getVolume());
		sanFont::print(800.0f, y, L"＋");

		sanFont::print(960.0f, y, L"－");
		sanFont::print(1000.0f, y, L"%.3f", pSound[i]->getFrequencyRatio());
		sanFont::print(1100.0f, y, L"＋");
	}
	sanScene::execute();
}

// 描画関数
void SceneSeTest::render()
{
	sanScene::render();
}