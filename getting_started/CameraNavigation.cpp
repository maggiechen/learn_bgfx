#include "CameraNavigation.h"
#include "Utils.h"

float CameraNavigation::s_cameraSpeed = 0.2f;
bx::Vec3 CameraNavigation::s_eyePos = {0.0f, 0.0f, 10.0f};
bx::Vec3 CameraNavigation::s_forward = {0.0f, 0.0f, -1.0f};
bx::Vec3 CameraNavigation::s_up = {0.0f, 1.0f, 0.0f};

void CameraNavigation::MoveForward() {
    s_eyePos = bx::add(s_eyePos, bx::mul(s_forward, TimerTicker::GetDeltaTime() * s_cameraSpeed));
}
void CameraNavigation::MoveBackward() {
    LOG("Bakcward");
    s_eyePos = bx::add(s_eyePos, bx::mul(s_forward, -TimerTicker::GetDeltaTime() * s_cameraSpeed));
}

void CameraNavigation::MoveLeft() {
    LOG("Left");
    s_eyePos = bx::add(s_eyePos, bx::mul(bx::cross(s_forward, s_up), TimerTicker::GetDeltaTime() * s_cameraSpeed));
}
void CameraNavigation::MoveRight() {
    LOG("Right");
    s_eyePos = bx::add(s_eyePos, bx::mul(bx::cross(s_forward, s_up), -TimerTicker::GetDeltaTime() * s_cameraSpeed));
}

void CameraNavigation::ProcessMouse() {
}

bx::Vec3 CameraNavigation::GetEyePos() {
    return s_eyePos;
}

void CameraNavigation::SetCameraSpeed(float newSpeed) {
    s_cameraSpeed = newSpeed;
}