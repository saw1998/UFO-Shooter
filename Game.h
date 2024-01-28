//
// Created by Sachin Saw on 23/01/24.
//

#ifndef GAMEENGINE_GAME_H
#define GAMEENGINE_GAME_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include "Entity.h"
#include "EntityManager.h"
#include "Config.h"

class Game {
    sf::RenderWindow m_window;         //window we will draw to
    EntityManager m_entityManager;     //vector of entities to maintain
    sf::Font m_font;                   //font we will use to draw
    sf::Text m_text;                   //score text to be drawn on the screen

    WindowConfig m_windowConfig;
    FontConfig m_fontConfig;
    PlayerConfig m_playerConfig;
    EnemyConfig m_enemyConfig;
    BulletConfig m_bulletConfig;

    int m_score = 0;
    int m_currentFrame = 0;
    bool m_running = true;
    bool m_paused = false;

    time_t m_lastEnemySpawnedAt = time(nullptr);

    std::shared_ptr<Entity> m_player;

    void init(const std::string & config);  //initialize the GameState with config
    void setPaused(bool paused);

    void sMovement();                       //System : Entity position / movement update
    void sUserInput();                      //System : User Input
    void sLifespan();
    void sRender();
    void sEnemySpawner();
    void sCollision();

    void spawnPlayer();
    void spawnEnemy();
    void spawnSmallEnemies(const std::shared_ptr<Entity>& entity);
    void spawnBullet(std::shared_ptr<Entity> entity, const Vec2 & mousePos);

public:
    Game(const std::string & config);
    void run();
};


#endif //GAMEENGINE_GAME_H
