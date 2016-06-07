
// Copyright (c) 2010, Peter Barrett 
/*
** Permission to use, copy, modify, and/or distribute this software for  
** any purpose with or without fee is hereby granted, provided that the  
** above copyright notice and this permission notice appear in all copies.  
**  
** THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL  
** WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED  
** WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR  
** BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES  
** OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,  
** WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,  
** ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS  
** SOFTWARE.  
*/

#ifndef __USBCORE_H__
#define __USBCORE_H__

#include "USBAPI.h"

//	Standard requests
#define GET_STATUS			0
#define CLEAR_FEATURE		1
#define SET_FEATURE			3
#define SET_ADDRESS			5
#define GET_DESCRIPTOR		6
#define SET_DESCRIPTOR		7
#define GET_CONFIGURATION	8
#define SET_CONFIGURATION	9
#define GET_INTERFACE		10
#define SET_INTERFACE		11


// bmRequestType
#define REQUEST_HOSTTODEVICE	0x00
#define REQUEST_DEVICETOHOST	0x80
#define REQUEST_DIRECTION		0x80

#define REQUEST_STANDARD		0x00
#define REQUEST_CLASS			0x20
#define REQUEST_VENDOR			0x40
#define REQUEST_TYPE			0x60

#define REQUEST_DEVICE			0x00
#define REQUEST_INTERFACE		0x01
#define REQUEST_ENDPOINT		0x02
#define REQUEST_OTHER			0x03
#define REQUEST_RECIPIENT		0x03

#define REQUEST_DEVICETOHOST_CLASS_INTERFACE    (REQUEST_DEVICETOHOST | REQUEST_CLASS | REQUEST_INTERFACE)
#define REQUEST_HOSTTODEVICE_CLASS_INTERFACE    (REQUEST_HOSTTODEVICE | REQUEST_CLASS | REQUEST_INTERFACE)
#define REQUEST_DEVICETOHOST_STANDARD_INTERFACE (REQUEST_DEVICETOHOST | REQUEST_STANDARD | REQUEST_INTERFACE)

//	Class requests

#define CDC_SET_LINE_CODING			0x20
#define CDC_GET_LINE_CODING			0x21
#define CDC_SET_CONTROL_LINE_STATE	0x22
#define CDC_SEND_BREAK				0x23

#define MSC_RESET					0xFF
#define MSC_GET_MAX_LUN				0xFE

//	Descriptors

#define USB_DEVICE_DESC_SIZE 18
#define USB_CONFIGUARTION_DESC_SIZE 9
#define USB_INTERFACE_DESC_SIZE 9
#define USB_ENDPOINT_DESC_SIZE 7

#define USB_DEVICE_DESCRIPTOR_TYPE             1
#define USB_CONFIGURATION_DESCRIPTOR_TYPE      2
#define USB_STRING_DESCRIPTOR_TYPE             3
#define USB_INTERFACE_DESCRIPTOR_TYPE          4
#define USB_ENDPOINT_DESCRIPTOR_TYPE           5
#define USB_DEVICE_QUALIFIER                   6
#define USB_OTHER_SPEED_CONFIGURATION          7

// usb_20.pdf Table 9.6 Standard Feature Selectors
#define DEVICE_REMOTE_WAKEUP                   1
#define ENDPOINT_HALT                          2
#define TEST_MODE                              3

// usb_20.pdf Figure 9-4. Information Returned by a GetStatus() Request to a Device
#define FEATURE_SELFPOWERED_ENABLED     (1 << 0)
#define FEATURE_REMOTE_WAKEUP_ENABLED   (1 << 1)

#define USB_DEVICE_CLASS_COMMUNICATIONS        0x02
#define USB_DEVICE_CLASS_HUMAN_INTERFACE       0x03
#define USB_DEVICE_CLASS_STORAGE               0x08
#define USB_DEVICE_CLASS_VENDOR_SPECIFIC       0xFF

#define USB_CONFIG_POWERED_MASK                0x40
#define USB_CONFIG_BUS_POWERED                 0x80
#define USB_CONFIG_SELF_POWERED                0xC0
#define USB_CONFIG_REMOTE_WAKEUP               0x20

// bMaxPower in Configuration Descriptor
#define USB_CONFIG_POWER_MA(mA)                ((mA)/2)

// bEndpointAddress in Endpoint Descriptor
#define USB_ENDPOINT_DIRECTION_MASK            0x80
#define USB_ENDPOINT_OUT(addr)                 (lowByte((addr) | 0x00))
#define USB_ENDPOINT_IN(addr)                  (lowByte((addr) | 0x80))

#define USB_ENDPOINT_TYPE_MASK                 0x03
#define USB_ENDPOINT_TYPE_CONTROL              0x00
#define USB_ENDPOINT_TYPE_ISOCHRONOUS          0x01
#define USB_ENDPOINT_TYPE_BULK                 0x02
#define USB_ENDPOINT_TYPE_INTERRUPT            0x03

#define TOBYTES(x) ((x) & 0xFF),(((x) >> 8) & 0xFF)

#define CDC_V1_10                               0x0110
#define CDC_COMMUNICATION_INTERFACE_CLASS       0x02

#define CDC_CALL_MANAGEMENT                     0x01
#define CDC_ABSTRACT_CONTROL_MODEL              0x02
#define CDC_HEADER                              0x00
#define CDC_ABSTRACT_CONTROL_MANAGEMENT         0x02
#define CDC_UNION                               0x06
#define CDC_CS_INTERFACE                        0x24
#define CDC_CS_ENDPOINT                         0x25
#define CDC_DATA_INTERFACE_CLASS                0x0A

#define MSC_SUBCLASS_SCSI						0x06 
#define MSC_PROTOCOL_BULK_ONLY					0x50 

//	Device
typedef struct {
	u8 len;				// 18
	u8 dtype;			// 1 USB_DEVICE_DESCRIPTOR_TYPE
	u16 usbVersion;		// 0x200
	u8	deviceClass;
	u8	deviceSubClass;
	u8	deviceProtocol;
	u8	packetSize0;	// Packet 0
	u16	idVendor;
	u16	idProduct;
	u16	deviceVersion;	// 0x100
	u8	iManufacturer;
	u8	iProduct;
	u8	iSerialNumber;
	u8	bNumConfigurations;
} __attribute__((packed)) DeviceDescriptor;

//	Config
typedef struct {
	u8	len;			// 9
	u8	dtype;			// 2
	u16 clen;			// total length
	u8	numInterfaces;
	u8	config;
	u8	iconfig;
	u8	attributes;
	u8	maxPower;
} __attribute__((packed)) ConfigDescriptor;

//	String

//	Interface
typedef struct
{
	u8 len;		// 9
	u8 dtype;	// 4
	u8 number;
	u8 alternate;
	u8 numEndpoints;
	u8 interfaceClass;
	u8 interfaceSubClass;
	u8 protocol;
	u8 iInterface;
} __attribute__((packed)) InterfaceDescriptor;

//	Endpoint
typedef struct
{
	u8 len;		// 7
	u8 dtype;	// 5
	u8 addr;
	u8 attr;
	u16 packetSize;
	u8 interval;
} __attribute__((packed)) EndpointDescriptor;

// Interface Association Descriptor
// Used to bind 2 interfaces together in CDC compostite device
typedef struct
{
	u8 len;				// 8
	u8 dtype;			// 11
	u8 firstInterface;
	u8 interfaceCount;
	u8 functionClass;
	u8 funtionSubClass;
	u8 functionProtocol;
	u8 iInterface;
} __attribute__((packed)) IADDescriptor;

//	CDC CS interface descriptor
typedef struct
{
	u8 len;		// 5
	u8 dtype;	// 0x24
	u8 subtype;
	u8 d0;
	u8 d1;
} __attribute__((packed)) CDCCSInterfaceDescriptor;

typedef struct
{
	u8 len;		// 4
	u8 dtype;	// 0x24
	u8 subtype;
	u8 d0;
} __attribute__((packed)) CDCCSInterfaceDescriptor4;

//  Device Qualifier (only needed for USB2.0 devices)
typedef struct {
  uint8_t bLength;
  uint8_t dtype;
  uint16_t bDescriptorType;
  uint8_t bDeviceClass;
  uint8_t bDeviceSubClass;
  uint8_t bDeviceProtocol;
  uint8_t bMaxPacketSize0;
  uint8_t bNumConfigurations;
} __attribute__((packed)) QualifierDescriptor;

typedef struct 
{
    u8	len;
    u8 	dtype;		// 0x24
    u8 	subtype;	// 1
    u8 	bmCapabilities;
    u8 	bDataInterface;
} __attribute__((packed)) CMFunctionalDescriptor;
	
typedef struct 
{
    u8	len;
    u8 	dtype;		// 0x24
    u8 	subtype;	// 1
    u8 	bmCapabilities;
} __attribute__((packed)) ACMFunctionalDescriptor;

typedef struct 
{
	//	IAD
	IADDescriptor				iad;	// Only needed on compound device

	//	Control
	InterfaceDescriptor			cif;	// 
	CDCCSInterfaceDescriptor	header;
	CMFunctionalDescriptor		callManagement;			// Call Management
	ACMFunctionalDescriptor		controlManagement;		// ACM
	CDCCSInterfaceDescriptor	functionalDescriptor;	// CDC_UNION
	EndpointDescriptor			cifin;

	//	Data
	InterfaceDescriptor			dif;
	EndpointDescriptor			in;
	EndpointDescriptor			out;
} __attribute__((packed)) CDCDescriptor;

typedef struct 
{
	InterfaceDescriptor			msc;
	EndpointDescriptor			in;
	EndpointDescriptor			out;
} __attribute__((packed)) MSCDescriptor;


#define D_DEVICE(_class,_subClass,_proto,_packetSize0,_vid,_pid,_version,_im,_ip,_is,_configs) \
	{ 18, 1, 0x200, _class,_subClass,_proto,_packetSize0,_vid,_pid,_version,_im,_ip,_is,_configs }

#define D_CONFIG(_totalLength,_interfaces) \
	{ 9, 2, _totalLength,_interfaces, 1, 0, USB_CONFIG_BUS_POWERED | USB_CONFIG_REMOTE_WAKEUP, USB_CONFIG_POWER_MA(100) }

#define D_INTERFACE(_n,_numEndpoints,_class,_subClass,_protocol) \
	{ 9, 4, _n, 0, _numEndpoints, _class,_subClass, _protocol, 0 }

#define D_ENDPOINT(_addr,_attr,_packetSize, _interval) \
	{ 7, 5, _addr,_attr,_packetSize, _interval }

#define D_QUALIFIER(_class,_subClass,_proto,_packetSize0,_configs) \
    { 10, 6, 0x200, _class,_subClass,_proto,_packetSize0,_configs }

#define D_IAD(_firstInterface, _count, _class, _subClass, _protocol) \
	{ 8, 11, _firstInterface, _count, _class, _subClass, _protocol, 0 }

#define D_CDCCS(_subtype,_d0,_d1)	{ 5, 0x24, _subtype, _d0, _d1 }
#define D_CDCCS4(_subtype,_d0)		{ 4, 0x24, _subtype, _d0 }

extern "C" {
  struct USBPHY;
  struct USBMAC;
  struct UBSLink;

  int usbStart(void);
  void usbAttach(struct USBPHY *phy);
  void usbDetach(struct USBPHY *phy);
  int usbSetup(struct USBPHY *phy, struct USBMAC *mac, struct USBLink *link);
  int usbSend(struct USBMAC *mac, int epnum, const void *data, int count);
  int usbReceive(struct USBMAC *mac, void (*callback)(const void *data, int count));
};

#endif
