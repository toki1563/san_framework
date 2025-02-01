#include "../../framework.h"
#include "../../framework/san_environment.h"

#include "character.h"

#define OUTPUT_BONE_DATA (1)

cCharacter::cCharacter(const WCHAR* folder, const WCHAR* boneFile)
{
	//階層構造ファイルのパス(フォルダ名とファイル名を結合)
	WCHAR path[256];
	swprintf_s(path, L"%s%s", folder, boneFile);

	//階層構造ファイルの読み込み
	FILE* fp = NULL;
	if (_wfopen_s(&fp, path, L"rb") != 0)
	{
		//ファイルが開けなかった
		assert(false);
	}

	//ファイルサイズの取得
	long size = 0;
	fseek(fp, 0, SEEK_END);	//ファイルポインタを最後尾に移動
	size = ftell(fp);		//現在のファイルポインタ位置を取得(Byte)
	fseek(fp, 0, SEEK_SET);	//ファイルポインタを先頭に移動

	//ファイルサイズ分のメモリを確保
	pBoneData = (sanModel_BoneData*)new BYTE[size];

	//ファイルの中身を全て取得
	fread(pBoneData, size, 1, fp);

	//ファイルを閉じる
	fclose(fp);



	//ファイル内に格納されたパーツ数を計算
	PartsNum = size / sizeof(sanModel_BoneData);

	//パーツの数だけオブジェクトを作成するためのオブジェクトポインタ配列を作成
	pParts = new sanObject * [PartsNum];

	//vnModel_BoneData構造体配列を読み取りながらパーツの作成＆階層の構築
	for (int i = 0; i < PartsNum; i++)
	{
		WCHAR partsname[256], partsfile[256];
		size_t ret;
		mbstowcs_s(&ret, partsname, 256, pBoneData[i].Name, strlen(pBoneData[i].Name));	//パーツ名をWCHARに変換
		swprintf_s(partsfile, L"%s.vnm", partsname);	//".vnm"を付けてモデルファイル名にする
		swprintf_s(path, L"%s%s", folder, partsfile);	//ファイルの存在を確認するためパスを作成

#if OUTPUT_BONE_DATA
		sanFont::output(L"Parts[%d].Name : %s\n", i, partsname);
#endif

		//パーツ名のvnmファイルが存在するか調べる(fopen()が失敗すればファイルがない)
		if ((_wfopen_s(&fp, path, L"rb")) != 0)
		{	//vnmファイルが存在しないパーツはvnObjectクラスで作成
			pParts[i] = new sanObject();
		}
		else
		{	//vnmファイルが存在しないパーツはvnModelクラスで作成
			sanModel* pModel = new sanModel(folder, partsfile);
			pParts[i] = pModel;
		}

		//親子関係の設定
		if (pBoneData[i].ParentID == -1)
		{	//※parentID == -1 はcCharacterオブジェクト自身を親に設定
			pParts[i]->setParent(this);
		}
		else
		{
			pParts[i]->setParent(pParts[pBoneData[i].ParentID]);
		}

		//pos, rot, sclの設定は後のbindPose()関数で行うのでここでは省略する
	}

	//バインドポーズにする(パーツの姿勢の初期化)
	bindPose();

	pMotion = NULL;
}

cCharacter::~cCharacter()
{
	/*パーツオブジェクトのdeleteは作成したシーン側で対応*/

	//ボーンデータの削除
	if (pBoneData != NULL)
	{
		delete[] pBoneData;
		pBoneData = NULL;
	}

}

void cCharacter::execute()
{
	if (pMotion == NULL)return;

	//時間経過
	time += 1.0f;

	//アニメーションのループ
	if (time >= pMotion->Length)
	{
		time = 0.0f;
	}
	else if (time < 0.0f)
	{
		time = pMotion->Length;
	}

	//今の時間での値を計算
	float value = 0.0f;

	//チャンネル構造体配列の先頭アドレス
	sanMotionData_Channel* channel = (sanMotionData_Channel*)(((BYTE*)pMotion) + pMotion->ChannelAccess);

	//キーフレーム構造体配列の先頭アドレス
	sanMotionData_KeyFrame* key = (sanMotionData_KeyFrame*)(((BYTE*)pMotion) + pMotion->KeyFrameAccess);

	for (int i = 0; i < (int)pMotion->ChannelNum; i++)	//vnMotionData_Channel
	{
		//各チャンネルにおけるキーフレーム配列の最初
		sanMotionData_KeyFrame* k = key + channel[i].StartIndex;

		//モーション対象のパーツ
		sanObject* pObj = getParts(channel[i].Name);

		//エラーチェック
		if (!k || !pObj)continue;

		for (int j = 1; j < channel[i].KeyFrameNum; j++)	//vnMotionData_KeyFrame
		{
			if (time > k[j].Time)continue;

			float rate = (time - k[j - 1].Time) / (k[j].Time - k[j - 1].Time);

			value = (k[j].Value - k[j - 1].Value) * rate + k[j - 1].Value;

			switch (channel[i].ChannelID)
			{
			case eMotionChannel::PosX:
				pObj->setPositionX(value);
				break;
			case eMotionChannel::PosY:
				pObj->setPositionY(value);
				break;
			case eMotionChannel::PosZ:
				pObj->setPositionZ(value);
				break;
			case eMotionChannel::RotX:
				pObj->setRotationX(value);
				break;
			case eMotionChannel::RotY:
				pObj->setRotationY(value);
				break;
			case eMotionChannel::RotZ:
				pObj->setRotationZ(value);
				break;
			case eMotionChannel::SclX:
				pObj->setScaleX(value);
				break;
			case eMotionChannel::SclY:
				pObj->setScaleY(value);
				break;
			case eMotionChannel::SclZ:
				pObj->setScaleZ(value);
				break;
			}

			break;
		}
	}
}

//バインドポーズ(キャラクター標準状態)に戻す
void cCharacter::bindPose()
{
	for (int i = 0; i < PartsNum; i++)
	{
		pParts[i]->setPosition(pBoneData[i].pos[0], pBoneData[i].pos[1], pBoneData[i].pos[2]);
		pParts[i]->setRotation(pBoneData[i].rot[0], pBoneData[i].rot[1], pBoneData[i].rot[2]);
		pParts[i]->setScale(pBoneData[i].scl[0], pBoneData[i].scl[1], pBoneData[i].scl[2]);
	}
}

//再生するモーションを設定(NULLで未適用に戻す)
void cCharacter::setMotion(sanMotionData* p)
{
	//同じモーションが来たら無視する
	if (pMotion == p)return;

	//モーションデータのポインタを代入
	pMotion = p;

	//タイムをゼロに戻す
	time = 0.0f;

	//ポーズを標準状態に戻す
	bindPose();
}

//現在のモーションを取得
sanMotionData* cCharacter::getMotion(void)
{
	return pMotion;
}

//再生位置(時間)を取得
float cCharacter::getTime(void)
{
	return time;
}


//パーツ数を取得
int cCharacter::getPartsNum(void)
{
	return PartsNum;
}

//パーツを番号で取得
sanObject* cCharacter::getParts(int i)
{
	if (i < 0 || i >= PartsNum)return NULL;
	return pParts[i];
}

//パーツを名前で取得
sanObject* cCharacter::getParts(char* name)
{
	for (int i = 0; i < PartsNum; i++)
	{
		if (strcmp(name, pBoneData[i].Name) == 0)
		{
			return pParts[i];
		}
	}
	return NULL;
}
