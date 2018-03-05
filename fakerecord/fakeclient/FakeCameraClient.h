#ifndef ANDROID_FAKE_CAMERA_CLIENT_H
#define ANDROID_FAKE_CAMERA_CLIENT_H

#include <thread>
#include <mutex>
#include <binder/IMemory.h>
#include <list>
#include "../common/IFakeCameraProxyListener.h"

namespace android {

class FakeCameraClient: public RefBase
{
public:
    FakeCameraClient();
    virtual ~FakeCameraClient();
    virtual void showFrame_loop();
    virtual void linkToServer();
protected:
    class ProxyListener: public BnFakeCameraProxyListener
    {
    public:
        ProxyListener(const sp<FakeCameraClient>& client);
        virtual void callBackFrame(int numFrame);
    private:
        sp<FakeCameraClient> mClient;
    };

    class DeathNotifier: public IBinder::DeathRecipient {
    public:
        DeathNotifier() {}
        virtual void binderDied(const wp<IBinder>& who);
    };

    virtual void callBackFrame(int numFrame);

private:
    // Memory used to send the buffers to encoder, where sp<IMemory> stores VideoNativeMetadata.
    sp<IMemoryHeap> mMemoryHeapBase;
    std::list<sp<IMemory>> mMemoryBases;

    std::list<sp<IMemory>> mFramesReceived;

    void createVideoBufferMemoryHeap(size_t size, uint32_t bufferCount);
    std::mutex mLock;
    std::thread t;
    sp<IFakeCameraService>   mServer;
    sp<DeathNotifier> mDeathNotifier;
};

}

#endif
