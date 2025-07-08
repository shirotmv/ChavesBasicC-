#ifndef LOADINGSTART_H
#define LOADINGSTART_H

#include <GL/glut.h>
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <string>

class LoadingStart {
private:
    // Estructura para guardar dimensiones de textura (debe estar ANTES de usarla)
    struct TextureInfo {
        GLuint id;
        int width;
        int height;
    };
    
    int currentSlide;
    int slideTimer;
    int slideDuration;
    float progressBar;
    bool complete;
    
    // Variables para efectos fade
    bool fadeInComplete;
    bool fadeOutStarted;
    int fadeTimer;
    int fadeInDuration;
    int fadeOutDuration;
    float fadeAlpha;
    
    // Variables para fade entre slides
    bool slideFadeOut;
    bool slideFadeIn;
    int slideFadeTimer;
    int slideFadeDuration;
    float slideFadeAlpha;
    
    // Variable para progreso continuo
    float continuousProgress;
    
    GLuint textures[3]; // Texturas para los 3 slides
    TextureInfo textureInfos[3]; // Información de dimensiones originales
    
    // Cargar texture BMP
    TextureInfo loadBMPTexture(const char* filename) {
        TextureInfo info;
        info.id = 0;
        info.width = 0;
        info.height = 0;
        
        FILE* file;
        unsigned char header[54];
        unsigned int dataPos;
        unsigned int imageSize;
        unsigned int width, height;
        unsigned char* data;
        
        file = fopen(filename, "rb");
        if (!file) {
            std::cout << "Error: No se pudo abrir " << filename << std::endl;
            return info;
        }
        
        if (fread(header, 1, 54, file) != 54) {
            std::cout << "Error: Archivo BMP incorrecto" << std::endl;
            fclose(file);
            return info;
        }
        
        if (header[0] != 'B' || header[1] != 'M') {
            std::cout << "Error: No es un archivo BMP válido" << std::endl;
            fclose(file);
            return info;
        }
        
        dataPos = *(int*)&(header[0x0A]);
        imageSize = *(int*)&(header[0x22]);
        width = *(int*)&(header[0x12]);
        height = *(int*)&(header[0x16]);
        
        if (imageSize == 0) imageSize = width * height * 3;
        if (dataPos == 0) dataPos = 54;
        
        data = new unsigned char[imageSize];
        fread(data, 1, imageSize, file);
        fclose(file);
        
        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        
        delete[] data;
        
        info.id = textureID;
        info.width = (int)width;
        info.height = (int)height;
        
        return info;
    }
    
    void playTransitionSound() {
        switch(currentSlide) {
            case 0:
                PlaySound(TEXT("slide1.wav"), NULL, SND_FILENAME | SND_ASYNC);
                break;
            case 1:
                PlaySound(TEXT("slide2.wav"), NULL, SND_FILENAME | SND_ASYNC);
                break;
            case 2:
                PlaySound(TEXT("slide3.wav"), NULL, SND_FILENAME | SND_ASYNC);
                break;
        }
    }
    
public:
    LoadingStart() {
        currentSlide = 0;
        slideTimer = 0;
        slideDuration = 180; // ~3 segundos a 60 FPS
        progressBar = 0.0f;
        complete = false;
        
        // Inicializar efectos fade
        fadeInComplete = false;
        fadeOutStarted = false;
        fadeTimer = 0;
        fadeInDuration = 120; // 2 segundos fade in
        fadeOutDuration = 120; // 2 segundos fade out
        fadeAlpha = 0.0f;
        
        // Inicializar fade entre slides
        slideFadeOut = false;
        slideFadeIn = false;
        slideFadeTimer = 0;
        slideFadeDuration = 120; // 2 segundos para fade entre slides
        slideFadeAlpha = 1.0f;
        
        // Inicializar progreso continuo
        continuousProgress = 0.0f;
        
        // Cargar texturas
        textureInfos[0] = loadBMPTexture("slide1.bmp");
        textureInfos[1] = loadBMPTexture("slide2.bmp");
        textureInfos[2] = loadBMPTexture("slide3.bmp");
        
        textures[0] = textureInfos[0].id;
        textures[1] = textureInfos[1].id;
        textures[2] = textureInfos[2].id;
        
        // Habilitar blending para efectos fade
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        // Reproducir primer sonido (se reproducirá cuando termine el fade in)
        // playTransitionSound(); // Comentado para que no suene inmediatamente
    }
    
    ~LoadingStart() {
        glDeleteTextures(3, textures);
    }
    
    void update() {
        if (complete) return;
        
        // Calcular duración total de toda la secuencia
        float totalDuration = (float)(3 * slideDuration + 2 * slideFadeDuration * 2 + fadeOutDuration);
        
        // Fade in inicial
        if (!fadeInComplete) {
            fadeTimer++;
            fadeAlpha = (float)fadeTimer / (float)fadeInDuration;
            if (fadeAlpha >= 1.0f) {
                fadeAlpha = 1.0f;
                fadeInComplete = true;
                fadeTimer = 0;
                // Reproducir primer sonido después del fade in
                playTransitionSound();
            }
            return;
        }
        
        // Incrementar progreso continuo en cada frame
        continuousProgress += 1.0f / totalDuration;
        if (continuousProgress > 1.0f) continuousProgress = 1.0f;
        
        // Actualizar barra de progreso
        progressBar = continuousProgress;
        
        // Animación normal de slides
        if (!fadeOutStarted && !slideFadeOut && !slideFadeIn) {
            slideTimer++;
            
            if (slideTimer >= slideDuration) {
                // Iniciar fade out del slide actual
                slideFadeOut = true;
                slideFadeTimer = 0;
                slideTimer = 0;
            }
        }
        
        // Fade out entre slides
        if (slideFadeOut) {
            slideFadeTimer++;
            slideFadeAlpha = 1.0f - ((float)slideFadeTimer / (float)slideFadeDuration);
            
            if (slideFadeAlpha <= 0.0f) {
                slideFadeAlpha = 0.0f;
                slideFadeOut = false;
                slideFadeIn = true;
                slideFadeTimer = 0;
                currentSlide++;
                
                if (currentSlide >= 3) {
                    // Iniciar fade out final
                    fadeOutStarted = true;
                    fadeTimer = 0;
                    slideFadeIn = false;
                    return;
                }
                
                playTransitionSound();
            }
        }
        
        // Fade in entre slides
        if (slideFadeIn) {
            slideFadeTimer++;
            slideFadeAlpha = (float)slideFadeTimer / (float)slideFadeDuration;
            
            if (slideFadeAlpha >= 1.0f) {
                slideFadeAlpha = 1.0f;
                slideFadeIn = false;
                slideFadeTimer = 0;
            }
        }
        
        // Fade out final
        if (fadeOutStarted) {
            fadeTimer++;
            fadeAlpha = 1.0f - ((float)fadeTimer / (float)fadeOutDuration);
            
            if (fadeAlpha <= 0.0f) {
                fadeAlpha = 0.0f;
                complete = true;
                progressBar = 1.0f; // Asegurar que esté completa
            }
        }
    }
    
    void render() {
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Aplicar alpha solo para los slides (no para la barra de progreso)
        float slideAlpha = fadeAlpha * slideFadeAlpha;
        glColor4f(1.0f, 1.0f, 1.0f, slideAlpha);
        
        // Obtener dimensiones actuales de la ventana
        int viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        float windowWidth = (float)viewport[2];
        float windowHeight = (float)viewport[3];
        
        // Renderizar slide actual
        if (currentSlide < 3 && textures[currentSlide] != 0) {
            glBindTexture(GL_TEXTURE_2D, textures[currentSlide]);
            glEnable(GL_TEXTURE_2D);
            
            // Obtener dimensiones originales de la imagen
            float imgWidth = (float)textureInfos[currentSlide].width;
            float imgHeight = (float)textureInfos[currentSlide].height;
            
            // Calcular el factor de escala para ocupar 35% de la ventana (más grande)
            float maxDisplayWidth = windowWidth * 0.35f;
            float maxDisplayHeight = windowHeight * 0.35f;
            
            // Mantener aspect ratio original
            float scaleWidth = maxDisplayWidth / imgWidth;
            float scaleHeight = maxDisplayHeight / imgHeight;
            float scale = (scaleWidth < scaleHeight) ? scaleWidth : scaleHeight;
            
            // Dimensiones finales del logo
            float displayWidth = imgWidth * scale;
            float displayHeight = imgHeight * scale;
            
            // Centrar el logo en la pantalla
            float x = (windowWidth - displayWidth) / 2.0f;
            float y = (windowHeight - displayHeight) / 2.0f;
            
            glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 1.0f); glVertex2f(x, y);
                glTexCoord2f(1.0f, 1.0f); glVertex2f(x + displayWidth, y);
                glTexCoord2f(1.0f, 0.0f); glVertex2f(x + displayWidth, y + displayHeight);
                glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y + displayHeight);
            glEnd();
            
            glDisable(GL_TEXTURE_2D);
        }
        
        // Renderizar barra de progreso
        renderProgressBar();
        
        // Restaurar color
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    }
    
    void renderProgressBar() {
        // Obtener dimensiones actuales de la ventana
        int viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        float windowWidth = (float)viewport[2];
        float windowHeight = (float)viewport[3];
        
        // Barra más pequeña
        float barWidth = 200.0f;  // Reducido de 300 a 200
        float barHeight = 5.0f;   // Reducido de 8 a 5
        float barX = windowWidth / 2.0f - barWidth / 2.0f; // Centrado dinámico
        float barY = windowHeight - 35.0f; // Ajustado para la nueva altura
        
        // Alpha solo para fade inicial y final (no para fades entre slides)
        float progressBarAlpha = fadeAlpha;
        
        // Fondo de la barra (gris oscuro) con alpha
        glColor4f(0.2f, 0.2f, 0.2f, progressBarAlpha);
        glBegin(GL_QUADS);
            glVertex2f(barX - 1, barY - 1);
            glVertex2f(barX + barWidth + 1, barY - 1);
            glVertex2f(barX + barWidth + 1, barY + barHeight + 1);
            glVertex2f(barX - 1, barY + barHeight + 1);
        glEnd();
        
        // Barra de progreso (blanca) con alpha
        glColor4f(1.0f, 1.0f, 1.0f, progressBarAlpha);
        glBegin(GL_QUADS);
            glVertex2f(barX, barY);
            glVertex2f(barX + barWidth * progressBar, barY);
            glVertex2f(barX + barWidth * progressBar, barY + barHeight);
            glVertex2f(barX, barY + barHeight);
        glEnd();
    }
    
    bool isComplete() const {
        return complete;
    }
    
    // Función para saltar slides con tecla
    void skipSlide() {
        if (complete) return;
        
        // Si estamos en fade in inicial, completarlo
        if (!fadeInComplete) {
            fadeInComplete = true;
            fadeAlpha = 1.0f;
            fadeTimer = 0;
            playTransitionSound();
            return;
        }
        
        // Si estamos en fade out final, completarlo
        if (fadeOutStarted) {
            complete = true;
            fadeAlpha = 0.0f;
            progressBar = 1.0f;
            return;
        }
        
        // Si estamos en animación normal o fades entre slides, saltar al siguiente
        if (!fadeOutStarted) {
            // Avanzar al siguiente slide inmediatamente
            currentSlide++;
            
            // Actualizar barra de progreso y progreso continuo inmediatamente
            progressBar = (float)currentSlide / 3.0f;
            continuousProgress = progressBar;
            
            // Resetear todos los timers y estados de fade
            slideTimer = 0;
            slideFadeTimer = 0;
            slideFadeOut = false;
            slideFadeIn = false;
            slideFadeAlpha = 1.0f;
            
            // Verificar si hemos terminado todos los slides
            if (currentSlide >= 3) {
                // Iniciar fade out final
                fadeOutStarted = true;
                fadeTimer = 0;
                progressBar = 1.0f; // Asegurar que la barra esté completa
                continuousProgress = 1.0f;
                return;
            }
            
            // Reproducir sonido del nuevo slide
            playTransitionSound();
        }
    }
};

#endif
