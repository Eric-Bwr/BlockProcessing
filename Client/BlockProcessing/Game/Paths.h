#pragma once

#include "iostream"

#include <mutex>
#include <string>

inline void print(auto s){
	static std::mutex m;
	std::lock_guard<std::mutex> lock(m);

	std::cout << s;
	std::cout << std::endl;
}

#define FILE_OPTIONS "Client.properties"

#define FONT "../Assets/Fonts/Minecraft.otf"

#define SHADER_SKYBOX "../Assets/Shader/SkyBox.glsl"
#define SHADER_CHUNK_BORDER "../Assets/Shader/ChunkBorderShader.glsl"
#define SHADER_TERRAIN "../Assets/Shader/TerrainShader.glsl"
#define SHADER_LINE "../Assets/Shader/LineShader.glsl"

#define TEXTURE_WINDOW_ICON "../Assets/Textures/Icon.png"
#define TEXTURE_BLOCKS "../Assets/Textures/Blocks/"
#define TEXTURE_SKYBOX "../Assets/Textures/Gui/SkyBox/panorama_"
#define TEXTURE_SKYBOX_ENDING "png"
#define TEXTURE_GUI "../Assets/Textures/Gui/Gui.png"
#define TEXTURE_SERVER "../Assets/Textures/Gui/Server.png"
