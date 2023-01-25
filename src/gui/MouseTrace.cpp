#include "MouseTrace.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>

#include "../utils/geometry.cpp"
#include "../utils/Utils.h"

MouseTrace::MouseTrace() { triangleStrip = sf::VertexArray(sf::PrimitiveType::TriangleStrip); }

MouseTrace::~MouseTrace() = default;

void MouseTrace::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    float r = clicked ? radius * 1.3 : radius;
    sf::CircleShape circle(r);
    circle.setFillColor(settings.cursorColor);
    circle.setPosition(positions[0].x - r, positions[0].y - r);
    target.draw(circle);
    if (size > 4)
    target.draw(triangleStrip);
}

void MouseTrace::push(sf::Vector2i position) {
    positions.push_front(position);
    if (positions.size() > maxLength) {
        positions.pop_back();
    }
    constructPolygon();
}

void MouseTrace::constructPolygon() const {
    sf::Color color(settings.cursorTraceColor);
    float decrease = (radius - 4) / (maxLength + 1);
    float r = radius - 2;
    triangleStrip.clear();
    size = 0;
    sf::Vertex vertex;
    vertex.color = color;
    for (int i = 0; i + 1 < positions.size(); ++i) {
        r -= decrease;
        if (r <= 0) break;
        geom::Point from(positions[i].x, positions[i].y);
        geom::Point to(positions[i + 1].x, positions[i + 1].y);
        geom::Point dir = to - from;
        if (dir.len() < 1) {
            continue;
        }
        geom::Point rot = dir.norm().rotate({0, 0}, 90) * r;
        geom::Point p1 = from + rot;
        geom::Point p2 = to + rot;
        geom::Point p3 = from - rot;
        geom::Point p4 = to - rot;

        vertex.position = {p1.x, p1.y};
        triangleStrip.append(vertex);
        vertex.position = {p3.x, p3.y};
        triangleStrip.append(vertex);
        color.a -= 5;
        vertex.color = color;

        vertex.position = {p2.x, p2.y};
        triangleStrip.append(vertex);
        vertex.position = {p4.x, p4.y};
        triangleStrip.append(vertex);
        color.a -= 5;
        vertex.color = color;

        size += 4;
    }
}

void MouseTrace::setClicked(bool b) { clicked = b; }

void MouseTrace::clear() {
    positions.clear();
    constructPolygon();
}
