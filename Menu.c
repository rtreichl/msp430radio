
#include "Encoder.h"
#include "LCDDOGMA.h"
#include "SI4735.h"
#include "msp430.h"
#include "MSP430G2553_USCI_I2C.h"
#include "Timer.h"
#include <stdio.h>
#include "Radio.h"
#include "Flash.h"
#include "Menu.h"
#include "string.h"
#include "Verstaerker.h"
//#include "AudioSwitch.h"
#include "MSP430G2553_Clock_Timer.h"



///////////////////////////////////////////////////////////////////////////DEFINES+EXTERNE VARIABLEN+FUNKTIONEN/////////////////////////////////////////////////////////////////////////////

extern volatile unsigned char encoder_1_button, encoder_2_button, sekunde, sec , posrt;

////////////////////////////////////////////////////////////////////////////////ENDE DEFINES////////////////////////////////////////////////////////////////////////////////////////////////

void menu(void)
{

	/********************************************************************DEKLARATION VARIABLEN*********************************************************************************************/
	
	static char counter2_first_lvl = MAX_FIRST_LVL,
				counter2_second_lvl = MAX_SECOND_LVL,
				counter2_third_lvl = MAX_THIRD_LVL,
				counter2_zero_lvl = MAX_ZERO_LVL;
	char string[17];
	static signed char en_counter1 = 0,act_vol = 20, vol_mute = 0,en_counter2 = 0;
	static unsigned char act_freq = 203;
	static int Radio_States = 0;
	static char Station_Name[9], Radion_Text[65], Akt_Radio_Text[65] = "Radio Text";
	static char delay_vol = 0,delay_freq = 0;
	static char s_time[7] = "\6  :  ", s_date[9] = "01.01.14";
	static char init = 0, set = 0, radio_on = 1;

	//char sel_mode,AM_MODE,FM_MODE,rds_mode,RDS_OFF,RDS_ON,pty_mode,tpta_mode,PTY_OFF,PTY_ON,TPTA_OFF,TPTA_ON,RESET,NO,YES,standby_mode;
	unsigned int i=0;
	/*********************************************************************ENDE VARIABLEN***************************************************************************************************/

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////BEGINN MENÜSTEUERUNG/////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	/***************************************************************encoder-auswahl für encoder 1******************************************************************************************/
	/****************************************Encoderzustände: l (lang gedrückt), k (kurz gedrückt), f (Taste wieder frei)******************************************************************/
	/**************************************************************************************************************************************************************************************/
	if(init == 0)																									//Beim einschalten Initialisieren
	{
		time_date(0, 0, 1, 1, 14, 1, 0, s_time, s_date);
		Amplifier_Gain(AMPLIFIER_GAIN);
		init = 1;
	}
	en_counter1 +=  Encoder_1_get_count();																			//Encoderwert für Encoder 1 um geänderten Encoderwert erhöhen
	en_counter2 +=  Encoder_2_get_count();																			//Encoderwert für Encoder 2 um geänderten Encoderwert erhöhen


	if (encoder_1_button=='k')																						//Bei kurzem Tastendruck Volume Mute ON/OFF					
	{
		vol_mute ^= 1;
		Amplifier_Shutdown(vol_mute);
		encoder_1_button = 'f';															
	}
	else if (encoder_1_button=='l')																					//Bei langem Tastendruck in Standby-Modus wechseln
	{
		if(set==0)
		{
			enter_standby();
			set=1;
		}
		if(set==1)
		{
			exit_standby();
			set=0;
		}
		encoder_1_button = 'f';
	}
	/**********************************************************************ende encoder 1**************************************************************************************************/

	/**************************************************************encoder-auswahl für encoder 2*******************************************************************************************/
	/*********************************************Encoderzustände: l (lang gedrückt), k (kurz gedrückt), f (Taste wieder frei)*************************************************************/
	/**************************************************************************************************************************************************************************************/
	switch (counter2_zero_lvl)
	{
	case 1:																											//Senderauswahl
		switch (counter2_first_lvl)
		{
		case 0:																										//Senderliste
			en_counter2 = check_for_out_of_range(en_counter2,15);
			en_counter2 = en_counter2%15;																			//Auf 15 Werte begrenzen(gibt nicht mehr)
			if(encoder_2_button=='k')
			{
				char tmp = 0;
				tmp = *(char *)(FLASH_ADR_STATION_FREQ + en_counter2*10);
				if(tmp != 0xFF)																						//Falls ein Sender ausgewählt	
				{
					//USCI_I2C_INIT(0x11,80);
					act_freq = tmp;																					//Übernehme Frequenz
					SI4735_Fm_Tune_Freq_2((875+(char)(act_freq-1))*10);
					Radio_States |= (1<<15);
					strncpy(Akt_Radio_Text,"Radio Text",64);
				}
				en_counter2 = 0;																					//Durchlaufender Counter wird wieder Null gesetzt, da eine Ebene höher
				counter2_zero_lvl = MAX_ZERO_LVL;																	//Wieder auf den Haupt-
				counter2_first_lvl = MAX_FIRST_LVL;																	//bildschirm kommen
				encoder_2_button = 'f';
			}
			break;
		case 1:																										//Speichern
			en_counter2 = check_for_out_of_range(en_counter2,15);
			en_counter2 = en_counter2%15;																			//Auf 15 Werte begrenzen(Anzahl der Speicherplätze)
			if (encoder_2_button == 'k')
			{
				stor_data_to_flash ((char *)FLASH_ADR_STATION_NAME, Station_Name, act_freq, en_counter2*10); 

				counter2_zero_lvl = MAX_ZERO_LVL;																	//Wieder auf den Haupt-
				counter2_first_lvl = MAX_FIRST_LVL;																	//bildschirm kommen
				en_counter2 = 0;																					//Durchlaufender Counter wird wieder Null gesetzt, da eine Ebene höher
				encoder_2_button = 'f';
			}
			break;
		default:
			en_counter2 = check_for_out_of_range(en_counter2,3);
			en_counter2 = en_counter2%3;																			//Auf 3 Werte begrenzen(Senderliste,Speichern,Return)
			if (encoder_2_button=='k')
			{
				counter2_first_lvl = en_counter2;																	//Encoderwert für erste ebene ermitteln
				encoder_2_button = 'f';
				en_counter2 = 0;																					//Durchlaufender Counter wird wieder Null gesetzt, da eine Ebene tiefer
			}
		}
		break;
	case 2:																											//Einstellungen
		switch (counter2_first_lvl)
		{
		case 0:																										//AM/FM
				en_counter2 = check_for_out_of_range(en_counter2,2);
				en_counter2 = en_counter2 % 2;																		//Auf 2 begrenzt(AM/FM)
				if (encoder_2_button=='k')
				{
					counter2_second_lvl = en_counter2;																//Bei kurzem Tastendruck Encoder-Wert für dritte Ebene übernehmen
					encoder_2_button = 'f';
				}

			break;
		case 1:																										//Anzeige
			switch(counter2_second_lvl)
			{
			case 0:																									//rds(ON/OFF)
					en_counter2 = check_for_out_of_range(en_counter2,2);
					en_counter2 = en_counter2 % 2;																	//Auf 2 begrenzt
					if (encoder_2_button=='k')
					{
						counter2_third_lvl = en_counter2;															//Bei kurzem Tastendruck Encoder-Wert für dritte Ebene übernehmen
						encoder_2_button = 'f';

					}

				break;
			case 1:																									//tp/ta
					en_counter2 = check_for_out_of_range(en_counter2,2);
					en_counter2 = en_counter2 % 2;																	//Auf 2 begrenzt(ON/OFF)
					if (encoder_2_button=='k')																		
					{
						counter2_third_lvl = en_counter2;															//Bei kurzem Tastendruck Encoder-Wert für dritte Ebene übernehmen
						encoder_2_button = 'f';
					}

				break;
			case 2:																									//return
					counter2_first_lvl = MAX_FIRST_LVL;																//Wieder in das Menü 
					counter2_second_lvl = MAX_SECOND_LVL;															//der Einstellungen springen
					encoder_2_button = 'f';
					en_counter2 = 0;																				
				break;
			default:																								//anzeige-auswahl
					en_counter2 = check_for_out_of_range(en_counter2,4);
					en_counter2 = en_counter2 % 3;																	//Auf 4 begrenzen(RDS/PTY/TPTA/RETURN)
					if (encoder_2_button=='k')
					{
						counter2_second_lvl = en_counter2;															//Bei kurzem Tastendruck Encoder-Wert für zweite Ebene übernehmen
						encoder_2_button = 'f';
						en_counter2 = 0;
					}
			}
			break;
		case 2:																										//werkszustand
				en_counter2 = check_for_out_of_range(en_counter2,2);
				en_counter2 = en_counter2 % 2;																		//Auf 2 begrenzen(ON/OFF)
				if (encoder_2_button=='k')
				{
					counter2_second_lvl = en_counter2;																//Bei kurzem Tastendruck Encoder-Wert für zweite Ebene übernehmen
					encoder_2_button = 'f';

				}
			break;
		case 3:																										//Equalizer
			en_counter2 = check_for_out_of_range(en_counter2,6);
			en_counter2 = en_counter2 % 6;																			//Auf 6 begrenzen(POP,ROCK,JAZZ;
			if (encoder_2_button=='k')
			{
				counter2_second_lvl = en_counter2;																	//Bei kurzem Tastendruck Encoder-Wert für zweite Ebene übernehmen
				encoder_2_button = 'f';
			}
			break;
		case 4:																										//Audio-Switch
			en_counter2 = check_for_out_of_range(en_counter2,2);
			en_counter2 = en_counter2 % 2;
			if (encoder_2_button =='k')
			{
				counter2_second_lvl = en_counter2;																	//Bei kurzem Tastendruck Encoder-Wert für zweite Ebene übernehmen
				encoder_2_button = 'f';
			}
			break;
		case 5:																										//return
				encoder_2_button = 'f';
				counter2_zero_lvl = MAX_ZERO_LVL;																	//Wieder auf Haupt-
				counter2_first_lvl = MAX_FIRST_LVL;																	//anzeige wechseln
				en_counter2 = 0;
			break;
		
		default:
				en_counter2 = check_for_out_of_range(en_counter2,6);
				en_counter2 = en_counter2 % 6;																		//Auf 6 begrenzen(AM/FM,Anzeige,Equalizer,Audio-Switch,Werkszustand,Return)
				if (encoder_2_button=='k')
				{
					counter2_first_lvl = en_counter2;																//Bei kurzem Tastendruck Encoder-Wert für erste Ebene übernehmen
					encoder_2_button = 'f';
					en_counter2 = 0;
				}
		}
		break;
		default:
		if (encoder_2_button=='k')																					
		{
			counter2_zero_lvl = 1;																					//Bei kurzem Tastendruck Auswahl der Senderliste
			encoder_2_button = 'f';
			en_counter2 = 0;
		}
		else if (encoder_2_button == 'l')								
		{
			counter2_zero_lvl = 2;																					//Bei langem Tastendruck Auswahl Einstellungen
			encoder_2_button = 'f';
			en_counter2 = 0;
		}
	}
	/***************************************************************************ende encoder 2********************************************************************************************/
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////ENDE MENÜ STEUERUNG////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////BEGINN MENÜ-AUSWAHL////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*****************************************************************************ENCODER 1***********************************************************************************************/

	if(counter2_zero_lvl == MAX_ZERO_LVL)
	{
		if (en_counter1 != 0)																						//Wenn sich der Encoder-wert ändert Lautstärke ändern
			{
				act_vol += en_counter1;
				if(act_vol > 60)																					//MAX-VOL erreicht
				{					
					act_vol = 60;

				}
				else if(act_vol < 0 )																				//MIN-VOL erreicht
				{					
					act_vol = 0;

				}
				//Amplifier_Gain(act_vol - 30);
				SI4735_Set_Volume(act_vol);
				en_counter1 = 0;
				delay_vol = 1;
				sekunde = 0;
			}
	}
	
	/********************************************************************************ENDE ENCODER 1***************************************************************************************/

	/**********************************************************************************ENCODER 2******************************************************************************************/
	switch (counter2_zero_lvl)
	{
	case 2:																											//Einstellungen

			switch (counter2_first_lvl)																				//Auswahl AM/FM,Anzeige oder Werkszustand
			{
			case 0:																									//AM/FM

					switch(counter2_second_lvl)
					{
					case 0:																							

							//sel_mode = AM_MODE;
							counter2_first_lvl = MAX_FIRST_LVL;														//Rücksprung zu 
							counter2_second_lvl = MAX_SECOND_LVL;													//Einstellungen
							en_counter2 = 0;																		//Pfeil soll auf AM/FM zeigen
						break;
					case 1:																							

							//sel_mode = FM_MODE;
							counter2_first_lvl = MAX_FIRST_LVL;														//Rücksprung zu
							counter2_second_lvl = MAX_SECOND_LVL;													//Einstellungen
							en_counter2 = 0;																		//Pfeil soll auf AM/FM zeigen
						break;
					default:																						//Anzeige für AM/FM Untermenü anzeigen
						i2c_lcd_create_view("AM",1,0,0);
						i2c_lcd_create_view("FM",1,1,0);
						i2c_lcd_create_view("~",0,en_counter2,1);
					}
					break;
			case 1:																									//Anzeigenauswahl

				switch (counter2_second_lvl)
				{
				case 0:																								//RDS

						switch (counter2_third_lvl)
						{
						case 0:

								//rds_mode = RDS_ON;
								counter2_second_lvl = MAX_SECOND_LVL;												//Rücksprung zu
								counter2_third_lvl = MAX_THIRD_LVL;													//Anzeigen-Untermenü
								en_counter2 = 0;																	//Pfeil soll auf RDS zeigen
								
							break;
						case 1:

								//rds_mode = RDS_OFF;
								counter2_second_lvl = MAX_SECOND_LVL;												//Rücksprung zu
								counter2_third_lvl = MAX_THIRD_LVL;													//Anzeigen-Untermenü
								en_counter2 = 0;																	//Pfeil soll auf RDS zeigen
								
							break;
						default:																					//RDS-Untermenü anzeigen
							i2c_lcd_create_view("RDS",1,0,0);
							i2c_lcd_create_view("PTY",1,1,0);
							i2c_lcd_create_view("~",0,en_counter2,1);
						}
					break;
				case 1:																								//TP/TA

						switch (counter2_third_lvl)
						{
						case 0:

								//tpta_mode = TPTA_ON;
								counter2_second_lvl = MAX_SECOND_LVL;												//Rücksprung zu
								counter2_third_lvl = MAX_THIRD_LVL;													//Anzeigen-Untermenü
								en_counter2 = 2;																	//Pfeil soll auf TPTA zeigen
								
							break;
						case 1:

								//tpta_mode = TPTA_OFF;
								counter2_second_lvl = MAX_SECOND_LVL;												//Rücksprung zu
								counter2_third_lvl = MAX_THIRD_LVL;													//Anzeigen-Untermenü
								en_counter2 = 2;																	//Pfeil soll auf TPTA zeigen
								
							break;
						default:																					//TPTA-Untermenü anzeigen
							i2c_lcd_create_view("TP/TA ON",1,0,0);
							i2c_lcd_create_view("TP/TA OFF",1,1,0);
							i2c_lcd_create_view("~",0,en_counter2,1);
						}
					break;
				default:																							//Anzeige-Untermenü anzeigen
																								// wenn einer der ersten drei elemente ausgewählt ist
						
							i2c_lcd_create_view("RDS/PTY",1,0,0);
							i2c_lcd_create_view("TP/TA",1,1,0);					
							i2c_lcd_create_view("Return",1,2,0);
							i2c_lcd_create_view("~",0,en_counter2,1);
						
				}
				break;
			case 2:																									//Werkszustand

					switch (counter2_second_lvl)
					{
					case 0:
							counter2_first_lvl = MAX_FIRST_LVL;														//Rücksprung zu
							counter2_second_lvl = MAX_SECOND_LVL;													//Einstellungen
							en_counter2 = 2;																		//Pfeil soll auf Werkszustand zeigen						
						break;
					case 1:
							for(i = 0; i < 15; i++)
							{
								stor_data_to_flash ((char *)FLASH_ADR_STATION_NAME, "<Empty>", 0xFF, i*10);			//Speicher löschen
							}
							counter2_first_lvl = MAX_FIRST_LVL;														//Rücksprung zu
							counter2_second_lvl = MAX_SECOND_LVL;													//Einstellungen
							en_counter2 = 2;																		//Pfeil soll auf Werkszustand zeigen
							
						break;
					default:																						//Werkszustand-Untermenü anzeigen
						i2c_lcd_create_view("NEIN",1,0,0);
						i2c_lcd_create_view("JA",1,1,0);
						i2c_lcd_create_view("~",0,en_counter2,1);
					}
				break;
			case 3:																									//Equalizer
				switch (counter2_second_lvl)
				{
				case 0:
					Amplifier_init(POP,AMPLIFIER_GAIN);																//pop
					counter2_first_lvl = MAX_FIRST_LVL;																//Rücksprung zu
					counter2_second_lvl = MAX_SECOND_LVL;															//Einstellungen
					en_counter2 = 3;																				//Pfeil soll auf Equalizer zeigen
					break;
				case 1:
					Amplifier_init(CLASSIC,AMPLIFIER_GAIN);															//Klassik
					counter2_first_lvl = MAX_FIRST_LVL;																//Rücksprung zu
					counter2_second_lvl = MAX_SECOND_LVL;															//Einstellungen
					en_counter2 = 3;																				//Pfeil soll auf Equalizer zeigen
					break;
				case 2:
					Amplifier_init(JAZZ,AMPLIFIER_GAIN);															//Jazz
					counter2_first_lvl = MAX_FIRST_LVL;																//Rücksprung zu
					counter2_second_lvl = MAX_SECOND_LVL;															//Einstellungen
					en_counter2 = 3;																				//Pfeil soll auf Equalizer zeigen
					break;
				case 3:
					Amplifier_init(RAP_HIP_HOP,AMPLIFIER_GAIN);														//Hip-Hop
					counter2_first_lvl = MAX_FIRST_LVL;																//Rücksprung zu
					counter2_second_lvl = MAX_SECOND_LVL;															//Einstellungen
					en_counter2 = 3;																				//Pfeil soll auf Equalizer zeigen
					break;
				case 4:
					Amplifier_init(ROCK,AMPLIFIER_GAIN);															//rock
					counter2_first_lvl = MAX_FIRST_LVL;																//Rücksprung zu
					counter2_second_lvl = MAX_SECOND_LVL;															//Einstellungen
					en_counter2 = 3;																				//Pfeil soll auf Equalizer zeigen
					break;
				case 5:
					Amplifier_init(NEWS_VOICE,AMPLIFIER_GAIN);														//news/voice
					counter2_first_lvl = MAX_FIRST_LVL;																//Rücksprung zu
					counter2_second_lvl = MAX_SECOND_LVL;															//Einstellungen
					en_counter2 = 3;																				//Pfeil soll auf Equalizer zeigen
					break;
				default:																							//Equalizer-Untermenü anzeigen
					if (en_counter2 < 3)																			// wenn einer der ersten drei elemente ausgewählt ist
					{
						i2c_lcd_create_view("Pop",1,0,0);
						i2c_lcd_create_view("Klassik",1,1,0);
						i2c_lcd_create_view("Jazz",1,2,0);
						i2c_lcd_create_view("~",0,en_counter2,1);
					}
					else
					{
						i2c_lcd_create_view("Hip-Hop",1,0,0);						
						i2c_lcd_create_view("Rock",1,1,0);
						i2c_lcd_create_view("News/Voice",1,2,0);
						i2c_lcd_create_view("~",0,en_counter2-3,1);
					}
				}
				break;
			case 4:																									//Audio-Switch
				switch (counter2_second_lvl)
				{
				case 0:																								//AUX-auswahl
					//AudioSwitch(AUX_LEFT,AUX_RIGHT);
					counter2_second_lvl = MAX_SECOND_LVL;															//Rücksprung zu
					counter2_first_lvl = MAX_FIRST_LVL;																//Einstellungen
					en_counter2 = 4;
					radio_on = 0;
					break;
				case 1:
					//AudioSwitch(RADIO_LEFT,RADIO_RIGHT);															//Audio-Switch auf lautsprecher
					counter2_second_lvl = MAX_SECOND_LVL;															//Rücksprung zu
					counter2_first_lvl = MAX_FIRST_LVL;																//Einstellungen
					en_counter2 = 4;
					radio_on = 1;
					break;
				default:
					i2c_lcd_create_view("AUX",1,0,0);
					i2c_lcd_create_view("Radio",1,1,0);
					i2c_lcd_create_view("~",0,en_counter2,1);
				}
				break;
			default:																								//Einstellungen-Untermenü anzeigen
				if (en_counter2 < 3)																				// wenn einer der ersten drei elemente ausgewählt ist
				{
					i2c_lcd_create_view("AM/FM",1,0,0);
					i2c_lcd_create_view("Anzeige",1,1,0);
					i2c_lcd_create_view("Werkszustand",1,2,0);
					i2c_lcd_create_view("~",0,en_counter2,1);
				}
				else
				{
					i2c_lcd_create_view("Equalizer",1,0,0);
					i2c_lcd_create_view("Audio-Switch",1,1,0);
					i2c_lcd_create_view("Return",1,2,0);
					i2c_lcd_create_view("~",0,en_counter2-3,1);
				}

			}
		break;											

		case 1:																										//Senderliste

			switch (counter2_first_lvl)
			{
			case 1:																									//Speichern
				i2c_lcd_create_view("S:",14,0,0);
			case 0:																									//listeneintrag auswählen und Einträge anzeigen
				if(counter2_first_lvl == 0)
				i2c_lcd_create_view("A:",14,0,0);
				for (i=(en_counter2/3)*3;i<(en_counter2/3)*3+3;i++)
				{
					null_befor_value(string, i+1, 0);
					string[2]='.';
					string[3]=0;
					i2c_lcd_create_view(string,1,i%3,0);
					i2c_lcd_create_view((char *)(FLASH_ADR_STATION_NAME+i*10),4,i%3,0);
				}
				i2c_lcd_create_view("~",0,en_counter2%3,1);
				break;
			case 2:																									//Return
				counter2_zero_lvl = MAX_ZERO_LVL;																	//Rücksprung zu
				counter2_first_lvl = MAX_FIRST_LVL;																	//Hauptbildschirm
				en_counter2 = 0;
				break;
			default:
				i2c_lcd_create_view("Senderliste",1,0,0);
				i2c_lcd_create_view("Speichern",1,1,0);
				i2c_lcd_create_view("Return",1,2,0);
				i2c_lcd_create_view("~",0,en_counter2,1);
			}
			break;
		default:																									//auf Frequenzänderung reagieren
			if (en_counter2 != 0)																					//wenn Encoder gedreht wird												
			{
				Radio_States |= (1<<15);
				strncpy(Akt_Radio_Text,"Radio Text",64);
				act_freq += en_counter2;
				if(act_freq > 205)																					//Freq bei 107.8MHz
				{
					act_freq = 1;
				}
				else if(act_freq < 1 )																				//Freq bei 87.5MHz
				{
					act_freq = 205;
				}
				SI4735_Fm_Tune_Freq_2((875+(char)(act_freq-1))*10);
				en_counter2 = 0;
				delay_freq = 1;
				sekunde = 0;
			}
			if(radio_on)
			{
			get_rds_data(&Radio_States, Station_Name, Radion_Text);													//RDS-Daten auslesen
			if(Radio_States & (1<<14))
			{
				i2c_lcd_create_view(Station_Name, 0, 0, 0);
			}
			else																									//ansonsten Frequenz anzeigen
			{
				sprintf(string,"%d.%dMHz",(act_freq+874)/10,(act_freq+874)%10);
				i2c_lcd_create_view(string,0,0,0);
			}
			}
			else
			{
				i2c_lcd_create_view("LINE-IN ",0,0,0);
			}
			if(Radio_States & 0x1000)																				//aktuelles Datum über RDS erhalten
			{
 				Radio_States &= ~0x1000;
				sec = 0;
			}
			time_date(0, 0, 0, 0, 0, 0, 0, s_time, s_date);															//Lese Datum und Zeit String aus
			i2c_lcd_create_view(s_time, 10, 0, 0);																	//Zeige Zeit an
			i2c_lcd_create_view(s_date,8,1,0);																		//Zeige Datum an
			if(vol_mute)																							//vol_mute zeichen ausgeben
			{
				sprintf(string,"\7÷",0);
				i2c_lcd_create_view(string,0,1,0);
			}
			else																									//vol_mute off zeichen ausgeben
			{
				sprintf(string,"\7û",act_vol);
				i2c_lcd_create_view(string,0,1,0);
			}
			if(delay_freq == 1 && sekunde < BLEND_OUT_AFTER)														//Delay für Bargraph Frequenz
			{
				i2c_lcd_bargraph((act_freq*60)/204);
			}
			else
			{
				delay_freq = 0;
			}
			if(delay_vol == 1 && sekunde < BLEND_OUT_AFTER)															//Delay für Bargraph Volume
			{
				sprintf(string,"=%d%c",(act_vol*100)/60,0x25);
				i2c_lcd_create_view(string,1,1,0);
				i2c_lcd_bargraph(act_vol);
			}
			else
			{
				delay_vol = 0;
			}
			if(delay_vol == 0 && delay_freq == 0)																	//Radiotext anzeigen wenn bargraph kein Encoder gedreht wird
			{
				if(posrt < 16)																						//So lange der Text nicht am linken Rand angekommen ist kopiere nur die anzahl der angezeigen Zeichen
				{
					strncpy(string, Akt_Radio_Text, posrt+1);														//Kopiere die Anzahl der Zeichen erscheinen sollen
					i2c_lcd_create_view(string,15-posrt,2,0);														//Text in den Display Speicher ablegen
				}
				else																								//Wenn der Text den bereich unten ausfüllt
				{
					strncpy(string, Akt_Radio_Text+posrt-15, 16);													//Kopiere die zeichen die aktuell durch geschiftet werden sollen
					i2c_lcd_create_view(string,0,2,0);																//Text in den Display Speicher ablegen
				}
				string[16] = 0;																						//String Abschluss einfügen
				if(string[0] == 0)																					//Wenn nix übergeben wird dann
				{
					string[0] = 0x20;																				//Leerzeichen einfügen
					string[1] = 0;																					//String Abschlss einfügen
					i2c_lcd_create_view(string,0,2,0);																//Text in den Display Speicher ablegen
				}
			}
			if(Radio_States & 0x2000 && posrt == 78)																//Radiotext(wenn neu vorhanden) kopieren
			{
				Radio_States &= ~0x2000;
				strncpy(Akt_Radio_Text, Radion_Text, 64);
			}
			i2c_lcd_create_view("",0,0,1);

	}
	/*******************************************************************************ENDE ENCODER 2**************************************************************************************/

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////ENDE MENÜ-AUSWAHL//////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}


void null_befor_value(char *string ,char value, unsigned char pos)
{
	if (value < 10)
	{
		string[pos] = '0';
		string[pos+1] = 0x30 + value;
	}
	else
	{
		string[pos] = 0x30 + value / 10;
		string[pos+1] = 0x30 + value % 10;
	}
}

void time_date(unsigned char t_hour, unsigned char t_minute, unsigned char t_day, unsigned char t_month, unsigned char t_year, unsigned char update, unsigned char minute_up, char *s_time, char *s_date)
{
	static unsigned char minute = 0, hour = 0, day= 1, month =  1, year = 14;
	unsigned char day_per_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	if(update)
	{
		hour = t_hour;
		minute = t_minute;

		day = t_day;
		month = t_month;
		year = t_year;
	}
	if(minute_up)
	{
		if(++minute == 60)
		{
			minute = 0;
			if(++hour == 24)
			{
				hour = 0;
				if(year%4 == 0 && month == 2)
				{
					day_per_month[1] = 29;
				}
				else
				{
					day_per_month[1] = 28;
				}
				if(day++ == day_per_month[month-1])
				{
					day = 1;
					if(++month == 13)
					{
						month = 1;
						year++;
					}
				}
			}
		}
	}
	if(s_date != 0)
	{
		null_befor_value(s_date, day, 0);
		null_befor_value(s_date, month, 3);
		null_befor_value(s_date, year, 6);
	}
	if(s_time != 0)
	{
		null_befor_value(s_time, hour, 1);
		null_befor_value(s_time, minute, 4);
	}
}

char check_for_out_of_range(signed char value,char modulo)
{
	if(value < 0)
	{
		value = modulo-1;
	}
	if(value == (127-(127%modulo)))
	{
		value = modulo;
	}
	return value;
}

void enter_standby(void)
{
	i2c_lcd_create_view("",0, 0, 1);

	P3DIR |=  BIT1;
	P3OUT &=~ BIT1;

	P3DIR |=  BIT2;
	P3OUT &=~  BIT2;

	SI4735_Power_Down();


	_BIS_SR(LPM0_bits + GIE);

}

void exit_standby(void)
{

	Clock_INIT2();

	_delay_ms(500);
	//GPIO_INIT();

	P3DIR |=  BIT1;
	P3OUT |=  BIT1;

	P3DIR |=  BIT2;
	P3OUT |=  BIT2;

	_delay_ms(1);
	i2c_lcd_init();



	Amplifier_init(POP,30);
	//AudioSwitch(0x05,0x01);


	// Init I2C Kommunikation übergebe Adresse des Slaves 0x11h und den Teiler für die Frequenz

	// Initialisiert den Radiochip
	SI4735_Power_Up();
	//SI4735_INIT();
	SI4735_Set_Volume(20);




}
