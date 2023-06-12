#include <asf.h>
#include "Features.h"
#include "osdp.h"
#include "Pwm.h"
#include "Timers.h"
#include "Leds.h"


/******************************************************************************
* Private Data Structures
******************************************************************************/
static uint8_t blinkingLed;
static uint8_t blinkingLedState;
static uint16_t blinkLedOnTime;
static uint16_t blinkLedOffTime;
static bool fRedLedState = LED_INACTIVE;
static bool fGreenLedState = LED_INACTIVE;
static bool fBlueLedState = LED_INACTIVE;
static bool fRedLedSavedState = LED_INACTIVE;
static bool fGreenLedSavedState = LED_INACTIVE;
static bool fBlueLedSavedState = LED_INACTIVE;


/******************************************************************************
* Global Function Definitions
******************************************************************************/
/*****************************************************************************
*
*  Function Name: SetLed
*
*  Inputs:
*    selectedLed
*    ledState - LED_ON or LED_OFF
*
*  Outputs
*    None
*
*  Return Values
*    None
*
*  Description:  This function turns the specified LED on or off.
*
*****************************************************************************/
void SetLed(uint8_t selectedLed, uint8_t ledState)
{
	bool  bLedState;
	uint16_t i;
	uint8_t redDutyCycleFactor, greenDutyCycleFactor, blueDutyCycleFactor;
	
	i = ( SystemFeatures.OpSettings[LED_OPTIONS_1_INDEX] & 
								RED_LED_PWM_DUTY_CYCLE_MASK );
	redDutyCycleFactor = (uint8_t)(i >> 4);
	if (redDutyCycleFactor >= PWM_DUTY_CYCLE_FACTOR_MAX)
	{
		redDutyCycleFactor = PWM_DUTY_CYCLE_FACTOR_MAX;
	}
	if (redDutyCycleFactor < PWM_DUTY_CYCLE_FACTOR_MIN)
	{
		redDutyCycleFactor = PWM_DUTY_CYCLE_FACTOR_MIN;
	}
	
	i = ( SystemFeatures.OpSettings[LED_OPTIONS_1_INDEX] & 
							     GREEN_LED_PWM_DUTY_CYCLE_MASK );
	greenDutyCycleFactor = (uint8_t)(i >> 8);
	if (greenDutyCycleFactor >= PWM_DUTY_CYCLE_FACTOR_MAX)
	{
		greenDutyCycleFactor = PWM_DUTY_CYCLE_FACTOR_MAX;
	}
	if (greenDutyCycleFactor < PWM_DUTY_CYCLE_FACTOR_MIN)
	{
		greenDutyCycleFactor = PWM_DUTY_CYCLE_FACTOR_MIN;
	}

	i = ( SystemFeatures.OpSettings[LED_OPTIONS_1_INDEX] &
								BLUE_LED_PWM_DUTY_CYCLE_MASK );
	blueDutyCycleFactor = (uint8_t)(i >> 12);
	if (blueDutyCycleFactor >= PWM_DUTY_CYCLE_FACTOR_MAX)
	{
		blueDutyCycleFactor = PWM_DUTY_CYCLE_FACTOR_MAX;
	}
	if (blueDutyCycleFactor < PWM_DUTY_CYCLE_FACTOR_MIN)
	{
		blueDutyCycleFactor = PWM_DUTY_CYCLE_FACTOR_MIN;
	}

	if (LED_ON == ledState)
	{
		bLedState = LED_ACTIVE;
	}
	else
	{
		bLedState = LED_INACTIVE;
	}

	if (GREEN_LED & selectedLed)
	{
		if (fGreenLedState != bLedState)
		{
			if (PWM_DUTY_CYCLE_FACTOR_MAX == greenDutyCycleFactor)
			{
				port_pin_set_output_level(GREEN_LED_PIN, bLedState);
			}
			else
			{
				if (LED_ACTIVE == bLedState)
				{
					EnableGreenLedPwm();
				}
				else
				{
					DisableGreenLedPwm();
				}
			}
			fGreenLedState = bLedState;
		}
	}
	
	if (RED_LED & selectedLed)
	{
		if (fRedLedState != bLedState)
		{
			if ( HW_PCB_REV_1_0 == HwPcbRev )
			{
				port_pin_set_output_level(LEGACY_RED_LED_PIN, bLedState);
			}
			if (PWM_DUTY_CYCLE_FACTOR_MAX == redDutyCycleFactor)
			{
				port_pin_set_output_level(RED_LED_PIN, bLedState);
			}
			else
			{
				if (LED_ACTIVE == bLedState)
				{
					EnableRedLedPwm();
				}
				else
				{
					DisableRedLedPwm();
				}
			}
			fRedLedState = bLedState;
		}
	}

	if (BLUE_LED & selectedLed)
	{
		if (fBlueLedState != bLedState)
		{
			if (PWM_DUTY_CYCLE_FACTOR_MAX == blueDutyCycleFactor)
			{
				port_pin_set_output_level(BLUE_LED_PIN, bLedState);
			}
			else
			{
				if (LED_ACTIVE == bLedState)
				{
					EnableBlueLedPwm();
				}
				else
				{
					DisableBlueLedPwm();
				}
			}
			fBlueLedState = bLedState;
		}
	}

}


/*****************************************************************************
*
*  Function Name: SetIdleLed
*
*  Inputs:
*    None
*
*  Outputs
*    None
*
*  Return Values
*    None
*
*  Description:
*
*****************************************************************************/
void SetIdleLed(void)
{
	uint8_t ledSetValue;
	
	//
	// Lot of reasons why this function cannot be serviced.
	//
	if ( (FALSE == OsdpAvIsActive()) &&
		 (FALSE == fLedControlLineActive) &&
		 (0 == BleAmberLedCountDown) &&
		 (0 == BlinkingLedCountDown) &&
		 (0 == PinEntryDurationTimer) &&
		 (0 == (SystemFeatures.OpSettings[OPERATION_INDEX] &
											ENABLE_HOST_CNTL_AV_EXCLUSIVE)) )
	{
		ledSetValue = (uint8_t)(SystemFeatures.OpSettings[LED_OPTIONS_1_INDEX] &
														LED_IDLE_COLOR_MASK);
		if ( 0 != (SystemFeatures.OpSettings[LED_OPTIONS_2_INDEX] & 
														BLUE_LED_IDLE_COLOR_MASK) )
		{
			ledSetValue |= BLUE_LED;
		}

		switch (ledSetValue)
		{
			case RED_LED:
				SetLed(BLUE_LED, LED_OFF);
				SetLed(GREEN_LED, LED_OFF);
				SetLed(RED_LED, LED_ON);
			break;
		
			case GREEN_LED:
				SetLed(BLUE_LED, LED_OFF);
				SetLed(RED_LED, LED_OFF);
				SetLed(GREEN_LED, LED_ON);
			break;
			
			case BLUE_LED:
				SetLed(BLUE_LED, LED_ON);
				SetLed(RED_LED, LED_OFF);
				SetLed(GREEN_LED, LED_OFF);
			break;

			case RED_GREEN_LEDS:
				SetLed(BLUE_LED, LED_OFF);
				SetLed(GREEN_LED, LED_ON);
				SetLed(RED_LED, LED_ON);
			break;

			case RED_BLUE_LEDS:
				SetLed(BLUE_LED, LED_ON);
				SetLed(GREEN_LED, LED_OFF);
				SetLed(RED_LED, LED_ON);
			break;
		
			case GREEN_BLUE_LEDS:
				SetLed(BLUE_LED, LED_ON);
				SetLed(GREEN_LED, LED_ON);
				SetLed(RED_LED, LED_OFF);
			break;
		
			case RED_GREEN_BLUE_LEDS:
				SetLed(BLUE_LED, LED_ON);
				SetLed(GREEN_LED, LED_ON);
				SetLed(RED_LED, LED_ON);
			break;

			default:
				SetLed(BLUE_LED, LED_OFF);
				SetLed(GREEN_LED, LED_OFF);
				SetLed(RED_LED, LED_OFF);
			break;
		}
	}
}


/*****************************************************************************
*
*  Function Name: SetCardActiveLed
*
*  Inputs:
*    None
*
*  Outputs
*    None
*
*  Return Values
*    None
*
*  Description:
*
*****************************************************************************/
void SetCardActiveLed(void)
{
	//
	// Lot of reasons why this function cannot be serviced.
	//
	if ( (FALSE == OsdpAvIsActive()) &&
		 (FALSE == fLedControlLineActive) &&
		 (0 == BleAmberLedCountDown) &&
		 (0 == BlinkingLedCountDown) &&
		 (0 == PinEntryDurationTimer) &&
		 (0 == (SystemFeatures.OpSettings[OPERATION_INDEX] &
								ENABLE_HOST_CNTL_AV_EXCLUSIVE)) )
	{
		SetLed(BLUE_LED, LED_OFF);
		SetLed(GREEN_LED, LED_ON);
		SetLed(RED_LED, LED_ON);
	}
}


/*****************************************************************************
*
*  Function Name: SetPinEntryActiveLed
*
*  Inputs:
*    None
*
*  Outputs
*    None
*
*  Return Values
*    None
*
*  Description:
*
*****************************************************************************/
void SetPinEntryActiveLed(void)
{
	uint8_t selectedLed;
	uint16_t opSettings, blinkTime;
	uint16_t duration;
	
	opSettings = SystemFeatures.OpSettings[LED_OPTIONS_3_INDEX] & 
												PIN_ENTRY_DURATION_MASK;
	if (0 != opSettings)
	{
		//
		// Several reasons why this function cannot be serviced.
		//
		if ( (FALSE == OsdpAvIsActive()) &&
			 (FALSE == fLedControlLineActive) &&
			 (0 == (SystemFeatures.OpSettings[OPERATION_INDEX] &
										ENABLE_HOST_CNTL_AV_EXCLUSIVE)) )
		{
			SetLed(BLUE_LED, LED_OFF);
			SetLed(GREEN_LED, LED_OFF);
			SetLed(RED_LED, LED_OFF);
			opSettings = SystemFeatures.OpSettings[LED_OPTIONS_2_INDEX] & 
														PIN_ENTRY_COLOR_MASK;
			selectedLed = (uint8_t)(opSettings >> 8);
			blinkTime = SystemFeatures.OpSettings[LED_OPTIONS_2_INDEX] & 
													PIN_BLINK_DURATION_MASK;
			blinkTime *= 50;
			duration = (uint32_t)(SystemFeatures.OpSettings[LED_OPTIONS_3_INDEX] & 
														PIN_ENTRY_DURATION_MASK);
			duration *= 1000;
			if (0 != blinkTime)
			{
				StartBlinkingLed(selectedLed, blinkTime,blinkTime);
			}
			else
			{
				SetLed(selectedLed, LED_ON);
			}
			SetPinEntryDurationTimer();
		}
	}
}


/*****************************************************************************
*
*  Function Name: SetPinEntryDurationTimer
*
*  Inputs:
*    None
*
*  Outputs
*    None
*
*  Return Values
*    None
*
*  Description:
*
*****************************************************************************/
void SetPinEntryDurationTimer(void) 
{
	uint16_t duration;
	
	duration = (uint32_t)(SystemFeatures.OpSettings[LED_OPTIONS_3_INDEX] &
													PIN_ENTRY_DURATION_MASK);
	duration *= 1000;
	SetTimerValue(PIN_ENRTY_DURATION_TIMER, duration);
}


/*****************************************************************************
*
*  Function Name: ForceLed
*
*  Inputs:
*    None
*
*  Outputs
*    None
*
*  Return Values
*    None
*
*  Description:
*
*****************************************************************************/
void ForceLed(uint8_t led)
{
	switch (led)
	{
		case RED_LED:
			SetLed(BLUE_LED, LED_OFF);
			SetLed(GREEN_LED, LED_OFF);
			SetLed(RED_LED, LED_ON);
		break;
			
		case GREEN_LED:
			SetLed(BLUE_LED, LED_OFF);
			SetLed(RED_LED, LED_OFF);
			SetLed(GREEN_LED, LED_ON);
		break;
			
		case BLUE_LED:
			SetLed(BLUE_LED, LED_ON);
			SetLed(RED_LED, LED_OFF);
			SetLed(GREEN_LED, LED_OFF);
		break;
			
		case RED_GREEN_LEDS:
			SetLed(BLUE_LED, LED_OFF);
			SetLed(GREEN_LED, LED_ON);
			SetLed(RED_LED, LED_ON);
		break;
			
		case RED_BLUE_LEDS:
			SetLed(BLUE_LED, LED_ON);
			SetLed(GREEN_LED, LED_OFF);
			SetLed(RED_LED, LED_ON);
		break;
			
		case GREEN_BLUE_LEDS:
			SetLed(BLUE_LED, LED_ON);
			SetLed(GREEN_LED, LED_ON);
			SetLed(RED_LED, LED_OFF);
		break;
			
		case RED_GREEN_BLUE_LEDS:
			SetLed(BLUE_LED, LED_ON);
			SetLed(GREEN_LED, LED_ON);
			SetLed(RED_LED, LED_ON);
		break;
		
		default:
			SetLed(BLUE_LED, LED_OFF);
			SetLed(GREEN_LED, LED_OFF);
			SetLed(RED_LED, LED_OFF);
		break;
	}
}


/*****************************************************************************
*
*  Function Name: BlinkLedOn
*
*  Inputs:
*    selectedLed - LED1 .. LED12, or ALL_LEDS
*    offTime - time in milliseconds the led is on
*
*  Outputs
*    None
*
*  Return Values
*    None
*
*  Description:  This function turns the led on for the indicated
*                durations in milliseconds.
*
*****************************************************************************/
void BlinkLedOn(uint8_t selectedLed, uint16_t onTime)
{
	SetLed(selectedLed, LED_ON);
	DelayMs(onTime);
	SetLed(selectedLed, LED_OFF);
}


/*****************************************************************************
*
*  Function Name: BlinkLedOff
*
*  Inputs:
*    selectedLed - LED1 .. LED12, or ALL_LEDS
*    offTime - time in milliseconds the led is off
*
*  Outputs
*    None
*
*  Return Values
*    None
*
*  Description:  This function turns the led off for the indicated
*                durations in milliseconds.
*
*****************************************************************************/
void BlinkLedOff(uint8_t selectedLed, uint16_t offTime)
{
	SetLed(selectedLed, LED_OFF);
	DelayMs(offTime);
	SetLed(selectedLed, LED_ON);
}


/*****************************************************************************
*
*  Function Name: BlinkLed
*
*  Inputs:
*    selectedLed - LED1 .. LED12, or ALL_LEDS
*    onTime - time in milliseconds the led is on
*    offTime - time in milliseconds the led is off
*
*  Outputs
*    None
*
*  Return Values
*    None
*
*  Description:  This function turns the led on for the indicated
*                durations in milliseconds then leaves it off for
*                the indicated durations in milliseconds.
*
*****************************************************************************/
void BlinkLed(uint8_t selectedLed, 
              uint8_t numBlinks,
			  uint16_t onTime, 
			  uint16_t offTime)
{
	while (numBlinks--)
	{
		SetLed(selectedLed, LED_ON);
		DelayMs(onTime);
		SetLed(selectedLed, LED_OFF);
		DelayMs(offTime);
	}
}


/*****************************************************************************
*
*  Function Name: StartBlinkingLed
*
*  Inputs:
*    None
*
*  Outputs
*    None
*
*  Return Values
*    None
*
*  Description:  
*
*****************************************************************************/
void StartBlinkingLed(uint8_t selectedLed, uint16_t onTime, uint16_t offTime)
{
	blinkingLed = selectedLed;
	blinkingLedState = LED_OFF;
	blinkLedOnTime = onTime;
	blinkLedOffTime = offTime;
	SetLed(RED_GREEN_BLUE_LEDS, LED_OFF);
	ProcessBlinkingLed();
}


/*****************************************************************************
*
*  Function Name: StopBlinkingLed
*
*  Inputs:
*    None
*
*  Outputs
*    None
*
*  Return Values
*    None
*
*  Description:  
*
*****************************************************************************/
void StopBlinkingLed(void)
{
	SetTimerValue(BLINKING_LED_COUNT_DOWN_TIMER, 0);
	SetTimerValue(PIN_ENRTY_DURATION_TIMER, 0);
	SetLed(RED_GREEN_BLUE_LEDS, LED_OFF);
}


/*****************************************************************************
*
*  Function Name: StartBleConnectionAv
*
*  Inputs:
*    None
*
*  Outputs
*    None
*
*  Return Values
*    None
*
*  Description:  
*
*****************************************************************************/
void StartBleConnectionAv(void)
{
	uint16_t i;
	uint8_t selectedLed;

	//
	// Set the LED color (default is amber).
	//
	i = SystemFeatures.OpSettings[BUILD_INDEX]  
												& FW_BLE_AV_COLOR_MASK;
	selectedLed = (uint8_t)(i >> 8);
	if (0 == selectedLed)
	{
		selectedLed = RED_GREEN_LEDS;
	}
	
	//
	// If the LED is supposed to blink, ...
	//
	if ( 0 != (SystemFeatures.OpSettings[BUILD_INDEX]  
												& FW_BLE_AV_ACTION_MASK) )
	{
		blinkingLed = selectedLed;
		blinkingLedState = LED_OFF;
		blinkLedOnTime = SystemFeatures.OpSettings[BUILD_INDEX]  
												& FW_BLE_AV_PERIOD_MASK;
		blinkLedOnTime >>= 12;
		blinkLedOnTime *= 50;
		blinkLedOffTime = blinkLedOnTime;
		StartBlinkingLed(selectedLed, blinkLedOnTime, blinkLedOffTime);
	}
	else // Otherwise, the LED stays on solid
	{
		SetLed(selectedLed, LED_ON);
	}
}


/*****************************************************************************
*
*  Function Name: ProcessBlinkingLed
*
*  Inputs:
*    None
*
*  Outputs
*    None
*
*  Return Values
*    None
*
*  Description:  
*
*****************************************************************************/
void ProcessBlinkingLed(void)
{
	if (LED_OFF == blinkingLedState)
	{
		blinkingLedState = LED_ON;
		SetTimerValue(BLINKING_LED_COUNT_DOWN_TIMER, blinkLedOnTime);
	}
	else
	{
		blinkingLedState = LED_OFF;
		SetTimerValue(BLINKING_LED_COUNT_DOWN_TIMER, blinkLedOffTime);
	}

	if (ALT_RED_GREEN_LEDS == blinkingLed)
	{
		if (LED_ON == blinkingLedState)
		{
			SetLed(RED_LED, LED_OFF);
			SetLed(GREEN_LED, LED_ON);
		}
		else
		{
			SetLed(RED_LED, LED_ON);
			SetLed(GREEN_LED, LED_OFF);
		}
	}
	else
	{
		SetLed(blinkingLed, blinkingLedState);
	}
}


/*****************************************************************************
*
*  Function Name: SaveLedStates
*
*  Inputs:
*    None
*
*  Outputs
*    None
*
*  Return Values
*    None
*
*  Description:
*
*****************************************************************************/
void SaveLedStates(void)
{
	fRedLedSavedState = fRedLedState;
	fGreenLedSavedState = fGreenLedState;
	fBlueLedSavedState = fBlueLedState;
}


/*****************************************************************************
*
*  Function Name: RecoverLedStates
*
*  Inputs:
*    None
*
*  Outputs
*    None
*
*  Return Values
*    None
*
*  Description:
*
*****************************************************************************/
void RecoverLedStates(void)
{
	if (LED_ACTIVE == fRedLedSavedState)
	{
		SetLed(RED_LED, LED_ON);
	}
	else
	{
		SetLed(RED_LED, LED_OFF);
	}
	
	if (LED_ACTIVE == fGreenLedSavedState)
	{
		SetLed(GREEN_LED, LED_ON);
	}
	else
	{
		SetLed(GREEN_LED, LED_OFF);
	}
	
	if (LED_ACTIVE == fBlueLedSavedState)
	{
		SetLed(BLUE_LED, LED_ON);
	}
	else
	{
		SetLed(BLUE_LED, LED_OFF);
	}
}

