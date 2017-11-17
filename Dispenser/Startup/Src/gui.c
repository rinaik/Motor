#include "gui.h"

#include "stm32f429i_discovery_ts.h"  // touch-screen header
#include "stm32f429i_discovery_lcd.h" // lcd header

#include <stdlib.h>

//Notes: Screen Size: 240x320

// do flash routine here for now

#include "stm32f4xx_hal_flash.h"
#define ADDR_FLASH_SECTOR_11  ((uint32_t)0x080E0000)
#define location ADDR_FLASH_SECTOR_11

void save_data_to_flash(int data) {
	HAL_FLASH_Unlock();
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGSERR );
    FLASH_Erase_Sector(FLASH_SECTOR_11, VOLTAGE_RANGE_3);
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,ADDR_FLASH_SECTOR_11,data);
	HAL_FLASH_Lock();
}

void LCDInit()
{
	  BSP_LCD_Init();
	  BSP_TS_Init(240,360);

	  BSP_LCD_LayerDefaultInit(1,SDRAM_DEVICE_ADDR);
	  BSP_LCD_SelectLayer(1);

	  BSP_LCD_DisplayOn();
}

void window_1_callback (UG_MESSAGE* msg)
{
    if (msg->type == MSG_TYPE_OBJECT )
    {
    	if (msg->id == OBJ_TYPE_BUTTON )
    	{
    		if (msg->sub_id == BTN_ID_0)
    		{
    		   if (msg->event == OBJ_EVENT_PRESSED) {
    			 gui_state = START;
    			 UG_ButtonSetBackColor (&window_1 , BTN_ID_0, C_GREEN ) ;
    			 UG_ButtonSetBackColor (&window_1 , BTN_ID_1, C_BLUE ) ;
    		     UG_ButtonSetText(&window_1, BTN_ID_0, "PRESS!");
    		   }
    		   else {
    			 gui_state = NO_TOUCH;
    			 UG_ButtonSetForeColor(&window_1, BTN_ID_0, C_BLACK );
    			 UG_ButtonSetText(&window_1, BTN_ID_0, "START");
    		   }
    		}
    		if (msg->sub_id == BTN_ID_1)
    		{
    		  if (msg->event == OBJ_EVENT_PRESSED) {
    			 gui_state = STOP;
    			 UG_ButtonSetBackColor (&window_1 , BTN_ID_0, C_BLUE ) ;
    			 UG_ButtonSetBackColor (&window_1 , BTN_ID_1, C_RED ) ;
    			 UG_ButtonSetText(&window_1, BTN_ID_1, "PRESS!");
    	      }
    	      else {
    			 gui_state = NO_TOUCH;
    			 UG_ButtonSetForeColor(&window_1, BTN_ID_1, C_BLACK );
    			 UG_ButtonSetText(&window_1, BTN_ID_1, "STOP");
    		  }
    		}
    		if (msg->sub_id == BTN_ID_2)
    		{
    		  if (msg->event == OBJ_EVENT_PRESSED) {
    			 gui_state = FWD;
    			 UG_ButtonSetBackColor (&window_1 , BTN_ID_2, C_YELLOW ) ;
    			 UG_ButtonSetBackColor (&window_1 , BTN_ID_3, C_BLUE ) ;
    			 UG_ButtonSetText(&window_1, BTN_ID_2, "PRESS!");
    		  }
    	      else {
    			 gui_state = NO_TOUCH;
                 UG_ButtonSetBackColor (&window_1 , BTN_ID_3, C_BLUE ) ;
    		     UG_ButtonSetText(&window_1, BTN_ID_2, "FWD");
    		  }
    		}
    		if (msg->sub_id == BTN_ID_3)
    		{
    		  if (msg->event == OBJ_EVENT_PRESSED) {
    		     gui_state = REV;
    			 UG_ButtonSetBackColor (&window_1 , BTN_ID_2, C_BLUE ) ;
    			 UG_ButtonSetBackColor (&window_1 , BTN_ID_3, C_YELLOW ) ;
    			 UG_ButtonSetText(&window_1, BTN_ID_3, "PRESS!");
    		  }
    		  else {
    			 gui_state = NO_TOUCH;
    			 UG_ButtonSetBackColor (&window_1 , BTN_ID_2, C_BLUE ) ;
    			 UG_ButtonSetText(&window_1, BTN_ID_3, "REV");
    		  }
    		}
    		if (msg->sub_id == BTN_ID_4)
    		{
    		  if (msg->event == OBJ_EVENT_PRESSED) {
    		     gui_state = PLUS;
    			 UG_ButtonSetBackColor (&window_1 , BTN_ID_4, C_GREEN ) ;
    		     UG_ButtonSetText(&window_1, BTN_ID_4, "PRESS!");
    		     save_data_to_flash(motor_speed);
    		  }
    		  else {

    			 gui_state = NO_TOUCH;
    			 UG_ButtonSetBackColor (&window_1 , BTN_ID_4, C_BLUE ) ;
    		     UG_ButtonSetText(&window_1, BTN_ID_4, "+");
    		  }
    		}
    		if (msg->sub_id == BTN_ID_5)
    		{
    		  if (msg->event == OBJ_EVENT_PRESSED) {
    		     gui_state = MINUS;
    			 UG_ButtonSetBackColor (&window_1 , BTN_ID_5, C_GREEN ) ;
                 UG_ButtonSetText(&window_1, BTN_ID_5, "PRESS!");
    		     save_data_to_flash(motor_speed);
    		  }
    		  else {
    		     gui_state = NO_TOUCH;
    			 UG_ButtonSetBackColor (&window_1 , BTN_ID_5, C_BLUE ) ;
    		     UG_ButtonSetText(&window_1, BTN_ID_5, "-");
    		  }
    	    }
    	}
    }
}

void GUIInit()
{
	 // Create a window

	          UG_WindowCreate(&window_1, obj_buff_wnd_1, MAX_OBJECTS, window_1_callback);

	          // Modify the window title

	          UG_WindowSetTitleText(&window_1,"Dispense System");
	          UG_WindowSetTitleTextFont(&window_1, &FONT_12X20);

	          // Change window fore and back color
	          UG_WindowSetForeColor(&window_1, C_WHITE);
	          UG_WindowSetBackColor(&window_1, C_BLUE);

	          // Create some buttons
	          UG_ButtonCreate(&window_1, &button_1, BTN_ID_0, 10, 10, 110, 60);
	          UG_ButtonCreate(&window_1, &button_2, BTN_ID_1, 10, 80, 110, 130);
	          UG_ButtonCreate(&window_1, &button_3, BTN_ID_2, 10, 150, 110, 200);
	          UG_ButtonCreate(&window_1, &button_4, BTN_ID_3, 10, 220, 110, 270);
	          UG_ButtonCreate(&window_1, &button_5, BTN_ID_4, 130, 150, 220, 200);
	          UG_ButtonCreate(&window_1, &button_6, BTN_ID_5, 130, 220, 220, 270);

	          // Label the buttons
	          UG_ButtonSetForeColor(&window_1, BTN_ID_0, C_GREEN);
	          UG_ButtonSetForeColor(&window_1, BTN_ID_1, C_BLACK);
	          UG_ButtonSetBackColor(&window_1, BTN_ID_1, C_RED);
	          UG_ButtonSetForeColor(&window_1, BTN_ID_2, C_BLACK);
	          UG_ButtonSetBackColor(&window_1, BTN_ID_2, C_YELLOW);
	          UG_ButtonSetForeColor(&window_1, BTN_ID_3, C_BLACK);
	          UG_ButtonSetForeColor(&window_1, BTN_ID_4, C_BLACK);
	          UG_ButtonSetForeColor(&window_1, BTN_ID_5, C_BLACK);

	          UG_ButtonSetFont ( &window_1, BTN_ID_0, &FONT_12X20);
	          UG_ButtonSetText ( &window_1, BTN_ID_0, "START");
	          UG_ButtonSetFont ( &window_1, BTN_ID_1, &FONT_12X20);
	          UG_ButtonSetText ( &window_1, BTN_ID_1, "STOP");
	          UG_ButtonSetFont ( &window_1, BTN_ID_2, &FONT_12X20);
	          UG_ButtonSetText ( &window_1, BTN_ID_2, "FWD");
	          UG_ButtonSetFont ( &window_1, BTN_ID_3, &FONT_12X20);
	          UG_ButtonSetText ( &window_1, BTN_ID_3, "REV");

	          UG_ButtonSetFont ( &window_1, BTN_ID_4, &FONT_12X20);
	          UG_ButtonSetText ( &window_1, BTN_ID_4, "+");

	          UG_ButtonSetFont ( &window_1, BTN_ID_5, &FONT_12X20);
	          UG_ButtonSetText ( &window_1, BTN_ID_5, "-");

	          //  Create Textboxs

	          UG_TextboxCreate( &window_1 , &textbox_1 , TXB_ID_0 , 130 , 10 , 220 , 40 );
	          UG_TextboxSetFont ( &window_1 , TXB_ID_0 , &FONT_12X20 ) ;
	          UG_TextboxSetText ( &window_1 , TXB_ID_0, "SPEED:" );
	          UG_TextboxSetAlignment ( &window_1 , TXB_ID_0 , ALIGN_TOP_CENTER );


	          UG_TextboxCreate( &window_1 , &textbox_2 , TXB_ID_1 , 130 , 40 , 220 , 70 );
	          UG_TextboxSetFont ( &window_1 , TXB_ID_1 , &FONT_12X20 );
	          motor_speed = *(int*)location;
	          itoa(motor_speed,buffer,10);
	          UG_TextboxSetText ( &window_1 , TXB_ID_1, buffer);
	          UG_TextboxSetAlignment ( &window_1 , TXB_ID_1 , ALIGN_TOP_CENTER );

	          UG_TextboxCreate( &window_1 , &textbox_3 , TXB_ID_2 , 130 , 80 , 220 , 110 );
	          UG_TextboxSetFont ( &window_1 , TXB_ID_2 , &FONT_12X20 ) ;
	          UG_TextboxSetText ( &window_1 , TXB_ID_2, "I/O:");
	          UG_TextboxSetAlignment ( &window_1 , TXB_ID_2 , ALIGN_TOP_CENTER );

	          UG_TextboxCreate( &window_1 , &textbox_4 , TXB_ID_3 , 130 , 110 , 220 , 140 );
	          UG_TextboxSetFont ( &window_1 , TXB_ID_3 , &FONT_12X20 ) ;
	          UG_TextboxSetText ( &window_1 , TXB_ID_3, "OFF");
	          UG_TextboxSetAlignment ( &window_1 , TXB_ID_3 , ALIGN_TOP_CENTER );

              UG_WindowShow(&window_1);
}


void UserPixelSet(UG_S16 x, UG_S16 y, UG_COLOR c) {BSP_LCD_DrawPixel(x, y, c);}

void TouchPress(void) {
	TS_StateTypeDef TS_State;

	BSP_TS_GetState(&TS_State);
	if ( TS_State.TouchDetected ) {
		UG_TouchUpdate(TS_State.X, TS_State.Y, TOUCH_STATE_PRESSED);
		}
	else {
		UG_TouchUpdate( -1, -1, TOUCH_STATE_RELEASED);
	}
}





