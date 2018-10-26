#ifndef QT3DRENDER_RENDER_VRSUBMITCOMMAND_H
#define QT3DRENDER_RENDER_VRSUBMITCOMMAND_H

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

#include <Qt3DRender/private/framegraphnode_p.h>
#include <Qt3DRender/qvrsubmitcommand.h>

QT_BEGIN_NAMESPACE

namespace Qt3DRender {

namespace Render {

class VRSubmitCommand : public FrameGraphNode
{
public:
    VRSubmitCommand();

    void sceneChangeEvent(const Qt3DCore::QSceneChangePtr &e) Q_DECL_OVERRIDE;
    Qt3DCore::QNodeId deviceId() const { return m_deviceId; }

private:
    Qt3DCore::QNodeId m_deviceId;

    void initializeFromPeer(const Qt3DCore::QNodeCreatedChangeBasePtr &change) Q_DECL_FINAL;
};

} // Render

} // Qt3DRender

QT_END_NAMESPACE

#endif // QT3DRENDER_RENDER_VRSUBMITCOMMAND_H
