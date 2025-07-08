// Fixed options.h - Sin reiniciar animaciones al cambiar valores
#ifndef OPTIONS_H
#define OPTIONS_H

#include <GL/glut.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>  // Para atoi
#include <cmath>    // Para sin()

enum OptionsAction {
    OPTIONS_NONE,
    OPTIONS_BACK,
    OPTIONS_APPLY,
    OPTIONS_RESET_DEFAULTS
};

struct Resolution {
    int width;
    int height;
    std::string name;
    
    Resolution(int w, int h, const std::string& n) : width(w), height(h), name(n) {}
};

struct OptionItem {
    std::string label;
    std::string value;
    bool isSelected;
    float alpha;
    
    OptionItem(const std::string& l, const std::string& v) : label(l), value(v), isSelected(false), alpha(0.0f) {}
};

// Funciones helper para C++98
std::string intToString(int value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

int stringToInt(const std::string& str) {
    return atoi(str.c_str());
}

class OptionsMenu {
private:
    std::vector<OptionItem> optionItems;
    std::vector<Resolution> availableResolutions;
    int selectedIndex;
    float fadeAlpha;
    bool fadingIn;
    int animationTimer;
    bool animationCompleted;
    
    // Configuraciones actuales
    int currentVolume;          // 0-100
    int currentResolutionIndex; // Índice en availableResolutions
    bool isFullscreen;
    
    // Archivo de configuración
    std::string configFileName;
    
    void initializeResolutions() {
        availableResolutions.clear();
        availableResolutions.push_back(Resolution(800, 600, "800x600"));
        availableResolutions.push_back(Resolution(1024, 768, "1024x768"));
        availableResolutions.push_back(Resolution(1280, 720, "1280x720"));
        availableResolutions.push_back(Resolution(1280, 1024, "1280x1024"));
        availableResolutions.push_back(Resolution(1366, 768, "1366x768"));
        availableResolutions.push_back(Resolution(1600, 900, "1600x900"));
        availableResolutions.push_back(Resolution(1920, 1080, "1920x1080"));
        availableResolutions.push_back(Resolution(2560, 1440, "2560x1440"));
    }
    
    void initializeOptions() {
        optionItems.clear();
        
        // Volumen
        optionItems.push_back(OptionItem("Volume", intToString(currentVolume) + "%"));
        
        // Resolución
        if (currentResolutionIndex >= 0 && currentResolutionIndex < (int)availableResolutions.size()) {
            optionItems.push_back(OptionItem("Resolution", availableResolutions[currentResolutionIndex].name));
        } else {
            optionItems.push_back(OptionItem("Resolution", "800x600"));
        }
        
        // Fullscreen
        optionItems.push_back(OptionItem("Fullscreen", isFullscreen ? "ON" : "OFF"));
        
        // Opciones de control
        optionItems.push_back(OptionItem("Apply Settings", ""));
        optionItems.push_back(OptionItem("Reset to Defaults", ""));
        optionItems.push_back(OptionItem("Back to Menu", ""));
        
        std::cout << "DEBUG: initializeOptions() created " << optionItems.size() << " items" << std::endl;
        
        // NO reiniciar selectedIndex aquí si ya está dentro del rango válido
        if (selectedIndex < 0 || selectedIndex >= (int)optionItems.size()) {
            selectedIndex = 0;
        }
        updateSelection();
    }
    
    void updateSelection() {
        std::cout << "DEBUG: updateSelection() called with selectedIndex=" << selectedIndex 
                  << " and optionItems.size()=" << optionItems.size() << std::endl;
        
        // Limpiar todas las selecciones
        for (size_t i = 0; i < optionItems.size(); i++) {
            optionItems[i].isSelected = false;
        }
        
        // Verificar límites
        if (selectedIndex < 0) {
            std::cout << "DEBUG: selectedIndex was < 0, setting to 0" << std::endl;
            selectedIndex = 0;
        }
        if (selectedIndex >= (int)optionItems.size()) {
            std::cout << "DEBUG: selectedIndex was >= size, setting to " << (int)optionItems.size() - 1 << std::endl;
            selectedIndex = (int)optionItems.size() - 1;
        }
        
        // Seleccionar el item actual
        if (selectedIndex >= 0 && selectedIndex < (int)optionItems.size()) {
            optionItems[selectedIndex].isSelected = true;
            std::cout << "DEBUG: Selected item " << selectedIndex << ": " << optionItems[selectedIndex].label << std::endl;
        } else {
            std::cout << "DEBUG: ERROR - selectedIndex out of bounds after bounds check!" << std::endl;
        }
        
        std::cout << "DEBUG: Final selectedIndex=" << selectedIndex << std::endl;
    }
    
    void loadConfiguration() {
        std::ifstream file(configFileName.c_str());
        
        if (!file.is_open()) {
            std::cout << "Config file not found, using defaults" << std::endl;
            setDefaults();
            return;
        }
        
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            
            size_t pos = line.find('=');
            if (pos == std::string::npos) continue;
            
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            
            // Remover espacios
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            
            if (key == "volume") {
                currentVolume = stringToInt(value);
                if (currentVolume < 0) currentVolume = 0;
                if (currentVolume > 100) currentVolume = 100;
            }
            else if (key == "resolution_width") {
                int width = stringToInt(value);
                for (size_t i = 0; i < availableResolutions.size(); i++) {
                    if (availableResolutions[i].width == width) {
                        currentResolutionIndex = (int)i;
                        break;
                    }
                }
            }
            else if (key == "fullscreen") {
                isFullscreen = (value == "true" || value == "1");
            }
        }
        
        file.close();
        std::cout << "Configuration loaded successfully" << std::endl;
    }
    
    void saveConfiguration() {
        std::ofstream file(configFileName.c_str());
        
        if (!file.is_open()) {
            std::cout << "Error: Cannot save configuration file" << std::endl;
            return;
        }
        
        file << "# Game Configuration File\n";
        file << "# Auto-generated - Do not edit manually\n\n";
        
        file << "volume=" << currentVolume << "\n";
        
        if (currentResolutionIndex >= 0 && currentResolutionIndex < (int)availableResolutions.size()) {
            file << "resolution_width=" << availableResolutions[currentResolutionIndex].width << "\n";
            file << "resolution_height=" << availableResolutions[currentResolutionIndex].height << "\n";
        }
        
        file << "fullscreen=" << (isFullscreen ? "true" : "false") << "\n";
        
        file.close();
        std::cout << "Configuration saved successfully" << std::endl;
    }
    
    void setDefaults() {
        currentVolume = 50;
        currentResolutionIndex = 0; // 800x600
        isFullscreen = false;
        
        std::cout << "Default configuration applied" << std::endl;
    }
    
    // FUNCIÓN CORREGIDA: Solo actualiza valores sin reiniciar animaciones
    void updateOptionValues() {
        if (optionItems.size() >= 3) {
            // Actualizar volumen
            optionItems[0].value = intToString(currentVolume) + "%";
            
            // Actualizar resolución
            if (currentResolutionIndex >= 0 && currentResolutionIndex < (int)availableResolutions.size()) {
                optionItems[1].value = availableResolutions[currentResolutionIndex].name;
            }
            
            // Actualizar fullscreen
            optionItems[2].value = isFullscreen ? "ON" : "OFF";
        }
        // NO llamar a initializeOptions() aquí - esa era la causa del problema
    }
    
    int getTextWidth(const char* text, void* font) {
        int width = 0;
        for (const char* c = text; *c != '\0'; c++) {
            width += glutBitmapWidth(font, *c);
        }
        return width;
    }
    
    void renderText(float x, float y, const char* text, void* font, float r, float g, float b, float alpha) {
        glColor4f(r, g, b, alpha);
        glRasterPos2f(x, y);
        for (const char* c = text; *c != '\0'; c++) {
            glutBitmapCharacter(font, *c);
        }
    }
    
    void renderOptionItem(float x, float y, const OptionItem& item, int index) {
        float alpha = item.alpha;
        
        if (item.isSelected) {
            // Línea subrayada simple y fija
            if (!item.value.empty()) {
                int labelWidth = getTextWidth(item.label.c_str(), GLUT_BITMAP_HELVETICA_18);
                int valueWidth = getTextWidth(item.value.c_str(), GLUT_BITMAP_HELVETICA_18);
                int totalWidth = labelWidth + 20 + valueWidth;
                
                glColor4f(1.0f, 1.0f, 1.0f, alpha * 0.6f);
                glLineWidth(1.0f);
                glBegin(GL_LINES);
                    glVertex2f(x, y + 3.0f);
                    glVertex2f(x + totalWidth, y + 3.0f);
                glEnd();
            } else {
                int labelWidth = getTextWidth(item.label.c_str(), GLUT_BITMAP_HELVETICA_18);
                glColor4f(1.0f, 1.0f, 1.0f, alpha * 0.6f);
                glLineWidth(1.0f);
                glBegin(GL_LINES);
                    glVertex2f(x, y + 3.0f);
                    glVertex2f(x + labelWidth, y + 3.0f);
                glEnd();
            }
            
            // Renderizar label seleccionado - color fijo
            renderText(x, y, item.label.c_str(), GLUT_BITMAP_HELVETICA_18, 1.0f, 1.0f, 1.0f, alpha);
        } else {
            // Renderizar label normal
            renderText(x, y, item.label.c_str(), GLUT_BITMAP_HELVETICA_18, 0.8f, 0.8f, 0.8f, alpha);
        }
        
        // Renderizar valor si existe
        if (!item.value.empty()) {
            int labelWidth = getTextWidth(item.label.c_str(), GLUT_BITMAP_HELVETICA_18);
            float valueX = x + labelWidth + 20.0f;
            
            // Color diferente para los valores
            if (item.isSelected) {
                renderText(valueX, y, item.value.c_str(), GLUT_BITMAP_HELVETICA_18, 1.0f, 1.0f, 0.0f, alpha);
            } else {
                renderText(valueX, y, item.value.c_str(), GLUT_BITMAP_HELVETICA_18, 0.7f, 0.7f, 0.7f, alpha);
            }
        }
    }
    
public:
    OptionsMenu() {
        selectedIndex = 0;
        fadeAlpha = 0.0f;
        fadingIn = true;
        animationTimer = 0;
        animationCompleted = false;
        configFileName = "game_config.txt";
        
        initializeResolutions();
        loadConfiguration();
        initializeOptions();
        
        std::cout << "Options menu initialized" << std::endl;
    }
    
    ~OptionsMenu() {
        std::cout << "Options menu destroyed" << std::endl;
    }
    
    void update() {
        animationTimer++;
        
        // Animación de fade in solo si no está completada
        if (fadingIn && !animationCompleted) {
            bool allItemsVisible = true;
            
            for (size_t i = 0; i < optionItems.size(); i++) {
                float delay = (float)i * 0.1f;
                float progress = (animationTimer * 0.02f) - delay;
                
                if (progress > 0.0f) {
                    optionItems[i].alpha += 0.03f;
                    if (optionItems[i].alpha > 1.0f) {
                        optionItems[i].alpha = 1.0f;
                    }
                }
                
                if (optionItems[i].alpha < 1.0f) {
                    allItemsVisible = false;
                }
            }
            
            // Marcar animación como completada
            if (allItemsVisible) {
                fadingIn = false;
                animationCompleted = true;
                std::cout << "DEBUG: Animation completed" << std::endl;
            }
        }
        
        // Sin más animaciones de selección - todo estático
    }
    
    void render() {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        int viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        float windowWidth = (float)viewport[2];
        float windowHeight = (float)viewport[3];
        
        // Fondo semi-transparente
        glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
        glBegin(GL_QUADS);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(windowWidth, 0.0f);
            glVertex2f(windowWidth, windowHeight);
            glVertex2f(0.0f, windowHeight);
        glEnd();
        
        // Título
        const char* title = "OPTIONS";
        int titleWidth = getTextWidth(title, GLUT_BITMAP_TIMES_ROMAN_24);
        float titleX = (windowWidth - titleWidth) / 2.0f;
        float titleY = 100.0f;
        
        renderText(titleX, titleY, title, GLUT_BITMAP_TIMES_ROMAN_24, 1.0f, 1.0f, 1.0f, 1.0f);
        
        // Línea decorativa debajo del título
        glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
        glLineWidth(2.0f);
        glBegin(GL_LINES);
            glVertex2f(titleX, titleY + 10.0f);
            glVertex2f(titleX + titleWidth, titleY + 10.0f);
        glEnd();
        glLineWidth(1.0f);
        
        // Opciones del menú
        float startY = windowHeight / 2.0f - 50.0f;
        float itemSpacing = 45.0f;
        
        for (size_t i = 0; i < optionItems.size(); i++) {
            float itemY = startY + (float)i * itemSpacing;
            float itemX = windowWidth / 2.0f - 150.0f;
            
            renderOptionItem(itemX, itemY, optionItems[i], (int)i);
        }
        
        // Instrucciones
        const char* instructions = "W/S: Navigate  A/D: Change value  ENTER: Select  ESC: Back";
        int instrWidth = getTextWidth(instructions, GLUT_BITMAP_HELVETICA_12);
        float instrX = (windowWidth - instrWidth) / 2.0f;
        
        renderText(instrX, windowHeight - 60.0f, instructions, GLUT_BITMAP_HELVETICA_12, 0.8f, 0.8f, 0.8f, 0.8f);
        
        glDisable(GL_BLEND);
    }
    
    OptionsAction handleKeyboard(unsigned char key, int x, int y) {
        std::cout << "DEBUG: handleKeyboard called with key=" << (int)key << " (" << key << ")" << std::endl;
        
        switch(key) {
            case 'w':
            case 'W':
                std::cout << "DEBUG: W pressed, current selectedIndex=" << selectedIndex << std::endl;
                if (selectedIndex > 0) {
                    selectedIndex--;
                    std::cout << "DEBUG: Moving up to " << selectedIndex << std::endl;
                    updateSelection();
                }
                break;
                
            case 's':
            case 'S':
                std::cout << "DEBUG: S pressed, current selectedIndex=" << selectedIndex << ", max=" << (int)optionItems.size() - 1 << std::endl;
                if (selectedIndex < (int)optionItems.size() - 1) {
                    selectedIndex++;
                    std::cout << "DEBUG: Moving down to " << selectedIndex << std::endl;
                    updateSelection();
                }
                break;
                
            case 'a':
            case 'A':
                std::cout << "DEBUG: A pressed for option " << selectedIndex << std::endl;
                if (selectedIndex == 0) { // Volumen
                    currentVolume -= 5;
                    if (currentVolume < 0) currentVolume = 0;
                    updateOptionValues(); // Solo actualiza valores, no reinicia animaciones
                }
                else if (selectedIndex == 1) { // Resolución
                    currentResolutionIndex--;
                    if (currentResolutionIndex < 0) currentResolutionIndex = (int)availableResolutions.size() - 1;
                    updateOptionValues(); // Solo actualiza valores, no reinicia animaciones
                }
                else if (selectedIndex == 2) { // Fullscreen
                    isFullscreen = !isFullscreen;
                    updateOptionValues(); // Solo actualiza valores, no reinicia animaciones
                }
                break;
                
            case 'd':
            case 'D':
                std::cout << "DEBUG: D pressed for option " << selectedIndex << std::endl;
                if (selectedIndex == 0) { // Volumen
                    currentVolume += 5;
                    if (currentVolume > 100) currentVolume = 100;
                    updateOptionValues(); // Solo actualiza valores, no reinicia animaciones
                }
                else if (selectedIndex == 1) { // Resolución
                    currentResolutionIndex++;
                    if (currentResolutionIndex >= (int)availableResolutions.size()) currentResolutionIndex = 0;
                    updateOptionValues(); // Solo actualiza valores, no reinicia animaciones
                }
                else if (selectedIndex == 2) { // Fullscreen
                    isFullscreen = !isFullscreen;
                    updateOptionValues(); // Solo actualiza valores, no reinicia animaciones
                }
                break;
                
            case 13: // ENTER
                std::cout << "DEBUG: ENTER pressed on option " << selectedIndex << std::endl;
                if (selectedIndex == 3) { // Apply Settings
                    saveConfiguration();
                    std::cout << "Settings saved! Main will apply them." << std::endl;
                    return OPTIONS_APPLY;
                }
                else if (selectedIndex == 4) { // Reset to Defaults
                    setDefaults();
                    updateOptionValues(); // Solo actualiza valores, no reinicia animaciones
                    std::cout << "Settings reset to defaults" << std::endl;
                    return OPTIONS_RESET_DEFAULTS;
                }
                else if (selectedIndex == 5) { // Back to Menu
                    return OPTIONS_BACK;
                }
                break;
                
            case 27: // ESC
                std::cout << "DEBUG: ESC pressed, returning to menu" << std::endl;
                return OPTIONS_BACK;
                
            default:
                std::cout << "DEBUG: Unhandled key: " << (int)key << std::endl;
                break;
        }
        
        return OPTIONS_NONE;
    }
    
    // Métodos para obtener configuración actual
    int getCurrentVolume() const { return currentVolume; }
    bool getIsFullscreen() const { return isFullscreen; }
    Resolution getCurrentResolution() const {
        if (currentResolutionIndex >= 0 && currentResolutionIndex < (int)availableResolutions.size()) {
            return availableResolutions[currentResolutionIndex];
        }
        return Resolution(800, 600, "800x600");
    }
    
    void setFullscreen(bool fullscreen) {
        isFullscreen = fullscreen;
        updateOptionValues(); // Solo actualiza valores, no reinicia animaciones
        std::cout << "Fullscreen set to: " << (isFullscreen ? "ON" : "OFF") << std::endl;
    }
    

};

#endif
