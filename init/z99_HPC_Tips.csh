# set SHOWTIPS=/opt/apps/HPCTips/HPCTips/bin/showTip
SHOWTIPS=""
if ( "$USER" != "root" && $?prompt && $?loginsh && ! -f $HOME/.no.tips && -x  $SHOWTIPS ) then
   $SHOWTIP -w
endif
unset SHOWTIP
