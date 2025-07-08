#ifndef MULTIPLECARANIMATION_H
#define MULTIPLECARANIMATION_H

#include "car.h"
#include <vector>
#include <random>

// Estructura para representar un carro individual
struct CarInstance {
    Car car;
    float posX;
    float posZ;
    float angle;
    int moveState;
    float moveSpeed;
    float squareSize;
    float offset; // Desplazamiento en la ruta para evitar colisiones
    
    CarInstance(float size = 8.0f, float speed = 0.05f, float startOffset = 0.0f) {
        posX = 0.0f;
        posZ = 0.0f;
        angle = 0.0f;
        moveState = 0;
        squareSize = size;
        moveSpeed = speed;
        offset = startOffset;
        
        // Aplicar offset inicial
        applyOffset();
    }
    
    void applyOffset() {
        float totalPerimeter = squareSize * 8.0f; // Perímetro total del cuadrado
        float offsetDistance = offset * totalPerimeter;
        
        // Calcular posición inicial basada en el offset
        if (offsetDistance <= squareSize * 2) {
            // Lado derecho
            posX = offsetDistance - squareSize;
            posZ = -squareSize;
            angle = 90.0f;
            moveState = 0;
        } else if (offsetDistance <= squareSize * 4) {
            // Lado inferior
            posX = squareSize;
            posZ = (offsetDistance - squareSize * 2) - squareSize;
            angle = 180.0f;
            moveState = 1;
        } else if (offsetDistance <= squareSize * 6) {
            // Lado izquierdo
            posX = squareSize - (offsetDistance - squareSize * 4);
            posZ = squareSize;
            angle = 270.0f;
            moveState = 2;
        } else {
            // Lado superior
            posX = -squareSize;
            posZ = squareSize - (offsetDistance - squareSize * 6);
            angle = 0.0f;
            moveState = 3;
        }
    }
    
    void update() {
        // Animar ruedas
        car.animateWheels();
        
        // Movimiento en cuadrilátero
        switch(moveState) {
            case 0: // Moverse a la derecha
                posX += moveSpeed;
                angle = 90.0f;
                if(posX >= squareSize) {
                    posX = squareSize;
                    moveState = 1;
                }
                break;
            case 1: // Moverse hacia abajo
                posZ += moveSpeed;
                angle = 180.0f;
                if(posZ >= squareSize) {
                    posZ = squareSize;
                    moveState = 2;
                }
                break;
            case 2: // Moverse a la izquierda
                posX -= moveSpeed;
                angle = 270.0f;
                if(posX <= -squareSize) {
                    posX = -squareSize;
                    moveState = 3;
                }
                break;
            case 3: // Moverse hacia arriba
                posZ -= moveSpeed;
                angle = 0.0f;
                if(posZ <= -squareSize) {
                    posZ = -squareSize;
                    moveState = 0;
                }
                break;
        }
    }
    
    void draw() {
        car.drawAt(posX, 0, posZ, angle);
    }
};

class MultipleCarAnimation {
private:
    std::vector<CarInstance> cars;
    bool autoMove;
    float baseSquareSize;
    float baseMoveSpeed;
    
    // Generador de números aleatorios
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<float> speedDist;
    std::uniform_real_distribution<float> offsetDist;
    
public:
    // Constructor
    MultipleCarAnimation(int numCars = 4, float squareSize = 8.0f, float baseSpeed = 0.05f) 
        : gen(rd()), speedDist(0.8f, 1.2f), offsetDist(0.0f, 1.0f) {
        autoMove = true;
        baseSquareSize = squareSize;
        baseMoveSpeed = baseSpeed;
        
        // Crear carros con diferentes velocidades y posiciones iniciales
        for(int i = 0; i < numCars; i++) {
            float speed = baseMoveSpeed * speedDist(gen);
            float offset = static_cast<float>(i) / numCars; // Distribución uniforme
            cars.emplace_back(squareSize, speed, offset);
        }
    }
    
    // Constructor con configuración personalizada
    MultipleCarAnimation(const std::vector<std::pair<float, float>>& carConfigs, float squareSize = 8.0f) 
        : gen(rd()), speedDist(0.8f, 1.2f), offsetDist(0.0f, 1.0f) {
        autoMove = true;
        baseSquareSize = squareSize;
        baseMoveSpeed = 0.05f;
        
        // Crear carros con configuraciones específicas (velocidad, offset)
        for(const auto& config : carConfigs) {
            cars.emplace_back(squareSize, config.first, config.second);
        }
    }
    
    // Función para actualizar todos los carros
    void update() {
        if(!autoMove) return;
        
        for(auto& car : cars) {
            car.update();
        }
    }
    
    // Función para dibujar todos los carros
    void draw() {
        for(auto& car : cars) {
            car.draw();
        }
    }
    
    // Función para dibujar la ruta
    void drawPath() {
        glColor3f(1.0f, 1.0f, 1.0f);
        glLineWidth(2.0f);
        glBegin(GL_LINE_LOOP);
        glVertex3f(-baseSquareSize, -0.01f, -baseSquareSize);
        glVertex3f(baseSquareSize, -0.01f, -baseSquareSize);
        glVertex3f(baseSquareSize, -0.01f, baseSquareSize);
        glVertex3f(-baseSquareSize, -0.01f, baseSquareSize);
        glEnd();
    }
    
    // Función para agregar un carro nuevo
    void addCar(float speed = 0.05f, float offset = 0.0f) {
        cars.emplace_back(baseSquareSize, speed, offset);
    }
    
    // Función para agregar carros con velocidades aleatorias
    void addRandomCars(int count) {
        for(int i = 0; i < count; i++) {
            float speed = baseMoveSpeed * speedDist(gen);
            float offset = offsetDist(gen);
            cars.emplace_back(baseSquareSize, speed, offset);
        }
    }
    
    // Función para pausar/reanudar la animación
    void toggleAnimation() {
        autoMove = !autoMove;
    }
    
    // Función para reiniciar todos los carros
    void reset() {
        for(int i = 0; i < cars.size(); i++) {
            cars[i].posX = 0.0f;
            cars[i].posZ = 0.0f;
            cars[i].angle = 0.0f;
            cars[i].moveState = 0;
            cars[i].offset = static_cast<float>(i) / cars.size();
            cars[i].applyOffset();
        }
        autoMove = true;
    }
    
    // Función para cambiar la velocidad de todos los carros
    void changeSpeed(float multiplier) {
        for(auto& car : cars) {
            car.moveSpeed = baseMoveSpeed * multiplier;
        }
    }
    
    // Función para cambiar el tamaño de la ruta
    void changeSquareSize(float newSize) {
        baseSquareSize = newSize;
        for(auto& car : cars) {
            car.squareSize = newSize;
        }
    }
    
    // Getters
    bool isAnimating() const { return autoMove; }
    int getCarCount() const { return cars.size(); }
    float getSquareSize() const { return baseSquareSize; }
    float getBaseSpeed() const { return baseMoveSpeed; }
    
    // Función para obtener información de un carro específico
    CarInstance* getCar(int index) {
        if(index >= 0 && index < cars.size()) {
            return &cars[index];
        }
        return nullptr;
    }
    
    // Función para eliminar todos los carros
    void clearCars() {
        cars.clear();
    }
    
    // Función para crear una formación específica de carros
    void createFormation(int numCars, float speedVariation = 0.2f) {
        cars.clear();
        
        for(int i = 0; i < numCars; i++) {
            float speed = baseMoveSpeed * (1.0f + speedVariation * (static_cast<float>(i) / numCars - 0.5f));
            float offset = static_cast<float>(i) / numCars;
            cars.emplace_back(baseSquareSize, speed, offset);
        }
    }
};

#endif // MULTIPLECARANIMATION_H
