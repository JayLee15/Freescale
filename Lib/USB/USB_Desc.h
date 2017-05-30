#ifndef _USB_DESC_H_
#define _USB_DESC_H_

/* Includes */
#include "FslTypes.h"




//��׼��USB�豸��5��USB���������豸���������������������ַ������������ӿ����������˵��������� һ���豸ֻ��һ���豸������


/*
 * Device and/or Interface Class codes
 */
//�豸����ĵ���ֵ����:
//�ο� http://www.usb.org/developers/defined_class
#define USB_CLASS_PER_INTERFACE     0
#define USB_CLAS_AUDIO              1       // �����豸
#define USB_CLASS_COMM              2       // ���ƽ������������ISDN����
#define USB_CLASS_HID               3       // HID�豸������꣬����
#define USB_CLASS_PHYSICAL          5       // �����豸
#define USB_CLASS_STILL_IMAGE       6       // ��ֹͼ��׽�豸
#define USB_CLASS_PRINTER           7       // ��ӡ��
#define USB_CLASS_MASS_STORAGE      8       // �����洢�豸
#define USB_CLASS_HUB               9       // USB HUBS
#define USB_CLASS_CSCID             0x0B    // ���ܿ�
#define USB_CLASS_VIDEO             0X0E    // ��Ƶ�豸������������ͷ
#define USB_CLASS_VENDOR_SPEC       0xFF    // �����Զ�����豸

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

//�豸������
//The USB_DEVICE_DESCRIPTOR structure is used by USB client drivers to retrieve a USB-defined device descriptor.
//http://technet.microsoft.com/zh-cn/library/ff539280
typedef struct _USB_DEVICE_DESCRIPTOR
{
    uint8  bLength;                   //ָ���ĳ��ȣ��̶�Ϊ0x12
    uint8  bDescriptorType;           //�豸���������ͣ��̶�Ϊ0x01( USB_DEVICE_DESCRIPTOR_TYPE )
    uint16 bcdUSB;                    //USB �淶�����ţ���ʾ�˱��豸������������Э�飬��2.0=0x0200��1.1=0x0110�ȣ�
    uint8  bDeviceClass;              //���ʹ��루��USBָ������������ֵ��0ʱ����ʾ���нӿ���������������������нӿ��Ƕ����ġ�������ֵ��1��0xFEʱ����ʾ��ͬ�Ľӿڹ����ġ�������ֵ��0xFFʱ�����ǳ����Լ�����ģ�
    uint8  bDeviceSubClass;           //�����ʹ��루��USB���䣩�����bDeviceClassֵ��0��һ��Ҫ����Ϊ0����������͸���USB-IF��֯����ı��룮
    uint8  bDeviceProtocol;           //Э����루��USB���䣩�����ʹ��USB-IF��֯�����Э�飬����Ҫ���������ֵ������ֱ������Ϊ0����������Լ�����Ŀ�������Ϊ0xFF��
    uint8  bMaxPacketSize0;           //�˵�0 �������С��ֻ��8,16,32,64��Ч����
    uint16 idVendor;                  //��Ӧ��ID����USB���䣩
    uint16 idProduct;                 //��ƷID���ɳ��̷��䣩���ɹ�Ӧ��ID�Ͳ�ƷID���Ϳ����ò���ϵͳ���ز�ͬ����������
    uint16 bcdDevice;                 //�豸�������룮�ɳ�����������
    uint8  iManufacturer;             //�����������ַ�����������������Ӧ���ַ����������� Ϊ0���ʾû�У�
    uint8  iProduct;                  //��Ʒ�������ַ���������ͬ��
    uint8  iSerialNumber;             //�豸���к��ַ���������ͬ��
    uint8  bNumConfigurations;        //��֧�ֵ������ַ���������
} USB_DEVICE_DESCRIPTOR, *PUSB_DEVICE_DESCRIPTOR;


const USB_DEVICE_DESCRIPTOR Device_Descriptor =
{
    0x12,               //blength
    0x01,               //bDescriptor
    0x0200,             //bcdUSB ver R=2.00
    USB_CLASS_COMM,     //bDeviceClass = CDC��
    0x00,               //bDeviceSubClass
    0x00,               //bDeviceProtocol
    MaxPacketSize,      //bMaxPacketSize0   ���ݰ����� MaxPacketSize �ֽ�
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
    //�ο����ϣ�http://technet.microsoft.com/zh-cn/subscriptions/aa912633
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
    50,           	//bMaxPower - ������ = bMaxPower *2 = 100mA

    /********************************************************************
    *       Interface Descriptor
    ********************************************************************/
    0x09,               //blength - �������ĳ��ȣ�0x09
    USB_DT_INTERFACE, 	//bDescriptorType - �����������ͣ�USB_DT_INTERFACE
    0x00,           	//bInterfaceNumber - Zero based value identifying the index of the config.
    0x00,           	//bAlternateSetting;
    0x01,           	//bNumEndpoints - �˵� 0 ����Ķ˵��� :1
    USB_CLASS_COMM,     //bInterfaceClass - �����: USB_CLASS_COMM
    0x02,           	//bInterfaceSubClass  �������:
    0x01,           	//bInterfaceProtocol  Э�����:
    0x01,           	//iInterface - �ַ���������������ֵ

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