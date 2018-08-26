#include "qopenvrdevice.h"
#include <Qt3DCore/qpropertyupdatedchange.h>
#include <Qt3DRender/qabstracttexture.h>

QT_BEGIN_NAMESPACE

namespace Qt3DRender {

QOpenVRDevice::QOpenVRDevice(Qt3DCore::QNode *parent)
    : Qt3DCore::QNode(parent)
{

}

QOpenVRDevice::~QOpenVRDevice()
{

}

QAbstractTexture *QOpenVRDevice::leftEyeTexture() const
{
    return m_leftEyeTexture;
}

QAbstractTexture *QOpenVRDevice::rightEyeTexture() const
{
    return m_rightEyeTexture;
}

QMatrix4x4 QOpenVRDevice::leftEyeProjectionMatrix() const
{
    return m_leftEyeProjectionMatrix;
}

QMatrix4x4 QOpenVRDevice::rightEyeProjectionMatrix() const
{
    return m_rightEyeProjectionMatrix;
}

QMatrix4x4 QOpenVRDevice::leftEyeViewMatrix() const
{
    return m_leftEyeViewMatrix;
}

QMatrix4x4 QOpenVRDevice::rightEyeViewMatrix() const
{
    return m_rightEyeViewMatrix;
}

QMatrix4x4 QOpenVRDevice::headsetPositionMatrix() const
{
    return m_headsetPositionMatrix;
}

float QOpenVRDevice::nearPlane() const
{
    return m_nearPlane;
}

float QOpenVRDevice::farPlane() const
{
    return m_farPlane;
}

QMatrix4x4 QOpenVRDevice::leftEyeHeadsetPositionMatrix() const
{
    return m_leftEyeViewMatrix * m_headsetPositionMatrix;
}

QMatrix4x4 QOpenVRDevice::rightEyeHeadsetPositionMatrix() const
{
    return m_rightEyeViewMatrix * m_headsetPositionMatrix;
}

void QOpenVRDevice::setLeftEyeTexture(QAbstractTexture *leftEyeTexture)
{
    if (m_leftEyeTexture == leftEyeTexture)
        return;

    m_leftEyeTexture = leftEyeTexture;
    emit leftEyeTextureChanged(leftEyeTexture);
}

void QOpenVRDevice::setRightEyeTexture(QAbstractTexture *rightEyeTexture)
{
    if (m_rightEyeTexture == rightEyeTexture)
        return;

    m_rightEyeTexture = rightEyeTexture;
    emit rightEyeTextureChanged(rightEyeTexture);
}

void QOpenVRDevice::setNearPlane(float nearPlane)
{
    if (m_nearPlane == nearPlane)
        return;

    m_nearPlane = nearPlane;
    emit nearPlaneChanged(nearPlane);
}

void QOpenVRDevice::setFarPlane(float farPlane)
{
    if (m_farPlane == farPlane)
        return;

    m_farPlane = farPlane;
    emit farPlaneChanged(farPlane);
}

void QOpenVRDevice::sceneChangeEvent(const Qt3DCore::QSceneChangePtr &change)
{
    Qt3DCore::QPropertyUpdatedChangePtr e = qSharedPointerCast<Qt3DCore::QPropertyUpdatedChange>(change);
    if (e->type() == Qt3DCore::PropertyUpdated) {
        if (e->propertyName() == QByteArrayLiteral("leftEyeProjectionMatrix")) {
            m_leftEyeProjectionMatrix = e->value().value<QMatrix4x4>();
            emit leftEyeProjectionMatrixChanged(m_leftEyeProjectionMatrix);
        } else if (e->propertyName() == QByteArrayLiteral("rightEyeProjectionMatrix")) {
            m_rightEyeProjectionMatrix = e->value().value<QMatrix4x4>();
            emit rightEyeProjectionMatrixChanged(m_rightEyeProjectionMatrix);
        } else if (e->propertyName() == QByteArrayLiteral("leftViewMatrix")) {
            m_leftEyeViewMatrix = e->value().value<QMatrix4x4>();
            emit leftEyeViewMatrixChanged();
        } else if (e->propertyName() == QByteArrayLiteral("rightViewMatrix")) {
            m_rightEyeViewMatrix = e->value().value<QMatrix4x4>();
            emit rightEyeViewMatrixChanged();
        } else if (e->propertyName() == QByteArrayLiteral("headsetViewMatrix")) {
            m_headsetPositionMatrix = e->value().value<QMatrix4x4>();
            emit rightEyeViewMatrixChanged();
        }
    }
//    qDebug() << "---------- HMD POS " << m_headsetPositionMatrix.column(3);
//    qDebug() << "---------- RIGHT HMD POS " << m_rightEyeViewMatrix.column(3);
//    qDebug() << "---------- LEFT HMD POS " << m_leftEyeViewMatrix.column(3);
}

Qt3DCore::QNodeCreatedChangeBasePtr QOpenVRDevice::createNodeCreationChange() const
{
    auto creationChange = Qt3DCore::QNodeCreatedChangePtr<QOpenVRDeviceData>::create(this);
    QOpenVRDeviceData &data = creationChange->data;
    data.leftEyeTextureId = Qt3DCore::qIdForNode(m_leftEyeTexture);
    data.rightEyeTextureId = Qt3DCore::qIdForNode(m_rightEyeTexture);
    data.nearPlane = m_nearPlane;
    data.farPlane = m_farPlane;
    return creationChange;
}

} // Qt3DRender

QT_END_NAMESPACE
