#include <GL/glut.h>
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include "loadingstart.h"
#include "game.h"
#include "options.h"

bool loadingComplete = false;
LoadingStart* loader;
Game* game;
OptionsMenu* globalOptionsMenu;

struct GlobalConfig {
    int windowWidth;
    int windowHeight;
    bool fullscreen;
    int volume;
    bool configLoaded;
    bool windowCreated;
    bool isApplying;
    
    GlobalConfig() : windowWidth(800), windowHeight(600), fullscreen(false), volume(50), configLoaded(false), windowCreated(false), isApplying(false) {}
};

GlobalConfig globalConfig;

void applyGlobalConfiguration() {
    if (!globalOptionsMenu || globalConfig.isApplying) return;
    
    globalConfig.isApplying = true;
    
    Resolution currentRes = globalOptionsMenu->getCurrentResolution();
    bool currentFullscreen = globalOptionsMenu->getIsFullscreen();
    int currentVolume = globalOptionsMenu->getCurrentVolume();
    
    bool resolutionChanged = (globalConfig.windowWidth != currentRes.width || 
                             globalConfig.windowHeight != currentRes.height);
    bool fullscreenChanged = (globalConfig.fullscreen != currentFullscreen);
    
    globalConfig.windowWidth = currentRes.width;
    globalConfig.windowHeight = currentRes.height;
    globalConfig.fullscreen = currentFullscreen;
    globalConfig.volume = currentVolume;
    
    if (globalConfig.windowCreated && (resolutionChanged || fullscreenChanged)) {
        if (resolutionChanged) {
            glutReshapeWindow(globalConfig.windowWidth, globalConfig.windowHeight);
        }
        
        if (!globalConfig.fullscreen) {
            int screenWidth = GetSystemMetrics(SM_CXSCREEN);
            int screenHeight = GetSystemMetrics(SM_CYSCREEN);
            int posX = (screenWidth - globalConfig.windowWidth) / 2;
            int posY = (screenHeight - globalConfig.windowHeight) / 2;
            glutPositionWindow(posX, posY);
        }
        
        if (fullscreenChanged) {
            if (globalConfig.fullscreen) {
                glutFullScreen();
            } else {
                glutReshapeWindow(globalConfig.windowWidth, globalConfig.windowHeight);
                int screenWidth = GetSystemMetrics(SM_CXSCREEN);
                int screenHeight = GetSystemMetrics(SM_CYSCREEN);
                int posX = (screenWidth - globalConfig.windowWidth) / 2;
                int posY = (screenHeight - globalConfig.windowHeight) / 2;
                glutPositionWindow(posX, posY);
            }
        }
    }
    
    globalConfig.configLoaded = true;
    globalConfig.isApplying = false;
}

void loadInitialConfiguration() {
    globalOptionsMenu = new OptionsMenu();
    
    if (globalOptionsMenu) {
        Resolution currentRes = globalOptionsMenu->getCurrentResolution();
        bool currentFullscreen = globalOptionsMenu->getIsFullscreen();
        int currentVolume = globalOptionsMenu->getCurrentVolume();
        
        globalConfig.windowWidth = currentRes.width;
        globalConfig.windowHeight = currentRes.height;
        globalConfig.fullscreen = currentFullscreen;
        globalConfig.volume = currentVolume;
        globalConfig.configLoaded = true;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    if (!loadingComplete) {
        loader->render();
        if (loader->isComplete()) {
            loadingComplete = true;
            delete loader;
            loader = NULL;
            
            game = new Game();
        }
    } else {
        if (game) {
            game->render();
        }
    }
    
    glutSwapBuffers();
}

void update(int value) {
    if (!loadingComplete) {
        if (loader) {
            loader->update();
        }
    } else {
        if (game) {
            game->update();
        }
    }
    
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void keyboard(unsigned char key, int x, int y) {
    if (!loadingComplete) {
        if (key == 27) {
            if (loader) {
                delete loader;
                loader = NULL;
            }
            if (game) {
                delete game;
                game = NULL;
            }
            if (globalOptionsMenu) {
                delete globalOptionsMenu;
                globalOptionsMenu = NULL;
            }
            exit(0);
        }
        
        if (loader) {
            loader->skipSlide();
        }
    }
    else if (loadingComplete && game) {
        if (key == 'F' || key == 'f') {
            if (globalOptionsMenu) {
                bool current = globalOptionsMenu->getIsFullscreen();
                globalOptionsMenu->setFullscreen(!current);
                applyGlobalConfiguration();
            }
        }
        
        game->handleKeyboard(key, x, y);
        
        if (game->getCurrentState() == STATE_OPTIONS) {
        }
    }
}

void reshape(int width, int height) {
    if (height == 0) height = 1;
    
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    if (!globalConfig.isApplying) {
        globalConfig.windowWidth = width;
        globalConfig.windowHeight = height;
    }
}

void cleanup() {
    if (loader) {
        delete loader;
        loader = NULL;
    }
    
    if (game) {
        delete game;
        game = NULL;
    }
    
    if (globalOptionsMenu) {
        delete globalOptionsMenu;
        globalOptionsMenu = NULL;
    }
}

void exitHandler() {
    cleanup();
}

void closeWindow() {
    cleanup();
    exit(0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_ALPHA);
    
    loadInitialConfiguration();
    
    glutInitWindowSize(globalConfig.windowWidth, globalConfig.windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Game Loading Demo - Global Config");
    
    globalConfig.windowCreated = true;
    
    if (globalConfig.fullscreen) {
        glutFullScreen();
    }
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    loader = new LoadingStart();
    game = NULL;
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    
    glutTimerFunc(16, update, 0);
    
    atexit(exitHandler);
    
    glutMainLoop();
    
    return 0;
}

