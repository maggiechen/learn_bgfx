#pragma once
#include "TimerTicker.h"
#include <bx/math.h>   // this doesn't like being included in cpp files. Complains about some BX_CONFIG_DEBUG macro not being defined
class CameraNavigation {
private:
    static float s_cameraSpeed;
    static float s_mouseSensitivity;
    static float s_zoomSensitivity;
    static bx::Vec3 s_focus;
    static bx::Vec3 s_eyePos;
    static bx::Vec3 s_up;
    static bx::Vec3 s_lookDirection;
    static float s_yaw;
    static float s_pitch;
    static bool s_ctrlPressed;

    static void UpdateLookDirection();
    static float GetFocalDistance();

    // Inverted QuickView
    static void SetBottomView();
    static void SetBackView();
    static void SetLeftView();
public:
    // CALLBACKS - Register these to the input manager
    static void MoveForward();
    static void MoveBackward();
    static void MoveLeft();
    static void MoveRight();
    static void Pan(int deltaX, int deltaY);
    static void Zoom(int zoomAmount);
    // QuickView
    static void SetTopView(); // down from Y axis
    static void SetFrontView(); // forward from Z axis
    static void SetRightView(); // right from X axis
    static void InvertQuickView();
    static void DoNotInvertQuickView();

    /// @brief Get the current world-space position of the camera
    static bx::Vec3& GetEyePos();

    /// @brief Get the current world-space look vector of the camera
    static bx::Vec3& GetLookDirection();

    /// @brief Change the speed of the camera when using WASD controls
    static void SetCameraSpeed(float newSpeed);

    /// @brief Change the speed at which the player turns when moving the mouse
    static void SetMouseSensitivity(float newSensitivity);

    /// @brief Change the speed at which the camera zooms when scrolling the mouse
    static void SetZoomSensitivity(float newSensitivity);
};