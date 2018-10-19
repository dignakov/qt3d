#include "qvrsubmitcommand.h"
#include <Qt3DRender/qframegraphnodecreatedchange.h>
#include <Qt3DRender/qvrdevice.h>

QT_BEGIN_NAMESPACE

namespace Qt3DRender {

QVRSubmitCommand::QVRSubmitCommand(Qt3DCore::QNode *parent)
    : Qt3DRender::QFrameGraphNode(parent)
    , m_device(nullptr)
{
}

QVRDevice *QVRSubmitCommand::device() const
{
    return m_device;
}

void QVRSubmitCommand::setDevice(QVRDevice *device)
{
    if (m_device == device)
        return;

    m_device = device;
    emit deviceChanged(device);
}

Qt3DCore::QNodeCreatedChangeBasePtr QVRSubmitCommand::createNodeCreationChange() const
{
    auto creationChange = QFrameGraphNodeCreatedChangePtr<QVRSubmitCommandData>::create(this);
    QVRSubmitCommandData &data = creationChange->data;
    data.deviceId = Qt3DCore::qIdForNode(m_device);
    return creationChange;
}

} // Qt3DRender

QT_END_NAMESPACE
