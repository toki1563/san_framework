//--------------------------------------------------------------//
//	"san_scene.cpp"												//
//	     シーンクラス												//
//													2024/11/20	//
//														sanda	//
//--------------------------------------------------------------//
#include "../../framework.h"
#include "../../framework/san_environment.h"

bool sanScene::UpdateRenderPriority = false;

// コンストラクタ
sanScene::sanScene()
{
	for (int i = 0; i < sanOBJECT2D_MAX; i++)
	{
		pObject2D_Array[i] = NULL;
	}
	for (int i = 0; i < sanOBJECT3D_MAX; i++)
	{
		pObject3D_Array[i] = NULL;
	}
	UpdateRenderPriority = false;
}

// デストラクタ
sanScene::~sanScene()
{
	for (int i = 0; i < sanOBJECT3D_MAX; i++)
	{
		if (pObject3D_Array[i] == NULL) continue;
		delete pObject3D_Array[i];
		pObject3D_Array[i] = NULL;
	}
	for (int i = 0; i < sanOBJECT2D_MAX; i++)
	{
		if (pObject2D_Array[i] == NULL) continue;
		delete pObject2D_Array[i];
		pObject2D_Array[i] = NULL;
	}
	UpdateRenderPriority = false;
}

// 処理関数
void sanScene::execute()
{
	// 3Dオブジェクトのexecuteを実行
	for (int i = 0; i < sanOBJECT3D_MAX; i++)
	{
		if (pObject3D_Array[i] == NULL) continue;
		if (pObject3D_Array[i]->isExcuteEnable() == false) continue;
		pObject3D_Array[i]->execute();
	}
	// 2Dオブジェクトのexecuteを実行
	for (int i = 0; i < sanOBJECT2D_MAX; i++)
	{
		if (pObject2D_Array[i] == NULL) continue;
		if (pObject2D_Array[i]->isExecuteEnable() == false) continue;
		pObject2D_Array[i]->execute();
	}
}

// 描画関数
void sanScene::render()
{
	// 2Dオブジェクトのrenderを実行
	for (int i = 0; i < sanOBJECT3D_MAX; i++)
	{
		if (pObject3D_Array[i] == NULL) continue;
		if (pObject3D_Array[i]->isRenderEnable() == false) continue;
		pObject3D_Array[i]->render();
	} 

	if (UpdateRenderPriority == true) // ソートの必要がある時のみソートする
	{
		// 描画優先順位に従って配列をソート
		for (int i = 0; i < sanOBJECT2D_MAX - 1; i++)
		{
			if (pObject2D_Array[i] == NULL)continue;
			for (int j = i + 1; j < sanOBJECT2D_MAX; j++)
			{
				if (pObject2D_Array[j] == NULL)continue;
				if (pObject2D_Array[i]->getRenderPriority() > pObject2D_Array[j]->getRenderPriority())
				{
					sanSprite* tmp = pObject2D_Array[i];
					pObject2D_Array[i] = pObject2D_Array[j];
					pObject2D_Array[j] = tmp;
				}
			}
		}
		UpdateRenderPriority = false; // ソートが完了したら、次にソートが必要になるまでソートしないようにする
	}

	// 2Dオブジェクトのrenderを実行
	for (int i = 0; i < sanOBJECT2D_MAX; i++)
	{
		if (pObject2D_Array[i] == NULL) continue;
		if (pObject2D_Array[i]->isRenderEnable() == false) continue;
		pObject2D_Array[i]->render();
	}
}

// 登録された2Dオブジェクトの描画優先順位が変更されていたので、配列をソートをする必要がある
void sanScene::setUpdateRenderPriority()
{
	UpdateRenderPriority = true;
}

// オブジェクトの登録(配列の空きに入れる)
bool sanScene::registerObject(sanSprite *p)
{
	if (p == NULL)
	{
		return false;
	}

	for (int i = 0; i < sanOBJECT2D_MAX; i++)
	{
		if (pObject2D_Array[i] != NULL) continue;
		pObject2D_Array[i] = p;
		return true;
	}
	return false;
}

// オブジェクトの破棄(配列から削除/オブジェクト自体をdeleteする)
void sanScene::deleteObject(sanSprite *p)
{
	if (p == NULL)
	{
		return;
	}

	for (int i = 0; i < sanOBJECT2D_MAX; i++)
	{
		if (pObject2D_Array[i] == NULL)continue;
		if (pObject2D_Array[i] == p)
		{
			delete pObject2D_Array[i];
			pObject2D_Array[i] = NULL;
		}
	}
}

// オブジェクトの登録(配列の空きに入れる)
bool sanScene::registerObject(sanObject* p)
{
	if (p == NULL)
	{
		return false;
	}

	for (int i = 0; i < sanOBJECT3D_MAX; i++)
	{
		if (pObject3D_Array[i] != NULL)continue;
		pObject3D_Array[i] = p;
		return true;
	}
	return false;
}

// オブジェクトの破棄(配列から削除/オブジェクト自体をdeleteする)
void sanScene::deleteObject(sanObject* p)
{
	if (p == NULL)
	{
		return;
	}

	for (int i = 0; i < sanOBJECT3D_MAX; i++)
	{
		if (pObject3D_Array[i] == NULL)continue;
		if (pObject3D_Array[i] == p)
		{
			delete pObject3D_Array[i];
			pObject3D_Array[i] = NULL;
		}
	}
}