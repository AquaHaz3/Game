#pragma once

#include <raylib.h>
#include <map>
#include <string>

class SoundLoader
{

public:

	static void InitSounds();
	static Sound getSound(std::string name);
	
private:

	static std::map<std::string, Sound> sounds;

};

