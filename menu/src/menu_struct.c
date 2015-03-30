#include <menu/menu_struct.h>

MENU_ENTRY main_entry = {
	&view_entry,
	0,
	0,
	0,
	0,
	main_text,
	MENU_MAIN_ENTRY,
};

MENU_ENTRY view_entry = {
	&text_entry,
	0,
	&main_entry,
	&radio_entry,
	0,
	view_text,
	0,
};

MENU_ENTRY radio_entry = {
	&tatp_entry,
	0,
	&main_entry,
	&audio_entry,
	&view_entry,
	radio_text,
	0,
};


MENU_ENTRY tatp_entry = {
	&tatp_on_entry,
	0,
	&radio_entry,
	&volume_entry,
	0,
	tatp_text,
	0,
};
MENU_ENTRY tatp_on_entry = {
	0,
	0,
	&tatp_entry,
	&tatp_off_entry,
	0,
	on_text,
	0,
};

MENU_ENTRY tatp_off_entry = {
	0,
	0,
	&tatp_entry,
	&tatp_return_entry,
	&tatp_on_entry,
	off_text,
	0,
};

MENU_ENTRY tatp_return_entry = {
	&tatp_entry,
	0,
	0,
	0,
	&tatp_off_entry,
	return_text,
	0,
};

MENU_ENTRY volume_entry = {
	0,
	0,
	&radio_entry,
	&freq_entry,
	&tatp_entry,
	volume_text,
	MENU_VOL_ENTRY,
};

MENU_ENTRY freq_entry = {
	&freq_seeking_entry,
	0,
	&radio_entry,
	&radio_return_entry,
	&volume_entry,
	freq_text,
	0,
};

MENU_ENTRY freq_seeking_entry = {
	0,
	0,
	&freq_entry,
	&freq_fixed_entry,
	0,
	seeking_text,
	0,
};

MENU_ENTRY freq_fixed_entry = {
	0,
	0,
	&freq_entry,
	&freq_return_entry,
	&freq_seeking_entry,
	fixed_text,
	MENU_FREQ_ENTRY,
};

MENU_ENTRY freq_return_entry = {
	&freq_entry,
	0,
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
	0,
	&freq_entry,
	return_text,
	0,
};

MENU_ENTRY text_entry = {
	&rds_entry,
	0,
	&view_entry,
	&contrast_entry,
	0,
	text_text,
	0,
};

MENU_ENTRY rds_entry = {
	0,
	0,
	&text_entry,
	&ptypi_entry,
	0,
	rds_text,
	0,
};

MENU_ENTRY ptypi_entry = {
	0,
	0,
	&text_entry,
	&rsq_entry,
	&rds_entry,
	ptypi_text,
	0,
};

MENU_ENTRY rsq_entry = {
	0,
	0,
	&text_entry,
	&text_return_entry,
	&ptypi_entry,
	rsq_text,
	0,
};

MENU_ENTRY text_return_entry = {
	&text_entry,
	0,
	0,
	0,
	&rsq_entry,
	return_text,
	0,
};

MENU_ENTRY contrast_entry = {
	0,
	0,
	&text_entry,
	&brightness_entry,
	&text_entry,
	contrast_text,
	MENU_CONT_ENTRY,
};

MENU_ENTRY brightness_entry = {
	&brightness_auto_entry,
	0,
	&view_entry,
	&view_return_entry,
	&contrast_entry,
	brightness_text,
	0,
};

MENU_ENTRY brightness_fixed_entry = {
	0,
	0,
	&brightness_entry,
	&brightness_return_entry,
	&brightness_auto_entry,
	fixed_text,
	MENU_BRIG_ENTRY,
};

MENU_ENTRY brightness_auto_entry = {
	0,
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
	0,
	&brightness_auto_entry,
	return_text,
	0,
};

MENU_ENTRY audio_entry = {
	&pop_entry,
	0,
	&main_entry,
	&source_entry,
	&radio_entry,
	audio_text,
	0,
};

MENU_ENTRY pop_entry = {
	0,
	0,
	&audio_entry,
	&classic_entry,
	0,
	pop_text,
	0,
};

MENU_ENTRY classic_entry = {
	0,
	0,
	&audio_entry,
	&jazz_entry,
	&pop_entry,
	classic_text,
	0,
};

MENU_ENTRY jazz_entry = {
	0,
	0,
	&audio_entry,
	&hiphop_entry,
	&classic_entry,
	jazz_text,
	0,
};

MENU_ENTRY hiphop_entry = {
	0,
	0,
	&audio_entry,
	&rock_entry,
	&jazz_entry,
	hiphop_text,
	0,
};

MENU_ENTRY rock_entry = {
	0,
	0,
	&audio_entry,
	&news_entry,
	&hiphop_entry,
	rock_text,
	0,
};

MENU_ENTRY news_entry = {
	0,
	0,
	&audio_entry,
	&audio_return_entry,
	&rock_entry,
	news_text,
	0,
};

MENU_ENTRY audio_return_entry = {
	&audio_entry,
	0,
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
	0,
	&audio_entry,
	return_text,
	0,
};

MENU_ENTRY source_entry = {
	&fm_entry,
	0,
	&main_entry,
	&reset_entry,
	&view_entry,
	source_text,
	0,
};

MENU_ENTRY fm_entry = {
	0,
	0,
	&source_entry,
	&am_entry,
	0,
	fm_text,
	0,
};

MENU_ENTRY am_entry = {
	0,
	0,
	&source_entry,
	&linein_entry,
	&fm_entry,
	am_text,
	0,
};

MENU_ENTRY linein_entry = {
	0,
	0,
	&source_entry,
	&source_return_entry,
	&am_entry,
	linein_text,
	0,
};

MENU_ENTRY source_return_entry = {
	&source_entry,
	0,
	0,
	0,
	&linein_entry,
	return_text,
	0,
};

MENU_ENTRY reset_entry = {
	&reset_no_entry,
	0,
	&main_entry,
	&main_return_entry,
	&source_entry,
	reset_text,
	0,
};

MENU_ENTRY reset_no_entry = {
	0,
	0,
	&reset_entry,
	&reset_yes_entry,
	0,
	no_text,
	0,
};

MENU_ENTRY reset_yes_entry = {
	0,
	0,
	&reset_entry,
	0,
	&reset_no_entry,
	yes_text,
	0,
};

MENU_ENTRY main_return_entry = {
	&main_entry,
	0,
	0,
	0,
	&reset_entry,
	return_text,
	0,
};
