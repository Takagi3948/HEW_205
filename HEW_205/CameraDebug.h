#pragma once
#include "Camera.h" 
#include "Input.h" 

class CameraDebug : public Camera {
private:
    float m_radXZ;
    float m_radY;
    float m_radius;

public:
    CameraDebug();
    ~CameraDebug();
    void Update() final;
};