#define LOG_TAG "FakeCameraClient"

#include <utils/Log.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include "../fakecamera/FakeCameraService.h"
#include <binder/IPCThreadState.h>
#include <binder/MemoryBase.h>
#include <binder/MemoryHeapBase.h>
#include "FakeCameraClient.h"

namespace android {

FakeCameraClient::ProxyListener::ProxyListener(const sp<FakeCameraClient>& client):
	mClient(client)
{
	ALOGE("jiazai proxy listener cons");
}

void FakeCameraClient::ProxyListener::callBackFrame(int numFrame)
{
	ALOGE("jiazai listener do callback %d", numFrame);
	mClient->callBackFrame(numFrame);
}

FakeCameraClient::FakeCameraClient()
{
	ALOGE("jiazai FakeCameraClient");
	createVideoBufferMemoryHeap(sizeof(int), 16);
	sp<IServiceManager> sm = defaultServiceManager();
    if (sm == NULL) {
        ALOGE("jiazai SerivceManager is NULL !!!!");
        return;
    }
    sp<IBinder> binder = sm->getService(String16("media.fakecamera"));
    if (binder == NULL) {
        ALOGE("jiazai Failed to get service");
        return;
    }

    mServer = IFakeCameraService::asInterface(binder);
    if (mServer == NULL) {
        ALOGE("jiazai get interface failed");
        return;
    }
    mDeathNotifier = new DeathNotifier();
    IInterface::asBinder(mServer)->linkToDeath(mDeathNotifier);
    t = std::thread([](FakeCameraClient* odj){odj->showFrame_loop();}, this);
}

FakeCameraClient::~FakeCameraClient()
{
	ALOGE("jiazai FakeCameraClient release");
}

void FakeCameraClient::createVideoBufferMemoryHeap(size_t size, uint32_t bufferCount) {
    mMemoryHeapBase = new MemoryHeapBase(size * bufferCount, 0,
            "Fake-CameraSource-BufferHeap");
    for (uint32_t i = 0; i < bufferCount; i++) {
        mMemoryBases.push_back(new MemoryBase(mMemoryHeapBase, i * size, size));
    }
}

void FakeCameraClient::linkToServer()
{
	ALOGE("jiazai strong count %d before linkToServer", getStrongCount());
	mServer->setListener(new ProxyListener(this));
	ALOGE("jiazai strong count %d after linkToServer", getStrongCount());
}

void FakeCameraClient::callBackFrame(int numFrame)
{
	ALOGE("jiazai client get callback %d", numFrame);
	std::lock_guard<std::mutex>  temp_lock(mLock);
	while (mMemoryBases.empty()) {
		ALOGE("jiazai share memory empty");
		return;
	}
	sp<IMemory> data = *mMemoryBases.begin();
    mMemoryBases.erase(mMemoryBases.begin());
    int *frameData = (int*)(data->pointer());
    *frameData = numFrame;
    mFramesReceived.push_back(data);
}

void FakeCameraClient::showFrame_loop() {
	while(true) {
		while (mFramesReceived.empty()) {
			ALOGE("jiazai Received memory empty");
			if (!IInterface::asBinder(mServer)->isBinderAlive()) {
				ALOGW("camera recording proxy is gone");
				return;
			}
			std::this_thread::sleep_for(std::chrono::seconds(1));
			continue;
		}
		mLock.lock();
		sp<IMemory> data = *mFramesReceived.begin();
		int *frameData = (int*)(data->pointer());
		ALOGE("jiazai client frame %d", *frameData);
		mMemoryBases.push_back(data);
		mFramesReceived.erase(mFramesReceived.begin());
		mLock.unlock();
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void FakeCameraClient::DeathNotifier::binderDied(const wp<IBinder>& who __unused) {
    ALOGI("Camera recording proxy died");
}

}

using namespace android;

int main(int argc, char** argv)
{

	sp<FakeCameraClient> client = new FakeCameraClient();
	
	client->linkToServer();

	/* neccessary for binder transfer.....*/
	ProcessState::self()->startThreadPool();
	/* not quit*/
	IPCThreadState::self()->joinThreadPool();

	return 0;
}
