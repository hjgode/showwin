========================================================================
    WIN32 APPLICATION : showWin Project Overview
========================================================================

Display a small window with some text inside

Single instance app

v 1.2:	added prval description

v 1.1:	second release, with more TBR related default values

v 1.0:	??

/*
 
 showWin -t "Text zum Anzeigen" -r 90 -g 80 -b 70 -s 8 -w 200 -h 50 -x 0 -y 0 -rt 200 -gt 20 -bt 20 -ti 10 -progr 30 -align left
 
 ARGS: 
 option/parameter:				meaning:				default:			limitations:
 -t "Text zum Anzeigen"			text to show			"Installing"		255 chars, no " inside, no line breaks, no tabs
 -r 90							background color RED	255					0-255
 -g 80							background color GREEN	207					0-255
 -b 70							background color BLUE	0					0-255
 -s 8							font size in points		10					7-24 points
 -w 200							window width pixels		460					100-screenwidth
 -h 50							window height pixels	40					menu bar height (ie 26pixels)
 -x 60							window pos X			12					0 + system window bordersize
 -y 60							window pos Y			48					0 + system taskbar bar height. Using 0;0 does not work nice on WM, win may be below taskbar
 -rt 200						text color RED			0					0-255
 -gt 20							text color GREEN		0					0-255
 -bt 20							text color BLUE			0					0-255
 
 -align center					text alignment			left				center|left|right
 
 -ti 10							timeout to autoclose	0					no autoclose, min: 1 (second), max: 3600 = one hour
 
 -progr 10						show with progress val	0					no progressbar, max: 100
								the progressbar is appended at bottom of textwindow
 -prval							update progress bar value					no default, min=1, max=100
 								
 -kill							kill existing window, exit app
 
 -m "new message text"			replace text in window						see -t
 
*/

