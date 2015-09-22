#include <driver/i2c.h>

/**
 * @struct I2C_CTRL
 * @brief This struct inclued all necessaries for I2C module.
 */

struct I2C_CTRL {
	int8_t endian;			///< Stores -1 for a little endian system on slave side and a 1 for a big endian system
	uint8_t : 5;
	uint8_t rept_start : 1;		///< Stores the value, if an repated start or stop will be send after performance on I2C.
	uint8_t status : 2;			///< Stores the actual state of I2C module.
	uint8_t *PTxData;			///< Poiniter to be transmitting bytes.
	uint8_t TxByteCtr;			///< Number of to be transmitting bytes.
	uint8_t TxByteRes;			///< Number of reserved bytes on stash.
	uint8_t *PRxData;			///< Pointer to be receiving bytes.
	uint8_t RxByteCtr;			///< Number of to be recieving bytes.
} i2c = { .PTxData = 0, .status = IDLE, .TxByteRes = 0 };


/**
 * @var *PTxData Pointer to reserved data for var args.
 */

#if VARIADIC_IMPLEMENTATION

uint8_t *PTxData = 0 ;

#else

uint8_t PTxData[I2C_MAX_ARGS] = {0};

#endif

uint8_t i2c_init (uint16_t smclk_freq, uint16_t i2c_freq)
{
	/* Reset USCI_B0 */
	UCB0CTL1 |= UCSWRST;

	/* Assign P1.6 and P1.7 to USIC_B0 */
	P1SEL |= BIT6 + BIT7;
	P1SEL2 |= BIT6 + BIT7;

	/* USCI_B0 as I2C Master */
	UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;

	/* Set SMCLK as clock for USCI_B0 */
	UCB0CTL1 = UCSSEL_2 + UCSWRST;

	/* Calculate prescaler for USCI_B0 based on SMCLK freq and I2C freq  */
	UCB0BR0 = 0x00FF & (uint16_t)smclk_freq * (1000 / i2c_freq);
	UCB0BR1 = (0xFF00 & (uint16_t)smclk_freq * (1000 / i2c_freq)) >> 8;

	//UCB0BR0 = 0x14;

	/* Enable NACK interrupt */
	UCB0I2CIE = UCNACKIE;

	/* Resume USCI_B0 to normal operation state */
	UCB0CTL1 &= ~UCSWRST;

	/* Enable gloabl interrupts */
	__enable_interrupt();

	return 0;
}

uint8_t i2c_write_var (uint8_t addr, enum I2C_CRTL_CMD rept_start, uint8_t n_args, ...)
{
	/* Wait until all transmitions and receptions done */
	while( i2c.status != IDLE);

#if VARIADIC_IMPLEMENTATION

	/* Reserve size of memory for transmit buffer if buffer is uninitialised*/
	if(PTxData == 0) {
		PTxData = (uint8_t *) malloc(n_args * sizeof(uint8_t));
		i2c.TxByteRes = n_args;
	}
	/* Reserve more memory if actual size not enough */
	else {
		if(i2c.TxByteRes < n_args) {
			PTxData = (uint8_t *) realloc(PTxData, n_args * sizeof(uint8_t));
			i2c.TxByteRes = n_args;
		}
	}

#endif /* VARIADIC_IMPLEMENTATION */

	uint8_t i = 0;
	va_list ap;

	/* Read n_args from function
	 * Copy args to transmit buffer
	 */
	va_start(ap, n_args);
	for(i = 0; i < n_args; i++)
		PTxData[i] = va_arg(ap, uint8_t);
	va_end(ap);

	return i2c_write_arr(addr, rept_start, n_args, PTxData);
}

uint8_t i2c_write_arr(uint8_t addr, enum I2C_CRTL_CMD rept_start, uint8_t n_size, const void *TxData)
{
	/* Copy pointer from TxData */
	i2c.PTxData = (uint8_t *)TxData;

	/* Set slave address */
	UCB0I2CSA = addr;

	/* Set Transmit Interrupt */
	IE2 = UCB0TXIE;

	i2c.rept_start = rept_start;
	i2c.endian = I2C_LITTLE_ENDIAN;

	/* Load TX byte counter */
	i2c.TxByteCtr = n_size;

	i2c.status = TRANSMIT;

	/* Send a start condition with write flag */
	UCB0CTL1 |= UCTR + UCTXSTT;

	/* Wait until Transmition is complet */
	if(rept_start == STOP) {
		while(UCB0STAT & UCBBUSY);
	}
	else {
		_delay_ten_us(20);
	}

	i2c.status = IDLE;

	return 0;

}

uint8_t i2c_read (uint8_t addr, enum I2C_CRTL_CMD rept_start, int8_t endian, uint8_t RXBytes, const void *RxData)
{
	while(i2c.status != IDLE);

	/* Set slave address */
	UCB0I2CSA = addr;

	/* Change pointer to given buffer */
	i2c.PRxData = (uint8_t *)RxData;

	/* Check endian to recalculate pointer adress */
	if(endian == I2C_BIG_ENDIAN) {
		i2c.PRxData += RXBytes;
	}

	/* Load RX counter */
	i2c.RxByteCtr = RXBytes;

	/* Clear Interrupt */
	IE2 = UCB0RXIE;

	/* configure receiver mode */
	UCB0CTL1 &= ~UCTR;

	i2c.endian = endian;

	i2c.status = RECEIVE;

	/* Send a start condition with read flag */
	if ( i2c.RxByteCtr == 1 ) {
		i2c.RxByteCtr = 0;
		__disable_interrupt();
		UCB0CTL1 |= UCTXSTT;
		while(UCB0CTL1 & UCTXSTT);
		UCB0CTL1 |= UCTXSTP;
		__enable_interrupt();
	}
	else {
		i2c.RxByteCtr -= 2;
		UCB0CTL1 |= UCTXSTT;
	}

	while(UCB0STAT & UCBBUSY);

	i2c.status = IDLE;

	return 0;
}

uint8_t i2c_write_arr_endian(uint8_t addr, enum I2C_CRTL_CMD rept_start, int8_t endian, uint8_t n_size, const void *TxData)
{
	/* Copy pointer from TxData */
	i2c.PTxData = (uint8_t *)TxData;

	if(endian == I2C_BIG_ENDIAN)
	{
		i2c.PTxData += n_size;
	}

	/* Set slave address */
	UCB0I2CSA = addr;

	/* Set Transmit Interrupt */
	IE2 = UCB0TXIE;

	i2c.rept_start = rept_start;

	/* Load TX byte counter */
	i2c.TxByteCtr = n_size;

	i2c.status = TRANSMIT;
	i2c.endian = endian;

	/* Send a start condition with write flag */
	UCB0CTL1 |= UCTR + UCTXSTT;

	/* Wait until Transmition is complet */
	if(rept_start == STOP) {
		while(UCB0STAT & UCBBUSY);
	}
	else {
		_delay_ten_us(20);
	}

	i2c.status = IDLE;

	return 0;

}

uint8_t i2c_get_status()
{
	return i2c.status;
}

#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
{
	if(IFG2 & UCB0RXIFG) {
		if( i2c.RxByteCtr == 0) {
			UCB0CTL1 |= UCTXSTP;
			*(i2c.PRxData) = UCB0RXBUF;
			i2c.PRxData += i2c.endian;
		}
		else {
			*(i2c.PRxData) = UCB0RXBUF;
			i2c.PRxData += i2c.endian;
			i2c.RxByteCtr--;
		}
	}
	else {
		if( i2c.TxByteCtr == 0) {
			if( i2c.rept_start == STOP) {
				UCB0CTL1 |= UCTXSTP;
			}
			IFG2 &= ~UCB0TXIFG;
			i2c.rept_start = STOP;
		}
		else {
			UCB0TXBUF = *(i2c.PTxData);
			i2c.PTxData += i2c.endian;
			i2c.TxByteCtr--;
		}
	}
}

#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR(void)
{
	if( UCB0STAT & UCNACKIFG) {
		UCB0CTL1 |= UCTXSTP;
		UCB0STAT &= ~UCNACKIFG;
	}
}
