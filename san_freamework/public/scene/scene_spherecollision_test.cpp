// 球の当たり判定
#include "../../framework.h"
#include "../../framework/san_environment.h"

// 初期化関数
bool SceneSphereCollisionTest::initialize()
{
	cursor = 0;
	radius = 10.0f;
	theta = 0.0f;
	phi = 0.0f / 180.0f * 3.14f;
	value_r = 1.0f * 3.141592f / 180.0f;
	radians = 180.0f / 3.141592f;
	radiusRoll = 0.5f;
	scrollMove = 0.05f;
	gridActive = true;
	axisActive = true;

	point[0] = XMVectorSet(-2.0f, 4.0f, -2.0f, 0.0f);
	point[1] = XMVectorSet(3.0f, -4.0f, 3.0f, 0.0f);
	spherePos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	add[0] = XMVectorSet(0.1f, 0.0f, 0.0f, 0.0f);
	add[1] = XMVectorSet(0.0f, 0.1f, 0.0f, 0.0f);
	add[2] = XMVectorSet(0.0f, 0.0f, 0.1f, 0.0f);

	pPoint = new sanModel(L"data/model/primitive/", L"sphere.vnm");
	pPoint->setScale(0.1f, 0.1f, 0.1f);
	pSphere = new sanModel(L"data/model/primitive/", L"sphere.vnm");
	pHit[0] = new sanModel(L"data/model/primitive/", L"sphere.vnm");
	pHit[0]->setScale(0.1f, 0.1f, 0.1f);
	pHit[1] = new sanModel(L"data/model/primitive/", L"sphere.vnm");
	pHit[1]->setScale(0.1f, 0.1f, 0.1f);

	registerObject(pPoint);	// シーンの基底クラスへ登録
	registerObject(pHit[0]);
	registerObject(pHit[1]);
	registerObject(pSphere);

	return true;
}

// 終了関数
void SceneSphereCollisionTest::terminate()
{
	deleteObject(pSphere);
	deleteObject(pHit[1]);
	deleteObject(pHit[0]);
	deleteObject(pPoint);
}

// 処理関数
void SceneSphereCollisionTest::execute()
{

}

// 描画関数
void SceneSphereCollisionTest::render()
{

}