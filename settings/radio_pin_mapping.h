/**
 * @date Mar 11 2015 13:52:21
 */

#ifndef RADIO_PIN_MAPPING_H_
#define RADIO_PIN_MAPPING_H_

//----------------------------------------------------------------------------------------
//
/// \description defines for MSP_RXD on PORT: 1. PIN: 1.
//
//----------------------------------------------------------------------------------------
#define MSP_RXD_PIN				BIT1
#define MSP_RXD_IN				P1IN
#define MSP_RXD_OUT				P1OUT
#define MSP_RXD_DIR				P1DIR
#define MSP_RXD_IFG				P1IFG
#define MSP_RXD_IES				P1IES
#define MSP_RXD_IE				P1IE
#define MSP_RXD_INT				1
#define MSP_RXD_SEL				P1SEL
#define MSP_RXD_SEL2			P1SEL2
#define MSP_RXD_REN				P1REN

//----------------------------------------------------------------------------------------
//
/// \description defines for MSP_TXD on PORT: 1. PIN: 2.
//
//----------------------------------------------------------------------------------------
#define MSP_TXD_PIN				BIT2
#define MSP_TXD_IN				P1IN
#define MSP_TXD_OUT				P1OUT
#define MSP_TXD_DIR				P1DIR
#define MSP_TXD_IFG				P1IFG
#define MSP_TXD_IES				P1IES
#define MSP_TXD_IE				P1IE
#define MSP_TXD_INT				2
#define MSP_TXD_SEL				P1SEL
#define MSP_TXD_SEL2			P1SEL2
#define MSP_TXD_REN				P1REN

//----------------------------------------------------------------------------------------
//
/// \description defines for EN1_A on PORT: 1. PIN: 3.
//
//----------------------------------------------------------------------------------------
#define EN1_A_PIN				BIT3
#define EN1_A_IN				P1IN
#define EN1_A_OUT				P1OUT
#define EN1_A_DIR				P1DIR
#define EN1_A_IFG				P1IFG
#define EN1_A_IES				P1IES
#define EN1_A_IE				P1IE
#define EN1_A_INT				3
#define EN1_A_SEL				P1SEL
#define EN1_A_SEL2				P1SEL2
#define EN1_A_REN				P1REN

//----------------------------------------------------------------------------------------
//
/// \description defines for EN1_B on PORT: 1. PIN: 4.
//
//----------------------------------------------------------------------------------------
#define EN1_B_PIN				BIT4
#define EN1_B_IN				P1IN
#define EN1_B_OUT				P1OUT
#define EN1_B_DIR				P1DIR
#define EN1_B_IFG				P1IFG
#define EN1_B_IES				P1IES
#define EN1_B_IE				P1IE
#define EN1_B_INT				4
#define EN1_B_SEL				P1SEL
#define EN1_B_SEL2				P1SEL2
#define EN1_B_REN				P1REN

//----------------------------------------------------------------------------------------
//
/// \description defines for EN1_TAST on PORT: 1. PIN: 5.
//
//----------------------------------------------------------------------------------------
#define EN1_TAST_PIN			BIT5
#define EN1_TAST_IN				P1IN
#define EN1_TAST_OUT			P1OUT
#define EN1_TAST_DIR			P1DIR
#define EN1_TAST_IFG			P1IFG
#define EN1_TAST_IES			P1IES
#define EN1_TAST_IE				P1IE
#define EN1_TAST_INT				5
#define EN1_TAST_SEL			P1SEL
#define EN1_TAST_SEL2			P1SEL2
#define EN1_TAST_REN			P1REN

//----------------------------------------------------------------------------------------
//
/// \description defines for SI_RST on PORT: 3. PIN: 1.
//
//----------------------------------------------------------------------------------------
#define SI_RST_PIN				BIT1
#define SI_RST_IN				P3IN
#define SI_RST_OUT				P3OUT
#define SI_RST_DIR				P3DIR
#define SI_RST_SEL				P3SEL
#define SI_RST_SEL2				P3SEL2
#define SI_RST_REN				P3REN

//----------------------------------------------------------------------------------------
//
/// \description defines for SI_INT on PORT: 2. PIN: 3.
//
//----------------------------------------------------------------------------------------
#define SI_INT_PIN				BIT3
#define SI_INT_IN				P2IN
#define SI_INT_OUT				P2OUT
#define SI_INT_DIR				P2DIR
#define SI_INT_IFG				P2IFG
#define SI_INT_IES				P2IES
#define SI_INT_IE				P2IE
#define SI_INT_INT				11
#define SI_INT_SEL				P2SEL
#define SI_INT_SEL2				P2SEL2
#define SI_INT_REN				P2REN

//----------------------------------------------------------------------------------------
//
/// \description defines for SI_EN on PORT: 3. PIN: 0.
//
//----------------------------------------------------------------------------------------
#define SI_EN_PIN				BIT0
#define SI_EN_IN				P3IN
#define SI_EN_OUT				P3OUT
#define SI_EN_DIR				P3DIR
#define SI_EN_SEL				P3SEL
#define SI_EN_SEL2				P3SEL2
#define SI_EN_REN				P3REN

//----------------------------------------------------------------------------------------
//
/// \description defines for EN2_A on PORT: 2. PIN: 0.
//
//----------------------------------------------------------------------------------------
#define EN2_A_PIN				BIT0
#define EN2_A_IN				P2IN
#define EN2_A_OUT				P2OUT
#define EN2_A_DIR				P2DIR
#define EN2_A_IFG				P2IFG
#define EN2_A_IES				P2IES
#define EN2_A_IE				P2IE
#define EN2_A_INT				8
#define EN2_A_SEL				P2SEL
#define EN2_A_SEL2				P2SEL2
#define EN2_A_REN				P2REN

//----------------------------------------------------------------------------------------
//
/// \description defines for EN2_B on PORT: 2. PIN: 1.
//
//----------------------------------------------------------------------------------------
#define EN2_B_PIN				BIT1
#define EN2_B_IN				P2IN
#define EN2_B_OUT				P2OUT
#define EN2_B_DIR				P2DIR
#define EN2_B_IFG				P2IFG
#define EN2_B_IES				P2IES
#define EN2_B_IE				P2IE
#define EN2_B_INT				9
#define EN2_B_SEL				P2SEL
#define EN2_B_SEL2				P2SEL2
#define EN2_B_REN				P2REN

//----------------------------------------------------------------------------------------
//
/// \description defines for EN2_TAST on PORT: 2. PIN: 2.
//
//----------------------------------------------------------------------------------------
#define EN2_TAST_PIN			BIT2
#define EN2_TAST_IN				P2IN
#define EN2_TAST_OUT			P2OUT
#define EN2_TAST_DIR			P2DIR
#define EN2_TAST_IFG			P2IFG
#define EN2_TAST_IES			P2IES
#define EN2_TAST_IE				P2IE
#define EN2_TAST_INT				10
#define EN2_TAST_SEL			P2SEL
#define EN2_TAST_SEL2			P2SEL2
#define EN2_TAST_REN			P2REN

//----------------------------------------------------------------------------------------
//
/// \description defines for AUDIO_SW_GND on PORT: 3. PIN: 2.
//
//----------------------------------------------------------------------------------------
#define AUDIO_SW_GND_PIN		BIT2
#define AUDIO_SW_GND_IN			P3IN
#define AUDIO_SW_GND_OUT		P3OUT
#define AUDIO_SW_GND_DIR		P3DIR
#define AUDIO_SW_GND_SEL		P3SEL
#define AUDIO_SW_GND_SEL2		P3SEL2
#define AUDIO_SW_GND_REN		P3REN

//----------------------------------------------------------------------------------------
//
/// \description defines for AUDIO_SW_LINE on PORT: 3. PIN: 3.
//
//----------------------------------------------------------------------------------------
#define AUDIO_SW_LINE_PIN		BIT3
#define AUDIO_SW_LINE_IN		P3IN
#define AUDIO_SW_LINE_OUT		P3OUT
#define AUDIO_SW_LINE_DIR		P3DIR
#define AUDIO_SW_LINE_SEL		P3SEL
#define AUDIO_SW_LINE_SEL2		P3SEL2
#define AUDIO_SW_LINE_REN		P3REN

//----------------------------------------------------------------------------------------
//
/// \description defines for PCA_ON_OFF on PORT: 2. PIN: 4.
//
//----------------------------------------------------------------------------------------
#define PCA_ON_OFF_PIN			BIT4
#define PCA_ON_OFF_IN			P2IN
#define PCA_ON_OFF_OUT			P2OUT
#define PCA_ON_OFF_DIR			P2DIR
#define PCA_ON_OFF_IFG			P2IFG
#define PCA_ON_OFF_IES			P2IES
#define PCA_ON_OFF_IE			P2IE
#define PCA_ON_OFF_INT			12
#define PCA_ON_OFF_SEL			P2SEL
#define PCA_ON_OFF_SEL2			P2SEL2
#define PCA_ON_OFF_REN			P2REN

//----------------------------------------------------------------------------------------
//
/// \description defines for PCA_INT on PORT: 2. PIN: 5.
//
//----------------------------------------------------------------------------------------
#define PCA_INT_PIN				BIT5
#define PCA_INT_IN				P2IN
#define PCA_INT_OUT				P2OUT
#define PCA_INT_DIR				P2DIR
#define PCA_INT_IFG				P2IFG
#define PCA_INT_IES				P2IES
#define PCA_INT_IE				P2IE
#define PCA_INT_INT				13
#define PCA_INT_SEL				P2SEL
#define PCA_INT_SEL2			P2SEL2
#define PCA_INT_REN				P2REN

//----------------------------------------------------------------------------------------
//
/// \description defines for ANT_MF on PORT: 3. PIN: 5.
//
//----------------------------------------------------------------------------------------
#define ANT_MF_PIN				BIT5
#define ANT_MF_IN				P3IN
#define ANT_MF_OUT				P3OUT
#define ANT_MF_DIR				P3DIR
#define ANT_MF_SEL				P3SEL
#define ANT_MF_SEL2				P3SEL2
#define ANT_MF_REN				P3REN

//----------------------------------------------------------------------------------------
//
/// \description defines for ANT_LF on PORT: 3. PIN: 6.
//
//----------------------------------------------------------------------------------------
#define ANT_LF_PIN				BIT6
#define ANT_LF_IN				P3IN
#define ANT_LF_OUT				P3OUT
#define ANT_LF_DIR				P3DIR
#define ANT_LF_SEL				P3SEL
#define ANT_LF_SEL2				P3SEL2
#define ANT_LF_REN				P3REN

//----------------------------------------------------------------------------------------
//
/// \description defines for AMP_SHUTDOWN on PORT: 3. PIN: 7.
//
//----------------------------------------------------------------------------------------
#define AMP_SHUTDOWN_PIN		BIT7
#define AMP_SHUTDOWN_IN			P3IN
#define AMP_SHUTDOWN_OUT		P3OUT
#define AMP_SHUTDOWN_DIR		P3DIR
#define AMP_SHUTDOWN_SEL		P3SEL
#define AMP_SHUTDOWN_SEL2		P3SEL2
#define AMP_SHUTDOWN_REN		P3REN

//----------------------------------------------------------------------------------------
//
/// \description defines for SCL on PORT: 1. PIN: 6.
//
//----------------------------------------------------------------------------------------
#define SCL_PIN					BIT6
#define SCL_IN					P1IN
#define SCL_OUT					P1OUT
#define SCL_DIR					P1DIR
#define SCL_IFG					P1IFG
#define SCL_IES					P1IES
#define SCL_IE					P1IE
#define SCL_INT					6
#define SCL_SEL					P1SEL
#define SCL_SEL2				P1SEL2
#define SCL_REN					P1REN

//----------------------------------------------------------------------------------------
//
/// \description defines for SDA on PORT: 1. PIN: 7.
//
//----------------------------------------------------------------------------------------
#define SDA_PIN					BIT7
#define SDA_IN					P1IN
#define SDA_OUT					P1OUT
#define SDA_DIR					P1DIR
#define SDA_IFG					P1IFG
#define SDA_IES					P1IES
#define SDA_IE					P1IE
#define SDA_INT					7
#define SDA_SEL					P1SEL
#define SDA_SEL2				P1SEL2
#define SDA_REN					P1REN

//----------------------------------------------------------------------------------------
//
/// \description defines for PWM_RST on PORT: 1. PIN: 0.
//
//----------------------------------------------------------------------------------------
#define PWM_RST_PIN				BIT0
#define PWM_RST_IN				P1IN
#define PWM_RST_OUT				P1OUT
#define PWM_RST_DIR				P1DIR
#define PWM_RST_IFG				P1IFG
#define PWM_RST_IES				P1IES
#define PWM_RST_IE				P1IE
#define PWM_RST_INT				0
#define PWM_RST_SEL				P1SEL
#define PWM_RST_SEL2			P1SEL2
#define PWM_RST_REN				P1REN

#endif /* RADIO_PIN_MAPPING */
