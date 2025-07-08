#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <fstream>

// Define M_PI if it's not already defined (common in Visual Studio)
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Simple BMP Image Loader (for 24-bit uncompressed BMPs)
// Enhanced version with better error handling and color format detection
class BMPLoader {
public:
    static GLubyte* loadBMP(const char* filename, int& width, int& height) {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open BMP file " << filename << std::endl;
            return NULL;
        }

        // Read BMP header
        unsigned char header[54];
        file.read(reinterpret_cast<char*>(header), 54);

        if (header[0] != 'B' || header[1] != 'M') {
            std::cerr << "Error: Not a valid BMP file." << std::endl;
            file.close();
            return NULL;
        }

        // Extract image information from header (little-endian format)
        width = header[18] | (header[19] << 8) | (header[20] << 16) | (header[21] << 24);
        height = header[22] | (header[23] << 8) | (header[24] << 16) | (header[25] << 24);
        int dataPos = header[10] | (header[11] << 8) | (header[12] << 16) | (header[13] << 24);
        int imageSize = header[34] | (header[35] << 8) | (header[36] << 16) | (header[37] << 24);
        int bitsPerPixel = header[28] | (header[29] << 8);

        // Handle negative height (top-down bitmap)
        bool topDown = false;
        if (height < 0) {
            height = -height;
            topDown = true;
        }

        // Make sure it's a 24-bit BMP
        if (bitsPerPixel != 24) {
            std::cerr << "Error: Only 24-bit BMP files are supported. This file has " 
                      << bitsPerPixel << " bits per pixel." << std::endl;
            file.close();
            return NULL;
        }

        // Calculate row size with padding (BMP rows are padded to 4-byte boundaries)
        int rowSize = ((width * 3 + 3) / 4) * 4;
        int actualImageSize = rowSize * height;

        // Some BMP files have wrong imageSize, use calculated size
        if (imageSize == 0 || imageSize != actualImageSize) {
            imageSize = actualImageSize;
        }
        if (dataPos == 0) dataPos = 54;

        // Allocate memory for the raw image data
        GLubyte* rawData = new GLubyte[imageSize];
        
        // Read the image data
        file.seekg(dataPos);
        file.read(reinterpret_cast<char*>(rawData), imageSize);
        file.close();

        // Check if we read the expected amount of data
        if (file.gcount() != imageSize) {
            std::cerr << "Warning: Read " << file.gcount() << " bytes, expected " << imageSize << std::endl;
        }

        // Allocate memory for the processed image (without padding)
        GLubyte* data = new GLubyte[width * height * 3];
        
        // Process the image data, removing padding
        // Most BMPs are stored as BGR, but we'll handle both cases
        for (int y = 0; y < height; y++) {
            int srcY = topDown ? y : (height - 1 - y); // Handle top-down vs bottom-up
            
            for (int x = 0; x < width; x++) {
                int srcIndex = srcY * rowSize + x * 3;
                int dstIndex = y * width * 3 + x * 3;
                
                // Check bounds to prevent buffer overflow
                if (srcIndex + 2 < imageSize && dstIndex + 2 < width * height * 3) {
                    // Try BGR to RGB conversion first (most common)
                    data[dstIndex] = rawData[srcIndex + 2];     // Red
                    data[dstIndex + 1] = rawData[srcIndex + 1]; // Green
                    data[dstIndex + 2] = rawData[srcIndex];     // Blue
                }
            }
        }

        // Clean up raw data
        delete[] rawData;

        // Simple test to detect if colors are swapped
        // If we have a lot of blue values in the red channel, colors might be swapped
        int redSum = 0, blueSum = 0;
        int sampleSize = (width * height < 1000) ? width * height : 1000;
        
        for (int i = 0; i < sampleSize; i++) {
            redSum += data[i * 3];
            blueSum += data[i * 3 + 2];
        }
        
        // If blue channel has significantly more data than red, try swapping
        if (blueSum > redSum * 2) {
            std::cout << "Detected possible color swap, attempting correction..." << std::endl;
            for (int i = 0; i < width * height; i++) {
                GLubyte temp = data[i * 3];
                data[i * 3] = data[i * 3 + 2];
                data[i * 3 + 2] = temp;
            }
        }

        std::cout << "Loaded BMP: " << filename << " (Width: " << width << ", Height: " << height 
                  << ", Bits: " << bitsPerPixel << ", TopDown: " << (topDown ? "Yes" : "No") << ")" << std::endl;
        return data;
    }
};

// Texture Manager Class for handling OpenGL textures
class TextureManager {
public:
    // Load a texture from BMP file and return texture ID
    static GLuint loadTexture(const char* filename) {
        int width, height;
        GLubyte* imageData = BMPLoader::loadBMP(filename, width, height);
        
        if (!imageData) {
            std::cerr << "Failed to load texture: " << filename << std::endl;
            return 0;
        }
        
        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        
        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        // Upload the texture data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
        
        // Clean up
        delete[] imageData;
        
        std::cout << "Texture loaded successfully: " << filename << " (ID: " << textureID << ")" << std::endl;
        return textureID;
    }
    
    // Delete a texture
    static void deleteTexture(GLuint& textureID) {
        if (textureID != 0) {
            glDeleteTextures(1, &textureID);
            textureID = 0;
        }
    }
    
    // Bind a texture for rendering
    static void bindTexture(GLuint textureID) {
        if (textureID != 0) {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, textureID);
        } else {
            glDisable(GL_TEXTURE_2D);
        }
    }
    
    // Unbind all textures
    static void unbindTexture() {
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};

#endif // TEXTURE_H
