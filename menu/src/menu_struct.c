#include <menu/menu_struct.h>

MENU_ENTRY * const menu_long_entry = &view_entry;
MENU_ENTRY * const menu_short_entry = &station_list_entry;

MENU_ENTRY main_long_entry = {
	&view_entry,
	0,
	0,
	0,
	main_long_text,
	MENU_MAIN_ENTRY,
};

MENU_ENTRY main_short_entry = {
	&station_list_entry,
	0,
	0,
	0,
	main_short_text,
	MENU_MAIN_ENTRY,
};

MENU_ENTRY view_entry = {
	&text_entry,
	&main_long_entry,
	&radio_entry,
	0,
	view_text,
	0,
};

MENU_ENTRY radio_entry = {
	&tatp_entry,
	&main_long_entry,
	&audio_entry,
	&view_entry,
	radio_text,
	0,
};


MENU_ENTRY tatp_entry = {
	&tatp_on_entry,
	&radio_entry,
	&volume_start_entry,
	0,
	tatp_text,
	0,
};
MENU_ENTRY tatp_on_entry = {
	0,
	&tatp_entry,
	&tatp_off_entry,
	0,
	on_text,
	MENU_TA_TP_ON,
};

MENU_ENTRY tatp_off_entry = {
	0,
	&tatp_entry,
	&tatp_return_entry,
	&tatp_on_entry,
	off_text,
	MENU_TA_TP_OFF,
};

MENU_ENTRY tatp_return_entry = {
	&tatp_entry,
	0,
	0,
	&tatp_off_entry,
	return_text,
	0,
};

MENU_ENTRY volume_start_entry = {
	0,
	&radio_entry,
	&volume_ta_entry,
	&tatp_entry,
	volume_start_text,
	MENU_VOL_START_ENTRY,
};

MENU_ENTRY volume_ta_entry = {
	0,
	&radio_entry,
	&freq_entry,
	&volume_start_entry,
	volume_ta_text,
	MENU_VOL_TA_ENTRY,
};


MENU_ENTRY freq_entry = {
	&freq_seek_up_entry,
	&radio_entry,
	&radio_return_entry,
	&volume_ta_entry,
	freq_text,
	0,
};

MENU_ENTRY freq_seek_up_entry = {
	0,
	&freq_entry,
	&freq_seek_down_entry,
	0,
	seeking_up_text,
	MENU_FREQ_SEEK_UP_ENTRY,
};

MENU_ENTRY freq_seek_down_entry = {
	0,
	&freq_entry,
	&freq_fixed_entry,
	&freq_seek_up_entry,
	seeking_down_text,
	MENU_FREQ_SEEK_DOWN_ENTRY,
};

MENU_ENTRY freq_fixed_entry = {
	0,
	&freq_entry,
	&freq_return_entry,
	&freq_seek_down_entry,
	choose_text,
	MENU_FREQ_CHOOSE_ENTRY,
};

MENU_ENTRY freq_return_entry = {
	&freq_entry,
	0,
	0,
	&freq_fixed_entry,
	return_text,
	0,
};

MENU_ENTRY radio_return_entry = {
	&radio_entry,
	0,
	0,
	&freq_entry,
	return_text,
	0,
};

MENU_ENTRY text_entry = {
	&rds_entry,
	&view_entry,
	&contrast_entry,
	0,
	text_text,
	0,
};

MENU_ENTRY rds_entry = {
	0,
	&text_entry,
	&ptypi_entry,
	0,
	rds_text,
	RADIO_RDS_VIEW_ENTRY,
};

MENU_ENTRY ptypi_entry = {
	0,
	&text_entry,
	&rsq_entry,
	&rds_entry,
	ptypi_text,
	RADIO_PIPTY_VIEW_ENTRY,
};

MENU_ENTRY rsq_entry = {
	0,
	&text_entry,
	&text_return_entry,
	&ptypi_entry,
	rsq_text,
	RADIO_RSQ_VIEW_ENTRY,
};

MENU_ENTRY text_return_entry = {
	&text_entry,
	0,
	0,
	&rsq_entry,
	return_text,
	0,
};

MENU_ENTRY contrast_entry = {
	0,
	&text_entry,
	&brightness_entry,
	&text_entry,
	contrast_text,
	MENU_CONT_ENTRY,
};

MENU_ENTRY brightness_entry = {
	&brightness_auto_entry,
	&view_entry,
	&view_return_entry,
	&contrast_entry,
	brightness_text,
	0,
};

MENU_ENTRY brightness_fixed_entry = {
	0,
	&brightness_entry,
	&brightness_return_entry,
	&brightness_auto_entry,
	fixed_text,
	MENU_BRIG_ENTRY,
};

MENU_ENTRY brightness_auto_entry = {
	0,
	&brightness_entry,
	&brightness_fixed_entry,
	0,
	auto_text,
	0,
};

MENU_ENTRY brightness_return_entry = {
	&brightness_entry,
	0,
	0,
	&brightness_auto_entry,
	return_text,
	0,
};

MENU_ENTRY audio_entry = {
	&pop_entry,
	&main_long_entry,
	&source_entry,
	&radio_entry,
	audio_text,
	0,
};

MENU_ENTRY pop_entry = {
	0,
	&audio_entry,
	&classic_entry,
	0,
	pop_text,
	AUDIO_POP_ENTRY,
};

MENU_ENTRY classic_entry = {
	0,
	&audio_entry,
	&jazz_entry,
	&pop_entry,
	classic_text,
	AUDIO_CLASSIC_ENTRY,
};

MENU_ENTRY jazz_entry = {
	0,
	&audio_entry,
	&hiphop_entry,
	&classic_entry,
	jazz_text,
	AUDIO_JAZZ_ENTRY,
};

MENU_ENTRY hiphop_entry = {
	0,
	&audio_entry,
	&rock_entry,
	&jazz_entry,
	hiphop_text,
	AUDIO_HIPHOP_ENTRY,
};

MENU_ENTRY rock_entry = {
	0,
	&audio_entry,
	&news_entry,
	&hiphop_entry,
	rock_text,
	AUDIO_ROCK_ENTRY,
};

MENU_ENTRY news_entry = {
	0,
	&audio_entry,
	&audio_return_entry,
	&rock_entry,
	news_text,
	AUDIO_NEWS_ENTRY,
};

MENU_ENTRY audio_return_entry = {
	&audio_entry,
	0,
	0,
	&news_entry,
	return_text,
	0,
};

MENU_ENTRY view_return_entry = {
	&view_entry,
	0,
	0,
	&brightness_entry,
	return_text,
	0,
};

MENU_ENTRY source_entry = {
	&fm_entry,
	&main_long_entry,
	&reset_entry,
	&audio_entry,
	source_text,
	0,
};

MENU_ENTRY fm_entry = {
	0,
	&source_entry,
	&am_entry,
	0,
	fm_text,
	SOURCE_FM_ENTRY,
};

MENU_ENTRY am_entry = {
	0,
	&source_entry,
	&linein_entry,
	&fm_entry,
	am_text,
	SOURCE_AM_ENTRY,
};

MENU_ENTRY linein_entry = {
	0,
	&source_entry,
	&source_return_entry,
	&am_entry,
	linein_text,
	SOURCE_LINEIN_ENTRY,
};

MENU_ENTRY source_return_entry = {
	&source_entry,
	0,
	0,
	&linein_entry,
	return_text,
	0,
};

MENU_ENTRY reset_entry = {
	&reset_no_entry,
	&main_long_entry,
	&main_long_return_entry,
	&source_entry,
	reset_text,
	0,
};

MENU_ENTRY reset_no_entry = {
	0,
	&reset_entry,
	&reset_yes_entry,
	0,
	no_text,
	0,
};

MENU_ENTRY reset_yes_entry = {
	0,
	&reset_entry,
	0,
	&reset_no_entry,
	yes_text,
	0,
};

MENU_ENTRY main_long_return_entry = {
	&main_long_entry,
	0,
	0,
	&reset_entry,
	return_text,
	0,
};

MENU_ENTRY station_list_entry = {
	0,
	&main_short_entry,
	&seeking_up_entry,
	0,
	station_list_text,
	STATION_VIEW,
};

MENU_ENTRY seeking_up_entry = {
	0,
	&main_short_entry,
	&seeking_down_entry,
	&station_list_entry,
	seeking_up_text,
	SEEKING_UP,
};

MENU_ENTRY seeking_down_entry = {
	0,
	&main_short_entry,
	&station_store_entry,
	&seeking_up_entry,
	seeking_down_text,
	SEEKING_DOWN,
};

MENU_ENTRY station_store_entry = {
	0,
	&main_short_entry,
	&auto_search_entry,
	&seeking_down_entry,
	station_store_text,
	STORE_STATION,
};

MENU_ENTRY auto_search_entry = {
	0,
	&main_short_entry,
	&main_short_return_entry,
	&station_store_entry,
	auto_search_text,
	AUTO_SEARCH,
};

MENU_ENTRY main_short_return_entry = {
	&main_short_entry,
	0,
	0,
	&auto_search_entry,
	return_text,
	0,
};
