//--------------------------------------------------------------//
//	"san_shader.cpp"											//
//	     シェーダー管理											//
//													2024/11/07	//
//														sanda	//
//--------------------------------------------------------------//
#include "../../framework.h"
#include "../san_environment.h"

extern HWND hWnd;

sanShader::sanShaderObject **sanShader::vshader = NULL;
sanShader::sanShaderObject **sanShader::pshader = NULL;

bool sanShader::initialize()
{
	// 頂点シェーダーの作成
	vshader = new sanShaderObject * [(unsigned int)eVertexShader::VS_MAX];
	if (vshader)
	{
		vshader[(unsigned int)eVertexShader::VS_3D] = new sanShaderObject(L"shader/vs_3d.cso");
		vshader[(unsigned int)eVertexShader::VS_3D_NoLight] = new sanShaderObject(L"shader/vs_3d_nolight.cso");
		vshader[(unsigned int)eVertexShader::VS_3D_Line] = new sanShaderObject(L"shader/vs_3d_line.cso");
		vshader[(unsigned int)eVertexShader::VS_3D_Cube] = new sanShaderObject(L"shader/vs_3d_cube.cso");
		vshader[(unsigned int)eVertexShader::VS_2D] = new sanShaderObject(L"shader/vs_2d.cso");
	}

	// ピクセルシェーダーの作成
	pshader = new sanShaderObject * [(unsigned int)ePixelShader::PS_MAX];
	if (pshader)
	{
		pshader[(unsigned int)ePixelShader::PS_3D] = new sanShaderObject(L"shader/ps_3d.cso");
		pshader[(unsigned int)ePixelShader::PS_3D_NoLight] = new sanShaderObject(L"shader/ps_3d_nolight.cso");
		pshader[(unsigned int)ePixelShader::PS_3D_Line] = new sanShaderObject(L"shader/ps_3d_line.cso");
		pshader[(unsigned int)ePixelShader::PS_3D_Cube] = new sanShaderObject(L"shader/ps_3d_cube.cso");
		pshader[(unsigned int)ePixelShader::PS_2D] = new sanShaderObject(L"shader/ps_2d.cso");
	}
	return (vshader != NULL && pshader != NULL);
}

void sanShader::terminate()
{
	for (int i = 0; i < (unsigned int)eVertexShader::VS_MAX; i++)
	{
		delete vshader[i];
		vshader = NULL;
	}
	delete[] vshader;
	vshader = NULL;

	for (int i = 0; i < (unsigned int)ePixelShader::PS_MAX; i++)
	{
		delete pshader[i];
		pshader[i] = NULL;
	}
	delete[] pshader;
	pshader = NULL;
}

sanShader::sanShaderObject::sanShaderObject(const WCHAR* path)
{
	code = NULL;
	length = 0;

	FILE *fp = NULL;
	// シェーダーファイルをバイナリモードで開く
	if ((_wfopen_s(&fp, path, L"rb")) != 0)
	{
		WCHAR text[256];
		swprintf_s(text, L"cannot open file \"%s\"", path);
		MessageBox(hWnd, text, L"sanShader Load Error", MB_OK);
		assert(false);
		return;
	}
	// ファイルサイズを取得
	fseek(fp, 0, SEEK_END);
	length = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	// バッファを確保し、ファイルデータを読み込む
	code = new BYTE[length];
	fread((void*)code, length, 1, fp);

	fclose(fp);
}

sanShader::sanShaderObject::~sanShaderObject()
{
	if (code)delete[] code;
}