#ifndef CASAJAIMITO_H
#define CASAJAIMITO_H

#include <GL/glut.h>
#include <cmath>

class CasaJaimito {
private:
    void drawBox(float width, float height, float depth) {
        glBegin(GL_QUADS);
        
        // Front face
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-width/2, -height/2, depth/2);
        glVertex3f(width/2, -height/2, depth/2);
        glVertex3f(width/2, height/2, depth/2);
        glVertex3f(-width/2, height/2, depth/2);
        
        // Back face
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(-width/2, -height/2, -depth/2);
        glVertex3f(-width/2, height/2, -depth/2);
        glVertex3f(width/2, height/2, -depth/2);
        glVertex3f(width/2, -height/2, -depth/2);
        
        // Top face
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-width/2, height/2, -depth/2);
        glVertex3f(-width/2, height/2, depth/2);
        glVertex3f(width/2, height/2, depth/2);
        glVertex3f(width/2, height/2, -depth/2);
        
        // Bottom face
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(-width/2, -height/2, -depth/2);
        glVertex3f(width/2, -height/2, -depth/2);
        glVertex3f(width/2, -height/2, depth/2);
        glVertex3f(-width/2, -height/2, depth/2);
        
        // Right face
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(width/2, -height/2, -depth/2);
        glVertex3f(width/2, height/2, -depth/2);
        glVertex3f(width/2, height/2, depth/2);
        glVertex3f(width/2, -height/2, depth/2);
        
        // Left face
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(-width/2, -height/2, -depth/2);
        glVertex3f(-width/2, -height/2, depth/2);
        glVertex3f(-width/2, height/2, depth/2);
        glVertex3f(-width/2, height/2, -depth/2);
        
        glEnd();
    }

    void setColor(float r, float g, float b) {
        glColor3f(r, g, b);
    }

    void drawWall(float width, float height, float depth) {
        this->drawBox(width, height, depth);
    }

    void drawDoor(float width, float height) {
        // Door frame (dark brown)
        setColor(0.4f, 0.2f, 0.1f);
        glPushMatrix();
        glTranslatef(0, 0, 0.02f);
        this->drawWall(width + 0.2f, height + 0.2f, 0.05f);
        glPopMatrix();
        
        // Door (brown)
        setColor(0.6f, 0.4f, 0.2f);
        glPushMatrix();
        glTranslatef(0, 0, 0.04f);
        this->drawWall(width, height, 0.04f);
        glPopMatrix();
        
        // Door handle
        setColor(0.8f, 0.8f, 0.2f);
        glPushMatrix();
        glTranslatef(width/3, 0, 0.06f);
        glutSolidSphere(0.05f, 8, 8);
        glPopMatrix();
        
        // Door lines
        setColor(0.3f, 0.2f, 0.1f);
        glLineWidth(3.0f);
        glBegin(GL_LINES);
        glNormal3f(0.0f, 0.0f, 1.0f);
        // Vertical center line
        glVertex3f(0, -height/2 + 0.1f, 0.05f);
        glVertex3f(0, height/2 - 0.1f, 0.05f);
        // Horizontal center line
        glVertex3f(-width/2 + 0.1f, 0, 0.05f);
        glVertex3f(width/2 - 0.1f, 0, 0.05f);
        glEnd();
    }

    void drawWindow(float width, float height) {
        // Window frame (brown)
        setColor(0.4f, 0.2f, 0.1f);
        glPushMatrix();
        glTranslatef(0, 0, 0.02f);
        this->drawWall(width + 0.1f, height + 0.1f, 0.05f);
        glPopMatrix();
        
        // Window glass (light blue)
        setColor(0.7f, 0.9f, 1.0f);
        glPushMatrix();
        glTranslatef(0, 0, 0.04f);
        this->drawWall(width, height, 0.02f);
        glPopMatrix();
        
        // Window divisions
        setColor(0.4f, 0.2f, 0.1f);
        glLineWidth(2.0f);
        glBegin(GL_LINES);
        glNormal3f(0.0f, 0.0f, 1.0f);
        // Vertical lines
        for(int i = 1; i < 3; i++) {
            float x = -width/2 + (width/3) * i;
            glVertex3f(x, -height/2, 0.05f);
            glVertex3f(x, height/2, 0.05f);
        }
        // Horizontal lines
        for(int i = 1; i < 3; i++) {
            float y = -height/2 + (height/3) * i;
            glVertex3f(-width/2, y, 0.05f);
            glVertex3f(width/2, y, 0.05f);
        }
        glEnd();
    }

    void drawRoof(float width, float depth, float height, float thickness) {
        setColor(0.8f, 0.2f, 0.2f); // Red for the roof
        glPushMatrix();
        glTranslatef(0, height - thickness / 2.0f, 0);
        this->drawBox(width, thickness, depth);
        glPopMatrix();
    }

public:
    CasaJaimito() {}
    ~CasaJaimito() {}

    void render() {
        glPushMatrix();
        
        // Main wall (yellow) - Front
        setColor(0.9f, 0.8f, 0.3f);
        glPushMatrix();
        glTranslatef(0, 1.5f, 1.5f);
        this->drawWall(4.0f, 3.0f, 0.1f);
        glPopMatrix();
        
        // Left wall (green)
        setColor(0.4f, 0.7f, 0.3f);
        glPushMatrix();
        glTranslatef(-2.0f, 1.5f, 0.0f);
        glRotatef(90, 0, 1, 0);
        this->drawWall(3.0f, 3.0f, 0.1f);
        glPopMatrix();
        
        // Right wall (pink)
        setColor(0.9f, 0.5f, 0.7f);
        glPushMatrix();
        glTranslatef(2.0f, 1.5f, 0.0f);
        glRotatef(-90, 0, 1, 0);
        this->drawWall(3.0f, 3.0f, 0.1f);
        glPopMatrix();
        
        // Back wall
        setColor(0.8f, 0.6f, 0.4f);
        glPushMatrix();
        glTranslatef(0, 1.5f, -1.5f);
        this->drawWall(4.0f, 3.0f, 0.1f);
        glPopMatrix();
        
        // Main door (on the front wall)
        glPushMatrix();
        glTranslatef(0, 1.0f, 1.55f);
        this->drawDoor(0.8f, 2.0f);
        glPopMatrix();

        // Window on the left wall
        glPushMatrix();
        glTranslatef(-2.05f, 1.8f, 0.5f);
        glRotatef(90, 0, 1, 0);
        this->drawWindow(1.0f, 1.0f);
        glPopMatrix();

        // Window on the right wall
        glPushMatrix();
        glTranslatef(2.05f, 1.8f, 0.5f);
        glRotatef(-90, 0, 1, 0);
        this->drawWindow(1.0f, 1.0f);
        glPopMatrix();
        
        // Floor/base
        setColor(0.5f, 0.5f, 0.5f);
        glPushMatrix();
        glTranslatef(0, -0.1f, 0.0f);
        this->drawBox(4.2f, 0.2f, 3.2f);
        glPopMatrix();
        
        // Flat roof
        setColor(0.8f, 0.2f, 0.2f);
        glPushMatrix();
        glTranslatef(0, 3.0f, 0.0f);
        this->drawRoof(4.2f, 3.2f, 0.2f, 0.2f);
        glPopMatrix();
        
        // Small decorative details
        // Pot next to the door
        setColor(0.6f, 0.3f, 0.1f);
        glPushMatrix();
        glTranslatef(-1.8f, 0.3f, 1.6f);
        glutSolidCube(0.3f);
        glPopMatrix();
        
        // Plant in the pot
        setColor(0.2f, 0.8f, 0.2f);
        glPushMatrix();
        glTranslatef(-1.8f, 0.6f, 1.6f);
        glutSolidSphere(0.15f, 8, 8);
        glPopMatrix();
        
        glPopMatrix();
    }
};

#endif // CASAJAIMITO_H
