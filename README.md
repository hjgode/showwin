<h1>WIN32 CE APPLICATION : showWin Project Overview</h1>

Display a small window with some text inside<p>

Single instance app<p>

v 1.1:  second release<p>

<pre>
 showWin -t "Text zum Anzeigen" -r 90 -g 80 -b 70 -s 8 -w 200 -h 50 -x 0 -y 0 -rt 200 -gt 20 -bt 20 -ti 10 -progr 30 -align left
</pre><p>

<table>
<tr><td>ARGS:</td><td></td><td></td><td></td></tr>
<tr><td>option/parameter:</td><td>meaning:</td><td>default:</td><td>limitations:</td></tr>
<tr><td>-t "Text zum Anzeigen"</td><td>text to show</td><td>"Installing"</td><td>255 chars, no " inside, no line breaks, no tabs</td></tr>
<tr><td>-r 90</td><td>background color RED</td><td>255</td><td>0-255</td></tr>
<tr><td>-g 80</td><td>background color GREEN</td><td>207</td><td>0-255</td></tr>
<tr><td>-b 70</td><td>background color BLUE</td><td>0</td><td>0-255</td></tr>
<tr><td>-s 8</td><td>font size in points</td><td>10</td><td>7-24 points</td></tr>
<tr><td>-w 200</td><td>window width pixels</td><td>460</td><td>100-screenwidth</td></tr>
<tr><td>-h 50</td><td>window height pixels</td><td>40</td><td>menu bar height (ie 26pixels)</td></tr>
<tr><td>-x 60</td><td>window pos X</td><td>12</td><td>0 + system window bordersize</td></tr>
<tr><td>-y 60</td><td>window pos Y</td><td>48</td><td>0 + system taskbar bar height. Using 0;0 does not work nice on WM, win may be below taskbar</td></tr>
<tr><td>-rt 200</td><td>text color RED</td><td>0</td><td>0-255</td></tr>
<tr><td>-gt 20</td><td>text color GREEN</td><td>0</td><td>0-255</td></tr>
<tr><td>-bt 20</td><td>text color BLUE</td><td>0</td><td>0-255</td></tr>
<tr></tr> 
<tr><td>-align center</td><td>text alignment</td><td>left</td><td>center|left|right</td></tr>
<tr></tr> 
<tr><td>-ti 10</td><td>timeout to autoclose</td><td>0</td><td>no autoclose, min: 1 (second), max: 3600 = one hour</td></tr>
<tr></tr> 
<tr><td>-progr 10</td><td>show with progress val</td><td>0</td><td>no progressbar, max: 100
</td><td>the progressbar is appended at bottom of textwindow</td></tr>
<tr></tr>
<tr><td>-kill</td><td>kill existing window, exit app</td></tr>
<tr></tr> 
<tr><td>-m "new message text"</td><td>replace text in window</td><td>see -t</td></tr>
<tr><td>-prval 30</td><td>new progress value</td></tr>
</table>
 

