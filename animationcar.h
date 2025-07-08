#ifndef ANIMATIONCAR_H
#define ANIMATIONCAR_H
#include "car.h"
#include <cstdlib>
#include <ctime>

class AnimationCar {
private:
    static const int NUM_CARS = 7;
    Car cars[NUM_CARS];
    
    // Variables para movimiento autom�tico
    bool autoMove;
    float carPosX[NUM_CARS];
    float carPosZ[NUM_CARS];
    float carAngle[NUM_CARS];
    int moveState[NUM_CARS]; // 0: derecha, 1: abajo, 2: izquierda, 3: arriba
    float squareSize;
    float baseMoveSpeed;
    
    // Variables para comportamiento randomizado
    float carSpeed[NUM_CARS];           // Velocidad individual de cada carro
    float targetSeparation[NUM_CARS];   // Separaci�n objetivo din�mica
    float currentSeparation[NUM_CARS];  // Separaci�n actual
    float minSeparation;                // Separaci�n m�nima entre carros
    float maxSeparation;                // Separaci�n m�xima entre carros
    float separationChangeRate;         // Velocidad de cambio de separaci�n
    int speedChangeTimer[NUM_CARS];     // Timer para cambiar velocidad
    int separationChangeTimer[NUM_CARS]; // Timer para cambiar separaci�n
    
    // Colores para diferenciar los carros
    float carColors[NUM_CARS][3];
    
    // Funci�n helper para generar n�meros aleatorios
    float randomFloat(float min, float max) {
        return min + (float(rand()) / float(RAND_MAX)) * (max - min);
    }
    
    int randomInt(int min, int max) {
        return min + rand() % (max - min + 1);
    }
    
public:
    // Constructor
    AnimationCar() {
        // Inicializar generador de n�meros aleatorios
        srand(static_cast<unsigned int>(time(0)));
        
        autoMove = true;
        squareSize = 60.0f;
        baseMoveSpeed = 0.05f;
        minSeparation = 55.0f;   // Separaci�n m�nima inicial m�s grande
        maxSeparation = 80.0f;   // Separaci�n m�xima inicial m�s grande
        separationChangeRate = 0.02f;
        
        // Inicializar colores diferentes para cada carro
        carColors[0][0] = 1.0f; carColors[0][1] = 0.85f; carColors[0][2] = 0.0f; // Amarillo
        carColors[1][0] = 1.0f; carColors[1][1] = 0.2f; carColors[1][2] = 0.2f;  // Rojo
        carColors[2][0] = 0.2f; carColors[2][1] = 0.4f; carColors[2][2] = 1.0f;  // Azul
        carColors[3][0] = 0.2f; carColors[3][1] = 0.8f; carColors[3][2] = 0.2f;  // Verde
        carColors[4][0] = 0.8f; carColors[4][1] = 0.2f; carColors[4][2] = 0.8f;  // Morado
        carColors[5][0] = 1.0f; carColors[5][1] = 0.6f; carColors[5][2] = 0.0f;  // Naranja
        carColors[6][0] = 0.9f; carColors[6][1] = 0.9f; carColors[6][2] = 0.9f;  // Blanco
        
        // Inicializar valores aleatorios para cada carro
        for(int i = 0; i < NUM_CARS; i++) {
            // Velocidades aleatorias (algunos m�s r�pidos, otros m�s lentos)
            carSpeed[i] = randomFloat(0.03f, 0.08f);
            
            // Separaciones aleatorias iniciales
            targetSeparation[i] = randomFloat(minSeparation, maxSeparation);
            currentSeparation[i] = targetSeparation[i];
            
            // Timers aleatorios para cambios
            speedChangeTimer[i] = randomInt(60, 180);  // Cambiar velocidad cada 1-3 segundos
            separationChangeTimer[i] = randomInt(120, 300); // Cambiar separaci�n cada 2-5 segundos
            
            // Posiciones iniciales con separaci�n aleatoria
            float totalSeparation = 0.0f;
            for(int j = 0; j < i; j++) {
                totalSeparation += currentSeparation[j];
            }
            
            carPosX[i] = -totalSeparation;
            carPosZ[i] = -squareSize;
            carAngle[i] = 0.0f;
            moveState[i] = 0;
        }
    }
    
    // Constructor con par�metros personalizados
    AnimationCar(float size, float speed) {
        srand(static_cast<unsigned int>(time(0)));
        
        autoMove = true;
        squareSize = size;
        baseMoveSpeed = speed;
        minSeparation = 20.0f;   // Separaci�n m�nima inicial m�s grande
        maxSeparation = 50.0f;   // Separaci�n m�xima inicial m�s grande
        separationChangeRate = 0.02f;
        
        // Inicializar colores (mismo c�digo que arriba)
        carColors[0][0] = 1.0f; carColors[0][1] = 0.85f; carColors[0][2] = 0.0f;
        carColors[1][0] = 1.0f; carColors[1][1] = 0.2f; carColors[1][2] = 0.2f;
        carColors[2][0] = 0.2f; carColors[2][1] = 0.4f; carColors[2][2] = 1.0f;
        carColors[3][0] = 0.2f; carColors[3][1] = 0.8f; carColors[3][2] = 0.2f;
        carColors[4][0] = 0.8f; carColors[4][1] = 0.2f; carColors[4][2] = 0.8f;
        carColors[5][0] = 1.0f; carColors[5][1] = 0.6f; carColors[5][2] = 0.0f;
        carColors[6][0] = 0.9f; carColors[6][1] = 0.9f; carColors[6][2] = 0.9f;
        
        // Inicializar valores aleatorios
        for(int i = 0; i < NUM_CARS; i++) {
            carSpeed[i] = randomFloat(speed * 0.6f, speed * 1.4f);
            targetSeparation[i] = randomFloat(minSeparation, maxSeparation);
            currentSeparation[i] = targetSeparation[i];
            speedChangeTimer[i] = randomInt(60, 180);
            separationChangeTimer[i] = randomInt(120, 300);
            
            float totalSeparation = 0.0f;
            for(int j = 0; j < i; j++) {
                totalSeparation += currentSeparation[j];
            }
            
            carPosX[i] = -totalSeparation;
            carPosZ[i] = -squareSize;
            carAngle[i] = 0.0f;
            moveState[i] = 0;
        }
    }
    
private:
    // Funci�n para calcular la posici�n total en el per�metro
    float calculateTotalPerimeter() {
        return 8.0f * squareSize; // 4 lados * 2 * squareSize
    }
    
    // Funci�n para obtener la posici�n en el per�metro con separaci�n din�mica
    float getPerimeterPosition(int carIndex) {
        float totalPerimeter = calculateTotalPerimeter();
        float basePosition = 0.0f;
        
        // Calcular posici�n base del carro l�der
        switch(moveState[0]) {
            case 0: // Derecha
                basePosition = carPosX[0] + squareSize;
                break;
            case 1: // Abajo
                basePosition = 2.0f * squareSize + (carPosZ[0] + squareSize);
                break;
            case 2: // Izquierda
                basePosition = 4.0f * squareSize + (squareSize - carPosX[0]);
                break;
            case 3: // Arriba
                basePosition = 6.0f * squareSize + (squareSize - carPosZ[0]);
                break;
        }
        
        // Calcular posici�n del carro actual con separaci�n din�mica
        float totalSeparation = 0.0f;
        for(int i = 0; i < carIndex; i++) {
            totalSeparation += currentSeparation[i];
        }
        
        float carPosition = basePosition - totalSeparation;
        
        // Manejar wrap-around
        while(carPosition < 0) carPosition += totalPerimeter;
        while(carPosition >= totalPerimeter) carPosition -= totalPerimeter;
        
        return carPosition;
    }
    
    // Funci�n para convertir posici�n en per�metro a coordenadas
    void perimeterToCoords(float perimeterPos, float &x, float &z, float &angle, int &state) {
        float sideLength = 2.0f * squareSize;
        
        if(perimeterPos < sideLength) {
            // Lado inferior (derecha)
            x = -squareSize + perimeterPos;
            z = -squareSize;
            angle = 90.0f;
            state = 0;
        } else if(perimeterPos < 2.0f * sideLength) {
            // Lado derecho (abajo)
            x = squareSize;
            z = -squareSize + (perimeterPos - sideLength);
            angle = 180.0f;
            state = 1;
        } else if(perimeterPos < 3.0f * sideLength) {
            // Lado superior (izquierda)
            x = squareSize - (perimeterPos - 2.0f * sideLength);
            z = squareSize;
            angle = 270.0f;
            state = 2;
        } else {
            // Lado izquierdo (arriba)
            x = -squareSize;
            z = squareSize - (perimeterPos - 3.0f * sideLength);
            angle = 0.0f;
            state = 3;
        }
    }
    
    // Funci�n para actualizar comportamiento aleatorio
    void updateRandomBehavior() {
        for(int i = 0; i < NUM_CARS; i++) {
            // Actualizar timer de velocidad
            speedChangeTimer[i]--;
            if(speedChangeTimer[i] <= 0) {
                // Cambiar velocidad aleatoriamente
                carSpeed[i] = randomFloat(baseMoveSpeed * 0.6f, baseMoveSpeed * 1.4f);
                speedChangeTimer[i] = randomInt(60, 180);
            }
            
            // Actualizar timer de separaci�n
            separationChangeTimer[i]--;
            if(separationChangeTimer[i] <= 0) {
                // Cambiar separaci�n objetivo
                targetSeparation[i] = randomFloat(minSeparation, maxSeparation);
                separationChangeTimer[i] = randomInt(120, 300);
            }
            
            // Interpolar suavemente hacia la separaci�n objetivo
            float diff = targetSeparation[i] - currentSeparation[i];
            if(diff > 0.1f) {
                currentSeparation[i] += separationChangeRate;
                if(currentSeparation[i] > targetSeparation[i]) {
                    currentSeparation[i] = targetSeparation[i];
                }
            } else if(diff < -0.1f) {
                currentSeparation[i] -= separationChangeRate;
                if(currentSeparation[i] < targetSeparation[i]) {
                    currentSeparation[i] = targetSeparation[i];
                }
            }
        }
    }
    
public:
    // Funci�n para actualizar el movimiento autom�tico
    void update() {
        if(!autoMove) return;
        
        // Actualizar comportamiento aleatorio
        updateRandomBehavior();
        
        // Animar ruedas de todos los carros
        for(int i = 0; i < NUM_CARS; i++) {
            cars[i].animateWheels();
        }
        
        // Mover el carro l�der (�ndice 0) con su velocidad espec�fica
        switch(moveState[0]) {
            case 0: // Moverse a la derecha
                carPosX[0] += carSpeed[0];
                carAngle[0] = 90.0f;
                if(carPosX[0] >= squareSize) {
                    carPosX[0] = squareSize;
                    moveState[0] = 1;
                }
                break;
            case 1: // Moverse hacia abajo
                carPosZ[0] += carSpeed[0];
                carAngle[0] = 180.0f;
                if(carPosZ[0] >= squareSize) {
                    carPosZ[0] = squareSize;
                    moveState[0] = 2;
                }
                break;
            case 2: // Moverse a la izquierda
                carPosX[0] -= carSpeed[0];
                carAngle[0] = 270.0f;
                if(carPosX[0] <= -squareSize) {
                    carPosX[0] = -squareSize;
                    moveState[0] = 3;
                }
                break;
            case 3: // Moverse hacia arriba
                carPosZ[0] -= carSpeed[0];
                carAngle[0] = 0.0f;
                if(carPosZ[0] <= -squareSize) {
                    carPosZ[0] = -squareSize;
                    moveState[0] = 0;
                }
                break;
        }
        
        // Actualizar posiciones de los carros seguidores
        for(int i = 1; i < NUM_CARS; i++) {
            float perimeterPos = getPerimeterPosition(i);
            perimeterToCoords(perimeterPos, carPosX[i], carPosZ[i], carAngle[i], moveState[i]);
        }
    }
    
    // Funci�n para dibujar todos los carros con animaci�n
    void draw() {
        for(int i = 0; i < NUM_CARS; i++) {
            glPushMatrix();
            
            // Aplicar color espec�fico para cada carro
            glColor3f(carColors[i][0], carColors[i][1], carColors[i][2]);
            
            // Dibujar el carro en su posici�n
            cars[i].drawAt(carPosX[i], 0, carPosZ[i], carAngle[i]);
            
            glPopMatrix();
        }
    }
    
    // Funci�n para pausar/reanudar la animaci�n
    void toggleAnimation() {
        autoMove = !autoMove;
    }
    
    // Funci�n para reiniciar la animaci�n con nuevos valores aleatorios
    void reset() {
        // Reinicializar valores aleatorios
        for(int i = 0; i < NUM_CARS; i++) {
            carSpeed[i] = randomFloat(baseMoveSpeed * 0.6f, baseMoveSpeed * 1.4f);
            targetSeparation[i] = randomFloat(minSeparation, maxSeparation);
            currentSeparation[i] = targetSeparation[i];
            speedChangeTimer[i] = randomInt(60, 180);
            separationChangeTimer[i] = randomInt(120, 300);
            
            float totalSeparation = 0.0f;
            for(int j = 0; j < i; j++) {
                totalSeparation += currentSeparation[j];
            }
            
            carPosX[i] = -totalSeparation;
            carPosZ[i] = -squareSize;
            carAngle[i] = 0.0f;
            moveState[i] = 0;
        }
        autoMove = true;
    }
    
    // Funci�n para cambiar el rango de separaci�n
    void setSeparationRange(float minSep, float maxSep) {
        minSeparation = minSep;
        maxSeparation = maxSep;
        
        // Actualizar separaciones actuales si est�n fuera del rango
        for(int i = 0; i < NUM_CARS; i++) {
            if(targetSeparation[i] < minSeparation) targetSeparation[i] = minSeparation;
            if(targetSeparation[i] > maxSeparation) targetSeparation[i] = maxSeparation;
        }
    }
    
    // Funci�n para cambiar el rango de velocidad
    void setSpeedRange(float baseSpeed) {
        baseMoveSpeed = baseSpeed;
        
        // Actualizar velocidades actuales
        for(int i = 0; i < NUM_CARS; i++) {
            carSpeed[i] = randomFloat(baseSpeed * 0.6f, baseSpeed * 1.4f);
        }
    }
    
    // Getters existentes
    bool isAnimating() const { return autoMove; }
    int getNumCars() const { return NUM_CARS; }
    float getSquareSize() const { return squareSize; }
    float getMoveSpeed() const { return baseMoveSpeed; }
    
    // Nuevos getters para valores aleatorios
    float getCarSpeed(int index) const {
        if(index >= 0 && index < NUM_CARS) return carSpeed[index];
        return 0.0f;
    }
    
    float getCurrentSeparation(int index) const {
        if(index >= 0 && index < NUM_CARS) return currentSeparation[index];
        return 0.0f;
    }
    
    float getTargetSeparation(int index) const {
        if(index >= 0 && index < NUM_CARS) return targetSeparation[index];
        return 0.0f;
    }
    
    // Getters de posici�n existentes
    float getCarPosX(int index) const { 
        if(index >= 0 && index < NUM_CARS) return carPosX[index]; 
        return 0.0f; 
    }
    
    float getCarPosZ(int index) const { 
        if(index >= 0 && index < NUM_CARS) return carPosZ[index]; 
        return 0.0f; 
    }
    
    float getCarAngle(int index) const { 
        if(index >= 0 && index < NUM_CARS) return carAngle[index]; 
        return 0.0f; 
    }
    
    // Setters
    void setSquareSize(float size) { squareSize = size; }
    void setAutoMove(bool enable) { autoMove = enable; }
    
    // Funci�n para cambiar el color de un carro espec�fico
    void setCarColor(int index, float r, float g, float b) {
        if(index >= 0 && index < NUM_CARS) {
            carColors[index][0] = r;
            carColors[index][1] = g;
            carColors[index][2] = b;
        }
    }
    
    // Funci�n para dibujar las l�neas de la ruta
    void drawPath() {
        glColor3f(1.0f, 1.0f, 1.0f);
        glLineWidth(2.0f);
        glBegin(GL_LINE_LOOP);
        glVertex3f(-squareSize, -0.01f, -squareSize);
        glVertex3f(squareSize, -0.01f, -squareSize);
        glVertex3f(squareSize, -0.01f, squareSize);
        glVertex3f(-squareSize, -0.01f, squareSize);
        glEnd();
    }
    
    // Funci�n para forzar cambios aleatorios inmediatos
    void randomizeNow() {
        for(int i = 0; i < NUM_CARS; i++) {
            carSpeed[i] = randomFloat(baseMoveSpeed * 0.6f, baseMoveSpeed * 1.4f);
            targetSeparation[i] = randomFloat(minSeparation, maxSeparation);
            speedChangeTimer[i] = randomInt(60, 180);
            separationChangeTimer[i] = randomInt(120, 300);
        }
    }
};

#endif // ANIMATIONCAR_H
