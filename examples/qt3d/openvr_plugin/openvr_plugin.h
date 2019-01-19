#ifndef OPENVR_PLUGIN_TEST_H
#define OPENVR_PLUGIN_TEST_H

#include <QObject>
#include <QtPlugin>
#include <QMatrix4x4>
#include <VRDeviceImplementation/IVRDeviceImplementation.h>
#include <chrono>


namespace vr {
class IVRSystem;
} // vr

class OpenVRDevice : public QObject, VR::Plugin::IVRDeviceImplementation
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID VRDEVICE_INTERFACE_IID)
    Q_INTERFACES(VR::Plugin::IVRDeviceImplementation)

public:
    OpenVRDevice ();
    ~OpenVRDevice ();

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

    //OpenVR HMD
    vr::IVRSystem *m_HMD = nullptr;
    //some timing
    std::chrono::high_resolution_clock::time_point t1_submit, t2_submit, t1_update, t2_update;


};

#endif
