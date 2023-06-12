#ifndef PWM_H_
#define PWM_H_


//
// Red LED PWM components  PIN_PB17
//
#define RED_LED_PWM_MODULE        TC6
#define RED_LED_PWM_OUT_PIN       PIN_PB17E_TC6_WO1
#define RED_LED_PWM_OUT_MUX       MUX_PB17E_TC6_WO1


//
// Green LED PWM components  PIN_PB23
//
#define GREEN_LED_PWM_MODULE        TC7
#define GREEN_LED_PWM_OUT_PIN       PIN_PB23F_TC7_WO1
#define GREEN_LED_PWM_OUT_MUX       MUX_PB23F_TC7_WO1


//
// Red2 LED PWM components  PIN_PB11
//
#define RED2_LED_PWM_MODULE        TC5
#define RED2_LED_PWM_OUT_PIN       PIN_PB11F_TC5_WO1
#define RED2_LED_PWM_OUT_MUX       MUX_PB11F_TC5_WO1


//
// Duty Cycle Register Settings
//
#define PWM_TOP_VALUE               200
#define PWM_DUTY_CYCLE_FACTOR_MAX   10
#define PWM_DUTY_CYCLE_FACTOR_MIN   1



/******************************************************************************
* Global Function Prototypes
******************************************************************************/
void SetupRedLedPwm(void);
void SetupRed2LedPwm(void);
void SetupGreenLedPwm(void);
void SetupGreen2LedPwm(void);
void SetupGreen3LedPwm(void);
void EnableRedLedPwm(void);
void EnableRed2LedPwm(void);
void EnableGreenLedPwm(void);
void EnableGreen2LedPwm(void);
void EnableGreen3LedPwm(void);
void DisableRedLedPwm(void);
void DisableRed2LedPwm(void);
void DisableGreenLedPwm(void);
void DisableGreen2LedPwm(void);
void DisableGreen3LedPwm(void);

#endif /* PWM_H_ */
