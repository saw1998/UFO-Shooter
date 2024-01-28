//
// Created by Sachin Saw on 26/01/24.
//

#ifndef GAMEENGINE_CONFIG_H
#define GAMEENGINE_CONFIG_H

#include <iostream>

struct WindowConfig {
    int width, height, frameLimit, isFullScreen;
};

struct FontConfig {
    std::string fontFile;
    int fontSize;
    int colorR, colorG, colorB;
};

struct PlayerConfig {
    int shapeRadius, collisionRadius, speed;
    int fillColorR, fillColorG, fillColorB;
    int outlineColorR, outlineColorG, outlineColorB;
    int outlineThickness;
    int vertexCount;
};

struct EnemyConfig {
    int shapeRadius, collisionRadius;
    int minSpeed, maxSpeed;
    int outlineColorR, outlineColorG, outlineColorB;
    int outlineThickness;
    int minVertex, maxVertex;
    int lifeSpan;
    int spawnInterval;
};

struct BulletConfig {
    int shapeRadius, collisionRadius;
    int speed;
    int fillColorR, fillColorG, fillColorB;
    int outlineColorR, outlineColorG, outlineColorB;
    int outlineThickness;
    int shapeVertex;
    int lifeSpan;
};

#endif //GAMEENGINE_CONFIG_H
