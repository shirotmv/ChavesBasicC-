#ifndef ESCALERA_H
#define ESCALERA_H

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>
#include <cstdio>
#include "texture.h"

class Escalera {
private:
    float posX, posY, posZ;
    float rotationY;
    float scale;
    int numSteps;
    float stepWidth, stepHeight, stepDepth;
    GLuint textureID;
    bool textureLoaded;
    
    void drawTexturedCube(float x, float y, float z, float width, float height, float depth) {
        glPushMatrix();
        glTranslatef(x, y, z);
        glScalef(width, height, depth);
        
        // Habilitar texturas si están disponibles
        if (textureLoaded) {
            TextureManager::bindTexture(textureID);
        }
        
        glBegin(GL_QUADS);
        
        // Cara frontal
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
        
        // Cara trasera
        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, -0.5f);
        
        // Cara superior
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, 0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);
        
        // Cara inferior
        glNormal3f(0.0f, -1.0f, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, -0.5f, -0.5f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
        
        // Cara derecha
        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, -0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
        
        // Cara izquierda
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
        
        glEnd();
        
        if (textureLoaded) {
            TextureManager::unbindTexture();
        }
        
        glPopMatrix();
    }
    
    void drawSteps(float posX, float posY, float posZ) {
        // Color base para los escalones (se mezcla con la textura)
        glColor3f(1.0f, 1.0f, 1.0f); // Blanco para mostrar textura natural
        
        int numSteps = 8;
        float stepWidth = 4.0f;
        float stepHeight = 0.5f;  // Reducido para menor inclinación (más echada)
        float stepDepth = 1.0f;   // Aumentado para mayor profundidad (más echada)
        
        for(int i = 0; i < numSteps; i++) {
            float x = posX;
            float y = posY + i * stepHeight;
            float z = posZ - i * stepDepth;
            
            drawTexturedCube(x, y, z, stepWidth, stepHeight * 2, stepDepth);
        }
    }
    
    void drawRailing(float posX, float posY, float posZ) {
        // Color oscuro para el barandal
        glColor3f(0.3f, 0.3f, 0.3f);
        
        int numSteps = 8;
        float stepHeight = 0.5f;  // Ajustado para coincidir con los escalones
        float stepDepth = 1.0f;   // Ajustado para coincidir con los escalones
        float railingHeight = 2.0f;
        
        // Postes verticales del barandal izquierdo
        for(int i = 0; i < numSteps; i++) {
            float x = posX - 1.8f;
            float y = posY + i * stepHeight + railingHeight/2;
            float z = posZ - i * stepDepth;
            
            drawTexturedCube(x, y, z, 0.1f, railingHeight, 0.1f);
        }
        
        // Barandal horizontal izquierdo
        for(int i = 0; i < numSteps-1; i++) {
            float x = posX - 1.8f;
            float y = posY + i * stepHeight + railingHeight;
            float z = posZ - i * stepDepth - stepDepth/2;
            
            drawTexturedCube(x, y, z, 0.08f, 0.08f, stepDepth);
        }
        
        // Postes verticales del barandal derecho
        for(int i = 0; i < numSteps; i++) {
            float x = posX + 1.8f;
            float y = posY + i * stepHeight + railingHeight/2;
            float z = posZ - i * stepDepth;
            
            drawTexturedCube(x, y, z, 0.1f, railingHeight, 0.1f);
        }
        
        // Barandal horizontal derecho
        for(int i = 0; i < numSteps-1; i++) {
            float x = posX + 1.8f;
            float y = posY + i * stepHeight + railingHeight;
            float z = posZ - i * stepDepth - stepDepth/2;
            
            drawTexturedCube(x, y, z, 0.08f, 0.08f, stepDepth);
        }
    }

public:
    // Constructor
    Escalera() {
        posX = posY = posZ = 0.0f;
        rotationY = 0.0f;
        scale = 1.0f;
        numSteps = 8;
        stepWidth = 4.0f;
        stepHeight = 0.5f;  // Actualizado para menor inclinación (más echada)
        stepDepth = 1.0f;   // Actualizado para menor inclinación (más echada)
        textureID = 0;
        textureLoaded = false;
    }
    
    // Destructor
    ~Escalera() {
        if (textureLoaded) {
            TextureManager::deleteTexture(textureID);
        }
    }
    
    // Cargar textura
    bool loadTexture(const char* filename) {
        textureID = TextureManager::loadTexture(filename);
        textureLoaded = (textureID != 0);
        return textureLoaded;
    }
    
    // Inicializar con textura por defecto
    void initialize() {
        if (!textureLoaded) {
            loadTexture("escaleratexture.bmp");
        }
    }
    
    // Método principal para dibujar la escalera completa
    void render(float x = 0.0f, float y = 0.0f, float z = 0.0f) {
        posX = x; posY = y; posZ = z;
        rotationY = 0.0f; scale = 1.0f;
        
        // Asegurarse de que la textura esté cargada
        if (!textureLoaded) {
            initialize();
        }
        
        glPushMatrix();
        drawSteps(x, y, z);
        drawRailing(x, y, z);
        glPopMatrix();
    }
    
    // Método para dibujar la escalera con rotación
    void drawRotated(float x, float y, float z, float rotY) {
        posX = x; posY = y; posZ = z;
        rotationY = rotY; scale = 1.0f;
        
        if (!textureLoaded) {
            initialize();
        }
        
        glPushMatrix();
        glTranslatef(x, y, z);
        glRotatef(rotY, 0.0f, 1.0f, 0.0f);
        glTranslatef(-x, -y, -z);
        drawSteps(x, y, z);
        drawRailing(x, y, z);
        glPopMatrix();
    }
    
    // Método para dibujar escalera con escala personalizada
    void drawScaled(float x, float y, float z, float sc) {
        posX = x; posY = y; posZ = z;
        rotationY = 0.0f; scale = sc;
        
        if (!textureLoaded) {
            initialize();
        }
        
        glPushMatrix();
        glTranslatef(x, y, z);
        glScalef(sc, sc, sc);
        glTranslatef(-x, -y, -z);
        drawSteps(x, y, z);
        drawRailing(x, y, z);
        glPopMatrix();
    }
    
    // Método para dibujar escalera con rotación y escala
    void drawTransformed(float x, float y, float z, float rotY, float sc) {
        posX = x; posY = y; posZ = z;
        rotationY = rotY; scale = sc;
        
        if (!textureLoaded) {
            initialize();
        }
        
        glPushMatrix();
        glTranslatef(x, y, z);
        glRotatef(rotY, 0.0f, 1.0f, 0.0f);
        glScalef(sc, sc, sc);
        glTranslatef(-x, -y, -z);
        drawSteps(x, y, z);
        drawRailing(x, y, z);
        glPopMatrix();
    }
    
    // ========== SISTEMA DE COLISIONES ==========
    
    // Verificar si un punto está dentro de la escalera
    bool checkCollision(float x, float y, float z) {
        // Transformar coordenadas según rotación y escala
        float localX, localZ;
        transformToLocal(x, z, localX, localZ);
        
        // Verificar límites generales de la escalera
        float halfWidth = (stepWidth * scale) / 2.0f;
        float totalLength = numSteps * stepDepth * scale;
        float totalHeight = numSteps * stepHeight * scale;
        
        if (localX < posX - halfWidth || localX > posX + halfWidth) return false;
        if (localZ > posZ || localZ < posZ - totalLength) return false;
        if (y < posY || y > posY + totalHeight) return false;
        
        return true;
    }
    
    // Obtener la altura del escalón en una posición específica
    float getStepHeight(float x, float z) {
        float localX, localZ;
        transformToLocal(x, z, localX, localZ);
        
        // Calcular qué escalón corresponde a esta posición Z
        float distanceFromStart = posZ - localZ;
        int stepIndex = (int)(distanceFromStart / (stepDepth * scale));
        
        if (stepIndex < 0) stepIndex = 0;
        if (stepIndex >= numSteps) stepIndex = numSteps - 1;
        
        return posY + stepIndex * stepHeight * scale + (stepHeight * scale);
    }
    
    // Verificar colisión con barandales
    bool checkRailingCollision(float x, float y, float z, float radius = 0.3f) {
        float localX, localZ;
        transformToLocal(x, z, localX, localZ);
        
        float railingLeftX = posX - 1.8f * scale;
        float railingRightX = posX + 1.8f * scale;
        float totalLength = numSteps * stepDepth * scale;
        
        // Verificar colisión con barandal izquierdo
        if (localX >= railingLeftX - radius && localX <= railingLeftX + radius &&
            localZ <= posZ + radius && localZ >= posZ - totalLength - radius &&
            y >= posY && y <= posY + 2.0f * scale) {
            return true;
        }
        
        // Verificar colisión con barandal derecho
        if (localX >= railingRightX - radius && localX <= railingRightX + radius &&
            localZ <= posZ + radius && localZ >= posZ - totalLength - radius &&
            y >= posY && y <= posY + 2.0f * scale) {
            return true;
        }
        
        return false;
    }
    
    // Verificar si se puede caminar sobre la escalera
    bool canWalkOn(float x, float y, float z) {
        if (!checkCollision(x, y, z)) return false;
        
        float stepHeightAtPos = getStepHeight(x, z);
        return (y >= stepHeightAtPos - 0.5f && y <= stepHeightAtPos + 0.5f);
    }
    
    // Ajustar Y del jugador para que esté sobre el escalón correcto
    float adjustPlayerY(float x, float z, float currentY) {
        if (checkCollision(x, currentY, z)) {
            return getStepHeight(x, z);
        }
        return currentY;
    }
    
    // Verificar colisión AABB (Axis-Aligned Bounding Box)
    bool checkAABBCollision(float x, float y, float z, float width, float height, float depth) {
        float halfWidth = (stepWidth * scale) / 2.0f;
        float totalLength = numSteps * stepDepth * scale;
        float totalHeight = numSteps * stepHeight * scale;
        
        // AABB de la escalera
        float minX = posX - halfWidth;
        float maxX = posX + halfWidth;
        float minY = posY;
        float maxY = posY + totalHeight;
        float minZ = posZ - totalLength;
        float maxZ = posZ;
        
        // AABB del objeto
        float objMinX = x - width/2;
        float objMaxX = x + width/2;
        float objMinY = y;
        float objMaxY = y + height;
        float objMinZ = z - depth/2;
        float objMaxZ = z + depth/2;
        
        return (objMinX <= maxX && objMaxX >= minX &&
                objMinY <= maxY && objMaxY >= minY &&
                objMinZ <= maxZ && objMaxZ >= minZ);
    }
    
    // Función auxiliar para transformar coordenadas mundiales a locales
    void transformToLocal(float worldX, float worldZ, float& localX, float& localZ) {
        if (rotationY == 0.0f) {
            localX = worldX;
            localZ = worldZ;
            return;
        }
        
        // Rotar punto alrededor del centro de la escalera
        float rad = -rotationY * M_PI / 180.0f;
        float cos_r = cos(rad);
        float sin_r = sin(rad);
        
        float dx = worldX - posX;
        float dz = worldZ - posZ;
        
        localX = posX + dx * cos_r - dz * sin_r;
        localZ = posZ + dx * sin_r + dz * cos_r;
    }
    
    // Obtener información para depuración
    void getCollisionInfo(float x, float y, float z) {
        printf("Escalera en (%.1f, %.1f, %.1f)\n", posX, posY, posZ);
        printf("Punto testeo: (%.1f, %.1f, %.1f)\n", x, y, z);
        printf("Colision: %s\n", checkCollision(x, y, z) ? "SI" : "NO");
        printf("Altura escalon: %.2f\n", getStepHeight(x, z));
        printf("Puede caminar: %s\n", canWalkOn(x, y, z) ? "SI" : "NO");
        printf("Textura cargada: %s\n", textureLoaded ? "SI" : "NO");
    }
    
    // Método para obtener información de la escalera (actualizado)
    float getHeight() { return 8 * 0.5f; } // 8 escalones * nueva altura (4.0 unidades)
    float getLength() { return 8 * 1.0f; } // 8 escalones * nueva profundidad (8.0 unidades)
    float getWidth() { return 4.0f; }      // Ancho de la escalera
    
    // Método para verificar si la textura está cargada
    bool isTextureLoaded() { return textureLoaded; }
    
    // Método para cambiar la textura
    bool changeTexture(const char* filename) {
        if (textureLoaded) {
            TextureManager::deleteTexture(textureID);
        }
        return loadTexture(filename);
    }
};

#endif // ESCALERA_H
