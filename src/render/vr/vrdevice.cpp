#include "vrdevice_p.h"
#include <Qt3DRender/qvrdevice.h>
#include <Qt3DCore/qpropertyupdatedchange.h>
//#include <openvr.h>

QT_BEGIN_NAMESPACE

namespace Qt3DRender {

namespace Render {

//namespace {

////inline QMatrix4x4 hmdMatrix4x4ToQMatrix(const vr::HmdMatrix44_t &mVr)
////{
////    return QMatrix4x4(mVr.m[0][0], mVr.m[1][0], mVr.m[2][0], mVr.m[3][0],
////                      mVr.m[0][1], mVr.m[1][1], mVr.m[2][1], mVr.m[3][1],
////                      mVr.m[0][2], mVr.m[1][2], mVr.m[2][2], mVr.m[3][2],
////                      mVr.m[0][3], mVr.m[1][3], mVr.m[2][3], mVr.m[3][3]);
////}

////inline QMatrix4x4 hmdMatrix3x4ToQMatrix(const vr::HmdMatrix34_t &mVr)
////{
////    return QMatrix4x4(mVr.m[0][0], mVr.m[1][0], mVr.m[2][0], 0.0f,
////                      mVr.m[0][1], mVr.m[1][1], mVr.m[2][1], 0.0f,
////                      mVr.m[0][2], mVr.m[1][2], mVr.m[2][2], 0.0f,
////                      mVr.m[0][3], mVr.m[1][3], mVr.m[2][3], 1.0f);
////}

//} // anonymous

VRDevice::VRDevice()
    : BackendNode(ReadWrite)
{
}

void VRDevice::initializeFromPeer(const Qt3DCore::QNodeCreatedChangeBasePtr &change)
{
    const auto typedChange = qSharedPointerCast<Qt3DCore::QNodeCreatedChange<QVRDeviceData>>(change);
    const QVRDeviceData &data = typedChange->data;
    m_leftEyeTextureId = data.leftEyeTextureId;
    m_rightEyeTextureId = data.rightEyeTextureId;
    m_nearPlane = data.nearPlane;
    m_farPlane = data.farPlane;
}

void VRDevice::sceneChangeEvent(const Qt3DCore::QSceneChangePtr &e)
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
void VRDevice::updatePoses()
{
    qDebug() << "[VRDevice::updatePose()] - NOT IMPLEMENTED";
    if (!m_vrInitialized)
        return;

    // TO DO: Retrieve Poses
    int validPoseCount = 0;

    // FAKE UPDATE hmdPos; // I would use hmdPose **
    QMatrix4x4 hmdPos;
    auto e = Qt3DCore::QPropertyUpdatedChangePtr::create(peerId());
    e->setDeliveryFlags(Qt3DCore::QSceneChange::DeliverToAll);
    e->setPropertyName("headsetViewMatrix");
    e->setValue(QVariant::fromValue(hmdPos));
    notifyObservers(e);

    // left eye pose
//    m_leftEyePosMatrix = stuff

    // right eye pose
//    m_rightEyePosMatrix = stuff

//    qDebug() << "---------- HMD POS " << hmdPos.column(3);
//    qDebug() << "---------- RIGHT HMD POS " << (hmdPos * m_rightEyePosMatrix).column(3);
//    qDebug() << "---------- LEFT HMD POS " << (hmdPos * m_leftEyePosMatrix).column(3);

    // Updating Projection Matrices // should not happen often . . .
    QMatrix4x4 leftEyeProjection; //  = stuff
    QMatrix4x4 rightEyeProjection; //  = stuff

//    qDebug() << "---------- leftEyeProjection" << leftEyeProjection;
//    qDebug() << "---------- rightEyeProjection" << rightEyeProjection;

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
void VRDevice::initializeVR()
{
    m_vrInitialized = true;
    qDebug() << "[VRDevice::intializeVR] - NOT IMPLEMENTED";
}

// Called by RenderThread
void VRDevice::submitVR(uint leftEyeTextureId, uint rightEyeTextureId)
{
    qDebug() << "[VRDevice::submitVR] - NOT IMPLEMENTED";
}

} // Render
} // Qt3DRender

QT_END_NAMESPACE
