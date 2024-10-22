// https://github.com/alekmaul/pvsneslib/wiki

#include <snes.h>
#include "fp_math.h"

// Sprites
extern char johnspr, johnspr_end;
extern char johnpal, johnpal_end;

// BGs
/*
extern char coltestbg, coltestbg_end;
extern char coltestpal, coltestpal_end;
extern char coltestmap, coltestmap_end;
*/

// Screen Stuffs
#define RES_X			256
#define RES_Y			224

// Memory Stuffs
#define MEM_SPRITES			0x4000
#define MEM_BACKGROUNDS		0x6000
#define MEM_MAPS			0x0000

u16 pad0, storePad0;	// pad0 = Current Input
						// storePad0 = Last Frame's Input

int main(void){
	// Init SNES
	consoleInit();
	
	// Init OAM stuffs.
	// The OAM ID should be in increments of 4.
	// ex. Player = 0, Enemy = 4
	oamInitGfxSet(
		&johnspr, (&johnspr_end - &johnspr),	// Sprites + Length
		&johnpal, (&johnpal_end - &johnpal),	// Palette + Length
		0,										// Palette Bank
		MEM_SPRITES,							// Where to put sprites.
		3 << 5									// Size of sprites.
												// https://github.com/alekmaul/pvsneslib/wiki/Sprites#sprite-sizes
	);
	oamSet(0, 100, 100, 3, 0, 0, 0, 0);
    oamSetEx(0, OBJ_SMALL, 1);
    oamSetVisible(0, OBJ_SHOW);
	
	// This needs to be done after ANY new backgrounds are loaded.
	// All accessible backgrounds are enabled by default.
	bgSetDisable(1);
	bgSetDisable(2);
	setScreenOn();
	
	// Activate Mode1 and set tile size to 8x8.
	setMode(BG_MODE1, 0);

	// Logo Loop.
	// Show logos for ~3 seconds.
	// Fade to black in ~1 second.
	/*unsigned int frameCounter = 0;
	while(frameCounter < 60 * 4){
		// TODO
		frameCounter++;
		WaitForVBlank();
	}
	
	// Main Menu Loop.
	// Show main menu until pressed start.
	// Fade to black in ~1 second.
	u8 showMainMenu = 1;
	while(showMainMenu){
		// TODO
		showMainMenu = 1;
		WaitForVBlank();
	}*/
	
	// Game Loop.
	while (1){
		// Updating relevant objects.
		oamSet(0, 100, 100, 3, 0, 0, 0, 0);
		
		WaitForVBlank();
	}
	return 0;
}