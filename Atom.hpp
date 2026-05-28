#pragma once
#include <string>
#include <vector>
#include <array>
#include <functional>
#include <atomic>
#include <SFML/Graphics.hpp>

using OrbitalFn = std::function<double(double, double, double)>;

struct OrbitalInfo {
    std::string name, quantumNumbers;
    bool isGroundState;
    OrbitalFn fn;
    double maxProb, range;
    sf::Color color;
};

struct AtomData {
    std::string symbol, fullName;
    int Z;
    std::vector<OrbitalInfo> orbitals;

    std::vector<std::vector<std::pair<std::array<double, 3>, sf::Color>>> points;
    std::vector<std::pair<std::array<double, 3>, sf::Color>> groundAll, all;

    std::atomic<bool> ready{false};
    std::atomic<int> loadProgress{0};
    std::atomic<bool> startedLoading{false};
};