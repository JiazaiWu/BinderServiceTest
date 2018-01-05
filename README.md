# BinderServiceTest
try to implement a android native service

1.0
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
1) add service claim at service_contexts (system/sepolicy/private/service_contexts)
media.fakecamera                          u:object_r:mediafakecamera_service:s0

   add service type as service.te (system/sepolicy/public/service.te)
type mediafakecamera_service,   service_manager_type;

   allow service be found (system_server.te, mediaserver.te)
allow system_server mediafakecamera_service:service_manager find;
allow mediaserver mediafakecamera_service:service_manager find;

2) the following code is neccessary to make Bp find Bn
   data.writeInterfaceToken(IFakeCameraService::getInterfaceDescriptor());

3) std::thread.join() will make block main thread untill thread finish
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


1.1
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
1) sp<> can call getStrongCount() for debug ^o^

2) The following code is nessessary for binder to find Bn
   ProcessState::self()->startThreadPool()
   IPCThreadState::self()->joinThreadPool();

3) Client can create Bnlisenter to Server to do callback (WHY server call listener is Bp call?)

4) log:
6801  6801 E FakeCameraService: jiazai create FakeCameraService // <--server main thread
6801  6802 E FakeCameraService: jiazai numFrame 0 // <-- server new thread
......
6887  6887 E FakeCameraClient: jiazai FakeCameraClient // <--client main thread
6887  6887 E IFakeCameraService: jiazai setListener
6801  6803 E FakeCameraService: jiazai setListener // <-- what is this? binder thread? it means binder thread cat server(6801)
6887  6888 E FakeCameraClient: jiazai client frame 0 // <-- client show thread
......
6801  6802 E FakeCameraService: jiazai server use listener // <-- following code will loop
6801  6802 E IFakeCameraProxyListener: jiazai Bp callBackFrame 7
6887  6887 E IFakeCameraProxyListener: jiazai Bn callBackFrame 7 // <-- NOTICE BN CALL HANDLE IN MAIN THREAD!!!
6887  6887 E FakeCameraClient: jiazai listener do callback 7
6887  6887 E FakeCameraClient: jiazai client get callback 7
6801  6802 E FakeCameraService: jiazai numFrame 7


