/* ========================================================================== */
/*                                                                            */
/*   startup.c                                                                */
/*   (c) 2017 Bob Cousins                                                     */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/*   Minimal blink for SAM4E (Duet)                                           */
/* ========================================================================== */

#include <stdint.h>
#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif


#define SCB_VTOR_ADDR 0xE000ED08    // Shared SAM3/4

#define SCB_VTOR *(volatile uint32_t *)SCB_VTOR_ADDR

// These must be defned in linker file
extern unsigned long _etext;
extern unsigned long _srelocate;
extern unsigned long _erelocate;
extern unsigned long _sbss;
extern unsigned long _ebss;
extern unsigned long _estack;

// PIO definitions
// Parallel Input/Output Controller (PIO) User Interface
// You can see that consecutive 32 bit registers are mapped at 4 bytes increments. 
// 0x0004 bytes = 4*8 = 32 bit

// typedef  struct { } gpio  alternative as in \system\CMSIS\Device\ATMEL\sam4e\include\component\pio.h
extern struct gpio {
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



extern int main(void);

typedef void( *const intfunc )( void );

void Reset_Handler(void) __attribute__((__interrupt__));
void Default_Handler(void);


#define NMI_Handler         Default_Handler
#define HardFault_Handler   Default_Handler
#define MemManage_Handler   Default_Handler
#define BusFault_Handler    Default_Handler
#define UsageFault_Handler  Default_Handler
#define MemManage_Handler   Default_Handler
#define SVC_Handler         Default_Handler
#define DebugMon_Handler    Default_Handler
#define PendSV_Handler      Default_Handler
#define SysTick_Handler     Default_Handler




__attribute__ ((section(".vectors")))
void (* const g_pfnVectors[])(void) = {
    (intfunc)((unsigned long)&_estack), /* The stack pointer after relocation */
    Reset_Handler,              /* Reset Handler */
    NMI_Handler,                /* NMI Handler */
    HardFault_Handler,          /* Hard Fault Handler */
    MemManage_Handler,          /* MPU Fault Handler */
    BusFault_Handler,           /* Bus Fault Handler */
    UsageFault_Handler,         /* Usage Fault Handler */
    0,                          /* Reserved */
    0,                          /* Reserved */
    0,                          /* Reserved */
    0,                          /* Reserved */
    SVC_Handler,                /* SVCall Handler */
    DebugMon_Handler,           /* Debug Monitor Handler */
    0,                          /* Reserved */
    PendSV_Handler,             /* PendSV Handler */
    SysTick_Handler             /* SysTick Handler */
};

void Reset_Handler(void)
{
    /* Init Data:
     * - Loads data from addresses defined in linker file into RAM
     * - Zero bss (statically allocated uninitialized variables)
     */
    unsigned long *src, *dst;

    /* copy the data segment into ram */
    src = &_etext;
    dst = &_srelocate;
    if (src != dst)
        while(dst < &_erelocate)
            *(dst++) = *(src++);

    /* zero the bss segment */
    dst = &_sbss;
    while(dst < &_ebss)
        *(dst++) = 0;

    SCB_VTOR = ((uint32_t)g_pfnVectors & (uint32_t)0x1FFFFF80);

    main();
    while(1) {}
}

void Default_Handler(void)
{
    while (1) {}
}

void delay (volatile uint32_t time)
{
  while (time--)
    __asm ("nop");
}


#ifdef __cplusplus
}
#endif