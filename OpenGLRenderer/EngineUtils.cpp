#include "EngineUtils.h"
#include <fstream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"


bool EngineUtils::InitGLFW()
{
    return glfwInit() == GLFW_TRUE;
}

void EngineUtils::ReleaseGLFW()
{
    glfwTerminate();
}

void EngineUtils::SetupOpenGl()
{

    glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    if constexpr (EngineUtils::MSAA_ENABLED)
    {
        glEnable(GL_MULTISAMPLE);
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

std::string EngineUtils::ReadFile(std::string_view path) {
    constexpr auto read_size = std::size_t{4096};
    auto stream = std::ifstream{path.data()};
    stream.exceptions(std::ios_base::badbit);

    auto out = std::string{};
    auto buf = std::string(read_size, '\0');
    while (stream.read(& buf[0], read_size)) {
        out.append(buf, 0, stream.gcount());
    }
    out.append(buf, 0, stream.gcount());
    return out;
}