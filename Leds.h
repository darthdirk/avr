#ifndef LEDS_H_
#define LEDS_H_

#define LED_OFF   0
#define LED_ON    1

#define BLACK_LED             0
#define RED_LED               1
#define GREEN_LED             2
#define RED2_LEDS             3
#define GREEN2_LEDS           4
#define GREEN3_LEDS           5

/******************************************************************************
* Global Function Prototypes
******************************************************************************/
void SetLed(uint8_t selectedLed, uint8_t ledState);
void SetIdleLed(void);
void SetButtonActiveLed(void);
void SetButton2ActiveLed(void);
void SetLedDurationTimer(void);
void ForceLed(uint8_t led);
void BlinkLedOn(uint8_t selectedLed, uint16_t onTime);
void BlinkLedOff(uint8_t selectedLed, uint16_t offTime);
void BlinkLed(uint8_t selectedLed,
              uint8_t numBlinks,
			  uint16_t onTime,
			  uint16_t offTime);
void StartBlinkingLed(uint8_t selectedLed, uint16_t onTime, uint16_t offTime);
void StopBlinkingLed(void);
void ProcessBlinkingLed(void);
void SaveLedStates(void);
void RecoverLedStates(void);

#endif /* LEDS_H_ */

