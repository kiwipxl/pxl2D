#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "API.h"

namespace pxl { namespace input {

    struct KeyEvent {

	    bool key_down = false;
    };

    extern KeyEvent* keys;

    typedef uint32 KeyCode;

    extern bool key_down(KeyCode key_code);
    extern bool key_up(KeyCode key_code);

    #define KEY_UNKNOWN					0x00	 /**< Unknown keyidentifier **/
    #define KEY_LBUTTON					0x01	 /**< Left mouse button **/
    #define KEY_RBUTTON					0x02	 /**< Right mouse button **/
    #define KEY_CANCEL					0x03	 /**< Control-break processing **/
    #define KEY_MBUTTON					0x04	 /**< Middle mouse button (three-button mouse) **/
    #define KEY_XBUTTON1				0x05	 /**< X1 mouse button **/
    #define KEY_XBUTTON2				0x06	 /**< X2 mouse button **/
    #define KEY_BACK					0x08	 /**< BACKSPACE key **/
    #define KEY_TAB						0x09	 /**< TAB key **/
    #define KEY_CLEAR					0x0C	 /**< CLEAR key **/
    #define KEY_RETURN					0x0D	 /**< ENTER key **/
    #define KEY_SHIFT					0x10	 /**< SHIFT key **/
    #define KEY_CONTROL					0x11	 /**< CTRL key **/
    #define KEY_MENU					0x12	 /**< ALT key **/
    #define KEY_PAUSE					0x13	 /**< PAUSE key **/
    #define KEY_CAPITAL					0x14	 /**< CAPS LOCK key **/
    #define KEY_KANA					0x15	 /**< IME Kana mode **/
    #define KEY_HANGUEL					0x15	 /**< IME Hanguel mode (maintained for compatibility; use KEY_HANGUL) **/
    #define KEY_HANGUL					0x15	 /**< IME Hangul mode **/
    #define KEY_JUNJA					0x17	 /**< IME Junja mode **/
    #define KEY_FINAL					0x18	 /**< IME final mode **/
    #define KEY_HANJA					0x19	 /**< IME Hanja mode **/
    #define KEY_KANJI					0x19	 /**< IME Kanji mode **/
    #define KEY_ESCAPE					0x1B	 /**< ESC key **/
    #define KEY_CONVERT					0x1C	 /**< IME convert **/
    #define KEY_NONCONVERT				0x1D	 /**< IME nonconvert **/
    #define KEY_ACCEPT					0x1E	 /**< IME accept **/
    #define KEY_MODECHANGE				0x1F	 /**< IME mode change request **/
    #define KEY_SPACE					0x20	 /**< SPACEBAR **/
    #define KEY_PRIOR					0x21	 /**< PAGE UP key **/
    #define KEY_NEXT					0x22	 /**< PAGE DOWN key **/
    #define KEY_END						0x23	 /**< END key **/
    #define KEY_HOME					0x24	 /**< HOME key **/
    #define KEY_LEFT					0x25	 /**< LEFT ARROW key **/
    #define KEY_UP						0x26	 /**< UP ARROW key **/
    #define KEY_RIGHT					0x27	 /**< RIGHT ARROW key **/
    #define KEY_DOWN					0x28	 /**< DOWN ARROW key **/
    #define KEY_SELECT					0x29	 /**< SELECT key **/
    #define KEY_PRINT					0x2A	 /**< PRINT key **/
    #define KEY_EXECUTE					0x2B	 /**< EXECUTE key **/
    #define KEY_SNAPSHOT				0x2C	 /**< PRINT SCREEN key **/
    #define KEY_INSERT					0x2D	 /**< INS key **/
    #define KEY_DELETE					0x2E	 /**< DEL key **/
    #define KEY_HELP					0x2F	 /**< HELP_KEY **/
    #define KEY_0						0x30	 /**< Key 0 **/
    #define KEY_1						0x31	 /**< Key 1 **/
    #define KEY_2						0x32	 /**< Key 2 **/
    #define KEY_3						0x33	 /**< Key 3 **/
    #define KEY_4						0x34	 /**< Key 4 **/
    #define KEY_5						0x35	 /**< Key 5 **/
    #define KEY_6						0x36	 /**< Key 6 **/
    #define KEY_7						0x37	 /**< Key 7 **/
    #define KEY_8						0x38	 /**< Key 8 **/
    #define KEY_9						0x39	 /**< Key 9 **/
    #define KEY_A						0x41	 /**< Key A **/
    #define KEY_B						0x42	 /**< Key B **/
    #define KEY_C						0x43	 /**< Key C **/
    #define KEY_D						0x44	 /**< Key D **/
    #define KEY_E						0x45	 /**< Key E **/
    #define KEY_F						0x46	 /**< Key F **/
    #define KEY_G						0x47	 /**< Key G **/
    #define KEY_H						0x48	 /**< Key H **/
    #define KEY_I						0x49	 /**< Key I **/
    #define KEY_J						0x4A	 /**< Key J **/
    #define KEY_K						0x4B	 /**< Key K **/
    #define KEY_L						0x4C	 /**< Key L **/
    #define KEY_M						0x4D	 /**< Key M **/
    #define KEY_N						0x4E	 /**< Key N **/
    #define KEY_O						0x4F	 /**< Key O **/
    #define KEY_P						0x50	 /**< Key P **/
    #define KEY_Q						0x51	 /**< Key Q **/
    #define KEY_R						0x52	 /**< Key R **/
    #define KEY_S						0x53	 /**< Key S **/
    #define KEY_T						0x54	 /**< Key T **/
    #define KEY_U						0x55	 /**< Key U **/
    #define KEY_V						0x56	 /**< Key V **/
    #define KEY_W						0x57	 /**< Key W **/
    #define KEY_X						0x58	 /**< Key X **/
    #define KEY_Y						0x59	 /**< Key Y **/
    #define KEY_Z						0x5A	 /**< Key Z **/
    #define KEY_LWIN					0x5B	 /**< Left Windows key(Natural keyboard)  **/
    #define KEY_RWIN					0x5C	 /**< Right Windows key(Natural keyboard)  **/
    #define KEY_APPS					0x5D	 /**< Applications key(Natural keyboard)  **/
    #define KEY_SLEEP					0x5F	 /**< Computer Sleep key **/
    #define KEY_NUMPAD0					0x60	 /**< Numeric keypad 0 key **/
    #define KEY_NUMPAD1					0x61	 /**< Numeric keypad 1 key **/
    #define KEY_NUMPAD2					0x62	 /**< Numeric keypad 2 key **/
    #define KEY_NUMPAD3					0x63	 /**< Numeric keypad 3 key **/
    #define KEY_NUMPAD4					0x64	 /**< Numeric keypad 4 key **/
    #define KEY_NUMPAD5					0x65	 /**< Numeric keypad 5 key **/
    #define KEY_NUMPAD6					0x66	 /**< Numeric keypad 6 key **/
    #define KEY_NUMPAD7					0x67	 /**< Numeric keypad 7 key **/
    #define KEY_NUMPAD8					0x68	 /**< Numeric keypad 8 key **/
    #define KEY_NUMPAD9					0x69	 /**< Numeric keypad 9 key **/
    #define KEY_MULTIPLY				0x6A	 /**< Multiply key **/
    #define KEY_ADD						0x6B	 /**< Add key **/
    #define KEY_SEPARATOR				0x6C	 /**< Separator key **/
    #define KEY_SUBTRACT				0x6D	 /**< Subtract key **/
    #define KEY_DECIMAL					0x6E	 /**< Decimal key **/
    #define KEY_DIVIDE					0x6F	 /**< Divide key **/
    #define KEY_F1						0x70	 /**< F1 key **/
    #define KEY_F2						0x71	 /**< F2 key **/
    #define KEY_F3						0x72	 /**< F3 key **/
    #define KEY_F4						0x73	 /**< F4 key **/
    #define KEY_F5						0x74	 /**< F5 key **/
    #define KEY_F6						0x75	 /**< F6 key **/
    #define KEY_F7						0x76	 /**< F7 key **/
    #define KEY_F8						0x77	 /**< F8 key **/
    #define KEY_F9						0x78	 /**< F9 key **/
    #define KEY_F10						0x79	 /**< F10 key **/
    #define KEY_F11						0x7A	 /**< F11 key **/
    #define KEY_F12						0x7B	 /**< F12 key **/
    #define KEY_F13						0x7C	 /**< F13 key **/
    #define KEY_F14						0x7D	 /**< F14 key **/
    #define KEY_F15						0x7E	 /**< F15 key **/
    #define KEY_F16						0x7F	 /**< F16 key **/
    #define KEY_F17						0x80	 /**< F17 key **/
    #define KEY_F18						0x81	 /**< F18 key **/
    #define KEY_F19						0x82	 /**< F19 key **/
    #define KEY_F20						0x83	 /**< F20 key **/
    #define KEY_F21						0x84	 /**< F21 key **/
    #define KEY_F22						0x85	 /**< F22 key **/
    #define KEY_F23						0x86	 /**< F23 key **/
    #define KEY_F24						0x87	 /**< F24 key **/
    #define KEY_NUMLOCK					0x90	 /**< NUM LOCK key **/
    #define KEY_SCROLL					0x91	 /**< SCROLL LOCK key **/
    #define KEY_NUMLOCK					0x90	 /**< NUM LOCK key **/
    #define KEY_SCROLL					0x91	 /**< SCROLL LOCK key **/
    #define KEY_LSHIFT					0xA0	 /**< Left SHIFT key **/
    #define KEY_RSHIFT					0xA1	 /**< Right SHIFT key **/
    #define KEY_LCONTROL				0xA2	 /**< Left CONTROL key **/
    #define KEY_RCONTROL				0xA3	 /**< Right CONTROL key **/
    #define KEY_LMENU					0xA4	 /**< Left MENU key **/
    #define KEY_RMENU					0xA5	 /**< Right MENU key **/
    #define KEY_BROWSER_BACK			0xA6	 /**< Browser Back key **/
    #define KEY_BROWSER_FORWARD			0xA7	 /**< Browser Forward key **/
    #define KEY_BROWSER_REFRESH			0xA8	 /**< Browser Refresh key **/
    #define KEY_BROWSER_STOP			0xA9	 /**< Browser Stop key **/
    #define KEY_BROWSER_SEARCH			0xAA	 /**< Browser Search key **/
    #define KEY_BROWSER_FAVORITES		0xAB	 /**< Browser Favorites key **/
    #define KEY_BROWSER_HOME			0xAC	 /**< Browser Start and Home key **/
    #define KEY_VOLUME_MUTE				0xAD	 /**< Volume Mute key **/
    #define KEY_VOLUME_DOWN				0xAE	 /**< Volume Down key **/
    #define KEY_VOLUME_UP				0xAF	 /**< Volume Up key **/
    #define KEY_MEDIA_NEXT_TRACK		0xB0	 /**< Next Track key **/
    #define KEY_MEDIA_PREV_TRACK		0xB1	 /**< Previous Track key **/
    #define KEY_MEDIA_STOP				0xB2	 /**< Stop Media key **/
    #define KEY_MEDIA_PLAY_PAUSE		0xB3	 /**< Play/Pause Media key **/
    #define KEY_LAUNCH_MAIL				0xB4	 /**< Start Mail key **/
    #define KEY_LAUNCH_MEDIA_SELECT		0xB5	 /**< Select Media key **/
    #define KEY_LAUNCH_APP1				0xB6	 /**< Start Application 1 key **/
    #define KEY_LAUNCH_APP2				0xB7	 /**< Start Application 2 key **/
    #define KEY_OEM_1					0xBA	 /**< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the \';:\' keyOEM_PLUS  **/
    #define KEY_OEM_PLUS				0xBB	 /**< For any country/region, the \'+\' key **/
    #define KEY_OEM_COMMA				0xBC	 /**< For any country/region, the \',\' key **/
    #define KEY_OEM_MINUS				0xBD	 /**< For any country/region, the \'-\' key **/
    #define KEY_OEM_PERIOD				0xBE	 /**< For any country/region, the \'.\' key **/
    #define KEY_OEM_2					0xBF	 /**< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the \'/?\' key **/
    #define KEY_OEM_3					0xC0	 /**< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the \'`~\' key **/
    #define KEY_OEM_4					0xDB	 /**< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the \'[{\' key **/
    #define KEY_OEM_5					0xDC	 /**< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the \'\\|\' key **/
    #define KEY_OEM_6					0xDD	 /**< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the \']}\' key **/
    #define KEY_OEM_7					0xDE	 /**< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the \'single-quote/double-quote\' key **/
    #define KEY_OEM_8					0xDF	 /**< Used for miscellaneous characters; it can vary by keyboard.  **/
    #define KEY_OEM_102					0xE2	 /**< Either the angle bracket keyor the backslash keyon the RT 102-keykeyboard  **/
    #define KEY_PROCESS					0xE5	 /**< IME PROCESS key **/
    #define KEY_ATTN					0xF6	 /**< Attn key **/
    #define KEY_CRSEL					0xF7	 /**< CrSel key **/
    #define KEY_EXSEL					0xF8	 /**< ExSel key **/
    #define KEY_EREOF					0xF9	 /**< Erase EOF key **/
    #define KEY_PLAY					0xFA	 /**< Play key **/
    #define KEY_ZOOM					0xFB	 /**< Zoom key **/
    #define KEY_NONAME					0xFC	 /**< Reserved  **/
    #define KEY_PA1						0xFD	 /**< PA1 key **/
    #define KEY_OEM_CLEAR				0xFE	 /**< Clear key **/
}};

#endif