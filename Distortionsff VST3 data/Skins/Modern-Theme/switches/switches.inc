<?xml version="1.0" encoding="utf-8" ?>
<DUMMY>
<TEMPLATE id="TPL_HSWITCH" param_id="#UNDEFINED#" reverse="false">
    <COLUMN h_margin="-12">
      <IMAGE_PARAM_BUTTON image="$THEME_PATH$/switches/switch_button.png" cursor="system::hand" param_id="$param_id$" reverse="$reverse$"/>
      <CELL height="-11" />
    </COLUMN>
</TEMPLATE>
<DEFINE>
  <CUS_V_SELECT_SWITCH base_type="IMAGE_PARAM_BUTTON" image="$THEME_PATH$/switches/v_select_switch.png" cursor="system::hand"/>
  <CUS_H_SELECT_SWITCH base_type="IMAGE_PARAM_BUTTON" image="$THEME_PATH$/switches/h_select_switch.png" cursor="system::hand"/>
  <CUS_LCD_SWITCH base_type="IMAGE_PARAM_BUTTON" image="$THEME_PATH$/switches/lcd_h_switch.svg"  opaque_mouse_mask="true" cursor="system::hand"/>
  <CUS_ON_OFF_SWITCH base_type="IMAGE_PARAM_BUTTON" image="$THEME_PATH$/switches/on_off.png" opaque_mouse_mask="true" cursor="system::hand"/>
</DEFINE>
</DUMMY>