#pragma once
class Cat
{
enum STATUS{stop=1,charge,jump,drop,dead};
public:
	STATUS Status; //è״̬
	float Power; //è��Ծ����
	float ChargeSpeed; //è�����ٶ�
	float Height = 0.0f; //è�߶�
	float V = 0.0f; //è�ٶ�
	float G = 0.005f; //è�������ٶ�
	bool Safe = true; //èƽ̨���

	Cat();
	~Cat();

	void UpdateCat(); //����è״̬

	void JumpCat(); //è��ȫ��Ծ

	void DropCat(); //è����ȫ��Ծ

	void DoCharge(); //è����

	void DoRelease(); //è����

	bool inAir(); //���è�Ƿ��ڿ���

	void Reset(); //����è

	bool isDead(); //���è�Ƿ�����
};

