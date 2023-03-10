#pragma once

class Uncopyable
{
protected://禁止している

	constexpr Uncopyable() = default;

	~Uncopyable() = default;

	Uncopyable(const Uncopyable&) = delete;

	Uncopyable& operator =(const Uncopyable&) = delete;
public:
	/*************************************************************
	* 定数
	*************************************************************/
	//最大1290、730
	#define MAXWIDTH		1290.0f
	#define MAXWIDTH_HALF	645.0f//半分
	#define MAXHEIGHT		730.0f
	#define MAXHEIGHT_HALF	365.0f//半分
};