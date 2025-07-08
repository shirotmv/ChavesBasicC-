#ifndef WORLD_H
#define WORLD_H

#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "casajaimito.h"
#include "casaclotilde.h"
#include "casaflorinda.h"
#include "casaramon.h"
#include "barril.h"
#include "pelota.h"
#include "escalera.h"
#include "pileta.h"
#include "animationcar.h"    
#include "pista.h"
#include "muros.h"
#include "minigame.h" // ¡NUEVA INCLUSIÓN!

class World {
private:
    CasaJaimito casa;
    CasaClotilde casaclotilde;
    FlorindaCasa casaflorinda;
    CasaRamon casaramon;
    Barril barril;
    Pelota pelota;
    Escalera escalera;
    Pileta pileta;
    Muros constructor; // Constructor con valores por defecto

    // Vector de carros animados
    AnimationCar miCarro;    
    Pista pista;
    Muros muros;
    
    // Instancia del MiniGame
    MiniGame minigame; // ¡NUEVA INSTANCIA!
    
    GLuint terrainTexture;
    bool textureLoaded;
    
    // Función para cargar textura BMP
    GLuint loadBMPTexture(const char* filename) {
        FILE* file = fopen(filename, "rb");
        if (!file) {
            printf("Error: No se pudo abrir el archivo %s\n", filename);
            return 0;
        }

        // Leer header del BMP
        unsigned char header[54];
        if (fread(header, 1, 54, file) != 54) {
            printf("Error: Archivo BMP inválido\n");
            fclose(file);
            return 0;
        }

        // Verificar que sea un BMP válido
        if (header[0] != 'B' || header[1] != 'M') {
            printf("Error: No es un archivo BMP válido\n");
            fclose(file);
            return 0;
        }

        // Extraer información del header
        unsigned int dataPos = *(int*)&(header[0x0A]);
        unsigned int imageSize = *(int*)&(header[0x22]);
        unsigned int width = *(int*)&(header[0x12]);
        unsigned int height = *(int*)&(header[0x16]);

        // Algunos archivos BMP no tienen estos campos completados
        if (imageSize == 0) imageSize = width * height * 3;
        if (dataPos == 0) dataPos = 54;

        // Leer los datos de la imagen
        unsigned char* data = new unsigned char[imageSize];
        fread(data, 1, imageSize, file);
        fclose(file);

        // Crear textura OpenGL
        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Los archivos BMP están en formato BGR, necesitamos convertir a RGB
        for (unsigned int i = 0; i < imageSize; i += 3) {
            unsigned char temp = data[i];
            data[i] = data[i + 2];
            data[i + 2] = temp;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

        // Configurar parámetros de textura
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        delete[] data;
        return textureID;
    }
    
    void drawCube(float x, float y, float z, float size, float r, float g, float b) {
        glPushMatrix();
        glTranslatef(x, y + size * 0.5f, z);
        glColor3f(r, g, b);

        float s = size * 0.5f;

        glBegin(GL_QUADS);

        // Front face
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-s, -s, s);
        glVertex3f(s, -s, s);
        glVertex3f(s, s, s);
        glVertex3f(-s, s, s);

        // Back face
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(-s, -s, -s);
        glVertex3f(-s, s, -s);
        glVertex3f(s, s, -s);
        glVertex3f(s, -s, -s);

        // Left face
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(-s, -s, -s);
        glVertex3f(-s, -s, s);
        glVertex3f(-s, s, s);
        glVertex3f(-s, s, -s);

        // Right face
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(s, -s, -s);
        glVertex3f(s, s, -s);
        glVertex3f(s, s, s);
        glVertex3f(s, -s, s);

        // Top face
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-s, s, -s);
        glVertex3f(-s, s, s);
        glVertex3f(s, s, s);
        glVertex3f(s, s, -s);

        // Bottom face
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(-s, -s, -s);
        glVertex3f(s, -s, -s);
        glVertex3f(s, -s, s);
        glVertex3f(-s, -s, s);

        glEnd();
        glPopMatrix();
    }

    void drawTerrainTile(float x, float z, float size, float r, float g, float b) {
        glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);

        glDisable(GL_CULL_FACE);
        glDisable(GL_LIGHTING);

        // Habilitar textura si está cargada
        if (textureLoaded) {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, terrainTexture);
            glColor3f(1.0f, 1.0f, 1.0f); // Color blanco para que la textura se vea sin tinte
        } else {
            glColor3f(r, g, b);
        }

        glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        
        // Coordenadas de textura que se repiten cada cierto número de tiles
        float texScale = 0.1f; // Ajusta este valor para controlar la repetición de la textura
        float u1 = x * texScale;
        float v1 = z * texScale;
        float u2 = (x + size) * texScale;
        float v2 = (z + size) * texScale;
        
        if (textureLoaded) {
            glTexCoord2f(u1, v1); glVertex3f(x, -0.1f, z);
            glTexCoord2f(u2, v1); glVertex3f(x + size, -0.1f, z);
            glTexCoord2f(u2, v2); glVertex3f(x + size, -0.1f, z + size);
            glTexCoord2f(u1, v2); glVertex3f(x, -0.1f, z + size);
        } else {
            glVertex3f(x, -0.1f, z);
            glVertex3f(x + size, -0.1f, z);
            glVertex3f(x + size, -0.1f, z + size);
            glVertex3f(x, -0.1f, z + size);
        }
        glEnd();

        // Cara inferior sin textura
        glDisable(GL_TEXTURE_2D);
        glColor3f(r * 0.8f, g * 0.8f, b * 0.8f); // Un poco más oscuro
        glBegin(GL_QUADS);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(x, -0.2f, z);
        glVertex3f(x, -0.2f, z + size);
        glVertex3f(x + size, -0.2f, z + size);
        glVertex3f(x + size, -0.2f, z);
        glEnd();

        glPopAttrib();
    }

    void drawSkybox() {
        glPushAttrib(GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT);

        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glDisable(GL_TEXTURE_2D);

        glPushMatrix();

        float skySize = 200.0f;

        glColor3f(0.5f, 0.7f, 1.0f);
        glBegin(GL_QUADS);
        glVertex3f(-skySize, skySize, -skySize);
        glVertex3f(skySize, skySize, -skySize);
        glVertex3f(skySize, skySize, skySize);
        glVertex3f(-skySize, skySize, skySize);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(0.7f, 0.8f, 1.0f);
        glVertex3f(-skySize, -skySize, skySize);
        glVertex3f(skySize, -skySize, skySize);
        glColor3f(0.5f, 0.7f, 1.0f);
        glVertex3f(skySize, skySize, skySize);
        glVertex3f(-skySize, skySize, skySize);

        glColor3f(0.7f, 0.8f, 1.0f);
        glVertex3f(-skySize, -skySize, -skySize);
        glVertex3f(-skySize, skySize, -skySize);
        glColor3f(0.5f, 0.7f, 1.0f);
        glVertex3f(skySize, skySize, -skySize);
        glVertex3f(skySize, -skySize, -skySize);

        glColor3f(0.7f, 0.8f, 1.0f);
        glVertex3f(-skySize, -skySize, -skySize);
        glVertex3f(-skySize, -skySize, skySize);
        glColor3f(0.5f, 0.7f, 1.0f);
        glVertex3f(-skySize, skySize, skySize);
        glVertex3f(-skySize, skySize, -skySize);

        glColor3f(0.7f, 0.8f, 1.0f);
        glVertex3f(skySize, -skySize, -skySize);
        glVertex3f(skySize, skySize, -skySize);
        glColor3f(0.5f, 0.7f, 1.0f);
        glVertex3f(skySize, skySize, skySize);
        glVertex3f(skySize, -skySize, skySize);
        glEnd();

        glPopMatrix();
        glPopAttrib();
    }

    void drawExtendedTerrain() {
        const float terrainSize = 500.0f;
        const float tileSize = 50.0f;
        const int tilesPerSide = (int)(terrainSize / tileSize);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        for (int i = -tilesPerSide; i < tilesPerSide; i++) {
            for (int j = -tilesPerSide; j < tilesPerSide; j++) {
                float x = i * tileSize;
                float z = j * tileSize;

                float distance = sqrtf(x * x + z * z);
                float colorVariation = 0.1f * sinf(distance * 0.01f);

                float r = 0.2f + colorVariation * 0.1f;
                float g = 0.9f + colorVariation * 0.1f;
                float b = 0.2f + colorVariation * 0.1f;

                r = fmaxf(0.0f, fminf(1.0f, r));
                g = fmaxf(0.0f, fminf(1.0f, g));
                b = fmaxf(0.0f, fminf(1.0f, b));

                drawTerrainTile(x, z, tileSize, r, g, b);
            }
        }

        // Deshabilitar texturas para las líneas de la grilla
        glDisable(GL_TEXTURE_2D);
        
        glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_LINE_BIT);

        glDisable(GL_LIGHTING);
        glDisable(GL_CULL_FACE);
        glColor3f(0.5f, 0.5f, 0.5f); // Color plomo/gris
        glLineWidth(1.0f); // Líneas más delgadas

        glBegin(GL_LINES);
        for (int i = -tilesPerSide; i <= tilesPerSide; i++) {
            float x = i * tileSize;
            glVertex3f(x, -0.25f, -terrainSize); // Más abajo para que no interfieran
            glVertex3f(x, -0.25f, terrainSize);
        }
        for (int j = -tilesPerSide; j <= tilesPerSide; j++) {
            float z = j * tileSize;
            glVertex3f(-terrainSize, -0.25f, z);
            glVertex3f(terrainSize, -0.25f, z);
        }
        glEnd();

        glPopAttrib();
    }

public:
    World() : textureLoaded(false) {
        // Inicializar semilla aleatoria
        srand(static_cast<unsigned>(time(0)));
        
        // Cargar la textura del terreno
        terrainTexture = loadBMPTexture("texture.bmp");
        if (terrainTexture != 0) {
            textureLoaded = true;
            printf("Textura del terreno cargada correctamente\n");
        } else {
            printf("Error al cargar la textura del terreno\n");
        }
        
        // Inicializar el MiniGame
        minigame.init(); // ¡NUEVA LÍNEA!
        // Opcional: configurar la posición del punto de misión y el número de esferas
        minigame.setMissionPoint(Vector3(10.0f, 0.0f, 5.0f)); // Ejemplo de nueva posición
        minigame.setTotalSpheres(7); // Ejemplo de más esferas
    }
    
    ~World() {
        // Liberar la textura si fue cargada
        if (textureLoaded) {
            glDeleteTextures(1, &terrainTexture);
        }
    }
    
    // Nuevo método para actualizar el mundo, incluyendo el minijuego
    void update(const Vector3& playerPos, float playerRadius) { // ¡NUEVO MÉTODO!
        // Actualizar el MiniGame
        minigame.update(playerPos, playerRadius);
        // Aquí puedes agregar lógica de actualización para otros elementos del mundo
        miCarro.update(); // Actualizar el carro también
    }

    void render() {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        drawSkybox();

        glClear(GL_DEPTH_BUFFER_BIT);

        drawExtendedTerrain();

        // Deshabilitar texturas para los objetos que no las usan
        glDisable(GL_TEXTURE_2D);

        // Enable lighting for lit objects like cubes and the house
        glEnable(GL_LIGHTING);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        // Draw Jaimito House
        glPushMatrix();
        glTranslatef(8.0f, 3.0f, -3.3f);
        
        casa.render();
        glPopMatrix();
        
        // Draw Clotilde House
        glPushMatrix();
        glTranslatef(25.0f, 0.0f, -6.0f);
        glScalef(1.4f, 1.4f,1.4f);
        glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
        casaclotilde.render();
        glPopMatrix();
        
        // Draw Florinda House
        glPushMatrix();
        glTranslatef(15.0f, 1.69f, -9.0f);
        glScalef(0.9f, 0.9f, 0.9f);
        casaflorinda.render();
        glPopMatrix();
        
        // Draw Ramon House
        glPushMatrix();
        glTranslatef(26.0f, 0.0f, 7.9f);
        glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
        glScalef(0.9f, 0.9f, 0.9f);
        casaramon.render();
        glPopMatrix();
        
        // Draw Barril
        glPushMatrix();
        glTranslatef(10.0f, 0.5f, 2.0f);
        glScalef(0.64f, 0.64f, 0.64f);
        barril.render();
        glPopMatrix();
        
        // Draw Pelota
        glPushMatrix();
        glTranslatef(14.0f, 0.3f, 0.0f);
        glScalef(0.34f, 0.34f, 0.34f);
        pelota.render();
        glPopMatrix();
        
        // Draw Escalera
        glPushMatrix();
        glTranslatef(8.0f, 0.0f, 4.0f);
        glScalef(0.74f, 0.74f, 0.74f);
        escalera.render();
        glPopMatrix();
        
        // Draw Pileta
        glPushMatrix();
        glTranslatef(40.0f, 0.3f, 0.0f);
        glScalef(1.34f, 1.34f, 1.34f);
        pileta.render();
        glPopMatrix();
        
        // Draw MUro
        glPushMatrix();
        glTranslatef(32.0f, -0.1f, 0.0f);
        glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
        glScalef(1.34f, 1.34f, 1.34f);
        muros.render();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(38.0f, 0.0f, 0.0f);
        pista.draw();

        // En tu loop de renderizado:
        glPushMatrix();
        glTranslatef(0.0f, 0.9f, 0.0f);
        // miCarro.update(); // Mover esta actualización al método update(playerPos, playerRadius)
        miCarro.draw();  // Dibujar carro
        miCarro.setSeparationRange(10.0f, 30.0f);
        miCarro.setSpeedRange(0.08f);
        miCarro.randomizeNow();
        glPopMatrix();

        glPopMatrix();

        // Renderizar el MiniGame
        // Es importante deshabilitar la iluminación antes de renderizar el texto del minijuego
        // y habilitarla de nuevo después, ya que el texto no debe ser afectado por la luz.
        glDisable(GL_LIGHTING); // Deshabilitar iluminación para el UI del minijuego
        minigame.render();      // ¡NUEVA LÍNEA!
        glEnable(GL_LIGHTING);  // Habilitar iluminación de nuevo para el resto del mundo
    }

    // Puedes agregar getters para la información del minijuego si es necesario fuera de World
    // Por ejemplo:
    Vector3 getMiniGameMissionPoint() const {
        return minigame.getMissionPoint();
    }
    float getMiniGameMissionRadius() const {
        return minigame.getMissionRadius();
    }
    // ... otros getters
};

#endif // WORLD_H
