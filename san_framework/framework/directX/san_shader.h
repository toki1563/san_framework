//--------------------------------------------------------------//
//	"san_shader.h"												//
//	     シェーダー管理											//
//													2024/11/07	//
//														sanda	//
//--------------------------------------------------------------//
#pragma once

class sanShader
{
public:

	enum class eVertexShader
	{
		VS_3D,
		VS_3D_NoLight,
		VS_3D_Line,
		VS_3D_Cube,
		VS_2D,
		VS_MAX,
	};

	enum class ePixelShader
	{
		PS_3D,
		PS_3D_NoLight,
		PS_3D_Line,
		PS_3D_Cube,
		PS_2D,
		PS_MAX,
	};

	class sanShaderObject
	{
	protected:
		const BYTE* code;
		long length;
	public:
		sanShaderObject(const WCHAR* path);
		~sanShaderObject();

		const BYTE* getCode() { return code; }
		long getLength() { return length; }
	};

private:
	static sanShaderObject** vshader;
	static sanShaderObject** pshader;

public:
	static bool initialize();
	static void terminate();

	static sanShaderObject* getVShader(eVertexShader vs) { return vshader[(unsigned int)vs]; }
	static sanShaderObject* getPShader(ePixelShader ps) { return pshader[(unsigned int)ps]; }
};