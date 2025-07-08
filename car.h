#ifndef CAR_H
#define CAR_H

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>

class Car {
private:
    float wheelRotation;
    
    // Función para dibujar un cubo suavizado
    void drawSmoothCube(float width, float height, float depth) {
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
    
    // Función para dibujar una rueda más detallada
    void drawWheel(float radius, float width) {
        glPushMatrix();
        
        // Llanta exterior (negra)
        glColor3f(0.05f, 0.05f, 0.05f);
        GLUquadricObj* quadric = gluNewQuadric();
        gluQuadricNormals(quadric, GLU_SMOOTH);
        gluCylinder(quadric, radius, radius, width, 24, 1);
        
        // Rin interior (plateado brillante)
        glColor3f(0.9f, 0.9f, 0.95f);
        gluDisk(quadric, 0, radius * 0.75f, 24, 1);
        
        glTranslatef(0, 0, width);
        gluDisk(quadric, 0, radius * 0.75f, 24, 1);
        
        // Detalles del rin
        glColor3f(0.7f, 0.7f, 0.8f);
        glTranslatef(0, 0, -width/2);
        
        // Rayos del rin
        for(int i = 0; i < 5; i++) {
            glPushMatrix();
            glRotatef(i * 72.0f, 0, 0, 1);
            glTranslatef(0, radius * 0.4f, 0);
            glutSolidSphere(0.02f, 8, 8);
            glPopMatrix();
        }
        
        // Centro del rin
        glColor3f(0.6f, 0.6f, 0.7f);
        glutSolidSphere(0.08f, 12, 12);
        
        gluDeleteQuadric(quadric);
        glPopMatrix();
    }
    
    // Función para dibujar ventanas con marco
    void drawWindows() {
        // Marco de ventanas (negro)
        glColor3f(0.1f, 0.1f, 0.1f);
        
        // Parabrisas delantero
        glPushMatrix();
        glTranslatef(0, 0.35f, 1.45f);
        glRotatef(-15, 1, 0, 0);
        glScalef(1.5f, 0.7f, 0.05f);
        drawSmoothCube(1, 1, 1);
        glPopMatrix();
        
        // Cristal del parabrisas
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.7f, 0.9f, 1.0f, 0.3f);
        
        glPushMatrix();
        glTranslatef(0, 0.35f, 1.48f);
        glRotatef(-15, 1, 0, 0);
        glScalef(1.4f, 0.65f, 0.02f);
        drawSmoothCube(1, 1, 1);
        glPopMatrix();
        
        // Ventana trasera
        glColor3f(0.1f, 0.1f, 0.1f);
        glPushMatrix();
        glTranslatef(0, 0.35f, -1.45f);
        glRotatef(20, 1, 0, 0);
        glScalef(1.5f, 0.7f, 0.05f);
        drawSmoothCube(1, 1, 1);
        glPopMatrix();
        
        glColor4f(0.7f, 0.9f, 1.0f, 0.3f);
        glPushMatrix();
        glTranslatef(0, 0.35f, -1.48f);
        glRotatef(20, 1, 0, 0);
        glScalef(1.4f, 0.65f, 0.02f);
        drawSmoothCube(1, 1, 1);
        glPopMatrix();
        
        // Ventanas laterales
        glColor3f(0.1f, 0.1f, 0.1f);
        glPushMatrix();
        glTranslatef(0.82f, 0.35f, 0);
        glScalef(0.05f, 0.7f, 2.2f);
        drawSmoothCube(1, 1, 1);
        glPopMatrix();
        
        glColor4f(0.7f, 0.9f, 1.0f, 0.3f);
        glPushMatrix();
        glTranslatef(0.85f, 0.35f, 0);
        glScalef(0.02f, 0.65f, 2.1f);
        drawSmoothCube(1, 1, 1);
        glPopMatrix();
        
        glColor3f(0.1f, 0.1f, 0.1f);
        glPushMatrix();
        glTranslatef(-0.82f, 0.35f, 0);
        glScalef(0.05f, 0.7f, 2.2f);
        drawSmoothCube(1, 1, 1);
        glPopMatrix();
        
        glColor4f(0.7f, 0.9f, 1.0f, 0.3f);
        glPushMatrix();
        glTranslatef(-0.85f, 0.35f, 0);
        glScalef(0.02f, 0.65f, 2.1f);
        drawSmoothCube(1, 1, 1);
        glPopMatrix();
        
        glDisable(GL_BLEND);
    }
    
    // Función para dibujar detalles de la carrocería
    void drawCarBody() {
        // Color principal amarillo Brasilia
        glColor3f(1.0f, 0.85f, 0.0f);
        
        // Carrocería principal
        glPushMatrix();
        glTranslatef(0, -0.15f, 0);
        glScalef(1.7f, 0.7f, 2.8f);
        drawSmoothCube(1, 1, 1);
        glPopMatrix();
        
        // Techo
        glPushMatrix();
        glTranslatef(0, 0.55f, 0);
        glScalef(1.5f, 0.5f, 2.4f);
        drawSmoothCube(1, 1, 1);
        glPopMatrix();
        
        // Capó
        glPushMatrix();
        glTranslatef(0, 0.05f, 1.6f);
        glScalef(1.7f, 0.35f, 0.5f);
        drawSmoothCube(1, 1, 1);
        glPopMatrix();
        
        // Parte trasera
        glPushMatrix();
        glTranslatef(0, 0.05f, -1.6f);
        glScalef(1.7f, 0.35f, 0.5f);
        drawSmoothCube(1, 1, 1);
        glPopMatrix();
        
        // Línea de cintura
        glColor3f(0.8f, 0.7f, 0.0f);
        glPushMatrix();
        glTranslatef(0, 0.0f, 0);
        glScalef(1.72f, 0.05f, 2.82f);
        drawSmoothCube(1, 1, 1);
        glPopMatrix();
        
        // Rejilla delantera
        glColor3f(0.2f, 0.2f, 0.2f);
        glPushMatrix();
        glTranslatef(0, -0.1f, 1.88f);
        glScalef(0.8f, 0.3f, 0.05f);
        drawSmoothCube(1, 1, 1);
        glPopMatrix();
        
        // Líneas de la rejilla
        glColor3f(0.9f, 0.9f, 0.9f);
        for(int i = -2; i <= 2; i++) {
            glPushMatrix();
            glTranslatef(i * 0.15f, -0.1f, 1.9f);
            glScalef(0.02f, 0.25f, 0.02f);
            drawSmoothCube(1, 1, 1);
            glPopMatrix();
        }
    }
    
    // Función para dibujar parachoques
    void drawBumpers() {
        // Parachoques delantero
        glColor3f(0.15f, 0.15f, 0.15f);
        glPushMatrix();
        glTranslatef(0, -0.45f, 2.0f);
        glScalef(1.8f, 0.15f, 0.15f);
        drawSmoothCube(1, 1, 1);
        glPopMatrix();
        
        // Parachoques trasero
        glPushMatrix();
        glTranslatef(0, -0.45f, -2.0f);
        glScalef(1.8f, 0.15f, 0.15f);
        drawSmoothCube(1, 1, 1);
        glPopMatrix();
        
        // Detalles cromados
        glColor3f(0.8f, 0.8f, 0.9f);
        glPushMatrix();
        glTranslatef(0, -0.45f, 2.02f);
        glScalef(1.6f, 0.08f, 0.08f);
        drawSmoothCube(1, 1, 1);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(0, -0.45f, -2.02f);
        glScalef(1.6f, 0.08f, 0.08f);
        drawSmoothCube(1, 1, 1);
        glPopMatrix();
    }
    
    // Función para dibujar faros
    void drawLights() {
        // Faros delanteros
        glColor3f(1.0f, 1.0f, 0.95f);
        glPushMatrix();
        glTranslatef(-0.6f, 0.0f, 1.92f);
        glutSolidSphere(0.16f, 16, 16);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(0.6f, 0.0f, 1.92f);
        glutSolidSphere(0.16f, 16, 16);
        glPopMatrix();
        
        // Reflectores de los faros
        glColor3f(0.9f, 0.9f, 0.9f);
        glPushMatrix();
        glTranslatef(-0.6f, 0.0f, 1.9f);
        glutSolidSphere(0.1f, 12, 12);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(0.6f, 0.0f, 1.9f);
        glutSolidSphere(0.1f, 12, 12);
        glPopMatrix();
        
        // Luces traseras
        glColor3f(0.8f, 0.1f, 0.1f);
        glPushMatrix();
        glTranslatef(-0.6f, 0.0f, -1.92f);
        glutSolidSphere(0.1f, 12, 12);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(0.6f, 0.0f, -1.92f);
        glutSolidSphere(0.1f, 12, 12);
        glPopMatrix();
        
        // Luces de freno
        glColor3f(1.0f, 0.2f, 0.2f);
        glPushMatrix();
        glTranslatef(-0.4f, 0.0f, -1.95f);
        glutSolidSphere(0.06f, 10, 10);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(0.4f, 0.0f, -1.95f);
        glutSolidSphere(0.06f, 10, 10);
        glPopMatrix();
    }

public:
    // Constructor
    Car() {
        wheelRotation = 0.0f;
    }
    
    // Función para animar las ruedas
    void animateWheels() {
        wheelRotation += 3.0f;
        if(wheelRotation > 360.0f) wheelRotation -= 360.0f;
    }
    
    // Función para dibujar el carro completo
    void draw() {
        glPushMatrix();
        
        // Cuerpo principal
        drawCarBody();
        
        // Parachoques
        drawBumpers();
        
        // Ventanas
        drawWindows();
        
        // Faros y luces
        drawLights();
        
        // Ruedas con animación
        // Rueda delantera izquierda
        glPushMatrix();
        glTranslatef(-0.9f, -0.65f, 1.2f);
        glRotatef(90, 0, 1, 0);
        glRotatef(wheelRotation, 0, 0, 1);
        drawWheel(0.35f, 0.18f);
        glPopMatrix();
        
        // Rueda delantera derecha
        glPushMatrix();
        glTranslatef(0.9f, -0.65f, 1.2f);
        glRotatef(-90, 0, 1, 0);
        glRotatef(-wheelRotation, 0, 0, 1);
        drawWheel(0.35f, 0.18f);
        glPopMatrix();
        
        // Rueda trasera izquierda
        glPushMatrix();
        glTranslatef(-0.9f, -0.65f, -1.2f);
        glRotatef(90, 0, 1, 0);
        glRotatef(wheelRotation, 0, 0, 1);
        drawWheel(0.35f, 0.18f);
        glPopMatrix();
        
        // Rueda trasera derecha
        glPushMatrix();
        glTranslatef(0.9f, -0.65f, -1.2f);
        glRotatef(-90, 0, 1, 0);
        glRotatef(-wheelRotation, 0, 0, 1);
        drawWheel(0.35f, 0.18f);
        glPopMatrix();
        
        // Manijas de puertas
        glColor3f(0.8f, 0.8f, 0.9f);
        glPushMatrix();
        glTranslatef(0.85f, 0.0f, 0.5f);
        glScalef(0.02f, 0.05f, 0.15f);
        drawSmoothCube(1, 1, 1);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(-0.85f, 0.0f, 0.5f);
        glScalef(0.02f, 0.05f, 0.15f);
        drawSmoothCube(1, 1, 1);
        glPopMatrix();
        
        // Espejos laterales
        glColor3f(0.1f, 0.1f, 0.1f);
        glPushMatrix();
        glTranslatef(0.92f, 0.3f, 1.0f);
        glutSolidSphere(0.06f, 8, 8);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(-0.92f, 0.3f, 1.0f);
        glutSolidSphere(0.06f, 8, 8);
        glPopMatrix();
        
        glPopMatrix();
    }
    
    // Función para dibujar el carro en una posición específica
    void drawAt(float x, float y, float z, float angle) {
        glPushMatrix();
        glTranslatef(x, y, z);
        glRotatef(angle, 0, 1, 0);
        draw();
        glPopMatrix();
    }
};

#endif // CAR_H
