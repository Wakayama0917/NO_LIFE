#pragma once

class Uncopyable
{
protected://�֎~���Ă���

	constexpr Uncopyable() = default;

	~Uncopyable() = default;

	Uncopyable(const Uncopyable&) = delete;

	Uncopyable& operator =(const Uncopyable&) = delete;
public:
	/*************************************************************
	* �萔
	*************************************************************/
	//�ő�1290�A730
	#define MAXWIDTH		1290.0f
	#define MAXWIDTH_HALF	645.0f//����
	#define MAXHEIGHT		730.0f
	#define MAXHEIGHT_HALF	365.0f//����
};