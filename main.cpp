#include "AtomFactory.hpp"
#include "Loader.hpp"
#include <mutex>

enum class AppState { MENU, SIM };

void drawMenu(sf::RenderWindow& window, const std::vector<std::unique_ptr<AtomData>>& atoms, int hoveredAtoms, sf::Font& font, bool hasFont) {
    window.clear(sf::Color::Black);

    if (!hasFont) {
        window.display();
        return;
    }

    sf::Text title;
    title.setFont(font);
    title.setCharacterSize(36);
    title.setFillColor(sf::Color::White);
    title.setPosition(250, 40);
    title.setString("Quantum Atom Visualizer");
    window.draw(title);

    sf::Text subtitle;
    subtitle.setFont(font);
    subtitle.setCharacterSize(16);
    subtitle.setFillColor(sf::Color(120, 120, 120));
    subtitle.setPosition(280, 90);
    subtitle.setString("Select an atom to visualize");
    window.draw(subtitle);

    for (int i = 0; i < static_cast<int>(atoms.size()); ++i) {
        float y = 160.0f + i * 80.0f;
        bool hovered = (i == hoveredAtoms);

        sf::RectangleShape card(sf::Vector2f(560.0f, 60.0f));
        card.setPosition(120, y);
        card.setFillColor(hovered ? sf::Color(40, 40, 70) : sf::Color(20, 20, 40));
        card.setOutlineThickness(1);
        card.setOutlineColor(hovered ? sf::Color(100, 150, 255) : sf::Color(50, 50, 80));
        window.draw(card);

        sf::Text symbol;
        symbol.setFont(font);
        symbol.setCharacterSize(28);
        symbol.setFillColor(hovered ? sf::Color(100, 180, 255) : sf::Color(200, 200, 200));
        symbol.setPosition(140, y + 8);
        symbol.setString(atoms[i]->symbol);
        window.draw(symbol);

        sf::Text name;
        name.setFont(font);
        name.setCharacterSize(20);
        name.setFillColor(sf::Color(180, 180, 180));
        name.setPosition(220, y + 8);
        name.setString(atoms[i]->fullName + " (Z=" + std::to_string(atoms[i]->Z) + ")");
        window.draw(name);

        sf::Text status;
        status.setFont(font);
        status.setCharacterSize(14);
        status.setPosition(220, y + 36);

        if (atoms[i]->ready) {
            status.setFillColor(sf::Color(100, 255, 100));
            status.setString("Ready");
        } else if (atoms[i]->startedLoading) {
            int prog = atoms[i]->loadProgress, total = static_cast<int>(atoms[i]->orbitals.size());
            status.setFillColor(sf::Color(255, 200, 50));
            status.setString("Loading... " + std::to_string(prog) + "/" +  std::to_string(total));

            sf::RectangleShape miniBarBg(sf::Vector2f(200.0f, 6.0f));
            miniBarBg.setPosition(340, y + 40);
            miniBarBg.setFillColor(sf::Color(50, 50, 50));
            window.draw(miniBarBg);

            sf::RectangleShape miniBarFill(sf::Vector2f(200.0f * prog / total, 6.0f));
            miniBarFill.setPosition(340, y + 40);
            miniBarFill.setFillColor(sf::Color(255, 200, 50));
            window.draw(miniBarFill);
        } else {
            status.setFillColor(sf::Color(120, 120, 120));
            status.setString("Click to load");
        }

        window.draw(status);
    }

    sf::Text controls;
    controls.setFont(font);
    controls.setCharacterSize(14);
    controls.setFillColor(sf::Color(80, 80, 80));
    controls.setPosition(20, 760);
    controls.setString("Click to select atom | ESC: back to menu");
    window.draw(controls);

    window.display();
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "Quantum Atom Visualizer");
    window.setFramerateLimit(60);

    sf::Font font;
    bool hasFont = font.loadFromFile("C:/Windows/Fonts/arial.ttf");

    std::vector<std::unique_ptr<AtomData>> atoms;
    atoms.push_back(makeHydrogen());
    atoms.push_back(makeHelium());
    atoms.push_back(makeLithium());
    atoms.push_back(makeCarbon());
    atoms.push_back(makeNitrogen());
    atoms.push_back(makeOxygen());
    atoms.push_back(makeNeon());

    AppState state = AppState::MENU;
    int hoveredAtom = -1;
    AtomData* currentAtom = nullptr;

    int currentOrbital = 0;
    std::vector<std::pair<std::array<double, 3>, sf::Color>> points3D;
    size_t visiblePoints = 0;
    size_t pointsPerFrame = 100;
    float angleX = 0.0f, angleY = 0.0f, zoom = 60.0f;
    bool dragging = false;
    bool autoRotate = false;
    float currRotSpeed = 0.1f;
    sf::Vector2i lastMouse;

    sf::Text label, qnLabel, controlsLabel;

    if (hasFont) {
        label.setFont(font);
        label.setCharacterSize(24);
        label.setFillColor(sf::Color::White);
        label.setPosition(20, 20);

        qnLabel.setFont(font);
        qnLabel.setCharacterSize(18);
        qnLabel.setFillColor(sf::Color(180, 180, 255));
        qnLabel.setPosition(20, 52);

        controlsLabel.setFont(font);
        controlsLabel.setCharacterSize(14);
        controlsLabel.setFillColor(sf::Color(120, 120, 120));
        controlsLabel.setPosition(20, 760);
        controlsLabel.setString("1-9: orbital | A: ground all | B: all | R: auto rotate | S: screenshot | scroll: zoom in/out | drag: rotate | ESC: back to menu");
    }

    auto selectOrbital = [&](int idx) {
        if (!currentAtom || !currentAtom->ready) {
            return;
        }

        int total = static_cast<int>(currentAtom->orbitals.size());

        if (idx == total) {
            points3D = currentAtom->groundAll;
            zoom = 15.0f;

            if (hasFont) {
                label.setString("All ground state orbitals");
                qnLabel.setString("");
            }
        } else if (idx == total + 1) {
            points3D = currentAtom->all;
            zoom = 10.0f;

            if (hasFont) {
                label.setString("All state orbitals");
                qnLabel.setString("");
            }
        } else if (idx < total) {
            points3D = currentAtom->points[idx];
            zoom = 60.0f / (currentAtom->orbitals[idx].range / 5.0f);
            std::string groundTag = currentAtom->orbitals[idx].isGroundState ? " [ground]" : " [excited]";

            if (hasFont) {
                label.setString("Orbital: " + currentAtom->orbitals[idx].name + groundTag);
                qnLabel.setString(currentAtom->orbitals[idx].quantumNumbers);
            }
        }

        currentOrbital = idx;
        visiblePoints = 0;
        pointsPerFrame = std::max(static_cast<size_t>(100), points3D.size() / 1000);
        currRotSpeed = 0.1f;
    };

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (state == AppState::MENU) {
                if (event.type == sf::Event::MouseMoved) {
                    sf::Vector2i mouse = sf::Mouse::getPosition(window);
                    hoveredAtom = -1;

                    for (int i = 0; i < static_cast<int>(atoms.size()); ++i) {
                        float y = 160.0f + i * 80.0f;

                        if (mouse.x >= 120 && mouse.x <= 680 && mouse.y >= y && mouse.y <= y + 60) {
                            hoveredAtom = i;
                        }
                    }
                }

                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && hoveredAtom != -1) {
                    currentAtom = atoms[hoveredAtom].get();
                    loadAtomAsync(*currentAtom);

                    if (!currentAtom->ready) {
                        ///stay in menu until ready
                    } else {
                        state = AppState::SIM;
                        angleX = angleY = 0.0f;
                        selectOrbital(0);
                    }
                }
            } else {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                    state = AppState::MENU;
                }

                if (event.type == sf::Event::MouseWheelScrolled) {
                    zoom += event.mouseWheelScroll.delta * 5.0f;
                    zoom = std::max(10.0f, std::min(200.0f, zoom));
                }

                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    dragging = true;
                    lastMouse = sf::Mouse::getPosition(window);
                }

                if (event.type == sf::Event::MouseButtonReleased) {
                    dragging = false;
                }

                if (event.type == sf::Event::MouseMoved && dragging) {
                    sf::Vector2i curr = sf::Mouse::getPosition(window);
                    angleX += (curr.y - lastMouse.y) * 0.3f;
                    angleY += (curr.x - lastMouse.x) * 0.3f;
                    lastMouse = curr;
                }

                if (event.type == sf::Event::KeyPressed && currentAtom && currentAtom->ready) {
                    int total = static_cast<int>(currentAtom->orbitals.size());

                    if (event.key.code == sf::Keyboard::R) {
                        autoRotate = !autoRotate;
                    }

                    if (event.key.code == sf::Keyboard::A) {
                        selectOrbital(total);
                    }

                    if (event.key.code == sf::Keyboard::B) {
                        selectOrbital(total + 1);
                    }

                    if (event.key.code == sf::Keyboard::S) {
                        sf::Texture tex;
                        tex.create(window.getSize().x, window.getSize().y);
                        tex.update(window);
                        tex.copyToImage().saveToFile("screenshot.png");
                    }

                    int numKeys[] = {
                        sf::Keyboard::Num1, sf::Keyboard::Num2, sf::Keyboard::Num3,
                        sf::Keyboard::Num4, sf::Keyboard::Num5, sf::Keyboard::Num6,
                        sf::Keyboard::Num7, sf::Keyboard::Num8, sf::Keyboard::Num9
                    };

                    for (int i = 0; i < std::min(total, 9); ++i) {
                        if (event.key.code == numKeys[i]) {
                            selectOrbital(i);
                        }
                    }
                }
            }
        }

        if (state == AppState::MENU) {
            if (currentAtom && currentAtom->ready) {
                state == AppState::SIM;
                angleX = angleY = 0.0f;
                selectOrbital(0);
            }

            drawMenu(window, atoms, hoveredAtom, font, hasFont);
        } else {
            if (autoRotate && !dragging) {
                angleY += currRotSpeed;
            }

            if (visiblePoints < points3D.size()) {
                visiblePoints = std::min(points3D.size(), visiblePoints + pointsPerFrame);
            }

            std::vector<sf::Vertex> vertices;
            vertices.reserve(visiblePoints);
            float radX = angleX * PI / 180.0f, radY = angleY * PI / 180.0;

            for (size_t i = 0; i < visiblePoints; ++i) {
                auto& p = points3D[i];
                double x = p.first[0], y = p.first[1], z = p.first[2];
                sf::Color col = p.second;

                double x1 = x * cos(radY) + z * sin(radY);
                double z1 = -x * sin(radY) + z * cos(radY);
                double y2 = y * cos(radX) - z1 * sin(radX);
                double z2 = y * sin(radX) + z1 * cos(radX);

                float px = 400.0f + x1 * zoom, py = 400.0f + y2 * zoom;

                sf::Uint8 brightness = static_cast<sf::Uint8>(std::min(255.0, std::max(30.0, 180.0 + z2 * 15.0)));
                vertices.push_back(sf::Vertex(
                    sf::Vector2f(px, py),
                    sf::Color(col.r * brightness / 255, col.g * brightness / 255, col.b * brightness / 255, 160)
                ));
            }

            window.clear(sf::Color::Black);
            window.draw(vertices.data(), vertices.size(), sf::Points);

            for (int g = 1; g <= 4; ++g) {
                sf::CircleShape glow(6.0f + g * 4.0f);
                glow.setOrigin(6.0f + g * 4.0f, 6.0f + g * 4.0f);
                glow.setPosition(400.0f, 400.0f);
                glow.setFillColor(sf::Color(255, 200, 50, 60 / g));
                window.draw(glow);
            }

            sf::CircleShape nucleus(6.0f);
            nucleus.setOrigin(6.0f, 6.0f);
            nucleus.setPosition(400.0f, 400.0f);
            nucleus.setFillColor(sf::Color(255, 220, 100));
            window.draw(nucleus);

            if (hasFont) {
                window.draw(label);
                window.draw(qnLabel);
                window.draw(controlsLabel);
            }

            window.display();
        }
    }
}