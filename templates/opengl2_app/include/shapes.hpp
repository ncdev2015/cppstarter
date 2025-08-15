#ifndef SHAPES_HPP
#define SHAPES_HPP

#include <GLFW/glfw3.h>
#include <cmath>

// Draw a triangle centered at (x, y)
inline void drawTriangle(float x, float y, float size) {
    float half = size / 2.0f;
    glBegin(GL_TRIANGLES);
        glVertex2f(x, y + half);
        glVertex2f(x - half, y - half);
        glVertex2f(x + half, y - half);
    glEnd();
}

// Draw a square centered at (x, y)
inline void drawSquare(float x, float y, float size) {
    float half = size / 2.0f;
    glBegin(GL_QUADS);
        glVertex2f(x - half, y - half);
        glVertex2f(x + half, y - half);
        glVertex2f(x + half, y + half);
        glVertex2f(x - half, y + half);
    glEnd();
}

// Draw a rectangle centered at (x, y)
inline void drawRectangle(float x, float y, float width, float height) {
    float hw = width / 2.0f;
    float hh = height / 2.0f;
    glBegin(GL_QUADS);
        glVertex2f(x - hw, y - hh);
        glVertex2f(x + hw, y - hh);
        glVertex2f(x + hw, y + hh);
        glVertex2f(x - hw, y + hh);
    glEnd();
}

// Draw a circle centered at (x, y)
inline void drawCircle(float x, float y, float radius, int segments = 32) {
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y); // center
        for (int i = 0; i <= segments; ++i) {
            float angle = i * 2.0f * M_PI / segments;
            float dx = cosf(angle) * radius;
            float dy = sinf(angle) * radius;
            glVertex2f(x + dx, y + dy);
        }
    glEnd();
}

#endif // SHAPES_HPP
