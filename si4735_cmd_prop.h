#ifndef si4735_cmd_prop
#define si4735_cmd_prop

#include <stdint.h>

#define HB(byte) ((byte & 0xFF00) >> 8)
#define LB(byte) (byte & 0x00FF)

/**
 * @def GPO_IEN
 * @brief Property for the FM/RDS Receiver (Si4704/05/06/2x/3x/4x/84/85), required struct gpo_ien
 *
 * @struct gpo_ien
 * @brief Property 0x0001. GPO_IEN
 *
 * Configures the sources for the GPO2/INT interrupt pin. Valid sources are the lower 8 bits of the STATUS byte,\n
 * including CTS, ERR, RSQINT, RDSINT (Si4705/21/31/35/37/39/41/43/45/85 only), and STCINT bits. The corresponding\n
 * bit is set before the interrupt occurs. The CTS bit (and optional interrupt) is set when it is safe to send the\n
 * next command. The CTS interrupt enable (CTSIEN) can be set with this property and the POWER_UP command. The state\n
 * of the CTSIEN bit set during the POWER_UP command can be read by reading this property and modified by writing this\n
 * property. This property may only be set or read when in powerup mode.
 * Errata:RSQIEN is non-functional on FMRX component 2.0.
 *
 * Available in: All
 *
 * Default: 0x0000
 *
 * @var gpo_ien::RSQREP
 * @brief RSQ Interrupt Repeat
 *
 * 0 = No interrupt generated when RSQINT is already set (default).\n
 * 1 = Interrupt generated even if RSQINT is already set.
 * @var gpo_ien::RDSREP
 * @brief RDS Interrupt Repeat (Si4705/21/31/35/37/39/41/43/45/85-C40 Only).
 *
 * 0 = No interrupt generated when RDSINT is already set (default).\n
 * 1 = Interrupt generated even if RDSINT is already set.
 * @var gpo_ien::STCREP
 * @brief STC Interrupt Repeat.
 *
 * 0 = No interrupt generated when STCINT is already set (default).\n
 * 1 = Interrupt generated even if STCINT is already set.
 * @var gpo_ien::CTSIEN
 * @brief CTS Interrupt Enable. After PowerUp, this bit reflects the CTSIEN bit in ARG1 of PowerUp Command.
 *
 * 0 = No interrupt generated when CTS is set.\n
 * 1 = Interrupt generated when CTS is set.
 * @var gpo_ien::ERRIEN
 * @brief ERR Interrupt Enable.
 *
 * 0 = No interrupt generated when ERR is set (default).\n
 * 1 = Interrupt generated when ERR is set.
 * @var gpo_ien::RDSIEN
 * @brief RDS Interrupt Enable (Si4705/21/31/35/37/39/41/43/45/85-C40 Only).
 *
 * 0 = No interrupt generated when RDSINT is set (default).
 * 1 = Interrupt generated when RDSINT is set.
 * @var gpo_ien::RSQIEN
 * @brief RSQ Interrupt Enable.
 *
 * 0 = No interrupt generated when RSQINT is set (default).\n
 * 1 = Interrupt generated when RSQINT is set
 * @var gpo_ien::STCIEN
 * @brief RSQ Interrupt Enable.
 *
 * 0 = No interrupt generated when RSQINT is set (default).\n
 * 1 = Interrupt generated when RSQINT is set
 */

#define GPO_IEN 	0x0001

typedef struct gpo_ien {
	uint16_t:4;				///< Reserved
	uint16_t RSQREP:1;
	uint16_t RDSREP:1;
	uint16_t :1;			///< Reserved
	uint16_t STCREP:1;
	uint16_t CTSIEN:1;
	uint16_t ERRIEN:1;
	uint16_t :2;			///< Reserved
	uint16_t RSQIEN:1;
	uint16_t RDSIEN:1;
	uint16_t :1;			///< Reserved
	uint16_t STCIEN:1;
} GPO_IEN_STC;

/**
 * @def DOUT_FORMAT
 * @brief Property for the FM/RDS Receiver (Si4704/05/06/2x/3x/4x/84/85), required struct dout_format
 *
 * @def OMODE_0
 * @brief Digital Output Mode 0000 = I2Suse for dout_format::OMODE.
 *
 * @def OMODE_1
 * @brief Digital Output Mode 0110 = Left-justifieduse for dout_format::OMODE.
 *
 * @def OMODE_2
 * @brief Digital Output Mode 1000 = MSB at second DCLK after DFS pulse, use for dout_format::OMODE.
 *
 * @def OMODE_3
 * @brief Digital Output Mode 1100 = MSB at first DCLK after DFS pulse, use for dout_format::OMODE.
 *
 * @def OSIZE_0
 * @brief Digital Output Audio Sample Precision 0 = 16 bits, use for dout_format::OSIZE.
 *
 * @def OSIZE_1
 * @brief Digital Output Audio Sample Precision 1 = 20 bits, use for dout_format::OSIZE.
 *
 * @def OSIZE_2
 * @brief Digital Output Audio Sample Precision 2 = 24 bits, use for dout_format::OSIZE.
 *
 * @def OSIZE_3
 * @brief Digital Output Audio Sample Precision 3 = 8 bits, use for dout_format::OSIZE.
 *
 * @struct dout_format
 * @brief Property 0x0102. DIGITAL_OUTPUT_FORMAT
 *
 * Configures the digital audio output format. Configuration options include DCLK edge, data format, force mono, and\n
 * sample precision.
 *
 * \note Available in: Si4705/06, Si4721/31/35/37/39, Si4730/34/36/38-D60 and later, Si4741/43/45, Si4784/85\n\n
 *
 * Default: 0x0000
 *
 * \warning DIGITAL_OUTPUT_FORMAT is supported in FM receive component 2.0 or later.
 *
 * @var dout_format::OFALL
 * @brief Digital Output DCLK Edge.
 *
 * 0 = use DCLK rising edge\n
 * 1 = use DCLK falling edge
 *
 * @var dout_format::OMODE
 * @brief Digital Output Mode.
 *
 *  OMONO_0: 0000 = I2S\n
 * OMONO_1: 0110 = Left-justified\n
 * OMONO_2: 1000 = MSB at second DCLK after DFS pulse\n
 * OMONO_3: 1100 = MSB at first DCLK after DFS pulse
 *
 * @var dout_format::OMONO
 * @brief Digital Output Mono Mode.
 *
 * 0 = Use mono/stereo blend (per blend thresholds)\n
 * 1 = Force mono
 *
 * @var dout_format::OSIZE
 * @brief Digital Output Audio Sample Precision.
 *
 * OSIZE_0 = 16-bits\n
 * OSIZE_1 = 20-bits\n
 * OSIZE_2 = 24-bits\n
 * OSIZE_3 = 8-bits
 */

#define DOUT_FORMAT	0x0102

#define OMODE_0		0
#define OMODE_1		6
#define OMODE_2		8
#define OMODE_3		12

#define OSIZE_0		0
#define OSIZE_1		1
#define OSIZE_2		2
#define OSIZE_3		3

typedef struct dout_format {
	uint16_t : 8;			///< Reserved
	uint16_t OFALL:1;
	uint16_t OMODE:4;
	uint16_t OMONO:1;
	uint16_t OSIZE:2;
};

/* Property 0x0104. DIGITAL_OUTPUT_SAMPLE_RATE */
#define DOUT_SRATE 	0x0104

/* Property 0x0201. REFCLK_FREQ */
#define REFCLK_FREQ	0x0201

/* Property 0x0202. REFCLK_PRESCALE */
#define REFCLK_PRE	0x0202
#define RCLKSEL		0x1000

/* Property 0x1100. FM_DEEMPHASIS */
#define FM_DEEMPH	0x1100
#define DEEMPH_US	0x0002
#define DEEMPH_EU	0x0001

/* Property 0x1102. FM_CHANNEL_FILTER */
#define FM_CH_FIL	0x1102
#define FM_FILTER_0	0x0000
#define FM_FILTER_1	0x0001
#define FM_FILTER_2	0x0002
#define FM_FILTER_3	0x0003
#define FM_FILTER_4	0x0004

/* Property 0x1105. FM_BLEND_STEREO_THRESHOLD */
#define STTHRESH	0x1105

/* Property 0x1106. FM_BLEND_MONO_THRESHOLD */
#define MONOTHRESH	0x1106

/* Property 0x1108. FM_MAX_TUNE_ERROR */
#define MAXTUNEERR	0x1108

/* Property 0x1200. FM_RSQ_INT_SOURCE */
#define FM_RSQ_INT	0x1200
#define BLENDIEN	0x0080
#define MULTHIEN	0x0020
#define MULTLIEN	0x0010
#define SNRHIEN		0x0008
#define SNRLIEN		0x0004
#define RSSIHIEN	0x0002
#define RSSILIEN	0x0001

/* 0x1201. FM_RSQ_SNR_HI_THRESHOLD */
#define SNRH		0x1201

/* Property 0x1202. FM_RSQ_SNR_LO_THRESHOLD */
#define SNRL		0x1202

/* Property 0x1203. FM_RSQ_RSSI_HI_THRESHOLD */
#define RSSIH		0x1203

/* Property 0x1204. FM_RSQ_RSSI_LO_THRESHOLD */
#define RSSIL		0x1204

/* Property 0x1205. FM_RSQ_MULTIPATH_HI_THRESHOLD */
#define MULTH		0x1205

/* Property 0x1206. FM_RSQ_MULTIPATH_LO_THRESHOLD */
#define MULTL		0x1206

/* Property 0x1207. FM_RSQ_BLEND_THRESHOLD */
#define BLEND		0x1207
#define PILOT		0x0080

/* Property 0x1300. FM_SOFT_MUTE_RATE */
#define SMRATE		0x1300

/* Property 0x1301. FM_SOFT_MUTE_SLOPE */
#define SMSLOPE		0x1301

/* Property 0x1302. FM_SOFT_MUTE_MAX_ATTENUATION */
#define SMATTN		0x1302

/* Property 0x1303. FM_SOFT_MUTE_SNR_THRESHOLD */
#define SMATHR		0x1303

/* Property 0x1304. FM_SOFT_MUTE_RELEASE_RATE */
#define RELEASE		0x1301

/* Property 0x1305. FM_SOFT_MUTE_ATTACK_RATE */
#define ATTACK		0x1305

/* Property 0x1400. FM_SEEK_BAND_BOTTOM */
#define FMSKFREQL	0x1400

/* Property 0x1401. FM_SEEK_BAND_TOP */
#define FMSKFREQH	0x1401

/* Property 0x1402. FM_SEEK_FREQ_SPACING */
#define SKSPACE		0x1402
#define SKSPACE_5	0x0005
#define SKSPACE_10	0x000A
#define SKSPACE_20	0x0014

/* Property 0x1403. FM_SEEK_TUNE_SNR_THRESHOLD */
#define SKSNR		0x1403

/* Property 0x1500. FM_RDS_INT_SOURCE */
#define SKRSSI		 0x1500
#define RDSNEWBLOCKB 0x0005
#define RDSNEWBLOCKA 0x000A
#define RDSSYNCFOUND 0x0014
#define RDSSYNCLOST	 0x0005
#define RDSRECV		 0x000A

/* Property 0x1501. FM_RDS_INT_FIFO_COUNT */
#define RDSFIFOCNT	0x1501

/* Property 0x1502. FM_RDS_CONFIG */
#define SKRSSI		0x1500
#define BLETHA		0xC000
#define BLETHB		0x3000
#define BLETHC		0x0C00
#define BLETHD		0x0300
#define BLETHA_0 	0x0000
#define BLETHA_1 	0x0001
#define BLETHA_2 	0x0002
#define BLETHA_3 	0x0003
#define BLETHB_0 	0x0000
#define BLETHB_1 	0x0001
#define BLETHB_2 	0x0002
#define BLETHB_3 	0x0003
#define BLETHC_0 	0x0000
#define BLETHC_1 	0x0001
#define BLETHC_2 	0x0002
#define BLETHC_3 	0x0003
#define BLETHD_0 	0x0000
#define BLETHD_1 	0x0001
#define BLETHD_2 	0x0002
#define BLETHD_3 	0x0003
#define RDSEN		0x0001

#endif
