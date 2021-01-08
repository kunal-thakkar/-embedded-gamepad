/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */

#include "stm8s.h"
#include "stm8s_delay.h"

#define LED_PORT			GPIOB
#define LED_PIN				GPIO_PIN_5
#define TX_PORT				GPIOD
#define TX_PIN				GPIO_PIN_5
// The data pin for the NES controller
#define NES_DATA_PORT GPIOA
#define NES_DATA_PIN	GPIO_PIN_2
//Outputs:
#define NES_CLK_PORT	GPIOD
#define NES_CLK_PIN		GPIO_PIN_6
// The latch pin for the NES controller
#define NES_LAT_PORT	GPIOA
#define NES_LAT_PIN		GPIO_PIN_1
	
void clock_setup(void);
void GPIO_setup(void);
void UART1_setup(void);
void TX_data(uint8_t bt1, uint8_t bt2);
uint8_t *readNesController(void);
uint16_t mapping[8] = {
	0x0100, //A 			=> button 1 (A)
	0x0200, //B 			=> button 2	(B)
	0x0004, //Select	=> button 12 Select
	0x0008, //Start		=> button 13 Start
	0x4000, //Up			=> button 7	 L1
	0x8000, //Down		=> button 9  R1
	0x0001, //Left		=> button 8	 L2
	0x0002	//Right		=> button 10 R2
};

main() {
	uint8_t _bt[2] = {0x00, 0x00};
	clock_setup();
	GPIO_setup();
	UART1_setup();

	while(TRUE) {
		uint8_t *bt = readNesController();
		if(bt[0] != _bt[0] || bt[1] != _bt[1]) {
			TX_data(bt[0], bt[1]);
			_bt[0] = bt[0];
			_bt[1] = bt[1];
		}
	}
}

void clock_setup(void){
	CLK_DeInit();
	
	CLK_HSECmd(DISABLE);
	CLK_LSICmd(DISABLE);
	CLK_HSICmd(ENABLE);
	while(CLK_GetFlagStatus(CLK_FLAG_HSIRDY) == FALSE);
	
	CLK_ClockSwitchCmd(ENABLE);
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
	CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
	
	CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, 
	DISABLE, CLK_CURRENTCLOCKSTATE_ENABLE);
	
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_ADC, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_AWU, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1, ENABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER1, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, DISABLE);
}

void GPIO_setup(void) { 
	GPIO_DeInit(LED_PORT); 
	GPIO_DeInit(TX_PORT); 
	GPIO_DeInit(NES_DATA_PORT);	
	GPIO_DeInit(NES_CLK_PORT);	
	GPIO_DeInit(NES_LAT_PORT);	

	GPIO_Init(LED_PORT, LED_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_Init(TX_PORT, TX_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_Init(NES_DATA_PORT, NES_DATA_PIN, GPIO_MODE_IN_PU_IT);
	GPIO_Init(NES_CLK_PORT, NES_CLK_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);	
	GPIO_Init(NES_LAT_PORT, NES_LAT_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
	
	GPIO_WriteHigh(LED_PORT, LED_PIN);
	GPIO_WriteLow(NES_CLK_PORT, NES_CLK_PIN);
	GPIO_WriteLow(NES_LAT_PORT, NES_LAT_PIN);
}

void UART1_setup(void){
	UART1_DeInit();
	UART1_Init(115200, 
		UART1_WORDLENGTH_8D, 
		UART1_STOPBITS_1, 
		UART1_PARITY_NO, 
		UART1_SYNCMODE_CLOCK_DISABLE, 
		UART1_MODE_TX_ENABLE);
	UART1_Cmd(ENABLE);
}

void TX_data(uint8_t bt1, uint8_t bt2) {
	uint8_t data[8] = {0xFD, 0x06, 0x00, 0x00, 0x00, 0x00, bt1, bt2};
	uint8_t i;
	GPIO_WriteLow(LED_PORT, LED_PIN);
	for(i = 0; i < 8; i++){
		while(UART1_GetFlagStatus(UART1_FLAG_TXE) == FALSE);
		UART1_SendData8(data[i]);
	}
	GPIO_WriteHigh(LED_PORT, LED_PIN);
	delay_ms(10);
}

uint8_t *readNesController() {
	uint8_t i;
	uint8_t data[2] = {0x00, 0x00};
	uint16_t rawData = 0x0000;
	//Quickly pulse the nesLatch pin so that the register grab
	//what it see on its parallel data pins.
	GPIO_WriteHigh(NES_LAT_PORT, NES_LAT_PIN);
	GPIO_WriteLow(NES_LAT_PORT, NES_LAT_PIN);

  /** Read data bit by bit from SR **/
  for (i = 0; i < 8; i++) {
		GPIO_WriteLow(NES_CLK_PORT, NES_CLK_PIN);
		if (GPIO_ReadInputPin(NES_DATA_PORT, NES_DATA_PIN) == FALSE)
			rawData |= mapping[i];
		GPIO_WriteHigh(NES_CLK_PORT, NES_CLK_PIN);
  }
	if(rawData > 0){
		data[0] = rawData >> 8;
		data[1] = rawData & 0xFF;
	}
	return data;
}