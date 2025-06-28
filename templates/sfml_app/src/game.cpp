#include "game.h"

Game::Game() : window(sf::VideoMode(800, 600), "SFML Game") {
    shape.setRadius(50.f);
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(375.f, 275.f);
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

void Game::update() {
    // Game logic here
}

void Game::render() {
    window.clear();
    window.draw(shape);
    window.display();
}
