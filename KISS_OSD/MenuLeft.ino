/* 1. Information
//=========================================================================================================================

Fonctions needed for the settings-menu off the OSD.

//=========================================================================================================================
//START OF CODE
*/

static uint8_t MenuPageLeft = 1;
static uint8_t OldMenuPageLeft = 0;
static uint8_t CursorlineMaxLeft = 0;
static uint8_t CursorlineLeft = 0;
static uint8_t CursorlineOldLeft = 0;
static uint8_t PauseLeft = 0; //makes a PauseLeft of multiples of 50ms

void MenuLeft_Main()
{
	boolean exitmenu = false;
	Menuall_start(GET_SETTINGS);

	while (!exitmenu && KissTelemetrie.armed == 0)
	{
		if (micros() - KissStatus.LastLoopTime > 100000) //limits the speed of the OSD to 20Hz  millis() - LastLoopTimeMenu > 100
		{
			KissStatus.LastLoopTime = micros();
			getSerialData(GET_TELEMETRY);

			//set cursor position
			if (KissTelemetrie.StickChanVals[2] < -800 && CursorlineLeft<CursorlineMaxLeft)
			{
				CursorlineLeft++;
				KissTelemetrie.StickChanVals[2] = 0;
				PauseLeft = 5;
			}
			if (KissTelemetrie.StickChanVals[2] > 800 && CursorlineLeft>1)
			{
				CursorlineLeft--;
				KissTelemetrie.StickChanVals[2] = 0;
				PauseLeft = 5;
			}

			//changevalue
			if (KissTelemetrie.StickChanVals[1] > 800)
			{
				MenuLeft_Valie(true);
			}
			if (KissTelemetrie.StickChanVals[1] < -800)
			{
				MenuLeft_Valie(false);
			}

			//changepage
			if (KissTelemetrie.StickChanVals[3] > 800)
			{
				if (MenuPageLeft<1)
				{
					MenuPageLeft++;
					KissTelemetrie.StickChanVals[3] = 0;
					PauseLeft = 5;
				}
				else
					exitmenu = true;
			}
			if (KissTelemetrie.StickChanVals[3] < -800)
			{
				if (MenuPageLeft > 1)
				{
					MenuPageLeft--;
					KissTelemetrie.StickChanVals[3] = 0;
					PauseLeft = 5;
				}
				else
					exitmenu = true;
			}


			//redraw menu site
			if (MenuPageLeft != OldMenuPageLeft)
			{
				MenuLeft_PrintSite();
				OldMenuPageLeft = MenuPageLeft;
			}

			//redraw MenuLeft_Valie
			MenuLeft_PrintValue();

			//draw cursor position
			if (CursorlineLeft != CursorlineOldLeft && MenuPageLeft != 7)
			{
				//there are not only normal rows here. so we need to place the cursor marker freely on the screen
				MenuLeft_Marker(false,CursorlineOldLeft,MenuPageLeft);		//clear the marker
				MenuLeft_Marker(true,CursorlineLeft,MenuPageLeft);			//set the new marker
			}

			//PauseLeft (multiples of 50ms)
			while (PauseLeft >0)
			{
				delay(50);
				PauseLeft--;
			}
		}
		//reset wdt
		//wdt_reset();
	}

	MenuAll_Exit(GET_SETTINGS);

}

//printing the actual menu page
void MenuLeft_PrintSite() {

	OSD.clear();
	while (OSD.clearIsBusy());
	OSD.home();
	CursorlineLeft = 1;
	CursorlineOldLeft = 0;

	switch (MenuPageLeft)
	{
	case 1:
		//PID
		OSD.grayBackground();
		OSD.print(F("SAMUD OSD - P1/8 PID        "));
		OSD.setCursor(0, 14);
		OSD.print(F(" <-YAW-> : PAGE / EXIT      "));
		OSD.setCursor(9, 2);
		OSD.print(F("P   "));
		OSD.setCursor(16, 2);
		OSD.print(F("I    "));
		OSD.setCursor(24, 2);
		OSD.print(F("D   "));
		OSD.setCursor(9, 7);
		OSD.print(F("RC-RT"));
		OSD.setCursor(16, 7);
		OSD.print(F("RATE "));
		OSD.setCursor(24, 7);
		OSD.print(F("RC-CV"));
		OSD.videoBackground();

		OSD.setCursor(1, 3);
		OSD.print(F("ROLL"));
		OSD.setCursor(1, 4);
		OSD.print(F("PITCH"));
		OSD.setCursor(1, 5);
		OSD.print(F("YAW"));
		OSD.setCursor(1, 8);
		OSD.print(F("ROLL"));
		OSD.setCursor(1, 9);
		OSD.print(F("PITCH"));
		OSD.setCursor(1, 10);
		OSD.print(F("YAW"));
		CursorlineMaxLeft = 18;
		break;
	}
}

//print the actual menu-value
void MenuLeft_PrintValue() {
	switch (MenuPageLeft)
	{
	case 1:
		//PID
		OSD.setCursor(9, 3);
		OSD.print(((float)KissSettings.PID_P[0]) / 1000);
		OSD.print(" ");
		OSD.setCursor(16, 3);
		OSD.print(((float)KissSettings.PID_I[0]) / 1000);
		OSD.print(" ");
		OSD.setCursor(24, 3);
		OSD.print(((float)KissSettings.PID_D[0]) / 1000);
		OSD.print(" ");
		OSD.setCursor(9, 4);
		OSD.print(((float)KissSettings.PID_P[1]) / 1000);
		OSD.print(" ");
		OSD.setCursor(16, 4);
		OSD.print(((float)KissSettings.PID_I[1]) / 1000);
		OSD.print(" ");
		OSD.setCursor(24, 4);
		OSD.print(((float)KissSettings.PID_D[1]) / 1000);
		OSD.print(" ");
		OSD.setCursor(9, 5);
		OSD.print(((float)KissSettings.PID_P[2]) / 1000);
		OSD.print(" ");
		OSD.setCursor(16, 5);
		OSD.print(((float)KissSettings.PID_I[2]) / 1000);
		OSD.print(" ");
		OSD.setCursor(24, 5);
		OSD.print(((float)KissSettings.PID_D[2]) / 1000);
		OSD.print(" ");

		OSD.setCursor(9, 8);
		OSD.print(((float)KissSettings.RC_Rate[0]) / 1000);
		OSD.print(" ");
		OSD.setCursor(16, 8);
		OSD.print(((float)KissSettings.Rate[0]) / 1000);
		OSD.print(" ");
		OSD.setCursor(24, 8);
		OSD.print(((float)KissSettings.RC_Curve[0]) / 1000);
		OSD.print(" ");
		OSD.setCursor(9, 9);
		OSD.print(((float)KissSettings.RC_Rate[1]) / 1000);
		OSD.print(" ");
		OSD.setCursor(16, 9);
		OSD.print(((float)KissSettings.Rate[1]) / 1000);
		OSD.print(" ");
		OSD.setCursor(24, 9);
		OSD.print(((float)KissSettings.RC_Curve[1]) / 1000);
		OSD.print(" ");
		OSD.setCursor(9, 10);
		OSD.print(((float)KissSettings.RC_Rate[2]) / 1000);
		OSD.print(" ");
		OSD.setCursor(16, 10);
		OSD.print(((float)KissSettings.Rate[2]) / 1000);
		OSD.print(" ");
		OSD.setCursor(24, 10);
		OSD.print(((float)KissSettings.RC_Curve[2]) / 1000);
		OSD.print(" ");
		break;	
	}
}

void MenuLeft_Valie(bool addsub)
{
	switch (MenuPageLeft)
	{
	case 1:
		//PID
		switch (CursorlineLeft)
		{
		case 1: changeval(addsub, 0, 67000, 100, &KissSettings.PID_P[0]);
			break;
		case 2: changeval(addsub, 0, 67000, 10, &KissSettings.PID_I[0]);
			break;
		case 3: changeval(addsub, 0, 67000, 100, &KissSettings.PID_D[0]);
			break;
		case 4: changeval(addsub, 0, 67000, 100, &KissSettings.PID_P[1]);
			break;
		case 5: changeval(addsub, 0, 67000, 10, &KissSettings.PID_I[1]);
			break;
		case 6: changeval(addsub, 0, 67000, 100, &KissSettings.PID_D[1]);
			break;
		case 7: changeval(addsub, 0, 67000, 100, &KissSettings.PID_P[2]);
			break;
		case 8: changeval(addsub, 0, 67000, 10, &KissSettings.PID_I[2]);
			break;
		case 9: changeval(addsub, 0, 67000, 100, &KissSettings.PID_D[2]);
			break;

		case 10: changeval(addsub, 0, 67000, 100, &KissSettings.RC_Rate[0]);
			break;
		case 11: changeval(addsub, 0, 67000, 100, &KissSettings.Rate[0]);
			break;
		case 12: changeval(addsub, 0, 67000, 100, &KissSettings.RC_Curve[0]);
			break;
		case 13: changeval(addsub, 0, 67000, 100, &KissSettings.RC_Rate[1]);
			break;
		case 14: changeval(addsub, 0, 67000, 100, &KissSettings.Rate[1]);
			break;
		case 15: changeval(addsub, 0, 67000, 100, &KissSettings.RC_Curve[1]);
			break;
		case 16: changeval(addsub, 0, 67000, 100, &KissSettings.RC_Rate[2]);
			break;
		case 17: changeval(addsub, 0, 67000, 100, &KissSettings.Rate[2]);
			break;
		case 18: changeval(addsub, 0, 67000, 100, &KissSettings.RC_Curve[2]);
			break;
		}
		break;
	}
	PauseLeft = 1;
}

void MenuLeft_Marker(bool addMarker, uint8_t MarkerLine, uint8_t CurrentPage)
{
	switch (CurrentPage)
	{
	case 1:
		//PID-Page
		if (MarkerLine > 9)
			i = 3;
		else
			i = 0;
		while (MarkerLine > 3)
		{
			MarkerLine = -3;
			i + 1;
		}
		switch (MarkerLine)
		{
		case 1:
			OSD.setCursor(8, 3 + i);
			break;
		case 2:
			OSD.setCursor(15, 3 + i);
				break;
		case 3:
			OSD.setCursor(23, 3 + i);
				break;
		}
	}

	if (addMarker)
		OSD.print(F(" "));
	else
		OSD.print(F(">"));
}













