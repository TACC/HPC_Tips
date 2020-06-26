# SHOWTIP=/opt/apps/HPC_Tips/HPC_Tips/bin/showTip

if [ -n "${BASH_VERSION+x}" ]; then
  if shopt -q login_shell; then
    LOGIN_SHELL=1
  fi
fi

if [ -n "${ZSH_VERSION+x}" ]; then
  if [[ -o login ]]; then
    LOGIN_SHELL=1
  fi
fi

if [ $USER != "root" -a  -n "$PS1" -a -n "${LOGIN_SHELL+x}" -a ! -f ~/.no.tips -a -x "${SHOWTIP:-}" ]; then
  $SHOWTIP -w
fi

unset SHOWTIP LOGIN_SHELL
