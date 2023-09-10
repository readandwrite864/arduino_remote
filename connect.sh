#!/bin/bash

device=/dev/ttyUSB0
baud_rate=115200
mode=mouse
step=5
delta=$step
power=true

stty $baud_rate -F $device raw -echo

handle_shared() {
  case "$line" in
    *KEY_POWER*)   [[ $power == true ]] && power=false || sudo $(which shutdown) now;;
    *KEY_ENTER*)   xdotool click 1;;
    *KEY_EXIT*)    [[ $repeat == true ]] || xdotool key ctrl+w;;

    KEY_A) [[ $repeat == true ]] || xdotool key super+w && sleep 1 && xdotool key ctrl+l && xdotool type "youtube.com" && xdotool key Return;;
    KEY_B) [[ $repeat == true ]] || xdotool click 2;;
    KEY_C) [[ $repeat == true ]] || xdotool key ctrl+shift+Tab;;
    KEY_D) [[ $repeat == true ]] || xdotool key ctrl+Tab;;
  esac
}

handle_mouse() {
  dx=0
  dy=0

  case "$line" in
    *KEY_ARROW_UP*)    dy=-$delta;;
    *KEY_ARROW_RIGHT*) dx=$delta;;
    *KEY_ARROW_DOWN*)  dy=$delta;;
    *KEY_ARROW_LEFT*)  dx=-$delta;;

    *KEY_RETURN*)        [[ $repeat == true ]] || mode="keyboard";;
  esac

  xdotool mousemove_relative -- $dx $dy
}

handle_keyboard() {
  key=false

  case "$line" in
    *KEY_ARROW_UP*)    key=Up;;
    *KEY_ARROW_RIGHT*) key=Right;;
    *KEY_ARROW_DOWN*)  key=Down;;
    *KEY_ARROW_LEFT*)  key=Left;;

    *KEY_RETURN*)        [[ $repeat == true ]] || mode="mouse";;
  esac

  [[ $key == false ]] || xdotool key $key
}

handle_command() {
  handle_shared
  
  if [[ $mode == "mouse" ]]; then
    handle_mouse
  else
    handle_keyboard
  fi
}

while IFS=$'\r\n' read -r line; do
  echo "$line"

  case "$line" in
    *Repeat*)  delta=$(($delta * 2)); repeat=true;;
    *Command*) delta=$step;           repeat=false;;
    *)         handle_command;;
  esac
done < "$device"
