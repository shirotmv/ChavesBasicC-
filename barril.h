#ifndef BARRIL_H
#define BARRIL_H

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>
#include <iostream>
#include "texture.h"

// Barrel Class - Simplified and focused on geometry
class Barril {
private:
    GLuint textureID;
    float height;
    float baseRadius;
    float midRadius;
    int segments;
    int rings;
    bool textureLoaded;

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

    // Draws a circle on the XZ plane with normals for shading
    void drawCircle(float radius, float y, int segments, bool top) {
        glBegin(GL_TRIANGLE_FAN);
        if (top) {
            glNormal3f(0.0f, 1.0f, 0.0f);
            if (textureLoaded) {
                glTexCoord2f(0.5f, 0.5f);
            }
            glVertex3f(0.0f, y, 0.0f);
        } else {
            glNormal3f(0.0f, -1.0f, 0.0f);
            if (textureLoaded) {
                glTexCoord2f(0.5f, 0.5f);
            }
            glVertex3f(0.0f, y, 0.0f);
        }

        for (int i = 0; i <= segments; i++) {
            float angle = 2.0f * static_cast<float>(M_PI) * i / segments;
            float x = radius * cos(angle);
            float z = radius * sin(angle);
            
            if (textureLoaded) {
                glTexCoord2f(0.5f + 0.5f * (x / radius), 0.5f + 0.5f * (z / radius));
            }
            glVertex3f(x, y, z);
        }
        glEnd();
    }

    // Draws the curved body of the barrel
    void drawBody() {
        if (textureLoaded) {
            TextureManager::bindTexture(textureID);
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
            resetGLState();
        } else {
            TextureManager::unbindTexture();
            // Set a nice brown wood color when no texture is loaded
            setMaterial(0.55f, 0.35f, 0.20f, 0.1f, 0.1f, 0.1f, 10.0f);
        }

        for (int i = 0; i < rings; i++) {
            glBegin(GL_QUAD_STRIP);
            for (int j = 0; j <= segments; j++) {
                for (int k = 0; k < 2; k++) {
                    int ring = i + k;
                    float y = -height / 2 + (height * ring) / rings;
                    float t = static_cast<float>(ring) / rings;
                    float curve = 1.0f - 4.0f * (t - 0.5f) * (t - 0.5f);
                    float r = baseRadius + (midRadius - baseRadius) * curve;
                    
                    float angle = 2.0f * static_cast<float>(M_PI) * j / segments;
                    float x = cos(angle);
                    float z = sin(angle);

                    glNormal3f(x, 0.0f, z);

                    if (textureLoaded) {
                        glTexCoord2f(static_cast<float>(j) / segments, t);
                    }
                    glVertex3f(r * x, y, r * z);
                }
            }
            glEnd();
        }
    }

    // Draws the metallic bands of the barrel
    void drawBands() {
        TextureManager::unbindTexture();
        
        // Set metallic material properties
        setMaterial(0.4f, 0.4f, 0.4f, 0.5f, 0.5f, 0.5f, 50.0f);

        float bandWidth = 0.1f;
        float bandPositions[3] = {-0.7f, 0.0f, 0.7f};

        for (int band = 0; band < 3; band++) {
            float y_center = bandPositions[band];
            float t = (y_center + height / 2) / height;
            float curve = 1.0f - 4.0f * (t - 0.5f) * (t - 0.5f);
            float radius = baseRadius + (midRadius - baseRadius) * curve;

            glBegin(GL_QUAD_STRIP);
            for (int i = 0; i <= segments; i++) {
                float angle = 2.0f * static_cast<float>(M_PI) * i / segments;
                float x_outer = (radius + 0.02f) * cos(angle);
                float z_outer = (radius + 0.02f) * sin(angle);

                glNormal3f(cos(angle), 0.0f, sin(angle));
                glVertex3f(x_outer, y_center + bandWidth / 2, z_outer);
                glVertex3f(x_outer, y_center - bandWidth / 2, z_outer);
            }
            glEnd();
        }
    }

    // Draws the top and bottom caps of the barrel
    void drawCaps() {
        if (textureLoaded) {
            TextureManager::bindTexture(textureID);
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
            resetGLState();
        } else {
            TextureManager::unbindTexture();
            // Darker wood color for caps
            setMaterial(0.45f, 0.25f, 0.15f, 0.1f, 0.1f, 0.1f, 10.0f);
        }

        // Top cap
        drawCircle(baseRadius, height / 2, segments, true);

        // Bottom cap
        drawCircle(baseRadius, -height / 2, segments, false);
    }

public:
    // Constructor with automatic texture loading
    Barril() : textureID(0), height(2.0f), baseRadius(0.6f), midRadius(0.8f), segments(32), rings(20), textureLoaded(false) {
        loadTexture("barriltexture.bmp");
    }

    // Constructor with custom texture path
    Barril(const char* texturePath) : textureID(0), height(2.0f), baseRadius(0.6f), midRadius(0.8f), segments(32), rings(20), textureLoaded(false) {
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
            std::cerr << "Failed to load barrel texture: " << texturePath << std::endl;
            std::cerr << "Make sure the texture file is in the same directory as the executable." << std::endl;
        }
    }

    // Reload texture
    void reloadTexture() {
        loadTexture("barriltexture.bmp");
    }

    // Check if texture is loaded
    bool isTextureLoaded() const {
        return textureLoaded;
    }

    // Setters for customization
    void setDimensions(float h, float baseR, float midR) {
        height = h;
        baseRadius = baseR;
        midRadius = midR;
    }

    void setDetail(int segs, int r) {
        segments = segs;
        rings = r;
    }

    // Render the complete barrel
    void render() {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glPushMatrix();

        resetGLState();

        drawBody();
        drawCaps();
        drawBands();

        glPopMatrix();
        glPopAttrib();
    }

    // Destructor to clean up OpenGL resources
    ~Barril() {
        TextureManager::deleteTexture(textureID);
    }
};

#endif // BARRIL_H
