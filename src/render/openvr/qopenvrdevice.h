#ifndef QT3DRENDER_QOPENVRDEVICE_H
#define QT3DRENDER_QOPENVRDEVICE_H

//#include <Qt3DRender/qt3drender_global.h>
//#include <Qt3DCore/qnode.h>
//#include <QMatrix4x4>

#include <Qt3DRender/qvrdevice.h>

QT_BEGIN_NAMESPACE

namespace Qt3DRender {

class QAbstractTexture;

struct QOpenVRDeviceData
{
    Qt3DCore::QNodeId leftEyeTextureId;
    Qt3DCore::QNodeId rightEyeTextureId;
    float nearPlane;
    float farPlane;
};

class Q_3DRENDERSHARED_EXPORT QOpenVRDevice : public Qt3DRender::QVRDevice
{
    Q_OBJECT
    Q_PROPERTY(Qt3DRender::QAbstractTexture *leftEyeTexture READ leftEyeTexture WRITE setLeftEyeTexture NOTIFY leftEyeTextureChanged)
    Q_PROPERTY(Qt3DRender::QAbstractTexture *rightEyeTexture READ rightEyeTexture WRITE setRightEyeTexture NOTIFY rightEyeTextureChanged)
    Q_PROPERTY(QMatrix4x4 leftEyeProjectionMatrix READ leftEyeProjectionMatrix NOTIFY leftEyeProjectionMatrixChanged)
    Q_PROPERTY(QMatrix4x4 rightEyeProjectionMatrix READ rightEyeProjectionMatrix NOTIFY rightEyeProjectionMatrixChanged)
    Q_PROPERTY(QMatrix4x4 leftEyeViewMatrix READ leftEyeViewMatrix NOTIFY leftEyeViewMatrixChanged)
    Q_PROPERTY(QMatrix4x4 rightEyeViewMatrix READ rightEyeViewMatrix NOTIFY rightEyeViewMatrixChanged)
    Q_PROPERTY(QMatrix4x4 headsetPositionMatrix READ headsetPositionMatrix NOTIFY headsetPositionMatrixChanged)

    Q_PROPERTY(float nearPlane READ nearPlane WRITE setNearPlane NOTIFY nearPlaneChanged)
    Q_PROPERTY(float farPlane READ farPlane WRITE setFarPlane NOTIFY farPlaneChanged)
public:
    explicit QOpenVRDevice(Qt3DCore::QNode *parent = nullptr);
    ~QOpenVRDevice();

    Qt3DRender::QAbstractTexture *leftEyeTexture() const;
    Qt3DRender::QAbstractTexture *rightEyeTexture() const;

    QMatrix4x4 leftEyeProjectionMatrix() const;
    QMatrix4x4 rightEyeProjectionMatrix() const;
    QMatrix4x4 leftEyeViewMatrix() const;
    QMatrix4x4 rightEyeViewMatrix() const;
    QMatrix4x4 headsetPositionMatrix() const;

    // Left * Headset
    QMatrix4x4 leftEyeHeadsetPositionMatrix() const;
    // Right * Headset
    QMatrix4x4 rightEyeHeadsetPositionMatrix() const;

    float nearPlane() const;
    float farPlane() const;

public Q_SLOTS:
    void setLeftEyeTexture(Qt3DRender::QAbstractTexture *leftEyeTexture);
    void setRightEyeTexture(Qt3DRender::QAbstractTexture *rightEyeTexture);

    void setNearPlane(float nearPlane);
    void setFarPlane(float farPlane);

Q_SIGNALS:
    void leftEyeTextureChanged(Qt3DRender::QAbstractTexture *leftEyeTexture);
    void rightEyeTextureChanged(Qt3DRender::QAbstractTexture *rightEyeTexture);

    void leftEyeProjectionMatrixChanged(QMatrix4x4 leftEyeProjectionMatrix);
    void rightEyeProjectionMatrixChanged(QMatrix4x4 rightEyeProjectionMatrix);

    void leftEyeViewMatrixChanged();
    void rightEyeViewMatrixChanged();
    void headsetPositionMatrixChanged();

    void nearPlaneChanged(float nearPlane);
    void farPlaneChanged(float farPlane);


protected:
    void sceneChangeEvent(const Qt3DCore::QSceneChangePtr &e) Q_DECL_OVERRIDE;

private:
    Qt3DCore::QNodeCreatedChangeBasePtr createNodeCreationChange() const Q_DECL_OVERRIDE;
    Qt3DRender::QAbstractTexture *m_leftEyeTexture = nullptr;
    Qt3DRender::QAbstractTexture *m_rightEyeTexture = nullptr;
    QMatrix4x4 m_leftEyeProjectionMatrix;
    QMatrix4x4 m_rightEyeProjectionMatrix;
    QMatrix4x4 m_leftEyeViewMatrix;
    QMatrix4x4 m_rightEyeViewMatrix;
    QMatrix4x4 m_headsetPositionMatrix;
    float m_nearPlane = 0.0f;
    float m_farPlane = 0.0f;
};

} // Qt3DRender

QT_END_NAMESPACE

#endif // QT3DRENDER_QOPENVRDEVICE_H
