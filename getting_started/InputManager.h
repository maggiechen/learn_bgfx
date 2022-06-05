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
class InputManager {
private:
    std::unordered_map<Uint32, InputAction> m_inputActionMap;
    std::unordered_map<SDL_Keycode, InputAction> m_keyInputActionMap;
public:
    /// @brief When this key is pressed, call this action
    void RegisterKeyInputAction(SDL_Keycode keyCode, InputAction action);

    /// @brief When this event (e.g. quit event) occurs, call this action
    void RegisterInputAction(Uint32 eventType, InputAction action);

    /// @brief Process this action by invoking any actions registered to it. If keys were registered and the event is SDL_KEYDOWN, then the corresponding registered key action, if it exists, will be invoked. 
    void ProcessInput(SDL_Event& inputEvent);
};
