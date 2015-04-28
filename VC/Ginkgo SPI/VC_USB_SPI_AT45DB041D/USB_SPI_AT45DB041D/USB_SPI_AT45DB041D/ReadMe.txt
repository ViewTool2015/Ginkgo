Copyright 2009-2014, ViewTool
http://www.viewtool.com/
All Rights Reserved

Project USB_SPI_AT45DB041D is a demo how to operate AT45DB041D via Ginkgo SPI adapter.

Connect description:

	  Ginkgo USB-SPI adaptor
	   17  13
	 19┃15┃11         1
	 ┃┃┃┃┃         ┃
	┏━━━━  ━━━━━┓
	┃* * * * * * * * * * ┃
	┃* * * * * * * * * * ┃
	┗━━━━━━━━━━┛
	 ┃                 ┃  
	 20                 2

                     SPI Module/ AT45DB041
                         ┏━━━┓
    SPI_MOSI/Pin17   SI━┃1    8┃━ SO  SPI_MISO/Pin15          
    SPI_SCK/Pin13   SCK━┃2    7┃━ GND GND/Pin19/Pin20
    3.3V/Pin2       SET━┃3    6┃━ VCC 3.3V/Pin2 
    SPI_SEL0/Pin11   CS━┃4    5┃━ WP  3.3V/Pin2 
                         ┗━━━┛ 

 
	SPI_MOSI/Pin17 ━  SI
	SPI_MISO/Pin15 ━  SO
	SPI_SCK/Pin13  ━  SCK
	SPI_SEL0/Pin11 ━  CS