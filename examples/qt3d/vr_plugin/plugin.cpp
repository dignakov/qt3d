#include <stdio.h>
#include "plugin.h"
#include <unistd.h>

//need to call this in the implementation
VRDEVICE_PLUGIN(TestVRDevice);

TestVRDevice::TestVRDevice(){
    printf("QT - VR Constructor\n");
}
TestVRDevice::~TestVRDevice(){
    printf("QT - VR Destructor\n");
}


//inline QMatrix4x4 hmdMatrix4x4ToQMatrix(const vr::HmdMatrix44_t &mVr)
//{
//    return QMatrix4x4(mVr.m[0][0], mVr.m[1][0], mVr.m[2][0], mVr.m[3][0],
//                      mVr.m[0][1], mVr.m[1][1], mVr.m[2][1], mVr.m[3][1],
//                      mVr.m[0][2], mVr.m[1][2], mVr.m[2][2], mVr.m[3][2],
//                      mVr.m[0][3], mVr.m[1][3], mVr.m[2][3], mVr.m[3][3]);
//}

//inline QMatrix4x4 hmdMatrix3x4ToQMatrix(const vr::HmdMatrix34_t &mVr)
//{
//    return QMatrix4x4(mVr.m[0][0], mVr.m[1][0], mVr.m[2][0], 0.0f,
//                      mVr.m[0][1], mVr.m[1][1], mVr.m[2][1], 0.0f,
//                      mVr.m[0][2], mVr.m[1][2], mVr.m[2][2], 0.0f,
//                      mVr.m[0][3], mVr.m[1][3], mVr.m[2][3], 1.0f);
//}


//inline QMatrix4x4 hmdMatrix3x4ToQMatrix_OnlyRotation(const vr::HmdMatrix34_t &mVr)
//{
//    return QMatrix4x4(mVr.m[0][0], mVr.m[1][0], mVr.m[2][0], 0.0f,
//                      mVr.m[0][1], mVr.m[1][1], mVr.m[2][1], 0.0f,
//                      mVr.m[0][2], mVr.m[1][2], mVr.m[2][2], 0.0f,
//                                0,           0,           0, 1.0f);
//}

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
    m_hmdPose.translate(QVector3D(0.0,0.0,0.5));

    m_default_mat = QMatrix4x4();

    t1_submit = std::chrono::high_resolution_clock::now();
    t2_submit = std::chrono::high_resolution_clock::now();
    t1_update = std::chrono::high_resolution_clock::now();
    t2_update = std::chrono::high_resolution_clock::now();
    return 0;
}

int TestVRDevice::submitVR(uintmax_t leftEyeTexID, uintmax_t rightEyeTexID, void* udata){
//    printf("QT - VR Submit: [%lu,%lu]\n",leftEyeTexID,rightEyeTexID);
    t1_submit = t2_submit;
    t2_submit = std::chrono::high_resolution_clock::now();
    float time_span = std::chrono::duration_cast<std::chrono::nanoseconds>(t2_submit - t1_submit).count()/1000000000.0;
    printf("[PLUGIN] [Qt submitVR]:         - dt: %0.2f\n", time_span*1000);

    usleep(1000000);

    return 0;
}

int TestVRDevice::updateVR(void* udata){
    t1_update = t2_update;
    t2_update = std::chrono::high_resolution_clock::now();
    t_utl = t2_update;
    float time_span = std::chrono::duration_cast<std::chrono::nanoseconds>(t2_update - t1_update).count()/1000000000.0;
    printf("[PLUGIN] [Qt updateVR]:         - dt: %0.2f\n", time_span*1000);
    utl += time_span*1000;
//    float t_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(t_utl)/1000000000.0f;
//    auto duration = std::chrono::system_clock::now().time_since_epoch();
//    float t_s = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count()/1000.0f;
    float q = 1.0f*sin(1.0f * utl);
    qDebug()<<q;
    m_hmdPose(0,3) = q;


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
//    printf("VR getting pose for HMD\n");
    qDebug()<<"PLUGRR" <<m_hmdPose.column(3);
    return m_hmdPose;
}
