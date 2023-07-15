#ifndef _I_RENDER_PASS_
#define _I_RENDER_PASS_

#include <functional>

#include "ShaderProgram.h"

class RenderPass
{
public:
    RenderPass(const std::function<void(ShaderProgram&)>& Draw, const ShaderProgram& shader);

    void Draw();

    virtual ~RenderPass();

protected:

    virtual void PreDraw() = 0;
    virtual void PostDraw() = 0;

    ShaderProgram shader_;
    std::function<void(ShaderProgram&)> drawFunc_;
};

#endif