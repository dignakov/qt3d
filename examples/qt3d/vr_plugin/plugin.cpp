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

    //save parent device
//    m_parentDevice = (Qt3DRender::Render::VRDevice*)udata;

//    m_parentDevice->setLeftEyeProjection(m_leftEyeProjection);
//    m_parentDevice->setRightEyeProjection(m_rightEyeProjection);
//    m_parentDevice->setLeftEyePose(m_leftEyePose);
//    m_parentDevice->setRightEyePose(m_rightEyePose);
//    m_parentDevice->setHeadPose(m_hmdPose);
    return 0;
}

int TestVRDevice::submitVR(uintmax_t leftEyeTexID, uintmax_t rightEyeTexID, void* udata){
    printf("QT - VR Submit: [%lu,%lu]\n",leftEyeTexID,rightEyeTexID);
    return 0;
}

int TestVRDevice::update(void* udata){
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
