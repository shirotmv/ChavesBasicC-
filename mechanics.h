// mechanics.h
#ifndef MECHANICS_H
#define MECHANICS_H

#include <GL/glut.h>
#include <map> // Using std::map as per original

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Input Manager completamente aislado
// mechanics.h - InputManager corregido
class InputManager {
private:
    static InputManager* instance;
    std::map<unsigned char, bool> keys;
    std::map<unsigned char, bool> keysPressed; // Para detectar pulsaciones únicas
    std::map<int, bool> specialKeys;

    int mouseX, mouseY;
    int lastMouseX, lastMouseY;
    bool mouseInitialized;
    bool mouseCaptured;
    int windowCenterX, windowCenterY;

    InputManager() : mouseX(0), mouseY(0), lastMouseX(0), lastMouseY(0),
                     mouseInitialized(false), mouseCaptured(false),
                     windowCenterX(400), windowCenterY(300) {}

public:
    static InputManager* getInstance() {
        if (!instance) {
            instance = new InputManager();
        }
        return instance;
    }

    void keyDown(unsigned char key) {
        if (!keys[key]) {
            keysPressed[key] = true; // Marcar como recién presionada
        }
        keys[key] = true;
    }

    void keyUp(unsigned char key) {
        keys[key] = false;
        keysPressed[key] = false; // AÑADIDO: Resetear también en keyUp
    }

    void specialKeyDown(int key) {
        specialKeys[key] = true;
    }

    void specialKeyUp(int key) {
        specialKeys[key] = false;
    }

    bool isKeyPressed(unsigned char key) const {
        std::map<unsigned char, bool>::const_iterator it = keys.find(key);
        return it != keys.end() && it->second;
    }

    bool isKeyJustPressed(unsigned char key) {
        std::map<unsigned char, bool>::iterator it = keysPressed.find(key);
        if (it != keysPressed.end() && it->second) {
            it->second = false; // Resetear después de leer
            return true;
        }
        return false;
    }

    bool isSpecialKeyPressed(int key) const {
        std::map<int, bool>::const_iterator it = specialKeys.find(key);
        return it != specialKeys.end() && it->second;
    }

    // AÑADIDO: Método para limpiar las teclas "just pressed" al final del frame
    void clearJustPressed() {
        keysPressed.clear();
    }

    // Resto del código igual...
    void updateMouse(int x, int y) {
        if (!mouseCaptured) return;

        int viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        windowCenterX = viewport[2] / 2;
        windowCenterY = viewport[3] / 2;

        mouseX = x;
        mouseY = y;

        if (!mouseInitialized) {
            lastMouseX = windowCenterX;
            lastMouseY = windowCenterY;
            mouseInitialized = true;
        }
    }

    void getMouseDelta(int& deltaX, int& deltaY) {
        if (!mouseInitialized || !mouseCaptured) {
            deltaX = 0;
            deltaY = 0;
            return;
        }

        deltaX = mouseX - windowCenterX;
        deltaY = mouseY - windowCenterY;

        if (deltaX > 20) deltaX = 20;
        if (deltaX < -20) deltaX = -20;
        if (deltaY > 20) deltaY = 20;
        if (deltaY < -20) deltaY = -20;

        if (deltaX != 0 || deltaY != 0) {
            glutWarpPointer(windowCenterX, windowCenterY);
        }
    }

    void reset() {
        keys.clear();
        keysPressed.clear();
        specialKeys.clear();
        mouseInitialized = false;
        mouseCaptured = false;
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
    }

    void setMouseCaptured(bool captured) {
        mouseCaptured = captured;
        if (captured) {
            glutSetCursor(GLUT_CURSOR_NONE);
            int viewport[4];
            glGetIntegerv(GL_VIEWPORT, viewport);
            windowCenterX = viewport[2] / 2;
            windowCenterY = viewport[3] / 2;
            glutWarpPointer(windowCenterX, windowCenterY);
            mouseInitialized = false;
        } else {
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        }
    }

    bool isMouseCaptured() const { return mouseCaptured; }
};

// Definir la instancia estática
InputManager* InputManager::instance = NULL;

#endif // MECHANICS_H
