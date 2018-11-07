#include <stdio.h>
#include "plugin.h"

//need to call this in the implementation
VRDEVICE_PLUGIN(TestVRDevice);

TestVRDevice::TestVRDevice(){
    printf("QT - VR Constructor\n");
}
TestVRDevice::~TestVRDevice(){
    printf("QT - VR Destructor\n");
}

int TestVRDevice::initializeVR(void* udata){
    printf("QT - VR Init\n");
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

int TestVRDevice::submitVR(uintmax_t leftEyeTexID, uintmax_t rightEyeTexID, void* udata){
    printf("QT - VR Submit: [%lu,%lu]\n",leftEyeTexID,rightEyeTexID);
    return 0;
}

int TestVRDevice::updateVR(void* udata){
    printf("QT - VR Update\n");
    return 0;
}

int TestVRDevice::shutdownVR(void* udata){
    printf("QT - VR Shutdown\n");
    return 0;
}

int TestVRDevice::shouldClose(){
    return false;
}

QMatrix4x4 TestVRDevice::getEyeProjection(VR::eyeId eye){
    printf("VR getting projecton for eye: %d\n",eye);
    if(eye==VR::eyeLeft)
        return m_leftEyeProjection;
    else if(eye==VR::eyeRight)
        return m_rightEyeProjection;
    else
        return m_default_mat;
}

QMatrix4x4 TestVRDevice::getEyePose(VR::eyeId eye){
    printf("VR getting pose for eye: %d\n",eye);
    if(eye==VR::eyeLeft)
        return m_leftEyePose;
    else if(eye==VR::eyeRight)
        return m_rightEyePose;
    else
        return m_default_mat;
}

QMatrix4x4 TestVRDevice::getHmdPose(){
    printf("VR getting pose for HMD\n");
    return m_hmdPose;
}
