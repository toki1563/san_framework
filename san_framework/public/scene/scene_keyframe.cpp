#include "../../framework.h"
#include "../../framework/san_environment.h"

extern stMotion motion_idle;
extern stMotion motion_walk;

extern void createCharacterMotion();

//初期化関数
bool SceneKeyframetest::initialize()
{
    createCharacterMotion();

    // キャラの作成
    pCharacter = new cCharacter();


    // シーンへの登録
    registerObject(pCharacter);

    for (int i = 0; i < cCharacter::eParts::PartsMax; i++)
    {
        registerObject(pCharacter->getParts(i));
    }

    // モーションファイルの読み込み(開く)
    FILE* fp = NULL;
    if(_wfopen_s(&fp, L"data/motion/motion_idle.mot", L"rb") != 0) // 読み込みはバイナリ型
    {
        // ファイルが開けなかった
        assert(false); // 強制終了
    }

    // ファイルサイズの取得
    long size = 0;
    fseek(fp, 0, SEEK_END); // ファイルポインタを最後尾に移動
    size = ftell(fp); // 現在のファイルポインタの位置を取得(Byte)
    fseek(fp, 0, SEEK_SET); // ファイルポインタを先頭に移動

    // ファイルサイズ分のメモリを確保
    mot_idle = (sanMotionData*)new BYTE[size];

    // ファイルの中身をすべて取得
    fread(mot_idle, size, 1, fp);

    // ファイルを閉じる
    fclose(fp);

    // データの中身
    sanFont::output(L"Lenght : %f\n", mot_idle->Length);
    sanFont::output(L"ChannelNum : %d\n", mot_idle->ChannelNum);
    sanFont::output(L"KeyFrameNum : %d\n", mot_idle->KeyFrameNum);

    sanMotionData_Channel *channel = 
        (sanMotionData_Channel*)((BYTE*)mot_idle + mot_idle->ChannelAccess);
    for (int i = 0; i < mot_idle->ChannelNum; i++)
    {
        // char文字列をWCHAR文字列に変換
        WCHAR name[256];
        size_t ret;
        mbstowcs_s(&ret, name, 256, channel[i].Name, strlen(channel[i].Name));

        sanFont::output(L"Channel[%d].Name : %s\n", i, name);
        sanFont::output(L"Channel[%d].channelID : %d\n", i, channel->ChannelID);
        sanFont::output(L"Channel[%d].KeyFrameNum : %d\n", i, channel->KeyFrameNum);
    }

    // キーフレーム
    sanMotionData_KeyFrame* key =
        (sanMotionData_KeyFrame*)((BYTE*)mot_idle + mot_idle->KeyFrameAccess);
    for (int i = 0; i < mot_idle->KeyFrameNum; i++)
    {
        sanFont::output(L"Key[%d].Time : %f\n", i, key[i].Time);
        sanFont::output(L"Key[%d].Value : %f\n", i, key[i].Value);
    }

    // モーションのポインタを渡す
    pCharacter->setMotion(mot_idle);


    radius = 10.0f; // 座標設定
    theta = 10.0f;
    phi = -10.0f;
    radiusRoll = 0.5f;
    scrollMove = 0.05f;
    gridAxisActive = true;

    scrollMove = 0.05f;

    return true;
}

//終了関数
void SceneKeyframetest::terminate()
{
    deleteObject(pCharacter);
    for (int i = 0; i < cCharacter::eParts::PartsMax; i++)
    {
        deleteObject(pCharacter->getParts(i));
    }
    delete[] mot_idle;
}

//処理関数
void SceneKeyframetest::execute()
{
    // カメラの回転
    if (sanMouse::getR() >= 0.1) // ﾏｳｽホイール値取得
    {
        radius += radiusRoll;
    }
    else if (sanMouse::getR() <= -0.1 && radius >= radiusRoll * 2)
    {
        radius -= radiusRoll;
    }
    if (sanMouse::onR() && sanMouse::getDX() >= 10) // ｽｸﾛｰﾙ
    {
        theta -= scrollMove;
    }
    if (sanMouse::onR() && sanMouse::getDX() <= -10)
    {
        theta += scrollMove;
    }
    if (sanMouse::onR() && sanMouse::getDY() <= -10)
    {
        phi += scrollMove;
    }
    if (sanMouse::onR() && sanMouse::getDY() >= 10)
    {
        phi -= scrollMove;
    }
    if (sanKeyboard::trg(DIK_L))
    {
        gridAxisActive = !gridAxisActive;
    }
    if (sanKeyboard::trg(DIK_1))
    {
        pCharacter->setMotion(mot_idle);
    }
    //if (sanKeyboard::trg(DIK_2))
    //{
    //    pCharacter->setMotion(mot_walk);
    //}



    // 仰角(Φ)の計算
    float px = cosf(theta) * cosf(phi) * radius;
    float py = sinf(phi) * radius;
    float pz = sin(theta) * cosf(phi) * radius;

    // カメラの座標
    sanCamera::setPosition(px, py, pz);

    sanFont::print(20.0f, 40.0f, L"LキーでGrid&Axis表示・非表示");
    sanDebugDraw::Grid(5, 1.0f, 2147483647UL, gridAxisActive);
    sanDebugDraw::Axis(5.0f, gridAxisActive);

    sanScene::execute();
}

void SceneKeyframetest::render()
{
    sanScene::render();
}