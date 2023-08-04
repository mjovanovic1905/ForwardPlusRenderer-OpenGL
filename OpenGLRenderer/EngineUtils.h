#pragma once

#include <string>
#include <string_view>

class EngineUtils
{
public:

	static constexpr bool DEBUG_MODE_ENABLED = true;
	static constexpr int WINDOW_WIDTH = 2048;
	static constexpr int WINDOW_HEIGHT = 1536;

	static constexpr int CSM_SHADOW_RES = 2048;
	static constexpr bool MSAA_ENABLED = true;
	static constexpr int MSAA_SAMPLES = 4;

	static constexpr int LIGHTS_PER_TILE = 1024;
	static constexpr int TILE_SIZE = 16;
	static constexpr int LIGHT_ID_END = -2;
	
	static constexpr bool USE_LIGHT_CULLING = true;
	static constexpr bool DRAW_DEBUG_LIGHTS = true;
	static constexpr bool DRAW_LIGHT_CULLING_DEPTH_MAP = false;

	static bool InitGLFW();
	static void ReleaseGLFW();
	static void SetupOpenGl();

	static std::string ReadFile(std::string_view path);
	static void PrintFrameTime();
};