#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <usb.h>

void transfer_bulk_data()
{
    int vid = 0x1D6B;
    int pid = 0x0104;

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
            printf("device: vid=%04X, pic=%04X, with %d iface\n",
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
                printf("interface #%d: class = %d, subclass = %d, protocol = %d\n",
                    i,
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
                        printf("OUT endpoint: %d\n", ep_out);
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
            }
            if (u && !claimed) usb_close(u);
        }
    }
}

int main(void) {
    return 0;
}
