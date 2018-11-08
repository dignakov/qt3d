#include <stdio.h>
#include "openvr_plugin.h"
#include <openvr.h>

inline QMatrix4x4 hmdMatrix4x4ToQMatrix(const vr::HmdMatrix44_t &mVr)
{
    return QMatrix4x4(mVr.m[0][0], mVr.m[1][0], mVr.m[2][0], mVr.m[3][0],
                      mVr.m[0][1], mVr.m[1][1], mVr.m[2][1], mVr.m[3][1],
                      mVr.m[0][2], mVr.m[1][2], mVr.m[2][2], mVr.m[3][2],
                      mVr.m[0][3], mVr.m[1][3], mVr.m[2][3], mVr.m[3][3]);
}

inline QMatrix4x4 hmdMatrix3x4ToQMatrix(const vr::HmdMatrix34_t &mVr)
{
    return QMatrix4x4(mVr.m[0][0], mVr.m[1][0], mVr.m[2][0], 0.0f,
                      mVr.m[0][1], mVr.m[1][1], mVr.m[2][1], 0.0f,
                      mVr.m[0][2], mVr.m[1][2], mVr.m[2][2], 0.0f,
                      mVr.m[0][3], mVr.m[1][3], mVr.m[2][3], 1.0f);
}



//need to call this in the implementation
VRDEVICE_PLUGIN(OpenVRDevice);

OpenVRDevice::OpenVRDevice(){
    printf("OpenVR Plugin Constructor\n");
}
OpenVRDevice::~OpenVRDevice(){
    printf("OpenVR  Plugin Destructor\n");
}

int OpenVRDevice::initializeVR(void* udata){
    printf("OpenVR Plugin Init\n");
    m_leftEyeProjection = QMatrix4x4();
    m_rightEyeProjection = QMatrix4x4();
    m_leftEyePose = QMatrix4x4();
    m_rightEyePose = QMatrix4x4();
    m_hmdPose = QMatrix4x4();
    m_default_mat = QMatrix4x4();

    vr::EVRInitError eError = vr::VRInitError_None;
    m_HMD = vr::VR_Init(&eError, vr::VRApplication_Scene);

    if (eError != vr::VRInitError_None) {
        qDebug()<<"OpenVR: could not initialize";
        return 1;  // some error flag...
    }

//    //DO I CARE ???
//    vr::IVRRenderModels *renderModels = static_cast<vr::IVRRenderModels *>(vr::VR_GetGenericInterface(vr::IVRRenderModels_Version, &eError));
//    if (renderModels == nullptr) {
//        qDebug() << "OpenVR: Couldn't create renderModels";
//        vr::VR_Shutdown();
//        return 2;  // some other error flag...
//    }

    if (!vr::VRCompositor()) {
        qDebug() << "OpenVR: Couldn't initialize Compositor";
        vr::VR_Shutdown();
        return 2;  // some other error flag...
    }

    //shoudl have this returnable to crate textures . . .
    uint32_t recommendedWidth, recommendedHeight;
    m_HMD->GetRecommendedRenderTargetSize(&recommendedWidth, &recommendedHeight);
    qDebug() << "-----------------------" << recommendedWidth << recommendedHeight;


    //just identity for now
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
