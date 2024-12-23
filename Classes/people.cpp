#include "people.h"

USING_NS_CC;

bool People::init()
{
	if (!Node::init())
	{
		return false;
	}
	auto Bear = Sprite::create("Bear.png");
	auto Alex = Sprite::create("Alex.png");
	auto Baby = Sprite::create("Baby.png");
	auto Emliy = Sprite::create("Emily.png");
	auto George = Sprite::create("George.png");
	auto Gunther = Sprite::create("Gunther.png");
	auto Jodi = Sprite::create("Jodi.png");
	auto Linus = Sprite::create("Linus.png");
	auto Sam = Sprite::create("Sam.png");

	return true;
}