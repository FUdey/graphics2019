#pragma once
#include <iostream>

class Block
{
public:
	float zPos = -2.0f; //ƽ̨λ��
	float Width = 1.0; //ƽ̨��С
	float V = 0.03; //ƽ̨�ƶ��ٶ�

	void SetBlock(float z, float width); //����ƽ̨ ����:λ�ã�ƽ̨��С

	void MoveBlock(); //ƽ̨λ��ˢ��

	bool isCoverd(float tolerance); //ƽ̨��� ����:�ж����ݶ�
};

