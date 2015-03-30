#ifndef si4735_cmd_prop
#define si4735_cmd_prop

#include <stdint.h>

#define HB(byte) ((byte & 0xFF00) >> 8)
#define LB(byte) (byte & 0x00FF)

typedef union int_status {
	uint8_t byte;
	struct {
		uint8_t CTS:1;
		uint8_t ERR:1;
		uint8_t :2;
		uint8_t RSQINT:1;
		uint8_t RDSINT:1;
		uint8_t :1;
		uint8_t STCINT:1;
	};
} INT_STATUS;


/* commands + send arguments types*/

#define POWER_UP		0x01

#define FUNC_0			0x00
#define FUNC_1			0x0F

#define OPMODE_0		0x00
#define OPMODE_1		0x05
#define OPMODE_2		0x0B
#define OPMODE_3		0xB0
#define OPMODE_4		0xB5

typedef union power_up_arg1 {
	uint8_t byte;
	struct {
		uint8_t CTSIEN:1;
		uint8_t GPO2OEN:1;
		uint8_t PATCH:1;
		uint8_t XOSCEN:1;
		uint8_t FUNC:4;
	};
} POWER_UP_ARG1_STC;

#define GET_REV			0x10

#define POWER_DOWN		0x11

#define SET_PROPERTY	0x12

#define GET_PROPERTY	0x13

#define GET_INT_STATUS	0x14

#define FM_TUNE_FREQ	0x20

typedef union fm_tune_freq_arg1 {
	uint8_t byte;
	struct  {
		uint8_t :6;
		uint8_t FREEZE:1;
		uint8_t FAST:1;
	};
} FM_TUNE_FREQ_ARG1_STC;

#define FM_SEEK_START	0x21

typedef union fm_seek_start_arg1 {
	uint8_t byte;
	struct {
		uint8_t :2;
		uint8_t WRAP:1;
		uint8_t SEEKUP:1;
		uint8_t :4;
	};
} FM_SEEK_START_ARG1_STC;


#define FM_TUNE_STATUS	0x22

typedef union fm_tune_status_arg1 {
	uint8_t byte;
	struct {
		uint8_t :6;
		uint8_t CANCEL:1;
		uint8_t INTACK:1;
	};
} FM_TUNE_STATUS_ARG1_STC;

typedef union fm_tune_status_resp1 {
	uint8_t byte;
	struct {
		uint8_t :6;
		uint8_t AFCRL:1;
		uint8_t VALID:1;
	};
} FM_TUNE_STATUS_RESP1_STC;


#define FM_RSQ_STATUS	0x23

typedef union fm_rsq_status_arg1 {
	uint8_t byte;
	struct {
		uint8_t :7;
		uint8_t INTACK:1;
	};
} FM_RSQ_STATUS_ARG1_STC;

typedef union fm_rsq_status_resp1 {
	uint8_t byte;
	struct {
		uint8_t BLENDINT:1;
		uint8_t :1;
		uint8_t MULTHINT:1;
		uint8_t MULTLINT:1;
		uint8_t SNRHINT:1;
		uint8_t SNRLINT:1;
		uint8_t RSSIHINT:1;
		uint8_t RSSILINT:1;
	};
} FM_RSQ_STATUS_RESP1_STC;

typedef union fm_rsq_status_resp2 {
	uint8_t byte;
	struct {
		uint8_t :4;
		uint8_t SMUTE:1;
		uint8_t :1;
		uint8_t AFCRL:1;
		uint8_t VALID:1;
	};
} FM_RSQ_STATUS_RESP2_STC;

typedef union fm_rsq_status_resp3 {
	uint8_t byte;
	struct {
		uint8_t PILOT:1;
		uint8_t STBLEND:7;
	};
} FM_RSQ_STATUS_RESP3_STC;

#define FM_RDS_STATUS	0x24

typedef union fm_rds_status_arg1 {
	uint8_t byte;
	struct {
		uint8_t :5;
		uint8_t STATUSONLY:1;
		uint8_t MTFIFO:1;
		uint8_t INTACK:1;
	};
} FM_RDS_STATUS_ARG1_STC;

typedef union fm_rds_status_resp1 {
	uint8_t byte;
	struct {
		uint8_t :2;
		uint8_t RDSNEWBLOCKB:1;
		uint8_t RDSNEWBLOCKA:1;
		uint8_t :1;
		uint8_t RDSSYNCFOUND:1;
		uint8_t RDSSYNCLOST:1;
		uint8_t RDSRECV:1;
	};
} FM_RDS_STATUS_RESP1_STC;

typedef union fm_rds_status_resp2 {
	uint8_t byte;
	struct {
		uint8_t :5;
		uint8_t GRPLOST:1;
		uint8_t :1;
		uint8_t RDSSYNC:1;
	};
} FM_RDS_STATUS_RESP2_STC;

typedef union fm_rds_status_resp12 {
	uint8_t byte;
	struct {
		uint8_t BLEA:2;
		uint8_t BLEB:1;
		uint8_t BLEC:2;
		uint8_t BLED:2;
	};
} FM_RDS_STATUS_RESP12_STC;

#define FM_AGC_STATUS	0x27

typedef union fm_agc_status_resp1 {
	uint8_t byte;
	struct {
		uint8_t :7;
		uint8_t READ_RFAGCDIS:1;
	};
} FM_AGC_STATUS_RESP1_STC;

typedef union fm_agc_status_resp2 {
	uint8_t byte;
	struct {
		uint8_t :3;
		uint8_t READ_LNA_GAIN_INDEX:5;
	};
} FM_AGC_STATUS_RESP2_STC;

#define FM_AGC_OVERRIDE	0x28

typedef union fm_agc_override_arg1 {
	uint8_t byte;
	struct {
		uint8_t :7;
		uint8_t RFAGCDIS:1;
	};
} FM_AGC_OVERRIDE_ARG1_STC;

typedef union fm_agc_override_arg2 {
	uint8_t byte;
	struct {
		uint8_t :3;
		uint8_t LNA_GAIN_INDEX:4;
	};
} FM_AGC_OVERRIDE_ARG2_STC;


#define GPIO_CTL		0x80

typedef union fm_gpio_ctl_arg1 {
	uint8_t byte;
	struct {
		uint8_t :4;
		uint8_t GPO3OEN:1;
		uint8_t GPO2OEN:1;
		uint8_t GPO1OEN:1;
		uint8_t :1;
	};
} FM_GPIO_CTL_ARG1_STC;

#define GPIO_SET		0x81

typedef union fm_gpio_set_arg1 {
	uint8_t byte;
	struct {
		uint8_t :4;
		uint8_t GPO3LEVEL:1;
		uint8_t GPO2LEVEL:1;
		uint8_t GPO1LEVEL:1;
		uint8_t :1;
	};
} FM_GPIO_SET_ARG1_STC;

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

typedef union gpo_ien {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t STCIEN:1;
		uint16_t :1;			///< Reserved
		uint16_t RDSIEN:1;
		uint16_t RSQIEN:1;
		uint16_t :2;			///< Reserved
		uint16_t ERRIEN:1;
		uint16_t CTSIEN:1;
		uint16_t STCREP:1;
		uint16_t :1;			///< Reserved
		uint16_t RDSREP:1;
		uint16_t RSQREP:1;
		uint16_t:4;				///< Reserved
	};
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

typedef union dout_format {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t : 8;			///< Reserved
		uint16_t OFALL:1;
		uint16_t OMODE:4;
		uint16_t OMONO:1;
		uint16_t OSIZE:2;
	};
} DOUT_FORMAT_STC;

/* Property 0x0104. DIGITAL_OUTPUT_SAMPLE_RATE */
#define DOUT_SAMPLE_RATE 	0x0104

/* Property 0x0201. REFCLK_FREQ */
#define REFCLK_FREQ	0x0201

/* Property 0x0202. REFCLK_PRESCALE */
#define REFCLK_PRESCALE	0x0202

typedef union refclk_pre {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t:3;				///< Reserved
		uint16_t RCLKSEL:1;
		uint16_t REFCLKP:12;
	};
} REFCLK_PRESCALE_STC;

/* Property 0x1100. FM_DEEMPHASIS */
#define FM_DEEMPH	0x1100

#define DEEMPH_US	2
#define DEEMPH_EU	1

typedef union fm_deemphasis {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t:14;				///< Reserved
		uint16_t DEEMPH:2;
	};
} FM_DEEMPHASIS_STC;

/* Property 0x1102. FM_CHANNEL_FILTER */
#define FM_CH_FIL	0x1102

#define FM_FILTER_AUTO		0
#define FM_FILTER_110KHZ	1
#define FM_FILTER_84KHZ		2
#define FM_FILTER_60KHZ		3
#define FM_FILTER_40KHZ		4

typedef union fm_channel_filter {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t:13;				///< Reserved
		uint16_t FMCHFILT:3;

	};
} FM_CHANNEL_FILTER_STC;

/* Property 0x1105. FM_BLEND_STEREO_THRESHOLD */
#define FM_BLEND_STEREO	0x1105

typedef union fm_blend_stereo {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t:9;				///< Reserved
		uint16_t STTHRESH:7;

	};
} FM_BLEND_STEREO_STC;

/* Property 0x1106. FM_BLEND_MONO_THRESHOLD */
#define FM_BLEND_MONO	0x1106

typedef union fm_blend_mono {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t:9;				///< Reserved
		uint16_t MONOTHRESH:7;

	};
} FM_BLEND_MONO_STC;

/* Property 0x1108. FM_MAX_TUNE_ERROR */
#define FM_MAX_TUNE_ERR	0x1108

typedef union fm_max_tune_err {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t:8;				///< Reserved
		uint16_t FMMAXTUNEERR:8;

	};
} FM_MAX_TUNE_ERR_STC;

/* Property 0x1200. FM_RSQ_INT_SOURCE */
#define FM_RSQ_INT_SOURCE	0x1200

typedef union fm_rsq_int_source {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t:8;				///< Reserved
		uint16_t BLENDIEN:1;
		uint16_t :1;			///< Reserved
		uint16_t MULTHIEN:1;
		uint16_t MULTLIEN:1;
		uint16_t SNRHIEN:1;
		uint16_t SNRLIEN:1;
		uint16_t RSSIHIEN:1;
		uint16_t RSSILIEN:1;
	};
} FM_RSQ_INT_SOURCE_STC;

/* 0x1201. FM_RSQ_SNR_HI_THRESHOLD */
#define FM_RSQ_SNR_HI_THRES		0x1201

typedef union fm_rsq_snr_hi_thres {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t:9;				///< Reserved
		uint16_t SNRH:7;

	};
} FM_RSQ_SNR_HI_THRES_STC;

/* Property 0x1202. FM_RSQ_SNR_LO_THRESHOLD */
#define FM_RSQ_SNR_LO_THRES		0x1202

typedef union fm_rsq_snr_lo_thres {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t:9;				///< Reserved
		uint16_t SNRL:7;

	};
} FM_RSQ_SNR_LO_THRES_STC;

/* Property 0x1203. FM_RSQ_RSSI_HI_THRESHOLD */
#define FM_RSQ_RSSI_HI_THRES		0x1203

typedef union fm_rsq_rssi_hi_thres {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t:9;				///< Reserved
		uint16_t RSSIH:7;

	};
} FM_RSQ_RSSI_HI_THRES_STC;

/* Property 0x1204. FM_RSQ_RSSI_LO_THRESHOLD */
#define FM_RSQ_RSSI_LO_THRES		0x1204

typedef union fm_rsq_rssi_lo_thres {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t:9;				///< Reserved
		uint16_t RSSIL:7;

	};
} FM_RSQ_RSSI_LO_THRES_STC;

/* Property 0x1205. FM_RSQ_MULTIPATH_HI_THRESHOLD */
#define FM_RSQ_MULTI_HI_THRES		0x1205

typedef union fm_rsq_multi_hi_thres {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t:9;				///< Reserved
		uint16_t MULTH:7;

	};
} FM_RSQ_MULTI_HI_THRES_STC;

/* Property 0x1206. FM_RSQ_MULTIPATH_LO_THRESHOLD */
#define FM_RSQ_MULTI_LO_THRES		0x1206

typedef union fm_rsq_multi_lo_thres {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t:9;				///< Reserved
		uint16_t MULTL:7;
	};
} FM_RSQ_MULTI_LO_THRES_STC;

/* Property 0x1207. FM_RSQ_BLEND_THRESHOLD */
#define FM_RSQ_BLEND_THRES		0x1207

typedef union fm_rsq_blend_thres {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t:8;				///< Reserved
		uint16_t PILOT:1;
		uint16_t BLEND:7;
	};
} FM_RSQ_BLEND_THRES_STC;

/* Property 0x1300. FM_SOFT_MUTE_RATE */
#define FM_SOFT_MUTE_RATE		0x1300

typedef union fm_soft_mute_rate {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t:8;				///< Reserved
		uint16_t SMRATE:8;
	};
} FM_SOFT_MUTE_RATE_STC;

/* Property 0x1301. FM_SOFT_MUTE_SLOPE */
#define FM_SOFT_MUTE_SLOPE		0x1301

typedef union fm_soft_mute_slope {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t:8;				///< Reserved
		uint16_t SMSLOPE:8;
	};
} FM_SOFT_MUTE_SLOPE_STC;

/* Property 0x1302. FM_SOFT_MUTE_MAX_ATTENUATION */
#define FM_SOFT_MUTE_MAX_ATT		0x1302

typedef union fm_soft_mute_max_att {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t:11;				///< Reserved
		uint16_t SMATTN:5;
	};
} FM_SOFT_MUTE_MAX_ATT_STC;

/* Property 0x1303. FM_SOFT_MUTE_SNR_THRESHOLD */
#define FM_SOFT_MUTE_SNR_THRES		0x1303

typedef union fm_soft_mute_snr_thres {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t:12;				///< Reserved
		uint16_t SMTHR:4;
	};
} FM_SOFT_MUTE_SNR_THRES_STC;

/* Property 0x1304. FM_SOFT_MUTE_RELEASE_RATE */
#define FM_SOFT_MUTE_REL_RATE		0x1301

typedef union fm_soft_mute_rel_rate {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t:1;				///< Reserved
		uint16_t RELEASE:15;
	};
} FM_SOFT_MUTE_REL_RATE_STC;

/* Property 0x1305. FM_SOFT_MUTE_ATTACK_RATE */
#define FM_SOFT_MUTE_ATK_RATE		0x1305

typedef union fm_soft_mute_atk_rate {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t:1;				///< Reserved
		uint16_t ATTACK:15;
	};
} FM_SOFT_MUTE_ATK_RATE_STC;

/* Property 0x1400. FM_SEEK_BAND_BOTTOM */
#define FM_SEEK_BAND_BOTTOM	0x1400

/* Property 0x1401. FM_SEEK_BAND_TOP */
#define FM_SEEK_BAND_TOP	0x1401

/* Property 0x1402. FM_SEEK_FREQ_SPACING */
#define FM_SEEK_FREQ_SPACING		0x1402

#define SKSPACE_5	0x0005
#define SKSPACE_10	0x000A
#define SKSPACE_20	0x0014

typedef union fm_seek_freq_spacing {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t:11;				///< Reserved
		uint16_t SKSPACE:5;
	};
} FM_SEEK_FREQ_SPACING_STC;

/* Property 0x1403. FM_SEEK_TUNE_SNR_THRESHOLD */
#define FM_SEEK_TUNE_SNR_THRES		0x1403

typedef union fm_seek_tune_snr_thres {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t:9;				///< Reserved
		uint16_t SKSNR:7;
	};
} FM_SEEK_TUNE_SNR_THRES_STC;

/* Property 0x1403. FM_SEEK_TUNE_SNR_THRESHOLD */
#define FM_SEEK_TUNE_RSSI_THRES		0x1404

typedef union fm_seek_tune_rssi_thres {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t:9;				///< Reserved
		uint16_t SKRSSI:7;
	};
} FM_SEEK_TUNE_RSSI_THRES_STC;

/* Property 0x1500. FM_RDS_INT_SOURCE */
#define FM_RDS_INT_SOURCE		 0x1500

typedef union fm_rds_int_source {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t RDSRECV:1;
		uint16_t RDSSYNCLOST:1;
		uint16_t RDSSYNCFOUND:1;
		uint16_t:1;				///< Reserved
		uint16_t RDSNEWBLOCKA:1;
		uint16_t RDSNEWBLOCKB:1;
		uint16_t:10;				///< Reserved
	};
} FM_RDS_INT_SOURCE_STC;

/* Property 0x1501. FM_RDS_INT_FIFO_COUNT */
#define FM_RDS_INT_FIFO_COUNT	0x1501

typedef union fm_rds_int_fifo_count {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t RDSFIFOCNT:8;
		uint16_t:8;				///< Reserved
	};
} FM_RDS_INT_FIFO_COUNT_STC;

/* Property 0x1502. FM_RDS_CONFIG */
#define FM_RDS_CONFIG		0x1502

#define BLETH_NO_ERR 	0
#define BLETH_1_2_ERR 	1
#define BLETH_3_5_ERR 	2
#define BLETH_NO_CORR 	3

typedef union fm_rds_config {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t RDSEN:1;
		uint16_t:7;				///< Reserved
		uint16_t BLETHD:2;
		uint16_t BLETHC:2;
		uint16_t BLETHB:2;
		uint16_t BLETHA:2;
	};
} FM_RDS_CONFIG_STC;

/* Property 0x1503. FM_RDS_CONFIDENCE */
#define FM_RDS_CONFIDENCE		0x1503

typedef union fm_rds_confidence {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t :4;
		uint16_t CONFIDENCEB:2;
		uint16_t CONFIDENCEC:2;
		uint16_t CONFIDENCED:2;
	};
} FM_RDS_CONFIDENCE_STC;

/* Property 0x1700. FM_AGC_ATTACK_RATE */
#define FM_AGC_ATK_RATE		0x1700

typedef union fm_agc_atk_rate {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t :8;
		uint16_t ATTACK:8;
	};
} FM_AGC_ATK_RATE_STC;

/* Property 0x1701. FM_AGC_RELEASE_RATE */
#define FM_AGC_REL_RATE		0x1701

typedef union fm_agc_rel_rate {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t :8;
		uint16_t RELEASE:8;
	};
} FM_AGC_REL_RATE_STC;

/* Property 0x1800. FM_BLEND_RSSI_STEREO_THRESHOLD */
#define FM_BLEND_RSSI_STEREO_THRES		0x1800

typedef union fm_blend_rssi_stero_thres {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t :9;
		uint16_t STRTHRESH:7;
	};
} FM_BLEND_RSSI_STEREO_THRES_STC;

/* Property 0x1801. FM_BLEND_RSSI_MONO_THRESHOLD */
#define FM_BLEND_RSSI_MONO_THRES		0x1801

typedef union fm_blend_rssi_mono_thres {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t :4;
		uint16_t CONFIDENCEB:2;
		uint16_t CONFIDENCEC:2;
		uint16_t CONFIDENCED:2;
	};
} FM_BLEND_RSSI_MONO_THRES_STC;

/* Property 0x1802. FM_BLEND_RSSI_ATTACK_RATE */
#define FM_BLEND_RSSI_ATK_RATE		0x1802

/* Property 0x1803. FM_BLEND_RSSI_RELEASE_RATE */
#define FM_BLEND_RSSI_REL_RATE		0x1803

/* Property 0x1804. FM_BLEND_SNR_STEREO_THRESHOLD */
#define FM_BLEND_SNR_STEREO_THRES		0x1804

typedef union fm_blend_snr_stereo_thres {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t :9;
		uint16_t STRTHRESH:7;
	};
} FM_BLEND_SNR_STEREO_THRES_STC;

/* Property 0x1805. FM_BLEND_SNR_MONO_THRESHOLD */
#define FM_BLEND_SNR_MONO_THRES		0x1805

typedef union fm_blend_snr_mono_thres {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t :9;
		uint16_t MONOTHRESH:7;
	};
} FM_BLEND_SNR_MONO_THRES_STC;

/* Property 0x1806. FM_BLEND_SNR_ATTACK_RATE */
#define FM_BLEND_SNR_ATK_RATE		0x1806

/* Property 0x1807. FM_BLEND_SNR_RELEASE_RATE */
#define FM_BLEND_SNR_REL_RATE		0x1807

/* Property 0x1808. FM_BLEND_MULTIPATH_STEREO_THRESHOLD */
#define FM_BLEND_MULTI_STEREO_THRES		0x1808

typedef union fm_blend_multi_stereo_thres {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t :4;
		uint16_t STRTHRESH:2;
	};
} FM_BLEND_MULTI_STEREO_THRES_STC;

/* Property 0x1809. FM_BLEND_MULTIPATH_MONO_THRESHOLD */
#define FM_BLEND_MULTI_MONO_THRES		0x1809

typedef union fm_blend_multi_mono_thres {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t :9;
		uint16_t MONOTHRESH:7;
	};
} FM_BLEND_MULTI_MONO_THRES_STC;

/* Property 0x180A. FM_BLEND_MULTIPATH_ATTACK_RATE */
#define FM_BLEND_MULTI_ATK_RATE		0x180A

/* Property 0x180B. FM_BLEND_MULTIPATH_RELEASE_RATE */
#define FM_BLEND_MULTI_REL_RATE		0x180B

/* Property 0x180C. FM_BLEND_MAX_STEREO_SEPARATION */
#define FM_BLEND_MAX_STEREO_SEPA		0x180C

typedef union fm_blend_max_stereo_sepa {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t :13;
		uint16_t MAX_SEP:3;
	};
} FM_BLEND_MAX_STEREO_SEPA_STC;

/* Property 0x1900. FM_NB_DETECT_THRESHOLD */
#define FM_BLEND_MULTI_ATK_RATE		0x180A

/* Property 0x1901. FM_NB_INTERVAL */
#define FM_BLEND_MULTI_ATK_RATE		0x180A

/* Property 0x1902. FM_NB_RATE */
#define FM_BLEND_MULTI_ATK_RATE		0x180A

/* Property 0x1903. FM_NB_IIR_FILTER */
#define FM_BLEND_MULTI_ATK_RATE		0x180A

/* Property 0x1904. FM_NB_DELAY */
#define FM_BLEND_MULTI_ATK_RATE		0x180A

/* Property 0x1A00. FM_HICUT_SNR_HIGH_THRESHOLD */
#define FM_HICUT_SNR_HIGH_THRES		0x180A

typedef union fm_hicut_snr_high_thres {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t :9;
		uint16_t SNR_HIGH:7;
	};
} FM_HICUT_SNR_HIGH_THRES_STC;

/* Property 0x1A01. FM_HICUT_SNR_LOW_THRESHOLD */
#define FM_HICUT_SNR_LOW_THRES		0x180A

typedef union fm_hicut_snr_low_thres {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t :9;
		uint16_t SNR_LOW:7;
	};
} FM_HICUT_SNR_LOW_THRES_STC;

/* Property 0x1A02. FM_HICUT_ATTACK_RATE */
#define FM_HICUT_ATK_RATE		0x1A02

/* Property 0x1A03. FM_HICUT_RELEASE_RATE */
#define FM_HICUT_REL_RATE		0x1A03

/* Property 0x1A04. FM_HICUT_MULTIPATH_TRIGGER_THRESHOLD */
#define FM_HICUT_MULTI_TRIGGER_THRES		0x1A04

typedef union fm_hicut_multi_trigger_thres {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t :9;
		uint16_t MULT_TRIGGER:7;
	};
} FM_HICUT_MULTI_TRIGGER_THRES_STC;

/* Property 0x1A05. FM_HICUT_MULTIPATH_END_THRESHOLD */
#define FM_HICUT_MULTI_END_THRES		0x1A05

typedef union fm_hicut_multi_end_thres {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t :9;
		uint16_t MULTI_END:7;
	};
} FM_HICUT_MULTI_END_THRES_STC;

/* Property 0x1A06. FM_HICUT_CUTOFF_FREQUENCY */
#define FM_HICUT_CUTOFF_FREQ		0x1A06

#define MAX_AUDIO_TRAN_FREQ_AUTO_BW		0
#define MAX_AUDIO_TRAN_FREQ_2KHZ		1
#define MAX_AUDIO_TRAN_FREQ_3KHZ		2
#define MAX_AUDIO_TRAN_FREQ_4KHZ		3
#define MAX_AUDIO_TRAN_FREQ_5KHZ		4
#define MAX_AUDIO_TRAN_FREQ_6KHZ		5
#define MAX_AUDIO_TRAN_FREQ_8KHZ		6
#define MAX_AUDIO_TRAN_FREQ_11KHZ		7

#define HICUT_AUDIO_TRAN_FREQ_DIS		0
#define HICUT_AUDIO_TRAN_FREQ_2KHZ		1
#define HICUT_AUDIO_TRAN_FREQ_3KHZ		2
#define HICUT_AUDIO_TRAN_FREQ_4KHZ		3
#define HICUT_AUDIO_TRAN_FREQ_5KHZ		4
#define HICUT_AUDIO_TRAN_FREQ_6KHZ		5
#define HICUT_AUDIO_TRAN_FREQ_8KHZ		6
#define HICUT_AUDIO_TRAN_FREQ_11KHZ		7

typedef union fm_hicut_cutoff_freq {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t :9;
		uint16_t MAX_AUDIO_FREQ:3;
		uint16_t :1;
		uint16_t FREQUENCY:3;
	};
} FM_HICUT_CUTOFF_FREQ_STC;

/* Property 0x4000. RX_VOLUME */
#define RX_VOLUME		0x4000

typedef union rx_volume {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t :10;
		uint16_t VOLUME:6;
	};
} RX_VOLUME_STC;

/* Property 0x4001. RX_HARD_MUTE */
#define RX_HARD_MUTE		0x4001

typedef union rx_hard_mute {
	uint16_t byte;
	uint8_t  bytes[2];
	struct {
		uint16_t :14;
		uint16_t LMUTE:1;
		uint16_t RMUTE:1;
	};
} RX_HARD_MUTE_STC;

#endif
