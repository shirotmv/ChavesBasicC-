#ifndef MENU_H
#define MENU_H
#include <GL/glut.h>
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

enum MenuAction {
    MENU_NONE,
    MENU_NEW_GAME,
    MENU_LOAD_GAME,
    MENU_CONTINUE,
    MENU_OPTIONS,
    MENU_BONUS,
    MENU_EXIT
};

enum MenuState {
    MENU_PRESS_ANY_KEY,
    MENU_MAIN_OPTIONS
};

struct MenuItem {
    std::string text;
    MenuAction action;
    float alpha;
    bool isSelected;
    
    MenuItem(const std::string& t, MenuAction a) : text(t), action(a), alpha(0.0f), isSelected(false) {}
};

class Menu {
private:
    MenuState currentState;
    std::vector<MenuItem> menuItems;
    int selectedIndex;
    float fadeAlpha;
    float pressAnyKeyAlpha;
    float backgroundAlpha;
    bool fadingIn;
    bool fadingOut;
    float fadeSpeed;
    int animationTimer;
    GLuint backgroundTexture;
    bool textureLoaded;
    
    // Para video de fondo
    int videoFrame;
    float videoTime;
    GLuint videoTextures[60]; // 60 frames de video simulado
    bool videoInitialized;
    
    void initializeColors() {
        // Colores para diferentes estados del menú
    }
    
    void loadTexture() {
        // Inicializar video de fondo simulado
        if (!videoInitialized) {
            glGenTextures(60, videoTextures);
            
            // Crear 60 frames de video simulado con efecto de niebla/partículas
            for (int frame = 0; frame < 60; frame++) {
                glBindTexture(GL_TEXTURE_2D, videoTextures[frame]);
                
                unsigned char textureData[256 * 256 * 3];
                for (int i = 0; i < 256; i++) {
                    for (int j = 0; j < 256; j++) {
                        int index = (i * 256 + j) * 3;
                        
                        // Efecto de niebla animada
                        float noise = sin((i + frame * 2) * 0.1f) * cos((j + frame * 3) * 0.08f);
                        float fog = 0.1f + 0.05f * noise;
                        
                        // Colores oscuros con toques de azul/verde (estilo RE)
                        textureData[index] = (unsigned char)(20 + fog * 30);     // R
                        textureData[index + 1] = (unsigned char)(30 + fog * 40); // G
                        textureData[index + 2] = (unsigned char)(50 + fog * 50); // B
                    }
                }
                
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            }
            
            videoInitialized = true;
        }
        
        textureLoaded = true;
        std::cout << "Video background loaded (60 frames)" << std::endl;
    }
    
    void playSound(const char* soundName) {
        // Simulación de reproducción de sonidos
        std::cout << "Playing sound: " << soundName << std::endl;
        
        // Efecto de sonido básico con Windows API
        Beep(800, 100); // Frecuencia 800Hz, duración 100ms
    }
    
    void renderBackground() {
        if (!textureLoaded) {
            loadTexture();
        }
        
        int viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        float windowWidth = (float)viewport[2];
        float windowHeight = (float)viewport[3];
        
        // Actualizar frame del video
        videoTime += 0.016f; // ~60 FPS
        videoFrame = (int)(videoTime * 30.0f) % 60; // 30 FPS
        
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, videoTextures[videoFrame]);
        
        glColor4f(1.0f, 1.0f, 1.0f, backgroundAlpha);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(0.0f, 0.0f);
            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(windowWidth, 0.0f);
            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(windowWidth, windowHeight);
            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(0.0f, windowHeight);
        glEnd();
        
        glDisable(GL_TEXTURE_2D);
        
        // Overlay oscuro para mejor legibilidad
        glColor4f(0.0f, 0.0f, 0.0f, 0.6f * backgroundAlpha);
        glBegin(GL_QUADS);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(windowWidth, 0.0f);
            glVertex2f(windowWidth, windowHeight);
            glVertex2f(0.0f, windowHeight);
        glEnd();
    }
    
    int getTextWidth(const char* text, void* font) {
        int width = 0;
        for (const char* c = text; *c != '\0'; c++) {
            width += glutBitmapWidth(font, *c);
        }
        return width;
    }
    
    void renderTextCentered(float x, float y, const char* text, void* font, float alpha = 1.0f) {
        glColor4f(1.0f, 1.0f, 1.0f, alpha);
        glRasterPos2f(x, y);
        
        for (const char* c = text; *c != '\0'; c++) {
            glutBitmapCharacter(font, *c);
        }
    }
    
    void renderMenuItem(float x, float y, const MenuItem& item, int index) {
        float alpha = item.alpha;
        
        if (item.isSelected) {
            // Efecto de brillo para item seleccionado
            float pulse = 0.9f + 0.1f * sin(animationTimer * 0.1f);
            alpha *= pulse;
            
            // Calcular ancho del texto
            int textWidth = getTextWidth(item.text.c_str(), GLUT_BITMAP_HELVETICA_18);
            
            // Efecto de iluminación exterior de la palabra (glow effect)
            // Renderizar el texto múltiples veces con desplazamiento para crear efecto de brillo
            for (int offset = 1; offset <= 3; offset++) {
                float glowAlpha = alpha * (0.3f / offset);
                glColor4f(1.0f, 1.0f, 1.0f, glowAlpha);
                
                // Renderizar texto con offset en todas las direcciones
                glRasterPos2f(x + offset, y);
                for (const char* c = item.text.c_str(); *c != '\0'; c++) {
                    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
                }
                glRasterPos2f(x - offset, y);
                for (const char* c = item.text.c_str(); *c != '\0'; c++) {
                    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
                }
                glRasterPos2f(x, y + offset);
                for (const char* c = item.text.c_str(); *c != '\0'; c++) {
                    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
                }
                glRasterPos2f(x, y - offset);
                for (const char* c = item.text.c_str(); *c != '\0'; c++) {
                    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
                }
            }
            
            // Línea subrayada debajo del texto (CORREGIDO: ahora está debajo)
            glColor4f(1.0f, 1.0f, 1.0f, alpha * 0.9f);
            glLineWidth(2.0f);
            glBegin(GL_LINES);
                glVertex2f(x, y + 3.0f);  // Justo debajo del texto
                glVertex2f(x + textWidth, y + 3.0f);
            glEnd();
            glLineWidth(1.0f);
        }
        
        // Texto del item principal
        glColor4f(1.0f, 1.0f, 1.0f, alpha);
        glRasterPos2f(x, y);
        for (const char* c = item.text.c_str(); *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }
    }
    
    void transitionToMainMenu() {
        if (currentState == MENU_PRESS_ANY_KEY) {
            currentState = MENU_MAIN_OPTIONS;
            fadingIn = true;
            fadeAlpha = 0.0f;
            playSound("menu_enter.wav");
            
            // Inicializar animación de aparición de items
            for (size_t i = 0; i < menuItems.size(); i++) {
                menuItems[i].alpha = 0.0f;
            }
            
            std::cout << "Transicionando al menu principal..." << std::endl;
        }
    }
    
public:
    Menu() {
        currentState = MENU_PRESS_ANY_KEY;
        selectedIndex = 0;
        fadeAlpha = 0.0f;
        pressAnyKeyAlpha = 0.0f;
        backgroundAlpha = 0.0f;
        fadingIn = true;
        fadingOut = false;
        fadeSpeed = 0.02f;
        animationTimer = 0;
        textureLoaded = false;
        backgroundTexture = 0;
        
        // Inicializar video
        videoFrame = 0;
        videoTime = 0.0f;
        videoInitialized = false;
        
        initializeColors();
        

        menuItems.push_back(MenuItem("New Game", MENU_NEW_GAME));
        menuItems.push_back(MenuItem("Load Game", MENU_LOAD_GAME));
        menuItems.push_back(MenuItem("Continue", MENU_CONTINUE));
        menuItems.push_back(MenuItem("Options", MENU_OPTIONS));
        menuItems.push_back(MenuItem("Bonuses", MENU_BONUS));
        menuItems.push_back(MenuItem("Exit", MENU_EXIT));
        
        // Marcar el primer item como seleccionado
        if (!menuItems.empty()) {
            menuItems[0].isSelected = true;
        }
        
        std::cout << "Menu inicializado - Estado: PRESS_ANY_KEY" << std::endl;
    }
    
    ~Menu() {
        if (textureLoaded && videoInitialized) {
            glDeleteTextures(60, videoTextures);
        }
        std::cout << "Menu destruido" << std::endl;
    }
    
    void update() {
        animationTimer++;
        
        // Actualizar tiempo del video SIEMPRE
        videoTime += 0.016f; // ~60 FPS
        
        // Fade in inicial del background
        if (fadingIn && backgroundAlpha < 1.0f) {
            backgroundAlpha += fadeSpeed;
            if (backgroundAlpha >= 1.0f) {
                backgroundAlpha = 1.0f;
                fadingIn = false;
            }
        }
        
        // Animación de "Press Any Key"
        if (currentState == MENU_PRESS_ANY_KEY) {
            pressAnyKeyAlpha = 0.5f + 0.5f * sin(animationTimer * 0.05f);
        }
        
        // Animación de fade in de items del menú
        if (currentState == MENU_MAIN_OPTIONS) {
            for (size_t i = 0; i < menuItems.size(); i++) {
                float targetAlpha = 1.0f;
                float delay = (float)i * 0.1f;
                float progress = (animationTimer * 0.02f) - delay;
                
                if (progress > 0.0f) {
                    menuItems[i].alpha += 0.03f;
                    if (menuItems[i].alpha > targetAlpha) {
                        menuItems[i].alpha = targetAlpha;
                    }
                }
            }
        }
    }
    
    void render() {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        // Renderizar background
        renderBackground();
        
        int viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        float windowWidth = (float)viewport[2];
        float windowHeight = (float)viewport[3];
        
        switch(currentState) {
            case MENU_PRESS_ANY_KEY:
                {
                    // Título principal "VILLAGE" más grande y centrado
                    const char* title = "CHAVES";
                    int titleWidth = getTextWidth(title, GLUT_BITMAP_TIMES_ROMAN_24);
                    
                    float titleX = (windowWidth - titleWidth) / 2.0f;
                    float titleY = windowHeight / 2.0f - 50.0f;
                    
                    // Efecto de brillo en el título
                    float titleGlow = 0.8f + 0.2f * sin(animationTimer * 0.03f);
                    glColor4f(0.95f * titleGlow, 0.95f * titleGlow, 0.9f * titleGlow, backgroundAlpha);
                    glRasterPos2f(titleX, titleY);
                    for (const char* c = title; *c != '\0'; c++) {
                        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
                    }
                    
                    // Subtítulo "resident evil" más grande y centrado
                    const char* subtitle = "New Life";
                    int subtitleWidth = getTextWidth(subtitle, GLUT_BITMAP_HELVETICA_18);
                    
                    float subtitleX = (windowWidth - subtitleWidth) / 2.0f;
                    float subtitleY = titleY + 50.0f;
                    
                    glColor4f(0.8f, 0.8f, 0.7f, backgroundAlpha);
                    glRasterPos2f(subtitleX, subtitleY);
                    for (const char* c = subtitle; *c != '\0'; c++) {
                        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
                    }
                    
                    // Mensaje inferior centrado (MEJORADO: más grande, más arriba, con iluminación)
                    const char* pressKey = "Press any key to continue";
                    int pressKeyWidth = getTextWidth(pressKey, GLUT_BITMAP_HELVETICA_18);
                    
                    float pressKeyX = (windowWidth - pressKeyWidth) / 2.0f;
                    float pressKeyY = windowHeight - 100.0f;  // Subido un poco más
                    
                    // Efecto de brillo en "Press any key"
                    float pressKeyGlow = 0.7f + 0.3f * sin(animationTimer * 0.08f);
                    float finalAlpha = pressAnyKeyAlpha * backgroundAlpha * pressKeyGlow;
                    
                    // Efecto de iluminación exterior (glow effect)
                    for (int offset = 1; offset <= 2; offset++) {
                        float glowAlpha = finalAlpha * (0.4f / offset);
                        glColor4f(1.0f, 1.0f, 1.0f, glowAlpha);
                        
                        // Renderizar texto con offset en todas las direcciones
                        glRasterPos2f(pressKeyX + offset, pressKeyY);
                        for (const char* c = pressKey; *c != '\0'; c++) {
                            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
                        }
                        glRasterPos2f(pressKeyX - offset, pressKeyY);
                        for (const char* c = pressKey; *c != '\0'; c++) {
                            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
                        }
                        glRasterPos2f(pressKeyX, pressKeyY + offset);
                        for (const char* c = pressKey; *c != '\0'; c++) {
                            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
                        }
                        glRasterPos2f(pressKeyX, pressKeyY - offset);
                        for (const char* c = pressKey; *c != '\0'; c++) {
                            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
                        }
                    }
                    
                    // Texto principal
                    glColor4f(1.0f, 1.0f, 1.0f, finalAlpha);
                    glRasterPos2f(pressKeyX, pressKeyY);
                    for (const char* c = pressKey; *c != '\0'; c++) {
                        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
                    }
                }
                break;
                
            case MENU_MAIN_OPTIONS:
                {
                    // Título principal "VILLAGE" en posición superior más grande
                    const char* title = "CHAVES";
                    int titleWidth = getTextWidth(title, GLUT_BITMAP_TIMES_ROMAN_24);
                    
                    float titleX = (windowWidth - titleWidth) / 2.0f;
                    float titleY = 150.0f;
                    
                    // Efecto de brillo en el título
                    float titleGlow = 0.8f + 0.2f * sin(animationTimer * 0.03f);
                    glColor4f(0.95f * titleGlow, 0.95f * titleGlow, 0.9f * titleGlow, backgroundAlpha);
                    glRasterPos2f(titleX, titleY);
                    for (const char* c = title; *c != '\0'; c++) {
                        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
                    }
                    
                    // Subtítulo "resident evil" centrado
                    const char* subtitle = "New Life";
                    int subtitleWidth = getTextWidth(subtitle, GLUT_BITMAP_HELVETICA_18);
                    
                    float subtitleX = (windowWidth - subtitleWidth) / 2.0f;
                    float subtitleY = titleY + 50.0f;
                    
                    glColor4f(0.8f, 0.8f, 0.7f, backgroundAlpha);
                    glRasterPos2f(subtitleX, subtitleY);
                    for (const char* c = subtitle; *c != '\0'; c++) {
                        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
                    }
                    
                    // Items del menú - posicionados verticalmente centrados
                    float startY = windowHeight / 2.0f - 30.0f;
                    float itemSpacing = 45.0f;
                    
                    for (size_t i = 0; i < menuItems.size(); i++) {
                        float itemY = startY + (float)i * itemSpacing;
                        
                        // Centrar cada item individualmente
                        int itemWidth = getTextWidth(menuItems[i].text.c_str(), GLUT_BITMAP_HELVETICA_18);
                        float menuX = (windowWidth - itemWidth) / 2.0f;
                        
                        renderMenuItem(menuX, itemY, menuItems[i], (int)i);
                    }
                    
                    // Texto inferior con instrucciones
                    const char* instructions = "Use W/S to navigate, ENTER to select, ESC to go back";
                    int instrWidth = getTextWidth(instructions, GLUT_BITMAP_HELVETICA_12);
                    
                    float instrX = (windowWidth - instrWidth) / 2.0f;
                    
                    glColor4f(0.8f, 0.8f, 0.8f, backgroundAlpha * 0.6f);
                    glRasterPos2f(instrX, windowHeight - 80.0f);
                    for (const char* c = instructions; *c != '\0'; c++) {
                        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
                    }
                }
                break;
        }
        
        glDisable(GL_BLEND);
    }
    
    MenuAction handleKeyboard(unsigned char key, int x, int y) {
        if (currentState == MENU_PRESS_ANY_KEY) {
            transitionToMainMenu();
            return MENU_NONE;
        }
        
        if (currentState == MENU_MAIN_OPTIONS) {
            switch(key) {
                case 'w':
                case 'W':
                    // Navegar hacia arriba
                    if (selectedIndex > 0) {
                        menuItems[selectedIndex].isSelected = false;
                        selectedIndex--;
                        menuItems[selectedIndex].isSelected = true;
                        playSound("menu_move.wav");
                    }
                    break;
                    
                case 's':
                case 'S':
                    // Navegar hacia abajo
                    if (selectedIndex < (int)menuItems.size() - 1) {
                        menuItems[selectedIndex].isSelected = false;
                        selectedIndex++;
                        menuItems[selectedIndex].isSelected = true;
                        playSound("menu_move.wav");
                    }
                    break;
                    
                case 13: // ENTER
                    // Seleccionar item actual
                    playSound("menu_select.wav");
                    return menuItems[selectedIndex].action;
                    
                case 27: // ESC - CORREGIDO: ahora solo retrocede, no cierra el programa
                    playSound("menu_cancel.wav");
                    if (currentState == MENU_MAIN_OPTIONS) {
                        currentState = MENU_PRESS_ANY_KEY;
                        fadingIn = true;
                        backgroundAlpha = 0.0f;
                        pressAnyKeyAlpha = 0.0f;
                        animationTimer = 0;
                        // Resetear selección
                        for (size_t i = 0; i < menuItems.size(); i++) {
                            menuItems[i].isSelected = false;
                            menuItems[i].alpha = 0.0f;
                        }
                        if (!menuItems.empty()) {
                            menuItems[0].isSelected = true;
                        }
                        selectedIndex = 0;
                    }
                    return MENU_NONE;
                    
                default:
                    break;
            }
        }
        
        return MENU_NONE;
    }
    
    MenuState getCurrentState() const {
        return currentState;
    }
    
    int getSelectedIndex() const {
        return selectedIndex;
    }
};

#endif
