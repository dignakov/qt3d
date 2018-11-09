#ifndef QT3DRENDER_RENDER_VRDEVICE_P_H
#define QT3DRENDER_RENDER_VRDEVICE_P_H

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

#include <Qt3DRender/private/backendnode_p.h>
#include <QtGui/QMatrix4x4>
#include <VRDeviceImplementation/IVRDeviceImplementation.h>
#include <VRDeviceImplementation/VRPluginInfo.h>

QT_BEGIN_NAMESPACE

namespace Qt3DRender {

namespace Render {

class VRDevice : public BackendNode
{
public:
    VRDevice();
    ~VRDevice() override;
    void sceneChangeEvent(const Qt3DCore::QSceneChangePtr &e) Q_DECL_OVERRIDE;
    void initializeVR();
    void submitVR(uint leftEyeTextureId, uint rightEyeTextureId);  // called from render thread
    void updatePoses();  // Called from Jobs
    // ###QUESTION (about above): Maybe make all motion tracking part of a separate aspect?
    // Could be useful for using 3rd party motion trackers, or custom ones, or just to experiment.

    void setLeftEyeProjection(QMatrix4x4 mat);
    void setRightEyeProjection(QMatrix4x4 mat);
    void setLeftEyePose(QMatrix4x4 mat);
    void setRightEyePose(QMatrix4x4 mat);
    void setHeadPose(QMatrix4x4 mat);

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
//    void *m_hMD = nullptr; //keeping this here for now, can specialize later...
    QMatrix4x4 m_leftEyePosMatrix; //would use Pose instead of Pos
    QMatrix4x4 m_rightEyePosMatrix;
    QMatrix4x4 m_leftEyeProjection; //  = stuff
    QMatrix4x4 m_rightEyeProjection; //  = stuff
    QMatrix4x4 m_headPosMatrix;

    QString m_pluginLocation;
    VR::Plugin::VRPluginInfo m_vrinfo;
    VR::Plugin::IVRDeviceImplementation *m_vrplugin = nullptr;


    //QUESTION: Add unerlay and overlay textures here? Would that even be useful? Or is that better for a child class?
};


} // Render
} // Qt3DRender

QT_END_NAMESPACE

#endif // QT3DRENDER_RENDER_VRDEVICE_P_H
