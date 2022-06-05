#include "InputManager.h"

void InputManager::RegisterKeyHoldAction(SDL_Keycode keyCode, KeyHoldAction action) {
    m_keyHoldActionMap[keyCode] = action;
}

void InputManager::RegisterKeyDownAction(SDL_Keycode keyCode, InputAction action) {
    m_keyDownActionMap[keyCode] = action;
}

void InputManager::RegisterInputAction(Uint32 eventType, std::function<void(SDL_Event)> action) {
    m_inputActionMap[eventType] = action;
}

void InputManager::ProcessInputAndUpdateKeyState(SDL_Event& inputEvent) {
    if (inputEvent.type == SDL_KEYDOWN) {
        m_keyState[inputEvent.key.keysym.sym] = KeyState::KeyState_Pressed;
        std::unordered_map<SDL_Keycode, InputAction>::iterator it = m_keyDownActionMap.find(inputEvent.key.keysym.sym);
        if (it != m_keyDownActionMap.end()) {
            it->second(inputEvent);
        }
        return;
    } else if (inputEvent.type == SDL_KEYUP) {
        m_keyState.erase(inputEvent.key.keysym.sym);
    }

    std::unordered_map<Uint32, InputAction>::iterator it = m_inputActionMap.find(inputEvent.type);    
    if (it != m_inputActionMap.end()) {
        it->second(inputEvent);
        return;
    }
}

void InputManager::ProcessFromKeyState() {
    for (auto&& keyToState : m_keyState) {
        std::unordered_map<SDL_Keycode, KeyHoldAction>::iterator it = m_keyHoldActionMap.find(keyToState.first);
        if (it != m_keyHoldActionMap.end()) {
            it->second();
        }
    }
}