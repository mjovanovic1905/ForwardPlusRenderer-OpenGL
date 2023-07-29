#ifndef _CAMERA_
#define _CAMERA_

#include <glm/glm.hpp>
#include <climits>

class Window;

class Camera
{
public:
    Camera();

    static constexpr float NEAR_PLANE = 0.1f;
    static constexpr float FAR_PLANE = 2000.f; 

    void Init(glm::vec3 position);
    void ProcessInput(float deltaTime);
    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix() const;
    glm::vec3 GetPosition() const;
    float GetFOV() const { return fov_; }

private:
    static constexpr float MOVE_SPEED = 150.f;

    glm::vec3 position_;
    glm::vec3 front_;
    glm::vec3 worldUp_;
    glm::vec3 up_;
    glm::vec3 right_;
    float fov_;
};

#endif