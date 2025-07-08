#ifndef FLORINDA_CASA_H
#define FLORINDA_CASA_H

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>

class FlorindaCasa {
private:
    void dibujarCubo(float ancho, float alto, float profundo) {
        glBegin(GL_QUADS);
        
        // Cara frontal
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-ancho/2, -alto/2, profundo/2);
        glVertex3f(ancho/2, -alto/2, profundo/2);
        glVertex3f(ancho/2, alto/2, profundo/2);
        glVertex3f(-ancho/2, alto/2, profundo/2);
        
        // Cara trasera
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(-ancho/2, -alto/2, -profundo/2);
        glVertex3f(-ancho/2, alto/2, -profundo/2);
        glVertex3f(ancho/2, alto/2, -profundo/2);
        glVertex3f(ancho/2, -alto/2, -profundo/2);
        
        // Cara superior
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-ancho/2, alto/2, -profundo/2);
        glVertex3f(-ancho/2, alto/2, profundo/2);
        glVertex3f(ancho/2, alto/2, profundo/2);
        glVertex3f(ancho/2, alto/2, -profundo/2);
        
        // Cara inferior
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(-ancho/2, -alto/2, -profundo/2);
        glVertex3f(ancho/2, -alto/2, -profundo/2);
        glVertex3f(ancho/2, -alto/2, profundo/2);
        glVertex3f(-ancho/2, -alto/2, profundo/2);
        
        // Cara derecha
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(ancho/2, -alto/2, -profundo/2);
        glVertex3f(ancho/2, alto/2, -profundo/2);
        glVertex3f(ancho/2, alto/2, profundo/2);
        glVertex3f(ancho/2, -alto/2, profundo/2);
        
        // Cara izquierda
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(-ancho/2, -alto/2, -profundo/2);
        glVertex3f(-ancho/2, -alto/2, profundo/2);
        glVertex3f(-ancho/2, alto/2, profundo/2);
        glVertex3f(-ancho/2, alto/2, -profundo/2);
        
        glEnd();
    }
    
    void dibujarPiso() {
        // Piso principal (beige)
        glColor3f(0.9f, 0.8f, 0.6f);
        glPushMatrix();
        glTranslatef(0.0f, -3.0f, 0.0f);
        dibujarCubo(12.0f, 0.2f, 8.0f);
        glPopMatrix();
        
        // Alfombra sala (amarilla)
        glColor3f(0.9f, 0.8f, 0.2f);
        glPushMatrix();
        glTranslatef(-2.5f, -2.8f, -1.0f);
        dibujarCubo(4.0f, 0.1f, 3.0f);
        glPopMatrix();
        
        // Alfombra comedor (amarilla)
        glColor3f(0.9f, 0.8f, 0.2f);
        glPushMatrix();
        glTranslatef(2.5f, -2.8f, 1.0f);
        dibujarCubo(3.0f, 0.1f, 3.0f);
        glPopMatrix();
    }
    
    void dibujarTecho() {
        // Techo principal (color blanco/crema)
        glColor3f(0.95f, 0.95f, 0.9f);
        glPushMatrix();
        glTranslatef(0.0f, 3.0f, 0.0f);
        dibujarCubo(12.0f, 0.2f, 8.0f);
        glPopMatrix();
    }
    
    void dibujarParedes() {
        // Pared trasera
        glColor3f(0.7f, 0.8f, 0.9f);
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, -4.0f);
        dibujarCubo(12.0f, 6.0f, 0.2f);
        glPopMatrix();
        
        // Pared izquierda
        glColor3f(0.7f, 0.8f, 0.9f);
        glPushMatrix();
        glTranslatef(-6.0f, 0.0f, 0.0f);
        dibujarCubo(0.2f, 6.0f, 8.0f);
        glPopMatrix();
        
        // Pared derecha
        glColor3f(0.7f, 0.8f, 0.9f);
        glPushMatrix();
        glTranslatef(6.0f, 0.0f, 0.0f);
        dibujarCubo(0.2f, 6.0f, 8.0f);
        glPopMatrix();
        
        // Divisor central
        glColor3f(0.7f, 0.8f, 0.9f);
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 0.5f);
        dibujarCubo(0.2f, 6.0f, 1.0f);
        glPopMatrix();
    }
    
    void dibujarParedFrontalConPuerta() {
        // Pared frontal - lado izquierdo
        glColor3f(0.7f, 0.8f, 0.9f);
        glPushMatrix();
        glTranslatef(-3.5f, 0.0f, 4.0f);
        dibujarCubo(5.0f, 6.0f, 0.2f);
        glPopMatrix();
        
        // Pared frontal - lado derecho
        glColor3f(0.7f, 0.8f, 0.9f);
        glPushMatrix();
        glTranslatef(3.5f, 0.0f, 4.0f);
        dibujarCubo(5.0f, 6.0f, 0.2f);
        glPopMatrix();
        
        // Pared frontal - parte superior de la puerta
        glColor3f(0.7f, 0.8f, 0.9f);
        glPushMatrix();
        glTranslatef(0.0f, 1.5f, 4.0f);
        dibujarCubo(2.0f, 3.0f, 0.2f);
        glPopMatrix();
        
        // Marco de la puerta
        glColor3f(0.6f, 0.4f, 0.2f);
        
        // Marco izquierdo
        glPushMatrix();
        glTranslatef(-1.1f, -1.0f, 4.05f);
        dibujarCubo(0.2f, 4.0f, 0.1f);
        glPopMatrix();
        
        // Marco derecho
        glPushMatrix();
        glTranslatef(1.1f, -1.0f, 4.05f);
        dibujarCubo(0.2f, 4.0f, 0.1f);
        glPopMatrix();
        
        // Marco superior
        glPushMatrix();
        glTranslatef(0.0f, 1.0f, 4.05f);
        dibujarCubo(2.2f, 0.2f, 0.1f);
        glPopMatrix();
        
        // Puerta
        glColor3f(0.5f, 0.3f, 0.1f);
        glPushMatrix();
        glTranslatef(0.0f, -1.0f, 4.1f);
        dibujarCubo(1.8f, 4.0f, 0.15f);
        glPopMatrix();
        
        // Manija de la puerta
        glColor3f(0.8f, 0.8f, 0.3f); // Color dorado
        glPushMatrix();
        glTranslatef(0.7f, -0.8f, 4.2f);
        dibujarCubo(0.1f, 0.1f, 0.05f);
        glPopMatrix();
    }
    
    void dibujarSofa() {
        // Base del sof?
        glColor3f(0.8f, 0.7f, 0.5f);
        glPushMatrix();
        glTranslatef(-3.5f, -2.0f, -2.0f);
        dibujarCubo(2.5f, 0.6f, 1.2f);
        glPopMatrix();
        
        // Respaldo del sof?
        glColor3f(0.8f, 0.7f, 0.5f);
        glPushMatrix();
        glTranslatef(-3.5f, -1.4f, -2.5f);
        dibujarCubo(2.5f, 1.2f, 0.3f);
        glPopMatrix();
        
        // Brazo izquierdo
        glColor3f(0.8f, 0.7f, 0.5f);
        glPushMatrix();
        glTranslatef(-4.6f, -1.6f, -2.0f);
        dibujarCubo(0.3f, 0.8f, 1.2f);
        glPopMatrix();
        
        // Brazo derecho
        glColor3f(0.8f, 0.7f, 0.5f);
        glPushMatrix();
        glTranslatef(-2.4f, -1.6f, -2.0f);
        dibujarCubo(0.3f, 0.8f, 1.2f);
        glPopMatrix();
    }
    
    void dibujarSillon() {
        // Base del sill?n
        glColor3f(0.8f, 0.7f, 0.5f);
        glPushMatrix();
        glTranslatef(-1.5f, -2.0f, -2.8f);
        dibujarCubo(1.2f, 0.6f, 1.0f);
        glPopMatrix();
        
        // Respaldo del sill?n
        glColor3f(0.8f, 0.7f, 0.5f);
        glPushMatrix();
        glTranslatef(-1.5f, -1.4f, -3.2f);
        dibujarCubo(1.2f, 1.2f, 0.2f);
        glPopMatrix();
        
        // Brazos del sill?n
        glColor3f(0.8f, 0.7f, 0.5f);
        glPushMatrix();
        glTranslatef(-2.0f, -1.6f, -2.8f);
        dibujarCubo(0.2f, 0.8f, 1.0f);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(-1.0f, -1.6f, -2.8f);
        dibujarCubo(0.2f, 0.8f, 1.0f);
        glPopMatrix();
    }
    
    void dibujarMesaCentro() {
        // Superficie
        glColor3f(0.4f, 0.2f, 0.1f);
        glPushMatrix();
        glTranslatef(-2.5f, -1.8f, -0.5f);
        dibujarCubo(1.5f, 0.1f, 0.8f);
        glPopMatrix();
        
        // Patas
        glColor3f(0.3f, 0.15f, 0.05f);
        float offsetX[] = {-0.6f, 0.6f, -0.6f, 0.6f};
        float offsetZ[] = {-0.3f, -0.3f, 0.3f, 0.3f};
        
        for(int i = 0; i < 4; i++) {
            glPushMatrix();
            glTranslatef(-2.5f + offsetX[i], -2.3f, -0.5f + offsetZ[i]);
            dibujarCubo(0.1f, 1.0f, 0.1f);
            glPopMatrix();
        }
    }
    
    void dibujarMesaComedor() {
        // Superficie
        glColor3f(0.6f, 0.4f, 0.2f);
        glPushMatrix();
        glTranslatef(3.0f, -1.6f, 1.0f);
        dibujarCubo(2.2f, 0.15f, 1.5f);
        glPopMatrix();
        
        // Patas
        glColor3f(0.5f, 0.3f, 0.1f);
        float mesaOffsetX[] = {-0.9f, 0.9f, -0.9f, 0.9f};
        float mesaOffsetZ[] = {-0.6f, -0.6f, 0.6f, 0.6f};
        
        for(int i = 0; i < 4; i++) {
            glPushMatrix();
            glTranslatef(3.0f + mesaOffsetX[i], -2.3f, 1.0f + mesaOffsetZ[i]);
            dibujarCubo(0.12f, 1.4f, 0.12f);
            glPopMatrix();
        }
        
        // Mantel
        glColor3f(1.0f, 0.9f, 0.9f);
        glPushMatrix();
        glTranslatef(3.0f, -1.5f, 1.0f);
        dibujarCubo(2.4f, 0.05f, 1.7f);
        glPopMatrix();
    }
    
    void dibujarSillas() {
        float posicionesX[] = {2.2f, 3.8f, 2.2f, 3.8f};
        float posicionesZ[] = {0.3f, 0.3f, 1.7f, 1.7f};
        
        for(int i = 0; i < 4; i++) {
            // Asiento
            glColor3f(0.5f, 0.3f, 0.1f);
            glPushMatrix();
            glTranslatef(posicionesX[i], -2.2f, posicionesZ[i]);
            dibujarCubo(0.4f, 0.1f, 0.4f);
            glPopMatrix();
            
            // Respaldo
            glColor3f(0.5f, 0.3f, 0.1f);
            glPushMatrix();
            glTranslatef(posicionesX[i], -1.5f, posicionesZ[i] + (i < 2 ? -0.15f : 0.15f));
            dibujarCubo(0.4f, 1.2f, 0.1f);
            glPopMatrix();
            
            // Patas
            glColor3f(0.4f, 0.25f, 0.05f);
            float sillaOffsetX[] = {-0.15f, 0.15f, -0.15f, 0.15f};
            float sillaOffsetZ[] = {-0.15f, -0.15f, 0.15f, 0.15f};
            
            for(int j = 0; j < 4; j++) {
                glPushMatrix();
                glTranslatef(posicionesX[i] + sillaOffsetX[j], -2.6f, posicionesZ[i] + sillaOffsetZ[j]);
                dibujarCubo(0.05f, 0.8f, 0.05f);
                glPopMatrix();
            }
        }
    }
    
    void dibujarMuebleTV() {
        // Mueble bajo
        glColor3f(0.6f, 0.4f, 0.2f);
        glPushMatrix();
        glTranslatef(0.0f, -2.4f, -3.5f);
        dibujarCubo(2.0f, 0.8f, 0.6f);
        glPopMatrix();
        
        // TV (pantalla negra)
        glColor3f(0.1f, 0.1f, 0.1f);
        glPushMatrix();
        glTranslatef(0.0f, -1.2f, -3.8f);
        dibujarCubo(1.5f, 0.9f, 0.1f);
        glPopMatrix();
        
        // Marco de TV
        glColor3f(0.2f, 0.2f, 0.2f);
        glPushMatrix();
        glTranslatef(0.0f, -1.2f, -3.75f);
        dibujarCubo(1.6f, 1.0f, 0.05f);
        glPopMatrix();
    }
    
    void dibujarCuadro() {
        // Marco
        glColor3f(0.8f, 0.7f, 0.5f);
        glPushMatrix();
        glTranslatef(-3.5f, 0.5f, -3.9f);
        dibujarCubo(1.0f, 0.7f, 0.05f);
        glPopMatrix();
        
        // Imagen (simulada con color oscuro)
        glColor3f(0.3f, 0.4f, 0.3f);
        glPushMatrix();
        glTranslatef(-3.5f, 0.5f, -3.85f);
        dibujarCubo(0.8f, 0.5f, 0.02f);
        glPopMatrix();
    }

public:
    FlorindaCasa() {}
    
    ~FlorindaCasa() {}
    
    // M?todo principal para dibujar toda la casa
    void render() {
        // Guardar estado actual de la matriz
        glPushMatrix();
        
        // Dibujar todos los elementos de la casa
        dibujarPiso();
        dibujarTecho();
        dibujarParedes();
        dibujarParedFrontalConPuerta();
        dibujarSofa();
        dibujarSillon();
        dibujarMesaCentro();
        dibujarMesaComedor();
        dibujarSillas();
        dibujarMuebleTV();
        dibujarCuadro();
        
        // Restaurar estado de la matriz
        glPopMatrix();
    }
    
    // M?todo para inicializar la iluminaci?n (opcional)
    void inicializarIluminacion() {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
        
        // Configurar luz
        GLfloat luzPos[] = {5.0f, 10.0f, 5.0f, 1.0f};
        GLfloat luzAmb[] = {0.3f, 0.3f, 0.3f, 1.0f};
        GLfloat luzDif[] = {0.8f, 0.8f, 0.8f, 1.0f};
        
        glLightfv(GL_LIGHT0, GL_POSITION, luzPos);
        glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmb);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDif);
    }
    
    // M?todo para dibujar la casa en una posici?n espec?fica
    void dibujarEnPosicion(float x, float y, float z) {
        glPushMatrix();
        glTranslatef(x, y, z);
        render();
        glPopMatrix();
    }
    
    // M?todo para dibujar la casa con escala espec?fica
    void dibujarConEscala(float escala) {
        glPushMatrix();
        glScalef(escala, escala, escala);
        render();
        glPopMatrix();
    }
    
    // M?todo para dibujar la casa con rotaci?n
    void dibujarConRotacion(float anguloX, float anguloY, float anguloZ) {
        glPushMatrix();
        glRotatef(anguloX, 1.0f, 0.0f, 0.0f);
        glRotatef(anguloY, 0.0f, 1.0f, 0.0f);
        glRotatef(anguloZ, 0.0f, 0.0f, 1.0f);
        render();
        glPopMatrix();
    }
    
    // M?todo completo con transformaciones
    void dibujarCompleto(float x, float y, float z, float escala, float rotX, float rotY, float rotZ) {
        glPushMatrix();
        glTranslatef(x, y, z);
        glRotatef(rotX, 1.0f, 0.0f, 0.0f);
        glRotatef(rotY, 0.0f, 1.0f, 0.0f);
        glRotatef(rotZ, 0.0f, 0.0f, 1.0f);
        glScalef(escala, escala, escala);
        render();
        glPopMatrix();
    }
};

#endif // FLORINDA_CASA_H

