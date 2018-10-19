#include "vrsubmitcommand_p.h"
#include <Qt3DCore/qpropertyupdatedchange.h>

QT_BEGIN_NAMESPACE

namespace Qt3DRender {

namespace Render {

VRSubmitCommand::VRSubmitCommand()
    : FrameGraphNode(FrameGraphNode::VRSubmit) //NOTE: add to framegraphnode_p.h
{
}


void VRSubmitCommand::initializeFromPeer(const Qt3DCore::QNodeCreatedChangeBasePtr &change)
{
    FrameGraphNode::initializeFromPeer(change);
    const auto typedChange = qSharedPointerCast<Qt3DCore::QNodeCreatedChange<QVRSubmitCommandData>>(change);
    const QVRSubmitCommandData &data = typedChange->data;
    m_deviceId = data.deviceId;
}

void VRSubmitCommand::sceneChangeEvent(const Qt3DCore::QSceneChangePtr &e)
{
    if (e->type() == Qt3DCore::PropertyUpdated) {
        Qt3DCore::QPropertyUpdatedChangePtr propertyChange = qSharedPointerCast<Qt3DCore::QPropertyUpdatedChange>(e);
        if (propertyChange->propertyName() == QByteArrayLiteral("device")) {
            m_deviceId = propertyChange->value().value<Qt3DCore::QNodeId>();
            qDebug() << "Device updated" << m_deviceId;
        }
        markDirty(AbstractRenderer::AllDirty);
    }
    FrameGraphNode::sceneChangeEvent(e);
}

} // Render

} // Qt3DRender

QT_END_NAMESPACE
