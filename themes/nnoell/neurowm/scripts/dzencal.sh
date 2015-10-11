#!/bin/bash

#LOOK AND FEEL
FONT="-*-montecarlo-medium-r-normal-*-11-*-*-*-*-*-*-*"
CRIT="#66ff66"
DZEN_BG="#020202"
DZEN_FG="#444444"
HEADER_FG="#44aacc"
TITLE_FG="#f7a16e"

#DATE
TODAY=$(expr `date +'%d'` + 0)
MONTH=$(date +'%m')
YEAR=$(date +'%Y')

(
echo -e "\n          ^fg("$TITLE_FG")CALENDAR"

# current month, highlight header and today
cal -m | sed -r -e "1,2 s/.*/^fg($HEADER_FG)&^fg()/" -e "s/(^| )($TODAY)($| )/\1^bg()^fg($CRIT)\2^fg()^bg()\3/" -e "s/^/    /"

# next month, hilight header
[[ $MONTH -eq 12 ]] && YEAR=`expr $YEAR + 1`
cal -m `expr \( $MONTH % 12 \) + 1` $YEAR | sed -e "1,2 s/.*/^fg($HEADER_FG)&^fg()/" -e "s/^/    /"
) \
| dzen2 -p 60 -bg $DZEN_BG -fg $DZEN_FG -fn $FONT -x 1745 -y 17 -w 170 -l 18 -sa l -e 'onstart=uncollapse;button3=exit'
