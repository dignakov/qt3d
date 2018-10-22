#ifndef QT3DRENDER_RENDER_OPENVRDEVICE_P_H
#define QT3DRENDER_RENDER_OPENVRDEVICE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of other Qt classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

//#include <Qt3DRender/private/backendnode_p.h>
//#include <QtGui/QMatrix4x4>
#include <Qt3DRender/private/vrdevice_p.h>

namespace vr {
class IVRSystem;
} // vr

QT_BEGIN_NAMESPACE

namespace Qt3DRender {

namespace Render {

class OpenVRDevice : public VRDevice
{
public:
    OpenVRDevice();
    void sceneChangeEvent(const Qt3DCore::QSceneChangePtr &e) Q_DECL_OVERRIDE;
    void initializeVR();
    void submitVR(uint leftEyeTextureId, uint rightEyeTextureId);

    // Called from Jobs
    void updatePoses();

    inline Qt3DCore::QNodeId leftEyeTextureId() const { return m_leftEyeTextureId; }
    inline Qt3DCore::QNodeId rightEyeTextureId() const { return m_rightEyeTextureId; }
    inline bool isVRInitialized() const { return m_vrInitialized; }
    inline float nearPlane() const { return m_nearPlane; }
    inline float farPlane() const { return m_farPlane; }

private:
    void initializeFromPeer(const Qt3DCore::QNodeCreatedChangeBasePtr &change) Q_DECL_FINAL;
    Qt3DCore::QNodeId m_leftEyeTextureId;
    Qt3DCore::QNodeId m_rightEyeTextureId;
    float m_nearPlane = 0.0f;
    float m_farPlane = 0.0f;
    bool m_vrInitialized = false;
    vr::IVRSystem *m_hMD = nullptr;
    QMatrix4x4 m_leftEyePosMatrix;
    QMatrix4x4 m_rightEyePosMatrix;
};


} // Render
} // Qt3DRender

QT_END_NAMESPACE

#endif // QT3DRENDER_RENDER_OPENVRDEVICE_P_H
