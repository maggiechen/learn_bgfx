#pragma once
#ifdef LINUX
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_syswm.h>
    #include <SDL2/SDL_keycode.h>
#else
    #include <SDL.h>
    #include <SDL_syswm.h>
    #include <SDL_stdinc.h>
    #include <SDL_keycode.h>
#endif
#include <functional>
#include <unordered_map>
using InputAction = std::function<void(SDL_Event)>;
using KeyHoldAction = std::function<void()>;
enum class KeyState {
    KeyState_Pressed,
    KeyState_Released
};

class InputManager {
private:
    std::unordered_map<Uint32, InputAction> m_inputActionMap;
    std::unordered_map<SDL_Keycode, InputAction> m_keyDownActionMap;
    std::unordered_map<SDL_Keycode, KeyHoldAction> m_keyHoldActionMap;
    std::unordered_map<SDL_Keycode, KeyState> m_keyState;
public:
    /// @brief When this key is pressed, call this action
    void RegisterKeyHoldAction(SDL_Keycode keyCode, KeyHoldAction action);

    /// @brief When this key is pressed down, call this action
    void RegisterKeyDownAction(SDL_Keycode keyCode, InputAction action);

    /// @brief When this event (e.g. quit event) occurs, call this action
    void RegisterInputAction(Uint32 eventType, InputAction action);

    /// @brief Process any input that can immediately be delegated to callbacks, and update the internal state of keys pressed. 
    void ProcessInputAndUpdateKeyState(SDL_Event& inputEvent);

    /// @brief From the current key state, fire off any keycode callbacks.
    void ProcessFromKeyState();
};
