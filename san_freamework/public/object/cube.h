#pragma once
// �L���[�u

// �|���S�����_�\����
struct stCubeVertex3D
{
	float x, y, z;    // ���W
	float nx, ny, nz; // �@���x�N�g��
	float u, v;		  // uv�Ƃ̓|���S���Ɖ摜�̈ʒu���������Ă���
};

// �L���[�u�N���X
class cCube
{
private:
	// ���_��
	int VertexNum;
	// ���_�f�[�^
	stCubeVertex3D *vtx;

	// �R���X�^���g�o�b�t�@�\����
	struct stConstantBuffer
	{
		XMFLOAT4X4 WVP;		 // World * View * Proj
		XMFLOAT4X4 World;    // World�}�g���N�X
		XMFLOAT4   LightDir; // ���s������(�t)����
		XMFLOAT4   LightCol; // ���s�����̐F
		XMFLOAT4   LightAmb; // �����̐F
	};

	// ���_�o�b�t�@
	ID3D12Resource* vertBuff;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	// �e�N�X�`��
	ID3D12Resource* texbuff;

	// �萔�o�b�t�@
	ID3D12Resource* constBuff;
	ID3D12DescriptorHeap* basicDescHeap;
	stConstantBuffer* pConstBuffer;

	// �ړ��l
	XMVECTOR Position;

	// ��]�l
	XMVECTOR Rotation;

	// �g��l
	XMVECTOR Scale;

	// �p���ό`�p�}�g���N�X
	XMMATRIX trans;
	XMMATRIX rotate;
	XMMATRIX scale;
	XMMATRIX world;

	// �ÓI���ʃf�[�^
	static int					    initCommon;			 // �������p
	static D3D12_INPUT_ELEMENT_DESC inputElementDescs[]; // ���_�v�f
	static ID3D12PipelineState*		pPipelineState;      // �p�C�v���C���X�e�[�g

public:
	// �ÓI���ʃf�[�^������
	static bool initializeCommon();

	// �ÓI���ʃf�[�^�폜
	static void terminateCommon();

	// �R���X�g���N�^
	cCube();
	// �f�X�g���N�^
	~cCube();

	// ����
	void execute();

	// �`��
	void render();

	// �ړ��l�̐ݒ�
	void setPositionX(float value);
	void setPositionY(float value);
	void setPositionZ(float value);
	void addPositionX(float value);
	void addPositionY(float value);
	void addPositionZ(float value);

	// �ړ��l�̎擾
	float getPositionX(void);
	float getPositionY(void);
	float getPositionZ(void);

	// ��]�l�̐ݒ�
	void setRotationX(float radian);
	void setRotationY(float radian);
	void setRotationZ(float radian);
	void addRotationX(float radian);
	void addRotationY(float radian);
	void addRotationZ(float radian);


	// ��]�l�̎擾
	float getRotationX(void);
	float getRotationY(void);
	float getRotationZ(void);

	// �g��l�̐ݒ�
	void setScaleX(float value);
	void setScaleY(float value);
	void setScaleZ(float value);
	void addScaleX(float value);
	void addScaleY(float value);
	void addScaleZ(float value);

	// �g��l�̎擾
	float getScaleX(void);
	float getScaleY(void);
	float getScaleZ(void);

	// ���[���h�s��̎擾
	XMMATRIX *getWorld();
};