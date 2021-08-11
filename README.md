# CANAL-View
CANAL View Qt GUI for TouCAN series USB to CAN bus converter

	Init string parameters:
	
	        1                  2           3       4       5       6     7
	device_type ; device_serial_number ; speed ; tseg1 ; tseg2 ;  sjw ; brp

	where:
	1 - device type: for TouCAN,TouCAN Marine,TouCAN Duo this paramer is always == 0
	2 - device serial number: always 8 symbols like 12345678
                               serial number you can find on bottom device case side like sn:12345678
	3 - CAN bus speed: standard speed list: 10;20;50;100;125;250;500;1000
	4 - CAN bus parameter tseg1
	5 - CAN bus parameter tseg2
	6 - CAN bus parameter sjw
	7 - CAN bus parameter brp

      	If standard speed list is used next arguments can be skipped.
	For example init string will looks like 0;12345678;1000

	For non standard CAN bus speeds: speed parameter must be == 0
	For example 1000 kbit speed init string will looks like: 0;12345678;0;7;2;2;5
	   where: tseg1 = 7, tseg2 = 2, sjw = 2, brp = 5
	CAN interface clock is : 50MHz
 
	CAN bus parameters calculator: http://www.bittiming.can-wiki.info
