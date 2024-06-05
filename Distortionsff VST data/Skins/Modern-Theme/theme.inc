<?xml version="1.0" encoding="utf-8" ?>
<DUMMY>	
  <!-- Dark Blue Theme by Blue Cat Audio -->
  <VARIABLE id="THEME_PATH" value="$_DIR_$" override="true"/>
  
  <!-- Includes -->
	<INC file="$THEME_PATH$/includes.inc" />
  
  <!-- Definitions of Generic types-->
 	<DEFINE>
    <!-- The main theme decorations, including the toolbar (optional)-->
    <THEME_DECORATIONS base_type="TPL_DECORATIONS"/>

    <!-- A cell that sets the default text style -->
    <THEME_TEXT_STYLE_CELL base_type="DECORATIONS_STYLE_CELL"/>

    <!-- Title bar, which can include additional buttons (in a row) -->
    <!-- Parameter: title (default is using the plug-in title metadata)-->
		<THEME_TITLE_BAR base_type="TPL_TITLE_BAR"/>
		
		<!-- Group Boxes -->
    <THEME_BOX base_type="IMAGE_GROUP_BOX" image="$THEME_PATH$/group_boxes/pad-bluelight.png" />
    <THEME_BOX_H_SEPARATOR base_type="IMAGE_GROUP_BOX" image="$THEME_PATH$/group_boxes/pad-bluelight_h_separator.png" stretch="true" width="100%" height="1"/>
    <THEME_BOX_V_SEPARATOR base_type="IMAGE_GROUP_BOX" image="$THEME_PATH$/group_boxes/pad-bluelight_v_separator.png" stretch="true" width="1" height="100%"/>

    <THEME_GROUP base_type="IMAGE_GROUP_BOX" image="$THEME_PATH$/group_boxes/group.png"/>
    <THEME_GRAPH_BOX base_type="IMAGE_GROUP_BOX" image="$THEME_PATH$/group_boxes/graph_box.png" h_margin="5" v_margin="4" stretch="true" ignore_mask_files="true"/>
    <THEME_ALT_GRAPH_BOX base_type="IMAGE_GROUP_BOX" image="$THEME_PATH$/group_boxes/alt_graph_box.png" h_margin="5" v_margin="4" stretch="true" ignore_mask_files="true"/>
    <THEME_GRAPH_BG_BOX base_type="IMAGE_GROUP_BOX" image="$THEME_PATH$/group_boxes/graph_bg.png" stretch="true" ignore_mask_files="true"/>

    <THEME_GRAPH_GLASS_OVERLAY  base_type="IMAGE_GROUP_BOX" image="$THEME_PATH$/group_boxes/graph_glass.png" width="100%" height="62" stretch="true" v_align="top" ignore_mask_files="true"/>

    <!-- Special Knobs -->
    <THEME_OPACITY_KNOB base_type="CUS_OPACITY_KNOB"/>
  
    <!-- Regular Knobs -->
    <THEME_KNOB base_type="CUS_BLUE_KNOB"/>
    <THEME_KNOB_SYM base_type="CUS_BLUE_KNOB_SYM"/>
    <THEME_SMALL_KNOB base_type="CUS_SMALL_BLUE_KNOB"/>
    <THEME_SMALL_KNOB_SYM base_type="CUS_SMALL_BLUE_KNOB_SYM"/>
    <THEME_SMALLER_KNOB base_type="CUS_SMALLER_BLUE_KNOB"/>
    <THEME_SMALLER_KNOB_SYM base_type="CUS_SMALLER_BLUE_KNOB_SYM"/>
    <THEME_MINI_KNOB base_type="CUS_MINI_BLUE_KNOB"/>
    <THEME_MINI_KNOB_SYM base_type="CUS_MINI_BLUE_KNOB_SYM"/>
    <THEME_MICRO_KNOB base_type="CUS_MICRO_BLUE_KNOB"/>
    <THEME_MICRO_KNOB_SYM base_type="CUS_MICRO_BLUE_KNOB_SYM"/>

    <THEME_KNOB_CHANNEL1 base_type="CUS_BLUE_KNOB"/>
    <THEME_KNOB_CHANNEL1_SYM base_type="CUS_BLUE_KNOB_SYM"/>
    <THEME_MINI_KNOB_CHANNEL1 base_type="CUS_MINI_BLUE_KNOB"/>
    <THEME_MINI_KNOB_CHANNEL1_SYM base_type="CUS_MINI_BLUE_KNOB_SYM"/>
    
    <THEME_KNOB_CHANNEL2 base_type="CUS_RED_KNOB"/>
    <THEME_KNOB_CHANNEL2_SYM base_type="CUS_RED_KNOB_SYM"/>
    <THEME_SMALL_KNOB_CHANNEL2 base_type="CUS_SMALL_RED_KNOB"/>
    <THEME_SMALL_KNOB_CHANNEL2_SYM base_type="CUS_SMALL_RED_KNOB_SYM"/>
    <THEME_MINI_KNOB_CHANNEL2 base_type="CUS_MINI_RED_KNOB"/>
    <THEME_MINI_KNOB_CHANNEL2_SYM base_type="CUS_MINI_RED_KNOB_SYM"/>
    
    <!-- Alternate Knobs -->
    <THEME_ALT_KNOB base_type="CUS_DARK_BLUE_KNOB"/>
    <THEME_ALT_KNOB_SYM base_type="CUS_DARK_BLUE_KNOB_SYM"/>
    <THEME_ALT_SMALL_KNOB base_type="CUS_DARK_SMALL_BLUE_KNOB"/>
    <THEME_ALT_SMALL_KNOB_SYM base_type="CUS_DARK_SMALL_BLUE_KNOB_SYM"/>
    <THEME_ALT_MINI_KNOB base_type="CUS_DARK_MINI_BLUE_KNOB"/>
    <THEME_ALT_MINI_KNOB_SYM base_type="CUS_DARK_MINI_BLUE_KNOB_SYM"/>

    <THEME_ALT_KNOB_CHANNEL1 base_type="CUS_DARK_BLUE_KNOB"/>
    <THEME_ALT_KNOB_CHANNEL1_SYM base_type="CUS_DARK_BLUE_KNOB_SYM"/>
    <THEME_ALT_MINI_KNOB_CHANNEL1 base_type="CUS_DARK_MINI_BLUE_KNOB"/>
    <THEME_ALT_MINI_KNOB_CHANNEL1_SYM base_type="CUS_DARK_MINI_BLUE_KNOB_SYM"/>
    
    <THEME_ALT_KNOB_CHANNEL2 base_type="CUS_DARK_RED_KNOB"/>
    <THEME_ALT_KNOB_CHANNEL2_SYM base_type="CUS_DARK_RED_KNOB_SYM"/>
    <THEME_ALT_SMALL_KNOB_CHANNEL2 base_type="CUS_DARK_SMALL_RED_KNOB"/>
    <THEME_ALT_SMALL_KNOB_CHANNEL2_SYM base_type="CUS_DARK_SMALL_RED_KNOB_SYM"/>
    <THEME_ALT_MINI_KNOB_CHANNEL2 base_type="CUS_DARK_MINI_RED_KNOB"/>
    <THEME_ALT_MINI_KNOB_CHANNEL2_SYM base_type="CUS_DARK_MINI_RED_KNOB_SYM"/>
    
     <!-- LCD Knobs -->
    <THEME_LCD_SMALL_KNOB base_type="CUS_LCD_SMALL_BLUE_KNOB"/>
    <THEME_LCD_SMALL_KNOB_SYM base_type="CUS_LCD_SMALL_BLUE_KNOB_SYM"/>
    <THEME_LCD_MINI_KNOB base_type="CUS_LCD_MINI_BLUE_KNOB"/>
    <THEME_LCD_MINI_KNOB_SYM base_type="CUS_LCD_MINI_BLUE_KNOB_SYM"/>
    
    <!-- Sliders -->
    <THEME_GRAPH_SLIDER base_type="IMAGE_PARAM_SLIDER" orientation="vertical" image="$THEME_PATH$/sliders/graph_slider.png"  opaque_mouse_mask="true" thumb_image="$THEME_PATH$/sliders/graph_slider_thumb.png" cursor="system::hand"/>
    <THEME_FADER base_type="CUS_FADER"/>
    <THEME_SLIM_FADER base_type="CUS_FADER_SLIM"/>

    <THEME_MINI_H_SLIDER base_type="IMAGE_PARAM_SLIDER" orientation="horizontal" image="$THEME_PATH$/sliders/h_slider_small.bmp" cursor="system::size_h"/>
    <THEME_MINI_V_SLIDER base_type="IMAGE_PARAM_SLIDER" orientation="vertical" image="$THEME_PATH$/sliders/v_slider_small.bmp" cursor="system::size_v"/>

    <!-- Meters & boxes -->
    <THEME_VMETER base_type="CUS_VMETER"/>
    <THEME_VMETER_SHORT base_type="CUS_VMETER_SHORT"/>
    <THEME_VMETER_HOLD base_type="CUS_VMETER_HOLD"/>
    <THEME_MAIN_METER_BOX base_type="CUS_METER_BOX"/>
    <THEME_METER_BOX base_type="CUS_METER_GREY_BOX"/>

    <!-- General Purpose Param Buttons -->
    <THEME_ON_OFF_BUTTON base_type="CUS_ON_OFF_BUTTON"/>
    <THEME_CHECK_BOX base_type="CUS_CHECK_BOX"/>
    <THEME_STATUS_BUTTON_80 base_type="CUS_STATUS_BUTTON_80"/>
    
    <!-- Switches -->
    <THEME_V_SELECT_SWITCH base_type="CUS_V_SELECT_SWITCH"/>
    <THEME_H_SELECT_SWITCH base_type="CUS_H_SELECT_SWITCH"/>
    <THEME_ON_OFF_SWITCH base_type="CUS_ON_OFF_SWITCH"/>

    <!-- Icon Buttons -->
    <THEME_SHOW_CONTROLS_BUTTON base_type="CUS_SHOW_CONTROLS_BUTTON"/>
    <THEME_SHOW_BROWSER_BUTTON base_type="CUS_SHOW_BROWSER_BUTTON"/>
    <THEME_SHOW_MAP_BUTTON base_type="CUS_SHOW_MAP_BUTTON"/>
    <THEME_SHOW_KNOBS_BUTTON    base_type="CUS_SHOW_KNOBS_BUTTON"/>
    <THEME_SHOW_PREFS_BUTTON    base_type="CUS_SHOW_PREFS_BUTTON"/>
    <THEME_SHOW_METERS_BUTTON   base_type="CUS_SHOW_METERS_BUTTON"/>
    <THEME_SHOW_GRAPH_BUTTON    base_type="CUS_SHOW_GRAPH_BUTTON"/>
    <THEME_MAGNIFY_BUTTON       base_type="CUS_MAGNIFY_BUTTON"/>
    <THEME_REFRESH_PARAM_BUTTON base_type="CUS_REFRESH_PARAM_BUTTON"/>
    <THEME_EDIT_ACTION_BUTTON   base_type="CUS_EDIT_ACTION_BUTTON"/>
    <THEME_CLEAN_ACTION_BUTTON   base_type="CUS_CLEAN_ACTION_BUTTON"/>
    <THEME_POWER_BUTTON         base_type="IMAGE_PARAM_BUTTON" image="$THEME_PATH$/titlebar/power_button.png" image_hover="$THEME_PATH$/titlebar/power_button_hi.png"  image_focus="$THEME_PATH$/titlebar/power_button_hi.png" cursor="system::hand"/>
    <THEME_TOOLBAR_LOCK_BUTTON  base_type="CUS_TOOLBAR_LOCK_BUTTON"/>
    
    <!-- Dropdown buttons -->
    <THEME_DROPDOWN_PARAM_BUTTON base_type="CUS_DROPDOWN_PARAM_BUTTON"/>
    <THEME_DROPDOWN_ACTION_BUTTON base_type="CUS_DROPDOWN_ACTION_BUTTON"/>
     
    <!-- Settings buttons -->
    <!-- Parameter: param_id (to be used only on dsp parameters)-->
    <THEME_DSP_PARAM_SETTINGS_BUTTON base_type="TPL_DSP_PARAM_SETTINGS_BUTTON" image_scaling="1.72"/>
    <THEME_DSP_PARAM_SMALL_SETTINGS_BUTTON base_type="TPL_DSP_PARAM_SETTINGS_BUTTON" image_scaling="1.23"/>
    <THEME_DSP_PARAM_MINI_SETTINGS_BUTTON base_type="TPL_DSP_PARAM_SETTINGS_BUTTON"/>
    
    <!-- for backward compat -->
    <THEME_DSP_PARAM_SMALL_ROUND_SETTINGS_BUTTON base_type="TPL_DSP_PARAM_SETTINGS_BUTTON" image_scaling="1.23"/>
    <THEME_DSP_PARAM_SMALL_RECT_SETTINGS_BUTTON base_type="TPL_DSP_PARAM_SETTINGS_BUTTON" image_scaling="1.23"/>
    <THEME_DSP_PARAM_ROUND_SETTINGS_BUTTON base_type="TPL_DSP_PARAM_SETTINGS_BUTTON" image_scaling="1.72"/>


	</DEFINE>
</DUMMY>