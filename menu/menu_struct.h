#ifndef _MENU_STRUCT_H_
#define _MENU_STRUCT_H_

#include "menu.h"

typedef const struct menu_entry {
	const struct menu_entry *child_short;
	const struct menu_entry *child_long;
	const struct menu_entry *parent;
	const struct menu_entry *next;
	const struct menu_entry *previous;
	const char *text;
	const uint16_t entry_num;
} MENU_ENTRY;

extern MENU_ENTRY main_entry;
extern MENU_ENTRY view_entry;
extern MENU_ENTRY radio_entry;
extern MENU_ENTRY freq_entry;
extern MENU_ENTRY volume_entry;
extern MENU_ENTRY source_entry;
extern MENU_ENTRY am_entry;
extern MENU_ENTRY fm_entry;
extern MENU_ENTRY linein_entry;
extern MENU_ENTRY text_entry;
extern MENU_ENTRY rds_entry;
extern MENU_ENTRY rsq_entry;
extern MENU_ENTRY ptypi_entry;
extern MENU_ENTRY tatp_entry;
extern MENU_ENTRY tatp_on_entry;
extern MENU_ENTRY tatp_off_entry;
extern MENU_ENTRY tatp_return_entry;
extern MENU_ENTRY freq_seeking_entry;
extern MENU_ENTRY freq_fixed_entry;
extern MENU_ENTRY freq_return_entry;
extern MENU_ENTRY radio_return_entry;
extern MENU_ENTRY contrast_entry;
extern MENU_ENTRY brightness_return_entry;
extern MENU_ENTRY text_return_entry;
extern MENU_ENTRY brightness_entry;
extern MENU_ENTRY brightness_return_entry;
extern MENU_ENTRY brightness_fixed_entry;
extern MENU_ENTRY brightness_auto_entry;
extern MENU_ENTRY audio_entry;
extern MENU_ENTRY rock_entry;
extern MENU_ENTRY hiphop_entry;
extern MENU_ENTRY pop_entry;
extern MENU_ENTRY jazz_entry;
extern MENU_ENTRY radio_return_entry;
extern MENU_ENTRY classic_entry;
extern MENU_ENTRY news_entry;
extern MENU_ENTRY audio_return_entry;
extern MENU_ENTRY source_return_entry;
extern MENU_ENTRY reset_entry;
extern MENU_ENTRY reset_no_entry;
extern MENU_ENTRY reset_yes_entry;
extern MENU_ENTRY view_return_entry;
extern MENU_ENTRY main_return_entry;

#endif /* _MENU_STRUCT_H_ */
