namespace Engine
{
	static constexpr bool DEBUG_MODE_ENABLED = true;
	static constexpr int WINDOW_WIDTH = 2048;
	static constexpr int WINDOW_HEIGHT = 1536;
}

namespace Graphics
{
	static constexpr int CSM_SHADOW_RES = 2048;
	static constexpr bool MSAA_ENABLED = true;
	static constexpr int MSAA_SAMPLES = 4;

	namespace ForwardPlus
	{
		static constexpr int LIGHTS_PER_TILE = 1024;
		static constexpr int TILE_SIZE = 16;
	}
}