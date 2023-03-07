#pragma once

class Uncopyable
{
protected://ã÷é~ÇµÇƒÇ¢ÇÈ

	constexpr Uncopyable() = default;

	~Uncopyable() = default;

	Uncopyable(const Uncopyable&) = delete;

	Uncopyable& operator =(const Uncopyable&) = delete;
public:
	/*************************************************************
	* íËêî
	*************************************************************/
	//ç≈ëÂ1290ÅA730
	#define MAXWIDTH		1290.0f
	#define MAXWIDTH_HALF	645.0f//îºï™
	#define MAXHEIGHT		730.0f
	#define MAXHEIGHT_HALF	365.0f//îºï™
};