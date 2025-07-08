#ifndef GAME_H
#define GAME_H

#include <GL/glut.h>
#include <string>
#include <cstdlib>
#include <sstream>
#include <cstdio>
#include "menu.h"
#include "options.h"
#include "playing.h"

enum GameState {
    STATE_MENU,
    STATE_PLAYING,
    STATE_PAUSED,
    STATE_GAME_OVER,
    STATE_OPTIONS
};

extern OptionsMenu* globalOptionsMenu;
extern void applyGlobalConfiguration();

class Config {
private:
    int windowWidth;
    int windowHeight;
    bool fullscreen;
    
public:
    Config() : windowWidth(800), windowHeight(600), fullscreen(false) {
    }
    
    ~Config() {
    }
    
    void updateFromGlobal() {
        if (globalOptionsMenu) {
            Resolution res = globalOptionsMenu->getCurrentResolution();
            windowWidth = res.width;
            windowHeight = res.height;
            fullscreen = globalOptionsMenu->getIsFullscreen();
        }
    }
    
    int getWidth() const { return windowWidth; }
    int getHeight() const { return windowHeight; }
    bool isFullscreen() const { return fullscreen; }
};

class Game {
private:
    GameState currentState;
    GameState previousState;
    Menu* menu;
    Config* config;
    Playing* playing;
    bool pendingConfigApply;
    bool stateJustChanged;
    bool menuStateSetup; // Nueva variable para controlar setup del menú
    
    // Instancia estatica para callbacks
    static Game* instance;
    
    // Callbacks estaticos para Game (menu y otras pantallas)
    static void gameKeyboardCallback(unsigned char key, int x, int y) {
        if (instance) {
            instance->handleKeyboard(key, x, y);
        }
    }
    
    static void gameKeyboardUpCallback(unsigned char key, int x, int y) {
        if (instance) {
            instance->handleKeyboardUp(key, x, y);
        }
    }
    
    static void gameSpecialKeyCallback(int key, int x, int y) {
        if (instance) {
            instance->handleSpecialKeys(key, x, y);
        }
    }
    
    static void gameMouseCallback(int button, int state, int x, int y) {
        // Callback vacio para estados de menu - evita problemas con mouse
    }
    
    static void gameMouseMotionCallback(int x, int y) {
        // Callback vacio para estados de menu - evita problemas con mouse
    }
    
    std::string intToString(int value) const {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }
    
    void renderText(float x, float y, const char* text) {
        glRasterPos2f(x, y);
        for (const char* c = text; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }
    }
    
    void renderPausedOverlay() {
        GLboolean lightingEnabled = glIsEnabled(GL_LIGHTING);
        GLboolean depthTestEnabled = glIsEnabled(GL_DEPTH_TEST);
        
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
        
        int viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, viewport[2], viewport[3], 0, -1, 1);
        
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        
        glBegin(GL_QUADS);
            glVertex2f(0.0f, 0.0f);
            glVertex2f((float)viewport[2], 0.0f);
            glVertex2f((float)viewport[2], (float)viewport[3]);
            glVertex2f(0.0f, (float)viewport[3]);
        glEnd();
        glDisable(GL_BLEND);
        
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f((float)viewport[2] / 2.0f - 40.0f, (float)viewport[3] / 2.0f);
        const char* pausedText = "PAUSADO";
        for (const char* c = pausedText; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }
        
        glRasterPos2f((float)viewport[2] / 2.0f - 80.0f, (float)viewport[3] / 2.0f + 30.0f);
        const char* continueText = "Presiona P para continuar";
        for (const char* c = continueText; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
        }
        
        glRasterPos2f((float)viewport[2] / 2.0f - 80.0f, (float)viewport[3] / 2.0f + 50.0f);
        const char* menuText = "Presiona ESC para menu";
        for (const char* c = menuText; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
        }
        
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        
        if (lightingEnabled) {
            glEnable(GL_LIGHTING);
        }
        if (depthTestEnabled) {
            glEnable(GL_DEPTH_TEST);
        }
    }
    
    void setupMenuLighting() {
        // Limpiar completamente el estado de iluminación
        glDisable(GL_LIGHTING);
        for (int i = 0; i < 8; i++) {
            glDisable(GL_LIGHT0 + i);
        }
        
        // Limpiar otros estados que pueden verse afectados
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glDisable(GL_NORMALIZE);
        glDisable(GL_COLOR_MATERIAL);
        glDisable(GL_BLEND);
        
        // Restaurar color de fondo para menús
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        
        // Establecer color base
        glColor3f(1.0f, 1.0f, 1.0f);
        
        // Configurar matriz de proyección para 2D
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        
        int viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        if (viewport[2] > 0 && viewport[3] > 0) {
            glOrtho(0, viewport[2], viewport[3], 0, -1, 1);
        } else {
            glOrtho(0, 800, 600, 0, -1, 1);
        }
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }
    
    void setupMenuState() {
        // Configuración completa del estado del menú - SE EJECUTA UNA SOLA VEZ
        setupMenuLighting();
        
        // Restaurar cursor normal para menus
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        
        // Configurar callbacks para estados de menu
        glutKeyboardFunc(gameKeyboardCallback);
        glutKeyboardUpFunc(gameKeyboardUpCallback);
        glutSpecialFunc(gameSpecialKeyCallback);
        glutMouseFunc(gameMouseCallback);
        glutPassiveMotionFunc(gameMouseMotionCallback);
        glutMotionFunc(gameMouseMotionCallback);
        
        // Asegurar que el buffer se limpia correctamente
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Marcar que el setup del menú ya se realizó
        menuStateSetup = true;
    }
    
    void renderGameOver() {
        if (!menuStateSetup) {
            setupMenuState();
        }
        
        glClearColor(0.1f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        int viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        
        glColor3f(1.0f, 0.0f, 0.0f);
        glRasterPos2f((float)viewport[2] / 2.0f - 60.0f, (float)viewport[3] / 2.0f);
        const char* gameOverText = "GAME OVER";
        for (const char* c = gameOverText; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }
        
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f((float)viewport[2] / 2.0f - 100.0f, (float)viewport[3] / 2.0f + 40.0f);
        const char* menuText = "Presiona ESC para menu";
        for (const char* c = menuText; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
        }
        
        glRasterPos2f((float)viewport[2] / 2.0f - 80.0f, (float)viewport[3] / 2.0f + 60.0f);
        const char* restartText = "Presiona R para reiniciar";
        for (const char* c = restartText; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
        }
    }
    
    void safeStateTransition(GameState newState) {
        previousState = currentState;
        
        // Cleanup específico del estado anterior
        if (previousState == STATE_PLAYING) {
            if (playing) {
                playing->setActive(false);
            }
        }
        
        currentState = newState;
        stateJustChanged = true;
        menuStateSetup = false; // Resetear flag para forzar setup en el próximo render
        
        // Setup específico del nuevo estado
        if (currentState == STATE_PLAYING) {
            if (playing) {
                playing->setActive(true);
            }
        } else {
            // Para cualquier estado que no sea PLAYING, forzar setup completo del menú
            setupMenuState();
        }
    }
    
    void cleanupLightingState() {
        // Deshabilitar sistema de iluminación
        glDisable(GL_LIGHTING);
        for (int i = 0; i < 8; i++) {
            glDisable(GL_LIGHT0 + i);
        }
        
        // Limpiar otros estados relacionados con rendering 3D
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glDisable(GL_NORMALIZE);
        glDisable(GL_COLOR_MATERIAL);
        glDisable(GL_BLEND);
        
        // Resetear color de fondo
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        
        // Resetear color actual
        glColor3f(1.0f, 1.0f, 1.0f);
        
        // Limpiar matriz de transformación
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        // Resetear viewport y proyección para UI
        int viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        if (viewport[2] > 0 && viewport[3] > 0) {
            glOrtho(0, viewport[2], viewport[3], 0, -1, 1);
        }
        glMatrixMode(GL_MODELVIEW);
    }
    
public:
    Game() : currentState(STATE_MENU), previousState(STATE_MENU), menu(NULL), config(NULL), playing(NULL), pendingConfigApply(false), stateJustChanged(false), menuStateSetup(false) {
        instance = this; // Establecer instancia estatica
        
        config = new Config();
        if (config) {
            config->updateFromGlobal();
        }
        
        menu = new Menu();
        playing = new Playing();
        
        // Configurar estado inicial del menú
        setupMenuState();
    }
    
    ~Game() {
        if (menu) {
            delete menu;
            menu = NULL;
        }
        if (config) {
            delete config;
            config = NULL;
        }
        if (playing) {
            delete playing;
            playing = NULL;
        }
        
        if (instance == this) {
            instance = NULL;
        }
    }
    
    void update() {
        if (pendingConfigApply) {
            applyGlobalConfiguration();
            pendingConfigApply = false;
        }
        
        static float deltaTime = 0.016f;
        
        switch(currentState) {
            case STATE_MENU:
                if (menu) {
                    menu->update();
                }
                break;
            case STATE_PLAYING:
                if (playing) {
                    playing->update(deltaTime);
                    
                    // Verificar si Playing solicita salir al menú
                    if (playing->shouldExit()) {
                        playing->resetExitRequest();
                        safeStateTransition(STATE_MENU);
                    }
                }
                break;
            case STATE_PAUSED:
                break;
            case STATE_GAME_OVER:
                break;
            case STATE_OPTIONS:
                if (globalOptionsMenu) {
                    globalOptionsMenu->update();
                }
                break;
        }
        
        // Resetear flag de cambio de estado después del primer update
        if (stateJustChanged) {
            stateJustChanged = false;
        }
    }
    
    void render() {
        switch(currentState) {
            case STATE_MENU:
                // Verificar si necesita setup del menú
                if (!menuStateSetup) {
                    setupMenuState();
                }
                // Solo limpiar el buffer, no reconfigurar todo el estado
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                if (menu) {
                    menu->render();
                }
                break;
            case STATE_PLAYING:
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                if (playing) {
                    playing->render();
                }
                break;
            case STATE_PAUSED:
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                if (playing) {
                    playing->render();
                }
                renderPausedOverlay();
                break;
            case STATE_GAME_OVER:
                renderGameOver();
                break;
            case STATE_OPTIONS:
                // Verificar si necesita setup del menú
                if (!menuStateSetup) {
                    setupMenuState();
                }
                // Solo limpiar el buffer, no reconfigurar todo el estado
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                if (globalOptionsMenu) {
                    globalOptionsMenu->render();
                }
                break;
        }
    }
    
    void handleKeyboard(unsigned char key, int x, int y) {
        switch(currentState) {
            case STATE_MENU:
                if (key == 27) {
                    return;
                }
                if (menu) {
                    MenuAction action = menu->handleKeyboard(key, x, y);
                    switch(action) {
                        case MENU_NEW_GAME:
                            if (playing) {
                                delete playing;
                                playing = new Playing();
                            }
                            safeStateTransition(STATE_PLAYING);
                            break;
                        case MENU_LOAD_GAME:
                            safeStateTransition(STATE_PLAYING);
                            break;
                        case MENU_CONTINUE:
                            safeStateTransition(STATE_PLAYING);
                            break;
                        case MENU_OPTIONS:
                            safeStateTransition(STATE_OPTIONS);
                            break;
                        case MENU_BONUS:
                            break;
                        case MENU_EXIT:
                            exit(0);
                            break;
                        case MENU_NONE:
                            break;
                    }
                }
                break;
                
            case STATE_PLAYING:
                if (key == 'p' || key == 'P') {
                    currentState = STATE_PAUSED;
                    return;
                }
                // Playing maneja sus propios inputs a través de sus callbacks
                break;
                
            case STATE_PAUSED:
                if (key == 27) {
                    safeStateTransition(STATE_MENU);
                    return;
                }
                if (key == 'p' || key == 'P' || key == ' ') {
                    currentState = STATE_PLAYING;
                }
                break;
                
            case STATE_GAME_OVER:
                if (key == 27) {
                    safeStateTransition(STATE_MENU);
                    return;
                }
                if (key == 'r' || key == 'R') {
                    if (playing) {
                        delete playing;
                        playing = new Playing();
                    }
                    safeStateTransition(STATE_PLAYING);
                }
                break;
                
            case STATE_OPTIONS:
                if (globalOptionsMenu) {
                    OptionsAction action = globalOptionsMenu->handleKeyboard(key, x, y);
                    switch(action) {
                        case OPTIONS_BACK:
                            safeStateTransition(STATE_MENU);
                            break;
                        case OPTIONS_APPLY:
                            pendingConfigApply = true;
                            break;
                        case OPTIONS_RESET_DEFAULTS:
                            pendingConfigApply = true;
                            break;
                        case OPTIONS_NONE:
                            break;
                    }
                }
                break;
        }
    }
    
    void handleKeyboardUp(unsigned char key, int x, int y) {
        // Solo Playing necesita manejar keyUp, y lo hace a través de sus callbacks
        // No necesitamos hacer nada aquí para otros estados
    }
    
    void handleSpecialKeys(int key, int x, int y) {
        switch(currentState) {
            case STATE_MENU:
                if (menu) {
                    // Manejar teclas especiales del menú si es necesario
                }
                break;
                
            case STATE_PLAYING:
                // Playing maneja sus propias teclas especiales a través de sus callbacks
                break;
                
            case STATE_PAUSED:
                break;
                
            case STATE_GAME_OVER:
                break;
                
            case STATE_OPTIONS:
                break;
        }
    }
    
    std::string getLightingInfo() const {
        std::string info = "Lighting: ";
        if (currentState == STATE_PLAYING && playing && playing->getLightning()) {
            info += "Dynamic lighting system active";
        } else {
            info += "Static lighting (Menu/UI)";
        }
        return info;
    }
    
    GameState getCurrentState() const {
        return currentState;
    }
    
    void setState(GameState newState) {
        safeStateTransition(newState);
    }
    
    Config* getConfig() const {
        return config;
    }
    
    Playing* getPlaying() const {
        return playing;
    }
    
    void setGameOver() {
        safeStateTransition(STATE_GAME_OVER);
    }
    
    GameState getPreviousState() const {
        return previousState;
    }
    
    bool hasStateJustChanged() const {
        return stateJustChanged;
    }
};

// Definir la instancia estatica
Game* Game::instance = NULL;

#endif
