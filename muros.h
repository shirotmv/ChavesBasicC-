#ifndef MUROS_H
#define MUROS_H
#include "texture.h"

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

class Muros {
private:
    float portonAncho;
    float portonAltura;
        GLuint wallTextureID;  // AÑADIR ESTO
    GLuint doorTextureID;  // AÑADIR ESTO

public:
    // Constructor
    Muros(float ancho, float altura)
    : portonAncho(ancho), portonAltura(altura) {
    wallTextureID = TextureManager::loadTexture("walltexture.bmp");
        doorTextureID = TextureManager::loadTexture("doortexture.bmp");  // AÑADIR ESTO

}
    // Default constructor
    Muros() : portonAncho(3.0f), portonAltura(2.5f) {
    wallTextureID = TextureManager::loadTexture("walltexture.bmp");
        doorTextureID = TextureManager::loadTexture("doortexture.bmp");  // AÑADIR ESTO

}

// Destructor
~Muros() {
    TextureManager::deleteTexture(wallTextureID);
        TextureManager::deleteTexture(doorTextureID);  // AÑADIR ESTO

}
    // Function to draw a rectangular wall with 1/8 thickness
    void dibujarMuro(float x1, float y1, float z1, float x2, float y2, float z2, float altura) {
    float grosor = 0.125f;
    
    TextureManager::bindTexture(wallTextureID);  // AÑADIR ESTO
    
    glBegin(GL_QUADS);
    
    // Front face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x1, 0.0f, z1);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x2, 0.0f, z2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x2, altura, z2);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x1, altura, z1);
    
    // Back face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x1, 0.0f, z1 - grosor);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x1, altura, z1 - grosor);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x2, altura, z2 - grosor);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x2, 0.0f, z2 - grosor);
    
    // Top face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x1, altura, z1);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x2, altura, z2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x2, altura, z2 - grosor);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x1, altura, z1 - grosor);
    
    // Left side face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x1, 0.0f, z1);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x1, altura, z1);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x1, altura, z1 - grosor);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x1, 0.0f, z1 - grosor);
    
    // Right side face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x2, 0.0f, z2);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x2, 0.0f, z2 - grosor);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x2, altura, z2 - grosor);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x2, altura, z2);
    
    glEnd();
    
    TextureManager::unbindTexture();  // AÑADIR ESTO
}
    
    // Function to draw the wooden gate (maintains original size)
    void dibujarPorton(float x1, float y1, float z1, float x2, float y2, float z2, float altura) {
    // Detect if it's a side wall (vertical) or front wall (horizontal)
    bool esMuroLateral = (x1 == x2);
    
    // Bind door texture instead of setting color
    TextureManager::bindTexture(doorTextureID);  // AÑADIR ESTO EN LUGAR DE glColor3f
    
    if (esMuroLateral) {
        // For side wall, move the gate to the right (towards the end)
        float centroZ = z1 + (z2 - z1) * 0.75f;
        float inicioPorton = centroZ - portonAncho / 2.0f;
        float finPorton = centroZ + portonAncho / 2.0f;
        
        // Main gate
        glBegin(GL_QUADS);
        
        // Front face of the gate
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x1 - 0.1f, 0.0f, inicioPorton);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x1 - 0.1f, 0.0f, finPorton);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x1 - 0.1f, portonAltura, finPorton);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x1 - 0.1f, portonAltura, inicioPorton);
        
        // Back face of the gate
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x1 - 0.05f, 0.0f, inicioPorton);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x1 - 0.05f, portonAltura, inicioPorton);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x1 - 0.05f, portonAltura, finPorton);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x1 - 0.05f, 0.0f, finPorton);
        
        // Sides of the gate
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x1 - 0.1f, 0.0f, inicioPorton);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x1 - 0.05f, 0.0f, inicioPorton);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x1 - 0.05f, portonAltura, inicioPorton);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x1 - 0.1f, portonAltura, inicioPorton);
        
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x1 - 0.1f, 0.0f, finPorton);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x1 - 0.1f, portonAltura, finPorton);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x1 - 0.05f, portonAltura, finPorton);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x1 - 0.05f, 0.0f, finPorton);
        
        glEnd();
        
        // Gate details (horizontal slats for side wall)
        dibujarListonesLateral(x1, inicioPorton, finPorton);
        
        // Gate handle
        dibujarManija(x1 - 0.12f, portonAltura / 2.0f, finPorton - 0.3f);
        
    } else {
        // For front wall (horizontal), move the gate to the right
        float centroX = x1 + (x2 - x1) * 0.75f;
        float inicioPorton = centroX - portonAncho / 2.0f;
        float finPorton = centroX + portonAncho / 2.0f;
        
        // Main gate
        glBegin(GL_QUADS);
        
        // Front face of the gate
        glTexCoord2f(0.0f, 0.0f); glVertex3f(inicioPorton, 0.0f, z1 + 0.1f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(finPorton, 0.0f, z2 + 0.1f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(finPorton, portonAltura, z2 + 0.1f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(inicioPorton, portonAltura, z1 + 0.1f);
        
        // Back face of the gate
        glTexCoord2f(0.0f, 0.0f); glVertex3f(inicioPorton, 0.0f, z1 + 0.05f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(inicioPorton, portonAltura, z1 + 0.05f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(finPorton, portonAltura, z2 + 0.05f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(finPorton, 0.0f, z2 + 0.05f);
        
        // Sides of the gate
        glTexCoord2f(0.0f, 0.0f); glVertex3f(inicioPorton, 0.0f, z1 + 0.05f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(inicioPorton, 0.0f, z1 + 0.1f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(inicioPorton, portonAltura, z1 + 0.1f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(inicioPorton, portonAltura, z1 + 0.05f);
        
        glTexCoord2f(0.0f, 0.0f); glVertex3f(finPorton, 0.0f, z2 + 0.05f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(finPorton, portonAltura, z2 + 0.05f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(finPorton, portonAltura, z2 + 0.1f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(finPorton, 0.0f, z2 + 0.1f);
        
        glEnd();
        
        // Gate details (vertical slats)

        
        // Gate handle
        dibujarManija(finPorton - 0.3f, portonAltura / 2.0f, z1 + 0.12f);
    }
    
    TextureManager::unbindTexture();  // AÑADIR ESTO AL FINAL
}
    
    // Function to draw a wall with an opening for the gate
    void dibujarMuroConPorton(float x1, float y1, float z1, float x2, float y2, float z2, float altura) {
        // Opaque yellow color for the wall
        glColor3f(0.8f, 0.8f, 0.0f);
        
        // Detect if it's a side wall (vertical) or front wall (horizontal)
        bool esMuroLateral = (x1 == x2); // If x1 == x2, it's a side wall
        
        if (esMuroLateral) {
            // For side wall (left), move the gate to the right (towards the end)
            float centroZ = z1 + (z2 - z1) * 0.75f;  // 75% towards the end
            float inicioPorton = centroZ - portonAncho / 2.0f;
            float finPorton = centroZ + portonAncho / 2.0f;
            
            // Bottom part of the wall (before the gate)
            if (inicioPorton > z1) {
                dibujarMuro(x1, y1, z1, x2, y2, inicioPorton, altura);
            }
            
            // Top part of the wall (after the gate)
            if (finPorton < z2) {
                dibujarMuro(x1, y1, finPorton, x2, y2, z2, altura);
            }
            
            // Top part of the gate (lintel)
            dibujarDintelLateral(x1, x2, inicioPorton, finPorton, altura);
            
        } else {
            // For front wall (horizontal), move the gate to the right
            float centroX = x1 + (x2 - x1) * 0.75f;  // 75% to the right
            float inicioPorton = centroX - portonAncho / 2.0f;
            float finPorton = centroX + portonAncho / 2.0f;
            
            // Left part of the wall
            if (inicioPorton > x1) {
                dibujarMuro(x1, y1, z1, inicioPorton, y2, z2, altura);
            }
            
            // Right part of the wall
            if (finPorton < x2) {
                dibujarMuro(finPorton, y1, z1, x2, y2, z2, altura);
            }
            
            // Top part of the gate (lintel)
            dibujarDintelFrontal(inicioPorton, finPorton, z1, z2, altura);
        }
    }
    

    // Setters to modify gate dimensions
    void setPortonDimensiones(float ancho, float altura) {
        portonAncho = ancho;
        portonAltura = altura;
    }
    
    // Getters
    float getPortonAncho() const { return portonAncho; }
    float getPortonAltura() const { return portonAltura; }

    // Simple render function - easy to call from world.h (much longer house)
    void render() {
        // Much longer rectangular enclosure with gate on front wall
        renderEnclosure(20.0f, 50.0f, 6.0f, 1); // 20x50 enclosure, 6m height, gate on wall 1
    }
    
    // Render function with parameters for customization (increased default dimensions)
    void renderEnclosure(float width, float depth, float height, int gateWallIndex) {
        // Calculate wall positions for a rectangular enclosure
        float halfWidth = width / 2.0f;
        float halfDepth = depth / 2.0f;
        

        // Wall 1 - Front wall (facing positive Z)
        if (gateWallIndex == 1) {
            dibujarMuroConPorton(-halfWidth, 0.0f, halfDepth, halfWidth, 0.0f, halfDepth, height);
            dibujarPorton(-halfWidth, 0.0f, halfDepth, halfWidth, 0.0f, halfDepth, height);
        } else {
            glColor3f(0.8f, 0.8f, 0.0f);
            dibujarMuro(-halfWidth, 0.0f, halfDepth, halfWidth, 0.0f, halfDepth, height);
        }

        // Wall 2 - Right wall
        if (gateWallIndex == 2) {
            dibujarMuroConPorton(halfWidth, 0.0f, halfDepth, halfWidth, 0.0f, -halfDepth, height);
            dibujarPorton(halfWidth, 0.0f, halfDepth, halfWidth, 0.0f, -halfDepth, height);
        } else {
            glColor3f(0.8f, 0.8f, 0.0f);
            dibujarMuro(halfWidth, 0.0f, halfDepth, halfWidth, 0.0f, -halfDepth, height);
        }
        
        // Wall 3 - Back wall (facing negative Z)
        if (gateWallIndex == 3) {
            dibujarMuroConPorton(halfWidth, 0.0f, -halfDepth, -halfWidth, 0.0f, -halfDepth, height);
            dibujarPorton(halfWidth, 0.0f, -halfDepth, -halfWidth, 0.0f, -halfDepth, height);
        } else {
            glColor3f(0.8f, 0.8f, 0.0f);
            dibujarMuro(halfWidth, 0.0f, -halfDepth, -halfWidth, 0.0f, -halfDepth, height);
        }

        // Wall 4 - Left wall
        if (gateWallIndex == 4) {
            dibujarMuroConPorton(-halfWidth, 0.0f, -halfDepth, -halfWidth, 0.0f, halfDepth, height);
            dibujarPorton(-halfWidth, 0.0f, -halfDepth, -halfWidth, 0.0f, halfDepth, height);
        } else {
            glColor3f(0.8f, 0.8f, 0.0f);
            dibujarMuro(-halfWidth, 0.0f, -halfDepth, -halfWidth, 0.0f, halfDepth, height);
        }
    }

    // Advanced render function with full customization (the original one)
    void renderCustom(float wall1_x1, float wall1_y1, float wall1_z1, float wall1_x2, float wall1_y2, float wall1_z2, float wall1_height,
                      float wall2_x1, float wall2_y1, float wall2_z1, float wall2_x2, float wall2_y2, float wall2_z2, float wall2_height,
                      float wall3_x1, float wall3_y1, float wall3_z1, float wall3_x2, float wall3_y2, float wall3_z2, float wall3_height,
                      float wall4_x1, float wall4_y1, float wall4_z1, float wall4_x2, float wall4_y2, float wall4_z2, float wall4_height,
                      int gateWallIndex) { // Index 1-4 to specify which wall has the gate
        

        // Wall 1
        if (gateWallIndex == 1) {
            dibujarMuroConPorton(wall1_x1, wall1_y1, wall1_z1, wall1_x2, wall1_y2, wall1_z2, wall1_height);
            dibujarPorton(wall1_x1, wall1_y1, wall1_z1, wall1_x2, wall1_y2, wall1_z2, wall1_height);
        } else {
            glColor3f(0.8f, 0.8f, 0.0f); // Reset color for regular walls
            dibujarMuro(wall1_x1, wall1_y1, wall1_z1, wall1_x2, wall1_y2, wall1_z2, wall1_height);
        }

        // Wall 2
        if (gateWallIndex == 2) {
            dibujarMuroConPorton(wall2_x1, wall2_y1, wall2_z1, wall2_x2, wall2_y2, wall2_z2, wall2_height);
            dibujarPorton(wall2_x1, wall2_y1, wall2_z1, wall2_x2, wall2_y2, wall2_z2, wall2_height);
        } else {
            glColor3f(0.8f, 0.8f, 0.0f);
            dibujarMuro(wall2_x1, wall2_y1, wall2_z1, wall2_x2, wall2_y2, wall2_z2, wall2_height);
        }
        
        // Wall 3
        if (gateWallIndex == 3) {
            dibujarMuroConPorton(wall3_x1, wall3_y1, wall3_z1, wall3_x2, wall3_y2, wall3_z2, wall3_height);
            dibujarPorton(wall3_x1, wall3_y1, wall3_z1, wall3_x2, wall3_y2, wall3_z2, wall3_height);
        } else {
            glColor3f(0.8f, 0.8f, 0.0f);
            dibujarMuro(wall3_x1, wall3_y1, wall3_z1, wall3_x2, wall3_y2, wall3_z2, wall3_height);
        }

        // Wall 4
        if (gateWallIndex == 4) {
            dibujarMuroConPorton(wall4_x1, wall4_y1, wall4_z1, wall4_x2, wall4_y2, wall4_z2, wall4_height);
            dibujarPorton(wall4_x1, wall4_y1, wall4_z1, wall4_x2, wall4_y2, wall4_z2, wall4_height);
        } else {
            glColor3f(0.8f, 0.8f, 0.0f);
            dibujarMuro(wall4_x1, wall4_y1, wall4_z1, wall4_x2, wall4_y2, wall4_z2, wall4_height);
        }
    }

private:
    // Auxiliary private functions
    void dibujarListonesLateral(float x, float inicioPorton, float finPorton) {
        glColor3f(0.4f, 0.2f, 0.0f);
        float numListones = 8;
        float espacioListones = portonAncho / numListones;
        
        for (int i = 0; i < numListones; i++) {
            float zListon = inicioPorton + i * espacioListones + espacioListones * 0.1f;
            float anchoListon = espacioListones * 0.8f;
            
            glBegin(GL_QUADS);
            glVertex3f(x - 0.11f, 0.2f, zListon);
            glVertex3f(x - 0.11f, 0.2f, zListon + anchoListon);
            glVertex3f(x - 0.11f, portonAltura - 0.2f, zListon + anchoListon);
            glVertex3f(x - 0.11f, portonAltura - 0.2f, zListon);
            glEnd();
        }
    }
    
    void dibujarListonesFrontal(float inicioPorton, float finPorton, float z) {
        glColor3f(0.4f, 0.2f, 0.0f);
        float numListones = 8;
        float espacioListones = portonAncho / numListones;
        
        for (int i = 0; i < numListones; i++) {
            float xListon = inicioPorton + i * espacioListones + espacioListones * 0.1f;
            float anchoListon = espacioListones * 0.8f;
            
            glBegin(GL_QUADS);
            glVertex3f(xListon, 0.2f, z);
            glVertex3f(xListon + anchoListon, 0.2f, z);
            glVertex3f(xListon + anchoListon, portonAltura - 0.2f, z);
            glVertex3f(xListon, portonAltura - 0.2f, z);
            glEnd();
        }
    }
    
    void dibujarManija(float x, float y, float z) {
        glColor3f(0.8f, 0.7f, 0.2f); // Gold color
        glBegin(GL_QUADS);
        glVertex3f(x - 0.05f, y - 0.05f, z - 0.05f);
        glVertex3f(x + 0.05f, y - 0.05f, z + 0.05f);
        glVertex3f(x + 0.05f, y + 0.05f, z + 0.05f);
        glVertex3f(x - 0.05f, y + 0.05f, z - 0.05f);
        glEnd();
    }
    
    void dibujarDintelLateral(float x1, float x2, float inicioPorton, float finPorton, float altura) {
    float grosor = 0.125f;
    
    TextureManager::bindTexture(wallTextureID);  // AÑADIR ESTO
    
    glBegin(GL_QUADS);
    // Front top face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x1, portonAltura, inicioPorton);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x2, portonAltura, finPorton);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x2, altura, finPorton);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x1, altura, inicioPorton);
    
    // Back top face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x1, portonAltura, inicioPorton - grosor);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x1, altura, inicioPorton - grosor);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x2, altura, finPorton - grosor);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x2, portonAltura, finPorton - grosor);
    
    // Top face of the lintel
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x1, altura, inicioPorton);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x2, altura, finPorton);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x2, altura, finPorton - grosor);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x1, altura, inicioPorton - grosor);
    
    // Bottom face of the lintel
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x1, portonAltura, inicioPorton);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x1, portonAltura, inicioPorton - grosor);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x2, portonAltura, finPorton - grosor);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x2, portonAltura, finPorton);
    
    glEnd();
    
    TextureManager::unbindTexture();  // AÑADIR ESTO
}
    
    void dibujarDintelFrontal(float inicioPorton, float finPorton, float z1, float z2, float altura) {
    float grosor = 0.125f;
    
    TextureManager::bindTexture(wallTextureID);  // AÑADIR ESTO
    
    glBegin(GL_QUADS);
    // Front top face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(inicioPorton, portonAltura, z1);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(finPorton, portonAltura, z2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(finPorton, altura, z2);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(inicioPorton, altura, z1);
    
    // Back top face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(inicioPorton, portonAltura, z1 - grosor);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(inicioPorton, altura, z1 - grosor);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(finPorton, altura, z2 - grosor);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(finPorton, portonAltura, z2 - grosor);
    
    // Top face of the lintel
    glTexCoord2f(0.0f, 0.0f); glVertex3f(inicioPorton, altura, z1);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(finPorton, altura, z2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(finPorton, altura, z2 - grosor);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(inicioPorton, altura, z1 - grosor);
    
    // Bottom face of the lintel
    glTexCoord2f(0.0f, 0.0f); glVertex3f(inicioPorton, portonAltura, z1);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(inicioPorton, portonAltura, z1 - grosor);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(finPorton, portonAltura, z2 - grosor);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(finPorton, portonAltura, z2);
    
    // Side faces of the lintel
    glTexCoord2f(0.0f, 0.0f); glVertex3f(inicioPorton, portonAltura, z1);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(inicioPorton, altura, z1);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(inicioPorton, altura, z1 - grosor);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(inicioPorton, portonAltura, z1 - grosor);
    
    glTexCoord2f(0.0f, 0.0f); glVertex3f(finPorton, portonAltura, z2);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(finPorton, portonAltura, z2 - grosor);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(finPorton, altura, z2 - grosor);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(finPorton, altura, z2);
    
    glEnd();
    
    TextureManager::unbindTexture();  // AÑADIR ESTO
}
    
};

#endif // MUROS_H
