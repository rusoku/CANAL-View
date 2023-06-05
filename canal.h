/*
 * CANAL interface DLL for RUSOKU technologies for TouCAN, TouCAN Marine, TouCAN Duo USB to CAN bus converter
 *
 * Copyright (C) 2005-2023 Gediminas Simanskis (gediminas@rusoku.com)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published
 * by the Free Software Foundation; version 3.0 of the License.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.
 *
 */

//https://learn.microsoft.com/en-us/cpp/build/exporting-from-a-dll-using-declspec-dllexport?view=msvc-170
//https://learn.microsoft.com/en-us/cpp/build/exporting-functions-from-a-dll-by-ordinal-rather-than-by-name?view=msvc-170
//https://learn.microsoft.com/en-us/cpp/build/linking-an-executable-to-a-dll?view=msvc-170#determining-which-linking-method-to-use
//https://learn.microsoft.com/en-us/cpp/build/run-time-library-behavior?view=msvc-170#initializing-a-dll
//https://stackoverflow.com/questions/39330898/c-winapi-call-exported-function-via-getprocaddress

#pragma once

#ifdef WIN32
#define DllExport __declspec(dllexport)
#define WINAPI   __stdcall
#endif

// Canal Levels
#define CANAL_LEVEL_STANDARD				1
#define CANAL_LEVEL_USES_TCPIP				2

// VSCP Daemon client Open types
#define CANAL_COMMAND_OPEN_VSCP_LEVEL1		1	  // VSCP Level I channel (CAN)
#define CANAL_COMMAND_OPEN_VSCP_LEVEL2		2	  // VSCP Level II channel
#define CANAL_COMMAND_OPEN_VSCP_CONTROL		3	  // Daemon Control channel

#define CANAL_MAIN_VERSION      1
#define CANAL_MINOR_VERSION     0
#define CANAL_SUB_VERSION       15

#define CAN_MAX_STANDARD_ID     0x7ff
#define CAN_MAX_EXTENDED_ID     0x1fffffff

#define COMMAND_FAILURE         (0)
#define COMMAND_SUCCESS         (1)
#define PACKAGE_ACK             (0)
#define PACKAGE_NACK            (1)
#define PACKAGE_TIMEOUT         (-1)
#define PACKAGE_UNKNOWN         (-1)
#define PACKAGE_FAILURE         (0)
#define PACKAGE_SUCCESS         (1)

#define CANAL_NONBLOCK  		 1

/// ID flags
#define CANAL_IDFLAG_STANDARD       0x00000000	// Standard message id (11-bit)
#define CANAL_IDFLAG_EXTENDED       0x00000001	// Extended message id (29-bit)
#define CANAL_IDFLAG_RTR            0x00000002	// RTR-Frame
#define CANAL_IDFLAG_STATUS         0x00000004	// This package is a status indication (id holds error code)
#define CANAL_IDFLAG_SEND           0x80000000  // Reserved for use by application software to indicate send

/// Communicaton speeds
#define CANAL_BAUD_USER			    	0						// User specified (In CANAL i/f DLL).
#define CANAL_BAUD_1000 			    1						//   1 Mbit
#define CANAL_BAUD_800				    2						// 800 Kbit
#define CANAL_BAUD_500				    3						// 500 Kbit
#define CANAL_BAUD_250				    4						// 250 Kbit
#define CANAL_BAUD_125				    5						// 125 Kbit
#define CANAL_BAUD_100				    6						// 100 Kbit
#define CANAL_BAUD_50			  	    7						//  50 Kbit
#define CANAL_BAUD_20			  	    8						//  20 Kbit
#define CANAL_BAUD_10			  	    9						//  10 Kbit

/// Status message codes ( in received message )
#define CANAL_STATUSMSG_OK				0x00	// Normal condition.
#define CANAL_STATUSMSG_OVERRUN		    0x01	// Overrun occured when sending data to CAN bus.
#define CANAL_STATUSMSG_BUSLIGHT	    0x02	// Error counter has reached 96.
#define CANAL_STATUSMSG_BUSHEAVY	    0x03	// Error counter has reached 128.
#define CANAL_STATUSMSG_BUSOFF		    0x04	// Device is in BUSOFF. CANAL_STATUS_OK is
// sent when returning to operational mode.
#define CANAL_STATUSMSG_STUFF			0x20	// Stuff Error.
#define CANAL_STATUSMSG_FORM			0x21	// Form Error.
#define CANAL_STATUSMSG_ACK				0x23	// Ack Error.
#define CANAL_STATUSMSG_BIT1			0x24	// Bit1 Error.
#define CANAL_STATUSMSG_BIT0			0x25	// Bit0 Error.
#define CANAL_STATUSMSG_CRC				0x27	// CRC Error.

/// Status codes /returned by status request)
#define CANAL_STATUS_NONE					0x00000000
#define CANAL_STATUS_ACTIVE				    0x10000000
#define CANAL_STATUS_PASSIVE				0x40000000
#define CANAL_STATUS_BUS_OFF				0x80000000
#define CANAL_STATUS_BUS_WARN				0x20000000
#define CANAL_STATUS_PHY_FAULT				0x08000000
#define CANAL_STATUS_PHY_H					0x04000000
#define CANAL_STATUS_PHY_L					0x02000000
#define CANAL_STATUS_SLEEPING				0x01000000
#define CANAL_STATUS_STOPPED				0x00800000
#define CANAL_STATUS_RECIVE_BUFFER_FULL		0x00400000	// Drivers buffer
#define CANAL_STATUS_TRANSMIT_BUFFER_FULL	0x00200000	// Drivers buffer

/// Error Codes
#define CANAL_ERROR_SUCCESS						0		// All is OK
#define CANAL_ERROR_BAUDRATE					1		// Baudrate error
#define CANAL_ERROR_BUS_OFF						2		// Bus off error
#define CANAL_ERROR_BUS_PASSIVE				    3		// Bus Passive error
#define CANAL_ERROR_BUS_WARNING					4		// Bus warning error
#define CANAL_ERROR_CAN_ID						5		// Invalid CAN ID
#define CANAL_ERROR_CAN_MESSAGE					6		// Invalid CAN message
#define CANAL_ERROR_CHANNEL						7		// Invalid channel
#define CANAL_ERROR_FIFO_EMPTY					8		// FIFO is empty
#define CANAL_ERROR_FIFO_FULL					9		// FIFI is full
#define CANAL_ERROR_FIFO_SIZE					10		// FIFO size error
#define CANAL_ERROR_FIFO_WAIT					11
#define CANAL_ERROR_GENERIC						12		// Generic error
#define CANAL_ERROR_HARDWARE					13		// Hardware error
#define CANAL_ERROR_INIT_FAIL					14		// Initialization failed
#define CANAL_ERROR_INIT_MISSING				15
#define CANAL_ERROR_INIT_READY					16
#define CANAL_ERROR_NOT_SUPPORTED				17		// Not supported
#define CANAL_ERROR_OVERRUN						18		// Overrun
#define CANAL_ERROR_RCV_EMPTY					19		// Receive buffer empty
#define CANAL_ERROR_REGISTER					20		// Register value error
#define CANAL_ERROR_TRM_FULL					21
#define CANAL_ERROR_ERRFRM_STUFF				22		// Errorframe: stuff error detected
#define CANAL_ERROR_ERRFRM_FORM					23		// Errorframe: form error detected
#define CANAL_ERROR_ERRFRM_ACK					24		// Errorframe: acknowledge error
#define CANAL_ERROR_ERRFRM_BIT1					25		// Errorframe: bit 1 error
#define CANAL_ERROR_ERRFRM_BIT0					26		// Errorframe: bit 0 error
#define CANAL_ERROR_ERRFRM_CRC					27		// Errorframe: CRC error
#define CANAL_ERROR_LIBRARY						28		// Unable to load library
#define CANAL_ERROR_PROCADDRESS					29		// Unable get library proc address
#define CANAL_ERROR_ONLY_ONE_INSTANCE			30		// Only one instance allowed
#define CANAL_ERROR_SUB_DRIVER					31		// Problem with sub driver call
#define CANAL_ERROR_TIMEOUT						32		// Blocking call timeout
#define CANAL_ERROR_NOT_OPEN					33 		// The device is not open.
#define CANAL_ERROR_PARAMETER					34 		// A parameter is invalid.
#define CANAL_ERROR_MEMORY						35 		// Memory exhausted.
#define CANAL_ERROR_INTERNAL					36 		// Some kind of internal program error
#define CANAL_ERROR_COMMUNICATION				37 		// Some kind of communication error
#define CANAL_ERROR_USER						38      // Login error

// CANAL commands sent over the pipe interface (depricated)
#define CANAL_COMMAND_NOOP					    0		// No command
#define CANAL_COMMAND_OPEN						1		// Open channel
#define CANAL_COMMAND_CLOSE						2		// Close channel
#define CANAL_COMMAND_SEND						3		// Send message
#define CANAL_COMMAND_RECEIVE					4		// Receive message
#define CANAL_COMMAND_CHECKDATA			    	5		// Check if data is available
#define CANAL_COMMAND_BAUDRATE					6		// Set Baudrate
#define CANAL_COMMAND_STATUS					7		// Get status
#define CANAL_COMMAND_STATISTICS				8		// Get statistics
#define CANAL_COMMAND_FILTER					9		// Set filter
#define CANAL_COMMAND_MASK						10		// Set mask
#define CANAL_COMMAND_VERSION					11		// CANAL version
#define CANAL_COMMAND_DLL_VERSION			    12		// CANAL DLL version
#define CANAL_COMMAND_VENDOR_STRING				13		// CANAL vendor string
#define CANAL_COMMAND_LEVEL						14		// CANAL Level bitarray

// CANAL responses sent over the pipe interface (depricated)
#define CANAL_RESPONSE_NONE						0		//
#define CANAL_RESPONSE_SUCCESS					1		// OK message
#define CANAL_RESPONSE_ERROR					2		// ERROR message
#define CANAL_RESPONSE_MESSAGE					3		// Response to read

// CANAL error codes sent over the client interface
// on error responses
#define CANAL_IFERROR_GENERAL					128		// General error
#define CANAL_IFERROR_UNKNOWN_COMMAND			129
#define CANAL_IFERROR_CHANNEL_OPEN				130
#define CANAL_IFERROR_CHANNEL_CLOSED			131
#define CANAL_IFERROR_SEND_SUCCESS				132
#define CANAL_IFERROR_SEND_MSG_ALLOCATON		133
#define CANAL_IFERROR_BUFFER_EMPTY				134
#define CANAL_IFERROR_BUFFER_FULL				135
#define CANAL_IFERROR_READ_FAILURE				136
#define CANAL_IFERROR_SEND_STORAGE				137

// * * * TCP/IP FAST mode interface constants

// FAST mode primary states
#define CANAL_BINARY_FRAME_TYPE_VSCP			0		// VSCP event
#define CANAL_BINARY_FRAME_TYPE_ERROR			1		// ACK/NACK/errors
#define CANAL_BINARY_FRAME_TYPE_COMMAND			2		// Command frame
#define CANAL_BINARY_FRAME_TYPE_CAN				3		// CAN Frame

#define CANAL_BINARY_COMMAND_NOOP				0		// No operation
#define CANAL_BINARY_COMMAND_READ				1		// Read one frame
#define CANAL_BINARY_COMMAND_CLOSE				2		// Close communication channel

// FAST error codes
#define CANAL_BINARY_ERROR_NONE					0		// OK
#define CANAL_BINARY_ERROR_GENERAL				1		// General error
#define CANAL_BINARY_ERROR_TO_SMALL				2		// Packet smaller then min packet
#define CANAL_BINARY_ERROR_FORMAT				3		// Packet have bad format
#define CANAL_BINARY_ERROR_UNKNOW_FRAME			4		// Unknown frame type
#define CANAL_BINARY_ERROR_MEMORY				5		// No room for event
#define CANAL_BINARY_ERROR_NO_DATA				6		// No data available
#define CANAL_BINARY_ERROR_INVALID_CMD			7		// Command not recognized.

// Filter mask settings
#define CANUSB_ACCEPTANCE_FILTER_ALL		    0x00000000
#define CANUSB_ACCEPTANCE_MASK_ALL			    0xFFFFFFFF

#define CANAL_DEVLIST_SIZE_MAX 64

typedef struct struct_CANAL_DEV_INFO {
    unsigned int    DeviceId;
    unsigned int    vid;
    unsigned int    pid;
    char            SerialNumber[10];
} canal_dev_info, *pcanal_dev_info;

typedef struct struct_CANAL_DEV_LIST{
    canal_dev_info canDevInfo[CANAL_DEVLIST_SIZE_MAX];
    unsigned int   canDevCount;
} canal_dev_list, *pcanal_dev_list;

/** @name  CanalMsg
 *  @brief CANAL CAN message structure
*/

typedef struct structCanalMsg {
    unsigned long flags;			    // CAN message flags
    unsigned long obid;			    	// Used by driver for channel info etc.
    unsigned long id;			      	// CAN id (11-bit or 29-bit)
    unsigned char sizeData;		  	    // Data size 0-8
    unsigned char data[8];		        // CAN Data
    unsigned long timestamp;		    // Relative time stamp for package in microseconds
} canalMsg;
typedef canalMsg*   PCANALMSG;

/** @name  CanalStatistics
 *  @brief CANAL statistics structure
*/
typedef struct structCanalStatistics {
    unsigned long cntReceiveFrames;				// # of receive frames
    unsigned long cntTransmitFrames;			// # of transmitted frames
    unsigned long cntReceiveData;			  	// # of received data bytes
    unsigned long cntTransmitData;			    // # of transmitted data bytes
    unsigned long cntOverruns;				    // # of overruns
    unsigned long cntBusWarnings;			  	// # of bys warnings
    unsigned long cntBusOff;					// # of bus off's
} canalStatistics;
typedef canalStatistics*    PCANALSTATISTICS;

/** @name  CanalStatus
 *  @brief CANAL status structure
*/
typedef struct structCanalStatus {
    unsigned long channel_status;	  	// Current state for channel
    unsigned long lasterrorcode;		// Last error code
    unsigned long lasterrorsubcode;		// Last error subcode
    char lasterrorstr[80];	    		// Last error string
} canalStatus;
typedef canalStatus*    PCANALSTATUS;

/** @name  CANHANDLE
 *  @brief CAN driver open handle
*/
typedef long    CANHANDLE;

#ifdef __cplusplus
extern "C" {
#endif

/** @name   CanalOpen
    @brief  CAN driver open handle.
   	@param  pDevice Physical device to connect to.
   	@param  flags   Give extra info to the CANAL i/F.
	@return Handle of device or -1 if error.
*/
DllExport   long  WINAPI  CanalOpen(const char *pDevice, unsigned long flags);

/** @name   CanalClose
    @brief  Close a CANAL chanel.
   	@param  handle Handle to close physical CANAL channel.
	@return zero on success or error-code on failure.
*/
DllExport   long  WINAPI  CanalClose(long handle);

/** @name   CanalGetLevel
    @brief
   	@param  handle Handle to close physical CANAL channel.
	@return
*/
DllExport  long  WINAPI  CanalGetLevel(long handle);

/**
    @name   CanalSend
    @brief  Send a message on a CANAL channel
	@param  handle - Handle to open physical CANAL channel.
	@param  pCanMsg - Message to send.
	@return zero on success or error-code on failure.
*/
DllExport   int WINAPI  CanalSend(long handle, PCANALMSG pCanalMsg);

/**
    @name   CanalBlockingSend
    @brief  Send a message on a CANAL channel
	@param  handle - Handle to open physical CANAL channel.
	@param  pCanMsg - Message to send.
    @param  @param timeout - Timeout in ms. 0 is forever.
	@return zero on success or error-code on failure.
*/
DllExport   int WINAPI  CanalBlockingSend(long handle, PCANALMSG pCanalMsg, unsigned long timeout);

/**
    @name   CanalSend
    @brief  Receive a message from a CANAL channel
	@param  handle - Handle to open physical CANAL channel.
	@param  pCanMsg - Message to receive.
	@return zero on success or error-code on failure.
*/
DllExport   int WINAPI  CanalReceive(long handle, PCANALMSG pCanalMsg);

/**
    @name   CanalBlockingSend
    @brief  Receive a message from a CANAL channel
	@param  handle - Handle to open physical CANAL channel.
	@param  pCanMsg - Message to receive.
    @param  @param timeout - Timeout in ms. 0 is forever.
	@return zero on success or error-code on failure.
*/
DllExport   int WINAPI  CanalBlockingReceive(long handle, PCANALMSG pCanalMsg, unsigned long timeout);

/**
    @name   CanalDataAvailable
    @brief  Check a CANAL channel for message availability
	@param  handle - Handle to open physical CANAL channel.
	@return Zero if no message is available or the number of messages is
	        there are messages waiting to be received.
*/
DllExport   int WINAPI  CanalDataAvailable(long handle);

/**
    @name   CanalGetStatus
    @brief  Get status for a CANAL channel.
	@param  handle - Handle to open physical CANAL channel.
    @param  pCanStatus Pointer to a CANAL status structure.
    @param  zero on success or error-code on failure.
*/
DllExport   int WINAPI  CanalGetStatus(long handle, PCANALSTATUS pCanalStatus);

/**
    @name   CanalGetStatistics
    @brief  Get statistics for a CANAL channel.
	@param  handle - Handle to open physical CANAL channel.
    @param  pCanStatus Pointer to a CANAL status structure.
    @param  zero on success or error-code on failure.
*/
DllExport   int WINAPI  CanalGetStatistics(long handle, PCANALSTATISTICS pCanalStatistics);

/**
    @name   CanalSetFilter
    @brief  Set the filter for a CANAL channel.
	@param  handle - Handle to open physical CANAL channel.
    @param  filter
    @param  zero on success or error-code on failure.
*/
DllExport   int WINAPI  CanalSetFilter(long handle, unsigned long filter);

/**
    @name   CanalSetMask
    @brief  Set the mask for a CANAL channel.
	@param  handle - Handle to open physical CANAL channel.
    @param  filter
    @param  zero on success or error-code on failure.
*/
DllExport   int WINAPI  CanalSetMask(long handle, unsigned long mask);

/**
    @name   CanalSetBaudrate
    @brief  Set the baudrate for a CANAL channel.
	@param  handle - Handle to open physical CANAL channel.
    @param  baudrate Baudrate for the channel
    @param  zero on success or error-code on failure.
*/
DllExport   int WINAPI  CanalSetBaudrate(long handle, unsigned long baudrate);

/**
    @name   CanalGetVersion
    @brief  Get CANAL version.
    @return version for CANAL i/f.
*/
DllExport unsigned long WINAPI  CanalGetVersion(void);

/**
    @name   CanalGetDllVersion
    @brief  Get CANAL DLL version.
    @return version for CANAL dll implemention.
*/
DllExport unsigned long WINAPI  CanalGetDllVersion(void);

/**
    @name   CanalGetDriverInfo
    @brief  Get CANAL driver properties.
    @return driver info string.
*/
DllExport  const char* WINAPI  CanalGetVendorString(void);

/**
    @name   CanalGetDriverInfo
    @brief  Get CANAL vendor string.
    @return vendor string.
*/
DllExport const char* WINAPI  CanalGetDriverInfo(void);

/*************************************************************
 *  EXTENDED NON STANDARD API LIST
 */

/* FILTER req type */
typedef enum {
    FILTER_ACCEPT_ALL   = 0,
    FILTER_REJECT_ALL,
    FILTER_VALUE,
}Filter_Type_TypeDef;

/*!
	Set the 11bit filter  ID, Mask for a CANAL channel

	@param handle Handle to open physical CANAL channel.
	@return zero on success or error-code on failure.
*/

DllExport int WINAPI CanalSetFilter11bit( long handle, Filter_Type_TypeDef type,  unsigned long list, unsigned long mask );

/*!
	Set the 29bit filter  ID, Mask for a CANAL channel

	@param handle Handle to open physical CANAL channel.
	@return zero on success or error-code on failure.
*/
DllExport int WINAPI CanalSetFilter29bit( long handle, Filter_Type_TypeDef type,  unsigned long list, unsigned long mask );

/*!
	Get bootloader ver

	@param handle Handle to open physical CANAL channel.
	@return zero on success or error-code on failure.
*/
DllExport int WINAPI CanalGetBootloaderVersion(long handle, unsigned long* bootloader_version);

DllExport int WINAPI CanalGetHardwareVersion(long handle, unsigned long* hardware_version);
DllExport int WINAPI CanalGetFirmwareVersion(long handle, unsigned long* firmware_version);
DllExport int WINAPI CanalGetSerialNumber(long handle, unsigned long* serial);
DllExport int WINAPI CanalGetVidPid(long handle, unsigned long* vidpid);
DllExport int WINAPI CanalGetDeviceId(long handle, unsigned long* deviceid);
DllExport int WINAPI CanalGetVendor(long handle, unsigned int size, char* vendor);
DllExport int WINAPI CanalInterfaceStart(long handle);
DllExport int WINAPI CanalInterfaceStop(long handle);
DllExport int WINAPI CanalGetDeviceList(pcanal_dev_list canalDeviceList, int canalDeviceListSize);

#ifdef __cplusplus
}
#endif

//DWORD WINAPI ReadThread(LPVOID lParam);
