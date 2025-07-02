#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <vector>
#include <random>

// Constants for conversion between Box2D and SFML
const float SCALE = 30.0f; // Box2D uses meters, SFML uses pixels
const float WINDOW_WIDTH = 800.0f;
const float WINDOW_HEIGHT = 600.0f;

class Ball {
public:
    sf::CircleShape shape;
    b2Body* body;
    
    Ball(b2World& world, float x, float y, float radius, sf::Color color) {
        // Create SFML visual shape
        shape.setRadius(radius);
        shape.setFillColor(color);
        shape.setOrigin(radius, radius);
        
        // Create Box2D physics body
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(x / SCALE, y / SCALE);
        body = world.CreateBody(&bodyDef);
        
        // Create physics shape (circle)
        b2CircleShape circleShape;
        circleShape.m_radius = radius / SCALE;
        
        // Create fixture (physical properties)
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &circleShape;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;
        fixtureDef.restitution = 0.8f; // Bounciness
        
        body->CreateFixture(&fixtureDef);
    }
    
    void update() {
        // Synchronize position between Box2D and SFML
        b2Vec2 position = body->GetPosition();
        shape.setPosition(position.x * SCALE, position.y * SCALE);
        
        // Synchronize rotation
        float angle = body->GetAngle();
        shape.setRotation(angle * 180.0f / b2_pi);
    }
};

class Ground {
public:
    sf::RectangleShape shape;
    b2Body* body;
    
    Ground(b2World& world, float x, float y, float width, float height) {
        // Create visual shape
        shape.setSize(sf::Vector2f(width, height));
        shape.setFillColor(sf::Color::Green);
        shape.setPosition(x - width/2, y - height/2);
        
        // Create physics body (static)
        b2BodyDef bodyDef;
        bodyDef.type = b2_staticBody;
        bodyDef.position.Set(x / SCALE, y / SCALE);
        body = world.CreateBody(&bodyDef);
        
        // Create physics shape (rectangle)
        b2PolygonShape boxShape;
        boxShape.SetAsBox((width/2) / SCALE, (height/2) / SCALE);
        
        body->CreateFixture(&boxShape, 0.0f);
    }
};

int main() {
    // Create SFML window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Box2D + SFML - Bouncing Balls");
    window.setFramerateLimit(60);
    
    // Create Box2D world with gravity
    b2Vec2 gravity(0.0f, 9.8f); // Downward gravity
    b2World world(gravity);
    
    // Create ground
    Ground ground(world, WINDOW_WIDTH/2, WINDOW_HEIGHT - 20, WINDOW_WIDTH, 40);
    
    // Create side walls
    Ground leftWall(world, 10, WINDOW_HEIGHT/2, 20, WINDOW_HEIGHT);
    Ground rightWall(world, WINDOW_WIDTH - 10, WINDOW_HEIGHT/2, 20, WINDOW_HEIGHT);
    
    // Vector to store balls
    std::vector<Ball> balls;
    
    // Random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> xDist(50, WINDOW_WIDTH - 50);
    std::uniform_real_distribution<float> radiusDist(10, 30);
    std::uniform_int_distribution<int> colorDist(0, 255);
    
    sf::Clock clock;
    sf::Clock ballSpawnClock;
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            // Create new ball on mouse click
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    float x = event.mouseButton.x;
                    float y = event.mouseButton.y;
                    float radius = radiusDist(gen);
                    sf::Color color(colorDist(gen), colorDist(gen), colorDist(gen));
                    
                    balls.emplace_back(world, x, y, radius, color);
                }
            }
        }
        
        // Automatically create balls every 2 seconds
        if (ballSpawnClock.getElapsedTime().asSeconds() > 2.0f) {
            float x = xDist(gen);
            float y = 50;
            float radius = radiusDist(gen);
            sf::Color color(colorDist(gen), colorDist(gen), colorDist(gen));
            
            balls.emplace_back(world, x, y, radius, color);
            ballSpawnClock.restart();
        }
        
        // Update physics
        float timeStep = 1.0f / 60.0f;
        int32 velocityIterations = 6;
        int32 positionIterations = 2;
        world.Step(timeStep, velocityIterations, positionIterations);
        
        // Update all balls
        for (auto& ball : balls) {
            ball.update();
        }
        
        // Clear window
        window.clear(sf::Color::Black);
        
        // Draw ground and walls
        window.draw(ground.shape);
        window.draw(leftWall.shape);
        window.draw(rightWall.shape);
        
        // Draw all balls
        for (const auto& ball : balls) {
            window.draw(ball.shape);
        }
        
        // Display everything
        window.display();
    }
    
    return 0;
}