/* Explicit virtual bindings; physical mapping is in control_events.c. */
#include "doomkeys.h"
#include "m_controls.h"
void episode1_bind_controls(void){
 key_menu_activate=KEY_ESCAPE;
 key_menu_forward=KEY_ENTER;
 key_menu_confirm=KEY_ENTER;
 key_menu_abort=KEY_ESCAPE;
 key_nextweapon=0xf1;
 key_speed=0xf0;
 key_strafe=KEY_LALT;
}
