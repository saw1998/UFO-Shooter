//
// Created by Sachin Saw on 23/01/24.
//

#include <fstream>
#include <SFML/Window/Event.hpp>
#include "Game.h"

Game::Game(const std::string &config) {
    init(config);

    //set up default window parameters
    m_window.create(sf::VideoMode(m_windowConfig.width,m_windowConfig.height), "UFO Shooter");
    m_window.setFramerateLimit(m_windowConfig.frameLimit);
    if(!m_font.loadFromFile(m_fontConfig.fontFile)){
        std::cout<<"Unable to load font file"<<std::endl;
    }
    m_text.setFont(m_font);
    m_text.setCharacterSize(20);

    srand(time(nullptr));
    spawnPlayer();
}

void Game::init(const std::string & path){

    std::ifstream fin(path);

    std::string input_type;
    while(fin >> input_type){
        if(input_type == "Window") {
            fin >> m_windowConfig.width
                >> m_windowConfig.height
                >> m_windowConfig.frameLimit
                >> m_windowConfig.isFullScreen;
        } else if(input_type == "Font") {
            fin >> m_fontConfig.fontFile
                >> m_fontConfig.fontSize
                >> m_fontConfig.colorR
                >> m_fontConfig.colorG
                >> m_fontConfig.colorB;
        } else if(input_type == "Player") {
            fin >> m_playerConfig.shapeRadius
                >> m_playerConfig.collisionRadius
                >> m_playerConfig.speed
                >> m_playerConfig.fillColorR
                >> m_playerConfig.fillColorG
                >> m_playerConfig.fillColorB
                >> m_playerConfig.outlineColorR
                >> m_playerConfig.outlineColorG
                >> m_playerConfig.outlineColorB
                >> m_playerConfig.outlineThickness
                >> m_playerConfig.vertexCount;
        } else if(input_type == "Enemy") {
            fin >> m_enemyConfig.shapeRadius
                >> m_enemyConfig.collisionRadius
                >> m_enemyConfig.minSpeed
                >> m_enemyConfig.maxSpeed
                >> m_enemyConfig.outlineColorR
                >> m_enemyConfig.outlineColorG
                >> m_enemyConfig.outlineColorB
                >> m_enemyConfig.outlineThickness
                >> m_enemyConfig.minVertex
                >> m_enemyConfig.maxVertex
                >> m_enemyConfig.lifeSpan
                >> m_enemyConfig.spawnInterval;
        } else if(input_type == "Bullet") {
            fin >> m_bulletConfig.shapeRadius
                >> m_bulletConfig.collisionRadius
                >> m_bulletConfig.speed
                >> m_bulletConfig.fillColorR
                >> m_bulletConfig.fillColorG
                >> m_bulletConfig.fillColorB
                >> m_bulletConfig.outlineColorR
                >> m_bulletConfig.outlineColorG
                >> m_bulletConfig.outlineColorB
                >> m_bulletConfig.outlineThickness
                >> m_bulletConfig.shapeVertex
                >> m_bulletConfig.lifeSpan;
        }
    }
}

void Game::run(){
    while(m_running) {
        m_entityManager.update();
        sUserInput();
        sMovement();
        sCollision();
        sEnemySpawner();
        sRender();
        m_currentFrame++;
    }
}

void Game::setPaused(bool paused) {
    m_paused = paused;
}

void Game::spawnPlayer() {
    auto entity = m_entityManager.addEntity("player");

    entity->cTransform = std::make_shared<CTransform>(Vec2((float)m_windowConfig.width/2, (float)m_windowConfig.height/2),
                                                      Vec2(0.0, 0.0),
                                                      0.0f);
    entity->cShape = std::make_shared<CShape>((float)m_playerConfig.shapeRadius,
                                              m_playerConfig.vertexCount,
                                              sf::Color(m_playerConfig.fillColorR,
                                                          m_playerConfig.fillColorG,
                                                          m_playerConfig.fillColorB),
                                              sf::Color(m_playerConfig.outlineColorR,
                                                        m_playerConfig.outlineColorG,
                                                        m_playerConfig.outlineColorB),
                                              (float)m_playerConfig.outlineThickness);
    entity->cInput = std::make_shared<CInput>();
    entity->cCollision = std::make_shared<CCollision>(m_playerConfig.collisionRadius);
    m_player = entity;
}

void Game::sRender() {
    m_window.clear();

    for(auto entity : m_entityManager.getEntities()){
        entity->cShape->circle.setPosition(entity->cTransform->pos.x, entity->cTransform->pos.y);
        entity->cShape->circle.setRotation(entity->cTransform->angle);
        m_window.draw(entity->cShape->circle);
    }

    m_text.setString("Score : " + std::to_string(m_score));
    m_window.draw(m_text);
    m_window.display();
}

void Game::sEnemySpawner() {
    time_t current_time = time(nullptr);
    if(current_time - m_lastEnemySpawnedAt > m_enemyConfig.spawnInterval) {
        m_lastEnemySpawnedAt = current_time;
        spawnEnemy();
    }
}

void Game::sCollision(){
    //collision with wall
    for(auto entity : m_entityManager.getEntities("Enemy")){
        float radius = entity->cCollision->radius;

        if(entity->cTransform->pos.x <= radius){
           entity->cTransform->velocity.x = abs(entity->cTransform->velocity.x);
        } else if(entity->cTransform->pos.x >= m_windowConfig.width - radius){
            entity->cTransform->velocity.x = -1 * abs(entity->cTransform->velocity.x);
        } else if(entity->cTransform->pos.y <= radius){
            entity->cTransform->velocity.y = abs(entity->cTransform->velocity.y);
        } else if(entity->cTransform->pos.y >= m_windowConfig.height - radius) {
            entity->cTransform->velocity.y = -1 * abs(entity->cTransform->velocity.y);
        }
    }

    //bullet out of window
    for(auto bullet : m_entityManager.getEntities("Bullet")){
        int x = bullet->cTransform->pos.x;
        int y = bullet->cTransform->pos.y;
        if(x < 0 || y < 0 || x > m_windowConfig.width || y > m_windowConfig.height){
            bullet->destroy();
        }
    }

    //collision with player
    for(auto enemy : m_entityManager.getEntities("Enemy")) {
        if(enemy->distanceFrom(m_player) <= enemy->cCollision->radius + m_player->cCollision->radius){
            //destroy enemy
            spawnSmallEnemies(enemy);
            enemy->destroy();
            m_player->cTransform->pos = Vec2(m_windowConfig.width/2, m_windowConfig.height/2);
            m_score = 0;
        }
    }

    //collision with bullet
    for(auto enemy : m_entityManager.getEntities("Enemy")){
        for(auto bullet : m_entityManager.getEntities("Bullet")){
            if(enemy->distanceFrom(bullet) <= enemy->cCollision->radius + bullet->cCollision->radius){
                spawnSmallEnemies(enemy);
                enemy->destroy();
                bullet->destroy();
                m_score += 5;
            }
        }
    }
}

void Game::sUserInput(){
    sf::Event event;
    while(m_window.pollEvent(event)){
        if(event.type == sf::Event::Closed){
            m_running = false;
        }

        if(event.type == sf::Event::KeyPressed){
            switch(event.key.code){
                case sf::Keyboard::W :
                    m_player->cInput->up = true;
                    break;
                case sf::Keyboard::S :
                    m_player->cInput->down = true;
                    break;
                case sf::Keyboard::A :
                    m_player->cInput->left = true;
                    break;
                case sf::Keyboard::D :
                    m_player->cInput->right = true;
                    break;
            }
        }

        else if(event.type == sf::Event::KeyReleased){
            switch (event.key.code) {
                case sf::Keyboard::W :
                    m_player->cInput->up = false;
                    break;
                case sf::Keyboard::S :
                    m_player->cInput->down = false;
                    break;
                case sf::Keyboard::A :
                    m_player->cInput->left = false;
                    break;
                case sf::Keyboard::D :
                    m_player->cInput->right = false;
                    break;
            }
        }

        if(event.type == sf::Event::MouseButtonPressed){
            switch(event.mouseButton.button) {
                case sf::Mouse::Left :
                    auto mousePosition = sf::Mouse::getPosition(m_window);
                    spawnBullet(m_player, Vec2((float)mousePosition.x, (float)mousePosition.y));
                    break;
            }
        }
    }
}

void Game::sMovement() {
    int interval = 1;
    int fadeSpeed = 5;
    for(auto entity : m_entityManager.getEntities()){
        if(entity->tag() == "small_enemy") {
            auto fillColor = entity->cShape->circle.getFillColor();
            auto outlineColor = entity->cShape->circle.getOutlineColor();
            const int updated_alpha = fillColor.a - fadeSpeed * interval;
            if(updated_alpha < 0) entity->destroy();
            else{
                fillColor.a = updated_alpha;
                outlineColor.a = updated_alpha;
                entity->cShape->circle.setFillColor(fillColor);
                entity->cShape->circle.setOutlineColor(outlineColor);
            }
        }

        if(entity->tag() == "player"){
            auto radius = (float)m_playerConfig.collisionRadius;

            if(entity->cInput->up
                && entity->cTransform->pos.y > radius) entity->cTransform->pos.y -= m_playerConfig.speed * interval;
            else if(entity->cInput->down
                && entity->cTransform->pos.y < m_windowConfig.height - radius) entity->cTransform->pos.y += m_playerConfig.speed * interval;
            else if(entity->cInput->right
                && entity->cTransform->pos.x < m_windowConfig.width - radius) entity->cTransform->pos.x += m_playerConfig.speed * interval;
            else if(entity->cInput->left
                && entity->cTransform->pos.x > radius) entity->cTransform->pos.x -= m_playerConfig.speed * interval;
        } else {
            entity->cTransform->pos.x += entity->cTransform->velocity.x * interval;
            entity->cTransform->pos.y += entity->cTransform->velocity.y * interval;
        }

        entity->cTransform->angle += 5;
    }
}

void Game::spawnEnemy() {
    auto entity = m_entityManager.addEntity("Enemy");

    int posX = m_enemyConfig.shapeRadius + rand() % (m_windowConfig.width - m_enemyConfig.shapeRadius);
    int posY = m_enemyConfig.shapeRadius + rand() % (m_windowConfig.height - m_enemyConfig.shapeRadius);

    int speed = 0;
    if(m_enemyConfig.maxSpeed == m_enemyConfig.minSpeed){
        speed = m_enemyConfig.minSpeed;
    } else {
        speed = m_enemyConfig.minSpeed + rand() % (m_enemyConfig.maxSpeed - m_enemyConfig.minSpeed);
    }
    int theta = rand() % 360;
    int vertexCount = m_enemyConfig.minVertex + rand() % (m_enemyConfig.maxVertex - m_enemyConfig.minVertex);

    entity->cTransform = std::make_shared<CTransform>(Vec2((float)posX, (float)posY),
                                                      Vec2((float)speed* cos(theta), (float)speed* sin(theta)),
                                                      0.0f);
    entity->cShape = std::make_shared<CShape>((float)m_enemyConfig.shapeRadius,
                                              vertexCount,
                                              sf::Color(rand() % 255,
                                                        rand() % 255,
                                                        rand() % 255),
                                              sf::Color(m_enemyConfig.outlineColorR,
                                                        m_enemyConfig.outlineColorG,
                                                        m_enemyConfig.outlineColorB),
                                              (float)m_enemyConfig.outlineThickness);
    entity->cCollision = std::make_shared<CCollision>(m_enemyConfig.collisionRadius);
    entity->cLifeSpan = std::make_shared<CLifespan>(m_enemyConfig.lifeSpan);
}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2 &mousePos) {
    Vec2 shootDirection = Vec2(mousePos.x - entity->cTransform->pos.x, mousePos.y - entity->cTransform->pos.y);
    float theta = atan2(shootDirection.y, shootDirection.x);

    auto bullet = m_entityManager.addEntity("Bullet");
    bullet->cTransform = std::make_shared<CTransform>( entity->cTransform->pos,
                                                       Vec2(m_bulletConfig.speed * cos(theta), m_bulletConfig.speed * sin(theta)),
                                                       0.0f);
    bullet->cShape = std::make_shared<CShape>(m_bulletConfig.shapeRadius,
                                              m_bulletConfig.shapeVertex,
                                              sf::Color(m_bulletConfig.fillColorR, m_bulletConfig.fillColorG, m_bulletConfig.fillColorB),
                                              sf::Color(m_bulletConfig.outlineColorR, m_bulletConfig.outlineColorG, m_bulletConfig.outlineColorB),
                                              (float) m_bulletConfig.outlineThickness);
    bullet->cCollision = std::make_shared<CCollision>(m_bulletConfig.collisionRadius);
}

void Game::spawnSmallEnemies(const std::shared_ptr<Entity>& entity) {
    int pointCount = entity->cShape->circle.getPointCount();
    Vec2 position = entity->cTransform->pos;

    for(int i=0 ; i<pointCount ; i++){
        auto smallEnemy = m_entityManager.addEntity("small_enemy");
        float theta = 6.2832f / (float)pointCount * i;
        smallEnemy->cTransform = std::make_shared<CTransform>(position,
                                                              Vec2(m_enemyConfig.maxSpeed* cos(theta),
                                                                   m_enemyConfig.maxSpeed*sin(theta)),
                                                              0.0f);

        smallEnemy->cShape = std::make_shared<CShape>(entity->cShape->circle.getRadius()/2,
                                                      pointCount,
                                                      entity->cShape->circle.getFillColor(),
                                                      entity->cShape->circle.getOutlineColor(),
                                                      entity->cShape->circle.getOutlineThickness());
    }
}