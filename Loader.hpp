#pragma once
#include "Atom.hpp"
#include <thread>
#include <random>

std::vector<std::pair<std::array<double, 3>, sf::Color>> generatePoints(const OrbitalInfo& orbital) {
    std::mt19937 rng(42);
    std::uniform_real_distribution<double> spaceDist(-orbital.range, orbital.range);
    std::uniform_real_distribution<double> probDist(0.0, orbital.maxProb);

    std::vector<std::pair<std::array<double, 3>, sf::Color>> points;
    for (int i = 0; i < 10000000; ++i) {
        double x = spaceDist(rng), y = spaceDist(rng), z = spaceDist(rng);
        double prob = orbital.fn(x, y, z);

        if (probDist(rng) < prob) {
            points.push_back({{x, y, z}, orbital.color});
        }
    }

    return points;
}

inline void loadAtomAsync(AtomData& atom) {
    if (atom.startedLoading) {
        return;
    }

    atom.startedLoading = true;

    std::thread([&atom] {
        int total = atom.orbitals.size();
        atom.points.resize(total);

        for (int i = 0; i < total; ++i) {
            atom.points[i] = generatePoints(atom.orbitals[i]);
            atom.loadProgress = i + 1;

            for (auto& p : atom.points[i]) {
                atom.all.push_back(p);

                if (atom.orbitals[i].isGroundState) {
                    atom.groundAll.push_back(p);
                }
            }
        }

        atom.ready = true;
    }).detach();
}