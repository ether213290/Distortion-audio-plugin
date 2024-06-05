<?xml version="1.0" encoding="utf-8" ?>
<DUMMY>
  <!-- for compatibility with legacy BC scripts -->
  <DEFINE>
    <PNS_DSP_PARAM_SETTINGS_BUTTON base_type="THEME_DSP_PARAM_SETTINGS_BUTTON"/>
  </DEFINE>

  <!-- Toolbar title -->
  <TEMPLATE id="TOOLBAR_TITLE">

    <CELL v_margin="$TB_TI_PAD_V$" h_margin="$TB_TI_PAD_H$" h_offset="$TB_TI_H_OFFSET$" v_offset="$TB_TI_V_OFFSET$" min_width="$TB_TI_MINW$" max_width="$TB_TI_MAXW$">
      <!-- show script name string if static title not set -->
      <STRING id="LM_toolbar_title" default="hehe" />
      
      <!-- <LV id="STRING_ID" value="$script_name_string$" /> -->
      <LV id="STRING_ID" value="a_toolbar_title_text" />

      <!-- if we want to add plugin version to title -->
      <!-- <STRING_MULTI_LINK from="$script_name_string$;LM_plugin_version_fmt" to="LM_toolbar_title"/>
      <LV id="STRING_ID" value="LM_toolbar_title" /> -->
      
      <REPEAT count="($TB_TI_TEXT_SET$ == true)">
        <LV id="STRING_ID" value="" />
      </REPEAT>
      
      <TEXT value="$TB_TI_TEXT$" string_id="$STRING_ID$" font_size="$TB_TI_FONT_SIZE$" max_width="$TB_TI_MAXW$" fit_text="abbreviate" font_weight="$TB_TI_FONT_WEIGHT$" font_style="$TB_TI_FONT_STYLE$" font_face="$TB_TI_FONT_FACE$" text_color="$TB_TI_TEXT_COLOR$" font_quality="$TB_TI_FONT_QUALITY$" h_align="$TB_TI_TEXT_ALIGN$">
        <REPEAT count="($TOOLBAR_TITLE_ACTION_NUM$ == 1)">
          <IAB action='$TOOLBAR_TITLE_CUS_ACTION$'><TT text='$TOOLBAR_TITLE_CUS_ACTION_HINT$'/></IAB>
        </REPEAT>
        <REPEAT count="($TOOLBAR_TITLE_ACTION_NUM$ == 2)">
          <IAB action='ReloadScript'><TT text='Reload script'/></IAB>
        </REPEAT>
        <REPEAT count="($TOOLBAR_TITLE_ACTION_NUM$ == 3)">
          <IAB action='ReloadSkin'><TT text='Reload skin'/></IAB>
        </REPEAT>
        <REPEAT count="($TOOLBAR_TITLE_ACTION_NUM$ == 4)">
          <ROW height='100%' width='100%'>
            <IAB action='ReloadSkin' width='30%'><TT text='Reload skin' /></IAB>
            <IAB action='ReloadScript' width='70%'><TT text='Reload script' /></IAB>
          </ROW>
        </REPEAT>
        <REPEAT count="($TOOLBAR_TITLE_ACTION_NUM$ == 5)">
          <IAB action='OpenManual'><TT text='Open manual'/></IAB>
        </REPEAT>
        <REPEAT count="($TOOLBAR_TITLE_ACTION_NUM$ == 6)">
          <IAB action='OpenPresetsMenu'><TT text='Presets menu'/></IAB>
        </REPEAT>
        <REPEAT count="($TOOLBAR_TITLE_ACTION_NUM$ == 7)">
          <ONLOAD script="a_toolbar_infolink = &quot;$TOOLBAR_INFO_LINK$&quot;;" requires="a_toolbar_infolink" />
          <IAB action="OpenInfo"><TT text="$TOOLBAR_INFO_LINK_HINT$" /></IAB>
        </REPEAT>
        
      </TEXT>
    </CELL>
  </TEMPLATE>

  <!-- Toolbar presets -->
  <TEMPLATE id="TOOLBAR_PRESETS">
    <LV id="TB_PR_AR_INPAD" formula="abs($TB_PR_AR_POS$)" />
    <ROW v_margin="$TB_PR_PAD_V$" h_offset="$TB_PR_H_OFFSET$" v_offset="$TB_PR_V_OFFSET$">
      <R count="($TB_PR_AR_POS$ > -1)">
        <LAYER_STACK h_align="left" v_offset="$TB_PR_AR_V_OFFSET$">
          <SVG_ICON_PREV size="$TB_PR_AR_SIZE$" color="$TB_PR_AR_COLOR$" opacity="$TB_PR_AR_OPACITY$" />
          <IAB action="PreviousPreset" />
        </LAYER_STACK>
      </R>
      <LAYER_STACK width="$presets_width$" min_height="$TB_PR_MIN_HEIGHT$" h_margin="$TB_PR_PAD_H$">
        <LM_BOX_RAW data="$TB_PR_BOX$" />
        <ROW>
          <R count="($TB_PR_AR_POS$ &lt; 0)">
            <CELL width="$TB_PR_AR_INPAD$" />
            <LAYER_STACK h_align="left" v_offset="$TB_PR_AR_V_OFFSET$">
              <SVG_ICON_PREV size="$TB_PR_AR_SIZE$" color="$TB_PR_AR_COLOR$" opacity="$TB_PR_AR_OPACITY$" />
              <IAB action="PreviousPreset" />
            </LAYER_STACK>
          </R>
          
          <LAYER_STACK width="$TB_PR_WIDTH$">
            <ROW h_margin="$TB_PR_TX_HPAD$" width='100%'>
              <TEXT width='100%' string_id="current_preset_name_with_asterisk" font_size="$TB_PR_FONT_SIZE$" font_weight="$TB_PR_FONT_WEIGHT$" font_style="$TB_PR_FONT_STYLE$" font_face="$TB_PR_FONT_FACE$" text_color="$TB_PR_TEXT_COLOR$" font_quality="$TB_PR_FONT_QUALITY$" v_offset="$TB_PR_FONT_V_OFFSET$" />
            </ROW>
            <IAB action="OpenPresetsMenu" />
          </LAYER_STACK>
          
          <R count="($TB_PR_AR_POS$ &lt; 0)">
            <LAYER_STACK h_align="right" v_offset="$TB_PR_AR_V_OFFSET$">
              <SVG_ICON_NEXT size="$TB_PR_AR_SIZE$" color="$TB_PR_AR_COLOR$" opacity="$TB_PR_AR_OPACITY$" />
              <IAB action="NextPreset" />
            </LAYER_STACK>
            <CELL width="$TB_PR_AR_INPAD$" />
          </R>

        </ROW>
          
      </LAYER_STACK>
      <R count="($TB_PR_AR_POS$ > -1)">
        <LAYER_STACK h_align="right" v_offset="$TB_PR_AR_V_OFFSET$">
          <SVG_ICON_NEXT size="$TB_PR_AR_SIZE$" color="$TB_PR_AR_COLOR$" opacity="$TB_PR_AR_OPACITY$" />
          <IAB action="NextPreset" />
        </LAYER_STACK>
      </R>
    </ROW>
  </TEMPLATE>

  <!-- toolbar icons tems -->
  <VAR id="TIN" value="0" /> <!-- helper counter -->
  <TEMPLATE id="TOOLBAR_BYPASS">
    <VAR id="TIN" formula="$TIN$+1" />
    <LAYER_STACK v_offset="$TOOLBAR_ICONS_V_OFFSET$" h_margin="$TOOLBAR_ICONS_PAD_H$" v_margin="$TOOLBAR_ICONS_PAD_V$">
      <TOOLBAR_ICON icon="POWER" widget_id="cell_power_button_$TIN$"  />
      <IAB action="toggle_bypass"><PARAM_TOOLTIP param_id="$bypass$" content="Bypass is {text_value}" /></IAB>
    </LAYER_STACK>
    <EXPOSED_OBJECTS object_ids="cell_power_button_$TIN$.opacity" />
    <PARAM_LINK from="$bypass$" to="cell_power_button_$TIN$.opacity" formula="abs($TOOLBAR_ICONS_OPACITY$-x*($TOOLBAR_ICONS_OPACITY$*0.7))" />
  </TEMPLATE>

  <TEMPLATE id="TOOLBAR_MAINMENU">
    <LAYER_STACK v_offset="$TOOLBAR_ICONS_V_OFFSET$" h_margin="$TOOLBAR_ICONS_PAD_H$" v_margin="$TOOLBAR_ICONS_PAD_V$">
      <TOOLBAR_ICON icon="MENU" />
      <IAB action="$TOOLBAR_MAINMENU_ACTION$"><TT text="$TOOLBAR_MAINMENU_ACTION_HINT$"/></IAB>
    </LAYER_STACK>
  </TEMPLATE>

  <TEMPLATE id="TOOLBAR_ZOOM">
    <LAYER_STACK v_offset="$TOOLBAR_ICONS_V_OFFSET$" h_margin="$TOOLBAR_ICONS_PAD_H$" v_margin="$TOOLBAR_ICONS_PAD_V$">
      <TOOLBAR_ICON icon="ZOOM" />
      <IPMB ignore_mask_file="true" param_id="gui.zoom" positions_count="14" reverse_mouse_wheel="false"><PARAM_TOOLTIP param_id="gui.zoom" content="Zoom: {text_value}{unit}"/></IPMB>
    </LAYER_STACK>
  </TEMPLATE>

  <TEMPLATE id="TOOLBAR_OPACITY" >
    <LAYER_STACK v_offset="$TOOLBAR_ICONS_V_OFFSET$" h_margin="$TOOLBAR_ICONS_PAD_H$" v_margin="$TOOLBAR_ICONS_PAD_V$">
      <TOOLBAR_ICON icon="ADJUST" />
      <PARAM_TEXT_CONTROL param_id="global_opacity" width="100%" height="100%" opacity="0" cursor="$CUR_SIZE_V$">
        <PARAM_TOOLTIP param_id="global_opacity" content="GUI opacity: {value}%" value_format="0.0"  />
      </PARAM_TEXT_CONTROL>
    </LAYER_STACK>
  </TEMPLATE>

  <TEMPLATE id="TOOLBAR_UNDO">
    <LAYER_STACK v_offset="$TOOLBAR_ICONS_V_OFFSET$" h_margin="$TOOLBAR_ICONS_PAD_H$" v_margin="$TOOLBAR_ICONS_PAD_V$">
      <TOOLBAR_ICON icon="UNDO" />
      <IAB action="Undo"><TT text="Undo" /></IAB>
    </LAYER_STACK>
  </TEMPLATE>

  <TEMPLATE id="TOOLBAR_REDO">
    <LAYER_STACK v_offset="$TOOLBAR_ICONS_V_OFFSET$" h_margin="$TOOLBAR_ICONS_PAD_H$" v_margin="$TOOLBAR_ICONS_PAD_V$">
      <TOOLBAR_ICON icon="REDO" />
      <IAB action="Redo"><TT text="Redo" /></IAB>
    </LAYER_STACK>
  </TEMPLATE>

  <TEMPLATE id="TOOLBAR_PLUGINFO">
    <ONLOAD script="a_toolbar_infolink = &quot;$TOOLBAR_INFO_LINK$&quot;;" requires="a_toolbar_infolink" />
    <LAYER_STACK v_offset="$TOOLBAR_ICONS_V_OFFSET$" h_margin="$TOOLBAR_ICONS_PAD_H$" v_margin="$TOOLBAR_ICONS_PAD_V$">
      <TOOLBAR_ICON icon="INFO" />
      <IAB action="OpenInfo"><TT text="Plugin info" /></IAB>
    </LAYER_STACK>
  </TEMPLATE>

  <TEMPLATE id="TOOLBAR_MANUAL">
    <LAYER_STACK v_offset="$TOOLBAR_ICONS_V_OFFSET$" h_margin="$TOOLBAR_ICONS_PAD_H$" v_margin="$TOOLBAR_ICONS_PAD_V$">
      <TOOLBAR_ICON icon="QUESTION" />
      <IAB action="OpenManual"><TT text="Open plugin manual" /></IAB>
    </LAYER_STACK>
  </TEMPLATE>

  <TEMPLATE id="TOOLBAR_PRESETMENU">
    <LAYER_STACK v_offset="$TOOLBAR_ICONS_V_OFFSET$" h_margin="$TOOLBAR_ICONS_PAD_H$" v_margin="$TOOLBAR_ICONS_PAD_V$">
      <TOOLBAR_ICON icon="LIST" />
      <IAB action="OpenPresetsMenu"><TT text="Presets menu" /></IAB>
    </LAYER_STACK>
  </TEMPLATE>

  <TEMPLATE id="TOOLBAR_SCRIPTMENU">
    <!--<IF condition="$SCRIPT_EDIT_MODE$">-->
      <LAYER_STACK v_offset="$TOOLBAR_ICONS_V_OFFSET$" h_margin="$TOOLBAR_ICONS_PAD_H$" v_margin="$TOOLBAR_ICONS_PAD_V$">
        <TOOLBAR_ICON icon="FILE" />
        <IAB action="$TOOLBAR_SCRIPTMENU_ACTION$" ><TT text="$TOOLBAR_SCRIPTMENU_ACTION_HINT$"/></IAB>
      </LAYER_STACK>
    <!--</IF>-->
  </TEMPLATE>

  <TEMPLATE id="TOOLBAR_SKINMENU">
    <LAYER_STACK v_offset="$TOOLBAR_ICONS_V_OFFSET$" h_margin="$TOOLBAR_ICONS_PAD_H$" v_margin="$TOOLBAR_ICONS_PAD_V$">
      <TOOLBAR_ICON icon="DROPDOWN" />
      <IAB action="$TOOLBAR_SKINMENU_ACTION$"><TT text="$TOOLBAR_SKINMENU_ACTION_HINT$"/></IAB>
    </LAYER_STACK>
  </TEMPLATE>

  <TEMPLATE id="TOOLBAR_SETTINGS">
    
      <LAYER_STACK v_offset="$TOOLBAR_ICONS_V_OFFSET$" h_margin="$TOOLBAR_ICONS_PAD_H$" v_margin="$TOOLBAR_ICONS_PAD_V$">
        <TOOLBAR_ICON icon="SETTINGS" widget_id="theme_control_settings_button" />
        <REPEAT count="($TOOLBAR_SETTINGS_ACTION_IS_SET$ == 0)" index_name="__das">
          <IAB action="ToggleControlsSettings"><TT text="Show controls settings" /></IAB>
          <PARAM_LINK from="theme.show_settings" to="theme_control_settings_button.opacity" formula="x+0.8" />
        </REPEAT>
        <REPEAT count="($TOOLBAR_SETTINGS_ACTION_IS_SET$ == 1)" index_name="__das1">
          <IAB action="$TOOLBAR_SETTINGS_ACTION$" ><TT text="$TOOLBAR_SETTINGS_ACTION_HINT$"/></IAB>
        </REPEAT>
      </LAYER_STACK>
      
  </TEMPLATE>
  
  <!-- ################ -->
  <!-- TOOLBAR HELPERS -->
  <!-- ################-->

  <TEMPLATE id="PARSE_ICONS" data="">
    <VAR id="N" value="0" />
    <REPEAT index_list="$data$">
      <VAR id="T$N$" value="$index$" /><NEXT_N />
    </REPEAT>
    <VAR id="TOOLBAR_ICONS_SIZE" value="$T0$" />
    <VAR id="TOOLBAR_ICONS_COLOR" value="$T1$" />
    <VAR id="TOOLBAR_ICONS_OPACITY" value="$T2$" />
    <VAR id="TOOLBAR_ICONS_PAD_H" value="$T3$" />
    <VAR id="TOOLBAR_ICONS_PAD_V" value="$T4$" />
    <VAR id="TOOLBAR_ICONS_V_OFFSET" value="$T5$" />
  </TEMPLATE>

  <TEMPLATE id="PARSE_MAX_IN_ROW" data="">
    <VAR id="N" value="0" />
    <REPEAT index_list="$data$">
      <VAR id="TOOLBAR_MAX_IN_ROW_$N$" value="$index$" />
      <NEXT_N />
    </REPEAT>
  </TEMPLATE>

  <TEMPLATE id="PARSE_TITLE" data="" text="">
    <VAR id="N" value="0" />
    <REPEAT index_list="$data$">
      <VAR id="T$N$" value="$index$" /><NEXT_N />
    </REPEAT>
    <VAR id="TB_TI_TEXT" value="$text$" />
    <VAR id="TB_TI_TEXT_SET" value="false" />
    <REPEAT index_list="$TB_TI_TEXT$">
      <VAR id="TB_TI_TEXT_SET" value="true" />
    </REPEAT>
    <VAR id="TB_TI_MAXW" value="$T0$" />
    <VAR id="TB_TI_MINW" value="$T1$" />
    <VAR id="TB_TI_PAD_H" value="$T2$" />
    <VAR id="TB_TI_PAD_V" value="$T3$" />
    <VAR id="TB_TI_H_OFFSET" value="$T4$" />
    <VAR id="TB_TI_V_OFFSET" value="$T5$" />
    <VAR id="TB_TI_TEXT_ALIGN" value="left" />
    <VAR id="T6" value="0$T6$" /> <!-- prevent formula errors -->
    <REPEAT count="($T6$==1)"><VAR id="TB_TI_TEXT_ALIGN" value="center" /></REPEAT>
    <REPEAT count="($T6$==2)"><VAR id="TB_TI_TEXT_ALIGN" value="right" /></REPEAT>
    <!-- T7-8 reserved -->
    <VAR id="TB_TI_FONT_FACE" value="$DEFAULT_FONT$" /> <!-- if empty - use default -->
    <REPEAT index_list="$T9$"><VAR id="TB_TI_FONT_FACE" value="$index$" /></REPEAT>
    <VAR id="TB_TI_FONT_SIZE" value="$T10$" />
    <VAR id="TB_TI_FONT_WEIGHT" value="$T11$" />
    <VAR id="TB_TI_FONT_STYLE" value="$T12$" />
    <VAR id="TB_TI_TEXT_COLOR" value="$T13$" />
    <VAR id="TB_TI_FONT_QUALITY" value="$T14$" />
  </TEMPLATE>

  <TEMPLATE id="PARSE_PRESETS" data="">
    <VAR id="N" value="0" />
    <REPEAT index_list="$data$">
      <VAR id="T$N$" value="$index$" /><NEXT_N />
    </REPEAT>
    <VAR id="TB_PR_WIDTH" value="$T0$" />
    <VAR id="TB_PR_MIN_HEIGHT" value="$T1$" />
    <VAR id="TB_PR_PAD_H" value="$T2$" />
    <VAR id="TB_PR_PAD_V" value="$T3$" />
    <VAR id="TB_PR_H_OFFSET" value="$T4$" />
    <VAR id="TB_PR_V_OFFSET" value="$T5$" />
    <VAR id="TB_PR_TX_HPAD" value="$T6$" />

    <VAR id="TB_PR_AR_SIZE" value="$T8$" />
    <VAR id="TB_PR_AR_COLOR" value="$T9$" />
    <VAR id="TB_PR_AR_OPACITY" value="$T10$" />
    <VAR id="TB_PR_AR_POS" value="$T11$" />
    <VAR id="TB_PR_AR_V_OFFSET" value="$T12$" />

    <!-- 11-20 reserved -->
    <VAR id="TB_PR_FONT_FACE" value="$DEFAULT_FONT$" /> <!-- if empty - use default -->
    <REPEAT index_list="$T21$"><VAR id="TB_PR_FONT_FACE" value="$index$" /></REPEAT>
    <VAR id="TB_PR_FONT_SIZE" value="$T22$" />
    <VAR id="TB_PR_FONT_WEIGHT" value="$T23$" />
    <VAR id="TB_PR_FONT_STYLE" value="$T24$" />
    <VAR id="TB_PR_TEXT_COLOR" value="$T25$" />
    <VAR id="TB_PR_FONT_QUALITY" value="$T26$" />
    <VAR id="TB_PR_FONT_V_OFFSET" value="$T27$" />
    <!-- 28-29 reserved -->
    
    <VAR id="TB_PR_BOX" value="$T30$;$T31$;$T32$;$T33$;$T34$;$T35$;$T36$;$T37$;$T38$;$T39$;$T40$;$T41$;$T42$;$T43$;$T44$;$T45$;$T46$;$T47$;$T48$" />
    <!-- gradient_type;whole_opacity;gradient_shift;stroke_width;stroke_expand;stroke_round;y_offset;;;;#bgcolor1;opacity1;pos1;#bgcolor2;opacity2;pos2;#strokecolor;opacity3;unused; -->
    
  </TEMPLATE>

  <!-- icons for toolbar -->
  <TEMPLATE id="TOOLBAR_ICON" icon="MENU" widget_id="" color='$TOOLBAR_ICONS_COLOR$' size='$TOOLBAR_ICONS_SIZE$' opacity="$TOOLBAR_ICONS_OPACITY$">
    <WIDGET id="$widget_id$" opacity="$opacity$">
      <LAYER_STACK>
        <SVG_BG color="#FFFFFF" opacity="0" />
        <DEFINE><_SVG_ICON base_type="SVG_ICON_$icon$" /></DEFINE>
        <_SVG_ICON color='$color$' size='$size$' />
        <UNDEFINE><_SVG_ICON /></UNDEFINE>
      </LAYER_STACK>
    </WIDGET>
  </TEMPLATE>
  
  <!-- template to display toolbar items layout split by max_in_row -->
  <TEMPLATE id="TB_LAYOUT_ITEMS" max_in_row="2" items="" h_align="left" flex="0">
    <LV id="ITEMS_N" value="0" />
    <REPEAT index_list="$items$">
      <VAR id="TBITEM_$ITEMS_N$" value="$index$" />
      <LV id="ITEMS_N" formula="$ITEMS_N$+1" />
    </REPEAT>
    <LV id="COLS_NO" formula="ceil($ITEMS_N$/$max_in_row$)" />
    <LV id="CUR_ITEM" value="0" />
    <COLUMN flex="$flex$">
      <REPEAT count="$COLS_NO$" index_name="CUR_COL">
        <LV id="ITEMS_IN_ROW" formula="(($CUR_ITEM$ + $max_in_row$) &lt;= $ITEMS_N$)*($max_in_row$) + (($CUR_ITEM$ + $max_in_row$) > $ITEMS_N$)*($ITEMS_N$-$CUR_ITEM$)" />
        <ROW h_align="$h_align$" internal_h_align="$h_align$">
          <REPEAT count="$ITEMS_IN_ROW$" start="$CUR_ITEM$">
            <DEFINE><_THIS_ base_type="TOOLBAR_$TBITEM_$index$$" /></DEFINE>
            <_THIS_ />
            <UNDEFINE><_THIS_ /></UNDEFINE>
            <LV id="CUR_ITEM" formula="$CUR_ITEM$+1" />
          </REPEAT>
        </ROW>
      </REPEAT>
    </COLUMN>
  </TEMPLATE>

  <!-- ################# -->
  <!-- TOOLBAR SKIN MENU -->
  <!-- ################# -->

  <!-- for exported plugin -->
  <IF_EXPORTED>
    <POPUP_MENU id="skin_menu">

      <!-- if toolbar is hidden, add main menu to this menu -->
      <IF condition="$TOOLBAR_DISPLAY$ == 0">
        <MENU_ITEM action_id="OpenMenuOriginal" />
        <MENU_SEPARATOR />
      </IF>

      <!-- actions for current skin -->
      <MENU_ITEM action_id="ReloadSkin" />
      <MENU_ITEM action_id="OpenSkinFile" />
      <MENU_ITEM action_id="OpenSkinDirWrap" />
      <MENU_SEPARATOR />
      
      <!-- actions for log -->
      <MENU_ITEM action_id="OpenLogFile" />
      <!-- <MENU_ITEM action_id="OpenScriptDoc" /> -->

    </POPUP_MENU>
  </IF_EXPORTED>

  <ONLOAD script="@a_lm_menu = skin_menu.Popup;" /> 

  <!-- #################### -->
  <!-- TOOLBAR LAYOUT START -->
  <!-- #################### -->

  <LAYER_STACK id="toolbar_area" width="100%" min_width="150" min_height="$TOOLBAR_MIN_HEIGHT$" display="$TOOLBAR_DISPLAY_BOOL$">

    <!-- toolbar background -->
    <IF condition="$SKIN_EDITABLE$">
      <KUIML_WIDGET width="100%" height="100%" id="toolbar_bg" innerKUIML="&lt;SKIN width='100%' height='100%'>&lt;LAYER_STACK width='100%' height='100%'> &lt;LM_BG_PARSER data='$TOOLBAR_BG$' />&lt;LM_TOOLBAR_DECOR toolbar='$DECOR_TOOLBAR$' opacity='$DECOR_OPACITY$' />&lt;/LAYER_STACK> &lt;/SKIN>" exposed="true" />
    </IF>
    <IF condition="!$SKIN_EDITABLE$">
      <LM_BG_PARSER data="$TOOLBAR_BG$" />
      <LM_TOOLBAR_DECOR toolbar='$DECOR_TOOLBAR$' opacity='$DECOR_OPACITY$' />
    </IF>

    <!-- toolbar contents -->
    <ROW id="toolbar_content" v_offset="$TOOLBAR_V_OFFSET$" h_margin="$TOOLBAR_PAD_H$" v_margin="$TOOLBAR_PAD_V$" flex="1">

      <!-- load toolbar statically for normal mode -->
      <IF condition="!$SKIN_EDITABLE$">
        <!-- unparse comma-separated data into vars -->
        <PARSE_ICONS data="$TOOLBAR_ICONS$" />
        <PARSE_MAX_IN_ROW data="$TOOLBAR_MAX_IN_ROW$" />
        <PARSE_TITLE data="$TOOLBAR_TITLE$" text="$TOOLBAR_TITLE_TEXT$" />
        <PARSE_PRESETS data="$TOOLBAR_PRESETS$" />

        <CELL layout_type="row" width="100%" text_color="$TB_TI_TEXT_COLOR$">
          <!-- left side items -->
          <TB_LAYOUT_ITEMS h_align="left" max_in_row="$TOOLBAR_MAX_IN_ROW_0$" items="$TOOLBAR_ITEMS_LEFT$" />
          
          <!-- center items-->
          <TB_LAYOUT_ITEMS h_align="center" max_in_row="$TOOLBAR_MAX_IN_ROW_1$" items="$TOOLBAR_ITEMS_CENTER$" flex="1" />

          <!-- right side items -->
          <TB_LAYOUT_ITEMS h_align="right" max_in_row="$TOOLBAR_MAX_IN_ROW_2$" items="$TOOLBAR_ITEMS_RIGHT$" />
        </CELL>
      </IF>

      <!-- load toolbar dynamically for skin preview mode -->
      <IF condition="$SKIN_EDITABLE$">
        <KUIML_WIDGET id="toolbar_widget" width='100%' innerKUIML="&lt;SKIN>&lt;/SKIN>" exposed="true" />
        <STRING_LINK id="link_toolbar_innerKUIML" from="a_toolbar_innerKUIML" to="toolbar_widget.innerKUIML" enabled="false" />
        <EXPOSED_OBJECTS object_ids="link_toolbar_innerKUIML.*" />
      </IF>
    </ROW> <!-- // END TOP ROW -->

  </LAYER_STACK>

  <!-- ################## -->
  <!-- TOOLBAR LAYOUT END -->
  <!-- ################## -->
</DUMMY>