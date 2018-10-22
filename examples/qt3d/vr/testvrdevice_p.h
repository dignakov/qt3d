#ifndef TESTVRDEVICE_P_H
#define TESTVRDEVICE_P_H


#include <Qt3DRender/private/vrdevice_p.h>


class TestVRDevice : public Qt3DRender::Render::VRDevice
{
public:
//    using VRDevice::VRDevice;
    explicit TestVRDevice()
        : Qt3DRender::Render::VRDevice()
    {}

};


#endif // QT3DRENDER_RENDER_OPENVRDEVICE_P_H
