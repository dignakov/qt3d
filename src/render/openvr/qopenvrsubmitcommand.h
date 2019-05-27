#ifndef QT3DRENDER_QOPENVRSUBMITCOMMAND_H
#define QT3DRENDER_QOPENVRSUBMITCOMMAND_H

#include <Qt3DRender/qt3drender_global.h>
#include <Qt3DRender/qframegraphnode.h>

QT_BEGIN_NAMESPACE

namespace Qt3DRender {

class QOpenVRDevice;

struct QOpenVRSubmitCommandData
{
    Qt3DCore::QNodeId deviceId;
};

class Q_3DRENDERSHARED_EXPORT QOpenVRSubmitCommand : public QFrameGraphNode
{
    Q_OBJECT
    Q_PROPERTY(Qt3DRender::QOpenVRDevice *device READ device WRITE setDevice NOTIFY deviceChanged)
public:
    explicit QOpenVRSubmitCommand(Qt3DCore::QNode *parent = nullptr);
    QOpenVRDevice *device() const;

public Q_SLOTS:
    void setDevice(QOpenVRDevice *device);

Q_SIGNALS:
    void deviceChanged(QOpenVRDevice *device);

private:
    QOpenVRDevice *m_device;
    Qt3DCore::QNodeCreatedChangeBasePtr createNodeCreationChange() const Q_DECL_OVERRIDE;
};

} // Qt3DRender

QT_END_NAMESPACE

#endif // QT3DRENDER_QOPENVRSUBMITCOMMAND_H
