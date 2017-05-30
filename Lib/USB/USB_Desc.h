#ifndef _USB_DESC_H_
#define _USB_DESC_H_

/* Includes */
#include "FslTypes.h"




//标准的USB设备有5种USB描述符：设备描述符，配置描述符，字符串描述符，接口描述符，端点描述符． 一个设备只有一个设备描述符


/*
 * Device and/or Interface Class codes
 */
//设备类码的典型值如下:
//参考 http://www.usb.org/developers/defined_class
#define USB_CLASS_PER_INTERFACE     0
#define USB_CLAS_AUDIO              1       // 声音设备
#define USB_CLASS_COMM              2       // 调制解调器，网卡，ISDN连接
#define USB_CLASS_HID               3       // HID设备，如鼠标，键盘
#define USB_CLASS_PHYSICAL          5       // 物理设备
#define USB_CLASS_STILL_IMAGE       6       // 静止图像捕捉设备
#define USB_CLASS_PRINTER           7       // 打印机
#define USB_CLASS_MASS_STORAGE      8       // 批量存储设备
#define USB_CLASS_HUB               9       // USB HUBS
#define USB_CLASS_CSCID             0x0B    // 智能卡
#define USB_CLASS_VIDEO             0X0E    // 视频设备，如网络摄像头
#define USB_CLASS_VENDOR_SPEC       0xFF    // 厂商自定义的设备

/*
 * Descriptor types
 */
#define USB_DT_DEVICE           		0x01
#define USB_DT_CONFIG          	 		0x02
#define USB_DT_STRING           		0x03
#define USB_DT_INTERFACE        		0x04
#define USB_DT_ENDPOINT         		0x05
#define USB_DT_DEVICE_QUALIFIER         0x06
#define USB_DT_OTHER_SPEED_CONFIG       0x07
#define USB_DT_INTERFACE_POWER          0x08
#define USB_DT_OTG                      0x09
#define USB_DT_DEBUG                    0x10
#define USB_DT_INTERFACE_ASSOCIATION    0x11

#define USB_DT_HID              		0x21
#define USB_DT_REPORT           		0x22
#define USB_DT_PHYSICAL         		0x23
#define USB_DT_CLASS_SPECIFIC_INTERFACE 0x24
#define USB_DT_CLASS_SPECIFIC_ENDPOINT  0x25

#define USB_DT_HUB             	 		0x29

//设备描述符
//The USB_DEVICE_DESCRIPTOR structure is used by USB client drivers to retrieve a USB-defined device descriptor.
//http://technet.microsoft.com/zh-cn/library/ff539280
typedef struct _USB_DEVICE_DESCRIPTOR
{
    uint8  bLength;                   //指定的长度，固定为0x12
    uint8  bDescriptorType;           //设备描述符类型．固定为0x01( USB_DEVICE_DESCRIPTOR_TYPE )
    uint16 bcdUSB;                    //USB 规范发布号．表示了本设备能适用于那种协议，如2.0=0x0200，1.1=0x0110等．
    uint8  bDeviceClass;              //类型代码（由USB指定）。当它的值是0时，表示所有接口在配置描述符里，并且所有接口是独立的。当它的值是1到0xFE时，表示不同的接口关联的。当它的值是0xFF时，它是厂商自己定义的．
    uint8  bDeviceSubClass;           //子类型代码（由USB分配）．如果bDeviceClass值是0，一定要设置为0．其它情况就根据USB-IF组织定义的编码．
    uint8  bDeviceProtocol;           //协议代码（由USB分配）．如果使用USB-IF组织定义的协议，就需要设置这里的值，否则直接设置为0。如果厂商自己定义的可以设置为0xFF．
    uint8  bMaxPacketSize0;           //端点0 最大分组大小（只有8,16,32,64有效）．
    uint16 idVendor;                  //供应商ID（由USB分配）
    uint16 idProduct;                 //产品ID（由厂商分配）．由供应商ID和产品ID，就可以让操作系统加载不同的驱动程序．
    uint16 bcdDevice;                 //设备出产编码．由厂家自行设置
    uint8  iManufacturer;             //厂商描述符字符串索引．索引到对应的字符串描述符． 为0则表示没有．
    uint8  iProduct;                  //产品描述符字符串索引．同上
    uint8  iSerialNumber;             //设备序列号字符串索引．同上
    uint8  bNumConfigurations;        //所支持的配置字符符串索引
} USB_DEVICE_DESCRIPTOR, *PUSB_DEVICE_DESCRIPTOR;


const USB_DEVICE_DESCRIPTOR Device_Descriptor =
{
    0x12,               //blength
    0x01,               //bDescriptor
    0x0200,             //bcdUSB ver R=2.00
    USB_CLASS_COMM,     //bDeviceClass = CDC类
    0x00,               //bDeviceSubClass
    0x00,               //bDeviceProtocol
    MaxPacketSize,      //bMaxPacketSize0   数据包长度 MaxPacketSize 字节
    0x15A2,             //idVendor - 0x15A2(freescale Vendor ID)
    0xA50F,             //idProduct
    0x0000,             //bcdDevice - Version 1.00
    0x01,               //iManufacturer - Index to string Manufacturer descriptor
    0x02,               //iProduct  - Index to string product descriptor
    0x03,               //iSerialNumber - Index to string serial number
    0x01                //bNumConfigurations - # of config. at current speed,
};


/********************************************************************
*       String 0
********************************************************************/
const uint8 String_Descriptor0[4] =
{
    0x04,		   //bLength;
    0x03,		   //bDescriptorType - STRING descriptor
    0x09, 0x04	  //wLANDID0 - English (American)
};

/********************************************************************
*       String 1
********************************************************************/
const uint8 String_Descriptor1[] =
{
    18,             //bLength; 18bytes
    USB_DT_STRING,  //bDescriptorType - STRING descriptor
    'w', 0x00,      //"wildfire"
    'i', 0x00,
    'l', 0x00,
    'd', 0x00,
    'f', 0x00,
    'i', 0x00,
    'r', 0x00,
    'e', 0x00,
};

/********************************************************************
*       String 2
********************************************************************/
const uint8 String_Descriptor2[] =
{
    18,                     //bLength; 18bytes
    USB_DT_STRING,          //bDescriptorType - STRING descriptor
    'U', 0x00,
    'S', 0x00,
    'B', 0x00,
    '-', 0x00,
    'U', 0x00,
    'A', 0x00,
    'R', 0x00,
    'T', 0x00
};
/********************************************************************
*       String 3
********************************************************************/
const uint8 String_Descriptor3[] =
{
    18,                     //bLength; 18bytes
    USB_DT_STRING,          //bDescriptorType - STRING descriptor
    'T', 0x00,
    'E', 0x00,
    'S', 0x00,
    'T', 0x00,
    '_', 0x00,
    '1', 0x00,
    '.', 0x00,
    '1', 0x00
};


/********************************************************************
*       Configuration Descriptor
********************************************************************/
const uint8 Configuration_Descriptor[0x43] =
{
    // device configuration
    //参考资料：http://technet.microsoft.com/zh-cn/subscriptions/aa912633
    0x09,           //blength
    USB_DT_CONFIG,  //bDescriptor
    0x43, 0x00,     //wTotalLength - # of bytes including interface and endpoint descpt.
    0x02,           //bNumInterfaces - at least 1 data interface
    0x01,           //bConfigurationValue -
    0x00,           //iConfiguration - index to string descriptor
    0xC0,           //bmAttributes -                bit 7- Compatibility with USB 1.0
    //								bit 6 if 1 self powered else Bus powered
    //								bit 5-remote wakeup
    //								bit 4-0-reserved
    50,           	//bMaxPower - 最大电流 = bMaxPower *2 = 100mA

    /********************************************************************
    *       Interface Descriptor
    ********************************************************************/
    0x09,               //blength - 描述符的长度：0x09
    USB_DT_INTERFACE, 	//bDescriptorType - 描述符的类型：USB_DT_INTERFACE
    0x00,           	//bInterfaceNumber - Zero based value identifying the index of the config.
    0x00,           	//bAlternateSetting;
    0x01,           	//bNumEndpoints - 端点 0 以外的端点数 :1
    USB_CLASS_COMM,     //bInterfaceClass - 类代码: USB_CLASS_COMM
    0x02,           	//bInterfaceSubClass  子类代码:
    0x01,           	//bInterfaceProtocol  协议代码:
    0x01,           	//iInterface - 字符串描述符的索引值

    // Header Functional Descriptor (marks beginning of the concatenated set of Functional Descriptors)
    0x05,               // bFunctionLength
    USB_DT_CLASS_SPECIFIC_INTERFACE,               // bDescriptorType: CS_INTERFACE
    0x00,               // bDescriptorSubtype: Header Func Desc
    0x10,               // bmCapabilities: D0+D1
    0x01,               // bDataInterface: 1

    0x05,
    USB_DT_CLASS_SPECIFIC_INTERFACE,
    0x01,				// bDescriptorSubtype: Call Management Func Desc
    0x00,
    0x01,

    0x04,
    USB_DT_CLASS_SPECIFIC_INTERFACE,
    0x02,				// bDescriptorSubtype: Abstract Control Management desc
    0x00,

    0x05,
    USB_DT_CLASS_SPECIFIC_INTERFACE,
    0x06,				// bDescriptorSubtype: Union func desc
    0x00,
    0x01,
    /********************************************************************
    *       Endpoint  Descriptor
    ********************************************************************/
    0x07,           //blength
    USB_DT_ENDPOINT,//bDescriptorType - EndPoint
    0x81,           //bEndpointAddress
    0x03,           //bmAttributes
    MaxPacketSize, 0x00,     //wMaxPacketSize
    0x02,           //bInterval
    /********************************************************************
    *       Interface Descriptor
    ********************************************************************/
    0x09,           //blength
    USB_DT_INTERFACE,//bDescriptorType - Interface descriptor
    0x01,           //bInterfaceNumber - Zero based value identifying the index of the config.
    0x00,           //bAlternateSetting;
    0x02,           //bNumEndpoints - 2 endpoints
    0x0A,           //bInterfaceClass - mass storage
    0x00,           //bInterfaceSubClass - SCSI Transparent command Set
    0x00,           //bInterfaceProtocol - Bulk-Only transport
    0x01,           //iInterface - Index to String descriptor
    /********************************************************************
    *       Endpoint OUT Descriptor
    ********************************************************************/
    0x07,           //blength
    USB_DT_ENDPOINT,//bDescriptorType - EndPoint
    0x82,           //bEndpointAddress
    0x02,           //bmAttributes
    MaxPacketSize, 0x00,     //wMaxPacketSize
    0x00,           //bInterval
    /********************************************************************
    *       Endpoint IN Descriptor
    ********************************************************************/
    0x07,           //blength
    USB_DT_ENDPOINT,//bDescriptorType - EndPoint
    0x03,           //bEndpointAddress
    0x02,           //bmAttributes
    MaxPacketSize, 0x00,     //wMaxPacketSize
    0x00,           //bInterval
};


#endif /* _USB_DESC_H_ */