#include <stdio.h>
#include "openvr_plugin.h"

//need to call this in the implementation
VRDEVICE_PLUGIN(OpenVRDevice);

OpenVRDevice::OpenVRDevice(){
    printf("OpenVR Constructor\n");
}
OpenVRDevice::~OpenVRDevice(){
    printf("OpenVR Destructor\n");
}

int OpenVRDevice::initializeVR(void* udata){
    printf("OpenVR Init\n");
    m_leftEyeProjection = QMatrix4x4();
    m_rightEyeProjection = QMatrix4x4();
    m_leftEyeProjection.perspective(90.0f, 1.3f, 0.01f, 1000.0f);
    m_rightEyeProjection.perspective(90.0f, 1.0f, 0.01f, 1000.0f);

    //just identity for now
    m_leftEyePose = QMatrix4x4();
    m_rightEyePose = QMatrix4x4();
    m_hmdPose = QMatrix4x4();

    m_default_mat = QMatrix4x4();
    return 0;
}

int OpenVRDevice::submitVR(uintmax_t leftEyeTexID, uintmax_t rightEyeTexID, void* udata){
    printf("OpenVR Submit: [%lu,%lu]\n",leftEyeTexID,rightEyeTexID);
    return 0;
}

int OpenVRDevice::updateVR(void* udata){
    printf("OpenVR Update\n");
    return 0;
}

int OpenVRDevice::shutdownVR(void* udata){
    printf("OpenVR Shutdown\n");
    return 0;
}

int OpenVRDevice::shouldClose(){
    return false;
}

QMatrix4x4 OpenVRDevice::getEyeProjection(VR::eyeId eye){
    printf("OpenVR getting projecton for eye: %d\n",eye);
    if(eye==VR::eyeLeft)
        return m_leftEyeProjection;
    else if(eye==VR::eyeRight)
        return m_rightEyeProjection;
    else
        return m_default_mat;
}

QMatrix4x4 OpenVRDevice::getEyePose(VR::eyeId eye){
    printf("OpenVR getting pose for eye: %d\n",eye);
    if(eye==VR::eyeLeft)
        return m_leftEyePose;
    else if(eye==VR::eyeRight)
        return m_rightEyePose;
    else
        return m_default_mat;
}

QMatrix4x4 OpenVRDevice::getHmdPose(){
    printf("OpenVR getting pose for HMD\n");
    return m_hmdPose;
}
