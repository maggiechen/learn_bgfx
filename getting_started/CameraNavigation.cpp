#include "CameraNavigation.h"
#include "Utils.h"

float CameraNavigation::s_cameraSpeed = 0.2f;
float CameraNavigation::s_mouseSensitivity = 0.1f;
float CameraNavigation::s_yaw = -90.0f;
float CameraNavigation::s_pitch = 0.0f;
bx::Vec3 CameraNavigation::s_eyePos = {0.0f, 0.0f, 10.0f};
bx::Vec3 CameraNavigation::s_up = {0.0f, 1.0f, 0.0f};
bx::Vec3 CameraNavigation::s_lookDirection = {0.0f, -1.0f, 0.0f};
bool first = true;
void CameraNavigation::MoveForward() {
    s_eyePos = bx::add(s_eyePos, bx::mul(s_lookDirection, TimerTicker::GetDeltaTime() * s_cameraSpeed));
}
void CameraNavigation::MoveBackward() {
    s_eyePos = bx::add(s_eyePos, bx::mul(s_lookDirection, -TimerTicker::GetDeltaTime() * s_cameraSpeed));
}

void CameraNavigation::MoveLeft() {
    s_eyePos = bx::add(s_eyePos, bx::mul(bx::cross(s_lookDirection, s_up), TimerTicker::GetDeltaTime() * s_cameraSpeed));
}
void CameraNavigation::MoveRight() {
    s_eyePos = bx::add(s_eyePos, bx::mul(bx::cross(s_lookDirection, s_up), -TimerTicker::GetDeltaTime() * s_cameraSpeed));
}

void CameraNavigation::ProcessMouse(int deltaX, int deltaY) {
    if (first) {
        first = false;
        deltaX = 0;
        deltaY = 0;
    }
    s_yaw += s_mouseSensitivity * deltaX;
    s_pitch += s_mouseSensitivity * deltaY;
    if (s_pitch > 89.0f) {
        s_pitch = 89.0f;
    } else if (s_pitch < -89.0f) {
        s_pitch = -89.0f;
    }
    float cosPitch = bx::cos(bx::toRad(s_pitch));
    s_lookDirection.x = bx::cos(bx::toRad(s_yaw)) * cosPitch;
    s_lookDirection.y = bx::sin(bx::toRad(s_pitch));
    s_lookDirection.z = bx::sin(bx::toRad(s_yaw)) * cosPitch;

}

bx::Vec3& CameraNavigation::GetEyePos() {
    return s_eyePos;
}

bx::Vec3& CameraNavigation::GetLookDirection() {
    return s_lookDirection;
}

void CameraNavigation::SetCameraSpeed(float newSpeed) {
    s_cameraSpeed = newSpeed;
}

void CameraNavigation::SetMouseSensitivity(float newSensitivity) {
    s_mouseSensitivity = newSensitivity;
}