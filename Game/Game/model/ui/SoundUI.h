#pragma once

#include <iostream>
#include <map>
#include <string>
#include <raylib.h>


class SoundUI {

	static std::map<std::string, Sound> Sounds;

public:

	static void AddSounds(std::string Name, std::string FilePath) 
	{
		Sound sound = LoadSound(FilePath.c_str());
		Sounds.emplace(Name, sound);
	}

	static void InitSounds()
	{
		AddSounds("walk", "resources/sounds/footsteps.wav");
		AddSounds("walk_stone", "resources/sounds/stone_walk.mp3");
		AddSounds("bow", "resources/sounds/bow.mp3");
		AddSounds("potion", "resources/sounds/potion.mp3");
		AddSounds("pickup", "resources/sounds/pick.mp3");
		AddSounds("explosion_small", "resources/sounds/explosion_small.wav");
		AddSounds("chest_open", "resources/sounds/open_chest.wav");
		AddSounds("chest_drop", "resources/sounds/chest_drop.wav");
	}

	static Sound GetSound(std::string Name) {
		
		if (Sounds.count(Name) <= 0) {
			throw std::exception("The sound with this name is not exist");
		}
		return Sounds[Name];
	}

	static void Play(std::string Name)
	{
		Sound s = GetSound(Name);
		if (!IsSoundPlaying(s)) {
			PlaySound(s);
		}
	}

	static void PlayOnce(std::string Name)
	{
		PlaySound(Sounds[Name]);
	}
	
	static void Stop(std::string Name)
	{
		Sound s = GetSound(Name);
		if (IsSoundPlaying(s)) {
			StopSound(s);
		}
	}

};
