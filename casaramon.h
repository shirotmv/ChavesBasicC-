#ifndef CASA_RAMON_H
#define CASA_RAMON_H

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <cmath>

class CasaRamon {
private:
    // Función auxiliar para dibujar un cubo con normales correctas para iluminación
    void drawCube(float x, float y, float z, float width, float height, float depth, float r, float g, float b) {
        glPushMatrix();
        glTranslatef(x, y, z);
        glColor3f(r, g, b);

        glBegin(GL_QUADS);
        
        // Front face (Positive Z)
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0, 0, depth);
        glVertex3f(width, 0, depth);
        glVertex3f(width, height, depth);
        glVertex3f(0, height, depth);

        // Back face (Negative Z)
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(width, 0, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, height, 0);
        glVertex3f(width, height, 0);

        // Left face (Negative X)
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, depth);
        glVertex3f(0, height, depth);
        glVertex3f(0, height, 0);

        // Right face (Positive X)
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(width, 0, depth);
        glVertex3f(width, 0, 0);
        glVertex3f(width, height, 0);
        glVertex3f(width, height, depth);

        // Top face (Positive Y)
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0, height, depth);
        glVertex3f(width, height, depth);
        glVertex3f(width, height, 0);
        glVertex3f(0, height, 0);

        // Bottom face (Negative Y)
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(0, 0, 0);
        glVertex3f(width, 0, 0);
        glVertex3f(width, 0, depth);
        glVertex3f(0, 0, depth);
        
        glEnd();
        glPopMatrix();
    }

    // Dibuja las paredes, el suelo y el techo de la habitación
    void drawRoom() {
        // Pared trasera (en Z = -6) - Normal hacia adelante para verla desde adentro
        glColor3f(0.6f, 0.8f, 0.9f);
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-6, 0, -6);
        glVertex3f(6, 0, -6);
        glVertex3f(6, 5.5f, -6);
        glVertex3f(-6, 5.5f, -6);
        glEnd();

        // Pared izquierda (en X = -6) - Normal hacia la derecha para verla desde adentro
        glColor3f(0.6f, 0.8f, 0.9f);
        glBegin(GL_QUADS);
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-6, 0, -6);
        glVertex3f(-6, 0, 6);
        glVertex3f(-6, 5.5f, 6);
        glVertex3f(-6, 5.5f, -6);
        glEnd();

        // Pared derecha (en X = 6) - Normal hacia la izquierda para verla desde adentro
        glColor3f(0.6f, 0.8f, 0.9f);
        glBegin(GL_QUADS);
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(6, 0, 6);
        glVertex3f(6, 0, -6);
        glVertex3f(6, 5.5f, -6);
        glVertex3f(6, 5.5f, 6);
        glEnd();

        // Pared frontal con ventana y puerta (en Z = 6) - Normal hacia atrás para verla desde adentro
        glColor3f(0.6f, 0.8f, 0.9f);
        glNormal3f(0.0f, 0.0f, -1.0f);

        // Parte izquierda de la pared frontal
        glBegin(GL_QUADS);
        glVertex3f(-6, 0, 6);
        glVertex3f(-4.0f, 0, 6);
        glVertex3f(-4.0f, 5.5f, 6);
        glVertex3f(-6, 5.5f, 6);
        glEnd();

        // Parte entre ventana y puerta
        glBegin(GL_QUADS);
        glVertex3f(-2.5f, 0, 6);
        glVertex3f(-1.0f, 0, 6);
        glVertex3f(-1.0f, 5.5f, 6);
        glVertex3f(-2.5f, 5.5f, 6);
        glEnd();

        // Parte superior de la ventana
        glBegin(GL_QUADS);
        glVertex3f(-4.0f, 3.0f, 6);
        glVertex3f(-2.5f, 3.0f, 6);
        glVertex3f(-2.5f, 5.5f, 6);
        glVertex3f(-4.0f, 5.5f, 6);
        glEnd();

        // Parte inferior de la ventana
        glBegin(GL_QUADS);
        glVertex3f(-4.0f, 0, 6);
        glVertex3f(-2.5f, 0, 6);
        glVertex3f(-2.5f, 1.0f, 6);
        glVertex3f(-4.0f, 1.0f, 6);
        glEnd();

        // Parte superior de la pared frontal (encima de la puerta)
        glBegin(GL_QUADS);
        glVertex3f(-1.0f, 3.0f, 6);
        glVertex3f(1.0f, 3.0f, 6);
        glVertex3f(1.0f, 5.5f, 6);
        glVertex3f(-1.0f, 5.5f, 6);
        glEnd();

        // Parte derecha de la pared frontal
        glBegin(GL_QUADS);
        glVertex3f(1.0f, 0, 6);
        glVertex3f(6, 0, 6);
        glVertex3f(6, 5.5f, 6);
        glVertex3f(1.0f, 5.5f, 6);
        glEnd();

        // Ventana (cristal celeste claro)
        glColor3f(0.8f, 0.9f, 1.0f);
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(-4.0f, 1.0f, 6.01f);
        glVertex3f(-2.5f, 1.0f, 6.01f);
        glVertex3f(-2.5f, 3.0f, 6.01f);
        glVertex3f(-4.0f, 3.0f, 6.01f);
        glEnd();

        // Marco de la ventana (blanco)
        glColor3f(0.9f, 0.9f, 0.9f);
        glNormal3f(0.0f, 0.0f, -1.0f);
        
        // Marco superior
        glBegin(GL_QUADS);
        glVertex3f(-4.1f, 2.9f, 6.02f);
        glVertex3f(-2.4f, 2.9f, 6.02f);
        glVertex3f(-2.4f, 3.1f, 6.02f);
        glVertex3f(-4.1f, 3.1f, 6.02f);
        glEnd();
        
        // Marco inferior
        glBegin(GL_QUADS);
        glVertex3f(-4.1f, 0.9f, 6.02f);
        glVertex3f(-2.4f, 0.9f, 6.02f);
        glVertex3f(-2.4f, 1.1f, 6.02f);
        glVertex3f(-4.1f, 1.1f, 6.02f);
        glEnd();
        
        // Marco izquierdo
        glBegin(GL_QUADS);
        glVertex3f(-4.1f, 1.0f, 6.02f);
        glVertex3f(-3.9f, 1.0f, 6.02f);
        glVertex3f(-3.9f, 3.0f, 6.02f);
        glVertex3f(-4.1f, 3.0f, 6.02f);
        glEnd();
        
        // Marco derecho
        glBegin(GL_QUADS);
        glVertex3f(-2.6f, 1.0f, 6.02f);
        glVertex3f(-2.4f, 1.0f, 6.02f);
        glVertex3f(-2.4f, 3.0f, 6.02f);
        glVertex3f(-2.6f, 3.0f, 6.02f);
        glEnd();
        
        // División central vertical
        glBegin(GL_QUADS);
        glVertex3f(-3.35f, 1.0f, 6.02f);
        glVertex3f(-3.15f, 1.0f, 6.02f);
        glVertex3f(-3.15f, 3.0f, 6.02f);
        glVertex3f(-3.35f, 3.0f, 6.02f);
        glEnd();
        
        // División central horizontal
        glBegin(GL_QUADS);
        glVertex3f(-4.0f, 1.9f, 6.02f);
        glVertex3f(-2.5f, 1.9f, 6.02f);
        glVertex3f(-2.5f, 2.1f, 6.02f);
        glVertex3f(-4.0f, 2.1f, 6.02f);
        glEnd();

        // Puerta (café/marrón)
        glColor3f(0.5f, 0.3f, 0.2f);
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(-1.0f, 0, 6);
        glVertex3f(1.0f, 0, 6);
        glVertex3f(1.0f, 3.0f, 6);
        glVertex3f(-1.0f, 3.0f, 6);
        glEnd();

        // Número de la puerta "72"
        glColor3f(0.9f, 0.9f, 0.9f);
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(-0.3f, 2.2f, 6.01f);
        glVertex3f(0.3f, 2.2f, 6.01f);
        glVertex3f(0.3f, 2.6f, 6.01f);
        glVertex3f(-0.3f, 2.6f, 6.01f);
        glEnd();

        // Manija de la puerta
        glColor3f(0.8f, 0.7f, 0.3f);
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(0.6f, 1.4f, 6.01f);
        glVertex3f(0.8f, 1.4f, 6.01f);
        glVertex3f(0.8f, 1.6f, 6.01f);
        glVertex3f(0.6f, 1.6f, 6.01f);
        glEnd();

        // Suelo gris (en Y = 0)
        glColor3f(0.7f, 0.7f, 0.7f);
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-6, 0, -6);
        glVertex3f(6, 0, -6);
        glVertex3f(6, 0, 6);
        glVertex3f(-6, 0, 6);
        glEnd();

        // Techo blanco (en Y = 5.5)
        glColor3f(0.9f, 0.9f, 0.9f);
        glBegin(GL_QUADS);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(-6, 5.5f, -6);
        glVertex3f(-6, 5.5f, 6);
        glVertex3f(6, 5.5f, 6);
        glVertex3f(6, 5.5f, -6);
        glEnd();
    }

    // Dibuja el sofá
    void drawSofa() {
        // Base del sofá (marrón)
        drawCube(-2.5f, 0.0f, -2.0f, 3.0f, 0.8f, 1.5f, 0.6f, 0.4f, 0.2f);

        // Respaldo del sofá
        drawCube(-2.5f, 0.8f, -2.0f, 3.0f, 1.2f, 0.3f, 0.6f, 0.4f, 0.2f);

        // Brazos del sofá (izquierda y derecha)
        drawCube(-2.5f, 0.8f, -2.0f, 0.3f, 0.8f, 1.5f, 0.6f, 0.4f, 0.2f);
        drawCube(2.2f, 0.8f, -2.0f, 0.3f, 0.8f, 1.5f, 0.6f, 0.4f, 0.2f);
    }

    // Dibuja la mesa
    void drawTable() {
        // Superficie de la mesa
        drawCube(-0.8f, 1.5f, 1.0f, 1.6f, 0.1f, 1.2f, 0.8f, 0.6f, 0.4f);

        // Patas de la mesa
        drawCube(-0.7f, 0.0f, 1.1f, 0.1f, 1.5f, 0.1f, 0.8f, 0.6f, 0.4f);
        drawCube(0.6f, 0.0f, 1.1f, 0.1f, 1.5f, 0.1f, 0.8f, 0.6f, 0.4f);
        drawCube(-0.7f, 0.0f, 2.1f, 0.1f, 1.5f, 0.1f, 0.8f, 0.6f, 0.4f);
        drawCube(0.6f, 0.0f, 2.1f, 0.1f, 1.5f, 0.1f, 0.8f, 0.6f, 0.4f);
    }

    // Dibuja una silla en la posición especificada
    void drawChair(float x, float z) {
        // Asiento
        drawCube(x, 0.8f, z, 0.8f, 0.1f, 0.8f, 0.8f, 0.6f, 0.4f);

        // Respaldo
        drawCube(x, 0.9f, z, 0.8f, 1.0f, 0.1f, 0.8f, 0.6f, 0.4f);

        // Patas
        drawCube(x, 0.0f, z, 0.1f, 0.8f, 0.1f, 0.8f, 0.6f, 0.4f);
        drawCube(x + 0.7f, 0.0f, z, 0.1f, 0.8f, 0.1f, 0.8f, 0.6f, 0.4f);
        drawCube(x, 0.0f, z + 0.7f, 0.1f, 0.8f, 0.1f, 0.8f, 0.6f, 0.4f);
        drawCube(x + 0.7f, 0.0f, z + 0.7f, 0.1f, 0.8f, 0.1f, 0.8f, 0.6f, 0.4f);
    }

    // Dibuja el aparador con objetos encima
    void drawSideboard() {
        drawCube(2.5f, 0.0f, -4.5f, 2.5f, 1.5f, 1.0f, 0.5f, 0.3f, 0.2f);

        // Objetos decorativos sobre el aparador
        drawCube(2.7f, 1.5f, -4.3f, 0.3f, 0.5f, 0.3f, 0.9f, 0.1f, 0.1f);
        drawCube(3.2f, 1.5f, -4.3f, 0.2f, 0.3f, 0.2f, 0.8f, 0.8f, 0.2f);
        drawCube(4.2f, 1.5f, -4.3f, 0.4f, 0.4f, 0.4f, 0.6f, 0.4f, 0.2f);
    }

    // Dibuja la estantería con libros
    void drawBookshelf() {
        drawCube(-5.5f, 0.0f, -3.0f, 0.5f, 4.5f, 2.0f, 0.7f, 0.5f, 0.3f);

        // Estantes individuales
        for(int i = 0; i < 6; i++) {
            drawCube(-5.5f, 0.7f * (i + 1), -3.0f, 0.5f, 0.05f, 2.0f, 0.8f, 0.6f, 0.4f);
        }

        // Libros en los estantes
        for(int shelf = 0; shelf < 5; shelf++) {
            for(int book = 0; book < 6; book++) {
                float bookY = 0.7f * (shelf + 1) + 0.05f;
                float bookZ = -2.8f + book * 0.3f;
                float r = 0.3f + (book % 3) * 0.2f;
                float g = 0.2f + (shelf % 2) * 0.3f;
                float b = 0.4f + ((book + shelf) % 2) * 0.3f;
                drawCube(-5.4f, bookY, bookZ, 0.3f, 0.6f, 0.25f, r, g, b);
            }
        }
    }

    // Dibuja el sillón
    void drawArmchair() {
        // Base del sillón
        drawCube(-4.5f, 0.0f, 2.0f, 1.2f, 0.8f, 1.2f, 0.4f, 0.6f, 0.3f);
        // Respaldo
        drawCube(-4.5f, 0.8f, 2.0f, 1.2f, 1.0f, 0.3f, 0.4f, 0.6f, 0.3f);
        // Brazos
        drawCube(-4.5f, 0.8f, 2.0f, 0.3f, 0.6f, 1.2f, 0.4f, 0.6f, 0.3f);
        drawCube(-3.6f, 0.8f, 2.0f, 0.3f, 0.6f, 1.2f, 0.4f, 0.6f, 0.3f);
    }

    // Dibuja las decoraciones de la pared
    void drawWallDecorations() {
        // Cuadros
        drawCube(-2.0f, 3.0f, -5.9f, 0.8f, 0.6f, 0.1f, 0.9f, 0.9f, 0.8f);
        drawCube(1.0f, 3.0f, -5.9f, 0.6f, 0.8f, 0.1f, 0.9f, 0.9f, 0.8f);
        drawCube(3.5f, 3.5f, -5.9f, 0.5f, 0.4f, 0.1f, 0.2f, 0.3f, 0.8f);

        // Decoración triangular
        glColor3f(0.8f, 0.2f, 0.3f);
        glBegin(GL_TRIANGLES);
        glNormal3f(0.0f, 0.0f, 1.0f);
        for(int i = 0; i < 5; i++) {
            float x = -3.0f + i * 1.2f;
            glVertex3f(x, 4.8f, -5.8f);
            glVertex3f(x + 0.4f, 4.8f, -5.8f);
            glVertex3f(x + 0.2f, 4.5f, -5.8f);
        }
        glEnd();
    }

    // Dibuja la alfombra en el suelo
    void drawRug() {
        // Base de la alfombra
        glColor3f(0.8f, 0.7f, 0.6f);
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-2.0f, 0.01f, 0.5f);
        glVertex3f(2.0f, 0.01f, 0.5f);
        glVertex3f(2.0f, 0.01f, 3.0f);
        glVertex3f(-2.0f, 0.01f, 3.0f);
        glEnd();

        // Patrón decorativo
        glColor3f(0.6f, 0.5f, 0.4f);
        glNormal3f(0.0f, 1.0f, 0.0f);
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 2; j++) {
                float x1 = -1.5f + i * 1.0f;
                float z1 = 0.8f + j * 1.0f;
                glBegin(GL_QUADS);
                glVertex3f(x1, 0.02f, z1);
                glVertex3f(x1 + 0.6f, 0.02f, z1);
                glVertex3f(x1 + 0.6f, 0.02f, z1 + 0.6f);
                glVertex3f(x1, 0.02f, z1 + 0.6f);
                glEnd();
            }
        }
    }

public:
    // Función para renderizar toda la casa de Ramón
    void render() {
        glPushMatrix();
        
        // Configurar el estado de renderizado
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_NORMALIZE);
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        
        // Configurar material especular
        GLfloat mat_specular[] = {0.3f, 0.3f, 0.3f, 1.0f};
        GLfloat mat_shininess[] = {10.0f};
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
        
        // Deshabilitar face culling para ver todas las caras
        glDisable(GL_CULL_FACE);
        
        // Dibujar todos los componentes
        drawRoom();
        drawRug();
        drawSofa();
        drawTable();
        drawChair(-1.5f, 0.2f);
        drawChair(0.7f, 0.2f);
        drawChair(-1.5f, 2.8f);
        drawChair(0.7f, 2.8f);
        drawSideboard();
        drawBookshelf();
        drawArmchair();
        drawWallDecorations();

        glPopMatrix();
    }

    // Inicialización de OpenGL
    void init() {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_NORMALIZE);
        
        // Deshabilitar face culling
        glDisable(GL_CULL_FACE);
        
        // Configurar material especular
        GLfloat mat_specular[] = {0.3f, 0.3f, 0.3f, 1.0f};
        GLfloat mat_shininess[] = {10.0f};
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
        
        // Habilitar iluminación de dos lados
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    }
};

#endif // CASA_RAMON_H
