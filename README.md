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
