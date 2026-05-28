#pragma once
#include "WaveFunction.hpp"
#include "Atom.hpp"
#include <memory>
#include <utility>

inline OrbitalFn scaleZ(OrbitalFn fn, int Z) {
    return [fn, Z](double x, double y, double z) {
        return fn(x * Z, y * Z, z * Z) * Z * Z * Z;
    };
}

inline std::unique_ptr<AtomData> makeHydrogen() {
    auto atom = std::make_unique<AtomData>();
    atom->symbol = "H";
    atom->fullName = "Hydrogen";
    atom->Z = 1;

    atom->orbitals = {
        {"1s", "n=1, l=0, m=0", true, psi2_1s, 1.0 / PI, 5.0, sf::Color(255, 255, 255)},
        {"2s", "n=2, l=0, m=0", true,  psi2_2s, 0.05, 10.0, sf::Color(200, 200, 100)},
        {"2pz", "n=2, l=1, m=0", false, psi2_2pz, 0.0005, 15.0, sf::Color(100, 180, 255)},
        {"2px", "n=2, l=1, m=1", false, psi2_2px, 0.0005, 15.0, sf::Color(255, 150, 100)},
        {"3dz2", "n=3, l=2, m=0", false, psi2_3dz2, 0.00002, 30.0, sf::Color(100, 255, 150)},
        {"3dxz", "n=3, l=2, m=1", false, psi2_3dxz, 0.00002, 30.0, sf::Color(255, 100, 200)},
        {"3dxy", "n=3, l=2, m=2", false, psi2_3dxy, 0.00002, 30.0, sf::Color(200, 100, 255)},
        {"4fz3", "n=4, l=3, m=0", false, psi2_4fz3, 0.000001, 50.0, sf::Color(255, 80, 80)},
    };

    return atom;
}

inline std::unique_ptr<AtomData> makeHelium() {
    auto atom = std::make_unique<AtomData>();
    atom->symbol = "He";
    atom->fullName = "Helium";
    atom->Z = 2;

    auto s1s = scaleZ(psi2_1s, 2);
    auto s2s = scaleZ(psi2_2s, 2);
    auto s2pz = scaleZ(psi2_2pz, 2);
    auto s2px = scaleZ(psi2_2px, 2);

    atom->orbitals = {
        {"1s", "n=1, l=0, m=0", true, s1s, 1.0 / PI * 8, 3.0, sf::Color(255, 255, 255)},
        {"2s", "n=2, l=0, m=0", false, s2s, 0.05, 8.0, sf::Color(200, 200, 100)},
        {"2pz", "n=2, l=1, m=0", false, s2pz, 0.001, 10.0, sf::Color(100, 180, 255)},
        {"2px", "n=2, l=1, m=1", false, s2px, 0.001, 10.0, sf::Color(255, 150, 100)},
    };

    return atom;
}

inline std::unique_ptr<AtomData> makeLithium() {
    auto atom = std::make_unique<AtomData>();
    atom->symbol = "Li";
    atom->fullName = "Lithium";
    atom->Z = 3;

    auto s1s = scaleZ(psi2_1s, 3);
    auto s2s = scaleZ(psi2_2s, 3);
    auto s2pz = scaleZ(psi2_2pz, 3);
    auto s2px = scaleZ(psi2_2px, 3);
    auto s3dz = scaleZ(psi2_3dz2, 3);

    atom->orbitals = {
        {"1s", "n=1, l=0, m=0", true, s1s, 1.0 / PI * 27, 2.0, sf::Color(255, 255, 255)},
        {"2s", "n=2, l=0, m=0", true, s2s, 0.05, 7.0, sf::Color(200, 200, 100)},
        {"2pz", "n=2, l=1, m=0", false, s2pz, 0.001, 8.0, sf::Color(100, 180, 255)},
        {"2px", "n=2, l=1, m=1", false, s2px, 0.001, 8.0, sf::Color(255, 150, 100)},
        {"3dz2", "n=3, l=2, m=0", false, s3dz, 0.0001, 15.0, sf::Color(100, 255, 150)},
    };

    return atom;
}

inline std::unique_ptr<AtomData> makeCarbon() {
    auto atom = std::make_unique<AtomData>();
    atom->symbol = "C";
    atom->fullName = "Carbon";
    atom->Z = 6;

    auto s1s = scaleZ(psi2_1s, 6);
    auto s2s = scaleZ(psi2_2s, 6);
    auto s2pz = scaleZ(psi2_2pz, 6);
    auto s2px = scaleZ(psi2_2px, 6);
    auto s2py = scaleZ(psi2_3py, 6);
    auto s3dz = scaleZ(psi2_3dz2, 6);

    atom->orbitals = {
        {"1s", "n=1, l=0, m=0", true, s1s, 1.0 / PI * 216, 1.0, sf::Color(255, 255, 255)},
        {"2s", "n=2, l=0, m=0", true, s2s, 0.05, 5.0, sf::Color(200, 200, 100)},
        {"2px", "n=2, l=1, m=1", true, s2px, 0.002, 6.0, sf::Color(255, 150, 100)},
        {"2py", "n=2, l=1, m=-1", true, s2py, 0.002, 6.0, sf::Color(100, 255, 255)},
        {"2pz", "n=2, l=1, m=0", true, s2pz, 0.002, 6.0, sf::Color(100, 180, 255)},
        {"3dz2", "n=3, l=2, m=0", false, s3dz, 0.0001, 12.0, sf::Color(100, 255, 150)},
    };

    return atom;
}

inline std::unique_ptr<AtomData> makeNitrogen() {
    auto atom = std::make_unique<AtomData>();
    atom->symbol = "N";
    atom->fullName = "Nitrogen";
    atom->Z = 7;

    auto s1s = scaleZ(psi2_1s, 7);
    auto s2s = scaleZ(psi2_2s, 7);
    auto s2pz = scaleZ(psi2_2pz, 7);
    auto s2px = scaleZ(psi2_2px, 7);
    auto s2py = scaleZ(psi2_3py, 7);
    auto s3dz = scaleZ(psi2_3dz2, 7);

    atom->orbitals = {
        {"1s", "n=1, l=0, m=0", true, s1s, 1.0 / PI * 343, 0.8, sf::Color(255, 255, 255)},
        {"2s", "n=2, l=0, m=0", true, s2s, 0.05, 4.5, sf::Color(200, 200, 100)},
        {"2px", "n=2, l=1, m=1", true, s2px, 0.003, 5.5,  sf::Color(255, 150, 100)},
        {"2py", "n=2, l=1, m=-1",true, s2py, 0.003, 5.5, sf::Color(100, 255, 255)},
        {"2pz", "n=2, l=1, m=0", true, s2pz, 0.003, 5.5, sf::Color(100, 180, 255)},
        {"3dz2", "n=3, l=2, m=0", false, s3dz, 0.0001, 11.0, sf::Color(100, 255, 150)},
    };

    return atom;
}

inline std::unique_ptr<AtomData> makeOxygen() {
    auto atom = std::make_unique<AtomData>();
    atom->symbol = "O";
    atom->fullName = "Oxygen";
    atom->Z = 8;

    auto s1s = scaleZ(psi2_1s, 8);
    auto s2s = scaleZ(psi2_2s, 8);
    auto s2pz = scaleZ(psi2_2pz, 8);
    auto s2px = scaleZ(psi2_2px, 8);
    auto s2py = scaleZ(psi2_3py, 8);
    auto s3dz = scaleZ(psi2_3dz2, 8);

    atom->orbitals = {
        {"1s", "n=1, l=0, m=0", true, s1s, 1.0 / PI * 512, 0.7, sf::Color(255, 255, 255)},
        {"2s", "n=2, l=0, m=0", true, s2s, 0.05, 4.0, sf::Color(200, 200, 100)},
        {"2px", "n=2, l=1, m=1", true, s2px, 0.003, 5.0, sf::Color(255, 150, 100)},
        {"2py", "n=2, l=1, m=-1",true, s2py, 0.003, 5.0, sf::Color(100, 255, 255)},
        {"2pz", "n=2, l=1, m=0", true, s2pz, 0.003, 5.0, sf::Color(100, 180, 255)},
        {"3dz2", "n=3, l=2, m=0", false, s3dz, 0.0001, 10.0, sf::Color(100, 255, 150)},
    };

    return atom;
}

inline std::unique_ptr<AtomData> makeNeon() {
    auto atom = std::make_unique<AtomData>();
    atom->symbol = "Ne";
    atom->fullName = "Neon";
    atom->Z = 10;

    auto s1s  = scaleZ(psi2_1s,  10);
    auto s2s  = scaleZ(psi2_2s,  10);
    auto s2pz = scaleZ(psi2_2pz, 10);
    auto s2px = scaleZ(psi2_2px, 10);
    auto s2py = scaleZ(psi2_3py, 10);
    auto s3dz = scaleZ(psi2_3dz2,10);

    atom->orbitals = {
        {"1s", "n=1, l=0, m=0", true, s1s, 1.0 / PI * 1000, 0.5, sf::Color(255, 255, 255)},
        {"2s", "n=2, l=0, m=0", true, s2s, 0.05, 3.5, sf::Color(200, 200, 100)},
        {"2px", "n=2, l=1, m=1", true, s2px, 0.004, 4.5, sf::Color(255, 150, 100)},
        {"2py", "n=2, l=1, m=-1",true, s2py, 0.004, 4.5, sf::Color(100, 255, 255)},
        {"2pz", "n=2, l=1, m=0", true, s2pz, 0.004, 4.5, sf::Color(100, 180, 255)},
        {"3dz2", "n=3, l=2, m=0", false, s3dz, 0.0001, 9.0, sf::Color(100, 255, 150)},
    };

    return atom;
}