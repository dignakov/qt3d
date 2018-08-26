#include "qopenvrsubmitcommand.h"
#include <Qt3DRender/qframegraphnodecreatedchange.h>
#include <Qt3DRender/qopenvrdevice.h>

QT_BEGIN_NAMESPACE

namespace Qt3DRender {

QOpenVRSubmitCommand::QOpenVRSubmitCommand(Qt3DCore::QNode *parent)
    : Qt3DRender::QFrameGraphNode(parent)
    , m_device(nullptr)
{
}

QOpenVRDevice *QOpenVRSubmitCommand::device() const
{
    return m_device;
}

void QOpenVRSubmitCommand::setDevice(QOpenVRDevice *device)
{
    if (m_device == device)
        return;

    m_device = device;
    emit deviceChanged(device);
}

Qt3DCore::QNodeCreatedChangeBasePtr QOpenVRSubmitCommand::createNodeCreationChange() const
{
    auto creationChange = QFrameGraphNodeCreatedChangePtr<QOpenVRSubmitCommandData>::create(this);
    QOpenVRSubmitCommandData &data = creationChange->data;
    data.deviceId = Qt3DCore::qIdForNode(m_device);
    return creationChange;
}

} // Qt3DRender

QT_END_NAMESPACE
