#pragma once

//�|���S�����_�\����(�X�L�����t��)
struct stSkinVertex3D
{
	float x, y, z;		//���W
	float nx, ny, nz;	//�@���x�N�g��
	float u, v;			//uv
	UINT16	boneID[2];  //�ό`���󂯂�{�[���ԍ�
	float	boneWeight[2]; // �{�[���E�F�C�g(�ό`�̘c��)
};

/*
�{�[���̉e����
Born Infuence

�� : ���G�ȕό`���\
�� : �v�Z�ʁA�������g�p�ʂ����Ȃ��ł���

��ʓI�Ȑ� : 4
*/

//�L���[�u�N���X
class cSkinnedCube
{
public:
	//�{�[�����
	struct stBone
	{
		stBone* pParent;	//�e
		XMVECTOR	Pos;	//�ʒu
		XMVECTOR	Rot;	//��]
		XMVECTOR	Scl;	//�g��
		XMMATRIX	Local;	//���[�J���}�g���N�X
		XMMATRIX	World;	//���[���h�}�g���N�X
		
		XMMATRIX	iBind;	//�o�C���h�t�s��
		XMMATRIX	SkinMtx;//�X�L�j���O�ϊ��}�g���N�X

		XMVECTOR	BindPos;	//�����ʒu
		XMVECTOR	BindRot;	//������]
		XMVECTOR	BindScl;	//�����g��

	};
	stBone bones[2];	//�{�[���z��

	int BoneNum;		//�{�[����

private:
	//���_��
	int VertexNum;
	//���_�f�[�^
	stCubeVertex3D	*vtx;

	//���_�f�[�^(�X�L�����t��)
	stSkinVertex3D	*vtxS;

	//�R���X�^���g�o�b�t�@�\����
	struct stConstantBuffer
	{
		XMFLOAT4X4	WVP;		//World*View*Proj
		XMFLOAT4X4	World;		//���[���h�}�g���N�X
		XMFLOAT4	LightDir;	//���s������(�t)����
		XMFLOAT4	LightCol;	//���s�����̐F
		XMFLOAT4	LightAmb;	//�����̐F
	};

	//���_�o�b�t�@
	ID3D12Resource* vertBuff;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	//�e�N�X�`��
	ID3D12Resource* texbuff;
	//�萔�o�b�t�@
	ID3D12Resource* constBuff;
	ID3D12DescriptorHeap* basicDescHeap;
	stConstantBuffer* pConstBuffer;

	//�ړ��l
	XMVECTOR	Position;

	//��]�l
	XMVECTOR	Rotation;

	//�g��l
	XMVECTOR	Scale;

	//�p���ό`�p�}�g���N�X
	XMMATRIX trans;
	XMMATRIX rotate;
	XMMATRIX scale;
	XMMATRIX world;

	//�ÓI���ʃf�[�^
	static int						initCommon;	//�������p
	static D3D12_INPUT_ELEMENT_DESC inputElementDescs[];	//���_�v�f
	static ID3D12PipelineState* pPipelineState;	//�p�C�v���C���X�e�[�g

public:
	//�ÓI���ʃf�[�^������
	static bool initializeCommon();

	//�ÓI���ʃf�[�^�폜
	static void terminateCommon();

	//�R���X�g���N�^
	cSkinnedCube();
	//�f�X�g���N�^
	~cSkinnedCube();
	
	//����
	void execute();
	//�`��
	void render();

	//�ړ��l�̐ݒ�
	void setPositionX(float value);
	void setPositionY(float value);
	void setPositionZ(float value);
	void addPositionX(float value);
	void addPositionY(float value);
	void addPositionZ(float value);

	//�ړ��l�̎擾
	float getPositionX(void);
	float getPositionY(void);
	float getPositionZ(void);


	//��]�l�̐ݒ�
	void setRotationX(float radian);
	void setRotationY(float radian);
	void setRotationZ(float radian);
	void addRotationX(float radian);
	void addRotationY(float radian);
	void addRotationZ(float radian);

	//��]�l�̎擾
	float getRotationX(void);
	float getRotationY(void);
	float getRotationZ(void);


	//�g��l�̐ݒ�
	void setScaleX(float value);
	void setScaleY(float value);
	void setScaleZ(float value);
	void addScaleX(float value);
	void addScaleY(float value);
	void addScaleZ(float value);

	//�g��l�̎擾
	float getScaleX(void);
	float getScaleY(void);
	float getScaleZ(void);


	//���[���h�s��̎擾
	XMMATRIX *getWorld();

};
