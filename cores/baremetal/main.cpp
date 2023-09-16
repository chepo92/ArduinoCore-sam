#include "Arduino.h"
#include <stdint.h>
#include "variant.h"

// LED on B27 (Arduino Due)
//#define LED_PIN 27

// Output on B14 (Duet)
#define LED_PIN 0  // Will blink pin4 PE0 E2_STOP on Duet Board

// PMC definitions
#define PMC_PCER0 *(volatile uint32_t *)0x400E0410    // PMC Peripheral Clock Enable Register 0; Address SAM3: 0x400E0610 SAM4E: 0x400E0410

// Peripheral Identifiers SAM3
// #define ID_PIOA 11
// #define ID_PIOB 12
// #define ID_PIOC 13
// #define ID_PIOD 14
// #define ID_PIOE 15
// #define ID_PIOF 16

// Peripheral Identifiers SAM4E
#define ID_PIOA 9
#define ID_PIOB 10
#define ID_PIOC 11
#define ID_PIOD 12
#define ID_PIOE 13

#define PMC_WPMR *(volatile uint32_t *)0x400E06E4 // SAM3: 0x400E06E4 SAM4: 0x400E04E4

#define PMC_WPKEY 0x504D43  // 0/1 = Disables/Enables the Write Protect if WPKEY corresponds to 0x504D43 (“PMC” in ASCII)

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

#define PIOA ((struct gpio *)0x400E0E00)
#define PIOB ((struct gpio *)0x400E1000)
#define PIOC ((struct gpio *)0x400E1200)
#define PIOD ((struct gpio *)0x400E1400)
#define PIOE ((struct gpio *)0x400E1600)
// #define PIOF ((struct gpio *)0x400E1800)  // SAM4 has no PIOF

#define PIOA_WPMR *(volatile uint32_t *)0x400E0EE4
#define PIOB_WPMR *(volatile uint32_t *)0x400E10E4
#define PIOC_WPMR *(volatile uint32_t *)0x400E12E4
#define PIOD_WPMR *(volatile uint32_t *)0x400E14E4
#define PIOE_WPMR *(volatile uint32_t *)0x400E16E4
// #define PIOE_WPMR *(volatile uint32_t *)0x400E18E4  // SAM4 has no PIOF, SAM3 was missing

#define PIO_WPKEY 0x50494F // 0/1: Disables/Enables the Write Protect if WPKEY corresponds to 0x50494F (“PIO” in ASCII).

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




