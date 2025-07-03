#include <SFML/Graphics.hpp> // For SFML graphics
#include <box2d/box2d.h>     // For Box2D physics

// Define a scale ratio to convert Box2D units (meters) to SFML pixels
const float SCALE = 30.0f; // 30 pixels per meter

int main()
{
    // --- SFML Setup ---
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML + Box2D Demo");
    window.setFramerateLimit(60); // Limit FPS to 60 for smoother simulation
    
    // --- Box2D Setup ---
    // Define gravity (positive Y vector points downwards)
    b2Vec2 gravity(0.0f, 9.8f);
    // Create the Box2D world
    b2World world(gravity);
    
    // --- Create the ground (static Body) ---
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(400.0f / SCALE, 550.0f / SCALE); // Position in meters
    
    b2Body* groundBody = world.CreateBody(&groundBodyDef);
    
    b2PolygonShape groundBox;
    // Ground dimensions: 380 pixels wide, 10 pixels high
    groundBox.SetAsBox(380.0f / SCALE, 10.0f / SCALE); // Half-width, half-height in meters
    
    groundBody->CreateFixture(&groundBox, 0.0f); // 0.0f density for a static body
    
    // --- Create a dynamic box (dynamic Body) ---
    b2BodyDef dynamicBoxDef;
    dynamicBoxDef.type = b2_dynamicBody; // It's a dynamic body, affected by gravity
    dynamicBoxDef.position.Set(400.0f / SCALE, 100.0f / SCALE); // Initial position in meters
    
    b2Body* dynamicBoxBody = world.CreateBody(&dynamicBoxDef);
    
    b2PolygonShape dynamicBox;
    // Box dimensions: 40 pixels wide, 40 pixels high
    dynamicBox.SetAsBox(20.0f / SCALE, 20.0f / SCALE); // Half-width, half-height in meters
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;    // Density to calculate mass
    fixtureDef.friction = 0.3f;   // Friction
    fixtureDef.restitution = 0.5f; // Bounciness
    
    dynamicBoxBody->CreateFixture(&fixtureDef);
    
    // --- Main game loop ---
    float timeStep = 1.0f / 60.0f; // 60 simulation steps per second
    int velocityIterations = 8;    // How many velocity iterations
    int positionIterations = 3;    // How many position iterations
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        // --- Update the Box2D world ---
        world.Step(timeStep, velocityIterations, positionIterations);
        
        // --- Render (draw) with SFML ---
        window.clear(sf::Color(100, 100, 250)); // Clear the window with light blue
        
        // Draw the ground (Box2D has center position, SFML uses top-left corner)
        sf::RectangleShape groundShape(sf::Vector2f(760.0f, 20.0f)); // Size in pixels
        // Convert Box2D center position to SFML top-left corner
        groundShape.setPosition(groundBody->GetPosition().x * SCALE - 380.0f,
                                groundBody->GetPosition().y * SCALE - 10.0f);
        groundShape.setFillColor(sf::Color(50, 150, 50)); // Dark green
        window.draw(groundShape);
        
        // Draw the dynamic box
        sf::RectangleShape dynamicBoxShape(sf::Vector2f(40.0f, 40.0f)); // Size in pixels
        // Convert Box2D center position to SFML top-left corner
        dynamicBoxShape.setPosition(dynamicBoxBody->GetPosition().x * SCALE - 20.0f,
                                    dynamicBoxBody->GetPosition().y * SCALE - 20.0f);
        // Set rotation (Box2D uses radians, SFML uses degrees)
        dynamicBoxShape.setRotation(dynamicBoxBody->GetAngle() * 180 / b2_pi);
        dynamicBoxShape.setFillColor(sf::Color::Red);
        window.draw(dynamicBoxShape);
        
        window.display();
    }
    
    return 0;
}
