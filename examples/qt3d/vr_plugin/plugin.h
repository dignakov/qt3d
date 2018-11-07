#ifndef VR_PLUGIN_TEST_H
#define VR_PLUGIN_TEST_H

#include <QMatrix4x4>
#include <Qt3DRender/private/vrdevice_p.h> // don't really want to be including private mats here
#include <VRDeviceImplementation/IVRDeviceImplementation.h>
//#include <VRDeviceImplementation/VRPluginTypes.h>

class TestVRDevice : public VR::Plugin::IVRDeviceImplementation
{
public:
    TestVRDevice ();
    ~TestVRDevice ();

    int initializeVR(void* udata=nullptr);
    int submitVR(uintmax_t leftEyeTexID, uintmax_t rightEyeTexID, void* udata = nullptr);
    int updateVR(void* udata=nullptr);
    int shutdownVR(void* udata = nullptr);
    int shouldClose();

    QMatrix4x4 getEyeProjection(VR::eyeId eye);
    QMatrix4x4 getEyePose(VR::eyeId eye);
    QMatrix4x4 getHmdPose();

private:

    QMatrix4x4 m_leftEyeProjection;
    QMatrix4x4 m_rightEyeProjection;
    QMatrix4x4 m_leftEyePose;
    QMatrix4x4 m_rightEyePose;
    QMatrix4x4 m_hmdPose;

    QMatrix4x4 m_default_mat;

    Qt3DRender::Render::VRDevice *m_parentDevice;
};

#endif
