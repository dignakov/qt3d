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
        qDebug()<< vr::VR_GetVRInitErrorAsEnglishDescription( eError );
        return 1;
    }

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

    //init eye projections
    // TODO: add near and far planes
    m_leftEyeProjection = hmdMatrix4x4ToQMatrix(m_HMD->GetProjectionMatrix(vr::Eye_Left, 0.01f, 1000.0f));
    m_rightEyeProjection = hmdMatrix4x4ToQMatrix(m_HMD->GetProjectionMatrix(vr::Eye_Right, 0.01f, 1000.0f));
    m_leftEyeProjection = m_leftEyeProjection.transposed();
    m_rightEyeProjection = m_rightEyeProjection.transposed();

    //init eye poses:
    m_leftEyePose = hmdMatrix3x4ToQMatrix(m_HMD->GetEyeToHeadTransform(vr::Eye_Left));
    m_leftEyePose = m_leftEyePose.inverted();
    m_leftEyePose = m_leftEyePose.transposed();

    m_rightEyePose = hmdMatrix3x4ToQMatrix(m_HMD->GetEyeToHeadTransform(vr::Eye_Right));
    m_rightEyePose = m_rightEyePose.inverted();
    m_rightEyePose = m_rightEyePose.transposed();

    //shoudl have this returnable to crate textures . . .
    uint32_t recommendedWidth, recommendedHeight;
    m_HMD->GetRecommendedRenderTargetSize(&recommendedWidth, &recommendedHeight);
    qDebug() << "-----------------------" << recommendedWidth << recommendedHeight;


    //just identity for now
    return 0;
}

int OpenVRDevice::submitVR(uintmax_t leftEyeTexID, uintmax_t rightEyeTexID, void* udata){
    printf("OpenVR Submit: [%lu,%lu]\n",leftEyeTexID,rightEyeTexID);

    vr::Texture_t leftEyeTexture = {(void*)(uintptr_t)leftEyeTexID,
                                    vr::TextureType_OpenGL,
                                    vr::ColorSpace_Gamma };
    vr::VRCompositor()->Submit(vr::Eye_Left, &leftEyeTexture);
    vr::Texture_t rightEyeTexture = {(void*)(uintptr_t)rightEyeTexID,
                                     vr::TextureType_OpenGL,
                                     vr::ColorSpace_Gamma };
    vr::VRCompositor()->Submit(vr::Eye_Right, &rightEyeTexture);

    return 0;
}

int OpenVRDevice::updateVR(void* udata){
    printf("OpenVR Update\n");

    //UPDATE ALL THE THINGS
    vr::TrackedDevicePose_t trackedDevicePose[vr::k_unMaxTrackedDeviceCount];
    QMatrix4x4 matricesDevicePose[vr::k_unMaxTrackedDeviceCount];
    int validPoseCount = 0;
    QString strPoseClasses = "";

    // TO DO: Initialize once
    char deviceClassChar[vr::k_unMaxTrackedDeviceCount];   // for each device, a character representing its class
    memset(deviceClassChar, 0, sizeof(deviceClassChar));

    vr::VRCompositor()->WaitGetPoses(trackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);

    for (uint nDevice = 0; nDevice < vr::k_unMaxTrackedDeviceCount; ++nDevice) {
        if (trackedDevicePose[nDevice].bPoseIsValid) {
            validPoseCount++;
            matricesDevicePose[nDevice] = hmdMatrix3x4ToQMatrix(trackedDevicePose[nDevice].mDeviceToAbsoluteTracking);
            if (deviceClassChar[nDevice] == 0) {
                switch (m_HMD->GetTrackedDeviceClass(nDevice))
                {
                case vr::TrackedDeviceClass_Controller:        deviceClassChar[nDevice] = 'C'; break;
                case vr::TrackedDeviceClass_HMD:               deviceClassChar[nDevice] = 'H'; break;
                case vr::TrackedDeviceClass_Invalid:           deviceClassChar[nDevice] = 'I'; break;
                case vr::TrackedDeviceClass_GenericTracker:    deviceClassChar[nDevice] = 'G'; break;
                case vr::TrackedDeviceClass_TrackingReference: deviceClassChar[nDevice] = 'T'; break;
                default:                                       deviceClassChar[nDevice] = '?'; break;
                }
            }
            strPoseClasses += deviceClassChar[nDevice];
        }
    }

    //update HMD pose
    if (trackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid) {
        m_hmdPose = matricesDevicePose[vr::k_unTrackedDeviceIndex_Hmd];
        m_hmdPose =m_hmdPose.inverted();
        m_hmdPose =m_hmdPose.transposed();
        qDebug()<<m_hmdPose;
    }

    //TODO: update controller pose(s)



    return 0;
}

int OpenVRDevice::shutdownVR(void* udata){
    printf("OpenVR Shutdown\n");
    vr::VR_Shutdown();
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
