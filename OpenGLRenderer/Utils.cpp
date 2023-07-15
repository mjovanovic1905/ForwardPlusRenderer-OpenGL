#include "Utils.h"
#include <fstream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

bool InitLibraries()
{
    return glfwInit() == GLFW_TRUE;
}

void ReleaseLibraryData()
{
    glfwTerminate();
}

std::string ReadFile(std::string_view path) {
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