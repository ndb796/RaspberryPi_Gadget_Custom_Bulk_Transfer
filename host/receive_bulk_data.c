#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <usb.h>

int main(void) {
    int vid = 0x1D6B;
    int pid = 0x0104;
    int targetInterfaceNumber = 1; // 인터페이스 1번 (Guest → Host)

    usb_init();
    usb_find_busses();
    usb_find_devices();

    struct usb_bus *bus;
    for (bus = usb_get_busses(); bus; bus = bus->next) {
        // 연결되어 있는 모든 USB 장치를 하나씩 확인하며
        struct usb_device *dev;
        for (dev = bus->devices; dev; dev = dev->next) {
            // 설정한 Vendor ID와 Product ID를 가지는 장치를 찾기
            if (vid > 0 && dev->descriptor.idVendor != vid) continue;
            if (pid > 0 && dev->descriptor.idProduct != pid) continue;
            if (!dev->config) continue;
            if (dev->config->bNumInterfaces < 1) continue;
            printf("Device: vid = %04X, pid = %04X, with %d interfaces\n",
                dev->descriptor.idVendor,
                dev->descriptor.idProduct,
                dev->config->bNumInterfaces);
            struct usb_interface *iface = dev->config->interface;
            usb_dev_handle *u = NULL;
            int claimed = 0;
            // 현재 기기의 모든 인터페이스(interface)를 확인하며
            for (int i = 0; i < dev->config->bNumInterfaces && iface; i++, iface++) {
                struct usb_interface_descriptor *desc = iface->altsetting;
                if (!desc) continue;
                printf("[ Interface Number = %d ]\n", desc->bInterfaceNumber);
                printf("Class = %d, Subclass = %d, Protocol = %d\n",
                       desc->bInterfaceClass,
                       desc->bInterfaceSubClass,
                       desc->bInterfaceProtocol);

                struct usb_endpoint_descriptor *ep = desc->endpoint;
                int ep_in = 0, ep_out = 0;
                // 해당 인터페이스의 모든 엔드포인트(endpoint)를 확인하며
                for (int n = 0; n < desc->bNumEndpoints; n++, ep++) {
                    if (ep->bEndpointAddress & 0x80) {
                        if (!ep_in) ep_in = ep->bEndpointAddress & 0x7F;
                        printf("IN endpoint number: %d\n", ep_in);
                    } else {
                        if (!ep_out) ep_out = ep->bEndpointAddress;
                        printf("OUT endpoint number: %d\n", ep_out);
                    }
                }
                if (!ep_in) continue;
                
                // 리눅스 usb.h 헤더에 구현되어 있는 usb_open() 함수 사용 
                if (!u) {
                    u = usb_open(dev);
                    if (!u) {
                        printf("unable to open device\n");
                        break;
                    }
                }
                claimed++;
                
                // 바로 여기에서 usb_bulk_read() 메서드를 이용해 데이터 송신 (\n 기호 필요)
                char buf[8];
                if (desc->bInterfaceNumber == targetInterfaceNumber) {
                    int length = usb_bulk_read(u, ep_out, buf, sizeof(buf), -1);
                    printf("Bulk read length = %d\n", length);
                    if (length <= 0) continue;
                    printf("받은 데이터: %s\n", buf);
                }
            }
            if (u && !claimed) usb_close(u);
        }
    }
    return 0;
}
