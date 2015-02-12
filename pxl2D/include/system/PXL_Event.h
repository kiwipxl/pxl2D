#ifndef _PXL_EVENT_H
#define _PXL_EVENT_H

#include <string>
#include <vector>
#include "PXL_Window.h"

#define PXL_EVENT_NULL			WM_NULL
#define PXL_EVENT_QUIT			WM_QUIT /**> Event message that occurs when a window sends a termination message **/

#define PXL_EVENT_KEYDOWN		WM_KEYDOWN
#define PXL_EVENT_KEYUP			WM_KEYUP

#define PXL_EVENT_MOVE			WM_MOVE
#define PXL_EVENT_SIZE			WM_SIZE
#define PXL_EVENT_ACTIVATE		WM_ACTIVATE
#define PXL_EVENT_SETFOCUS		WM_SETFOCUS

#define PXL_KEY_UNKNOWN					0x00	 /**< Unknown keyidentifier **/
#define PXL_KEY_LBUTTON					0x01	 /**< Left mouse button **/
#define PXL_KEY_RBUTTON					0x02	 /**< Right mouse button **/
#define PXL_KEY_CANCEL					0x03	 /**< Control-break processing **/
#define PXL_KEY_MBUTTON					0x04	 /**< Middle mouse button (three-button mouse) **/
#define PXL_KEY_XBUTTON1				0x05	 /**< X1 mouse button **/
#define PXL_KEY_XBUTTON2				0x06	 /**< X2 mouse button **/
#define PXL_KEY_BACK					0x08	 /**< BACKSPACE key **/
#define PXL_KEY_TAB						0x09	 /**< TAB key **/
#define PXL_KEY_CLEAR					0x0C	 /**< CLEAR key **/
#define PXL_KEY_RETURN					0x0D	 /**< ENTER key **/
#define PXL_KEY_SHIFT					0x10	 /**< SHIFT key **/
#define PXL_KEY_CONTROL					0x11	 /**< CTRL key **/
#define PXL_KEY_MENU					0x12	 /**< ALT key **/
#define PXL_KEY_PAUSE					0x13	 /**< PAUSE key **/
#define PXL_KEY_CAPITAL					0x14	 /**< CAPS LOCK key **/
#define PXL_KEY_KANA					0x15	 /**< IME Kana mode **/
#define PXL_KEY_HANGUEL					0x15	 /**< IME Hanguel mode (maintained for compatibility; use PXL_KEY_HANGUL) **/
#define PXL_KEY_HANGUL					0x15	 /**< IME Hangul mode **/
#define PXL_KEY_JUNJA					0x17	 /**< IME Junja mode **/
#define PXL_KEY_FINAL					0x18	 /**< IME final mode **/
#define PXL_KEY_HANJA					0x19	 /**< IME Hanja mode **/
#define PXL_KEY_KANJI					0x19	 /**< IME Kanji mode **/
#define PXL_KEY_ESCAPE					0x1B	 /**< ESC key **/
#define PXL_KEY_CONVERT					0x1C	 /**< IME convert **/
#define PXL_KEY_NONCONVERT				0x1D	 /**< IME nonconvert **/
#define PXL_KEY_ACCEPT					0x1E	 /**< IME accept **/
#define PXL_KEY_MODECHANGE				0x1F	 /**< IME mode change request **/
#define PXL_KEY_SPACE					0x20	 /**< SPACEBAR **/
#define PXL_KEY_PRIOR					0x21	 /**< PAGE UP key **/
#define PXL_KEY_NEXT					0x22	 /**< PAGE DOWN key **/
#define PXL_KEY_END						0x23	 /**< END key **/
#define PXL_KEY_HOME					0x24	 /**< HOME key **/
#define PXL_KEY_LEFT					0x25	 /**< LEFT ARROW key **/
#define PXL_KEY_UP						0x26	 /**< UP ARROW key **/
#define PXL_KEY_RIGHT					0x27	 /**< RIGHT ARROW key **/
#define PXL_KEY_DOWN					0x28	 /**< DOWN ARROW key **/
#define PXL_KEY_SELECT					0x29	 /**< SELECT key **/
#define PXL_KEY_PRINT					0x2A	 /**< PRINT key **/
#define PXL_KEY_EXECUTE					0x2B	 /**< EXECUTE key **/
#define PXL_KEY_SNAPSHOT				0x2C	 /**< PRINT SCREEN key **/
#define PXL_KEY_INSERT					0x2D	 /**< INS key **/
#define PXL_KEY_DELETE					0x2E	 /**< DEL key **/
#define PXL_KEY_HELP					0x2F	 /**< HELP_KEY **/
#define PXL_KEY_0						0x30	 /**< Key 0 **/
#define PXL_KEY_1						0x31	 /**< Key 1 **/
#define PXL_KEY_2						0x32	 /**< Key 2 **/
#define PXL_KEY_3						0x33	 /**< Key 3 **/
#define PXL_KEY_4						0x34	 /**< Key 4 **/
#define PXL_KEY_5						0x35	 /**< Key 5 **/
#define PXL_KEY_6						0x36	 /**< Key 6 **/
#define PXL_KEY_7						0x37	 /**< Key 7 **/
#define PXL_KEY_8						0x38	 /**< Key 8 **/
#define PXL_KEY_9						0x39	 /**< Key 9 **/
#define PXL_KEY_A						0x41	 /**< Key A **/
#define PXL_KEY_B						0x42	 /**< Key B **/
#define PXL_KEY_C						0x43	 /**< Key C **/
#define PXL_KEY_D						0x44	 /**< Key D **/
#define PXL_KEY_E						0x45	 /**< Key E **/
#define PXL_KEY_F						0x46	 /**< Key F **/
#define PXL_KEY_G						0x47	 /**< Key G **/
#define PXL_KEY_H						0x48	 /**< Key H **/
#define PXL_KEY_I						0x49	 /**< Key I **/
#define PXL_KEY_J						0x4A	 /**< Key J **/
#define PXL_KEY_K						0x4B	 /**< Key K **/
#define PXL_KEY_L						0x4C	 /**< Key L **/
#define PXL_KEY_M						0x4D	 /**< Key M **/
#define PXL_KEY_N						0x4E	 /**< Key N **/
#define PXL_KEY_O						0x4F	 /**< Key O **/
#define PXL_KEY_P						0x50	 /**< Key P **/
#define PXL_KEY_Q						0x51	 /**< Key Q **/
#define PXL_KEY_R						0x52	 /**< Key R **/
#define PXL_KEY_S						0x53	 /**< Key S **/
#define PXL_KEY_T						0x54	 /**< Key T **/
#define PXL_KEY_U						0x55	 /**< Key U **/
#define PXL_KEY_V						0x56	 /**< Key V **/
#define PXL_KEY_W						0x57	 /**< Key W **/
#define PXL_KEY_X						0x58	 /**< Key X **/
#define PXL_KEY_Y						0x59	 /**< Key Y **/
#define PXL_KEY_Z						0x5A	 /**< Key Z **/
#define PXL_KEY_LWIN					0x5B	 /**< Left Windows key(Natural keyboard)  **/
#define PXL_KEY_RWIN					0x5C	 /**< Right Windows key(Natural keyboard)  **/
#define PXL_KEY_APPS					0x5D	 /**< Applications key(Natural keyboard)  **/
#define PXL_KEY_SLEEP					0x5F	 /**< Computer Sleep key **/
#define PXL_KEY_NUMPAD0					0x60	 /**< Numeric keypad 0 key **/
#define PXL_KEY_NUMPAD1					0x61	 /**< Numeric keypad 1 key **/
#define PXL_KEY_NUMPAD2					0x62	 /**< Numeric keypad 2 key **/
#define PXL_KEY_NUMPAD3					0x63	 /**< Numeric keypad 3 key **/
#define PXL_KEY_NUMPAD4					0x64	 /**< Numeric keypad 4 key **/
#define PXL_KEY_NUMPAD5					0x65	 /**< Numeric keypad 5 key **/
#define PXL_KEY_NUMPAD6					0x66	 /**< Numeric keypad 6 key **/
#define PXL_KEY_NUMPAD7					0x67	 /**< Numeric keypad 7 key **/
#define PXL_KEY_NUMPAD8					0x68	 /**< Numeric keypad 8 key **/
#define PXL_KEY_NUMPAD9					0x69	 /**< Numeric keypad 9 key **/
#define PXL_KEY_MULTIPLY				0x6A	 /**< Multiply key **/
#define PXL_KEY_ADD						0x6B	 /**< Add key **/
#define PXL_KEY_SEPARATOR				0x6C	 /**< Separator key **/
#define PXL_KEY_SUBTRACT				0x6D	 /**< Subtract key **/
#define PXL_KEY_DECIMAL					0x6E	 /**< Decimal key **/
#define PXL_KEY_DIVIDE					0x6F	 /**< Divide key **/
#define PXL_KEY_F1						0x70	 /**< F1 key **/
#define PXL_KEY_F2						0x71	 /**< F2 key **/
#define PXL_KEY_F3						0x72	 /**< F3 key **/
#define PXL_KEY_F4						0x73	 /**< F4 key **/
#define PXL_KEY_F5						0x74	 /**< F5 key **/
#define PXL_KEY_F6						0x75	 /**< F6 key **/
#define PXL_KEY_F7						0x76	 /**< F7 key **/
#define PXL_KEY_F8						0x77	 /**< F8 key **/
#define PXL_KEY_F9						0x78	 /**< F9 key **/
#define PXL_KEY_F10						0x79	 /**< F10 key **/
#define PXL_KEY_F11						0x7A	 /**< F11 key **/
#define PXL_KEY_F12						0x7B	 /**< F12 key **/
#define PXL_KEY_F13						0x7C	 /**< F13 key **/
#define PXL_KEY_F14						0x7D	 /**< F14 key **/
#define PXL_KEY_F15						0x7E	 /**< F15 key **/
#define PXL_KEY_F16						0x7F	 /**< F16 key **/
#define PXL_KEY_F17						0x80	 /**< F17 key **/
#define PXL_KEY_F18						0x81	 /**< F18 key **/
#define PXL_KEY_F19						0x82	 /**< F19 key **/
#define PXL_KEY_F20						0x83	 /**< F20 key **/
#define PXL_KEY_F21						0x84	 /**< F21 key **/
#define PXL_KEY_F22						0x85	 /**< F22 key **/
#define PXL_KEY_F23						0x86	 /**< F23 key **/
#define PXL_KEY_F24						0x87	 /**< F24 key **/
#define PXL_KEY_NUMLOCK					0x90	 /**< NUM LOCK key **/
#define PXL_KEY_SCROLL					0x91	 /**< SCROLL LOCK key **/
#define PXL_KEY_NUMLOCK					0x90	 /**< NUM LOCK key **/
#define PXL_KEY_SCROLL					0x91	 /**< SCROLL LOCK key **/
#define PXL_KEY_LSHIFT					0xA0	 /**< Left SHIFT key **/
#define PXL_KEY_RSHIFT					0xA1	 /**< Right SHIFT key **/
#define PXL_KEY_LCONTROL				0xA2	 /**< Left CONTROL key **/
#define PXL_KEY_RCONTROL				0xA3	 /**< Right CONTROL key **/
#define PXL_KEY_LMENU					0xA4	 /**< Left MENU key **/
#define PXL_KEY_RMENU					0xA5	 /**< Right MENU key **/
#define PXL_KEY_BROWSER_BACK			0xA6	 /**< Browser Back key **/
#define PXL_KEY_BROWSER_FORWARD			0xA7	 /**< Browser Forward key **/
#define PXL_KEY_BROWSER_REFRESH			0xA8	 /**< Browser Refresh key **/
#define PXL_KEY_BROWSER_STOP			0xA9	 /**< Browser Stop key **/
#define PXL_KEY_BROWSER_SEARCH			0xAA	 /**< Browser Search key **/
#define PXL_KEY_BROWSER_FAVORITES		0xAB	 /**< Browser Favorites key **/
#define PXL_KEY_BROWSER_HOME			0xAC	 /**< Browser Start and Home key **/
#define PXL_KEY_VOLUME_MUTE				0xAD	 /**< Volume Mute key **/
#define PXL_KEY_VOLUME_DOWN				0xAE	 /**< Volume Down key **/
#define PXL_KEY_VOLUME_UP				0xAF	 /**< Volume Up key **/
#define PXL_KEY_MEDIA_NEXT_TRACK		0xB0	 /**< Next Track key **/
#define PXL_KEY_MEDIA_PREV_TRACK		0xB1	 /**< Previous Track key **/
#define PXL_KEY_MEDIA_STOP				0xB2	 /**< Stop Media key **/
#define PXL_KEY_MEDIA_PLAY_PAUSE		0xB3	 /**< Play/Pause Media key **/
#define PXL_KEY_LAUNCH_MAIL				0xB4	 /**< Start Mail key **/
#define PXL_KEY_LAUNCH_MEDIA_SELECT		0xB5	 /**< Select Media key **/
#define PXL_KEY_LAUNCH_APP1				0xB6	 /**< Start Application 1 key **/
#define PXL_KEY_LAUNCH_APP2				0xB7	 /**< Start Application 2 key **/
#define PXL_KEY_OEM_1					0xBA	 /**< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the \';:\' keyPXL_OEM_PLUS  **/
#define PXL_KEY_OEM_PLUS				0xBB	 /**< For any country/region, the \'+\' key **/
#define PXL_KEY_OEM_COMMA				0xBC	 /**< For any country/region, the \',\' key **/
#define PXL_KEY_OEM_MINUS				0xBD	 /**< For any country/region, the \'-\' key **/
#define PXL_KEY_OEM_PERIOD				0xBE	 /**< For any country/region, the \'.\' key **/
#define PXL_KEY_OEM_2					0xBF	 /**< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the \'/?\' key **/
#define PXL_KEY_OEM_3					0xC0	 /**< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the \'`~\' key **/
#define PXL_KEY_OEM_4					0xDB	 /**< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the \'[{\' key **/
#define PXL_KEY_OEM_5					0xDC	 /**< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the \'\\|\' key **/
#define PXL_KEY_OEM_6					0xDD	 /**< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the \']}\' key **/
#define PXL_KEY_OEM_7					0xDE	 /**< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the \'single-quote/double-quote\' key **/
#define PXL_KEY_OEM_8					0xDF	 /**< Used for miscellaneous characters; it can vary by keyboard.  **/
#define PXL_KEY_OEM_102					0xE2	 /**< Either the angle bracket keyor the backslash keyon the RT 102-keykeyboard  **/
#define PXL_KEY_PROCESS					0xE5	 /**< IME PROCESS key **/
#define PXL_KEY_PACKET					0xE7	 /**< Used to pass Unicode characters as if they were keystrokes. The PXL_PACKET keyis the low word of a 32-bit Virtual keyvalue used for non-keyboard input methods. For more information, see Remark in KEYBDINPUT, SendInput, WM_KEYDOWN, and WM_KEYUP  **/
#define PXL_KEY_PACKET					0xE7	 /**< Used to pass Unicode characters as if they were keystrokes. The PXL_PACKET keyis the low word of a 32-bit Virtual keyvalue used for non-keyboard input methods. For more information, see Remark in KEYBDINPUT, SendInput, WM_KEYDOWN, and WM_KEYUP  **/
#define PXL_KEY_ATTN					0xF6	 /**< Attn key **/
#define PXL_KEY_CRSEL					0xF7	 /**< CrSel key **/
#define PXL_KEY_EXSEL					0xF8	 /**< ExSel key **/
#define PXL_KEY_EREOF					0xF9	 /**< Erase EOF key **/
#define PXL_KEY_PLAY					0xFA	 /**< Play key **/
#define PXL_KEY_ZOOM					0xFB	 /**< Zoom key **/
#define PXL_KEY_NONAME					0xFC	 /**< Reserved  **/
#define PXL_KEY_PA1						0xFD	 /**< PA1 key **/
#define PXL_KEY_OEM_CLEAR				0xFE	 /**< Clear key **/

class PXL_Event {

	public:
		PXL_Event() { }

		HWND window_handle;
		uint key_code;
		uint type; /**> The message value **/
		ulong time; /**> The time value for when this event was posted **/
};

extern bool PXL_poll_event(PXL_Event& e);

#endif