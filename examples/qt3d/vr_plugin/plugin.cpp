#include <stdio.h>
#include "plugin.h"

//need to call this in the implementation
VRDEVICE_PLUGIN(TestVRDevice);

TestVRDevice::TestVRDevice(){
    printf("QT - VR Constructor\n");
}
TestVRDevice::~TestVRDevice(){
    printf("QT - VR Destructor\n");
}

int TestVRDevice::initializeVR(void* udata){
    printf("QT - VR Init\n");
    return 0;
}

int TestVRDevice::submitVR(uintmax_t leftEyeTexID, uintmax_t rightEyeTexID, void* udata){
    printf("QT - VR Submit: [%lu,%lu]\n",leftEyeTexID,rightEyeTexID);
    return 0;
}

int TestVRDevice::update(void* udata){
    printf("QT - VR Update\n");
    return 0;
}

int TestVRDevice::shutdownVR(void* udata){
    printf("QT - VR Shutdown\n");
    return 0;
}

int TestVRDevice::shouldClose(){
    return false;
}
