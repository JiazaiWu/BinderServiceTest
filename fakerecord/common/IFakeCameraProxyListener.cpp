#define LOG_TAG "IFakeCameraProxyListener"

#include <utils/Log.h>
#include <binder/Parcel.h>
#include "IFakeCameraProxyListener.h"

namespace android {

enum {
    CALLBACK_FRAME = 0
};

class BpFakeCameraProxyListener : public BpInterface<IFakeCameraProxyListener>
{
public:
    explicit BpFakeCameraProxyListener(const sp<IBinder>& impl)
        : BpInterface<IFakeCameraProxyListener>(impl)
    {
    }

    virtual void callBackFrame(int numFrame) {
        ALOGE("jiazai Bp callBackFrame %d" ,numFrame);
        Parcel data, reply;
        /* neccessary to communicate with FakeCameraService*/
        data.writeInterfaceToken(IFakeCameraProxyListener::getInterfaceDescriptor());
        data.writeInt32(numFrame);
        remote()->transact(CALLBACK_FRAME, data, &reply);
    }
};

IMPLEMENT_META_INTERFACE(FakeCameraProxyListener, "android.media.IFakeCameraProxyListener");

// ----------------------------------------------------------------------

status_t BnFakeCameraProxyListener::onTransact(
    uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{
    switch (code) {
        case CALLBACK_FRAME: {
            CHECK_INTERFACE(IFakeCameraProxyListener, data, reply);
            int numFrame = data.readInt32();
            ALOGE("jiazai Bn callBackFrame %d" ,numFrame);
            callBackFrame(numFrame);
            return NO_ERROR;
        }
        default:
            return BBinder::onTransact(code, data, reply, flags);
    }
}

}