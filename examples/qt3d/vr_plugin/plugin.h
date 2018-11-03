#ifndef VR_PLUGIN_TEST_H
#define VR_PLUGIN_TEST_H

// ###QUESTION: how do I get these headers as part of QT
#include "../../../src/render/vr_plugin_interface/include/VRDeviceImplementation/IVRDeviceImplementation.h"

class TestVRDevice : public VR::Plugin::IVRDeviceImplementation
{
public:
    TestVRDevice ();
    ~TestVRDevice ();

    int initializeVR(void* udata=nullptr);
    int submitVR(uintmax_t leftEyeTexID, uintmax_t rightEyeTexID, void* udata = nullptr);
    int update(void* udata=nullptr);
    int shutdownVR(void* udata = nullptr);
    int shouldClose();


};

#endif
