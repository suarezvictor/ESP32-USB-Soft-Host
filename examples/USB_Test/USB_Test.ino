#include <ESP32-USBSoftHost.hpp>
#include "usbkbd.h" // KeyboardReportParser

#if defined ARDUINO_LOLIN_D32_PRO
  // [KO] 15/13 : unassigned pins seem unresponsive
  // [OK] 22/23 : pins for MOSI/SCK work just fine
  #define PROFILE_NAME "LoLin D32 Pro"
  #define DP_P0  22  // always enabled
  #define DM_P0  21  // always enabled
#elif defined ARDUINO_M5STACK_Core2
  // [OK] 33/32 are the GROVE port pins for I2C, but there are other devices interferring on the bus
  #define PROFILE_NAME "M5Stack Core2"
  #define DP_P0  33  // always enabled
  #define DM_P0  32  // always enabled
#elif defined ARDUINO_M5STACK_FIRE
  // [KO] 16/17 : GROVE port pins for RX2/TX2 but seem unresponsive
  // [KO] 21/22 : GROVE port pins for SDA/SCL, but there are other devices interferring on the bus
  // [KO] 26/36 : GROVE port pins for I/O but seem unresponsive
  #define PROFILE_NAME "M5Stack Fire"
  #define DP_P0  16  // always enabled
  #define DM_P0  17  // always enabled
#elif defined ARDUINO_M5Stack_Core_ESP32
  // [OK] 16/17 : M5Bottom pins for RX2/TX2 work just fine
  #define PROFILE_NAME "M5Stack Gray"
  #define DP_P0  16  // always enabled
  #define DM_P0  17  // always enabled
#elif CONFIG_IDF_TARGET_ESP32C3 || defined ESP32C3
  #define PROFILE_NAME "ESP32 C3 Dev module"
  #define DP_P0   6
  #define DM_P0   8
#else
  // default pins tested on ESP32-Wroom
  #define PROFILE_NAME "Default Wroom"
  #define DP_P0  12  // always enabled
  #define DM_P0  14  // always enabled
#endif

#ifndef DP_P1
  #define DP_P1  -1
  #define DM_P1  -1
#endif
#ifndef DP_P2
  #define DP_P2  -1
  #define DM_P2  -1
#endif
#ifndef DP_P3
  #define DP_P3  -1
  #define DM_P3  -1
#endif



static void my_USB_DetectCB( uint8_t usbNum, void * dev )
{
  sDevDesc *device = (sDevDesc*)dev;
  printf("New device detected on USB#%d\n", usbNum);
  printf("desc.bcdUSB             = 0x%04x\n", device->bcdUSB);
  printf("desc.bDeviceClass       = 0x%02x\n", device->bDeviceClass);
  printf("desc.bDeviceSubClass    = 0x%02x\n", device->bDeviceSubClass);
  printf("desc.bDeviceProtocol    = 0x%02x\n", device->bDeviceProtocol);
  printf("desc.bMaxPacketSize0    = 0x%02x\n", device->bMaxPacketSize0);
  printf("desc.idVendor           = 0x%04x\n", device->idVendor);
  printf("desc.idProduct          = 0x%04x\n", device->idProduct);
  printf("desc.bcdDevice          = 0x%04x\n", device->bcdDevice);
  printf("desc.iManufacturer      = 0x%02x\n", device->iManufacturer);
  printf("desc.iProduct           = 0x%02x\n", device->iProduct);
  printf("desc.iSerialNumber      = 0x%02x\n", device->iSerialNumber);
  printf("desc.bNumConfigurations = 0x%02x\n", device->bNumConfigurations);
  // if( device->iProduct == mySupportedIdProduct && device->iManufacturer == mySupportedManufacturer ) {
  //   myListenUSBPort = usbNum;
  // }
}


static void my_USB_PrintCB(uint8_t usbNum, uint8_t byte_depth, uint8_t* data, uint8_t data_len)
{
  // if( myListenUSBPort != usbNum ) return;
  printf("in: ");
  for(int k=0;k<data_len;k++) {
    printf("0x%02x ", data[k] );
  }
  printf("\n");
}

usb_pins_config_t USB_Pins_Config =
{
  DP_P0, DM_P0,
  DP_P1, DM_P1,
  DP_P2, DM_P2,
  DP_P3, DM_P3
};


void setup()
{

  Serial.begin(115200);
  delay(200);
  Serial.printf("USB Soft Host Test for %s\n", PROFILE_NAME );
  delay(1000);

  USH.init( USB_Pins_Config, my_USB_DetectCB, my_USB_PrintCB );

}

void loop()
{
#ifdef TIMER_INTERVAL0_SEC
  yield();
#else
  static int t = -1;
  int tnow = millis();
  if(tnow != t)
  {
    t = tnow;
    usb_process();
  }
#endif
}



