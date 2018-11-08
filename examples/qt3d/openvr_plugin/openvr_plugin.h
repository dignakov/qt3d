#ifndef OPENVR_PLUGIN_TEST_H
#define OPENVR_PLUGIN_TEST_H

#include <QMatrix4x4>
#include <VRDeviceImplementation/IVRDeviceImplementation.h>


namespace vr {
class IVRSystem;
} // vr

class OpenVRDevice : public VR::Plugin::IVRDeviceImplementation
{
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

};

#endif
