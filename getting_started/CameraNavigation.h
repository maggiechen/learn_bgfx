#pragma once
#include "TimerTicker.h"
#include <bx/math.h>   // this doesn't like being included in cpp files. Complains about some BX_CONFIG_DEBUG macro not being defined
class CameraNavigation {
private:
    static float s_cameraSpeed;
    static bx::Vec3 s_eyePos;
    static bx::Vec3 s_forward;
    static bx::Vec3 s_up;
public:
    // CALLBACKS - Register these to the input manager
    static void MoveForward();
    static void MoveBackward();
    static void MoveLeft();
    static void MoveRight();
    static void ProcessMouse();

    /// @brief Get the current world-space position of the camera
    static bx::Vec3 GetEyePos();

    /// @brief Change the speed of the camera when using WASD controls
    static void SetCameraSpeed(float newSpeed);
};