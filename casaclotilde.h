#ifndef CASA_CLOTILDE_H
#define CASA_CLOTILDE_H

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

class CasaClotilde {
private:
    // Funci?n para dibujar un cubo con textura de color
    void drawColoredCube(float x, float y, float z, float width, float height, float depth, 
                         float r, float g, float b) {
        glPushMatrix();
        glTranslatef(x, y, z);
        glScalef(width, height, depth);
        glColor3f(r, g, b);
        
        glBegin(GL_QUADS);
        // Frente
        glVertex3f(-0.5f, -0.5f,  0.5f);
        glVertex3f( 0.5f, -0.5f,  0.5f);
        glVertex3f( 0.5f,  0.5f,  0.5f);
        glVertex3f(-0.5f,  0.5f,  0.5f);
        
        // Atr?s
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f,  0.5f, -0.5f);
        glVertex3f( 0.5f,  0.5f, -0.5f);
        glVertex3f( 0.5f, -0.5f, -0.5f);
        
        // Izquierda
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f, -0.5f,  0.5f);
        glVertex3f(-0.5f,  0.5f,  0.5f);
        glVertex3f(-0.5f,  0.5f, -0.5f);
        
        // Derecha
        glVertex3f( 0.5f, -0.5f, -0.5f);
        glVertex3f( 0.5f,  0.5f, -0.5f);
        glVertex3f( 0.5f,  0.5f,  0.5f);
        glVertex3f( 0.5f, -0.5f,  0.5f);
        
        // Arriba
        glVertex3f(-0.5f,  0.5f, -0.5f);
        glVertex3f(-0.5f,  0.5f,  0.5f);
        glVertex3f( 0.5f,  0.5f,  0.5f);
        glVertex3f( 0.5f,  0.5f, -0.5f);
        
        // Abajo
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f( 0.5f, -0.5f, -0.5f);
        glVertex3f( 0.5f, -0.5f,  0.5f);
        glVertex3f(-0.5f, -0.5f,  0.5f);
        glEnd();
        
        glPopMatrix();
    }

    // Funci?n para dibujar las paredes exteriores amarillas
    void drawWalls() {
        // Pared frontal con puerta (cara frontal Z+)
        drawColoredCube(0.0f, 1.5f, 3.0f, 6.0f, 3.0f, 0.2f, 0.9f, 0.7f, 0.2f);
        
        // Pared lateral con ventana (cara derecha X+)  
        drawColoredCube(3.0f, 1.5f, 0.0f, 0.2f, 3.0f, 6.0f, 0.9f, 0.7f, 0.2f);
        
        // Pared trasera interior
        drawColoredCube(0.0f, 1.5f, -3.0f, 6.0f, 3.0f, 0.2f, 0.95f, 0.9f, 0.8f);
        
        // Pared interior izquierda
        drawColoredCube(-3.0f, 1.5f, 0.0f, 0.2f, 3.0f, 6.0f, 0.95f, 0.9f, 0.8f);
        
        // Piso exterior (concreto)
        drawColoredCube(0.0f, 3.0f, 0.0f, 6.5f, 0.2f, 6.5f, 0.9f, 0.9f, 0.9f);        
        // Piso interior (madera)
        drawColoredCube(0.0f, 0.05f, 0.0f, 5.8f, 0.05f, 5.8f, 0.6f, 0.4f, 0.2f);
        
        // Techo
// Ahora m?s peque?o, no sobresale
drawColoredCube(0.0f, 3.0f, 0.0f, 5.8f, 0.2f, 5.8f, 0.9f, 0.9f, 0.9f);
    }

    // Funci?n para dibujar una puerta exterior mirando hacia afuera
    void drawExteriorDoor() {
        // Hueco de la puerta cortado en la pared frontal (movido a la derecha)
        glPushMatrix();
        glTranslatef(1.0f, 1.0f, 3.0f);
        glColor3f(0.0f, 0.0f, 0.0f); // Negro para el hueco
        glScalef(0.8f, 2.0f, 0.25f);
        glutSolidCube(1.0f);
        glPopMatrix();
        
        // Marco de la puerta
        drawColoredCube(1.0f, 1.0f, 3.1f, 0.85f, 2.05f, 0.05f, 0.4f, 0.25f, 0.1f);
        
        // Puerta de madera (hacia afuera)
        drawColoredCube(1.0f, 1.0f, 3.15f, 0.75f, 1.9f, 0.08f, 0.5f, 0.3f, 0.15f);
        
        // Manija de la puerta
        glPushMatrix();
        glTranslatef(1.3f, 1.0f, 3.18f);
        glColor3f(0.8f, 0.7f, 0.3f);
        glutSolidSphere(0.04f, 8, 8);
        glPopMatrix();
        
        // N?mero de la casa
        drawColoredCube(1.0f, 2.2f, 3.18f, 0.15f, 0.2f, 0.02f, 1.0f, 1.0f, 1.0f);
    }

    // Funci?n para dibujar ventana exterior mirando hacia afuera
    void drawExteriorWindow() {
        // Hueco de la ventana cortado en la pared lateral
        glPushMatrix();
        glTranslatef(3.0f, 1.8f, 1.0f);
        glColor3f(0.6f, 0.8f, 1.0f); // Azul claro para simular cristal
        glScalef(0.25f, 1.2f, 1.0f);
        glutSolidCube(1.0f);
        glPopMatrix();
        
        // Marco de la ventana
        drawColoredCube(3.1f, 1.8f, 1.0f, 0.05f, 1.25f, 1.05f, 0.4f, 0.25f, 0.1f);
        
        // Cristal de la ventana (hacia afuera)
        drawColoredCube(3.15f, 1.8f, 1.0f, 0.02f, 1.15f, 0.95f, 0.7f, 0.9f, 1.0f);
        
        // Divisiones de la ventana (cruz)
        drawColoredCube(3.16f, 1.8f, 1.0f, 0.015f, 1.2f, 0.03f, 0.4f, 0.25f, 0.1f);
        drawColoredCube(3.16f, 1.8f, 1.0f, 0.015f, 0.03f, 1.0f, 0.4f, 0.25f, 0.1f);
        
        // Alf?izar de la ventana
        drawColoredCube(3.18f, 1.15f, 1.0f, 0.08f, 0.05f, 1.1f, 0.4f, 0.25f, 0.1f);
        
        // Maceta en el alf?izar
        glPushMatrix();
        glTranslatef(3.22f, 1.25f, 1.0f);
        glColor3f(0.6f, 0.3f, 0.1f);
        glutSolidCone(0.08f, 0.1f, 8, 8);
        glPopMatrix();
        
        // Planta en la maceta
        glPushMatrix();
        glTranslatef(3.22f, 1.35f, 1.0f);
        glColor3f(0.2f, 0.6f, 0.2f);
        glutSolidSphere(0.06f, 8, 8);
        glPopMatrix();
    }

    // Funci?n para dibujar el sof? verde con patr?n floral
    void drawSofa() {
        // Base del sof?
        drawColoredCube(0.0f, 0.3f, -1.5f, 2.5f, 0.6f, 0.8f, 0.4f, 0.6f, 0.3f);
        
        // Respaldo del sof?
        drawColoredCube(0.0f, 0.8f, -1.9f, 2.5f, 0.8f, 0.2f, 0.4f, 0.6f, 0.3f);
        
        // Brazos del sof?
        drawColoredCube(-1.0f, 0.6f, -1.5f, 0.3f, 0.8f, 0.8f, 0.4f, 0.6f, 0.3f);
        drawColoredCube(1.0f, 0.6f, -1.5f, 0.3f, 0.8f, 0.8f, 0.4f, 0.6f, 0.3f);
        
        // Cojines decorativos
        drawColoredCube(-0.5f, 0.7f, -1.5f, 0.4f, 0.2f, 0.6f, 0.5f, 0.7f, 0.4f);
        drawColoredCube(0.5f, 0.7f, -1.5f, 0.4f, 0.2f, 0.6f, 0.5f, 0.7f, 0.4f);
    }

    // Funci?n para dibujar una mesa
    void drawTable(float x, float y, float z, float width, float height, float depth) {
        // Superficie de la mesa
        drawColoredCube(x, y + height/2, z, width, 0.1f, depth, 0.4f, 0.25f, 0.1f);
        
        // Patas de la mesa
        float legHeight = height - 0.1f;
        drawColoredCube(x - width/2 + 0.1f, y + legHeight/2, z - depth/2 + 0.1f, 0.1f, legHeight, 0.1f, 0.3f, 0.2f, 0.1f);
        drawColoredCube(x + width/2 - 0.1f, y + legHeight/2, z - depth/2 + 0.1f, 0.1f, legHeight, 0.1f, 0.3f, 0.2f, 0.1f);
        drawColoredCube(x - width/2 + 0.1f, y + legHeight/2, z + depth/2 - 0.1f, 0.1f, legHeight, 0.1f, 0.3f, 0.2f, 0.1f);
        drawColoredCube(x + width/2 - 0.1f, y + legHeight/2, z + depth/2 - 0.1f, 0.1f, legHeight, 0.1f, 0.3f, 0.2f, 0.1f);
    }

    // Funci?n para dibujar una l?mpara
    void drawLamp(float x, float y, float z) {
        // Base de la l?mpara
        drawColoredCube(x, y, z, 0.3f, 0.1f, 0.3f, 0.3f, 0.3f, 0.3f);
        
        // Poste de la l?mpara
        drawColoredCube(x, y + 0.4f, z, 0.05f, 0.8f, 0.05f, 0.6f, 0.4f, 0.2f);
        
        // Pantalla de la l?mpara (amarilla/beige)
        glPushMatrix();
        glTranslatef(x, y + 0.9f, z);
        glColor3f(0.9f, 0.8f, 0.6f);
        glutSolidCone(0.3f, 0.4f, 12, 12);
        glPopMatrix();
    }

    // Funci?n para dibujar cuadros en la pared
    void drawPicture(float x, float y, float z, float width, float height) {
        // Marco del cuadro
        drawColoredCube(x, y, z, width + 0.05f, height + 0.05f, 0.02f, 0.3f, 0.2f, 0.1f);
        
        // Imagen del cuadro
        drawColoredCube(x, y, z + 0.015f, width, height, 0.01f, 0.8f, 0.8f, 0.7f);
    }

    // Funci?n para dibujar objetos decorativos en la mesa
    void drawDecorations() {
        // Jarr?n en la mesa
        glPushMatrix();
        glTranslatef(1.5f, 0.8f, 0.0f);
        glColor3f(0.6f, 0.3f, 0.2f);
        glutSolidCone(0.1f, 0.3f, 8, 8);
        glPopMatrix();
        
        // Libros en la mesa
        drawColoredCube(1.2f, 0.75f, -0.2f, 0.15f, 0.05f, 0.2f, 0.8f, 0.2f, 0.2f);
        drawColoredCube(1.2f, 0.8f, -0.2f, 0.15f, 0.05f, 0.2f, 0.2f, 0.6f, 0.8f);
    }

public:
    // Constructor
    CasaClotilde() {}
    
    // Destructor
    ~CasaClotilde() {}
    
    // Funci?n principal para renderizar toda la casa
    void render() {
        glPushMatrix();
        
        // Dibujar la estructura exterior
        drawWalls();
        drawExteriorDoor();
        drawExteriorWindow();
        
        // Dibujar los muebles interiores
        drawSofa();
        drawTable(1.5f, 0.0f, 0.0f, 1.0f, 0.7f, 0.6f);
        drawLamp(2.2f, 0.7f, 0.0f);
        
        // Dibujar cuadros en la pared
        drawPicture(-1.0f, 2.0f, -2.95f, 0.4f, 0.3f);
        drawPicture(-0.3f, 2.0f, -2.95f, 0.3f, 0.4f);
        drawPicture(0.3f, 1.8f, -2.95f, 0.25f, 0.3f);
        
        // Dibujar decoraciones
        drawDecorations();
        
        glPopMatrix();
    }
    
    // Funci?n para renderizar solo el exterior (si quieres optimizar)
    void renderExterior() {
        glPushMatrix();
        drawWalls();
        drawExteriorDoor();
        drawExteriorWindow();
        glPopMatrix();
    }
    
    // Funci?n para renderizar solo el interior
    void renderInterior() {
        glPushMatrix();
        drawSofa();
        drawTable(1.5f, 0.0f, 0.0f, 1.0f, 0.7f, 0.6f);
        drawLamp(2.2f, 0.7f, 0.0f);
        drawPicture(-1.0f, 2.0f, -2.95f, 0.4f, 0.3f);
        drawPicture(-0.3f, 2.0f, -2.95f, 0.3f, 0.4f);
        drawPicture(0.3f, 1.8f, -2.95f, 0.25f, 0.3f);
        drawDecorations();
        glPopMatrix();
    }
    
    // Funci?n para renderizar con transformaciones personalizadas
    void renderAt(float x, float y, float z, float scale = 1.0f, float rotationY = 0.0f) {
        glPushMatrix();
        glTranslatef(x, y, z);
        glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
        glScalef(scale, scale, scale);
        render();
        glPopMatrix();
    }
};

#endif // CASA_CLOTILDE_H

