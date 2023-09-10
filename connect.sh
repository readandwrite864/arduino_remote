#!/bin/bash

device=/dev/ttyUSB0
baud_rate=115200
mode=mouse
step=5
delta=$step

stty $baud_rate -F $device raw -echo

handle_shared() {
  case "$line" in
    *KEY_ENTER*)       xdotool click 1;;
    # *KEY_RETURN*) echo "ret";;

    # *KEY_A*) echo "a";;
    # *KEY_B*) echo "b";;
    # *KEY_C*) echo "c";;
    # *KEY_D*) echo "d";;
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

    *KEY_EXIT*)        mode="keyboard";;
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

    *KEY_EXIT*)        mode="mouse";;
  esac

  [[ $key ]] && xdotool key $key
}

handle_command() {
  handle_shared
  
  if [[ $mode == "mouse" ]]; then
    handle_mouse
  else
    handle_keyboard
  fi
}

while read -r line; do
  echo "$line"

  case "$line" in
    *Repeat*)  delta=$(($delta * 2));;
    *Command*) delta=$step;;
    *)         handle_command;;
  esac
done < "$device"
