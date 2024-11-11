#if !defined(__TI_TIVA_C_H__)
#define __TI_TIVA_C_H__

#include "tm4c123gh6pm.h"
#include "part.h"
#include <Energia.h>
#define TARGET_IS_BLIZZARD_RB1
#define PART_TM4C1233H6PM
#define PART_LM4F120H5QR
// standard libs
#include <math.h>
// #include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <avr/dtostrf.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

// others
#include "itoa.h"
#include "part.h"
#include "binary.h"

// Driver libs
#include "driverlib/debug.h"
#include "driverlib/pwm.h"
#include "driverlib/eeprom.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"

// INC libs
#include "inc/hw_memmap.h"
#include "inc/hw_eeprom.h"
#include "inc/hw_gpio.h"
#include "inc/hw_nvic.h"
#include "inc/hw_types.h"

#ifdef __cplusplus
extern "C"
{

  void _init(void)
  {
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);
    if (ROM_EEPROMInit() == EEPROM_INIT_ERROR)
    {
      if (ROM_EEPROMInit() != EEPROM_INIT_ERROR)
        EEPROMMassErase();
    }

    timerInit();

    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOQ);

    // Unlock and commit NMI pins PD7 and PF0
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0x4C4F434B;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x1;
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0x4C4F434B;
    HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= 0x80;

// Deep Sleep mode init
// DSLP clock = PIOSC / 16 = 1MHz
// Note: Couldn't find the define constants for SysCtlDeepSleepPowerSet in the driverlib.
//
#ifdef TARGET_IS_BLIZZARD_RB1
    ROM_SysCtlDeepSleepClockSet(SYSCTL_DSLP_DIV_16 | SYSCTL_DSLP_OSC_INT);
    SysCtlDeepSleepPowerSet(0x21);           // FLASHPM = LOW_POWER_MODE, SRAMPM = STANDBY_MODE
    SysCtlLDODeepSleepSet(SYSCTL_LDO_1_00V); // Going lower tends to be very flaky and cause continual resets
                                             // particularly when measuring MCU current.
#endif

  } /* void _init(void) */

} /* extern "C" */
#endif

#ifdef DEBUG
void __error__(char *pcFilename, uint32_t ui32Line)
{
  while (1)
    ;
}
#endif

#endif // __TI_TIVA_C_H__
