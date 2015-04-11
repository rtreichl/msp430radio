#include <driver/encoder.h>

#ifdef ENCODER_H_

#ifdef ENCODER_1

//Variables from Encoder 1
volatile uint16_t encoder_1_count = 0;							//Shows the actual value from Encoder 1
volatile int8_t encoder_1_last = 0;									//Last state from Encoder 1
volatile uint8_t encoder_1_button = 'f';									//Button Value for Menu
volatile int8_t encoder_1_state = 0;									//Button Value for State mashine in Encoder
volatile int16_t count_button_1 = 0;									//

#endif

#ifdef ENCODER_2

//Variables from Encoder 2
volatile uint16_t encoder_2_count = 0;							//Shows the actual value from Encoder 2
volatile int8_t encoder_2_last = 0;									//Last state from Encoder 2
volatile uint8_t encoder_2_button = 'f';
volatile int8_t encoder_2_state = 0;
volatile int16_t count_button_2 = 0;

#endif

#ifdef ENCODER_1

int16_t encoder_1_update(int8_t *count, uint8_t **button)
{
	static uint16_t tmp_count_1 = 0x8000;						//Set a start Value in the Middle of Range
	uint16_t tmp_en = encoder_1_count;							//Save encoder_2_count if there is a break form a Interrupt
	uint16_t tmp_t = tmp_count_1/4;								//Calculate number of Steps
	uint16_t tmp_en_t = tmp_en/4;								//Calculate number of Steps
	if((tmp_en%4) == 3)												//Check if Encoder is near the next full Rast point
	{
		tmp_en_t++;													//if true up Value for Steps of act. Value
	}
	if((tmp_count_1%4) == 3)										//Check if last Value of Encoder was near the next full Rast point
	{
		tmp_t++;													//if true up Value for Steps of last Value
	}
	int16_t diff = tmp_en_t - tmp_t;									//make the difference of act. and last Value
	tmp_count_1 = tmp_en;											//save act. Value as last Value
	*count += diff;
	*button = (uint8_t *)&encoder_1_button;
	return 0;													//give difference back
}

void Encoder_1_decoder(void)
{
	int8_t encoder_1_new = 0, encoder_1_diff = 0;
	if((EN1_A_IN & EN1_A_PIN) != 0)							//Check Signal A for a High Value
	{
		encoder_1_new = 3;											//Decode Graycode new = Bit1 := A and Bit0 := A^B


	}
	if((EN1_B_IN & EN1_B_PIN) != 0)							//Check Signal A for a High Value
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
	encoder_1_button = 'f';
	encoder_1_state = 0;
	EN1_A_IE &= ~(EN1_A_PIN);					//Disable Interrupt on all listed Pins
	EN1_B_IE &= ~(EN1_B_PIN);
	EN1_A_DIR &= ~(EN1_A_PIN);	//Set Signal A, B, Button as Input
	EN1_B_DIR &= ~(EN1_B_PIN);
	EN1_TAST_DIR &= ~(EN1_TAST_PIN);
	EN1_TAST_REN  |=  EN1_TAST_PIN;									//Enable Pullup/Pulldown on Button Pin
	EN1_TAST_OUT |=  EN1_TAST_PIN;									//Set Pullup Resistor on Button Pin
	EN1_A_OUT &= ~(EN1_A_PIN);
	EN1_B_OUT &= ~(EN1_B_PIN);
	if((EN1_A_IN & EN1_A_PIN) != 0)
	{
		encoder_1_last = 3;											//Decode Graycode new = Bit1 := A and Bit0 := A^B
		EN1_A_IES |=  EN1_A_PIN;								//Interrupt for a falling edge for Signal A

	}
	if((EN1_B_IN & EN1_B_PIN) != 0)
	{
		encoder_1_last ^= 1;										//Decode Graycode new = Bit1 := A and Bit0 := A^B
		EN1_B_IES |=  EN1_B_PIN;								//Interrupt for a falling edge for Signal B
	}
	/*EN1_A_IFG &= ~(EN1_A_PIN);									//Clear Interrupt Flag before enable Interrupt on all listed Pins
	EN1_B_IFG &= ~(EN1_B_PIN);
	EN1_A_IE |= EN1_A_PIN;										//Enable Interrupt on all listed Pins
	EN1_B_IE |= EN1_B_PIN;*/
	ext_interrupt_create(EN1_A_INT, encoder_interrupt);
	ext_interrupt_create(EN1_B_INT, encoder_interrupt);
	ext_interrupt_enable(EN1_A_INT);
	ext_interrupt_enable(EN1_B_INT);
	__bis_SR_register(GIE);
}

#endif	//ENCODER_1

#ifdef ENCODER_2

int16_t encoder_2_update(int8_t *count, uint8_t **button)
{
	static uint16_t tmp_count_2 = 0x8000;						//Set a start Value in the Middle of Range
	uint16_t tmp_en = encoder_2_count;							//Save encoder_2_count if there is a break form a Interrupt
	uint16_t tmp_t = tmp_count_2/4;								//Calculate number of Steps
	uint16_t tmp_en_t = tmp_en/4;								//Calculate number of Steps
	if((tmp_en%4) == 3)												//Check if Encoder is near the next full Rast point
	{
		tmp_en_t++;													//if true up Value for Steps of act. Value
	}
	if((tmp_count_2%4) == 3)										//Check if last Value of Encoder was near the next full Rast point
	{
		tmp_t++;													//if true up Value for Steps of last Value
	}
	int16_t diff = tmp_en_t - tmp_t;									//make the difference of act. and last Value
	tmp_count_2 = tmp_en;											//save act. Value as last Value
	*count += diff;
	*button = (uint8_t *)&encoder_2_button;
	return 0;													//give difference back
}

void Encoder_2_decoder(void)
{
	int8_t encoder_2_new = 0, encoder_2_diff = 0;
	if((EN2_A_IN & EN2_A_PIN) != 0)							//Check Signal A for a High Value
	{
		encoder_2_new = 3;											//Decode Graycode new = Bit1 := A and Bit0 := A^B

	}
	if((EN2_B_IN & EN2_B_PIN) != 0)							//Check Signal A for a High Value
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
	encoder_2_button = 'f';
	encoder_2_state = 0;
	EN2_A_IE &= ~(EN2_A_PIN);					//Disable Interrupt on all listed Pins
	EN2_B_IE &= ~(EN2_B_PIN);
	EN2_A_DIR &= ~(EN2_A_PIN);	//Set Signal A, B, Button as Input
	EN2_B_DIR &= ~(EN2_B_PIN);
	EN2_TAST_DIR &= ~(EN2_TAST_PIN);
	EN2_TAST_REN  |=  EN2_TAST_PIN;									//Enable Pullup/Pulldown on Button Pin
	EN2_TAST_OUT |=  EN2_TAST_PIN;									//Set Pullup Resistor on Button Pin
	EN2_A_OUT &= ~(EN2_A_PIN);
	EN2_B_OUT &= ~(EN2_B_PIN);
	if((EN2_A_IN & EN2_A_PIN) != 0)
	{
		encoder_2_last = 3;											//Decode Graycode new = Bit2 := A and Bit0 := A^B
		EN2_A_IES |=  EN2_A_PIN;								//Interrupt for a falling edge for Signal A

	}
	if((EN2_B_IN & EN2_B_PIN) != 0)
	{
		encoder_2_last ^= 1;										//Decode Graycode new = Bit2 := A and Bit0 := A^B
		EN2_B_IES |=  EN2_B_PIN;								//Interrupt for a falling edge for Signal B
	}
	/*EN2_A_IFG &= ~(EN2_A_PIN);									//Clear Interrupt Flag before enable Interrupt on all listed Pins
	EN2_B_IFG &= ~(EN2_B_PIN);
	EN2_A_IE |= EN2_A_PIN;										//Enable Interrupt on all listed Pins
	EN2_B_IE |= EN2_B_PIN;*/
	ext_interrupt_create(EN2_A_INT, encoder_interrupt);
	ext_interrupt_create(EN2_B_INT, encoder_interrupt);
	ext_interrupt_enable(EN2_A_INT);
	ext_interrupt_enable(EN2_B_INT);
	__bis_SR_register(GIE);
}

#endif	//ENCODER_2

//#pragma vector=TIMER0_A0_VECTOR
//__interrupt void Timer_A0(void)
void encoder_interrupt2(void)
{
	if((EN1_TAST_IN & EN1_TAST_PIN) == 0)
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
         if((EN2_TAST_IN & EN2_TAST_PIN) == 0)
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

void encoder_interrupt(void)
{
	#ifdef ENCODER_1
		if((EN1_A_IFG & EN1_A_PIN) || (EN1_A_IFG & EN1_B_PIN))
			Encoder_1_decoder();
		EN1_A_IES ^= (EN1_A_IFG & EN1_A_PIN);
		EN1_B_IES ^= (EN1_B_IFG & EN1_B_PIN);
	#endif
	#ifdef ENCODER_2
		if((EN2_A_IFG & EN2_A_PIN) || (EN2_A_IFG & EN2_B_PIN))
			Encoder_2_decoder();
		EN2_A_IES ^= (EN2_A_IFG & EN2_A_PIN);
		EN2_B_IES ^= (EN2_B_IFG & EN2_B_PIN);
	#endif
}

#endif //ENCODER_H_
