<?xml version="1.0" encoding="utf-8" ?>
<DUMMY>

<!-- we make a complicated meters template to support different layouts -->

<TEMPLATE id="LM_METERS" type="input" h_align="left" num="1">
  <!-- convert type (input/output) into numeric value -->
  <LV id="meters_type" value="0" />
  <LV id="meters_type_input" value="0" />
  <LV id="meters_type_output" value="1" />
  <REPEAT index_list="$type$" index_name="_lm_i_">
    <LV id="meters_type" value="$meters_type_$_lm_i_$$" />
  </REPEAT>
  <!-- set vars appropriate to type (input/output) -->
  <LV id="v_align" value="top" />
  <LV id="h_align" value="left" />
  <REPEAT count="$meters_type$"> <!-- for output meters = 1 -->
    <LV id="v_align" value="bottom" />
    <LV id="h_align" value="right" />
  </REPEAT>
  <!-- set vars according to vertical/horizontal meters -->
  <LV id="lay_type" value="row" />
  <LV id="lwidth" value="" />
  <LV id="lheight" value="100%" />
  <LV id="min_height" value="$METERS_MIN_WIDTH$" />
  <LV id="min_width" value="$METERS_MIN_WIDTH$" />
  <LV id="min_h_single" value="$METERS_MIN_WIDTH_SINGLE$" />
  <LV id="min_w_single" value="$METERS_MIN_WIDTH_SINGLE$" />
  <LV id="h_margin" value="$METERS_PAD_H$" />
  <LV id="h_margin_b" value="$METERS_PAD_H_half$" />
  <!-- <LV id="h_margin_b" value="$METERS_PAD_H_half_$type$$" /> -->
  <LV id="v_margin" value="$METERS_PAD_V$" />
  <LV id="h_offset" value="$METERS_PAD_H_half_$type$$" />
  <LV id="v_offset" value="$METERS_V_OFFSET$" />
  <LV id="bg_v_offset" value="$METERS_BOX_OFFSET$" />
  <LV id="bg_h_offset" value="0" />
  <LV id="bg_pad_h" value="$METERS_BOX_PAD_A$" />
  <LV id="bg_pad_v" value="$METERS_BOX_PAD_B$" />
  <REPEAT count="!$METERS_ARE_HORIZONTAL$">
    <LV id="v_align" value="" />
    <LV id="min_height" value="" />
    <LV id="min_h_single" value="" />
  </REPEAT>
  <REPEAT count="$METERS_ARE_HORIZONTAL$">
    <LV id="lay_type" value="column" />
    <LV id="lwidth" value="100%" />
    <LV id="lheight" value="" />
    <LV id="lheight" value="" />
    <LV id="h_align" value="" />
    <LV id="min_width" value="" />
    <LV id="min_w_single" value="" />
    <LV id="h_margin" value="0" />
    <LV id="h_margin_b" value="$METERS_PAD_V$" />
    <LV id="v_margin" value="$METERS_PAD_H_half$" />
    <LV id="v_offset" formula="$METERS_PAD_H_half_$type$$" />
    <LV id="h_offset" formula="-($METERS_V_OFFSET$)" />
    <LV id="bg_v_offset" value="0" />
    <LV id="bg_h_offset" formula="-($METERS_BOX_OFFSET$)" />
    <LV id="bg_pad_h" value="$METERS_BOX_PAD_B$" />
    <LV id="bg_pad_v" value="$METERS_BOX_PAD_A$" />
  </REPEAT>

  <VAR id="typenum" value="$type$$num$" />
  <!-- this param is used for limiting max number of meters -->
  <LV id="max_cnt_formula" value="$$type$_channels_count$" />
  <REPEAT count="($METERS_HIDE_INACTIVE$)" index_name="__dd">
    <LV id="max_cnt_formula" value="min(dsp.connected_$type$s_count, max(1,floor(a_meters_$type$_channels)))" />
  </REPEAT>
  <FORMULA_PARAM id="$typenum$_meters_max_cnt" max="16" formula="$max_cnt_formula$" />

  <ONLOAD script="a_meters_$type$_channels = $METERS_$type$_CHANNELS$;" />
  <!-- VARs to create formula_param for combined meters (see below) -->
  <VAR id="$type$_lvls_combo_formula" value="-60"/>
  <VAR id="$type$_hlvls_combo_formula" value="-60"/>

  <!-- start meters layout -->
  <CELL id="cell_$typenum$_levels_wrap" height="$lheight$" width="$lwidth$" display="$METERS_DISPLAY_$type$$" h_align="$h_align$" v_align="$v_align$" h_margin="$h_margin_b$">
    <CELL layout_type="$lay_type$" height="$lheight$" width="$lwidth$" id="cell_meters_$typenum$">
      <LAYER_STACK id="cell_$typenum$_levels" height="$lheight$" width="$lwidth$" spacing="0" h_align="$h_align$" v_align="$v_align$" v_margin="$v_margin$"  h_offset="$h_offset$" v_offset="$v_offset$" min_height="$min_height$" min_width="$min_width$">
        <!-- load background statically (skin loads faster) -->
        <IF condition="!$SKIN_EDITABLE$"><LM_METERS_BOX /></IF>
        <!-- now for each meters -->
        <CELL layout_type="$lay_type$" flex="1" id="cell_meters_all_$typenum$" h_margin="$bg_pad_h$" v_margin="$bg_pad_v$" v_offset="$bg_v_offset$" h_offset="$bg_h_offset$" spacing="$METERS_BOX_SPACING$">
          <REPEAT count="$$type$_channels_count$" index_name="i" start="1">
            <LV id="LEVEL_PARAM_NAME" value="$$type$_level$i$$" />
            <LV id="HLEVEL_PARAM_NAME" value="$$type$_hold_level$i$$" />
            <!-- if we combine all channels in one -->
            <IF condition="($METERS_$type$_CHANNELS$ == 0)" index_name="_d$typenum$1_">
              <!-- building vars holding 'max' formula -->
              <VAR id="$type$_lvls_combo_formula" value="max($$type$_lvls_combo_formula$, $LEVEL_PARAM_NAME$)" />
              <VAR id="$type$_hlvls_combo_formula" value="max($$type$_hlvls_combo_formula$, $HLEVEL_PARAM_NAME$)" />
              <!-- change params names passing to CANVAS render -->
              <LV id="LEVEL_PARAM_NAME" value="$type$_levels_max" />
              <LV id="HLEVEL_PARAM_NAME" value="$type$_hlevels_max" />
            </IF>

            <!-- showing the meter itself -->
            <LAYER_STACK height="$lheight$" width="$lwidth$" id="lm_meter_$typenum$_ch$i$" flex="1" min_height="$min_h_single$" min_width="$min_w_single$">
              <CANVAS opacity="1" id="lm_canvas_$typenum$_ch$i$" width="100%" height="100%" render_script="LM::renderMeter(Kt::Graphics::GetCurrentContext(), this.width, this.height, $METERS_ORIENTATION$, LM::meterRenderParams, $LEVEL_PARAM_NAME$, $HLEVEL_PARAM_NAME$);" requires="lm_canvas_$typenum$_ch$i$.width;lm_canvas_$typenum$_ch$i$.height;$LEVEL_PARAM_NAME$;$HLEVEL_PARAM_NAME$" />
            </LAYER_STACK>

            <!-- action to refresh on changing parameter -->
            <ACTION_TRIGGER event_id="$LEVEL_PARAM_NAME$.value_changed;$HLEVEL_PARAM_NAME$.value_changed;a_meters_invalidate.value_changed" script="lm_canvas_$typenum$_ch$i$.Invalidate()" requires="lm_canvas_$typenum$_ch$i$.Invalidate" async="true" />
            <!-- hiding currently unactive channels -->
            <PARAM_LINK from="$typenum$_meters_max_cnt" to="lm_meter_$typenum$_ch$i$.display" formula="$i$&lt;=x" />
            
            <!-- cell to space between groups of meters -->
            <CELL id="cell_space_$typenum$_ch$i$" width="$METERS_GROUP_SPACING$" height="$METERS_GROUP_SPACING$" />
            <FORMULA_PARAM id="show_spacer_$typenum$_ch$i$" formula="$i$&lt;=($typenum$_meters_max_cnt-1)*($i$/a_meters_group_$type$ == floor($i$/a_meters_group_$type$))*(a_meters_group_$type$>0)" />
            <PARAM_LINK from="show_spacer_$typenum$_ch$i$" to="cell_space_$typenum$_ch$i$.display" />
          </REPEAT>
        </CELL>
      </LAYER_STACK>
      <!-- params for "single combined" meters (one for all channels) -->
      <IF condition="(($METERS_$type$_CHANNELS$ == 0)*($num$==1))" index_name="_d$type$2_">
        <FORMULA_PARAM id="$type$_levels_max" min="-60" max="0" formula="$$type$_lvls_combo_formula$" />
        <FORMULA_PARAM id="$type$_hlevels_max" min="-60" max="0" formula="$$type$_hlvls_combo_formula$" />
      </IF>
    </CELL>
  </CELL>
</TEMPLATE>


<TEMPLATE id="LM_INPUT_METERS">
  <IF condition="!$SKIN_EDITABLE$">
    <!-- for static skin show input meter here only if placed "outside body" -->
    <IF condition="($METERS_PLACEMENT$ == 0)+($METERS_PLACEMENT$ == 2)" index_name="_">
      <LM_METERS type="input" /> 
    </IF>
  </IF>
</TEMPLATE>

<TEMPLATE id="LM_OUTPUT_METERS">
  <IF condition="!$SKIN_EDITABLE$">
    <!-- for static skin show input meter here if placed "inside body", 
    so that it overlays the body -->
    <IF condition="($METERS_PLACEMENT$ == 1)+($METERS_PLACEMENT$ == 3)" index_name="_">
      <LM_METERS type="input" /> 
    </IF>
  </IF>
  <LM_METERS type="output" h_align="right" /> <!-- output meters -->
</TEMPLATE>


<!-- ==================== -->
<!-- Body templates -->
<!-- ==================== -->

<TEMPLATE id="PARSE_BODY_FONT" data="">
  <VAR id="N" value="0" />
  <REPEAT index_list="$data$">
    <VAR id="T$N$" value="$index$" /><NEXT_N />
  </REPEAT>
  <VAR id="BODY_FONT_FACE" value="$DEFAULT_FONT$" /> <!-- if empty - use default -->
  <REPEAT index_list="$T0$"><VAR id="BODY_FONT_FACE" value="$index$" /></REPEAT>
  <VAR id="BODY_FONT_SIZE" value="$T1$" />
  <VAR id="BODY_FONT_WEIGHT" value="$T2$" />
  <VAR id="BODY_FONT_STYLE" value="$T3$" />
  <VAR id="BODY_TEXT_COLOR" value="$T4$" />
  <VAR id="BODY_FONT_QUALITY" value="$T5$" />
</TEMPLATE>

<!-- ================= -->
<!-- BODY LAYOUT START -->
<!-- ================= -->

<!-- Main column -->
<!-- RE100 is 100% when skin is resizeable, otherwise empty -->
<LAYER_STACK id="body_wrap_cell" min_width="$BODY_MIN_WIDTH$" min_height="$BODY_MIN_HEIGHT$" v_align="$BODY_V_ALIGN$" width="$RE100$" height="$RE100$" flex="1">
  
  <!-- Body background -->
  <!-- for exported plugins show background 
  without KUIML_WIDGET for faster loading -->
  <IF condition="!$SKIN_EDITABLE$">
      <LM_BG_PARSER data="$BODY_BG$" />
      <LM_BODY_DECOR body1='$DECOR_BODY1$' body2='$DECOR_BODY2$' body3='$DECOR_BODY3$' body4='$DECOR_BODY4$' opacity='$DECOR_OPACITY$' />
  </IF>

  <!-- body shading -->
  <SCRIPT script="LM::bodyShaderClass bodyShader(&quot;$BODY_SHADING_DATA$&quot;, body_shading_data);" />
  <CANVAS opacity="1" id="body_shading_canvas" requires="body_shading_canvas.*" width="100%" height="100%" render_script="bodyShader.Draw(this.height, this.width)"  />
  
  <!-- Body content -->
  <CELL layout_type="$METERS_PLACEMENT_LAYOUT$" id="body_and_meters_cell" min_width="$BODY_MIN_WIDTH$" min_height="$BODY_MIN_HEIGHT$" width="$RE100$" height="$RE100$">
    
    <LM_INPUT_METERS />
    
    <CELL id="body_content_cell" flex="1" _v_align="$BODY_V_ALIGN$" width="$RE100$" height="$RE100$">
      <IF condition="!$BODY_STATIC_FILE_SET$">
        <!-- A placeholder for dynamic subskin (.kuiml file)
         if no .kuiml is available, then auto-layout is used -->
        
        <KUIML_WIDGET id="subskin_widget" v_margin="$BODY_PAD_V$" h_margin="$BODY_PAD_H$" v_align="$BODY_V_ALIGN$" innerKUIML="" exposed="true" width="$RE100$" height="$RE100$" />
        <!-- body content is auto-loaded via "reloadBody()" in update_skin.as -->
        <STRING_LINK from="a_body_innerKUIML" to="subskin_widget.innerKUIML" />
        <ONLOAD script="a_body_font = &quot;$BODY_FONT$&quot;;" />
      </IF>

       <!-- if static kuiml file set, include it directly -->
      <IF condition="$BODY_STATIC_FILE_SET$">
        <PARSE_BODY_FONT data="$BODY_FONT$" />
        <CELL id="subskin_static" v_margin="$BODY_PAD_V$" h_margin="$BODY_PAD_H$" v_align="$BODY_V_ALIGN$" font_size="$BODY_FONT_SIZE$" font_face="$BODY_FONT_FACE$" font_weight="$BODY_FONT_WEIGHT$" font_style="$BODY_FONT_STYLE$" font_quality="$BODY_FONT_QUALITY$" text_color="$BODY_TEXT_COLOR$" width="$RE100$" height="$RE100$">
          <INC file="$SKINS_DIR$$BODY_STATIC_FILE$" />
        </CELL>
      </IF>
    </CELL>

    <LM_OUTPUT_METERS />
  </CELL>
  
  <!-- demo warning -->
  <IF_PNS><IF_EXPORTED><REPEAT count="$SCRIPT_DEMO_MODE$">
    <WIDGET id="demo_pane" layout_type="column" margin="20" background_color="#000000" text_color="#eeeeee" opacity="0.95" visible="false">
      <TEXT value="DEMO VERSION" font_size="+1" font_weight="bold"/>
      <CELL height="5" />
      <TEXT value="This is a limited demo version." font_weight="bold"/>
      <TEXT value="Saving is disabled, and the plug-in"/>
      <TEXT value="will produce silence after 10 minutes."/>
    </WIDGET>
    <PARAM id="demo_shown" persistent="true" default="0" max="1" exposed="true"/>
    <ACTION_TRIGGER event_id="window.loaded.value_changed" script="if (demo_shown == 0) demo_pane.visible = true; " requires="demo_pane.visible" />
    <TIMER refresh_time_ms="3000" id="demo_pane.timer"/>
    <ACTION_TRIGGER event_id="demo_pane.timer.elapsed" script="demo_pane.timer.enabled=false; demopane_animator = 0; if (demo_shown==0) {demo_shown=1;}" requires="demo_pane.timer.enabled"/>
    <PARAM_ANIMATOR id="demopane_animator" param_id="demo_pane.opacity" refresh_time_ms="50" duration_ms="1000" enabled="true" exposed="true" />
  </REPEAT></IF_EXPORTED></IF_PNS>
  <!-- // demo warning end -->

</LAYER_STACK>

<!-- ================= -->
<!-- BODY LAYOUT ENDED -->
<!-- ================= -->


<ONLOAD script="body_loaded = body_loaded+1;" />
</DUMMY>