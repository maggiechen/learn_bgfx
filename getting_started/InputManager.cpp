#include "InputManager.h"
#include "Utils.h"
InputManager::InputManager() {
    SDL_SetRelativeMouseMode(SDL_TRUE);
    // SDL_CaptureMouse(SDL_TRUE);
    // SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1", SDL_HINT_OVERRIDE);
}

void InputManager::RegisterKeyDownAction(SDL_Keycode keyCode, KeyDownAction action) {
    m_keyDownActionMap[keyCode] = action;
}

void InputManager::RegisterKeyHoldAction(SDL_Keycode keyCode, KeyHoldAction action) {
    m_keyHoldActionMap[keyCode] = action;
}

void InputManager::RegisterKeyUpAction(SDL_Keycode keyCode, KeyUpAction action) {
    m_keyUpActionMap[keyCode] = action;
}

void InputManager::RegisterInputAction(Uint32 eventType, std::function<void(SDL_Event)> action) {
    m_inputActionMap[eventType] = action;
}

void InputManager::RegisterMouseMove(MouseMoveAction action) {
    m_mouseMoveAction = action;
}

void InputManager::RegisterMouseScroll(MouseScrollAction action) {
    m_mouseScrollAction = action;
}

void InputManager::ProcessInputAndUpdateKeyState(SDL_Event& inputEvent) {
    if (inputEvent.type == SDL_KEYDOWN) {
        m_keyState[inputEvent.key.keysym.sym] = KeyState::KeyState_Pressed;
        std::unordered_map<SDL_Keycode, KeyDownAction>::iterator it = m_keyDownActionMap.find(inputEvent.key.keysym.sym);
        if (it != m_keyDownActionMap.end()) {
            it->second();
        }
        return;
    } else if (inputEvent.type == SDL_KEYUP) {
        std::unordered_map<SDL_Keycode, KeyUpAction>::iterator it = m_keyUpActionMap.find(inputEvent.key.keysym.sym);
        if (it != m_keyUpActionMap.end()) {
            it->second();
        }
        m_keyState.erase(inputEvent.key.keysym.sym);
        return;
    }

    if (inputEvent.type == SDL_MOUSEMOTION) {
        // Because one tick may receive multiple mouse motion events, we need to store the cumulative deltas of the mouse position
        // so that the mouse move callback will receive the correct aggregate values.
        m_deltaMouseX += inputEvent.motion.xrel;
        m_deltaMouseY += inputEvent.motion.yrel;
        return;
    }

    if (inputEvent.type == SDL_MOUSEWHEEL) {
        m_deltaScrollY = inputEvent.wheel.y;
        return;
    }

    std::unordered_map<Uint32, InputAction>::iterator it = m_inputActionMap.find(inputEvent.type);    
    if (it != m_inputActionMap.end()) {
        it->second(inputEvent);
        return;
    }
}

void InputManager::ProcessFromInputState() {
    if (m_deltaMouseX != 0 || m_deltaMouseY != 0) {
        if (m_mouseMoveAction != nullptr) {
            m_mouseMoveAction(m_deltaMouseX, m_deltaMouseY);
        }
        m_deltaMouseX = 0;
        m_deltaMouseY = 0;
    }

    if (m_deltaScrollY != 0) {
        if (m_mouseScrollAction != nullptr) {
            m_mouseScrollAction(m_deltaScrollY);
        }
        m_deltaScrollY = 0;
    }

    for (auto&& keyToState : m_keyState) {
        std::unordered_map<SDL_Keycode, KeyHoldAction>::iterator it = m_keyHoldActionMap.find(keyToState.first);
        if (it != m_keyHoldActionMap.end()) {
            it->second();
        }
    }
}