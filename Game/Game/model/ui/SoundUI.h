#pragma once

#include <map>
#include <string>
#include <raylib.h>

class SoundUI {
	std::map<std::string, std::string> Sounds;
public:
	void InitSounds(std::string Name, std::string FilePath) {
		std::map<std::string, std::string>::iterator it;
		for (it = Sounds.begin(); it != Sounds.end(); it++) {
			if (it->first == Name) {
				throw "The sound with this name is already exist";
			}
		}
		this->Sounds.insert(std::pair < std::string, std::string>(Name, FilePath));
		return;
	}
	Music GetSound(std::string Name) {
		Music ToPlay;
		std::map<std::string, std::string>::iterator it; 
		for (it = Sounds.begin(); it != Sounds.end(); it++) {
			if (it->first == Name) {
				ToPlay = LoadMusicStream((it->second).c_str());
				PlayMusicStream(ToPlay);
				return ToPlay;
			}
		}
		if (it == Sounds.end() && it->first != Name) {
			throw "The sound with this name is not exist";
		}
	}
};

//int main() {
//	const int width = 1080, height = 1080;
//	InitWindow(width, height, "Test");
//	InitAudioDevice();
//	SoundUI s;
//	try {
//		s.InitSounds("walk", "C:/Users/User/Downloads/footsteps.wav");
//	}
//	catch(std::string str){
//		std::cout << str << std::endl;
//		return 0;
//	}
//	Music m;
//	try {
//		m = s.Play("walk");
//	}
//	catch (std::string str) {
//		std::cout << str << std::endl;
//		return 0;
//	}
//	while (!WindowShouldClose()) {
//		if (IsKeyDown(KEY_W)) {
//			UpdateMusicStream(m);
//			PlayMusicStream(m);
//		}
//		BeginDrawing();
//		ClearBackground(RAYWHITE);
//		DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
//		EndDrawing();
//	}
//	CloseWindow();
//	CloseAudioDevice();
//	return 0;
//}