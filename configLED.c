#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */

void PORTA_IRQHandler()
{
	static uint8_t state = 0;
	PORT_ClearPinsInterruptFlags(PORTA, 1<<4);

	GPIO_WritePinOutput(GPIOB,21,state);
	state = ( 0 == state ) ? 1 : 0;
}

int main(void)
{

	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
	/* Init FSL debug console. */
	BOARD_InitDebugConsole();

	CLOCK_EnableClock(kCLOCK_PortB);
	CLOCK_EnableClock(kCLOCK_PortA);

	port_pin_config_t config_led =
	{ kPORT_PullDisable, kPORT_SlowSlewRate, kPORT_PassiveFilterDisable,
			kPORT_OpenDrainDisable, kPORT_LowDriveStrength, kPORT_MuxAsGpio,
			kPORT_UnlockRegister, };

	PORT_SetPinConfig(PORTB, 21, &config_led);


	port_pin_config_t config_switch =
	{ kPORT_PullDisable, kPORT_SlowSlewRate, kPORT_PassiveFilterDisable,
			kPORT_OpenDrainDisable, kPORT_LowDriveStrength, kPORT_MuxAsGpio,
			kPORT_UnlockRegister};
	PORT_SetPinInterruptConfig(PORTA, 4, kPORT_InterruptFallingEdge);

	PORT_SetPinConfig(PORTA, 4, &config_switch);

	gpio_pin_config_t led_config_gpio =
	{ kGPIO_DigitalOutput, 1 };

	GPIO_PinInit(GPIOB, 21, &led_config_gpio);

	gpio_pin_config_t switch_config_gpio =
	{ kGPIO_DigitalInput, 1 };

	GPIO_PinInit(GPIOA, 4, &switch_config_gpio);

	NVIC_EnableIRQ(PORTA_IRQn);

	/* Force the counter to be placed into memory. */
	volatile static int i = 0;
	/* Enter an infinite loop, just incrementing a counter. */

	PRINTF("Hola mundo!");

	GPIO_WritePinOutput(GPIOB,21,0);

	while (1)
	{
		i++;
	}
	return 0;
}
