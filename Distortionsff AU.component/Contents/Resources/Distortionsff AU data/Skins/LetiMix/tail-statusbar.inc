<?xml version="1.0" encoding="utf-8" ?>
<DUMMY>

<!-- ============================== -->
<!-- STATUS BAR ITEMS AND TEMPLATES -->
<!-- ============================== -->

<TEMPLATE id="STATUS_ICON">
  <REPEAT count="($SB_ICON_SHOW_MODE$ == 3)">
    <STATUS_SEPARATOR cell_id="separator_for_status_icon" />
  </REPEAT>
  <ROW h_margin="$SB_ICON_PAD_H$" v_offset="$SB_ICON_V_OFFSET$" h_offset="$SB_ICON_H_OFFSET$" id="cell_status_icon">
    <LS>
      <CELL id="cell_status_0"><SVG_ICON_RUNNING size="$SB_ICON_SIZE$" color="$SB_ICON_C1$" opacity="$SB_ICON_O1$" /></CELL>
      <CELL id="cell_status_1"><SVG_ICON_WARNING size="$SB_ICON_SIZE$" color="$SB_ICON_C2$" opacity="$SB_ICON_O2$" /></CELL>
      <CELL id="cell_status_2"><SVG_ICON_ERROR size="$SB_ICON_SIZE$" color="$SB_ICON_C3$" opacity="$SB_ICON_O3$" /></CELL>
      <CELL id="cell_status_3"><SVG_ICON_STOPWATCH size="$SB_ICON_SIZE$" color="$SB_ICON_C4$" opacity="$SB_ICON_O4$" /></CELL>
      <IAB action="OpenLogFile"><TOOLTIP string_id="status_log_text" /></IAB>
    </LS>
    <PARAM_LINK from="$script_status$" to="cell_status_0.display" formula="(x==0)" />
    <PARAM_LINK from="$script_status$" to="cell_status_1.display" formula="(x==1)" />
    <PARAM_LINK from="$script_status$" to="cell_status_2.display" formula="(x==2)" />
    <PARAM_LINK from="$script_status$" to="cell_status_3.display" formula="(x==3)" />
  </ROW>
  <REPEAT count="($SB_ICON_SHOW_MODE$ == 2)">
    <STATUS_SEPARATOR cell_id="separator_for_status_icon" />
  </REPEAT>
  <REPEAT count="($SB_ICON_SHOW_MODE$ > 0.9)">
    <PARAM_LINK from="$script_status$" to="cell_status_icon.display" formula="(x>0)*(x&lt;3)" />
  </REPEAT>
  <REPEAT count="($SB_ICON_SHOW_MODE$ > 1.9)">
    <PARAM_LINK from="$script_status$" to="separator_for_status_icon.display" formula="(x>0)*(x&lt;3)" />
  </REPEAT>
</TEMPLATE>

<TEMPLATE id="STATUS_STATE">
    <PARAM_TEXT param_id="$script_status$" content="{text_value}">
      <IAB action="OpenLogFile"><TOOLTIP string_id="status_log_text" /></IAB>
    </PARAM_TEXT>
</TEMPLATE>

<TEMPLATE id="STATUS_LOG_LINE">
  <REPEAT count="($SB_LOG_SHOW_MODE$ == 3)">
    <STATUS_SEPARATOR cell_id="separator_for_log_line" />
  </REPEAT>
  <LAYER_STACK width="$LWIDTH$" min_width="$SB_LOG_MINWIDTH$" flex="1" id="cell_status_log_text">
    <TEXT width="100%" id="tx_status_log_text" string_id="$script_status_string$" v_offset="0" text_h_align="left" h_align="left" />
    <!-- a nice tiny animation on "pushed" log line -->
    <PARAM_LINK from="open_log_button.pushed" to="tx_status_log_text.v_offset"/>
    <PARAM_LINK from="open_log_button.pushed" to="tx_status_log_text.h_offset"/>
    <IAB action="OpenLogFile" id="open_log_button"><TOOLTIP string_id="status_log_text" /></IAB>
  </LAYER_STACK>
  <REPEAT count="($SB_LOG_SHOW_MODE$ ==2)">
    <STATUS_SEPARATOR cell_id="separator_for_log_line" />
  </REPEAT>
  <REPEAT count="($SB_LOG_SHOW_MODE$ > 0)" index_name="_ddisnfs">
    <PARAM_LINK from="_a_log_unusual_text" to="cell_status_log_text.display" />
  </REPEAT>
  <REPEAT count="($SB_LOG_SHOW_MODE$ > 1.9)">
    <PARAM_LINK from="_a_log_unusual_text" to="separator_for_log_line.display" />
  </REPEAT>
</TEMPLATE>

<TEMPLATE id="STATUS_CUSTOM_TEXT">
  <LAYER_STACK min_width="$SB_CUSTEXT_MINWIDTH$" flex="1" >
    <UNDEFINE><SB_CUS_TEXT /></UNDEFINE>
    <REPEAT count="$SB_CUSTEXT_EDITABLE$ == 0" index_name="_dcte10">
      <DEFINE><SB_CUS_TEXT base_type="TEXT" /></DEFINE>
    </REPEAT>
    <REPEAT count="$SB_CUSTEXT_EDITABLE$ == 1" index_name="_dcte11">
      <DEFINE><SB_CUS_TEXT base_type="TEXT_FIELD"  /></DEFINE>
    </REPEAT>
    <SB_CUS_TEXT width="100%" id="tx_status_custom_text" string_id="status_custom_text" text_h_align="left" h_align="left"><TT string_id="status_custom_text" /></SB_CUS_TEXT>
    <EXPOSED_OBJECTS object_ids="tx_status_custom_text.*" />
    <PARAM_CONNECTION id1="a_status_custom_text_opacity" id2="tx_status_custom_text.opacity" />
  </LAYER_STACK>
</TEMPLATE>

<TEMPLATE id="STATUS_SEPARATOR" cell_id="">
  <CELL v_margin="0" h_margin="$SB_SEP_PAD_H$" v_offset="$SB_SEP_OFFSET$" id="$cell_id$">
    <svg width="$SB_SEP_W$" height="$SB_SEP_H$" viewBox='0 0 10 10' opacity="$SB_SEP_OP$">
      <rect fill="$SB_SEP_COLOR$" width="100%" height="100%" />
    </svg>
  </CELL>
</TEMPLATE>

<TEMPLATE id="STATUS_SAMPLERATE">
  <PARAM_TEXT param_id="dsp.sample_rate" content="{value} Hz" value_format="0.0" />
</TEMPLATE>

<TEMPLATE id="STATUS_BLOCKSIZE">
  <PARAM_TEXT param_id="dsp.max_block_size" content="{value} smp" value_format="0.0" />
</TEMPLATE>

<TEMPLATE id="STATUS_CHANNELS">
  <PARAM_TEXT param_id="dsp.connected_inputs_count" content="{value}" value_format="0.0" />
  <TEXT value="/" />
  <PARAM_TEXT param_id="dsp.connected_outputs_count" content="{value}" value_format="0.0" />
</TEMPLATE>

<TEMPLATE id="STATUS_CORNER_RESIZER_SPACE">
  <CELL width="$STATUS_CORNER_RESIZER_SPACE$" />
</TEMPLATE>


<!-- showing tooltip with whole log line -->
<STRING id="status_subtext_status" default="" />
<PARAM_TO_STRING_LINK from="$script_status$" to="status_subtext_status" content="Status: {text_value}" />
<STRING id="status_subtext_click" default="[ Click to open log file ]" />
<STRING id="status_log_text" default="---" />
<STRING_MULTI_LINK from="$script_status_string$;status_subtext_status;status_subtext_click" to="status_log_text" enabled="true" separator="&#10;" />

<!-- explode params into vars -->
<TEMPLATE id="PARSE_STATUS_PARAMS" content=";0;-;-;#FFFFFF;default;5;150" layout="0;0;135;1;150" flex_cells="1;0;0">
  <VAR id="N" value="0" />
  <REPEAT index_list="$content$">
    <VAR id="T$N$" value="$index$" /><NEXT_N />
  </REPEAT>
  <VAR id="SB_FONT_FACE" value="$DEFAULT_FONT$" /> <!-- if empty - use default -->
  <REPEAT index_list="$T0$"><VAR id="SB_FONT_FACE" value="$index$" /></REPEAT>
  <VAR id="SB_FONT_SIZE" value="$T1$" />
  <VAR id="SB_FONT_WEIGHT" value="$T2$" />
  <VAR id="SB_FONT_STYLE" value="$T3$" />
  <VAR id="SB_TEXT_COLOR" value="$T4$" />
  <VAR id="SB_FONT_QUALITY" value="$T5$" />
  <VAR id="SB_ITEMS_SPACING" value="$T6$" />
  <VAR id="SB_CELLS_SPACING" value="$T7$" />
  <!-- 8 reserved -->
  <!-- <VAR id="SB_LOG_SHOW_MODE" value="$T9$" /> 
  <VAR id="SB_LOG_MINWIDTH" value="$T10$" /> -->
  <VAR id="SB_ICON_SIZE" value="$T11$" />
  <VAR id="SB_ICON_PAD_H" value="$T12$" />
  <VAR id="SB_ICON_H_OFFSET" value="$T13$" />
  <VAR id="SB_ICON_V_OFFSET" value="$T14$" />
  <!-- <VAR id="SB_ICON_SHOW_MODE" value="$T15$" /> -->
  <VAR id="SB_ICON_C1" value="$T16$" />
  <VAR id="SB_ICON_O1" value="$T17$" />
  <VAR id="SB_ICON_C2" value="$T18$" />
  <VAR id="SB_ICON_O2" value="$T19$" />
  <VAR id="SB_ICON_C3" value="$T20$" />
  <VAR id="SB_ICON_O3" value="$T21$" />
  <VAR id="SB_ICON_C4" value="$T22$" />
  <VAR id="SB_ICON_O4" value="$T23$" />

  <VAR id="SB_SEP_COLOR" value="$T25$" />
  <VAR id="SB_SEP_OP" value="$T26$" />
  <VAR id="SB_SEP_W" value="$T27$" />
  <VAR id="SB_SEP_H" value="$T28$" />
  <VAR id="SB_SEP_PAD_H" value="$T29$" />
  <VAR id="SB_SEP_OFFSET" value="$T30$" />
  <!--
  <VAR id="SB_CUSTEXT_MINWIDTH" value="$T33$" />
  <VAR id="SB_CUSTEXT_EDITABLE" value="$T34$" />
  -->

  <VAR id="N" value="0" />
  <REPEAT index_list="$flex_cells$">
    <VAR id="T$N$" value="$index$" /><NEXT_N />
  </REPEAT>
  <VAR id="STATUS_FLEX_left" value="$T0$" />
  <VAR id="STATUS_FLEX_center" value="$T1$" />
  <VAR id="STATUS_FLEX_right" value="$T2$" />

  <VAR id="N" value="0" />
  <REPEAT index_list="$layout$">
    <VAR id="T$N$" value="$index$" /><NEXT_N />
  </REPEAT>
  <VAR id="SB_ICON_SHOW_MODE" value="$T0$" />
  <VAR id="SB_LOG_SHOW_MODE" value="$T1$" /> 
  <VAR id="SB_LOG_MINWIDTH" value="$T2$" />
  <VAR id="SB_CUSTEXT_EDITABLE" value="$T3$" />
  <VAR id="SB_CUSTEXT_MINWIDTH" value="$T4$" />
  
  <R count="$LM_SKIN_RESIZEABLE$">
    <VAR id="STATUS_ITEMS_RIGHT" value="$STATUS_ITEMS_RIGHT$;CORNER_RESIZER_SPACE" />
  </R>
</TEMPLATE>

<!-- template to display status bar items  -->
<TEMPLATE id="SB_LAYOUT_ITEMS" items="" h_align="left" spacing="0">
  <LV id="is_flex" value="0" />
  <REPEAT count="($STATUS_FLEX_$h_align$$ > 0)">
    <LV id="is_flex" value="1" />
  </REPEAT>
  <ROW h_align="$h_align$" internal_h_align="$h_align$" flex="$is_flex$" spacing="$spacing$">
    <REPEAT index_list="$items$">
      <DEFINE><_THIS_ base_type="STATUS_$index$" /></DEFINE>
      <_THIS_ is_flex="$is_flex$" />
      <UNDEFINE><_THIS_ /></UNDEFINE>
    </REPEAT>
  </ROW>
</TEMPLATE>

<!-- ======================= -->
<!-- STATUS BAR LAYOUT START -->
<!-- ======================= -->

<LAYER_STACK width="100%" min_width="150" id="status_area" display="$STATUS_DISPLAY_BOOL$" min_height="$STATUS_MIN_HEIGHT$">

  <!-- status background -->
  <IF condition="!$SKIN_EDITABLE$">
    <LM_BG_PARSER data="$STATUS_BG$" />
    <LM_STATUS_DECOR status='$DECOR_STATUS$' opacity='$DECOR_OPACITY$' />
  </IF>

  <!-- status bar content -->
  <ROW id="status_content" width="100%" v_offset="$STATUS_V_OFFSET$" h_margin="$STATUS_PAD_H$">

    <!-- load status statically for normal mode -->
    <IF condition="!$SKIN_EDITABLE$">
      <PARSE_STATUS_PARAMS content="$STATUS_CONTENT$" flex_cells="$STATUS_FLEX_CELLS$" layout="$STATUS_LAYOUT$" />
      <CELL layout_type="row" width="100%" font_face="$SB_FONT_FACE$" font_size="$SB_FONT_SIZE$" font_weight="$SB_FONT_WEIGHT$" font_style="$SB_FONT_STYLE$" font_quality="$SB_FONT_QUALITY$" text_color="$SB_TEXT_COLOR$" spacing="$SB_CELLS_SPACING$">
        <!-- left side items -->
        <SB_LAYOUT_ITEMS h_align="left" items="$STATUS_ITEMS_LEFT$" spacing="$SB_ITEMS_SPACING$" />
        
        <!-- center items-->
        <SB_LAYOUT_ITEMS h_align="center" items="$STATUS_ITEMS_CENTER$" spacing="$SB_ITEMS_SPACING$" />

        <!-- right side items -->
        <SB_LAYOUT_ITEMS h_align="right" items="$STATUS_ITEMS_RIGHT$" spacing="$SB_ITEMS_SPACING$" />
      </CELL>
    </IF>
  </ROW> 

</LAYER_STACK>

<!-- ===================== -->
<!-- STATUS BAR LAYOUT END -->
<!-- ===================== -->

<!-- param for status bar dynamic show/hide -->
<ONLOAD script="a_status_display = $STATUS_DISPLAY$; _a_log_unusual_text = false; " />

<!-- refresh log lines analyzing on script reloading/status change -->
<ACTION_TRIGGER event_id="$script_status$.value_changed" requires="$script_status$" script="_a_log_unusual_text = false;" />

<!-- check if we receive unusual text in log -->
<ACTION_TRIGGER event_id="$script_status_string$.value_changed;window.loaded.value_changed;_a_log_unusual_text.value_changed" requires="$script_status_string$" script="
  
  /* skip if unusual text was already detected */
  if (_a_log_unusual_text &lt; 0.5) {
    string logline = $script_status_string$; 
    bool usual_text = (((logline.findFirst(&quot;Script successfully loaded&quot;) > -1) or (logline.findFirst(&quot;Loading script file&quot;) > -1) or (logline.findFirst(&quot;Loading native script file&quot;) > -1) or (logline == &quot;&quot;)) and !(logline.findFirst(&quot;warning&quot;) > -1));
    if (!usual_text) {
      _a_log_unusual_text = true;
      a_status_invalidate++;
    }
  }
" />

<!-- <PARAM_TEXT param_id="_a_log_unusual_text" /> -->

<!-- show status bar only if unusual strings received -->
<ACTION_TRIGGER event_id="a_status_invalidate.value_changed" script="
  _a_do_show_status_area = false; 
  /* if always show statubar */
  if (a_status_display > 1.9) {
    _a_do_show_status_area = true;
  } else 
  /* if show statubar only when needed */
  if (a_status_display > 0.9) { 
    _a_do_show_status_area = (_a_log_unusual_text > 0.5); 
  } 
  status_area.display = _a_do_show_status_area;
  " requires="status_area.display;$script_status_string$" async="true" />

</DUMMY>
