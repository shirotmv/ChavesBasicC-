#ifndef PELOTA_H
#define PELOTA_H

#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>
#include <iostream>
#include "texture.h"

// Pelota Class with texture support
class Pelota {
private:
    GLuint textureID;
    float radius;
    int slices;
    int stacks;
    bool textureLoaded;
    
    // Default color (celeste)
    float defaultR, defaultG, defaultB;

    // Helper to set material properties
    void setMaterial(float r, float g, float b, float specular_r, float specular_g, float specular_b, float shininess) {
        GLfloat ambient[4] = {r * 0.3f, g * 0.3f, b * 0.3f, 1.0f};
        GLfloat diffuse[4] = {r, g, b, 1.0f};
        GLfloat specular[4] = {specular_r, specular_g, specular_b, 1.0f};
        GLfloat shini[1] = {shininess};

        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shini);
    }

    // Reset OpenGL state to neutral
    void resetGLState() {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        
        GLfloat white[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        GLfloat black[4] = {0.0f, 0.0f, 0.0f, 1.0f};
        GLfloat defaultAmbient[4] = {0.2f, 0.2f, 0.2f, 1.0f};
        GLfloat defaultShininess[1] = {0.0f};
        
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, defaultAmbient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, black);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, defaultShininess);
    }

    // Draw textured sphere manually for better texture mapping
    void drawTexturedSphere() {
        for (int i = 0; i < stacks; i++) {
            float phi1 = static_cast<float>(M_PI) * i / stacks;
            float phi2 = static_cast<float>(M_PI) * (i + 1) / stacks;
            
            glBegin(GL_QUAD_STRIP);
            for (int j = 0; j <= slices; j++) {
                float theta = 2.0f * static_cast<float>(M_PI) * j / slices;
                
                // First vertex
                float x1 = radius * sin(phi1) * cos(theta);
                float y1 = radius * cos(phi1);
                float z1 = radius * sin(phi1) * sin(theta);
                
                // Second vertex
                float x2 = radius * sin(phi2) * cos(theta);
                float y2 = radius * cos(phi2);
                float z2 = radius * sin(phi2) * sin(theta);
                
                // Texture coordinates
                float u = static_cast<float>(j) / slices;
                float v1 = static_cast<float>(i) / stacks;
                float v2 = static_cast<float>(i + 1) / stacks;
                
                // First vertex with normal and texture
                glNormal3f(x1/radius, y1/radius, z1/radius);
                glTexCoord2f(u, v1);
                glVertex3f(x1, y1, z1);
                
                // Second vertex with normal and texture
                glNormal3f(x2/radius, y2/radius, z2/radius);
                glTexCoord2f(u, v2);
                glVertex3f(x2, y2, z2);
            }
            glEnd();
        }
    }

public:
    // Constructor with automatic texture loading
    Pelota() : textureID(0), radius(1.0f), slices(50), stacks(50), textureLoaded(false),
               defaultR(0.3f), defaultG(0.5f), defaultB(0.8f) {
        loadTexture("pelotatexture.bmp");
    }

    // Constructor with custom texture path
    Pelota(const char* texturePath) : textureID(0), radius(1.0f), slices(50), stacks(50), textureLoaded(false),
                                      defaultR(0.3f), defaultG(0.5f), defaultB(0.8f) {
        loadTexture(texturePath);
    }

    // Constructor with custom radius
    Pelota(float r) : textureID(0), radius(r), slices(50), stacks(50), textureLoaded(false),
                      defaultR(0.3f), defaultG(0.5f), defaultB(0.8f) {
        loadTexture("pelotatexture.bmp");
    }

    // Constructor with custom radius and texture
    Pelota(float r, const char* texturePath) : textureID(0), radius(r), slices(50), stacks(50), textureLoaded(false),
                                               defaultR(0.3f), defaultG(0.5f), defaultB(0.8f) {
        loadTexture(texturePath);
    }

    // Load texture method
    void loadTexture(const char* texturePath) {
        if (textureID != 0) {
            TextureManager::deleteTexture(textureID);
        }
        
        textureID = TextureManager::loadTexture(texturePath);
        textureLoaded = (textureID != 0);
        
        if (!textureLoaded) {
            std::cerr << "Failed to load pelota texture: " << texturePath << std::endl;
            std::cerr << "Make sure the texture file is in the same directory as the executable." << std::endl;
        }
    }

    // Reload texture
    void reloadTexture() {
        loadTexture("pelotatexture.bmp");
    }

    // Check if texture is loaded
    bool isTextureLoaded() const {
        return textureLoaded;
    }

    // Set default color
    void setDefaultColor(float r, float g, float b) {
        defaultR = r;
        defaultG = g;
        defaultB = b;
    }

    // Set sphere detail
    void setDetail(int sl, int st) {
        slices = sl;
        stacks = st;
    }

    // Set radius
    void setRadius(float r) {
        radius = r;
    }

    // Get radius
    float getRadius() const {
        return radius;
    }

    // Render pelota with texture (if available) or default color
    void render() {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glPushMatrix();
        
        glEnable(GL_NORMALIZE);
        resetGLState();

        if (textureLoaded) {
            // Render with texture
            TextureManager::bindTexture(textureID);
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
            setMaterial(1.0f, 1.0f, 1.0f, 0.3f, 0.3f, 0.3f, 80.0f);
            drawTexturedSphere();
        } else {
            // Render with default color
            TextureManager::unbindTexture();
            setMaterial(defaultR, defaultG, defaultB, 1.0f, 1.0f, 1.0f, 80.0f);
            glutSolidSphere(radius, slices, stacks);
        }

        glPopMatrix();
        glPopAttrib();
    }

    // Render pelota with custom color (ignores texture)
    void renderWithColor(float r, float g, float b) {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glPushMatrix();
        
        glEnable(GL_NORMALIZE);
        resetGLState();
        TextureManager::unbindTexture();
        
        setMaterial(r, g, b, 1.0f, 1.0f, 1.0f, 80.0f);
        glutSolidSphere(radius, slices, stacks);

        glPopMatrix();
        glPopAttrib();
    }

    // Render pelota with custom radius and color
    void renderCustom(float customRadius, float r, float g, float b) {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glPushMatrix();
        
        glEnable(GL_NORMALIZE);
        resetGLState();
        TextureManager::unbindTexture();
        
        setMaterial(r, g, b, 1.0f, 1.0f, 1.0f, 80.0f);
        glutSolidSphere(customRadius, slices, stacks);

        glPopMatrix();
        glPopAttrib();
    }

    // Destructor to clean up OpenGL resources
    ~Pelota() {
        TextureManager::deleteTexture(textureID);
    }
};

// Funciones compatibles con el código legacy (mantienen la interfaz original)
namespace PelotaLegacy {
    // Función para configurar el material de la pelota
    void setPelotaMaterial(float r, float g, float b) {
        GLfloat customColor[] = { r, g, b, 1.0f };
        GLfloat white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, customColor);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 80.0f);
    }

    // Función principal para dibujar la pelota (color celeste por defecto)
    void DrawPelota() {
        glPushMatrix();
        glEnable(GL_NORMALIZE);
        setPelotaMaterial(0.3f, 0.5f, 0.8f);
        glutSolidSphere(1.0, 50, 50);
        glPopMatrix();
    }

    // Pelota con radio personalizado (color celeste)
    void DrawPelotaCustom(float radio) {
        glPushMatrix();
        glEnable(GL_NORMALIZE);
        setPelotaMaterial(0.3f, 0.5f, 0.8f);
        glutSolidSphere(radio, 50, 50);
        glPopMatrix();
    }

    // Pelota con color personalizado
    void DrawPelotaColor(float r, float g, float b) {
        glPushMatrix();
        glEnable(GL_NORMALIZE);
        setPelotaMaterial(r, g, b);
        glutSolidSphere(1.0, 50, 50);
        glPopMatrix();
    }

    // Pelota con color y radio personalizados
    void DrawPelotaCompleta(float radio, float r, float g, float b) {
        glPushMatrix();
        glEnable(GL_NORMALIZE);
        setPelotaMaterial(r, g, b);
        glutSolidSphere(radio, 50, 50);
        glPopMatrix();
    }
}

// Macros para compatibilidad directa (opcional)
#define DrawPelota() PelotaLegacy::DrawPelota()
#define DrawPelotaCustom(radio) PelotaLegacy::DrawPelotaCustom(radio)
#define DrawPelotaColor(r, g, b) PelotaLegacy::DrawPelotaColor(r, g, b)
#define DrawPelotaCompleta(radio, r, g, b) PelotaLegacy::DrawPelotaCompleta(radio, r, g, b)

#endif // PELOTA_H
