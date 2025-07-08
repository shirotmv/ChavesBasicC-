#ifndef PILETA_H
#define PILETA_H

#include <cstdio>
#include <cmath>
#include <ctime>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "texture.h"

// Define M_PI if not already defined
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Pileta {
private:
    float tiempoAgua;
    clock_t tiempoInicio;
    clock_t tiempoAnterior;
    
    // IDs de texturas
    GLuint texturaAzulejos;
    GLuint texturaLadrillos;
    GLuint texturaPiso;
    bool texturasInicializadas;
    
    // Parámetros de animación automática
    float velocidadAnimacion;
    float amplitudOndas;
    
    void inicializarTexturas() {
        if (!texturasInicializadas) {
            // Cargar texturas si existen los archivos (silenciosamente)
            texturaAzulejos = TextureManager::loadTexture("azulejos.bmp");
            texturaLadrillos = TextureManager::loadTexture("ladrillos.bmp");
            texturaPiso = TextureManager::loadTexture("piso.bmp");
            texturasInicializadas = true;
        }
    }
    
    void actualizarTiempoAutomatico() {
        clock_t tiempoActual = clock();
        if (tiempoAnterior == 0) {
            tiempoAnterior = tiempoActual;
        }
        
        float deltaTime = (float)(tiempoActual - tiempoAnterior) / CLOCKS_PER_SEC;
        tiempoAgua += deltaTime * velocidadAnimacion;
        tiempoAnterior = tiempoActual;
    }
    
    void dibujarCilindro(float radio, float altura, int segmentos) {
        float angulo = 2.0f * M_PI / segmentos;
        
        // Tapa superior
        glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.5f, 0.5f);
        glVertex3f(0.0f, altura/2, 0.0f);
        for(int i = 0; i <= segmentos; i++) {
            float x = radio * cos(i * angulo);
            float z = radio * sin(i * angulo);
            glTexCoord2f(0.5f + 0.5f * cos(i * angulo), 0.5f + 0.5f * sin(i * angulo));
            glVertex3f(x, altura/2, z);
        }
        glEnd();
        
        // Tapa inferior
        glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glTexCoord2f(0.5f, 0.5f);
        glVertex3f(0.0f, -altura/2, 0.0f);
        for(int i = segmentos; i >= 0; i--) {
            float x = radio * cos(i * angulo);
            float z = radio * sin(i * angulo);
            glTexCoord2f(0.5f + 0.5f * cos(i * angulo), 0.5f + 0.5f * sin(i * angulo));
            glVertex3f(x, -altura/2, z);
        }
        glEnd();
        
        // Lados
        glBegin(GL_QUAD_STRIP);
        for(int i = 0; i <= segmentos; i++) {
            float x = radio * cos(i * angulo);
            float z = radio * sin(i * angulo);
            glNormal3f(x/radio, 0.0f, z/radio);
            glTexCoord2f((float)i/segmentos, 1.0f);
            glVertex3f(x, altura/2, z);
            glTexCoord2f((float)i/segmentos, 0.0f);
            glVertex3f(x, -altura/2, z);
        }
        glEnd();
    }
    
    void dibujarAguaAnimada() {
        // Actualizar animación automáticamente
        actualizarTiempoAutomatico();
        
        // Habilitar transparencia
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        int resolucion = 30;
        float radioAgua = 1.9f;
        
        // Agua principal con ondas automáticas
        for(int i = 0; i < resolucion-1; i++) {
            glBegin(GL_TRIANGLE_STRIP);
            for(int j = 0; j <= resolucion; j++) {
                for(int k = 0; k < 2; k++) {
                    float angulo = 2.0f * M_PI * j / resolucion;
                    float radio = radioAgua * (i + k) / (resolucion-1);
                    
                    float x = radio * cos(angulo);
                    float z = radio * sin(angulo);
                    
                    // Ondas automáticas con múltiples frecuencias
                    float onda1 = amplitudOndas * sin(tiempoAgua * 2.5f + radio * 6.0f);
                    float onda2 = amplitudOndas * 0.6f * sin(tiempoAgua * 1.8f + angulo * 3.0f);
                    float onda3 = amplitudOndas * 0.5f * sin(tiempoAgua * 3.2f + x * 4.0f + z * 4.0f);
                    float onda4 = amplitudOndas * 0.3f * sin(tiempoAgua * 4.1f + radio * 2.0f);
                    float y = onda1 + onda2 + onda3 + onda4;
                    
                    // Color dinámico del agua
                    float profundidad = 1.0f - radio / radioAgua;
                    float brillo = 0.5f + 0.3f * sin(tiempoAgua * 1.5f + radio * 3.0f);
                    glColor4f(0.05f + 0.15f * profundidad * brillo, 
                             0.25f + 0.35f * profundidad * brillo, 
                             0.7f + 0.2f * profundidad * brillo, 
                             0.65f + 0.25f * profundidad);
                    
                    // Normal dinámica para iluminación
                    float dx = 0.4f * cos(tiempoAgua * 2.5f + radio * 6.0f) * cos(angulo);
                    float dz = 0.4f * cos(tiempoAgua * 2.5f + radio * 6.0f) * sin(angulo);
                    glNormal3f(-dx, 1.0f, -dz);
                    
                    glTexCoord2f(x * 0.5f + 0.5f + 0.1f * sin(tiempoAgua + radio), 
                                z * 0.5f + 0.5f + 0.1f * cos(tiempoAgua + angulo));
                    glVertex3f(x, y, z);
                }
            }
            glEnd();
        }
        
        // Reflejos automáticos brillantes
        glColor4f(1.0f, 1.0f, 1.0f, 0.4f);
        for(int i = 0; i < 8; i++) {
            float anguloReflejo = tiempoAgua * 1.3f + i * M_PI * 0.25f;
            float radioReflejo = 0.2f + 0.6f * sin(tiempoAgua * 0.8f + i * 0.7f);
            float x = radioReflejo * cos(anguloReflejo);
            float z = radioReflejo * sin(anguloReflejo);
            float y = 0.02f + 0.03f * sin(tiempoAgua * 2.8f + i * 0.5f);
            
            glPushMatrix();
            glTranslatef(x, y, z);
            
            // Reflejos circulares con variación automática
            float tamanoBase = 0.06f + 0.04f * sin(tiempoAgua * 3.5f + i);
            glBegin(GL_TRIANGLE_FAN);
            glTexCoord2f(0.5f, 0.5f);
            glVertex3f(0.0f, 0.008f, 0.0f);
            for(int j = 0; j <= 12; j++) {
                float a = 2.0f * M_PI * j / 12;
                float tamano = tamanoBase + 0.02f * sin(tiempoAgua * 4.2f + i + j * 0.3f);
                glTexCoord2f(0.5f + 0.5f * cos(a), 0.5f + 0.5f * sin(a));
                glVertex3f(tamano * cos(a), 0.008f, tamano * sin(a));
            }
            glEnd();
            glPopMatrix();
        }
        
        // Ondas concéntricas adicionales
        glColor4f(0.8f, 0.9f, 1.0f, 0.2f);
        for(int i = 0; i < 3; i++) {
            float radioOnda = 0.5f + (tiempoAgua * 0.3f + i * 0.8f);
            radioOnda = fmod(radioOnda, 2.0f);
            if(radioOnda > 1.8f) continue;
            
            float alpha = 0.3f * (1.0f - radioOnda / 1.8f);
            glColor4f(1.0f, 1.0f, 1.0f, alpha);
            
            glBegin(GL_LINE_LOOP);
            for(int j = 0; j < 32; j++) {
                float angulo = 2.0f * M_PI * j / 32;
                float x = radioOnda * cos(angulo);
                float z = radioOnda * sin(angulo);
                float y = 0.01f * sin(tiempoAgua * 5.0f + radioOnda * 8.0f);
                glVertex3f(x, y, z);
            }
            glEnd();
        }
        
        glDisable(GL_BLEND);
    }

public:
    Pileta() : tiempoAgua(0.0f), tiempoAnterior(0), velocidadAnimacion(1.0f), 
               amplitudOndas(0.04f), texturasInicializadas(false),
               texturaAzulejos(0), texturaLadrillos(0), texturaPiso(0) {
        tiempoInicio = clock();
        inicializarTexturas();
    }
    
    ~Pileta() {
        limpiarTexturas();
    }
    
    void limpiarTexturas() {
        if (texturasInicializadas) {
            TextureManager::deleteTexture(texturaAzulejos);
            TextureManager::deleteTexture(texturaLadrillos);
            TextureManager::deleteTexture(texturaPiso);
            texturasInicializadas = false;
        }
    }
    
    void render() {
        glPushMatrix();
        
        // Base exterior de la pileta (concreto)
        glColor3f(0.7f, 0.7f, 0.7f);
        TextureManager::unbindTexture();
        dibujarCilindro(2.8f, 0.4f, 32);
        
        // Crear el "hoyo" de la pileta
        glTranslatef(0.0f, 0.2f, 0.0f);
        
        // Pared interior de la pileta (azulejos celestes)
        if (texturaAzulejos != 0) {
            TextureManager::bindTexture(texturaAzulejos);
            glColor3f(0.9f, 0.95f, 1.0f);
        } else {
            glColor3f(0.4f, 0.7f, 0.9f);
        }
        
        float radioExterior = 2.2f;
        float radioInterior = 2.0f;
        float alturaParedes = 1.2f;
        
        // Dibujar paredes interiores con texturas
        glBegin(GL_QUAD_STRIP);
        for(int i = 0; i <= 32; i++) {
            float angulo = 2.0f * M_PI * i / 32;
            float x = radioInterior * cos(angulo);
            float z = radioInterior * sin(angulo);
            glNormal3f(-cos(angulo), 0.0f, -sin(angulo));
            glTexCoord2f((float)i/32 * 4.0f, 1.0f);
            glVertex3f(x, alturaParedes/2, z);
            glTexCoord2f((float)i/32 * 4.0f, 0.0f);
            glVertex3f(x, -alturaParedes/2, z);
        }
        glEnd();
        
        // Fondo de la pileta (azulejos azules más oscuros)
        glColor3f(0.2f, 0.4f, 0.7f);
        glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.5f, 0.5f);
        glVertex3f(0.0f, -alturaParedes/2, 0.0f);
        for(int i = 0; i <= 32; i++) {
            float angulo = 2.0f * M_PI * i / 32;
            float x = radioInterior * cos(angulo);
            float z = radioInterior * sin(angulo);
            glTexCoord2f(0.5f + 0.5f * cos(angulo), 0.5f + 0.5f * sin(angulo));
            glVertex3f(x, -alturaParedes/2, z);
        }
        glEnd();
        
        // Borde superior de la pileta
        glColor3f(0.85f, 0.85f, 0.85f);
        glBegin(GL_QUAD_STRIP);
        for(int i = 0; i <= 32; i++) {
            float angulo = 2.0f * M_PI * i / 32;
            float x1 = radioInterior * cos(angulo);
            float z1 = radioInterior * sin(angulo);
            float x2 = radioExterior * cos(angulo);
            float z2 = radioExterior * sin(angulo);
            glNormal3f(0.0f, 1.0f, 0.0f);
            glTexCoord2f((float)i/32 * 2.0f, 0.0f);
            glVertex3f(x1, alturaParedes/2, z1);
            glTexCoord2f((float)i/32 * 2.0f, 1.0f);
            glVertex3f(x2, alturaParedes/2, z2);
        }
        glEnd();
        
        // Agua de la pileta con animación automática
        glTranslatef(0.0f, 0.4f, 0.0f);
        TextureManager::unbindTexture();
        dibujarAguaAnimada();
        
        // Borde decorativo de ladrillos
        glTranslatef(0.0f, -0.8f, 0.0f);
        
        if (texturaLadrillos != 0) {
            TextureManager::bindTexture(texturaLadrillos);
            glColor3f(0.95f, 0.9f, 0.85f);
        } else {
            glColor3f(0.8f, 0.4f, 0.3f);
        }
        
        int numLadrillos = 20;
        float radioLadrillo = 2.6f;
        
        for(int i = 0; i < numLadrillos; i++) {
            glPushMatrix();
            float angulo = 2.0f * M_PI * i / numLadrillos;
            
            glRotatef(angulo * 180.0f / M_PI, 0.0f, 1.0f, 0.0f);
            glTranslatef(radioLadrillo, 0.0f, 0.0f);
            
            // Ladrillo con coordenadas de textura
            glBegin(GL_QUADS);
            // Cara frontal
            glNormal3f(1.0f, 0.0f, 0.0f);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(0.15f, -0.15f, -0.2f);
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(0.15f, 0.15f, -0.2f);
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(0.15f, 0.15f, 0.2f);
            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(0.15f, -0.15f, 0.2f);
            
            // Cara superior
            glNormal3f(0.0f, 1.0f, 0.0f);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(-0.1f, 0.15f, -0.2f);
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(0.15f, 0.15f, -0.2f);
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(0.15f, 0.15f, 0.2f);
            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(-0.1f, 0.15f, 0.2f);
            
            // Cara lateral
            glNormal3f(0.0f, 0.0f, 1.0f);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(-0.1f, -0.15f, 0.2f);
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(0.15f, -0.15f, 0.2f);
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(0.15f, 0.15f, 0.2f);
            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(-0.1f, 0.15f, 0.2f);
            glEnd();
            
            glPopMatrix();
        }
        
        TextureManager::unbindTexture();
        glPopMatrix();
    }
    
    void dibujarPiso() {
        if (texturaPiso != 0) {
            TextureManager::bindTexture(texturaPiso);
            glColor3f(1.0f, 1.0f, 1.0f);
        } else {
            glColor3f(0.9f, 0.9f, 0.8f);
        }
        
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-10.0f, -0.65f, -10.0f);
        glTexCoord2f(5.0f, 0.0f);
        glVertex3f(10.0f, -0.65f, -10.0f);
        glTexCoord2f(5.0f, 5.0f);
        glVertex3f(10.0f, -0.65f, 10.0f);
        glTexCoord2f(0.0f, 5.0f);
        glVertex3f(-10.0f, -0.65f, 10.0f);
        glEnd();
        
        TextureManager::unbindTexture();
    }
    
    // Métodos para obtener información de la pileta
    float getRadio() const { return 2.0f; }
    float getAltura() const { return 0.6f; }
    float getPosicionY() const { return -0.05f; }
    
    // Verificar si un punto está dentro de la pileta
    bool estaDentro(float x, float z) const {
        float distancia = sqrt(x*x + z*z);
        return distancia <= getRadio();
    }
    
    // Obtener tiempo de ejecución
    float getTiempoEjecucion() const {
        return (float)(clock() - tiempoInicio) / CLOCKS_PER_SEC;
    }
};

#endif // PILETA_H
