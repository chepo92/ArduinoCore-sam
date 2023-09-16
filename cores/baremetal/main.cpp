#include "Arduino.h"
#include <stdint.h>
#include "variant.h"



// PIO definitions
// Parallel Input/Output Controller (PIO) User Interface
// You can see that consecutive 32 bit registers are mapped at 4 bytes increments. 
// 0x0004 bytes = 4*8 = 32 bit
struct gpio {
  // + 0x00
  volatile uint32_t PIO_PER;      // PIO Enable Register
  volatile uint32_t PIO_PDR;      // PIO Disable Register
  volatile uint32_t PIO_PSR;      // PIO Status Register 
  volatile uint32_t res1;
  // + 0x10
  volatile uint32_t PIO_OER;
  volatile uint32_t PIO_ODR;
  volatile uint32_t PIO_OSR;
  volatile uint32_t res2;
  // + 0x20
  volatile uint32_t PIO_IFER;
  volatile uint32_t PIO_IFDR;
  volatile uint32_t PIO_IFSR;
  volatile uint32_t res3;
  // + 0x30
  volatile uint32_t PIO_SODR;
  volatile uint32_t PIO_CODR;
  volatile uint32_t PIO_ODSR;
  volatile uint32_t PIO_PDSR;
  // + 0x40
  volatile uint32_t PIO_IER;
  volatile uint32_t PIO_IDR;
  volatile uint32_t PIO_IMR;
  volatile uint32_t PIO_ISR;
  // + 0x50
  volatile uint32_t PIO_MDER;
  volatile uint32_t PIO_MDDR;
  volatile uint32_t PIO_MDSR;
  volatile uint32_t res4;
  // + 0x60
  volatile uint32_t PIO_PUDR;
  volatile uint32_t PIO_PUER;
  volatile uint32_t PIO_PUSR;
  volatile uint32_t res5;
  // + 0x70
  volatile uint32_t PIO_ABCDSR1;    // SAM4E Table 33-5. Register Mapping (Continued)
  volatile uint32_t PIO_ABCDSR2;    // SAM4E Table 33-5. Register Mapping (Continued)
  volatile uint32_t res6[2];
  // + 0x80
  volatile uint32_t PIO_SCIFSR;
  volatile uint32_t PIO_DIFSR;
  volatile uint32_t PIO_IFDGSR;
  volatile uint32_t PIO_SCDR;
  // + 0x90
  volatile uint32_t res7[4];
  // + 0xA0
  volatile uint32_t PIO_OWER;
  volatile uint32_t PIO_OWDR;
  volatile uint32_t PIO_OWSR;
  volatile uint32_t res8;
  // ...
};

void HardwareInit (void)
{
  // enable peripheral clock
	//  PMC_WPMR  = PMC_WPKEY << 8 | 0;
  PMC_PCER0 = 1 << ID_PIOE;  // PIO Clock on E

  // PIOC
	//  PIOC_WPMR  = PIO_WPKEY << 8 | 0;

  PIOE->PIO_PER = 1 << LED_PIN;
  PIOE->PIO_OER = 1 << LED_PIN;
  PIOE->PIO_OWER = 1 << LED_PIN;
}

void delay (volatile uint32_t time)
{
  while (time--)
    __asm ("nop");
}

static uint32_t* const WDT_MR = (uint32_t*) 0x400E1854;     //  Watchdog Timer Mode Register SAM4: 0x400E1854  SAM3: 0x400E1A54


int main()
{
  // watchdog timer is actived on boot with a default timeout so disable it
  // note: you can only write to WDT_MR once after power on reset
  // Atmel SAM3X8E Datasheet, section 15.4, page 261
  // Atmel SAM4E8E Datasheet, section 16.5.2, page 362
  *WDT_MR |= (1 << 15); // WDDIS (watchdog disable) bit, SAM3 page 265, SAM 4 page 362


  HardwareInit ();

	setup();

	for (;;)
	{
		loop();
		//if (serialEventRun) serialEventRun();
    // while (1)
    // {
    //   PIOE->PIO_SODR = 1 << LED_PIN;
    //   delay (200000);
    //   PIOE->PIO_CODR = 1 << LED_PIN;
    //   delay (200000);
    // }    
	}

	return 0 ; 
  
}




