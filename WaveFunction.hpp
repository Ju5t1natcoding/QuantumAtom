#pragma once
#include <cmath>

constexpr double PI = 3.14159265358979323846;

/// 1s - simple sphere
inline double psi2_1s(double x, double y, double z) {
    double r = std::sqrt(x * x + y * y + z * z);
    return (1.0 / PI) * std::exp(-2.0 * r);
}

///2s - sphere with a radial node
inline double psi2_2s(double x, double y, double z) {
    double r = std::sqrt(x * x + y * y + z * z);
    double psi = (1.0 / (4.0 * std::sqrt(2.0 * PI))) * (2.0 - r) * std::exp(-r / 2.0);
    return psi * psi;
}

/// 2pz - for the Z-axis
inline double psi2_2pz(double x, double y, double z) {
    double r = std::sqrt(x * x + y * y + z * z);
    double cosTheta = (r > 0) ? z / r : 0;
    double R = (1.0 / (4.0 * std::sqrt(2.0))) * (r / 2.0) * std::exp(-r / 2.0);
    double Y = std::sqrt(3.0 / (4.0 * PI)) * cosTheta;
    double psi = R * Y;
    return psi * psi;
}

///2px - for the X-axis
inline double psi2_2px(double x, double y, double z) {
    double r = std::sqrt(x * x + y * y + z * z);
    double sinTheta = (r > 0) ? std::sqrt(x * x + y * y) / r : 0;
    double cosPhi = (std::sqrt(x * x + y * y) > 0) ? x / std::sqrt(x * x + y * y) : 0;
    double R = (1.0 / (4.0 * std::sqrt(2.0))) * (r / 2.0) * std::exp(-r / 2.0);
    double Y = std::sqrt(3.0 / (4.0 * PI)) * sinTheta * cosPhi;
    double psi = R * Y;
    return psi * psi;
}

// 3py
inline double psi2_3py(double x, double y, double z) {
    double r = std::sqrt(x * x + y * y + z * z);
    double sinTheta = (r > 0) ? std::sqrt(x * x + y * y) / r : 0;
    double sinPhi = (std::sqrt(x * x + y * y) > 0) ? y / std::sqrt(x * x + y * y) : 0;
    double R = (1.0 / (81.0 * std::sqrt(6.0))) * (r / 3.0) * std::exp(-r / 3.0);
    double Y = std::sqrt(3.0 / (4.0 * PI)) * sinTheta * sinPhi;
    double psi = R * Y;
    return psi * psi;
}

// 3pz
inline double psi2_3pz(double x, double y, double z) {
    double r = std::sqrt(x * x + y * y + z * z);
    double cosTheta = (r > 0) ? z / r : 0;
    double R = (1.0 / (81.0 * std::sqrt(6.0))) * (r / 3.0) * std::exp(-r / 3.0);
    double Y = std::sqrt(3.0 / (4.0 * PI)) * cosTheta;
    double psi = R * Y;
    return psi * psi;
}

// 3dxy - forma de trifoi in planul XY
inline double psi2_3dxy(double x, double y, double z) {
    double r = std::sqrt(x * x + y * y + z * z);
    double sinTheta = (r > 0) ? std::sqrt(x * x + y * y) / r : 0;
    double rxy = std::sqrt(x * x + y * y);
    double sin2Phi = (rxy > 0) ? 2.0 * x * y / (rxy * rxy) : 0;
    double R = (1.0 / (81.0 * std::sqrt(6.0))) * (r * r / 9.0) * std::exp(-r / 3.0);
    double Y = std::sqrt(15.0 / (16.0 * PI)) * sinTheta * sinTheta * sin2Phi;
    double psi = R * Y;
    return psi * psi;
}

///3dz2 - trifoi dublu
inline double psi2_3dz2(double x, double y, double z) {
    double r = std::sqrt(x * x + y * y + z * z);
    double cosTheta = (r > 0) ? z / r : 0;
    double R = (1.0 / (81.0 * std::sqrt(6.0))) * (r * r / 9.0) * std::exp(-r / 3.0);
    double Y = std::sqrt(5.0 / (16.0 * PI)) * (3.0 * cosTheta * cosTheta - 1.0);
    double psi = R * Y;
    return psi * psi;
}

///3dxz
inline double psi2_3dxz(double x, double y, double z) {
    double r = std::sqrt(x * x + y * y + z * z);
    double cosTheta = (r > 0) ? z / r : 0;
    double sinTheta = (r > 0) ? std::sqrt(x *x + y * y) / r : 0;
    double cosPhi = (std::sqrt(x * x + y * y) > 0) ? x / std::sqrt(x * x + y * y) : 0;
    double R = (1.0 / (81.0 * std::sqrt(6.0))) * (r * r / 9.0) * std::exp(-r / 3.0);
    double Y = std::sqrt(15.0 / (4.0 * PI)) * sinTheta * cosTheta * cosPhi;
    double psi = R * Y;
    return psi * psi;
}

// 4fz3 - orbital f, forma complexa
inline double psi2_4fz3(double x, double y, double z) {
    double r = std::sqrt(x * x + y * y + z * z);
    double cosTheta = (r > 0) ? z / r : 0;
    double R = (1.0 / (768.0)) * (r * r * r / 64.0) * std::exp(-r / 4.0);
    double Y = std::sqrt(7.0 / (16.0 * PI)) * (5.0 * cosTheta*cosTheta*cosTheta - 3.0 * cosTheta);
    double psi = R * Y;
    return psi * psi;
}