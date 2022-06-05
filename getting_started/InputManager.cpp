#include "InputManager.h"

void InputManager::RegisterKeyInputAction(SDL_Keycode keyCode, InputAction action) {
    m_keyInputActionMap[keyCode] = action;
}

void InputManager::RegisterInputAction(Uint32 eventType, std::function<void(SDL_Event)> action) {
    m_inputActionMap[eventType] = action;
}

void InputManager::ProcessInput(SDL_Event& inputEvent) {
    if (inputEvent.type == SDL_KEYDOWN) {
        std::unordered_map<SDL_Keycode, InputAction>::iterator it = m_keyInputActionMap.find(inputEvent.key.keysym.sym);
        if (it != m_keyInputActionMap.end()) {
            it->second(inputEvent);
        }
        return;
    }

    std::unordered_map<Uint32, InputAction>::iterator it = m_inputActionMap.find(inputEvent.type);    
    if (it != m_inputActionMap.end()) {
        it->second(inputEvent);
        return;
    }
}
