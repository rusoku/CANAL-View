# CANAL-View
CANAL View Qt GUI for TouCAN series USB to CAN bus converter

	Init string parameters:
	
	        1                  2           3       4       5       6     7
	device_type ; device_serial_number ; speed ; tseg1 ; tseg2 ;  sjw ; brp

	where:
	1 - device type: for &quot;TouCAN&quot;, &quot;TouCAN Marine&quot;, &quot;TouCAN Duo&quot; this parameter		                               always == &quot;0&quot;
	2 - device serial number: always 8 symbols like &quot;12345678&quot;
                               serial number you can find on bottom device case side like &quot;sn:12345678&quot;
	3 - CAN bus speed: standard speed list: 10;20;50;100;125;250;500;1000
	4 - CAN bus parameter &quot;tseg1&quot;
	5 - CAN bus parameter &quot;tseg2&quot;
	6 - CAN bus parameter &quot;sjw&quot;
	7 - CAN bus parameter &quot;brp&quot;

      	If standard speed list is used next arguments can be skipped.
	For example init string will looks like &quot;0; 12345678; 1000&quot;

	For non standard CAN bus speeds: &quot;speed&quot; parameter must be == &quot;0&quot;
	For example 1000 Mbit speed init string will looks like: &quot;0;12345678;0;7;2;2;5&quot;
	   where: tseg1 = 7, tseg2 = 2, sjw = 2, brp = 5
	CAN interface clock is : 100MHz
 
	CAN bus parameters calculator: http://www.bittiming.can-wiki.info/
