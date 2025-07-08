#ifndef PLAYING_H
#define PLAYING_H

#include <GL/glut.h>
#include <cstdio> // For sprintf
#include "lightning.h"
#include "player.h" // Asumes que Player tiene la posición del jugador, si no, usa Camera
#include "world.h"
#include "mechanics.h" // For InputManager
#include "collision.h" // Include the collision system header

// Forward declaration para evitar dependencias circulares
class Game;

class Playing {
private:
    bool active;
    Lightning* lightning;
    Camera* camera; // Tu Camera es el 'player' en términos de posición para el minigame
    World* world;
    InputManager* input;
    CollisionSystem* collisionSystem;

    // Variables para controlar el estado de salida
    bool exitRequested;

    // Variables para controlar la velocidad
    bool sprintMode;     // Modo velocidad rápida (toggle con X)
    bool slowMode;       // Modo velocidad lenta (toggle con Z)

    // Callbacks estáticos para GLUT
    static Playing* instance;

    // Puntero al juego para restaurar callbacks
    static Game* gameInstance;

    static void keyboardCallback(unsigned char key, int x, int y) {
        if (instance && instance->active) {
            // DEBUG: Imprimir todos los códigos de teclas que llegan
            printf("Tecla presionada: %d (char: '%c')\n", (int)key, key);

            // Manejar ESC directamente
            if (key == 27) { // ESC
                instance->exitRequested = true;
                return;
            }

            instance->input->keyDown(key);

            // Manejar teclas especiales
            if (key == 'v' || key == 'V') {
                instance->camera->toggleView();
            }

            // Manejar toggle de velocidad rápida con X
            if (key == 'x' || key == 'X') {
                instance->sprintMode = !instance->sprintMode;
                // Si activamos sprint, desactivamos modo lento
                if (instance->sprintMode) {
                    instance->slowMode = false;
                }
                printf("Modo velocidad rápida: %s\n", instance->sprintMode ? "ON" : "OFF");
            }

            // Manejar toggle de velocidad lenta con Z
            if (key == 'z' || key == 'Z') {
                instance->slowMode = !instance->slowMode;
                // Si activamos modo lento, desactivamos sprint
                if (instance->slowMode) {
                    instance->sprintMode = false;
                }
                printf("Modo velocidad lenta: %s\n", instance->slowMode ? "ON" : "OFF");
            }

            // DEBUG: Verificar específicamente el espacio
            if (key == 32 || key == ' ') {
                printf("¡ESPACIO DETECTADO! Código: %d\n", (int)key);
                // Intentar saltar directamente aquí como prueba
                if (instance->camera && instance->camera->getIsGrounded()) {
                    printf("Intentando saltar...\n");
                    instance->camera->jump();
                } else {
                    printf("No puede saltar - Grounded: %s\n",
                                   instance->camera ? (instance->camera->getIsGrounded() ? "Si" : "No") : "No camera");
                }
            }
        }
    }

    static void specialKeyCallback(int key, int x, int y) {
        if (instance && instance->active) {
            printf("Tecla especial presionada: %d\n", key);

            if (key == 32 || key == 0) { // GLUT_KEY_SPACE no es una tecla especial estándar, 32 es el ASCII de espacio
                printf("¡ESPACIO COMO TECLA ESPECIAL! Código: %d\n", key);
                if (instance->camera && instance->camera->getIsGrounded()) {
                    printf("Saltando desde tecla especial...\n");
                    instance->camera->jump();
                }
            }

            instance->input->specialKeyDown(key);
        }
    }

    static void specialKeyUpCallback(int key, int x, int y) {
        if (instance && instance->active) {
            instance->input->specialKeyUp(key);
        }
    }

    static void keyboardUpCallback(unsigned char key, int x, int y) {
        if (instance && instance->active) {
            instance->input->keyUp(key);
        }
    }

    static void mouseMotionCallback(int x, int y) {
        if (instance && instance->active && instance->input->isMouseCaptured()) {
            instance->input->updateMouse(x, y);
        }
    }

    static void mouseCallback(int button, int state, int x, int y) {
        if (instance && instance->active) {
            if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
                instance->input->setMouseCaptured(true);
            }
        }
    }

    // COMPLETELY REWRITTEN: processInput method with proper collision handling
    void processInput(float deltaTime) {
        if (!active || !input || !camera) return;

        float baseSpeed = 8.0f;
        float speed = baseSpeed;

        // Aplicar modificadores de velocidad
        if (sprintMode) {
            speed = baseSpeed * 2.0f;
        } else if (slowMode) {
            speed = baseSpeed * 0.5f;
        }

        // Manejo del salto
        bool jumpPressed = input->isKeyJustPressed(32) || input->isKeyPressed(32);
        if (jumpPressed && camera->getIsGrounded()) {
            camera->jump();
        }

        // Calcular movimiento horizontal
        float moveX = 0.0f;
        float moveZ = 0.0f;

        if (input->isKeyPressed('w') || input->isKeyPressed('W')) {
            moveZ += speed * deltaTime;
        }
        if (input->isKeyPressed('s') || input->isKeyPressed('S')) {
            moveZ -= speed * deltaTime;
        }
        if (input->isKeyPressed('a') || input->isKeyPressed('A')) {
            moveX -= speed * deltaTime;
        }
        if (input->isKeyPressed('d') || input->isKeyPressed('D')) {
            moveX += speed * deltaTime;
        }

        // PASO 1: Aplicar movimiento básico (horizontal + física vertical)
        camera->move(moveZ, moveX, deltaTime);

        // PASO 2: Verificar y resolver colisiones
        if (collisionSystem) {
            float playerX = camera->getX();
            float playerY = camera->getY();
            float playerZ = camera->getZ();
            float playerRadius = camera->getBodyRadius(); // Asume que Camera tiene este método

            // Verificar colisiones
            CollisionResult result = collisionSystem->checkCollision(playerX, playerY, playerZ, playerRadius);
            
            if (result.hasCollision) {
                // Aplicar corrección de colisión
                camera->handleCollision(result.penetrationX, result.penetrationY, result.penetrationZ,
                                         result.normalX, result.normalY, result.normalZ);
                
                // Actualizar posición después de la corrección
                playerX = camera->getX();
                playerY = camera->getY();
                playerZ = camera->getZ();
            }

            // PASO 3: Verificar estado del suelo (importante para colisiones desde arriba)
            float highestSupportY = -1000.0f; // Inicializar muy bajo
            
            // Buscar la superficie más alta que esté justo debajo del jugador
            for (size_t i = 0; i < collisionSystem->getBoxCount(); ++i) {
                const CollisionBox& box = collisionSystem->getBox(i);
                
                // Verificar si el jugador está horizontalmente sobre este cubo
                if (playerX >= box.getMinX() && playerX <= box.getMaxX() &&
                    playerZ >= box.getMinZ() && playerZ <= box.getMaxZ()) {
                    
                    // Verificar si el jugador está cerca de la superficie superior
                    float surfaceY = box.getMaxY();
                    if (playerY >= surfaceY && playerY <= surfaceY + playerRadius + 0.5f) { // Considerar un pequeño margen
                        if (surfaceY > highestSupportY) {
                            highestSupportY = surfaceY;
                        }
                    }
                }
            }
            
            // Actualizar estado del suelo basado en la superficie más alta encontrada
            if (highestSupportY > -999.0f) { // Si se encontró una superficie de soporte válida
                camera->checkGroundStatus(highestSupportY);
            } else { // Si no se encontró ninguna superficie debajo del jugador (e.g., está en el aire o sobre un vacío)
                camera->checkGroundStatus(-1000.0f); // O un valor que indique "sin suelo"
            }
        }

        // Procesar mouse
        if (input->isMouseCaptured()) {
            int deltaX, deltaY;
            input->getMouseDelta(deltaX, deltaY);
            if (deltaX != 0 || deltaY != 0) {
                camera->rotate((float)deltaX, (float)deltaY);
            }
        }
        // Limpiar teclas "just pressed" al final del procesamiento de input
        input->clearJustPressed(); // ¡IMPORTANTE: Añade esta llamada!
    }

    void resetOpenGLState() {
        glDisable(GL_LIGHTING);
        for (int i = 0; i < 8; i++) {
            glDisable(GL_LIGHT0 + i);
        }
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glDisable(GL_COLOR_MATERIAL);
        glDisable(GL_NORMALIZE);
        glDisable(GL_BLEND);
        glColor3f(1.0f, 1.0f, 1.0f);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

public:
    Playing() : active(false), lightning(NULL), camera(NULL), world(NULL), input(NULL),
                exitRequested(false), sprintMode(false), slowMode(false),
                collisionSystem(NULL) {
        instance = this;

        lightning = new Lightning();
        camera = new Camera();
        world = new World();
        input = InputManager::getInstance();
        collisionSystem = new CollisionSystem();
    }

    ~Playing() {
        if (active) {
            setActive(false);
        }
        if (lightning) {
            delete lightning;
            lightning = NULL;
        }
        if (camera) {
            delete camera;
            camera = NULL;
        }
        if (world) {
            delete world;
            world = NULL;
        }
        if (collisionSystem) {
            delete collisionSystem;
            collisionSystem = NULL;
        }
        instance = NULL;
    }

    static void setGameInstance(Game* game) {
        gameInstance = game;
    }

    void setActive(bool isActive) {
        active = isActive;
        exitRequested = false;

        if (active) {
            glutKeyboardFunc(keyboardCallback);
            glutKeyboardUpFunc(keyboardUpCallback);
            glutSpecialFunc(specialKeyCallback);
            glutSpecialUpFunc(specialKeyUpCallback);
            glutMouseFunc(mouseCallback);
            glutPassiveMotionFunc(mouseMotionCallback);
            glutMotionFunc(mouseMotionCallback);

            glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);

            if (lightning) {
                lightning->setup();
            }

            input->reset();
            input->setMouseCaptured(true);

            if (collisionSystem) {
                collisionSystem->setupWorldCollisions();
            }

            sprintMode = false;
            slowMode = false;
        } else {
            glutSpecialFunc(NULL);
            glutSpecialUpFunc(NULL);

            if (lightning) {
                lightning->cleanup();
            }

            if (input) {
                input->reset();
                input->setMouseCaptured(false);
            }

            resetOpenGLState();
        }
    }

    void update(float deltaTime) {
        if (!active) return;
        
        // 1. Procesar la entrada y actualizar la posición del jugador/cámara
        processInput(deltaTime);

        // 2. Obtener la posición y el radio actual del jugador (desde la cámara en este caso)
        Vector3 playerPos(camera->getX(), camera->getY(), camera->getZ());
        float playerRadius = camera->getBodyRadius(); // Asegúrate de que Camera tenga este método

        // 3. Actualizar el mundo, pasándole la posición del jugador
        if (world) {
            world->update(playerPos, playerRadius); // ¡NUEVA LÍNEA CLAVE!
        }
    }

    void render() {
        if (!active) return;

        if (camera) {
            camera->setup();
        }

        if (world) {
            world->render(); // Esto ya incluye la llamada a minigame.render()
        }

        if (camera && camera->isThirdPerson()) {
            camera->drawBody();
        }

        // Restablecer el estado OpenGL para dibujar el HUD 2D
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();

        int viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        glOrtho(0, viewport[2], viewport[3], 0, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);

        // Dibujar el texto del HUD
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(10.0f, 20.0f);

        const char* speedMode = "Normal";
        if (sprintMode) {
            speedMode = "Rápido";
        } else if (slowMode) {
            speedMode = "Lento";
        }

        char text[128];
        sprintf(text, "Pos: (%.1f, %.1f, %.1f) | Vista: %s | Suelo: %s | Velocidad: %s",
                        camera->getX(), camera->getY(), camera->getZ(),
                        camera->isThirdPerson() ? "3ra" : "1ra",
                        camera->getIsGrounded() ? "Si" : "No",
                        speedMode);

        for (const char* c = text; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
        }

        glRasterPos2f(10.0f, viewport[3] - 40);
        const char* controls = "WASD: Mover | ESPACIO: Saltar | V: Vista | X: Velocidad rápida | Z: Velocidad lenta | ESC: Menu";
        for (const char* c = controls; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *c);
        }

        // Restaurar el estado OpenGL 3D
        glEnable(GL_DEPTH_TEST);
        if (lightning && lightning->isEnabled()) {
            glEnable(GL_LIGHTING);
        }

        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    }

    void handleKeyboard(unsigned char key, int x, int y) {
        // This method is no longer used, everything is handled in static callbacks
    }

    void handleKeyboardUp(unsigned char key, int x, int y) {
        // This method is no longer used, everything is handled in static callbacks
    }

    void handleSpecialKeys(int key, int x, int y) {
        // This method is no longer used, everything is handled in static callbacks
    }

    bool shouldExit() const {
        return exitRequested;
    }

    void resetExitRequest() {
        exitRequested = false;
    }

    Lightning* getLightning() const { return lightning; }
    bool isActive() const { return active; }
};

// Definir la instancia estática para InputManager
// Este bloque es parte de mechanics.h, pero si lo tienes en el mismo archivo, está bien.
// Moverlo a mechanics.cpp o asegurarte de que solo se defina una vez.
// InputManager* InputManager::instance = NULL; // Ya está definido en mechanics.h/cpp

// Definir las instancias estáticas para Playing
Playing* Playing::instance = NULL;
Game* Playing::gameInstance = NULL;

#endif // PLAYING_H
