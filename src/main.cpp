#include "tivac.h"

#define PWM_FREQUENCY 55
#define PWM_PIN GPIO_PIN_1
#define PWM_PIN_CONF GPIO_PCTL_PD0_M1PWM0

void setup_clock()
{
  ROM_SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
  ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_64);
}

void setup_peripherals()
{
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
}

void setup_pins()
{

  ROM_GPIOPinTypePWM(GPIO_PORTD_BASE, PWM_PIN);
  ROM_GPIOPinConfigure(PWM_PIN_CONF);

  HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
  HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
  HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;

  ROM_GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4 | PWM_PIN, GPIO_DIR_MODE_IN);
  ROM_GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4 | PWM_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}

void setup_pwm(volatile uint32_t &load, volatile uint8_t &adjust)
{
  PWMGenConfigure(PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN);
  PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, load);
  ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, (adjust * load) / 1000);
  ROM_PWMOutputState(PWM1_BASE, PWM_OUT_3_BIT, true);
  ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_0);
}

int main(void)
{
  volatile uint32_t ui32Load;
  volatile uint32_t ui32PWMClock;
  volatile uint8_t ui8Adjust;
  ui8Adjust = 1;

  setup_clock();
  setup_peripherals();
  setup_pins();
  ui32PWMClock = SysCtlClockGet() / 64;
  ui32Load = (ui32PWMClock / PWM_FREQUENCY) - 1;
  setup_pwm(ui32Load, ui8Adjust);

  while (1)
    ;
  while (1)
  {
    if (ROM_GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) == 0x00)
    {
      ui8Adjust--;
      if (ui8Adjust < 56)
      {
        ui8Adjust = 56;
      }
      ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_3, ui8Adjust * ui32Load / 1000);
    }
    if (ROM_GPIOPinRead(GPIO_PORTF_BASE, PWM_PIN) == 0x00)
    {
      ui8Adjust++;
      if (ui8Adjust > 111)
      {
        ui8Adjust = 111;
      }
      ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_3, ui8Adjust * ui32Load / 1000);
    }
    ROM_SysCtlDelay(100000);
  }
}