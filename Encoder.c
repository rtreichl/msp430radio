#include <msp430.h>
#include "Encoder.h"

#ifdef ENCODER_H_

#ifdef ENCODER_1

//Variables from Encoder 1
volatile unsigned int encoder_1_count = 0;							//Shows the actual value from Encoder 1
volatile char encoder_1_last = 0;									//Last state from Encoder 1
volatile char encoder_1_button = 0;									//Button Value for Menu
volatile char encoder_1_state = 0;									//Button Value for State mashine in Encoder
volatile int count_button_1 = 0;									//

#endif

#ifdef ENCODER_2

//Variables from Encoder 2
volatile unsigned int encoder_2_count = 0;							//Shows the actual value from Encoder 2
volatile char encoder_2_last = 0;									//Last state from Encoder 2
volatile char encoder_2_button = 0;
volatile char encoder_2_state = 0;
volatile int count_button_2 = 0;

#endif

void Encoder_Timer_init()
{
		TA0CCTL0 |= CCIE;											//Enable Compare Interrupt
        TA0CCR0 = 500;												//Compare Value 500
        TA0CTL = TASSEL_2 | ID0 | ID1;           					//ACLK, up to TCCR0, interrupt
        TA0CTL |= MC_1;												//Enable Timer in up Mode
}

#ifdef ENCODER_1

int Encoder_1_get_count()
{
	static unsigned int tmp_count_1 = 0x8000;						//Set a start Value in the Middle of Range
	unsigned int tmp_en = encoder_1_count;							//Save encoder_2_count if there is a break form a Interrupt
	unsigned int tmp_t = tmp_count_1/4;								//Calculate number of Steps
	unsigned int tmp_en_t = tmp_en/4;								//Calculate number of Steps
	if((tmp_en%4) == 3)												//Check if Encoder is near the next full Rast point
	{
		tmp_en_t++;													//if true up Value for Steps of act. Value
	}
	if((tmp_count_1%4) == 3)										//Check if last Value of Encoder was near the next full Rast point
	{
		tmp_t++;													//if true up Value for Steps of last Value
	}
	int diff = tmp_en_t - tmp_t;									//make the difference of act. and last Value
	tmp_count_1 = tmp_en;											//save act. Value as last Value
	return diff;													//give difference back
}

void Encoder_1_decoder(void)
{
	char encoder_1_new = 0, encoder_1_diff = 0;
	if((ENCODER_1_PIN & ENCODER_1_A) != 0)							//Check Signal A for a High Value
	{
		encoder_1_new = 3;											//Decode Graycode new = Bit1 := A and Bit0 := A^B


	}
	if((ENCODER_1_PIN & ENCODER_1_B) != 0)							//Check Signal A for a High Value
	{
		encoder_1_new ^= 1;											//Decode Graycode new = Bit1 := A and Bit0 := A^B

	}
	encoder_1_diff = encoder_1_last - encoder_1_new;				//The difference of last and new can only be -3, -1, 1, 3
	if(encoder_1_diff & 1 == 1)										//Check difference if an Error occurred. Bit0 is always 1
	{
		encoder_1_last = encoder_1_new;								//Save the new Value form Encoder to last
#if ENCODER_1_ROTARY == 0											//Rotary direction can be set to right(0) or left(1) this option only demand the count direction
		encoder_1_count -= ((encoder_1_diff & 2) - 1);				//the difference is for right rotary 1 1 1 -3 and for left -1 -1 -1 3 the bit mask will force the second bit
#else
		encoder_1_count += ((encoder_1_diff & 2) - 1);				//the difference is for left rotary 1 1 1 -3 and for right -1 -1 -1 3 the bit mask will force the second bit
#endif
	}
}

void Encoder_1_init(void)
{
	encoder_1_last = 0;
	encoder_1_count = 0x8000;
	encoder_1_button = 0;
	encoder_1_state = 0;
	ENCODER_1_IE  &=  ~(ENCODER_1_A | ENCODER_1_B);					//Disable Interrupt on all listed Pins
	ENCODER_1_DIR &= ~(ENCODER_1_A | ENCODER_1_B | ENCODER_1_T);	//Set Signal A, B, Button as Input
	ENCODER_1_REN  |=  ENCODER_1_T;									//Enable Pullup/Pulldown on Button Pin
	ENCODER_1_OUT |=  ENCODER_1_T;									//Set Pullup Resistor on Button Pin
	ENCODER_1_OUT &=  ~(ENCODER_1_A | ENCODER_1_B);
	if((ENCODER_1_PIN & ENCODER_1_A) != 0)
	{
		encoder_1_last = 3;											//Decode Graycode new = Bit1 := A and Bit0 := A^B
		ENCODER_1_IES |=  ENCODER_1_A;								//Interrupt for a falling edge for Signal A

	}
	if((ENCODER_1_PIN & ENCODER_1_B) != 0)
	{
		encoder_1_last ^= 1;										//Decode Graycode new = Bit1 := A and Bit0 := A^B
		ENCODER_1_IES |=  ENCODER_1_B;								//Interrupt for a falling edge for Signal B
	}
	ENCODER_1_IFG &=~ (ENCODER_1_A | ENCODER_1_B);					//Clear Interrupt Flag before enable Interrupt on all listed Pins
	ENCODER_1_IE  |=  (ENCODER_1_A | ENCODER_1_B);					//Enable Interrupt on all listed Pins
	__bis_SR_register(GIE);
}

#endif	//ENCODER_1

#ifdef ENCODER_2

int Encoder_2_get_count()
{
	static unsigned int tmp_count_2 = 0x8000;						//Set a start Value in the Middle of Range
	unsigned int tmp_en = encoder_2_count;							//Save encoder_2_count if there is a break form a Interrupt
	unsigned int tmp_t = tmp_count_2/4;								//Calculate number of Steps
	unsigned int tmp_en_t = tmp_en/4;								//Calculate number of Steps
	if((tmp_en%4) == 3)												//Check if Encoder is near the next full Rast point
	{
		tmp_en_t++;													//if true up Value for Steps of act. Value
	}
	if((tmp_count_2%4) == 3)										//Check if last Value of Encoder was near the next full Rast point
	{
		tmp_t++;													//if true up Value for Steps of last Value
	}
	int diff = tmp_en_t - tmp_t;									//make the difference of act. and last Value
	tmp_count_2 = tmp_en;											//save act. Value as last Value
	return diff;													//give difference back
}

void Encoder_2_decoder(void)
{
	char encoder_2_new = 0, encoder_2_diff = 0;
	if((ENCODER_2_PIN & ENCODER_2_A) != 0)							//Check Signal A for a High Value
	{
		encoder_2_new = 3;											//Decode Graycode new = Bit1 := A and Bit0 := A^B

	}
	if((ENCODER_2_PIN & ENCODER_2_B) != 0)							//Check Signal A for a High Value
	{
		encoder_2_new ^= 1;											//Decode Graycode new = Bit1 := A and Bit0 := A^B
	}
	encoder_2_diff = encoder_2_last - encoder_2_new;				//The difference of last and new can only be -3, -1, 1, 3
	if(encoder_2_diff & 1 == 1)										//Check difference if an Error occurred. Bit0 is always 1
	{
		encoder_2_last = encoder_2_new;								//Save the new Value form Encoder to last
#if ENCODER_2_ROTARY == 0											//Rotary direction can be set to right(0) or left(1) this option only demand the count direction
		encoder_2_count -= ((encoder_2_diff & 2) - 1);				//the difference is for right rotary  1 1 1 -3 and for left -1 -1 -1 3 the bit mask will force the second bit
#else
		encoder_2_count += ((encoder_2_diff & 2) - 1);				//the difference is for left rotary 1 1 1 -3 and for right -1 -1 -1 3 the bit mask will force the second bit
#endif
	}
}

void Encoder_2_init(void)
{
	encoder_2_last = 0;
	encoder_2_count = 0x8000;
	encoder_2_button = 0;
	encoder_2_state = 0;
	ENCODER_2_IE  &=  ~(ENCODER_2_A | ENCODER_2_B);	//Disable Interrupt on all listed Pins
	ENCODER_2_DIR &= ~(ENCODER_2_A | ENCODER_2_B | ENCODER_2_T);	//Set Signal A, B, Button as Input
	ENCODER_2_REN  |=  ENCODER_2_T;									//Enable Pullup/Pulldown on Button Pin
	ENCODER_2_OUT |=  ENCODER_2_T;									//Set Pullup Resistor on Button Pin
	ENCODER_2_OUT &=  ~(ENCODER_2_A | ENCODER_2_B);
	if((ENCODER_2_PIN & ENCODER_2_A) != 0)
	{
		encoder_2_last = 3;											//Decode Graycode new = Bit1 := A and Bit0 := A^B
		ENCODER_2_IES |=  ENCODER_2_A;								//Interrupt for a falling edge for Signal A

	}
	if((ENCODER_2_PIN & ENCODER_2_B) != 0)
	{
		encoder_2_last ^= 1;										//Decode Graycode new = Bit1 := A and Bit0 := A^B
		ENCODER_2_IES |=  ENCODER_2_B;								//Interrupt for a falling edge for Signal B
	}
	ENCODER_2_IFG &=~ (ENCODER_2_A | ENCODER_2_B);					//Clear Interrupt Flag befor enable Interrupt on all listed Pins
	ENCODER_2_IE  |=  (ENCODER_2_A | ENCODER_2_B);					//Enable Interrupt on all listed Pins
	__bis_SR_register(GIE);
}

#endif	//ENCODER_2

#pragma vector = PORT1_VECTOR
__interrupt void Port1_interrupts(void)
{
	if(P1IFG&BIT5)
	{
		if((P1IN&BIT5)==0)
		{
			_BIC_SR(LPM0_EXIT);
			P1IFG&=~BIT5;
		}
	}


#ifdef ENCODER_1
	if(ENCODER_1_IFG & (ENCODER_1_A | ENCODER_1_B))
		Encoder_1_decoder();
	ENCODER_1_IES ^=  ENCODER_1_IFG;
	ENCODER_1_IFG &=~ (ENCODER_1_A | ENCODER_1_B);
#endif
}

#pragma vector = PORT2_VECTOR
__interrupt void Port2_interrupts(void)
{
#ifdef ENCODER_2
	if(ENCODER_2_IFG & (ENCODER_2_A | ENCODER_2_B))
		Encoder_2_decoder();
	ENCODER_2_IES ^=  ENCODER_2_IFG;
	ENCODER_2_IFG &=~ (ENCODER_2_A | ENCODER_2_B);
#endif
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A1(void)
{
	if((ENCODER_1_PIN & ENCODER_1_T) == 0)
         {
        	 if(count_button_1++ >= TIME_FOR_LONG_PRESS)
        	 {
        	 	 encoder_1_button = 'l';
        	 	 encoder_1_state = 'l';
        	 }
         }
         else
         {
        	 if(count_button_1 != 0 && count_button_1 < TIME_FOR_LONG_PRESS)
        	 {
        		 encoder_1_button = 'k';
        		 encoder_1_state = 'k';
        	 }
        	 count_button_1 = 0;
         }
         if((ENCODER_2_PIN & ENCODER_2_T) == 0)
         {
        	 if(count_button_2++ >= TIME_FOR_LONG_PRESS)
        	 {
        		 encoder_2_button = 'l';
        		 encoder_2_state = 'l';
        	 }
         }
         else
         {
        	 if(count_button_2 != 0 && count_button_2 < TIME_FOR_LONG_PRESS)
        	 {
        		 encoder_2_button = 'k';
        		 encoder_2_state = 'k';
        	 }
        	 count_button_2 = 0;
         }

}

#endif //ENCODER_H_
