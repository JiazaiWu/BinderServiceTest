#define LOG_TAG "FakeCameraService"

#include <utils/Log.h>
#include <utils/Errors.h>
#include <binder/IMemory.h>
#include <binder/IPCThreadState.h>
#include <binder/MemoryBase.h>
#include <binder/MemoryHeapBase.h>
#include "FakeCameraService.h"

namespace android {

FakeCameraService::FakeCameraService():BnFakeCameraService(),
	numFrame(0),
	mStart(false)
{
	ALOGE("jiazai create FakeCameraService +++++");
	mStart = true;
	startAddFrame();
	ALOGE("jiazai create FakeCameraService -----");
}

void FakeCameraService::startAddFrame() {
	t = std::thread([](FakeCameraService* odj){odj->addAndshowFrame();}, this);
	/* join will make main thread wait
	*  so code after startAddFrame will not go on*/
	//t.join();
}

void FakeCameraService::stopFrame() {
	mStart = false;
}


int FakeCameraService::getFrame() {
    std::lock_guard<std::mutex>  temp_lock(mLock);
    ALOGE("jiazai getFrame %d", numFrame);
    return numFrame;
}

void FakeCameraService::addAndshowFrame() {
	int res;
	while (mStart) {
		mLock.lock();
		res = numFrame;
		numFrame++;
		mLock.unlock();
		std::this_thread::sleep_for(std::chrono::seconds(1));
    	ALOGE("jiazai numFrame %d", res);
	}
}


status_t FakeCameraService::onTransact(uint32_t code, const Parcel& data, Parcel* reply,
        uint32_t flags)
{
    return BnFakeCameraService::onTransact(code, data, reply, flags);
}


}// namespace android

using namespace android;

int main(int argc, char** argv)
{

	strcpy(argv[0], FakeCameraService::getServiceName());


	/*see BinderService.h 
	 *sm->addSerivce(String16(SERVICE::getServiceName()) here	
	 *FakeCameraService::instantiate();
	 */

	FakeCameraService::publishAndJoinThreadPool();
	ALOGE("jiazai add service done");


	ProcessState::self()->startThreadPool();
    IPCThreadState::self()->joinThreadPool();

}