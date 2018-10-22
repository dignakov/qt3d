#include "openvrdevice_p.h"
#include <Qt3DRender/qopenvrdevice.h>
#include <Qt3DCore/qpropertyupdatedchange.h>
#include <openvr.h>

QT_BEGIN_NAMESPACE

namespace Qt3DRender {

namespace Render {

namespace {

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

} // anonymous

OpenVRDevice::OpenVRDevice()
    : VRDevice ()
{
}

void OpenVRDevice::initializeFromPeer(const Qt3DCore::QNodeCreatedChangeBasePtr &change)
{
    qDebug()<<"[OpenVRDEvice::initializeFromPeer]";
    const auto typedChange = qSharedPointerCast<Qt3DCore::QNodeCreatedChange<QOpenVRDeviceData>>(change);
    const QOpenVRDeviceData &data = typedChange->data;
    m_leftEyeTextureId = data.leftEyeTextureId;
    m_rightEyeTextureId = data.rightEyeTextureId;
    m_nearPlane = data.nearPlane;
    m_farPlane = data.farPlane;
}

void OpenVRDevice::sceneChangeEvent(const Qt3DCore::QSceneChangePtr &e)
{
    switch (e->type()) {
    case Qt3DCore::PropertyUpdated: {
        const auto change = qSharedPointerCast<Qt3DCore::QPropertyUpdatedChange>(e);
        if (change->propertyName() == QByteArrayLiteral("nearPlane")) {
            m_nearPlane = change->value().toFloat();
            qDebug() << "nearPlane udated" << m_nearPlane;
        } else if (change->propertyName() == QByteArrayLiteral("farPlane")) {
            m_farPlane = change->value().toFloat();
            qDebug() << "farPlane udated" << m_farPlane;
        } else if (change->propertyName() == QByteArrayLiteral("leftEyeTexture")) {
            m_leftEyeTextureId = change->value().value<Qt3DCore::QNodeId>();
            qDebug() << "leftEyeTextureId udated" << m_leftEyeTextureId;
        } else if (change->propertyName() == QByteArrayLiteral("rightEyeTexture")) {
            m_rightEyeTextureId = change->value().value<Qt3DCore::QNodeId>();
            qDebug() << "rightEyeTextureId udated" << m_rightEyeTextureId;
        }
        break;
    }
    }
}

// Called from a Job
void OpenVRDevice::updatePoses()
{
    qDebug() << "[OpenVRDevice::updatePoses()]";
    if (!m_vrInitialized)
        return;

    qDebug() << "[OpenVRDevice::updatePoses()] -- INIT";
    // TO DO: Retrieve Poses
    vr::TrackedDevicePose_t trackedDevicePose[vr::k_unMaxTrackedDeviceCount];
    QMatrix4x4 matricesDevicePose[vr::k_unMaxTrackedDeviceCount];
    int validPoseCount = 0;
    QString strPoseClasses = "";

    // TO DO: Initialize once
    char deviceClassChar[vr::k_unMaxTrackedDeviceCount];   // for each device, a character representing its class
    memset(deviceClassChar, 0, sizeof(deviceClassChar));


    vr::VRCompositor()->WaitGetPoses(trackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);

    for (int nDevice = 0; nDevice < vr::k_unMaxTrackedDeviceCount; ++nDevice) {
        if (trackedDevicePose[nDevice].bPoseIsValid) {
            validPoseCount++;
            matricesDevicePose[nDevice] = hmdMatrix3x4ToQMatrix(trackedDevicePose[nDevice].mDeviceToAbsoluteTracking);
            if (deviceClassChar[nDevice] == 0) {
                switch (m_hMD->GetTrackedDeviceClass(nDevice))
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

    QMatrix4x4 hmdPos;
    if (trackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid) {
        hmdPos = matricesDevicePose[vr::k_unTrackedDeviceIndex_Hmd];
        hmdPos = hmdPos.inverted();
        hmdPos = hmdPos.transposed();
        auto e = Qt3DCore::QPropertyUpdatedChangePtr::create(peerId());
        e->setDeliveryFlags(Qt3DCore::QSceneChange::DeliverToAll);
//        qDebug() << "----------- leftEyeView " << m_leftEyePosMatrix;
        e->setPropertyName("headsetViewMatrix");
        e->setValue(QVariant::fromValue(hmdPos));
        notifyObservers(e);
    }



    if (m_leftEyePosMatrix.isIdentity()) {
        m_leftEyePosMatrix = hmdMatrix3x4ToQMatrix(m_hMD->GetEyeToHeadTransform(vr::Eye_Left));
        m_leftEyePosMatrix = m_leftEyePosMatrix.inverted();
        m_leftEyePosMatrix = m_leftEyePosMatrix.transposed();
    }

    if (m_rightEyePosMatrix.isIdentity()) {
        m_rightEyePosMatrix = hmdMatrix3x4ToQMatrix(m_hMD->GetEyeToHeadTransform(vr::Eye_Right));
        m_rightEyePosMatrix = m_rightEyePosMatrix.inverted();
        m_rightEyePosMatrix = m_rightEyePosMatrix.transposed();
    }

//    qDebug() << "---------- HMD POS " << hmdPos.column(3);
//    qDebug() << "---------- RIGHT HMD POS " << (hmdPos * m_rightEyePosMatrix).column(3);
//    qDebug() << "---------- LEFT HMD POS " << (hmdPos * m_leftEyePosMatrix).column(3);

    // Updating Projection Matrices
    /*const*/ QMatrix4x4 leftEyeProjection = hmdMatrix4x4ToQMatrix(m_hMD->GetProjectionMatrix(vr::Eye_Left, m_nearPlane, m_farPlane));
    /*const*/ QMatrix4x4 rightEyeProjection = hmdMatrix4x4ToQMatrix(m_hMD->GetProjectionMatrix(vr::Eye_Right, m_nearPlane, m_farPlane));

    leftEyeProjection = leftEyeProjection.transposed();
    rightEyeProjection = rightEyeProjection.transposed();

//    qDebug() << "---------- leftEyeProjection" << leftEyeProjection;
//    qDebug() << "---------- rightEyeProjection" << rightEyeProjection;

    //    qDebug() << leftEyeProjection << rightEyeProjection;


    {
        auto e = Qt3DCore::QPropertyUpdatedChangePtr::create(peerId());
        e->setDeliveryFlags(Qt3DCore::QSceneChange::DeliverToAll);
        e->setPropertyName("leftEyeProjectionMatrix");
        e->setValue(QVariant::fromValue(leftEyeProjection));
        notifyObservers(e);
    }
    {
        auto e = Qt3DCore::QPropertyUpdatedChangePtr::create(peerId());
        e->setDeliveryFlags(Qt3DCore::QSceneChange::DeliverToAll);
        e->setPropertyName("rightEyeProjectionMatrix");
        e->setValue(QVariant::fromValue(rightEyeProjection));
        notifyObservers(e);
    }
    {
        auto e = Qt3DCore::QPropertyUpdatedChangePtr::create(peerId());
        e->setDeliveryFlags(Qt3DCore::QSceneChange::DeliverToAll);
        e->setPropertyName("leftViewMatrix");
        e->setValue(QVariant::fromValue((m_leftEyePosMatrix * hmdPos)));
        notifyObservers(e);
    }
    {
        auto e = Qt3DCore::QPropertyUpdatedChangePtr::create(peerId());
        e->setDeliveryFlags(Qt3DCore::QSceneChange::DeliverToAll);
        e->setPropertyName("rightViewMatrix");
        e->setValue(QVariant::fromValue((m_rightEyePosMatrix * hmdPos)));
        notifyObservers(e);
    }
}

// Called by Render Thread
void OpenVRDevice::initializeVR()
{
    qDebug() << "[OpenVRDevice::initializeVR()]";
    vr::EVRInitError eError = vr::VRInitError_None;
    m_hMD = vr::VR_Init(&eError, vr::VRApplication_Scene);
    m_vrInitialized = false;

    if (eError == vr::VRInitError_None) {
        vr::IVRRenderModels *renderModels = static_cast<vr::IVRRenderModels *>(vr::VR_GetGenericInterface(vr::IVRRenderModels_Version, &eError));
        if (renderModels == nullptr) {
            qDebug() << "OpenVR: Couldn't create renderModels";
            vr::VR_Shutdown();
            return;
        }
        if (!vr::VRCompositor()) {
            qDebug() << "OpenVR: Couldn't initialize Compositor";
            vr::VR_Shutdown();
            return;
        }

        uint32_t recommendedWidth, recommendedHeight;
        m_hMD->GetRecommendedRenderTargetSize(&recommendedWidth, &recommendedHeight);
        qDebug() << "-----------------------" << recommendedWidth << recommendedHeight;

        m_vrInitialized = true;

        // Get eye Positions
        return;
    }
    qDebug() << eError << "OpenVR: Initialization Failed";
}

// Called by RenderThread
void OpenVRDevice::submitVR(uint leftEyeTextureId, uint rightEyeTextureId)
{
    qDebug() << "[OpenVRDevice::submitVR()]";
    vr::Texture_t leftEyeTexture = {(void*)(uintptr_t)leftEyeTextureId,
                                    vr::TextureType_OpenGL,
                                    vr::ColorSpace_Gamma };
    vr::VRCompositor()->Submit(vr::Eye_Left, &leftEyeTexture);
    vr::Texture_t rightEyeTexture = {(void*)(uintptr_t)rightEyeTextureId,
                                     vr::TextureType_OpenGL,
                                     vr::ColorSpace_Gamma };
    vr::VRCompositor()->Submit(vr::Eye_Right, &rightEyeTexture);
}

} // Render
} // Qt3DRender

QT_END_NAMESPACE
