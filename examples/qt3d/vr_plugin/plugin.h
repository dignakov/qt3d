#ifndef VR_PLUGIN_TEST_H
#define VR_PLUGIN_TEST_H

#include <QObject>
#include <QtPlugin>
#include <QMatrix4x4>
//#include <Qt3DRender/private/vrdevice_p.h> // don't really want to be including private mats here
#include <VRDeviceImplementation/IVRDeviceImplementation.h>
#include <chrono>

class TestVRDevice : public QObject, VR::Plugin::IVRDeviceImplementation
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID VRDEVICE_INTERFACE_IID)
    Q_INTERFACES(VR::Plugin::IVRDeviceImplementation)

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

//    Qt3DRender::Render::VRDevice *m_parentDevice;

    float utl = 0;

    //some timing
    std::chrono::high_resolution_clock::time_point t1_submit, t2_submit, t1_update, t2_update, t_utl;
};

#endif
