#ifndef QT3DRENDER_QVRSUBMITCOMMAND_H
#define QT3DRENDER_QVRSUBMITCOMMAND_H

#include <Qt3DRender/qt3drender_global.h>
#include <Qt3DRender/qframegraphnode.h>

QT_BEGIN_NAMESPACE

namespace Qt3DRender {

class QVRDevice;

struct QVRSubmitCommandData
{
    Qt3DCore::QNodeId deviceId;
};

class Q_3DRENDERSHARED_EXPORT QVRSubmitCommand : public QFrameGraphNode
{
    Q_OBJECT
    Q_PROPERTY(Qt3DRender::QVRDevice *device READ device WRITE setDevice NOTIFY deviceChanged)
public:
    explicit QVRSubmitCommand(Qt3DCore::QNode *parent = nullptr);
    QVRDevice *device() const;

public Q_SLOTS:
    void setDevice(QVRDevice *device);

Q_SIGNALS:
    void deviceChanged(QVRDevice *device);

private:
    QVRDevice *m_device;
    Qt3DCore::QNodeCreatedChangeBasePtr createNodeCreationChange() const Q_DECL_OVERRIDE;
};

} // Qt3DRender

QT_END_NAMESPACE

#endif // QT3DRENDER_QVRSUBMITCOMMAND_H
