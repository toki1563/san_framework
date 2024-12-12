﻿//--------------------------------------------------------------//
//	"san_debugDraw.h"											//
//	     デバッグ描画												//
//													2024/11/07	//
//														sanda	//
//--------------------------------------------------------------//
#pragma once

// ポリゴン頂点構造体
struct sanVertex3D_Line
{
	float x, y, z;
	DWORD color;
};

// 頂点数
#define sanDebugDraw_VertexMax	(1024*4)

class sanDebugDraw
{
protected:
	// 頂点数
	static int						 VertexNum;
	// 頂点データ
	static sanVertex3D_Line			 *vtx;
	// マトリクス
	static XMFLOAT4X4				 *VP;

	// 頂点バッファ
	static ID3D12Resource*			 vertBuff;
	static D3D12_VERTEX_BUFFER_VIEW  vertexBufferView;

	// 定数バッファ
	static ID3D12Resource*			 constBuff;
	static ID3D12DescriptorHeap*	 basicDescHeap;

	static D3D12_INPUT_ELEMENT_DESC  inputElementDescs[]; // 頂点要素
	static ID3D12PipelineState*		 pPipelineState;	  // パイプラインステート

public:
	// システム関数
	static bool initialize();
	static void terminate();
	static void render();

	static void Line(const XMVECTOR* start, const XMVECTOR* end, DWORD = 0xffffffff);
	static void Line(float start_x, float start_y, float start_z, float end_x, float end_y, float end_z, DWORD color = 0xffffffff);

	static void Grid(int num = 5, float interval = 1.0f, DWORD color = 0x7fffffff, bool gridActive = true);
	static void Axis(float length = 5.0f, bool axisAxtive = true);
};