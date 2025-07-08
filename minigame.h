#ifndef MINIGAME_H
#define MINIGAME_H

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <sstream>

// Estructura para posiciones 3D
struct Vector3 {
    float x, y, z;
    Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
};

// Estructura para esferas
struct Sphere {
    Vector3 pos;
    float radius;
    bool active;
    float colorR, colorG, colorB;
    
    Sphere() : pos(0, 0, 0), radius(0.5f), active(true), colorR(1.0f), colorG(0.0f), colorB(0.0f) {}
    Sphere(Vector3 position, float r) : pos(position), radius(r), active(true), colorR(1.0f), colorG(0.0f), colorB(0.0f) {}
};

// Estados del juego
enum MiniGameState {
    MINIGAME_WAITING,
    MINIGAME_COLLECTING,
    MINIGAME_WON,
    MINIGAME_GAME_OVER
};

class MiniGame {
private:
    // Variables del juego
    MiniGameState gameState;
    float winTimer;
    float winDisplayTime;
    Vector3 missionPoint;
    float missionRadius;
    float glowIntensity;
    float glowDirection;
    
    std::vector<Sphere> collectSpheres;
    int collectedCount;
    int totalSpheres;
    
    // Utilidades
    float distance2D(const Vector3& a, const Vector3& b);
    void generateRandomSpheres();
    void drawMissionPoint();
    void drawSphere(const Vector3& pos, float radius, float r, float g, float b);
    void drawText(const std::string& text, float x, float y);
    void drawCenteredText(const std::string& text, float y);
    
public:
    // Constructor y destructor
    MiniGame();
    ~MiniGame();
    
    // Métodos públicos
    void init();
    void update(const Vector3& playerPos, float playerRadius);
    void render();
    void reset();
    
    // Getters
    bool isActive() const;
    bool isWon() const;
    bool isCollecting() const;
    int getCollectedCount() const;
    int getTotalSpheres() const;
    Vector3 getMissionPoint() const;
    float getMissionRadius() const;
    
    // Setters
    void setMissionPoint(const Vector3& point);
    void setTotalSpheres(int count);
};

// Implementación de métodos inline
inline bool MiniGame::isActive() const {
    return gameState != MINIGAME_WAITING;
}

inline bool MiniGame::isWon() const {
    return gameState == MINIGAME_WON;
}

inline bool MiniGame::isCollecting() const {
    return gameState == MINIGAME_COLLECTING;
}

inline int MiniGame::getCollectedCount() const {
    return collectedCount;
}

inline int MiniGame::getTotalSpheres() const {
    return totalSpheres;
}

inline Vector3 MiniGame::getMissionPoint() const {
    return missionPoint;
}

inline float MiniGame::getMissionRadius() const {
    return missionRadius;
}

inline void MiniGame::setMissionPoint(const Vector3& point) {
    missionPoint = point;
}

inline void MiniGame::setTotalSpheres(int count) {
    totalSpheres = count;
}

// Implementación de métodos
MiniGame::MiniGame() : gameState(MINIGAME_WAITING), winTimer(0.0f), winDisplayTime(3.0f),
                        missionPoint(5.0f, 0.0f, 5.0f), missionRadius(1.2f),
                        glowIntensity(0.0f), glowDirection(1.0f),
                        collectedCount(0), totalSpheres(5) {
}

MiniGame::~MiniGame() {
}

void MiniGame::init() {
    gameState = MINIGAME_WAITING;
    winTimer = 0.0f;
    glowIntensity = 0.0f;
    glowDirection = 1.0f;
    collectedCount = 0;
    collectSpheres.clear();
    
    // Inicializar random si no se ha hecho
    static bool randomInitialized = false;
    if (!randomInitialized) {
        srand(time(NULL));
        randomInitialized = true;
    }
}

float MiniGame::distance2D(const Vector3& a, const Vector3& b) {
    float dx = a.x - b.x;
    float dz = a.z - b.z;
    return sqrt(dx*dx + dz*dz);
}

void MiniGame::generateRandomSpheres() {
    collectSpheres.clear();
    collectedCount = 0;
    
    for (int i = 0; i < totalSpheres; i++) {
        Vector3 pos;
        bool validPos = false;
        int attempts = 0;
        
        // Generar posición válida sobre el suelo
        while (!validPos && attempts < 50) {
            pos.x = ((float)rand() / RAND_MAX) * 16.0f - 8.0f; // -8 a 8
            pos.y = 0.4f; // Sobre el suelo
            pos.z = ((float)rand() / RAND_MAX) * 16.0f - 8.0f; // -8 a 8
            
            validPos = true;
            
            // Verificar distancia del punto de misión
            if (distance2D(pos, missionPoint) < 2.0f) {
                validPos = false;
            }
            
            // Verificar distancia de otras esferas
            for (size_t j = 0; j < collectSpheres.size(); j++) {
                if (distance2D(pos, collectSpheres[j].pos) < 1.5f) {
                    validPos = false;
                    break;
                }
            }
            
            attempts++;
        }
        
        Sphere sphere(pos, 0.4f);
        // Colores brillantes para las esferas de recolección
        sphere.colorR = ((float)rand() / RAND_MAX) * 0.4f + 0.6f;
        sphere.colorG = ((float)rand() / RAND_MAX) * 0.4f + 0.6f;
        sphere.colorB = ((float)rand() / RAND_MAX) * 0.4f + 0.6f;
        
        collectSpheres.push_back(sphere);
    }
}

void MiniGame::drawMissionPoint() {
    glPushMatrix();
    glTranslatef(missionPoint.x, missionPoint.y, missionPoint.z);
    
    // Dibujar círculo en el suelo con efecto de brillo
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Círculo exterior con brillo
    float outerGlow = 0.3f + glowIntensity * 0.4f;
    glColor4f(1.0f, 0.2f, 0.2f, outerGlow);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, 0.01f, 0.0f);
    for (int i = 0; i <= 32; i++) {
        float angle = (float)i * 2.0f * 3.14159f / 32.0f;
        float x = cos(angle) * missionRadius;
        float z = sin(angle) * missionRadius;
        glVertex3f(x, 0.01f, z);
    }
    glEnd();
    
    // Círculo interior más brillante
    float innerGlow = 0.6f + glowIntensity * 0.4f;
    glColor4f(1.0f, 0.4f, 0.4f, innerGlow);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, 0.02f, 0.0f);
    for (int i = 0; i <= 32; i++) {
        float angle = (float)i * 2.0f * 3.14159f / 32.0f;
        float x = cos(angle) * (missionRadius * 0.6f);
        float z = sin(angle) * (missionRadius * 0.6f);
        glVertex3f(x, 0.02f, z);
    }
    glEnd();
    
    // Círculo central muy brillante
    glColor4f(1.0f, 0.8f, 0.8f, 0.8f + glowIntensity * 0.2f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, 0.03f, 0.0f);
    for (int i = 0; i <= 32; i++) {
        float angle = (float)i * 2.0f * 3.14159f / 32.0f;
        float x = cos(angle) * (missionRadius * 0.3f);
        float z = sin(angle) * (missionRadius * 0.3f);
        glVertex3f(x, 0.03f, z);
    }
    glEnd();
    
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

void MiniGame::drawSphere(const Vector3& pos, float radius, float r, float g, float b) {
    glPushMatrix();
    glTranslatef(pos.x, pos.y, pos.z);
    glColor3f(r, g, b);
    glutSolidSphere(radius, 16, 16);
    glPopMatrix();
}

void MiniGame::drawText(const std::string& text, float x, float y) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(x, y);
    
    for (size_t i = 0; i < text.length(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
    
    glEnable(GL_LIGHTING);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void MiniGame::drawCenteredText(const std::string& text, float y) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 0.0f); // Amarillo brillante
    
    // Calcular posición centrada
    float textWidth = 0;
    for (size_t i = 0; i < text.length(); i++) {
        textWidth += glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
    }
    
    float centerX = (glutGet(GLUT_WINDOW_WIDTH) - textWidth) / 2.0f;
    glRasterPos2f(centerX, y);
    
    for (size_t i = 0; i < text.length(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
    }
    
    glEnable(GL_LIGHTING);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void MiniGame::update(const Vector3& playerPos, float playerRadius) {
    // Actualizar efecto de brillo del punto de misión
    glowIntensity += glowDirection * 0.03f;
    if (glowIntensity > 1.0f) {
        glowIntensity = 1.0f;
        glowDirection = -1.0f;
    } else if (glowIntensity < 0.0f) {
        glowIntensity = 0.0f;
        glowDirection = 1.0f;
    }
    
    // Lógica del juego
    if (gameState == MINIGAME_WAITING) {
        // Verificar si está dentro del punto de misión
        if (distance2D(playerPos, missionPoint) < missionRadius) {
            gameState = MINIGAME_COLLECTING;
            generateRandomSpheres();
        }
    }
    else if (gameState == MINIGAME_COLLECTING) {
        // Verificar colisiones con esferas de recolección
        for (size_t i = 0; i < collectSpheres.size(); i++) {
            if (collectSpheres[i].active) {
                if (distance2D(playerPos, collectSpheres[i].pos) < (playerRadius + collectSpheres[i].radius)) {
                    collectSpheres[i].active = false;
                    collectedCount++;
                    
                    if (collectedCount >= totalSpheres) {
                        gameState = MINIGAME_WON;
                        winTimer = 0.0f;
                    }
                }
            }
        }
    }
    else if (gameState == MINIGAME_WON) {
        winTimer += 0.016f; // Aproximadamente 60 FPS
        if (winTimer >= winDisplayTime) {
            reset();
        }
    }
}

void MiniGame::render() {
    // Dibujar elementos según estado del juego
    if (gameState == MINIGAME_WAITING) {
        // Dibujar punto de misión con efecto de brillo
        drawMissionPoint();
        
        // Texto de instrucciones
        drawText("Entra en el circulo rojo para empezar la mision", 20, 30);
        drawText("Minijuego: Recoleccion de esferas", 20, 50);
    }
    else if (gameState == MINIGAME_COLLECTING) {
        // Dibujar esferas de recolección
        for (size_t i = 0; i < collectSpheres.size(); i++) {
            if (collectSpheres[i].active) {
                drawSphere(collectSpheres[i].pos, collectSpheres[i].radius,
                            collectSpheres[i].colorR, collectSpheres[i].colorG, collectSpheres[i].colorB);
            }
        }
        
        // Texto de estado
        drawText("Recolectando esferas...", 20, 30);
        
        std::stringstream ss;
        ss << "Progreso: " << collectedCount << "/" << totalSpheres;
        drawText(ss.str(), 20, 50);
    }
    else if (gameState == MINIGAME_WON) {
        // Dibujar punto de misión de nuevo (para próxima partida)
        drawMissionPoint();
        
        // Mensaje de victoria centrado y grande
        drawCenteredText("MISION COMPLETADA!", glutGet(GLUT_WINDOW_HEIGHT) / 2.0f);
        drawCenteredText("Regresando...", glutGet(GLUT_WINDOW_HEIGHT) / 2.0f + 40);
    }
}

void MiniGame::reset() {
    gameState = MINIGAME_WAITING;
    winTimer = 0.0f;
    collectedCount = 0;
    collectSpheres.clear();
}

#endif // MINIGAME_H
