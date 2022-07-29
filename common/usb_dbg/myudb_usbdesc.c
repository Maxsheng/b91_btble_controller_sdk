/********************************************************************************************************
 * @file     myudb_usbdesc.c
 *
 * @brief    This is the source file for BTBLE SDK
 *
 * @author	 BTBLE GROUP
 * @date         2,2022
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *******************************************************************************************************/

#if(1)

#include "../../drivers.h"
#include "myudb.h"
#include "myudb_usbdesc.h"

#ifdef		MYUDB_DISABLE
// request parameters
/** Language descriptor structure. This descriptor, located in FLASH memory, is returned when the host requests
 *  the string descriptor with index 0 (the first index). It is actually an array of 16-bit integers, which indicate
 *  via the language ID table available at USB.org what languages the device supports for its string descriptors.
 */
static const HCI_USB_Descriptor_String_t hci_language_desc = { {
		sizeof(HCI_USB_Descriptor_Header_t) + 2, DTYPE_String },
		{ LANGUAGE_ID_ENG } };

/** Manufacturer descriptor string. This is a Unicode string containing the manufacturer's details in human readable
 *  form, and is read out upon request by the host when the appropriate string ID is requested, listed in the Device
 *  Descriptor.
 */
static const HCI_USB_Descriptor_String_t hci_vendor_desc = { { sizeof(HCI_USB_Descriptor_Header_t)
		+ sizeof(HCI_USB_STRING_VENDOR) - 2, DTYPE_String }, // Header
		HCI_USB_STRING_VENDOR };

/** Product descriptor string. This is a Unicode string containing the product's details in human readable form,
 *  and is read out upon request by the host when the appropriate string ID is requested, listed in the Device
 *  Descriptor.
 */
static const HCI_USB_Descriptor_String_t hci_product_desc = { {
		sizeof(HCI_USB_Descriptor_Header_t) + sizeof(HCI_USB_STRING_PRODUCT) - 2,
		DTYPE_String }, // Header
		HCI_USB_STRING_PRODUCT };

/** Serial number string. This is a Unicode string containing the device's unique serial number, expressed as a
 *  series of uppercase hexadecimal digits.
 */
static const HCI_USB_Descriptor_String_t hci_serial_desc = { { sizeof(HCI_USB_Descriptor_Header_t)
		+ sizeof(HCI_USB_STRING_SERIAL) - 2, DTYPE_String }, // Header
		HCI_USB_STRING_SERIAL };


static const HCI_USB_Descriptor_Device_t hci_device_desc = { {
		sizeof(HCI_USB_Descriptor_Device_t), DTYPE_Device }, // Header
		0x0200, // USBSpecification, USB 2.0
		0xE0, // Class -- Wireless Controller
		0x01, // SubClass -- RF controller
		0x01, // Protocol -- Bluetooth Primary Controller
		8, // Endpoint0Size, Maximum Packet Size for Zero Endpoint. Valid Sizes are 8, 16, 32, 64
		0x0a12, // VendorID
		0x0001, // ProductID
		0x8891/*0x0100*/, // .ReleaseNumber
//		HCI_USB_ID_VERSION,
		0x00,
		HCI_USB_STRING_PRODUCT_ID, // .ProductStrIndex
		0, 	// .SerialNumStrIndex, iSerialNumber
		1 };

static const HCI_USB_Descriptor_Configuration_t
		hci_configuration_desc = {
				{
					{sizeof(HCI_USB_Descriptor_Configuration_Header_t),
					DTYPE_Configuration }, // Length, type
					sizeof(HCI_USB_Descriptor_Configuration_t), // TotalLength: variable
					2, // NumInterfaces
					1, // Configuration index
					NO_DESCRIPTOR, // Configuration String
					0xE0,		 // Attributes
					//USB_CONFIG_ATTR_RESERVED, // Attributes
					USB_CONFIG_POWER_MA(100) // MaxPower = 500mA
				},

				// primary interface0 HCI events and ACL data
				{ { sizeof(HCI_USB_Descriptor_Interface_t), DTYPE_Interface },
						0, 0, // AlternateSetting
						3, // bNumEndpoints
						0xE0, // bInterfaceclass ->wireless controller
						0x01, // bInterfaceSubClass -> RF controller
						0x01,// bInterfaceProtocol -> Bluetooth Primary controller
						NO_DESCRIPTOR // iInterface,  same as iProduct in USB_Descriptor_Device_t, or else not working
				},
				//interrupt_endpoint----HCI event
				{ { sizeof(HCI_USB_Descriptor_Endpoint_t), DTYPE_Endpoint }, // length, bDescriptorType
						ENDPOINT_DIR_IN | HCI_USB_EDP_IN_INTERRUPT, // endpoint_1 _IN
						EP_TYPE_INTERRUPT, // endpoint type
						0x0010, // wMaxPacketSize
						1 // polling bInterval,1ms
				},
				// bulk_out_endpoint----ACI data
				{ { sizeof(HCI_USB_Descriptor_Endpoint_t), DTYPE_Endpoint }, // length, bDescriptorType
						HCI_USB_EDP_OUT_BULK, // endpoint id
						EP_TYPE_BULK, // endpoint type
						0x0020, // wMaxPacketSize
						0x00 // polling bInterval. valid for iso or interrupt type
				},
				// bulk_input_endpoint ---- HCI data
				{ { sizeof(HCI_USB_Descriptor_Endpoint_t), DTYPE_Endpoint }, // length, bDescriptorType
						ENDPOINT_DIR_IN | HCI_USB_EDP_IN_BULK, // endpoint id
						EP_TYPE_BULK, // endpoint type
						0x0020, // wMaxPacketSize
						0x00 // polling bInterval. valid for iso or interrupt type
				},

				// primary interface1 HCI SCO data
				{ { sizeof(HCI_USB_Descriptor_Interface_t), DTYPE_Interface },
						1, 0, // AlternateSetting
						2, // bNumEndpoints
						0xE0, // bInterfaceclass ->wireless controller
						0x01, // bInterfaceSubClass -> RF controller
						0x01,// bInterfaceProtocol -> Bluetooth Primary controller
						NO_DESCRIPTOR // iInterface,  same as iProduct in USB_Descriptor_Device_t, or else not working
				},
				// iso_out_endpoint
				{ { sizeof(HCI_USB_Descriptor_Endpoint_t), DTYPE_Endpoint }, // length, bDescriptorType
						HCI_USB_EDP_OUT_ISO, // endpoint id
						EP_TYPE_ISOCHRONOUS, // endpoint type
						0x0000, // wMaxPacketSize
						0x01 // polling bInterval. valid for iso or interrupt type
				},
				// iso_in_endpoint
				{ { sizeof(HCI_USB_Descriptor_Endpoint_t), DTYPE_Endpoint }, // length, bDescriptorType
						ENDPOINT_DIR_IN | HCI_USB_EDP_IN_ISO, // endpoint id
						EP_TYPE_ISOCHRONOUS, // endpoint type
						0x0000, // wMaxPacketSize
						0x01 // bInterval
				},

				// primary interface1 alternate1
				{ { sizeof(HCI_USB_Descriptor_Interface_t), DTYPE_Interface },
						1, 1, // AlternateSetting
						2, // bNumEndpoints
						0xE0, // bInterfaceclass ->wireless controller
						0x01, // bInterfaceSubClass -> RF controller
						0x01,// bInterfaceProtocol -> Bluetooth Primary controller
						NO_DESCRIPTOR // iInterface,  same as iProduct in USB_Descriptor_Device_t, or else not working
				},
				// iso_out_endpoint
				{ { sizeof(HCI_USB_Descriptor_Endpoint_t), DTYPE_Endpoint }, // length, bDescriptorType
						HCI_USB_EDP_OUT_ISO, // endpoint id
						EP_TYPE_ISOCHRONOUS, // endpoint type
						0x0009, // wMaxPacketSize
						0x01 // polling bInterval. valid for iso or interrupt type
				},
				// iso_in_endpoint
				{ { sizeof(HCI_USB_Descriptor_Endpoint_t), DTYPE_Endpoint }, // length, bDescriptorType
						ENDPOINT_DIR_IN | HCI_USB_EDP_IN_ISO, // endpoint id
						EP_TYPE_ISOCHRONOUS, // endpoint type
						0x0009, // wMaxPacketSize
						0x01 // bInterval
				},

				// primary interface1 alternate2
				{ { sizeof(HCI_USB_Descriptor_Interface_t), DTYPE_Interface },
						1, 2, // AlternateSetting
						2, // bNumEndpoints
						0xE0, // bInterfaceclass ->wireless controller
						0x01, // bInterfaceSubClass -> RF controller
						0x01,// bInterfaceProtocol -> Bluetooth Primary controller
						NO_DESCRIPTOR // iInterface,  same as iProduct in USB_Descriptor_Device_t, or else not working
				},
				// iso_out_endpoint
				{ { sizeof(HCI_USB_Descriptor_Endpoint_t), DTYPE_Endpoint }, // length, bDescriptorType
						HCI_USB_EDP_OUT_ISO, // endpoint id
						EP_TYPE_ISOCHRONOUS, // endpoint type
						0x0011, // wMaxPacketSize
						0x01 // polling bInterval. valid for iso or interrupt type
				},
				// iso_in_endpoint
				{ { sizeof(HCI_USB_Descriptor_Endpoint_t), DTYPE_Endpoint }, // length, bDescriptorType
						ENDPOINT_DIR_IN | HCI_USB_EDP_IN_ISO, // endpoint id
						EP_TYPE_ISOCHRONOUS, // endpoint type
						0x0011, // wMaxPacketSize
						0x01 // bInterval
				},

				// primary interface1 alternate3
				{ { sizeof(HCI_USB_Descriptor_Interface_t), DTYPE_Interface },
						1, 3, // AlternateSetting
						2, // bNumEndpoints
						0xE0, // bInterfaceclass ->wireless controller
						0x01, // bInterfaceSubClass -> RF controller
						0x01,// bInterfaceProtocol -> Bluetooth Primary controller
						NO_DESCRIPTOR // iInterface,  same as iProduct in USB_Descriptor_Device_t, or else not working
				},
				// iso_out_endpoint
				{ { sizeof(HCI_USB_Descriptor_Endpoint_t), DTYPE_Endpoint }, // length, bDescriptorType
						HCI_USB_EDP_OUT_ISO, // endpoint id
						EP_TYPE_ISOCHRONOUS, // endpoint type
						0x0019, // wMaxPacketSize
						0x01 // polling bInterval. valid for iso or interrupt type
				},
				// iso_in_endpoint
				{ { sizeof(HCI_USB_Descriptor_Endpoint_t), DTYPE_Endpoint }, // length, bDescriptorType
						ENDPOINT_DIR_IN | HCI_USB_EDP_IN_ISO, // endpoint id
						EP_TYPE_ISOCHRONOUS, // endpoint type
						0x0019, // wMaxPacketSize
						0x01 // bInterval
				},

				// primary interface1 alternate4
				{ { sizeof(HCI_USB_Descriptor_Interface_t), DTYPE_Interface },
						1, 4, // AlternateSetting
						2, // bNumEndpoints
						0xE0, // bInterfaceclass ->wireless controller
						0x01, // bInterfaceSubClass -> RF controller
						0x01,// bInterfaceProtocol -> Bluetooth Primary controller
						NO_DESCRIPTOR // iInterface,  same as iProduct in USB_Descriptor_Device_t, or else not working
				},
				// iso_out_endpoint
				{ { sizeof(HCI_USB_Descriptor_Endpoint_t), DTYPE_Endpoint }, // length, bDescriptorType
						HCI_USB_EDP_OUT_ISO, // endpoint id
						EP_TYPE_ISOCHRONOUS, // endpoint type
						0x0021, // wMaxPacketSize
						0x01 // polling bInterval. valid for iso or interrupt type
				},
				// iso_in_endpoint
				{ { sizeof(HCI_USB_Descriptor_Endpoint_t), DTYPE_Endpoint }, // length, bDescriptorType
						ENDPOINT_DIR_IN | HCI_USB_EDP_IN_ISO, // endpoint id
						EP_TYPE_ISOCHRONOUS, // endpoint type
						0x0021, // wMaxPacketSize
						0x01 // bInterval
				},

				// primary interface1 alternate5
				{ { sizeof(HCI_USB_Descriptor_Interface_t), DTYPE_Interface },
						1, 5, // AlternateSetting
						2, // bNumEndpoints
						0xE0, // bInterfaceclass ->wireless controller
						0x01, // bInterfaceSubClass -> RF controller
						0x01,// bInterfaceProtocol -> Bluetooth Primary controller
						NO_DESCRIPTOR // iInterface,  same as iProduct in USB_Descriptor_Device_t, or else not working
				},
				// iso_out_endpoint
				{ { sizeof(HCI_USB_Descriptor_Endpoint_t), DTYPE_Endpoint }, // length, bDescriptorType
						HCI_USB_EDP_OUT_ISO, // endpoint id
						EP_TYPE_ISOCHRONOUS, // endpoint type
						0x003f, // wMaxPacketSize
						0x01 // polling bInterval. valid for iso or interrupt type
				},
				// iso_in_endpoint
				{ { sizeof(HCI_USB_Descriptor_Endpoint_t), DTYPE_Endpoint }, // length, bDescriptorType
						ENDPOINT_DIR_IN | HCI_USB_EDP_IN_ISO, // endpoint id
						EP_TYPE_ISOCHRONOUS, // endpoint type
						0x003f, // wMaxPacketSize
						0x01 // bInterval
				},
		};


static const HCI_USB_StdDescriptor_Devicequalifier_t hci_devicequalifier_desc = {0};

u8* hci_usbdesc_get_language(void) {
	return (u8*) (&hci_language_desc);
}

u8* hci_usbdesc_get_vendor(void) {
	return (u8*) (&hci_vendor_desc);
}

u8* hci_usbdesc_get_product(void) {
	return (u8*) (&hci_product_desc);
}
u8* hci_usbdesc_get_serial(void) {
	return (u8*) (&hci_serial_desc);
}

u8* hci_usbdesc_get_device(void) {
	return (u8*) (&hci_device_desc);
}

u8* hci_usbdesc_get_configuration(void) {
	return (u8*) (&hci_configuration_desc);
}
u8* hci_usbdesc_get_devicequalifier(void) {
	return (u8*) (&hci_devicequalifier_desc);
}
#else
// request parameters
/** Language descriptor structure. This descriptor, located in FLASH memory, is returned when the host requests
 *  the string descriptor with index 0 (the first index). It is actually an array of 16-bit integers, which indicate
 *  via the language ID table available at USB.org what languages the device supports for its string descriptors.
 */
static const USB_Descriptor_String_t language_desc = { {
		sizeof(USB_Descriptor_Header_t) + 2, DTYPE_String },
		{ LANGUAGE_ID_ENG } };

/** Manufacturer descriptor string. This is a Unicode string containing the manufacturer's details in human readable
 *  form, and is read out upon request by the host when the appropriate string ID is requested, listed in the Device
 *  Descriptor.
 */
static const USB_Descriptor_String_t vendor_desc = { { sizeof(USB_Descriptor_Header_t)
		+ sizeof(MYUDB_STRING_VENDOR) - 2, DTYPE_String }, // Header
		MYUDB_STRING_VENDOR };

/** Product descriptor string. This is a Unicode string containing the product's details in human readable form,
 *  and is read out upon request by the host when the appropriate string ID is requested, listed in the Device
 *  Descriptor.
 */
static const USB_Descriptor_String_t product_desc = { {
		sizeof(USB_Descriptor_Header_t) + sizeof(MYUDB_STRING_PRODUCT) - 2,
		DTYPE_String }, // Header
		MYUDB_STRING_PRODUCT };

/** Serial number string. This is a Unicode string containing the device's unique serial number, expressed as a
 *  series of uppercase hexadecimal digits.
 */
static const USB_Descriptor_String_t serial_desc = { { sizeof(USB_Descriptor_Header_t)
		+ sizeof(MYUDB_STRING_SERIAL) - 2, DTYPE_String }, // Header
		MYUDB_STRING_SERIAL };


static const USB_Descriptor_Device_t device_desc = { {
		sizeof(USB_Descriptor_Device_t), DTYPE_Device }, // Header
		0x0200, // USBSpecification, USB 2.0
		USB_CSCP_NoDeviceClass, // Class
		USB_CSCP_NoDeviceSubclass, // SubClass
		USB_CSCP_NoDeviceProtocol, // Protocol
		8, // Endpoint0Size, Maximum Packet Size for Zero Endpoint. Valid Sizes are 8, 16, 32, 64
		MYUDB_ID_VENDOR, // VendorID
		MYUDB_ID_PRODUCT, // ProductID
		MYUDB_ID_VERSION/*0x0100*/, // .ReleaseNumber
		MYUDB_USB_STRING_VENDOR, 	// .ManufacturerStrIndex
		MYUDB_USB_STRING_PRODUCT, // .ProductStrIndex
		0, 	// .SerialNumStrIndex, iSerialNumber
		1 };

static const MYUDB_USB_Descriptor_Configuration_t
		configuration_desc = {
				{
					{sizeof(USB_Descriptor_Configuration_Header_t),
					DTYPE_Configuration }, // Length, type
					sizeof(MYUDB_USB_Descriptor_Configuration_t), // TotalLength: variable
					2, // NumInterfaces
					1, // Configuration index
					NO_DESCRIPTOR, // Configuration String
					USB_CONFIG_ATTR_RESERVED, // Attributes
					USB_CONFIG_POWER_MA(100) // MaxPower = 100mA
				},
				// printer_interface
				{ { sizeof(USB_Descriptor_Interface_t), DTYPE_Interface },
						0, 0, // AlternateSetting
						2, // bNumEndpoints
						PRNT_CSCP_PrinterClass, // bInterfaceclass ->Printer
						PRNT_CSCP_PrinterSubclass, // bInterfaceSubClass -> Control
						PRNT_CSCP_BidirectionalProtocol,// bInterfaceProtocol
						NO_DESCRIPTOR // iInterface,  same as iProduct in USB_Descriptor_Device_t, or else not working
				},
				// printer_in_endpoint
				{ { sizeof(USB_Descriptor_Endpoint_t), DTYPE_Endpoint }, // length, bDescriptorType
						ENDPOINT_DIR_IN | MYUDB_EDP_IN_HCI, // endpoint id
						EP_TYPE_BULK, // endpoint type
						0x0040, // wMaxPacketSize
						0 // bInterval
				},
				// printer_out_endpoint
				{ { sizeof(USB_Descriptor_Endpoint_t), DTYPE_Endpoint }, // length, bDescriptorType
						MYUDB_EDP_OUT_HCI, // endpoint id
						EP_TYPE_BULK, // endpoint type
						0x0040, // wMaxPacketSize
						0 // polling bInterval. valid for iso or interrupt type
				},

				// printer_interface
				{ { sizeof(USB_Descriptor_Interface_t), DTYPE_Interface },
						1, 0, // AlternateSetting
						2, // bNumEndpoints
						PRNT_CSCP_PrinterClass, // bInterfaceclass ->Printer
						PRNT_CSCP_PrinterSubclass, // bInterfaceSubClass -> Control
						PRNT_CSCP_BidirectionalProtocol,// bInterfaceProtocol
						NO_DESCRIPTOR // iInterface,  same as iProduct in USB_Descriptor_Device_t, or else not working
				},
				// printer_in_endpoint
				{ { sizeof(USB_Descriptor_Endpoint_t), DTYPE_Endpoint }, // length, bDescriptorType
						ENDPOINT_DIR_IN | MYUDB_EDP_IN_VCD, // endpoint id
						EP_TYPE_BULK, // endpoint type
						0x0040, // wMaxPacketSize
						0 // bInterval
				},
				// printer_out_endpoint
				{ { sizeof(USB_Descriptor_Endpoint_t), DTYPE_Endpoint }, // length, bDescriptorType
						MYUDB_EDP_OUT_VCD, // endpoint id
						EP_TYPE_BULK, // endpoint type
						0x0040, // wMaxPacketSize
						0 // polling bInterval. valid for iso or interrupt type
				},
		};

u8* myudb_usbdesc_get_language(void) {
	return (u8*) (&language_desc);
}

u8* myudb_usbdesc_get_vendor(void) {
	return (u8*) (&vendor_desc);
}

u8* myudb_usbdesc_get_product(void) {
	return (u8*) (&product_desc);
}
u8* myudb_usbdesc_get_serial(void) {
	return (u8*) (&serial_desc);
}

u8* myudb_usbdesc_get_device(void) {
	return (u8*) (&device_desc);
}

u8* myudb_usbdesc_get_configuration(void) {
	return (u8*) (&configuration_desc);
}

#endif
#endif


