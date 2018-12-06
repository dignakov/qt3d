#include "qvrdevice.h"
#include <Qt3DCore/qpropertyupdatedchange.h>
#include <Qt3DRender/qabstracttexture.h>

QT_BEGIN_NAMESPACE

namespace Qt3DRender {

QVRDevice::QVRDevice(Qt3DCore::QNode *parent)
    : Qt3DCore::QNode(parent)
{

}

QVRDevice::~QVRDevice()
{

}

QAbstractTexture *QVRDevice::leftEyeTexture() const
{
    return m_leftEyeTexture;
}

QAbstractTexture *QVRDevice::rightEyeTexture() const
{
    return m_rightEyeTexture;
}

QMatrix4x4 QVRDevice::leftEyeProjectionMatrix() const
{
    return m_leftEyeProjectionMatrix;
}

QMatrix4x4 QVRDevice::rightEyeProjectionMatrix() const
{
    return m_rightEyeProjectionMatrix;
}

QMatrix4x4 QVRDevice::leftEyeViewMatrix() const
{
//    qDebug()<< "QR LHM" << m_leftEyeViewMatrix;
    return m_leftEyeViewMatrix;
}

QMatrix4x4 QVRDevice::rightEyeViewMatrix() const
{
    return m_rightEyeViewMatrix;
}

QMatrix4x4 QVRDevice::headsetPositionMatrix() const
{
    return m_headsetPositionMatrix;
}

float QVRDevice::nearPlane() const
{
    return m_nearPlane;
}

float QVRDevice::farPlane() const
{
    return m_farPlane;
}

QString QVRDevice::pluginLocation() const
{
    return m_pluginLocation;
}

//QMatrix4x4 QVRDevice::leftEyeHeadsetPositionMatrix() const
//{
////    qDebug() << m_leftEyeViewMatrix;
////    qDebug()<<(m_leftEyeViewMatrix * m_headsetPositionMatrix);
//    return m_leftEyeViewMatrix * m_headsetPositionMatrix;
//}

//QMatrix4x4 QVRDevice::rightEyeHeadsetPositionMatrix() const
//{
//    return m_rightEyeViewMatrix * m_headsetPositionMatrix;
//}

void QVRDevice::setLeftEyeTexture(QAbstractTexture *leftEyeTexture)
{
    if (m_leftEyeTexture == leftEyeTexture)
        return;

    m_leftEyeTexture = leftEyeTexture;
    emit leftEyeTextureChanged(leftEyeTexture);
}

void QVRDevice::setRightEyeTexture(QAbstractTexture *rightEyeTexture)
{
    if (m_rightEyeTexture == rightEyeTexture)
        return;

    m_rightEyeTexture = rightEyeTexture;
    emit rightEyeTextureChanged(rightEyeTexture);
}

void QVRDevice::setNearPlane(float nearPlane)
{
    //not suitable for gereneral case of very small or very large floats
    if (std::abs(m_nearPlane - nearPlane)<0.0000001f)
        return;

    m_nearPlane = nearPlane;
    emit nearPlaneChanged(nearPlane);
}

void QVRDevice::setFarPlane(float farPlane)
{
    if (std::abs(m_farPlane - farPlane)<0.0000001f)
        return;

    m_farPlane = farPlane;
    emit farPlaneChanged(farPlane);
}

void QVRDevice::setPluginLocation(QString pluginLocation)
{
    if(m_pluginLocation==pluginLocation)
        return;

    m_pluginLocation = pluginLocation;
    emit pluginLocationChanged(pluginLocation);
}

void QVRDevice::sceneChangeEvent(const Qt3DCore::QSceneChangePtr &change)
{
    Qt3DCore::QPropertyUpdatedChangePtr e = qSharedPointerCast<Qt3DCore::QPropertyUpdatedChange>(change);
    if (e->type() == Qt3DCore::PropertyUpdated) {
        if (e->propertyName() == QByteArrayLiteral("leftEyeProjectionMatrix")) {
            m_leftEyeProjectionMatrix = e->value().value<QMatrix4x4>();
            emit leftEyeProjectionMatrixChanged(m_leftEyeProjectionMatrix);
            //qDebug()<<"getting LEFT projection: " << m_leftEyeProjectionMatrix;
        } else if (e->propertyName() == QByteArrayLiteral("rightEyeProjectionMatrix")) {
            m_rightEyeProjectionMatrix = e->value().value<QMatrix4x4>();
            emit rightEyeProjectionMatrixChanged(m_rightEyeProjectionMatrix);
            //qDebug()<<"getting RIGHT projection: " << m_rightEyeProjectionMatrix;
        } else if (e->propertyName() == QByteArrayLiteral("leftViewMatrix")) {
            m_leftEyeViewMatrix = e->value().value<QMatrix4x4>();
            emit leftEyeViewMatrixChanged();
//            qDebug()<<"QTSin LHM: " << m_leftEyeViewMatrix;
        } else if (e->propertyName() == QByteArrayLiteral("rightViewMatrix")) {
            m_rightEyeViewMatrix = e->value().value<QMatrix4x4>();
            emit rightEyeViewMatrixChanged();
            //qDebug()<<"getting RIGHT view: " << m_rightEyeViewMatrix;
        } else if (e->propertyName() == QByteArrayLiteral("headsetViewMatrix")) {
            m_headsetPositionMatrix = e->value().value<QMatrix4x4>();
//            emit rightEyeViewMatrixChanged(); //TODO: this is wrong
            emit headsetPositionMatrixChanged();
//            qDebug()<<"    QT" << m_headsetPositionMatrix.column(3);
//            qDebug() << ()
        }
    }
//    qDebug() << "---------- HMD POS " << m_headsetPositionMatrix.column(3);
//    qDebug() << "---------- RIGHT HMD POS " << m_rightEyeViewMatrix.column(3);
//    qDebug() << "---------- LEFT HMD POS " << m_leftEyeViewMatrix.column(3);
}

Qt3DCore::QNodeCreatedChangeBasePtr QVRDevice::createNodeCreationChange() const
{
    auto creationChange = Qt3DCore::QNodeCreatedChangePtr<QVRDeviceData>::create(this);
    QVRDeviceData &data = creationChange->data;
    data.leftEyeTextureId = Qt3DCore::qIdForNode(m_leftEyeTexture);
    data.rightEyeTextureId = Qt3DCore::qIdForNode(m_rightEyeTexture);
    data.nearPlane = m_nearPlane;
    data.farPlane = m_farPlane;
    data.pluginLocation = m_pluginLocation;
//    data.leftEyeProjection = m_leftEyeProjectionMatrix;
//    data.rightEyeProjection = m_rightEyeProjectionMatrix;
    return creationChange;
}

} // Qt3DRender

QT_END_NAMESPACE
