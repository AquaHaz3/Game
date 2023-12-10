#pragma once

#include <iostream>
#include <map>
#include <string>
#include <raylib.h>


class SoundUI {
	static std::map<std::string, std::string> Sounds;
public:
	static void AddSounds(std::string Name, std::string FilePath) {
		std::map<std::string, std::string>::iterator it;
		for (it = Sounds.begin(); it != Sounds.end(); it++) {
			if (it->first == Name) {
				/*throw "The sound with this name is already exist";*/
				it->second = FilePath;
				return;
			}
		}
		Sounds.insert(std::pair < std::string, std::string>(Name, FilePath));
	}
	static void InitSounds() {
		AddSounds("walk", "../../resources/sounds/footsteps.wav");
	}
	static Music GetSound(std::string Name) {
		Music ToPlay;
		std::map<std::string, std::string>::iterator it; 
		for (it = Sounds.begin(); it != Sounds.end(); it++) {
			if (it->first == Name) {
				ToPlay = LoadMusicStream((it->second).c_str());
				PlayMusicStream(ToPlay);
				return ToPlay;
			}
		}
		/*if (it == Sounds.end() && it->first != Name) {
			throw "The sound with this name is not exist";
		}*/
	}
	static void Play(std::string Name) {
		Music played;
		/*try {*/
			played = GetSound(Sounds[Name]);
		/*}*/
		/*catch(std::string err){
			std::cout << err << std::endl;
		}*/
		UpdateMusicStream(played);
		PlayMusicStream(played);
	}
};
