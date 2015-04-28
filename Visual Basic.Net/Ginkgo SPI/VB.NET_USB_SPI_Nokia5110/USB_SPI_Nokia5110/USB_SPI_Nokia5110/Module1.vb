
'Hardware connection
'Nokia5110	Ginkgo USB-SPI
'1.RST		GPIO_10(P7)
'2.CE		SPI_SEL0(P11)
'3.DC		GPIO_11(P9)
'4.DIN		SPI_MOSI(P17)
'5.CLK		SPI_SCK(P13)
'6.VCC		5V
'7.BL		GPIO_9(P5)
'8.GND		GND


Module Module1

    Sub Main()
        LCD5110.LCD_Init()
        LCD5110.LCD_Clear()
        LCD5110.LCD_WriteString(0, 1, "Hello viewtool")
        LCD5110.LCD_WriteString(0, 3, "Ginkgo USB-SPI")

    End Sub

End Module
