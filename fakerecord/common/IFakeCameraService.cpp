#define LOG_TAG "IMediaCodecService"

#include <utils/Log.h>
#include <binder/Parcel.h>
#include "IFakeCameraService.h"

namespace android {

enum {
    GET_FRAME = 0
};

class BpFakeCameraService : public BpInterface<IFakeCameraService>
{
public:
    explicit BpFakeCameraService(const sp<IBinder>& impl)
        : BpInterface<IFakeCameraService>(impl)
    {
    }

    virtual int getFrame() {
        Parcel data, reply;
        /* neccessary to communicate with FakeCameraService*/
        data.writeInterfaceToken(IFakeCameraService::getInterfaceDescriptor());
        remote()->transact(GET_FRAME, data, &reply);
        return reply.readInt32();
    }
};

IMPLEMENT_META_INTERFACE(FakeCameraService, "android.media.IFakeCameraService");

// ----------------------------------------------------------------------

status_t BnFakeCameraService::onTransact(
    uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{
    switch (code) {
        case GET_FRAME: {
            CHECK_INTERFACE(IFakeCameraService, data, reply);
            reply->writeInt32(getFrame());
            return NO_ERROR;
        }
        default:
            return BBinder::onTransact(code, data, reply, flags);
    }
}

}