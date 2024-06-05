<?xml version="1.0" encoding="utf-8" ?>
<DUMMY>
	<!-- basic defines -->
	<DEFINE>
		<VAR base_type="VARIABLE" override="true" />
		<LV base_type="LOCAL_VARIABLE" override="true" value="" />
		<R base_type="REPEAT" />
		<EPARAM base_type="PARAM" min="0" max="1" default="0" exposed="true" />
		<EPPARAM base_type="PARAM" min="0" max="1" default="0" exposed="true" persistent="true" />
	  <FPARAM base_type="FORMULA_PARAM" min="0" max="1" default="0" exposed="true" />
	  <ESTRING base_type="STRING" exposed="true" default="" />
	  <INC base_type="INCLUDE_ONCE" ignore_missing="true" />
	  <USE base_type="INCLUDE" ignore_missing="true" /> <!-- USE doesn't get stripped/flattened on export -->
	</DEFINE>

	<!-- define subdir names -->
	<VAR id="LM_DIR" value="$_DIR_$" />
	<VAR id="SKINS_DIR" value="$LM_DIR$../" />
	<VAR id="CONTROLS_SUBDIR" value="controls" />
	<VAR id="EDITORS_SUBDIR" value="editors" />
	<VAR id="EDITORS_USER_DATA_SUBDIR" value="user_data" />
	<VAR id="EDITORS_USER_DATA_DIR" value="$LM_DIR$$EDITORS_SUBDIR$/$EDITORS_USER_DATA_SUBDIR$/" />
	<VAR id="DECOR_SUBDIR" value="decorations" />
	<VAR id="LAYOUTS_SUBDIR" value="layouts" />
	<VAR id="SCRIPTS_SUBDIR" value="scripts" />
	<VAR id="THEMES_SUBDIR" value="themes" />
	<VAR id="FONTS_SUBDIR" value="fonts" />
	<VAR id="EXTENSIONS_SUBDIR" value="extensions" />
	<VAR id="EXTENSIONS_DIR" value="$LM_DIR$$EXTENSIONS_SUBDIR$/" />
	<VAR id="KUIML_DIR" value="$SKINS_DIR$" /> <!-- may be useful for exported plugins -->
	

	<VAR id="LM_CORE_VER" value="1.191" />
	<VAR id="LM_CORE_BUILD" value="201024" />

	<!-- check if plugin builddate is set or not -->
	<VAR id="PLUGIN_BUILDDATE" script="if (&quot;$PLUGIN_BUILDDATE$&quot; == &quot;$&quot;+&quot;PLUGIN_BUILDDATE$&quot;) return &quot;000000&quot;; else return &quot;$PLUGIN_BUILDDATE$&quot;;" />
	
	<!-- define cursors to be able to override them later for unsupported DAWs/OS like Reaper -->
	<VAR id="CUR_HAND" value="system::hand" />
	<VAR id="CUR_CROSS" value="system::cross" />
	<VAR id="CUR_OPEN_HAND" value="system::open_hand" />
	<VAR id="CUR_SIZE" value="system::size" />
	<VAR id="CUR_SIZE_V" value="system::size_v" />
	<VAR id="CUR_SIZE_H" value="system::size_h" />
	<VAR id="CUR_SIZE_UP_LEFT" value="system::size_up_left" />
	<VAR id="CUR_IBEAM" value="system::Ibeam" />

	<!-- more defines -->
	<DEFINE>
		<LS base_type="CELL" layout_type="layer_stack" />
		<ACT base_type="ACTION" type="Script" />

	  <IAB base_type="INVISIBLE_ACTION_BUTTON" width="100%" height="100%" cursor="$CUR_HAND$" />
	  <IPB base_type="INVISIBLE_PARAM_BUTTON" positions_count="2" width="100%" height="100%" cursor="$CUR_HAND$" />
	  <IPMB base_type="INVISIBLE_PARAM_MENU_BUTTON" width="100%" height="100%" cursor="$CUR_HAND$" reverse_mouse_wheel="true" />
	  <PTC base_type="PARAM_TEXT_CONTROL" cursor="$CUR_SIZE_V$"/>

	  <ONLOAD base_type="ACTION_TRIGGER" event_id="window.loaded.value_changed" condition_formula="window.loaded==1" />
	  <ONCLOSE base_type="ACTION_TRIGGER" event_id="window.loaded.value_changed" condition_formula="window.loaded==0" />
	  <ONALIVE base_type="ACTION_TRIGGER" event_id="alive.value_changed" condition_formula="alive==1" />
	  <ONDEAD base_type="ACTION_TRIGGER" event_id="alive.value_changed" condition_formula="alive==0" />
	  <TT base_type="TOOLTIP" width="100%" height="100%" />
	  <W base_type="WIDGET" />
	  <TEB base_type="TEXT_EDIT_BOX" background_color="#FFFFFF" />
	  
	  <IS_LM_SKIN base_type="VARIABLE" id="IS_LM_SKIN" value="1" override="true" />

	</DEFINE>
	
	<!-- expose data model params -->
	<REQUIRED_OBJECTS object_ids="model.*;shared.*;global.*" />

	<!-- include common scriptbase and update canvas number counter -->
	<TEMPLATE id="COMMON_SCRIPTS" n='0' p='' include_lmr_scripts="0" include_common_renders="1">
	  <!-- update LMID (used to generate unique IDs for CANVAS-based and other elements) -->
	  <VAR id='LMID' formula='($n$==0)*($LMID$)+($n$>0)*($n$)' />
	  <VAR id='LMPR' value='$p$' /> <!-- usually used as prefix in conjunction with LMID -->
	  <!-- create dummy CANVAS to prevent stripping Kt::Graphics::GetCurrentContext -->
	  <CELL display="false"><CANVAS opacity="0" width="0" height="0" render_script="auto ctx=Kt::Graphics::GetCurrentContext();" /></CELL>
	  <VAR id="PI" formula="pi" />
	  <VAR id="UNKNOWN_VALUE" formula="-99999999" />

	  <VAR id="SCRIPTS_DIR" value="$LM_DIR$$SCRIPTS_SUBDIR$/" />
	  
	  <SCRIPT requires="" preprocess="true" src="$SCRIPTS_DIR$common.as" />
	  
	  <!-- different stuff for Windows and Mac -->
	  <IF_WINDOWS>
	  	<SCRIPT script="bool IS_WINDOWS = true; bool IS_MAC = false; " />
	  </IF_WINDOWS>

	  <IF_MAC>
	  	<SCRIPT script="bool IS_WINDOWS = false; bool IS_MAC = true; " />
	  </IF_MAC>

		<!-- additional scripting possibilities -->
	  <SCRIPT script="
	  	/* add line to kuiml log */
	  	void log(string s){
	  	  kuiml_log = &quot;&quot; + kuiml_log + s+ &quot;\n&quot;;
	  	}
	  	/* status set  */
	  	void status(string text){  
	  		status_custom_text = text; 
	  	} 
	  	/* status get */
	  	string status(){ 
	  		return &quot;&quot; + status_custom_text; 
	  	} 
			/* animated status fade-in/fade-out messages */
			void status(string text, float seconds_to_show){
				st_anim_timer_passed = 0;
				st_anim_timer_end = seconds_to_show*1000; /* from seconds to milliseconds */
				if (st_anim_timer.enabled &lt; 1) {
					if (status_custom_text_prev == &quot;&quot;) { 
						status_custom_text_prev = status(); 
					}
					st_anim_timer.enabled = true;
				}
				a_status_custom_text_opacity = 0;
				status(text);
			}
	  	" requires="st_anim_timer.enabled;st_anim_timer_passed;st_anim_timer_end;a_status_custom_text_opacity;status_custom_text;status_custom_text_prev;kuiml_log" />
		
	  <!-- include LMR scripts -->
	  <VAR id="SCRIPTS_LMR_DIR" value="$LM_DIR$$CONTROLS_SUBDIR$/LMR_scripts/" />
		
		<R count="(($include_lmr_scripts$==1) + ($include_common_renders$==1))>0">
			<SCRIPT requires="" preprocess="true" src="$SCRIPTS_DIR$common_renders.as" />
			<EXPOSED_OBJECTS object_ids="render_settings_changed.*;render_settings_data.*" />
			<ACTION_TRIGGER event_id="render_settings_data.value_changed" script="LM::RenderSettingsParser.parseString(render_settings_data); render_settings_changed++; " requires="render_settings_data.value_changed;render_settings_changed" />
		</R>

		<!-- include render objects -->
		<R count="($include_lmr_scripts$==1)">
			<SCRIPT src="$SCRIPTS_LMR_DIR$CanvasObject.as" preprocess="true" />
			<SCRIPT src="$SCRIPTS_LMR_DIR$CanvasWidget.as" preprocess="true" />
			<SCRIPT src="$SCRIPTS_LMR_DIR$Knob.as" preprocess="true" />
			<SCRIPT src="$SCRIPTS_LMR_DIR$Slider.as" preprocess="true" />
			<SCRIPT src="$SCRIPTS_LMR_DIR$Joystick.as" preprocess="true" />
			<SCRIPT src="$SCRIPTS_LMR_DIR$Meter.as" preprocess="true" />
		</R>
		
	</TEMPLATE>
	
	<!-- used for generating unique IDs for CANVASes and other elements -->
	<VAR id="ID_OR" value="if (&quot;$id$&quot; != &quot;$&quot;+&quot;id$&quot;) return &quot;$id$&quot;; else return" /> <!-- piece of common inline script -->
	<VAR id="LMID" formula="0" /> <!-- keep here number to generate unique IDs -->
	<TEMPLATE id="NEXT_LMID"><VAR id="LMID" formula="$LMID$+1" /></TEMPLATE>
	<VAR id="LMPR" value="" /> <!-- prefix to use with LMID -->
	<!-- often used vars -->


	<!--  COMMON TEMPLATES -->

	<!-- build time IF -->
	<TEMPLATE id="IF" condition="0">
	  <REPEAT count="($condition$)!=0" index_name="_if_index_">
	    <TEMPLATE_INNER_CONTENT/>
	  </REPEAT>
	</TEMPLATE>

	<!-- simple next N (increase N) -->
	<TEMPLATE id="NEXT_N">
	  <VAR id="N" formula="$N$+1" />
	</TEMPLATE>

	<!-- system type (Windows, Mac) detection tags -->
	<!-- first convert _SYSTEM_TYPE_ to integer to use in IF formulas -->
	<TEMPLATE id="SYSTEM_DETECTION">
	  <VAR id="SYS_Windows" value="1" />
	  <VAR id="SYS_Mac" value="2" />
	  <VAR id="SYS_NO" value="$SYS_$_SYSTEM_TYPE_$$" />
	</TEMPLATE>
	<SYSTEM_DETECTION />
	<TEMPLATE id="IF_WINDOWS">
	  <IF condition="$SYS_NO$ == $SYS_Windows$"><TEMPLATE_INNER_CONTENT /></IF>
	</TEMPLATE>
	<TEMPLATE id="IF_MAC">
	  <IF condition="$SYS_NO$ == $SYS_Mac$"><TEMPLATE_INNER_CONTENT /></IF>
	</TEMPLATE>

	<VAR id="DISABLE_CURSORS" value="0" />

	<!-- depending on system type -->
	<IF_WINDOWS>
	  <VAR id="DEFAULT_EDITOR" value="notepad.exe"/>
	  <VAR id="FILE_EXPLORER_APP_NAME" value="Explorer"/>
	  <VAR id="FILE_EXPLORER_APP" value="explorer.exe"/>
	  <VAR id="DEFAULT_FONT" value="Microsoft Sans Serif"/>
	  <VAR id="DEFAULT_MONOSPACE_FONT" value="Consolas"/>
	  <VAR id="SYS_NO" value="1" />
	  <!-- host app detection for WINDOW is disabled by default, cause there's no need for it yet and it slows skin loading -->
	  <VAR id="HOST_APP" value="" _script="
			string sysout; 
			string command = &quot;echo off&amp;TITLE lm_host_detect &amp; FOR /F \&quot;tokens=* USEBACKQ\&quot; %F IN (`tasklist /v /fo csv ^| findstr /i \&quot;lm_host_detect\&quot;`) DO ( FOR /F \&quot;tokens=2 delims=,\&quot; %G IN (\&quot;%F\&quot;) DO ( wmic process get processid,parentprocessid,executablepath|find /i %G )) &amp; &quot;;
			system(command, sysout);
			if (sysout.length>0) {
				int pos = sysout.findFirstOf(&quot;  &quot;);
				if (pos>1) {
					sysout=sysout.substr(pos);
					pos=sysout.findFirstNotOf(&quot; &quot;);
					sysout=sysout.substr(pos);
					string parent_pid = &quot;&quot;+parseInt(sysout);
					system(&quot;wmic process where processId=&quot;+parent_pid+&quot; get ExecutablePath&quot;, sysout);
					if (sysout.length > 0) {
						pos=sysout.findFirstOf(&quot;\n&quot;);
						sysout=sysout.substr(pos+1);
						for(uint i = sysout.length-1;i>0;i--) {
								if (sysout[i] &lt; 33) continue;
								return sysout.substr(0,i+1);
						}
					}
				}
			}
			return &quot;&quot;;
			" />
	</IF_WINDOWS>
	<IF_MAC>
	  <VAR id="DEFAULT_EDITOR" value="TextEdit"/>
	  <VAR id="FILE_EXPLORER_APP_NAME" value="Finder"/>
	  <VAR id="FILE_EXPLORER_APP" value="Finder"/>
	  <VAR id="DEFAULT_FONT" value="Lucida Grande"/>
	  <VAR id="DEFAULT_MONOSPACE_FONT" value="Courier"/>
	  <VAR id="SYS_NO" value="2" />
	  <VAR id="HOST_APP" value="" _script="
			string sysout;       
			system(&quot;ps aux -o ppid | grep $$&quot;, sysout); 
			if (sysout.length > 0) {
				string pid = sysout.substr((sysout.length)-7,10);
				int pos = pid.findFirstNotOf(&quot; &quot;);
				if (pos > 0) pid = &quot;&quot;+parseInt(pid.substr(pos));
				if (pid.length > 0) {
					system(&quot;lsof -p &quot;+pid+&quot; | grep txt | grep -m1 \&quot;\&quot;&quot;, sysout); 
					if (sysout.length > 0) {
						pos = sysout.findFirstOf(&quot;/&quot;);
						if (pos > -1) {
							sysout = sysout.substr(pos);
							for(uint i = sysout.length-1;i>0;i--) {
									if (sysout[i] &lt; 33) continue;
									return sysout.substr(0,i+1);
							}
						} else {
						  /* return &quot;no / found&quot;; */
						}
					}
				}
			}
			return &quot;---&quot;;
			" />
		<VAR id="IS_REAPER_ON_MAC" value="" _script="string hostapp = &quot;$HOST_APP$&quot;; if ((hostapp.findFirst(&quot;REAPER&quot;)>-1) || (hostapp.findFirst(&quot;Reaper&quot;)>-1)) return &quot;1&quot;; else return &quot;0&quot;;" />

		<VAR id="DISABLE_CURSORS" value="0" _script="string sysout; system(&quot;echo $&quot;+&quot;LM_SKIN_DISABLE_CURSORS&quot;, sysout); if (sysout.findFirst(&quot;1&quot;) >-1) return &quot;1&quot;; else return &quot;0&quot;; " />
			
	</IF_MAC>
	
	<!-- include additional flags -->

	<INC file="$PLUGIN_USER_DOCUMENTS_PATH$lm_common_flags.inc" />

	<!-- disable cursors if needed -->
	<R count="($DISABLE_CURSORS$ > 0)" index_name="_tmprom">
		<VAR id="CUR_HAND" value="system::normal" />
		<VAR id="CUR_CROSS" value="system::normal" />
		<VAR id="CUR_OPEN_HAND" value="system::normal" />
		<VAR id="CUR_SIZE" value="system::normal" />
		<VAR id="CUR_SIZE_V" value="system::normal" />
		<VAR id="CUR_SIZE_H" value="system::normal" />
		<VAR id="CUR_SIZE_UP_LEFT" value="system::normal" />
		<VAR id="CUR_IBEAM" value="system::normal" />
		<!-- and redefine some elements without cursor changes -->
		<UNDEFINE>
			<IAB /><IPB /><IPMB />
		</UNDEFINE>
		<DEFINE>
			<IAB base_type="INVISIBLE_ACTION_BUTTON" width="100%" height="100%" />
		  <IPB base_type="INVISIBLE_PARAM_BUTTON" positions_count="2" width="100%" height="100%"  />
		  <IPMB base_type="INVISIBLE_PARAM_MENU_BUTTON" width="100%" height="100%" reverse_mouse_wheel="true" />
		</DEFINE>
	</R>
	
	
	<!-- determining if we're in Plug-n-Script or generic Blue Cat Plugin -->
	<VAR id="IS_PNS" value="false" />
	<IF condition="($PLUGIN_OUTPUT_STRINGS_COUNT$ > 20)*($PLUGIN_INPUT_STRINGS_COUNT$ > 16)*($PLUGIN_INPUT_CUSTOM_PROPERTIES_COUNT$ == 0)*($PLUGIN_OUTPUT_CURVES_COUNT$ == 0)*($PLUGIN_OUTPUT_SURFACES_COUNT$ == 0)">
	  <VAR id="IS_PNS" value="true" />
	</IF>

	<!-- quick templates to isolate portions of code depending on state -->
	<TEMPLATE id="IF_PNS">
	  <REPEAT count="($IS_PNS$ == true)" index_name="_if_pns_"><TEMPLATE_INNER_CONTENT /></REPEAT>
	</TEMPLATE>
	<TEMPLATE id="IF_NOT_PNS">
	  <REPEAT count="($IS_PNS$ == false)" index_name="_if_notpns_"><TEMPLATE_INNER_CONTENT /></REPEAT>
	</TEMPLATE>
	<VAR id="IS_EXPORTED" value="true" />
	<IF_PNS>
	  <REPEAT count="($SCRIPT_EDIT_MODE$ == true)">
	    <VAR id="IS_EXPORTED" value="false" />
	  </REPEAT>
	</IF_PNS>
	<TEMPLATE id="IF_EXPORTED">
	  <REPEAT count="($IS_EXPORTED$ == true)" index_name="_if_expted_"><TEMPLATE_INNER_CONTENT /></REPEAT>
	</TEMPLATE>
	<TEMPLATE id="IF_NOT_EXPORTED">
	  <REPEAT count="($IS_EXPORTED$ == false)" index_name="_if_not_expted_"><TEMPLATE_INNER_CONTENT /></REPEAT>
	</TEMPLATE>

	<!-- if we're in some generic Blue Cat Plugin, generate default PNS vars -->
	<IF_NOT_PNS>
	  <VAR id="SCRIPT_EDIT_MODE" value="0" />
	  <VAR id="SCRIPT_PARAMS_COUNT" value="0" />
	  <VAR id="SCRIPT_OUT_PARAMS_COUNT" value="0" />
	  <VAR id="SCRIPT_STRINGS_COUNT" value="0" />
	</IF_NOT_PNS>

	<!-- strings holding text editor files -->
	<STRING id="edit_script_app" default="$DEFAULT_EDITOR$" exposed="true" />
	<STRING id="edit_kuiml_app" default="$DEFAULT_EDITOR$" exposed="true" />
	<STRING id="open_log_app" default="$DEFAULT_EDITOR$" exposed="true" />
	<REQUIRED_OBJECTS object_ids="edit_script_app;edit_kuiml_app;open_log_app" />

	<!-- when in Plug-N-Script trying to catch selected editors -->
	<IF condition="$SCRIPT_EDIT_MODE$">
	  <STRING_LINK from="global.edit_script_application" to="edit_script_app" enabled="true" />
	  <STRING_LINK from="global.edit_kuiml_application" to="edit_kuiml_app" enabled="true" />
	  <STRING_LINK from="global.open_log_application" to="open_log_app" enabled="true" />
	</IF>
	
	<!-- ================== -->
	<!-- ADDITIONAL HELPERS -->
	<!-- ================== -->
	
	<!-- shorter action trigger on value change -->
	<TEMPLATE id="ONCHANGE" param_id="_" obj="" param_ids="" o=""
		script="int d=1;" action="" action_id=""
		condition="1" condition_formula="" if="" when=""
		async="false" requires="">
		<!-- get objects -->
		<LV id="objs_" value="$param_id$" />
		<R index_list="$param_ids$"><LV id="objs_" value="$param_ids$" /></R>
		<R index_list="$obj$"><LV id="objs_" value="$obj$" /></R>
		<R index_list="$o$"><LV id="objs_" value="$o$" /></R>
		<!-- get condition -->
		<LV id="_condition" value="$condition$" />
		<R index_list="$condition_formula$"><LV id="_condition" value="$condition_formula$" /></R>
		<R index_list="$if$"><LV id="_condition" value="$if$" /></R>
		<R index_list="$when$"><LV id="_condition" value="$when$" /></R>
		<LV id="_act_id" value="" /><LV id="IS_ACTION" value="0" />
		<R index_list="$action$"><LV id="_act_id" value="$action$" /></R>
		<R index_list="$action_id$"><LV id="_act_id" value="$action_id$" /></R>
		<R index_list="$_act_id$"><LV id="IS_ACTION" value="1" /></R>
		<!-- output element -->
		<R count="($IS_ACTION$==0)" index_name="_tmpnisa">
			<R index_list="$objs_$">
			  <ACTION_TRIGGER event_id="$index$.value_changed" requires="$o$.*" script="$script$" condition_formula="$_condition$" async="$async$" />
			</R>
		</R>
		<R count="($IS_ACTION$)" index_name="_tmpisa">
			<R index_list="$objs_$">
			  <ACTION_TRIGGER event_id="$index$.value_changed" requires="$o$.*" action_id="$_act_id$" condition_formula="$_condition$" async="$async$" />
			</R>
		</R>

		<R index_list="$requires$">
			<REQUIRED_OBJECTS object_ids="$requires$" />
			<EXPOSED_OBJECTS object_ids="$requires$" />
		</R>
	</TEMPLATE>

	<TEMPLATE id="ONCLICK" o="" 
		script="" left_script="" right_script="" left_down_script="" right_down_script="" left_up_script="" right_up_script="" any_up_script="" any_down_script=""
		action="" action_id="" 
		url="" title="" title_string_id=""
		requires="" cursor="$CUR_HAND$">
		
		<!-- generate widget id or take given -->
		<LV id="WID" script="if (&quot;$id$&quot; == &quot;$&quot;+&quot;id$&quot;) return &quot;onclick_$LMPR$$LMID$&quot;; else return &quot;$id$&quot;;" />
		<NEXT_LMID />

		<!-- We can have several scenarios for onclick:
		1) Create a clickable element (INVISIBLE_ACTION_BUTTON) in place of ONCLICK (if no "o" is set)
		2) Add handler to trace clicks on another object, if "o" (object) is set.
		 -->
		
		<!-- if we have an "o" (objects) set, we set handlers to watch that another object -->
		<LV id="_on_another_object" value="0" />
		<LV id="_objects" value="" />
		<R index_list="$o$">
			<LV id="_on_another_object" value="1" />
			<LV id="_objects" value="$o$" />
		</R>
		
		<!-- prepare scripts for left clicks -->
		<LV id="_left_down_script" value="$left_down_script$" />
		<LV id="_left_up_script" value="$script$" />
		<R index_list="$left_script$"><LV id="_left_up_script" value="$left_script$" /></R>
		<R index_list="$left_up_script$"><LV id="_left_up_script" value="$left_up_script$" /></R>

		<!-- prepare scripts for right clicks -->
		<LV id="_right_down_script" value="$right_down_script$" />
		<LV id="_right_up_script" value="$right_script$" />
		<R index_list="$right_up_script$"><LV id="_right_up_script" value="$right_up_script$" /></R>
		
		<!-- scripts for both left or right buttons -->
		<R index_list="$any_down_script$">
			<LV id="_left_down_script" value="$any_down_script$" />
			<LV id="_right_down_script" value="$any_down_script$" />
		</R>
		<R index_list="$any_up_script$">
			<LV id="_left_up_script" value="$any_up_script$" />
			<LV id="_right_up_script" value="$any_up_script$" />
		</R>

		<!-- generate action id, or take given -->
		<LV id="ACTID" value="action_$WID$" />\
		<R index_list="$action$"><LV id="ACTID" value="$action$" /></R>
		<R index_list="$action_id$"><LV id="ACTID" value="$action_id$" /></R>
		
		<!-- if open url, create an action for that -->
    <LV id="IS_OPEN_URL" value="0" />
    <R index_list="$url$"><LV id="IS_OPEN_URL" value="1" /></R>
		<R count="$IS_OPEN_URL$">
			<ACTION id="$ACTID$" type="OpenUrl" name="$title$" url="$url$" />
		</R>
		
		<!-- if simple left click script is set, create action for that -->
    <LV id="IS_SCRIPT" value="0" />
    <R index_list="$_left_up_script$"><LV id="IS_SCRIPT" value="1" /></R>
		<R count="$IS_SCRIPT$">
			<ACTION id="$ACTID$" type="Script" name="$title$" script="$_left_up_script$" />
		</R>

		<!-- Scenario 1: if no objects given: create element for clicking -->
    <R count="($_on_another_object$ == 0)">
    	
    	<LV id="SIMPLE_IAB" value="1" />
    	<R index_list="$_left_down_script$$_right_down_script$$_right_up_script$" index_name="_tmpoc1"><LV id="SIMPLE_IAB" value="0" /></R>
    	
    	<!-- 
    		if no left down or right scripts given
    		we can create simple invisible action button
    	-->
    	<R count="$SIMPLE_IAB$ > 0">
    		<IAB id="$WID$" action_id="$ACTID$" cursor="$cursor$"><R index_list="$title$" index_name="_tmpoctt1"><TT text="$title$" /></R><R index_list="$title_string_id$" index_name="_tmpoctt2"><TT string_id="$title_string_id$" /></R></IAB>
    	</R>
			
			<!-- otherwise we create widget for tracking mouse events -->
    	<R count="$SIMPLE_IAB$ == 0">
    		<WIDGET id="$WID$" cursor="$cursor$" mouse_sensitive="true" width="100%" height="100%"><R index_list="$title$" index_name="_tmpoctt1"><TT text="$title$" /></R><R index_list="$title_string_id$" index_name="_tmpoctt2"><TT string_id="$title_string_id$" /></R></WIDGET>
    		<!-- add handlers like for scenario 2 -->
    		<LV id="_objects" value="$WID$" />
    	</R>

    </R>
    
		<!-- Scenario 2: if objects given: create handlers to watch mouse_events -->
    <R index_list="$_objects$">
			<!-- var to remember mouse button -->
			<SCRIPT script="int $WID$_$index$_mouse_down_before = -1;" />

    	<!-- on mouse down event -->
			<ONCHANGE when="$index$.mouse_down > 0" o="$index$.mouse_down" script="
				int m = rint($index$.mouse_down);
				$WID$_$index$_mouse_down_before = m;
				if (m == 1) { $_left_down_script$; }
				else if (m == 2) { $_right_down_script$; }
    		" />

    	<!-- on mouse up event -->
    	<ONCHANGE when="$index$.mouse_down == 0" o="$index$.mouse_down" script="
				int m = $WID$_$index$_mouse_down_before;
				if (m == 1) { $_left_up_script$; }
				else if (m == 2) { $_right_up_script$; }
				else { $_left_up_script$; $_right_up_script$;  }
    		" />
    </R>

    <R index_list="$requires$">
    	<REQUIRED_OBJECTS object_ids="$requires$" />
    	<EXPOSED_OBJECTS object_ids="$requires$" />
    </R>
  </TEMPLATE>

	<!-- a quick alias for right click only.
	If you need both left and right handlers, use ONCLICK directly -->
	<TEMPLATE id="ONRCLICK" o="" script="">
		<ONCLICK o="$o$" right_up_script="$script$" />
	</TEMPLATE>

	<!-- quickly add toggle param items to popup menu -->
	<TEMPLATE id="MENU_ITEM_TOGGLE" param_id="" text="" enabled_when="" when="">
		<ACT name="$text$" id="act_toggle_$param_id$" script="$param_id$ = abs($param_id$ - 1);" requires="$param_id$" />
		<MENU_ITEM action_id="act_toggle_$param_id$" checked_param_id="$param_id$" />
		<R index_list="$enabled_when$$when$">
			<FPARAM id="fp_act_toggle_$param_id$" formula="$enabled_when$" />
			<PARAM_LINK from="fp_act_toggle_$param_id$" to="act_toggle_$param_id$.enabled" />
		</R>
	</TEMPLATE>

	<!-- link that can be easily enabled or disabled by formula (`when`)-->
	<TEMPLATE id="LINK" from="" to="" response_curve="linear" formula="x" reverse="false" normalized="false" enabled="true" mode="absolute" when="" enabled_when="">
	  <LV id="LINK_ID" value="link_$LMPR$_$$LMID$" /><NEXT_LMID />
	  <PARAM_LINK id="$LINK_ID$" from="$from$" to="$to$" response_curve="$response_curve$" formula="$formula$" reverse="$reverse$" normalized="$normalized$" enabled="$enabled$" mode="$mode$" /> 
	  <!-- enabled_when -->
	  <R index_list="$when$$enabled_when$">
	    <FORMULA_PARAM id="fp_link_$LMID$" formula="$index$" /> 
	    <PARAM_LINK from="fp_link_$LMID$" to="$LINK_ID$.enabled" />
	    <NEXT_LMID />
	  </R>
	</TEMPLATE>

	<TEMPLATE id="SLINK" from="" to="" enabled="true" when="" enabled_when="">
	  <LV id="LINK_ID" value="link_$LMPR$_$LMID$" /><NEXT_LMID />
	  <STRING_LINK id="$LINK_ID$" from="$from$" to="$to$" enabled="$enabled$" /> 
	  <!-- enabled_when -->
	  <R index_list="$when$$enabled_when$">
	    <FORMULA_PARAM id="fp_link_$LMID$" formula="$index$" /> 
	    <PARAM_LINK from="fp_link_$LMID$" to="$LINK_ID$.enabled" />
	    <NEXT_LMID />
	  </R>
	</TEMPLATE>

	<TEMPLATE id="DISPLAY" o="" when="">
  	<LV id="FP_ID" value="fp_$o$_display_$LMPR$_$LMID$" /><NEXT_LMID />
  	<FORMULA_PARAM id="$FP_ID$" formula="$when$" />
  	<R index_list="$o$"><PARAM_LINK from="$FP_ID$" to="$index$.display" /></R>
  </TEMPLATE>
	

	<!-- ###################################### -->
	<!-- MAPPINGS (rewritten from legacy mappings.inc) -->
	<!-- ###################################### -->
	
	<EPARAM id="generic_param" /> <!-- to use in case of missing parameters -->
	<STRING id="str_empty" default="" />
	<EXPOSED_OBJECTS object_ids="generic_param.*" />
	<IF_PNS>
	  <!-- extra input params exist when plugin is not exported -->
	  <VAR id="HAS_EXTRA_INPUT_PARAMS" formula="$SCRIPT_PARAMS_COUNT$!=($PLUGIN_INPUT_PARAMS_COUNT$-1)"/>

	  <!-- map status bar params -->
	  <VAR id="script_status" value="dsp.private_output0" />
	  <VAR id="script_status_string" value="dsp.output_string0" />

	  <!-- map input parameters -->
	  <VAR id="bypass" value="dsp.input0" />
	  <VAR id="N" value="1" />
	  <VAR id="reload_script" value="generic_param" />
	  <IF condition="$HAS_EXTRA_INPUT_PARAMS$">
	    <VAR id="reload_script" value="dsp.input1" />
	    <VAR id="show_levels" value="dsp.input2" />
	    <REQUIRED_OBJECTS object_ids="$show_levels$" />
	    <PARAM_LINK from="a_meters_display" to="$show_levels$" />
	  	<VAR id="N" value="8" /> <!-- start from 7, when extra params exist -->
	  </IF>
	  <REPEAT count="$SCRIPT_PARAMS_COUNT$">
	    <VAR id="script_input$index$" value="dsp.input$N$" /><NEXT_N />
	  </REPEAT>

	  <!-- map input strings -->
	  <STRING id="script_file_selection" value="dsp.input_string0" />
	  <VAR id="N" value="1" />
	  <REPEAT count="$SCRIPT_STRINGS_COUNT$">
	  	<VAR id="script_input_string$index$" value="dsp.input_string$N$" />
	  	<VAR id="custom_string$index$" value="dsp.input_string$N$"/>
	  	<NEXT_N />
	  </REPEAT>

	  <!-- map private output params -->
	  <VAR id="script_status" value="dsp.private_output0" />
	  <VAR id="script_input_params_count" value="dsp.private_output1" />
	  <VAR id="script_output_params_count" value="dsp.private_output2" />
	  <VAR id="N" value="3" />
	  <REPEAT count="$SCRIPT_PARAMS_COUNT$" index_name="i">
	      <VAR id="script_input_param$i$_min" value="dsp.private_output$N$" /><NEXT_N />
	      <VAR id="script_input_param$i$_max" value="dsp.private_output$N$" /><NEXT_N />
	      <VAR id="script_input_param$i$_default" value="dsp.private_output$N$" /><NEXT_N />
	      <REQUIRED_OBJECTS object_ids="dsp.private_output$N$" /> <!-- to not strip steps -->
	      <VAR id="script_input_param$i$_steps" value="dsp.private_output$N$" /><NEXT_N />
	  </REPEAT>
	  <REPEAT count="$SCRIPT_OUT_PARAMS_COUNT$">
	      <VAR id="script_output_param$index$_min" value="dsp.private_output$N$" /><NEXT_N />
	      <VAR id="script_output_param$index$_max" value="dsp.private_output$N$" /><NEXT_N />
	      <VAR id="script_output_param$index$_default" value="dsp.private_output$N$" /><NEXT_N />
	  </REPEAT>
	  <REPEAT count="$PLUGIN_AUDIO_INPUTS_COUNT$" index_name="CH" start="1">
	      <VAR id="input_level$CH$" value="dsp.private_output$N$" /><NEXT_N />
	      <VAR id="input_hold_level$CH$" value="dsp.private_output$N$" /><NEXT_N />
	  </REPEAT>
	  <REPEAT count="$PLUGIN_AUDIO_OUTPUTS_COUNT$" index_name="CH" start="1">
	      <VAR id="output_level$CH$" value="dsp.private_output$N$" /><NEXT_N />
	      <VAR id="output_hold_level$CH$" value="dsp.private_output$N$" /><NEXT_N />
	  </REPEAT>
	  <VAR id="script_input_strings_count" value="dsp.private_output$N$" /><NEXT_N />
	  <VAR id="script_output_strings_count" value="dsp.private_output$N$" /><NEXT_N />
	  <VAR id="script_file_type" value="dsp.private_output$N$" /><NEXT_N />

	  <!-- map output params -->
	  <VAR id="N" value="0" />
	  <REPEAT count="$SCRIPT_OUT_PARAMS_COUNT$">
	      <VAR id="script_output$index$" value="dsp.output$N$" /><NEXT_N />
	  </REPEAT>

	  <!-- map output strings -->
	  <VAR id="script_status_string" value="dsp.output_string0" />
	  <VAR id="script_name_string" value="dsp.output_string1" />
	  <VAR id="script_description_string" value="dsp.output_string2" />
	  <VAR id="script_author_string" value="dsp.output_string3" />
	  <VAR id="script_file_path" value="dsp.output_string4" />
	  <VAR id="log_file_path" value="dsp.output_string5" />
	  <VAR id="script_doc_path" value="dsp.output_string6" />
	  <VAR id="script_gui_path" value="dsp.output_string7" />
	  <VAR id="N" value="8" />
	  <REPEAT count="$SCRIPT_PARAMS_COUNT$"> <!-- 48 IN VERSION 3.2 -->
      <VAR id="script_input_param$index$_name" value="dsp.output_string$N$"/><NEXT_N />
      <VAR id="script_input_param$index$_unit" value="dsp.output_string$N$"/><NEXT_N />
      <VAR id="script_input_param$index$_enum_values" value="dsp.output_string$N$"/><NEXT_N />
      <VAR id="script_input_param$index$_value_format" value="dsp.output_string$N$"/><NEXT_N />
	  </REPEAT>
	  <REPEAT count="$SCRIPT_OUT_PARAMS_COUNT$"> <!-- 32 IN VERSION 3.2 -->
      <VAR id="script_output_param$index$_name" value="dsp.output_string$N$"/><NEXT_N />
      <VAR id="script_output_param$index$_unit" value="dsp.output_string$N$"/><NEXT_N />
      <VAR id="script_output_param$index$_enum_values" value="dsp.output_string$N$"/><NEXT_N />
      <VAR id="script_output_param$index$_value_format" value="dsp.output_string$N$"/><NEXT_N />
	  </REPEAT>
	  <REPEAT count="$SCRIPT_STRINGS_COUNT$"> <!-- 16 IN VERSION 3.2 -->
	     <VAR id="script_input_string$index$_name" value="dsp.output_string$N$"/><NEXT_N />
	  </REPEAT>
		<REPEAT count="$SCRIPT_OUT_STRINGS_COUNT$"> <!-- 16 IN VERSION 3.2 -->
			<VAR id="script_output_string$index$_name" value="dsp.output_string$N$"/><NEXT_N />
		</REPEAT>
		<REPEAT count="$SCRIPT_OUT_STRINGS_COUNT$"> <!-- 16 IN VERSION 3.2 -->
			<VAR id="script_output_string$index$" value="dsp.output_string$N$"/>
			<VAR id="custom_out_string$index$" value="dsp.output_string$N$"/>
			<NEXT_N />
		</REPEAT>

	  <!-- Main audio bus channels-->
	  <FORMULA_PARAM id="main_bus_channels_count" formula="min(dsp.connected_inputs_count,dsp.connected_outputs_count)"/>

	  <!-- Custom parameters, built from param details in current script -->
	  <REPEAT count="$SCRIPT_PARAMS_COUNT$">
	    <!-- Input param for display and control, linked to actual DSP param-->
	    <PARAM id="custom_param$index$" min="0" max="1" default="0" name="Param $index$"/>

	    <PARAM_LINK from="$script_input_param$index$_min$" to="custom_param$index$.min"/>
	    <PARAM_LINK from="$script_input_param$index$_max$" to="custom_param$index$.max"/>
	    <PARAM_LINK from="$script_input_param$index$_default$" to="custom_param$index$.default"/>
	    <STRING_LINK from="$script_input_param$index$_unit$" to="custom_param$index$.unit"/>
	    <STRING_LINK from="$script_input_param$index$_name$" to="custom_param$index$.name"/>
	    <STRING_LINK from="$script_input_param$index$_value_format$" to="custom_param$index$.value_format"/>
	    <STRING_LINK from="$script_input_param$index$_enum_values$" to="custom_param$index$.enum_values"/>

	    <!-- additional steps link from letimix skin -->
	    <PARAM id="custom_param$index$_steps" min="0" max="$_PARAM_MAX_$" default="2" />
			<PARAM_LINK from="$script_input_param$index$_steps$" to="custom_param$index$_steps" />
			
			<PARAM_LINK from="$script_input$index$$" to="custom_param$index$" normalized="true" enabled="true" capture_enabled="false"/>
	    <!-- we enable back link from custom_paramN to dsp.inputM -->
	    <!-- in the default BC skin it's enabled only when capturing, but why? -->
	    <!-- <PARAM_LINK from="custom_param$index$" to="$script_input$index$$" enabled="true" normalized="true"/>
	    -->
			<PARAM_LINK from="custom_param$index$" to="$script_input$index$$" enabled="false" id="custom_param$index$_backlink" normalized="true"/>
	    <PARAM_LINK from="custom_param$index$.capturing" to="custom_param$index$_backlink.enabled"/>
	  	
	    <REQUIRED_OBJECTS object_ids="custom_param$index$.value_changed" />
	  </REPEAT>

	  <REPEAT count="$SCRIPT_OUT_PARAMS_COUNT$">
	    <!-- Output param for display, linked to actual DSP param-->
	    <PARAM id="custom_out_param$index$" min="0" max="1" default="0" name="Out Param $index$"/>
	    <PARAM_LINK from="$script_output_param$index$_min$" to="custom_out_param$index$.min"/>
	    <PARAM_LINK from="$script_output_param$index$_max$" to="custom_out_param$index$.max"/>
	    <PARAM_LINK from="$script_output_param$index$_default$" to="custom_out_param$index$.default"/>
	    <STRING_LINK from="$script_output_param$index$_unit$" to="custom_out_param$index$.unit"/>
	    <STRING_LINK from="$script_output_param$index$_name$" to="custom_out_param$index$.name"/>
	    <STRING_LINK from="$script_output_param$index$_value_format$" to="custom_out_param$index$.value_format"/>
	    <STRING_LINK from="$script_output_param$index$_enum_values$" to="custom_out_param$index$.enum_values"/>
	    <PARAM_LINK from="$script_output$index$$" to="custom_out_param$index$" normalized="true"/>
	    <REQUIRED_OBJECTS object_ids="custom_out_param$index$.value_changed" />
	  </REPEAT>

	  <!-- make sure DSP data is not stripped -->
	  <REQUIRED_OBJECTS object_ids="dsp;dsp.sample_rate;dsp.max_block_size;dsp.output_string0;dsp.private_output0" />
		
		<!-- make sure DSP data is not stripped -->
		<REPEAT count="$PLUGIN_INPUT_PARAMS_COUNT$">
		  <REQUIRED_OBJECTS object_ids="dsp.input$index$;dsp.input$index$.value_changed;Show_dsp.input$index$_ControlSetup"/>
		</REPEAT>
		<REPEAT count="$PLUGIN_INPUT_STRINGS_COUNT$">
		  <REQUIRED_OBJECTS object_ids="dsp.input_string$index$;dsp.input_string$index$.value_changed"/>
		</REPEAT>
		<REPEAT count="$PLUGIN_OUTPUT_PARAMS_COUNT$">
		  <REQUIRED_OBJECTS object_ids="dsp.output$index$;dsp.output$index$.value_changed;Show_dsp.output$index$_ControlSetup"/>
		</REPEAT>
		<REPEAT count="$PLUGIN_OUTPUT_STRINGS_COUNT$">
	    <REQUIRED_OBJECTS object_ids="dsp.output_string$index$;dsp.output_string$index$.value_changed"/>
	  </REPEAT>

	  <!-- editable mode: make sure DSP data is not stripped (not exposing all to improve performance)-->
	  <IF condition="$SCRIPT_EDIT_MODE$">
	    <REPEAT count="$PLUGIN_PRIVATE_OUTPUT_PARAMS_COUNT$">
	      <REQUIRED_OBJECTS object_ids="dsp.private_output$index$;dsp.private_output$index$.value_changed"/>
	    </REPEAT>
	  </IF>
	</IF_PNS>

	<!-- dummy params for generic plugin (not PNS) -->
	<IF_NOT_PNS>
	  <ESTRING id="generic_string" default="" />
	  <ESTRING id="plugin_name_string" default="$PLUGIN_NAME$" />
	  <VAR id="bypass" value="dsp.input0" />
	  <VAR id="reload_script" value="generic_param" />
	  <VAR id="script_status" value="generic_param" />
	  <VAR id="script_status_string" value="generic_string" />
	  <VAR id="script_name_string" value="plugin_name_string" />
	  <VAR id="script_description_string" value="generic_string" />
	  <VAR id="script_author_string" value="generic_string" />
	  <VAR id="script_file_path" value="generic_string" />
	  <VAR id="log_file_path" value="generic_string" />
	  <VAR id="script_doc_path" value="generic_string" />
	  <VAR id="script_gui_path" value="generic_string" />
	  <VAR id="script_input_params_count" value="generic_param" />
	  <VAR id="script_output_params_count" value="generic_param" />
	  <VAR id="script_input_strings_count" value="generic_param" />
	  <VAR id="script_output_strings_count" value="generic_param" />

	  <!-- make sure DSP data is not stripped -->
	  <REQUIRED_OBJECTS object_ids="dsp.sample_rate;dsp.max_block_size"/>
	</IF_NOT_PNS>
	  
	  <!-- for both PNS and Generic plugin -->
	  <VAR id="input_channels_count" value="$PLUGIN_AUDIO_INPUTS_COUNT$"/>
	  <VAR id="output_channels_count" value="$PLUGIN_AUDIO_OUTPUTS_COUNT$"/>
	  <!-- Opacity -->
	  <PARAM id="global_opacity" min="0" max="100" default="100" unit="%" persistent="true"/>
	  <PARAM_LINK from="global_opacity" to="window.opacity" normalized="true"/>
	  
	  <!-- for compliance with default skin -->
	  <PARAM id="theme.show_settings" type="boolean" default="0" locked="false" exposed="true" />

	  <!-- more useful params -->
	  <FORMULA_PARAM id="skin_loading" formula="alive&amp;&amp;(window.loaded==false)" type="boolean" default="false"/>
	  <!--
	  <FORMULA_PARAM id="gui_zoom_ok" formula="(gui.zoom-floor(gui.zoom)&lt;0.2)" type="boolean" default="true"/>
	-->

	  <STRING id="current_preset_name_with_asterisk" default="" />
	  <STRING id="asterisk" default=" *" />
	  <STRING id="noasterisk" default="" />
	  <STRING_MULTI_LINK id="link_current_preset_name_with_asterisk" from="current_preset.name;asterisk" to="current_preset_name_with_asterisk" />
	  <STRING_MULTI_LINK id="link_current_preset_name_with_noasterisk" from="current_preset.name;noasterisk" to="current_preset_name_with_asterisk" />
	  <PARAM_LINK from="current_preset.modified" to="link_current_preset_name_with_asterisk.enabled" formula="(x > 0.5)"/>
	  <PARAM_LINK from="current_preset.modified" to="link_current_preset_name_with_noasterisk.enabled" formula="(x &lt; 0.5)" />

	<!-- ###################################### -->
	<!-- END OF MAPPINGS  -->
	<!-- ###################################### -->


	











	<!-- ###################################### -->
	<!-- COMMON GRAPHICS AND TEMPLATES  -->
	<!-- ###################################### -->


	<!-- Simple SVG Background -->
	<TEMPLATE id="SVG_BG" color="#EEEEEE" opacity="1">
	  <svg width="100%" height="100%" viewBox='0 0 10 10' opacity="$opacity$">
	    <rect fill="$color$" width="100%" height="100%" />
	  </svg>
	</TEMPLATE>

	<!-- Custom Gradient-capable Background -->
	<TEMPLATE id="SVG_GRADIENT" type="1" opacity="1" color1="#EEEEEE" color2="#000000" opacity1="1" opacity2="1" pos1="0" pos2="100" shift="0">
	    <!-- vertical up-down -->
	    <LV id="x1" value="0" />
	    <LV id="y1" value="0" />
	    <LV id="y2" value="10" />
	    <LV id="x2" value="0" />
	    <LV id="gcolor1" value="$color1$" />
	    <LV id="gcolor2" value="$color2$" />
	    <LV id="gopacity1" value="$opacity1$" />
	    <LV id="gopacity2" value="$opacity2$" />
	    <LV id="s1" formula="$pos1$+$shift$" /> <!-- stop positions -->
	    <LV id="s2" formula="$pos2$+$shift$" />
	    <LV id="s3" value="100+$shift$" />
	    <LV id="s4" value="100+$shift$" />
	    <!-- flat (no gradient, fill with primary color) -->
	    <REPEAT count="($type$ == 0)" index_name="_i00_">
	      <LV id="gcolor2" value="$color1$" />
	      <!-- <LV id="gopacity1" value="1" /> ignore color1 opacity (use global) -->
	      <LV id="gopacity2" value="$gopacity1$" />
	      <LV id="s1" value="0" />
	      <LV id="s2" value="100" />
	    </REPEAT>
	    <!-- vertical up-down -->
	    <REPEAT count="($type$ == 1)" index_name="_i01_">
	      <LV id="x1" value="0" />
	      <LV id="y1" value="0" />
	      <LV id="y2" value="10" />
	      <LV id="x2" value="0" />
	    </REPEAT>
	    <!-- vertical down-up -->
	    <REPEAT count="($type$ == 2)" index_name="_i02_">
	      <LV id="x1" value="0" />
	      <LV id="y1" value="10" />
	      <LV id="y2" value="0" />
	      <LV id="x2" value="0" />
	    </REPEAT>
	    <!-- vertical edges-center -->
	    <REPEAT count="($type$ == 3)" index_name="_i03_">
	      <LV id="s2" formula="-50+$pos2$+$shift$" />
	      <LV id="s3" formula="150-$pos2$+$shift$" />
	      <LV id="s4" formula="100-$pos1$+$shift$" />
	    </REPEAT>
	    <!-- vertical center-edges -->
	    <REPEAT count="($type$ == 4)" index_name="_i04_">
	      <LV id="s1" formula="100-$pos2$+$shift$" />
	      <LV id="s2" formula="50-$pos1$+$shift$" />
	      <LV id="s3" formula="50+$pos1$+$shift$" />
	      <LV id="s4" formula="$pos2$+$shift$" />
	      <LV id="gcolor1" value="$color2$" />
	      <LV id="gcolor2" value="$color1$" />
	      <LV id="gopacity1" value="$opacity2$" />
	      <LV id="gopacity2" value="$opacity1$" />
	    </REPEAT>

	    <!-- horizontal left-right -->
	    <REPEAT count="($type$ == 5)" index_name="_i05_">
	      <LV id="x1" value="0" />
	      <LV id="y1" value="0" />
	      <LV id="y2" value="0" />
	      <LV id="x2" value="10" />
	    </REPEAT>
	    <!-- horizontal right-left -->
	    <REPEAT count="($type$ == 6)" index_name="_i06_">
	      <LV id="x1" value="10" />
	      <LV id="y1" value="0" />
	      <LV id="y2" value="0" />
	      <LV id="x2" value="0" />
	    </REPEAT>
	    <!-- horizontal edges-center -->
	    <REPEAT count="($type$ == 7)" index_name="_i07_">
	      <LV id="x1" value="0" />
	      <LV id="y1" value="0" />
	      <LV id="y2" value="0" />
	      <LV id="x2" value="10" />
	      <LV id="s2" formula="-50+$pos2$+$shift$" />
	      <LV id="s3" formula="150-$pos2$+$shift$" />
	      <LV id="s4" formula="100-$pos1$+$shift$" />
	    </REPEAT>
	    <!-- horizontal center-edges -->
	    <REPEAT count="($type$ == 8)" index_name="_i08_">
	      <LV id="x1" value="0" />
	      <LV id="y1" value="0" />
	      <LV id="y2" value="0" />
	      <LV id="x2" value="10" />
	      <LV id="gcolor1" value="$color2$" />
	      <LV id="gcolor2" value="$color1$" />
	      <LV id="gopacity1" value="$opacity2$" />
	      <LV id="gopacity2" value="$opacity1$" />
	      <LV id="s1" formula="100-$pos2$+$shift$" />
	      <LV id="s2" formula="50-$pos1$+$shift$" />
	      <LV id="s3" formula="50+$pos1$+$shift$" />
	      <LV id="s4" formula="$pos2$+$shift$" />
	    </REPEAT>

	    <!-- show gradient -->
	    <svg width="100%" height="100%" viewBox='0 0 10 10' opacity="$opacity$">
	      <linearGradient id="grad" x1="$x1$" x2="$x2$" y1="$y1$" y2="$y2$">
	        <stop offset="$s1$%" stop-color="$gcolor1$" stop-opacity="$gopacity1$" />
	        <stop offset="$s2$%" stop-color="$gcolor2$" stop-opacity="$gopacity2$" />
	        <stop offset="$s3$%" stop-color="$gcolor2$" stop-opacity="$gopacity2$" />
	        <stop offset="$s4$%" stop-color="$gcolor1$" stop-opacity="$gopacity1$" />
	      </linearGradient>
	      <rect fill="url(#grad)" width="100%" height="100%" />
	    </svg>
	</TEMPLATE>

	<!-- complex background with edges -->
	<TEMPLATE id="LM_BG" type="1" opacity="1" color1="#EEEEEE" color2="#000000" opacity1="1" opacity2="1" pos1="0" pos2="100" shift="0" edges_mode="0" edge1="10" edge2="20" color3="#00FF00" color4="#FF0000" opacity3="1" opacity4="1" pos3="0" pos4="100">
	  <!-- edge A gradient -->
	  <LV id="ga_type" value="1" />
	  <LV id="ga_col1" value="$color3$" />
	  <LV id="ga_col2" value="$color1$" />
	  <LV id="ga_op1" value="$opacity3$" />
	  <LV id="ga_op2" value="$opacity1$" />
	  <LV id="ga_pos1" value="$pos3$" />
	  <LV id="ga_pos2" value="100" />

	  <!-- edge B gradient -->
	  <LV id="gb_type" value="1" />
	  <LV id="gb_col1" value="$color2$" />
	  <LV id="gb_col2" value="$color4$" />
	  <LV id="gb_op1" value="$opacity2$" />
	  <LV id="gb_op2" value="$opacity4$" />
	  <LV id="gb_pos1" value="0" />
	  <LV id="gb_pos2" value="$pos4$" />

	  <!-- if no gradient -->
	  <REPEAT count="($type$ == 0)" index_name="_i10_">
	    <LV id="gb_col1" value="$color1$" />
	    <LV id="gb_op1" value="$opacity1$" />
	  </REPEAT>

	  <!-- if reversed colors -->
	  <REPEAT count="($type$ == 2)+($type$ == 6)" index_name="_i12_">
	    <LV id="ga_col2" value="$color2$" />
	    <LV id="ga_op2" value="$opacity2$" />
	    <LV id="gb_col1" value="$color1$" />
	    <LV id="gb_op1" value="$opacity1$" />
	  </REPEAT>

	  <!-- if colors to center -->
	  <REPEAT count="($type$ == 3)+($type$ == 7)" index_name="_i13_">
	    <LV id="gb_col1" value="$color1$" />
	    <LV id="gb_op1" value="$opacity1$" />
	  </REPEAT>

	  <!-- if colors from center -->
	  <REPEAT count="($type$ == 4)+($type$ == 8)" index_name="_i14_">
	    <LV id="ga_col2" value="$color2$" />
	    <LV id="ga_op2" value="$opacity2$" />
	  </REPEAT>

	  <!-- define are edges up-down or left-right -->
	  <LV id="edges_are_updown" formula="($type$ &lt; 5)*($edges_mode$ == 0) + ($edges_mode$ &lt; 9)*($edges_mode$ != 0)" />
	  <LV id="edges_are_leftright" formula="($type$ > 4)*($edges_mode$ == 0) + ($edges_mode$ > 8)*($edges_mode$ != 0)" />

	  <!-- if up-down edges -->
	  <REPEAT count="($edges_are_updown$ > 0)" index_name="_ilud_">
	    <!-- different edges types -->
	    <REPEAT count="($edges_mode$ == 1)" index_name="_et1_">
	      <LV id="ga_type" value="1" /> 
	      <LV id="gb_type" value="1" /> 
	    </REPEAT>
	    <REPEAT count="($edges_mode$ == 2)" index_name="_et2_">
	      <LV id="ga_type" value="5" /> 
	      <LV id="gb_type" value="5" /> 
	    </REPEAT>
	    <REPEAT count="($edges_mode$ == 3)" index_name="_et3_">
	      <LV id="ga_type" value="6" /> 
	      <LV id="gb_type" value="6" /> 
	    </REPEAT>
	    <REPEAT count="($edges_mode$ == 4)" index_name="_et4_">
	      <LV id="ga_type" value="5" /> 
	      <LV id="gb_type" value="6" /> 
	    </REPEAT>
	    <REPEAT count="($edges_mode$ == 5)" index_name="_et5_">
	      <LV id="ga_type" value="1" /> 
	      <LV id="gb_type" value="5" /> 
	    </REPEAT>
	    <REPEAT count="($edges_mode$ == 6)" index_name="_et6_">
	      <LV id="ga_type" value="5" /> 
	      <LV id="gb_type" value="1" /> 
	    </REPEAT>
	    <REPEAT count="($edges_mode$ == 7)" index_name="_et7_">
	      <LV id="ga_type" value="7" /> 
	      <LV id="gb_type" value="8" /> 
	    </REPEAT>
	    <REPEAT count="($edges_mode$ == 8)" index_name="_et8_">
	      <LV id="ga_type" value="8" /> 
	      <LV id="gb_type" value="7" /> 
	    </REPEAT>

	    <CELL width="100%" height="100%">
	      <CELL width="100%" height="$edge1$">
	        <SVG_GRADIENT type="$ga_type$" opacity="$opacity$" color1="$ga_col1$" color2="$ga_col2$" opacity1="$ga_op1$" opacity2="$ga_op2$" pos1="$ga_pos1$" pos2="$ga_pos2$" shift="0" />
	      </CELL>
	      <CELL width="100%" flex="1">
	        <SVG_GRADIENT type="$type$" opacity="$opacity$" color1="$color1$" color2="$color2$" opacity1="$opacity1$" opacity2="$opacity2$" pos1="$pos1$" pos2="$pos2$" shift="$shift$" />
	      </CELL>
	      <CELL width="100%" height="$edge2$">
	        <SVG_GRADIENT type="$gb_type$" opacity="$opacity$" color1="$gb_col1$" color2="$gb_col2$" opacity1="$gb_op1$" opacity2="$gb_op2$" pos1="$gb_pos1$" pos2="$gb_pos2$" shift="0" />
	      </CELL>
	    </CELL>
	  </REPEAT>

	  <!-- if left-right edges -->
	  <REPEAT count="($edges_are_leftright$ > 0)" index_name="_illr_">

	    <REPEAT count="($edges_mode$ &lt; 10)" index_name="_et09_">
	      <LV id="ga_type" value="5" /> 
	      <LV id="gb_type" value="5" /> 
	    </REPEAT>
	    <REPEAT count="($edges_mode$ == 10)" index_name="_et10_">
	      <LV id="ga_type" value="1" /> 
	      <LV id="gb_type" value="1" /> 
	    </REPEAT>
	    <REPEAT count="($edges_mode$ == 11)" index_name="_et11_">
	      <LV id="ga_type" value="1" /> 
	      <LV id="gb_type" value="2" /> 
	    </REPEAT>
	    <REPEAT count="($edges_mode$ == 12)" index_name="_et12_">
	      <LV id="ga_type" value="2" /> 
	      <LV id="gb_type" value="1" /> 
	    </REPEAT>
	    <REPEAT count="($edges_mode$ == 13)" index_name="_et13_">
	      <LV id="ga_type" value="2" /> 
	      <LV id="gb_type" value="2" /> 
	    </REPEAT>
	    <REPEAT count="($edges_mode$ == 14)" index_name="_et14_">
	      <LV id="ga_type" value="3" /> 
	      <LV id="gb_type" value="3" /> 
	    </REPEAT>
	    <REPEAT count="($edges_mode$ == 15)" index_name="_et15_">
	      <LV id="ga_type" value="4" /> 
	      <LV id="gb_type" value="4" /> 
	    </REPEAT>
	    <REPEAT count="($edges_mode$ == 16)" index_name="_et16_">
	      <LV id="ga_type" value="4" /> 
	      <LV id="gb_type" value="3" /> 
	    </REPEAT>

	    <ROW width="100%" height="100%">
	      <CELL width="$edge1$" height="100%">
	        <SVG_GRADIENT type="$ga_type$" opacity="$opacity$" color1="$ga_col1$" color2="$ga_col2$" opacity1="$ga_op1$" opacity2="$ga_op2$" pos1="$ga_pos1$" pos2="$ga_pos2$" shift="0" />
	      </CELL>
	      <CELL height="100%" flex="1">
	        <SVG_GRADIENT type="$type$" opacity="$opacity$" color1="$color1$" color2="$color2$" opacity1="$opacity1$" opacity2="$opacity2$" pos1="$pos1$" pos2="$pos2$" shift="$shift$" />
	      </CELL>
	      <CELL width="$edge2$" height="100%">
	        <SVG_GRADIENT type="$gb_type$" opacity="$opacity$" color1="$gb_col1$" color2="$gb_col2$" opacity1="$gb_op1$" opacity2="$gb_op2$" pos1="$gb_pos1$" pos2="$gb_pos2$" shift="0" />
	      </CELL>
	    </ROW>
	  </REPEAT>
	</TEMPLATE>

	<!-- show background with data comma-separated -->
	<TEMPLATE id="LM_BG_PARSER" data="">
	  <VAR id="N" value="0" />
	  <REPEAT index_list="$data$">
	    <VAR id="T$N$" value="$index$" /><NEXT_N />
	  </REPEAT>
	  <LM_BG type='$T0$' opacity='$T1$' shift='$T2$' edges_mode='$T3$' edge1='$T4$' edge2='$T5$' color1='$T10$' opacity1='$T11$' pos1='$T12$' color2='$T13$' opacity2='$T14$' pos2='$T15$' color3='$T16$' opacity3='$T17$' pos3='$T18$' color4='$T19$' opacity4='$T20$' pos4='$T21$' />
	</TEMPLATE>


	<!-- LM_BOX_RAW: An element capable of drawing gradient-filled backgrounds, stroked with rounded corners (similar to LM_BG, LM_BG_PARSER, but using CANVAS instead of svg)
	gradient_type;global_opacity;gradient_shift;stroke_width;stroke_expand;stroke_round;y_offset;;;;#bgcolor1;opacity1;pos1;#bgcolor2;opacity2;pos2;#strokecolor;opacity3;unused; -->
	<TEMPLATE id="LM_BOX_RAW" data="0;0.6;0;1.7;0;10;0;;;;#FFFFFF;1;0;#FFFFFF;1;100;#777777;0.5;0" expand_invert="0" width="100%" height="100%" opacity="1" min_width="10" min_height="10" visible="true">
		<LV id="LMBRWID" script="if (&quot;$id$&quot; == &quot;$&quot;+&quot;id$&quot;) return &quot;lmboxraw_$LMPR$$LMID$&quot;; else return &quot;$id$&quot;;" />
		<NEXT_LMID />
		<!-- <LV id="CNVID" value="cnvboxraw_$LMPR$$LMID$" /><NEXT_LMID /> -->
	   <CANVAS opacity="$opacity$" id="$LMBRWID$" width="$width$" height="$height$" min_width="$min_width$" min_height="$min_height$" visible="$visible$" render_script="LM::renderBox(this.width, this.height, &quot;$data$;;;;;;;;;;;;;;;;;;;&quot;, ($expand_invert$>0.5));" requires="$LMBRWID$.width;$LMBRWID$.height;$LMBRWID$.opacity" />
	</TEMPLATE>
	
	<!-- a wrap-up for LM_BOX_RAW with more readable params -->
	<TEMPLATE id="LM_BOX" width="100%" height="100%" round="10" opacity="0.5" fill_color="#EEEEEE" fill_opacity="0" fill_pos="0" stroke_color="#777777" stroke_opacity="0.5" stroke_width="1.7" stroke_expand="0" gradient_type="0" gradient_shift="0" fill_color2="" fill_opacity2="0" fill_pos2="100" y_offset="0" render_opacity="1" min_width="10" min_height="10" visible="true">
		<LV id="LMBWID" script="if (&quot;$id$&quot; == &quot;$&quot;+&quot;id$&quot;) return &quot;lmbox_$LMPR$$LMID$&quot;; else return &quot;$id$&quot;;" />
		<NEXT_LMID />
		<LM_BOX_RAW id="$LMBWID$" opacity="$opacity$" width="$width$" height="$height$" min_width="$min_width$" min_height="$min_height$" visible="$visible$" data="$gradient_type$;$render_opacity$;$gradient_shift$;$stroke_width$;$stroke_expand$;$round$;$y_offset$;;;;$fill_color$;$fill_opacity$;$fill_pos$;$fill_color2$;$fill_opacity2$;$fill_pos2$;$stroke_color$;$stroke_opacity$;;" />
	</TEMPLATE>

	<!-- a more customizeable replacement for SYSTEM_ACTION_BUTTON -->
	<TEMPLATE id="LM_ACTION_BUTTON" action_id="undefined" width="" height="" round="10" h_margin="12" v_margin="2" v_offset="-1" font_size="">
		<!-- generate widget id or take given -->
		<LV id="WID" script="if (&quot;$id$&quot; == &quot;$&quot;+&quot;id$&quot;) return &quot;lmactbutton_$LMPR$$LMID$&quot;; else return &quot;$id$&quot;;" />
		<NEXT_LMID />
	  <LAYER_STACK id="ls_$WID$">
	    <LM_BOX id="bg_$WID$_off" round="$round$" opacity="1.00" fill_opacity="0.37" fill_pos="40" stroke_width=".5" stroke_opacity="0.8" stroke_color="#000000" gradient_type="1" fill_color2="#000000" fill_opacity2="0.07" min_width="40" min_height="20" />

	    <LM_BOX id="bg_$WID$_on" round="$round$" opacity="1.00" fill_opacity="0.37" fill_pos="40" stroke_width=".5" stroke_opacity="0.8" stroke_color="#000000" gradient_type="1" fill_color2="#077dbe" fill_opacity2="0.4" min_width="40" min_height="20" />

	    <LM_BOX id="bg_$WID$_pushed" round="$round$" opacity="1.00" fill_opacity="0.37" fill_pos="40" stroke_width="0.5" stroke_opacity="0.9" stroke_color="#000033" gradient_type="1" fill_color2="#178dce" fill_opacity2="0.5" min_width="40" min_height="20" visible="false" />

	    <PARAM id="bg_$WID$_opacity" min="0" max="1" />
	    <PARAM_ANIMATOR id="anim_bg_$WID$_opacity" param_id="bg_$WID$_opacity" enabled="true" duration_ms="120" refresh_time_ms="10" />
	    <LINK from="bg_$WID$_opacity" to="bg_$WID$_on.opacity" />
	    <LINK from="bg_$WID$_opacity" to="bg_$WID$_off.opacity" reverse="true" />

	    <PARAM_MULTI_LINK from="w_$WID$.mouse_over;iab_$WID$.mouse_over" to="anim_bg_$WID$_opacity"  mux_function="max" />

	    <W id="w_$WID$" width="$width$" height="$height$" h_margin="$h_margin$" v_margin="$v_margin$" v_offset="$v_offset$" mouse_sensitive="true">
	    	<LS width="$width$" height="$height$">
			    <TEXT id="tx_$WID$" string_id="$action_id$.name" font_size="$font_size$" />
			    <INVISIBLE_ACTION_BUTTON id="iab_$WID$" action_id="$action_id$" width="100%" height="100%"><TEMPLATE_INNER_CONTENT /></INVISIBLE_ACTION_BUTTON>
			  </LS>
		  </W>
		  <PARAM_MULTI_LINK from="iab_$WID$.pushed" to="bg_$WID$_pushed.visible" />
	  </LAYER_STACK>

	</TEMPLATE>

	<!-- ###################################### -->
	<!-- COMMON RENDER OBJECTS, BASIC RENDER CONTROLS ETC   -->
	<!-- ###################################### -->


	<!-- LM_CANVAS template (CANVAS that can be used to draw multiple widgets) -->
	<TEMPLATE id="LM_CANVAS" width="300" height="200">
	  <LV id="ELEMID" value="$id$" />
	  <SCRIPT script="LM::Canvas obj_$ELEMID$($width$, $height$); " />
	  <CANVAS opacity="1" id="$ELEMID$" width="$width$" height="$height$" render_script="obj_$ELEMID$.DrawItems()" />
	</TEMPLATE>

	<!-- template for simple knob -->
	<TEMPLATE id="LMR_TPL_SIMPLE_KNOB" 
	  param_id="_" visible="true" opacity="1" positions_count="201" pixel_range="128" zoom="1" offset="0" reverse="false" 
	  disabled="false" disabled_opacity="0.2"
	  size="52" body_size="0.95" scaling="1" color="#000000" body_opacity="1"
	  marker_start="0.0" marker_end="0.56" marker_width="1" marker_color="#FFFFFF" marker_opacity="1" marker_stroke_width="1" marker_type="1"
	  angle_start="-135" angle_end="135"
	  color1="" color2="" opacity1="" opacity2=""
	  title="" title_as_param_tooltip="0"
	  >
	  
	  <!-- generate element id -->
	  <LV id="ELEMID" value="knob_$LMPR$_$LMID$" /><VAR id="LMID" formula="$LMID$ + 1" />
	  <LV id="ID_IS_SET" script="if (&quot;$id$&quot; == &quot;$&quot;+&quot;id$&quot;) return &quot;0&quot;; else return &quot;1&quot;;" />

	  <!-- generate id if not given -->
	  <LV id="W_ID" value="$id$" />
	  <R count="$ID_IS_SET$ == 0">
	  	<LV id="W_ID" value="$ELEMID$" />
	  </R>
	  <LV id="_color" value="$color$" />
	  <LV id="_body_opacity" value="$body_opacity$" />
	  <LV id="_marker_color" value="$marker_color$" />
	  <LV id="_marker_opacity" value="$marker_opacity$" />
	  <!-- aliases for auto-layout compatibility -->
	  <R index_list="$color1$"><LV id="_color" value="$color1$" /></R>
		<R index_list="$color2$"><LV id="_marker_color" value="$color2$" /></R>
		<R index_list="$opacity1$"><LV id="_body_opacity" value="$opacity1$" /></R>
		<R index_list="$opacity2$"><LV id="_marker_opacity" value="$opacity2$" /></R>

		<LV id="_size" formula="$size$*$scaling$" />

	  <!-- create param with normalized value (from 0 to 1) -->
	  <FPARAM id="$ELEMID$_nval" min="0" max="1" formula="$W_ID$.zoom*(abs($reverse$-($param_id$-$param_id$.min)/($param_id$.max-$param_id$.min))) - ($W_ID$.zoom - 1)*$W_ID$.offset" exposed="true" />
	  
	  <!-- calculate additional local vars -->
	  <LV id="knob_image_wh" formula="$_size$" />

	  <!-- create object -->
	  <SCRIPT script="LM::SimpleKnob obj_$ELEMID$($_size$, &quot;$angle_start$;$angle_end$;$_color$;$_body_opacity$;$body_size$;$marker_start$;$marker_end$;$marker_width$;$_marker_color$;$_marker_opacity$;$marker_type$;$marker_stroke_width$&quot;); 
	    bool obj_$ELEMID$_initialized = false;" />

	  <CELL>
	    <LAYER_STACK>
	        <CANVAS opacity="$opacity$" id="cnv_$ELEMID$" width="$_size$" height="$_size$" render_script="obj_$ELEMID$.render($ELEMID$_nval);" requires="" />
	        <INVISIBLE_PARAM_KNOB id="$W_ID$" param_id="$param_id$" opacity="$opacity$" width="$knob_image_wh$" height="$knob_image_wh$" positions_count="$positions_count$" visible="$visible$" pixel_range="$pixel_range$" images_count="1" image_orientation="vertical" cursor="$CUR_HAND$" ignore_mask_files="true" reverse="$reverse$" zoom="$zoom$" offset="$offset$" opaque_mouse_mask="true">
	        	<R index_list="$title$">
	        		<R count="$title_as_param_tooltip$==0"><TT text="$title$"/></R>
							<R count="$title_as_param_tooltip$"><PARAM_TOOLTIP param_id="$param_id$" content="$title$"/></R>
	        	</R>
	        	<TEMPLATE_INNER_CONTENT />
	        </INVISIBLE_PARAM_KNOB>
	        
	        <!-- if id is set, create control links  -->
	        <R count="$ID_IS_SET$">
	          <PARAM_LINK from="$W_ID$.visible" to="cnv_$ELEMID$.display" id="link_$W_ID$_visible" />
	          <PARAM_LINK from="$W_ID$.display" to="cnv_$ELEMID$.display" />
	          <PARAM_LINK from="$W_ID$.opacity" to="cnv_$ELEMID$.opacity" />
	          <!-- additional "disabled" attribute -->
	          <PARAM id="$id$.disabled" type="boolean" default="false" />
	          <PARAM_LINK from="$id$.disabled" to="cnv_$ELEMID$.opacity" formula="1+$disabled_opacity$-x" />
	          <PARAM_LINK from="$id$.disabled" to="link_$W_ID$_visible.enabled" formula="1-x" />
	          <PARAM_LINK from="$id$.disabled" to="$W_ID$.visible" formula="1-x" />
	         </R>
	    </LAYER_STACK>

	    <ACTION_TRIGGER event_id="$param_id$.value_changed" script="cnv_$ELEMID$.Invalidate()" requires="cnv_$ELEMID$.Invalidate" />
	  </CELL>

	</TEMPLATE>
	
	<DEFINE>
		<LMR_KNOB base_type="LMR_TPL_SIMPLE_KNOB" size="50" color="#2c9fdf" marker_color="#EEEEEE" marker_end="0.56" />
	</DEFINE>

	<!-- ==================== -->
	<!-- SVG ICONS  -->
	<!-- ==================== -->

	<TEMPLATE id="SVG_ICON_DROPDOWN" color="#777777" vo="0" size="16" ho="0" opacity="1">
	  <svg v_offset="$vo$" h_offset="$ho$" width="$size$" height="$size$" viewBox="20 20 60 60" opacity="$opacity$"><polygon points="51.2,62.7 69,45 33,45" stroke="$color$" fill="$color$" /></svg>
	</TEMPLATE>

	<TEMPLATE id="SVG_ICON_UPDOWN" color="#777777" size="16" opacity="1">
	<svg width="$size$" height="20" viewBox="20 10 60 60"  opacity="$opacity$">
	  <polygon points="51,20 69,36 33,36" stroke="$color$" fill="$color$" /><polygon points="51,62 69,46 33,46" stroke="$color$" fill="$color$" /></svg>
	</TEMPLATE>

	<TEMPLATE id="SVG_ICON_PREV" color="#777777" size="16" opacity="1">
		<svg width="$size$" height="$size$" viewBox="-120 170 700 650" opacity="$opacity$">
		<path fill="$color$" d="M400 270c0 0 0 460 0 460c0 0 -400 -230 -400 -230c0 0 400 -230 400 -230" />
		</svg>
	</TEMPLATE>

	<TEMPLATE id="SVG_ICON_NEXT" color="#777777" size="16" opacity="1">
	<svg width="$size$" height="$size$" viewBox="-180 170 700 650" opacity="$opacity$">
	<path fill="$color$" d="M0 270c0 0 400 230 400 230c0 0 -400 230 -400 230c0 0 0 -460 0 -460c0 0 0 0 0 0" />
	</svg>
	</TEMPLATE>

	<TEMPLATE id="SVG_ICON_MENU" color="#777777" size="16" opacity="1">
	<svg h_offset="2" width="$size$" height="$size$" viewBox="0 1 80 40" opacity="$opacity$">
	  <line class="cls-1" x1="0" y1="10" x2="60" y2="10" stroke="$color$" stroke-width="5" /><line class="cls-1" x1="0" y1="20" x2="60" y2="20" stroke="$color$" stroke-width="5" /><line class="cls-1" x1="0" y1="30" x2="60" y2="30" stroke="$color$" stroke-width="5" />
	</svg>
	</TEMPLATE>

	<TEMPLATE id="SVG_ICON_INFO" color="#777777" size="16" opacity="1">
	<svg width="$size$" height="$size$" viewBox="1 2 21 21" opacity="$opacity$">
	    <path fill="$color$" stroke="none" d="M11 17h2v-6h-2v6zm1-15C6.48 2 2 6.48 2 12s4.48 10 10 10 10-4.48 10-10S17.52 2 12 2zm0 18c-4.41 0-8-3.59-8-8s3.59-8 8-8 8 3.59 8 8-3.59 8-8 8zM11 9h2V7h-2v2z"/></svg>
	</TEMPLATE>

	<TEMPLATE id="SVG_ICON_QUESTION" color="#777777" size="16" opacity="1">
		<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 525 525"  width="$size$" height="$size$" fill="$color$" opacity="$opacity$"><path d="M256 8C119.043 8 8 119.083 8 256c0 136.997 111.043 248 248 248s248-111.003 248-248C504 119.083 392.957 8 256 8zm0 448c-110.532 0-200-89.431-200-200 0-110.495 89.472-200 200-200 110.491 0 200 89.471 200 200 0 110.53-89.431 200-200 200zm107.244-255.2c0 67.052-72.421 68.084-72.421 92.863V300c0 6.627-5.373 12-12 12h-45.647c-6.627 0-12-5.373-12-12v-8.659c0-35.745 27.1-50.034 47.579-61.516 17.561-9.845 28.324-16.541 28.324-29.579 0-17.246-21.999-28.693-39.784-28.693-23.189 0-33.894 10.977-48.942 29.969-4.057 5.12-11.46 6.071-16.666 2.124l-27.824-21.098c-5.107-3.872-6.251-11.066-2.644-16.363C184.846 131.491 214.94 112 261.794 112c49.071 0 101.45 38.304 101.45 88.8zM298 368c0 23.159-18.841 42-42 42s-42-18.841-42-42 18.841-42 42-42 42 18.841 42 42z"/></svg>
	</TEMPLATE>

	<TEMPLATE id="SVG_ICON_UNDO" color="#777777" size="16" opacity="1">
	<svg width="$size$" height="$size$" viewBox="0 -2 18 18" opacity="$opacity$">
	<path fill="$color$" stroke="none" d="M11.904 16c1.777-3.219 2.076-8.13-4.904-7.966v3.966l-6-6 6-6v3.881c8.359-0.218 9.29 7.378 4.904 12.119z"></path>
	</svg>
	</TEMPLATE>

	<TEMPLATE id="SVG_ICON_REDO" color="#777777" size="16" opacity="1">
	<svg width="$size$" height="$size$" viewBox="0 -2 18 18" opacity="$opacity$">
	<path fill="$color$" stroke="none" d="M9 3.881v-3.881l6 6-6 6v-3.966c-6.98-0.164-6.681 4.747-4.904 7.966-4.386-4.741-3.455-12.337 4.904-12.119z"></path>
	</svg>
	</TEMPLATE>

	<!-- status icons -->
	<!-- play -->
	<TEMPLATE id="SVG_ICON_RUNNING" size="16" color="#777777" opacity="1">
	  <svg width="$size$" height="$size$" viewBox="0 0 20 20" opacity="$opacity$">
	    <path fill="$color$" d="M2.93 17.07A10 10 0 1 1 17.07 2.93 10 10 0 0 1 2.93 17.07zm12.73-1.41A8 8 0 1 0 4.34 4.34a8 8 0 0 0 11.32 11.32zM7 6l8 4-8 4V6z"/>
	  </svg>
	</TEMPLATE>

	<!-- attention -->
	<TEMPLATE id="SVG_ICON_WARNING" size="16" color="#777777" opacity="1">
	  <svg width="$size$" height="$size$" viewBox="0 0 1000 1000" opacity="$opacity$">
	    <path fill="$color$" d="M956.5390625 874c6.667 10.667 6.667 22 0 34c-6.667 10.667 -16.667 16 -30 16c0 0 -892 0 -892 0c-12 0 -21.333 -5.333 -28 -16c-8 -12 -8.667 -23.333 -2 -34c0 0 446 -782 446 -782c5.333 -12 15.333 -18 30 -18c14.667 0 24.667 6 30 18c0 0 446 782 446 782m-420 -50c0 0 0 -100 0 -100c0 0 -110 0 -110 0c0 0 0 100 0 100c0 0 110 0 110 0c0 0 0 0 0 0m0 -174c0 0 0 -300 0 -300c0 0 -110 0 -110 0c0 0 0 300 0 300c0 0 110 0 110 0c0 0 0 0 0 0" />
	  </svg>
	</TEMPLATE>

	<!-- errors -->
	<TEMPLATE id="SVG_ICON_ERROR" size="16" color="#777777" opacity="1">
	  <svg width="$size$" height="$size$" viewBox="0 0 20 20" opacity="$opacity$"><path fill="$color$" d="M2.93 17.07A10 10 0 1 1 17.07 2.93 10 10 0 0 1 2.93 17.07zM11.4 10l2.83-2.83-1.41-1.41L10 8.59 7.17 5.76 5.76 7.17 8.59 10l-2.83 2.83 1.41 1.41L10 11.41l2.83 2.83 1.41-1.41L11.41 10z"/></svg>
	</TEMPLATE>

	<!-- stopwatch -->
	<TEMPLATE id="SVG_ICON_STOPWATCH" size="16" color="#777777" opacity="1">
	  <svg width="$size$" height="$size$" viewBox="2 3 20 21" opacity="$opacity$"><g><path fill="$color$" d="M19.414 8.902c.104-.048.206-.108.293-.195l.5-.5c.391-.391.391-1.023 0-1.414s-1.023-.391-1.414 0l-.5.5-.115.173c-1.387-1.312-3.188-2.19-5.189-2.41l.011-.056v-1h1c.55 0 1-.45 1-1s-.45-1-1-1h-4c-.55 0-1 .45-1 1s.45 1 1 1h1v1l.012.057c-4.506.492-8.012 4.307-8.012 8.943 0 4.971 4.029 9 9 9s9-4.029 9-9c0-1.894-.588-3.648-1.586-5.098zm-7.414 12.098c-3.859 0-7-3.14-7-7s3.141-7 7-7 7 3.14 7 7-3.141 7-7 7zM13 13v-2c0-.55-.45-1-1-1s-1 .45-1 1v3c0 .55.45 1 1 1h3c.55 0 1-.45 1-1s-.45-1-1-1h-2zM12 8c-3.312 0-6 2.688-6 6s2.688 6 6 6 6-2.688 6-6-2.688-6-6-6zm0 11c-2.757 0-5-2.243-5-5s2.243-5 5-5 5 2.243 5 5-2.243 5-5 5z"/></g></svg>
	</TEMPLATE>

	<!-- setting icons -->
	<TEMPLATE id="SVG_ICON_SETTINGS" color="#777777" size="18" opacity="1">
	  <svg width="$size$" height="$size$" viewBox="-1 -1 26 26" stroke="$color$" fill="none" stroke-width="2" opacity="$opacity$"><circle cx="12" cy="12" r="3"></circle><path stroke="$color$" d="M19.4 15a1.65 1.65 0 0 0 .33 1.82l.06.06a2 2 0 0 1 0 2.83 2 2 0 0 1-2.83 0l-.06-.06a1.65 1.65 0 0 0-1.82-.33 1.65 1.65 0 0 0-1 1.51V21a2 2 0 0 1-2 2 2 2 0 0 1-2-2v-.09A1.65 1.65 0 0 0 9 19.4a1.65 1.65 0 0 0-1.82.33l-.06.06a2 2 0 0 1-2.83 0 2 2 0 0 1 0-2.83l.06-.06a1.65 1.65 0 0 0 .33-1.82 1.65 1.65 0 0 0-1.51-1H3a2 2 0 0 1-2-2 2 2 0 0 1 2-2h.09A1.65 1.65 0 0 0 4.6 9a1.65 1.65 0 0 0-.33-1.82l-.06-.06a2 2 0 0 1 0-2.83 2 2 0 0 1 2.83 0l.06.06a1.65 1.65 0 0 0 1.82.33H9a1.65 1.65 0 0 0 1-1.51V3a2 2 0 0 1 2-2 2 2 0 0 1 2 2v.09a1.65 1.65 0 0 0 1 1.51 1.65 1.65 0 0 0 1.82-.33l.06-.06a2 2 0 0 1 2.83 0 2 2 0 0 1 0 2.83l-.06.06a1.65 1.65 0 0 0-.33 1.82V9a1.65 1.65 0 0 0 1.51 1H21a2 2 0 0 1 2 2 2 2 0 0 1-2 2h-.09a1.65 1.65 0 0 0-1.51 1z"></path></svg>
	</TEMPLATE>

	<TEMPLATE id="SVG_ICON_SETTINGS_SOLID" color="#777777" size="20" opacity="1">
	  <svg width="$size$" height="$size$" viewBox="0 0 16 16" opacity="$opacity$"><path stroke="$color$" fill="$color$" d="M14 8.77v-1.6l-1.94-.64-.45-1.09.88-1.84-1.13-1.13-1.81.91-1.09-.45-.69-1.92h-1.6l-.63 1.94-1.11.45-1.84-.88-1.13 1.13.91 1.81-.45 1.09L0 7.23v1.59l1.94.64.45 1.09-.88 1.84 1.13 1.13 1.81-.91 1.09.45.69 1.92h1.59l.63-1.94 1.11-.45 1.84.88 1.13-1.13-.92-1.81.47-1.09L14 8.75v.02zM7 11c-1.66 0-3-1.34-3-3s1.34-3 3-3 3 1.34 3 3-1.34 3-3 3z"/></svg>
	</TEMPLATE>



	<TEMPLATE id="SVG_ICON_SAVE" color="#777777" size="20" opacity="1">
	  <svg xmlns="http://www.w3.org/2000/svg" width="$size$" height="$size$" viewBox="0 0 24 24" fill="none" stroke="$color$" stroke-width="2" opacity="$opacity$"><path d="M19 21H5a2 2 0 0 1-2-2V5a2 2 0 0 1 2-2h11l5 5v11a2 2 0 0 1-2 2z"></path><polyline points="17 21 17 13 7 13 7 21"></polyline><polyline points="7 3 7 8 15 8"></polyline></svg>
	</TEMPLATE>

	<TEMPLATE id="SVG_ICON_EYE" color="#777777" size="20" opacity="1">
	  <svg width="$size$" height="$size$" viewBox="0 0 576 512" opacity="$opacity$"><path fill="$color$" stroke="$color$" d="M288 144a110.94 110.94 0 0 0-31.24 5 55.4 55.4 0 0 1 7.24 27 56 56 0 0 1-56 56 55.4 55.4 0 0 1-27-7.24A111.71 111.71 0 1 0 288 144zm284.52 97.4C518.29 135.59 410.93 64 288 64S57.68 135.64 3.48 241.41a32.35 32.35 0 0 0 0 29.19C57.71 376.41 165.07 448 288 448s230.32-71.64 284.52-177.41a32.35 32.35 0 0 0 0-29.19zM288 400c-98.65 0-189.09-55-237.93-144C98.91 167 189.34 112 288 112s189.09 55 237.93 144C477.1 345 386.66 400 288 400z"/></svg>
	</TEMPLATE>

	<TEMPLATE id="SVG_ICON_ADJUST" color="#777777" size="20" opacity="1">
	  <svg width="$size$" height="$size$" viewBox="1.5 1.2 21 22.2" opacity="$opacity$"><path fill="$color$" fill-opacity="0.9" stroke="none" d="M12 4c-4.418 0-8 3.582-8 8s3.582 8 8 8 8-3.582 8-8-3.582-8-8-8zm0 14c-3.314 0-6-2.686-6-6s2.686-6 6-6 6 2.686 6 6-2.686 6-6 6zM12 7v10c2.757 0 5-2.243 5-5s-2.243-5-5-5z"/></svg>
	</TEMPLATE>

	<TEMPLATE id="SVG_ICON_POWER" color="#777777" size="20" opacity="1">
	  <svg width="$size$" height="$size$" viewBox="3 4 17 17" opacity="$opacity$"><path fill="$color$" stroke="none" d="M11.5 18.573c-1.736 0-3.368-.676-4.596-1.903-1.227-1.228-1.904-2.86-1.904-4.597s.677-3.369 1.904-4.597c.391-.391 1.023-.391 1.414 0s.391 1.023 0 1.414c-.85.851-1.318 1.981-1.318 3.183s.468 2.333 1.318 3.183c.85.85 1.979 1.317 3.182 1.317s2.332-.468 3.182-1.317c.851-.85 1.318-1.98 1.318-3.183s-.468-2.333-1.318-3.183c-.391-.391-.391-1.023 0-1.414s1.023-.391 1.414 0c1.227 1.229 1.904 2.861 1.904 4.597s-.677 3.369-1.904 4.597c-1.228 1.227-2.86 1.903-4.596 1.903zM11.5 11c-.553 0-1-.448-1-1v-5c0-.552.447-1 1-1s1 .448 1 1v5c0 .552-.447 1-1 1z"/></svg>
	</TEMPLATE>

	<TEMPLATE id="SVG_ICON_ZOOM" color="#777777" size="16" opacity="1">
	  <svg width="$size$" height="$size$" viewBox="-55 -55 420 420" opacity="$opacity$">
	    <path d="M273.587,214.965c49.11-49.111,49.109-129.021,0-178.132c-49.111-49.111-129.02-49.111-178.13,0
	      C53.793,78.497,47.483,140.462,76.51,188.85c0,0,2.085,3.498-0.731,6.312c-16.065,16.064-64.263,64.263-64.263,64.263
	      c-12.791,12.79-15.836,30.675-4.493,42.02l1.953,1.951c11.343,11.345,29.229,8.301,42.019-4.49c0,0,48.096-48.097,64.128-64.128
	      c2.951-2.951,6.448-0.866,6.448-0.866C169.958,262.938,231.923,256.629,273.587,214.965z M118.711,191.71
	      c-36.288-36.288-36.287-95.332,0.001-131.62c36.288-36.287,95.332-36.288,131.619,0c36.288,36.287,36.288,95.332,0,131.62
	      C214.043,227.996,155,227.996,118.711,191.71z" stroke="#AAAAAA" fill="$color$"/>
	    <path d="M126.75,118.424c-1.689,0-3.406-0.332-5.061-1.031c-6.611-2.798-9.704-10.426-6.906-17.038
	        c17.586-41.559,65.703-61.062,107.261-43.476c6.611,2.798,9.704,10.426,6.906,17.038c-2.799,6.612-10.425,9.703-17.039,6.906
	        c-28.354-11.998-61.186,1.309-73.183,29.663C136.629,115.445,131.815,118.424,126.75,118.424z" stroke="#AAAAAA" fill="$color$"/>
	  </svg>
	</TEMPLATE>

	<TEMPLATE id="SVG_ICON_FILE" color="#777777" size="16" opacity="1">
	  <svg width="$size$" height="$size$" viewBox="25.5 25 50 50" opacity="$opacity$"><polyline points="55.63 30.4 55.63 41.28 66.58 41.28" stroke="$color$" fill="none" stroke-width="3" /><path d="M54.29,30H37.37a3.28,3.28,0,0,0-3.28,3.28V66.72A3.28,3.28,0,0,0,37.37,70H63.63a3.28,3.28,0,0,0,3.28-3.28V42.62A3.23,3.23,0,0,0,66,40.34l-9.39-9.39A3.23,3.23,0,0,0,54.29,30Z" stroke="$color$" fill="none" stroke-width="3" /></svg>
	</TEMPLATE>
	
	 <TEMPLATE id="SVG_ICON_FOLDER" color='#777777' size="16" opacity='1'>
    <svg width="$size$" height="$size$" viewBox="0 0 576 512" opacity="$opacity$"><path fill="$color$" d="M527.9 224H480v-48c0-26.5-21.5-48-48-48H272l-64-64H48C21.5 64 0 85.5 0 112v288c0 26.5 21.5 48 48 48h400c16.5 0 31.9-8.5 40.7-22.6l79.9-128c20-31.9-3-73.4-40.7-73.4zM48 118c0-3.3 2.7-6 6-6h134.1l64 64H426c3.3 0 6 2.7 6 6v42H152c-16.8 0-32.4 8.8-41.1 23.2L48 351.4zm400 282H72l77.2-128H528z"/>
    </svg>
  </TEMPLATE>

	<TEMPLATE id="SVG_ICON_EDIT" color="#777777" size="14" opacity="1">
	  <svg width="$size$" height="$size$" viewBox="0 0 576 512" opacity="$opacity$"><path fill="$color$" stroke="$color$" d="M402.3 344.9l32-32c5-5 13.7-1.5 13.7 5.7V464c0 26.5-21.5 48-48 48H48c-26.5 0-48-21.5-48-48V112c0-26.5 21.5-48 48-48h273.5c7.1 0 10.7 8.6 5.7 13.7l-32 32c-1.5 1.5-3.5 2.3-5.7 2.3H48v352h352V350.5c0-2.1.8-4.1 2.3-5.6zm156.6-201.8L296.3 405.7l-90.4 10c-26.2 2.9-48.5-19.2-45.6-45.6l10-90.4L432.9 17.1c22.9-22.9 59.9-22.9 82.7 0l43.2 43.2c22.9 22.9 22.9 60 .1 82.8zM460.1 174L402 115.9 216.2 301.8l-7.3 65.3 65.3-7.3L460.1 174zm64.8-79.7l-43.2-43.2c-4.1-4.1-10.8-4.1-14.8 0L436 82l58.1 58.1 30.9-30.9c4-4.2 4-10.8-.1-14.9z"/></svg>
	</TEMPLATE>

	<TEMPLATE id="SVG_ICON_OPACITY" color="#777777" size="12" opacity="1">
	  <svg height="$size$" width="$size$" viewBox="0 0 512 512" opacity="$opacity$"><path stroke="$color$" fill="$color$" d="M8 256c0 136.966 111.033 248 248 248s248-111.034 248-248S392.966 8 256 8 8 119.033 8 256zm248 184V72c101.705 0 184 82.311 184 184 0 101.705-82.311 184-184 184z"  /></svg>
	</TEMPLATE>

	<TEMPLATE id="SVG_ICON_LIST" color="#777777" size="16" opacity="1">
	  <svg height="$size$" width="$size$" viewBox="0 0 20 20" opacity="$opacity$"><path stroke="none" fill="$color$" d="M1 4h2v2H1V4zm4 0h14v2H5V4zM1 9h2v2H1V9zm4 0h14v2H5V9zm-4 5h2v2H1v-2zm4 0h14v2H5v-2z"/></svg>
	</TEMPLATE>

	<TEMPLATE id="SVG_ICON_CLOSE" color="#777777" size="16" opacity="1">
	  <svg viewBox="0 0 20 20" width="$size$" height="$size$" opacity="$opacity$"><path fill="$color$" d="M10 8.586L2.929 1.515 1.515 2.929 8.586 10l-7.071 7.071 1.414 1.414L10 11.414l7.071 7.071 1.414-1.414L11.414 10l7.071-7.071-1.414-1.414L10 8.586z"/></svg>
	</TEMPLATE>

	<TEMPLATE id="SVG_ICON_CORNER_RESIZE" color="#777777" size="16" opacity="1">
		<svg width="$size$" height="$size$" viewBox="0 0 10 10" opacity="$opacity$">
		  <polygon points="0,10 10,0" stroke="$color$" fill="$color$" />
		  <polygon points="4,10 10,4" stroke="$color$" fill="$color$" />
		  <polygon points="8,10 10,8" stroke="$color$" fill="$color$" />
		</svg>
	</TEMPLATE>
	
	<TEMPLATE id="SVG_ICON_CHECKBOX" size="16" color='#777777' opacity='1'>
		<svg width="$size$" height="$size$" opacity="$opacity$" viewBox="0 0 512 512"><path stroke="$color$" fill="$color$" d="M400 32H48C21.49 32 0 53.49 0 80v352c0 26.51 21.49 48 48 48h352c26.51 0 48-21.49 48-48V80c0-26.51-21.49-48-48-48zm0 400H48V80h352v352zm-35.864-241.724L191.547 361.48c-4.705 4.667-12.303 4.637-16.97-.068l-90.781-91.516c-4.667-4.705-4.637-12.303.069-16.971l22.719-22.536c4.705-4.667 12.303-4.637 16.97.069l59.792 60.277 141.352-140.216c4.705-4.667 12.303-4.637 16.97.068l22.536 22.718c4.667 4.706 4.637 12.304-.068 16.971z"/></svg>
	</TEMPLATE>

	<TEMPLATE id="SVG_ICON_CHECKBOX_EMPTY" size="16" color='#777777' opacity='1'>
		<svg width="$size$" height="$size$" opacity="$opacity$" viewBox="0 0 512 512"><path stroke="$color$" fill="$color$" d="M400 32H48C21.49 32 0 53.49 0 80v352c0 26.51 21.49 48 48 48h352c26.51 0 48-21.49 48-48V80c0-26.51-21.49-48-48-48zm0 400H48V80h352v352z"/></svg>
	</TEMPLATE>

	
	<!-- template to use other elements as on/off elements -->
	<TEMPLATE id="LM_ON_OFF" param_id="generic_param" hint="" text="" size="16" color="#777777" opacity="1" o="" on="SVG_ICON_CHECKBOX" off="SVG_ICON_CHECKBOX_EMPTY" off_opacity="0.5" id_piece="on_off" h_offset="0" v_offset="0" visible_when="">
		<LV id="_on" value="$on$" /><LV id="_off" value="$off$" />
		<R index_list="$o$"><LV id="_on" value="$o$" /><LV id="_off" value="$o$" /></R>
		<DEFINE><_LOCAL_ON base_type="$_on$" /><_LOCAL_OFF base_type="$_off$" /></DEFINE>
		<LV id="ELEMID" script="if (&quot;$id$&quot; != &quot;$&quot;+&quot;id$&quot;) return &quot;$id$&quot;; else return &quot;$id_piece$_$LMPR$_$LMID$&quot;;" /><NEXT_LMID />

		<W id="$ELEMID$" h_offset="$h_offset$" v_offset="$v_offset$" layout_type="layer_stack">
			<ROW>
				<LAYER_STACK>
					<CELL id='$ELEMID$_on'><_LOCAL_ON size="$size$" color="$color$" opacity="$opacity$" /></CELL>
					<LV id="_off_opacity" formula="$off_opacity$*$opacity$" />
					<CELL id='$ELEMID$_off'><_LOCAL_OFF size="$size$" color="$color$" opacity="$_off_opacity$" /></CELL>
					<PARAM_LINK from='$param_id$' to="$ELEMID$_on.display" />
					<PARAM_LINK from='$ELEMID$_on.display' to="$ELEMID$_off.display" reverse="true" />
				</LAYER_STACK>
				<R index_list="$text$">
					<CELL width='2' />
					<TEXT value="$text$"></TEXT>
				</R>
			</ROW>
			<IPB param_id="$param_id$"><TOOLTIP text='$hint$' /></IPB>
		</W>
		<UNDEFINE><_LOCAL_ON /><_LOCAL_OFF /></UNDEFINE>
		<R index_list="$visible_when$">
			<FPARAM id="fp_$ELEMID$_visible" formula="$visible_when$" />
			<LINK from="fp_$ELEMID$_visible" to="$ELEMID$.visible" />
		</R>
	</TEMPLATE>

	<TEMPLATE id="LM_CHECKBOX" param_id="" hint="" text="" size="16" color="#777777" opacity="1">
		<LM_ON_OFF param_id="$param_id$" hint="$hint$" text="$text$" size="$size$" color="$color$" opacity="$opacity$" id_piece="checkbox" on="SVG_ICON_CHECKBOX" off="SVG_ICON_CHECKBOX_EMPTY" off_opacity="1" />
	</TEMPLATE>

	<TEMPLATE id="SVG_ICON_PLUS" color="#777777" size="14" opacity="0.9" vo="0" ho="0" >
	  <svg v_offset="$vo$" h_offset="$ho$" width="$size$" height="$size$" viewBox="0 0 512 512" opacity="$opacity$"><path stroke="$color$" fill="$color$" d="M352 240v32c0 6.6-5.4 12-12 12h-88v88c0 6.6-5.4 12-12 12h-32c-6.6 0-12-5.4-12-12v-88h-88c-6.6 0-12-5.4-12-12v-32c0-6.6 5.4-12 12-12h88v-88c0-6.6 5.4-12 12-12h32c6.6 0 12 5.4 12 12v88h88c6.6 0 12 5.4 12 12zm96-160v352c0 26.5-21.5 48-48 48H48c-26.5 0-48-21.5-48-48V80c0-26.5 21.5-48 48-48h352c26.5 0 48 21.5 48 48zm-48 346V86c0-3.3-2.7-6-6-6H54c-3.3 0-6 2.7-6 6v340c0 3.3 2.7 6 6 6h340c3.3 0 6-2.7 6-6z"/></svg>
	</TEMPLATE>

	<TEMPLATE id="SVG_ICON_MINUS" color="#777777" size="14" opacity="0.9" vo="0" ho="0" >
	  <svg v_offset="$vo$" h_offset="$ho$" width="$size$" height="$size$" viewBox="0 0 512 512" opacity="$opacity$"><path stroke="$color$" fill="$color$" d="M108 284c-6.6 0-12-5.4-12-12v-32c0-6.6 5.4-12 12-12h232c6.6 0 12 5.4 12 12v32c0 6.6-5.4 12-12 12H108zM448 80v352c0 26.5-21.5 48-48 48H48c-26.5 0-48-21.5-48-48V80c0-26.5 21.5-48 48-48h352c26.5 0 48 21.5 48 48zm-48 346V86c0-3.3-2.7-6-6-6H54c-3.3 0-6 2.7-6 6v340c0 3.3 2.7 6 6 6h340c3.3 0 6-2.7 6-6z" /></svg>
	</TEMPLATE>

	<!-- ###################################### -->
	<!-- END OF COMMON GRAPHICS AND TEMPLATES  -->
	<!-- ###################################### -->







<!-- <PARAM_TEXT param_id="$reload_script$" /> -->

	<!-- ###################################### -->
	<!-- CORE SKIN VARS, ACTIONS AND TRIGGERS  -->
	<!-- ###################################### -->

	<!-- is rewritten to "true" if settings editor is included -->
	<VAR id="SKIN_EDITABLE" value="false" />
	<FPARAM id="skin_kuiml_tool_enabled" formula="((skin_scanner_enabled > 0.5) or (controls_gen_enabled > 0.5))" type="boolean" />
	
	
	
	<!--
	<FORMULA_PARAM id="gzd" formula="gui.zoom-floor(gui.zoom)" />
	<PARAM_TEXT param_id="gzd" value_format="3.10" content="{value}" />
	<PARAM_TEXT_CONTROL param_id="gui.zoom" value_format="3.10" content="{value}" />
	-->
	
	<!-- if we're in exported plugin -->
	<IF condition="!$SCRIPT_EDIT_MODE$">
			<!-- NO_EXPORT_IF_STATIC -->
	    <!-- trigger to reload subgui on opening  -->
	    <ACTION_TRIGGER event_id="body_loaded.value_changed" condition_formula="window.loaded==1" script="reloadBody()" requires="" async="true" />
	    <!-- END_NO_EXPORT_IF_STATIC -->
	    <!-- in exported plugin there might be no easy way to reload script -->
	    <ACTION id="ReloadScript" type="Script" name="---" script="bool no_reload_possible = true;" requires="$reload_script$" />
	    <!-- action for full skin reloading -->
	    <ACTION id="ReloadSkin" type="Script" name="Reload skin" script="double zoom_delta = 0.0000000001; double gzd = gui.zoom-floor(gui.zoom); if ((gzd > 0) and (gzd &lt; 0.5)) { zoom_delta = -zoom_delta; } gui.zoom=gui.zoom+zoom_delta; " requires="gui.zoom" exposed="true" />
	</IF>

	<!-- trigger to reload GUI back to previous zoom level on skin reload -->
	<!--
	<ACTION_TRIGGER event_id="window.loaded.value_changed" condition_formula="(gui.zoom-floor(gui.zoom)>0.5)" script="gui.zoom=gui.zoom+0.5; " requires="gui.zoom"/>
	-->

	<!-- ==================== -->
	<!-- MISC SKIN PARTS -->
	<!-- ==================== -->

	<!-- ###################################### -->
	<!-- TOOLBAR ELEMENTS  -->
	<!-- ###################################### -->
	
	<!-- add version info to plugin title (for exported plugins) -->
	<LV id="LM_plugin_version_fmt_val" value="" />
	<IF_EXPORTED>
		<R index_list="$PLUGIN_VERSION$">
			<LV id="LM_plugin_version_fmt_val" value=" $PLUGIN_VERSION$" />
		</R>
	</IF_EXPORTED>
	<IF_NOT_EXPORTED>
		<LV id="LM_plugin_version_fmt_val" value="" />
	</IF_NOT_EXPORTED>
	<STRING id="LM_plugin_version_fmt" default="$LM_plugin_version_fmt_val$" />

	<STRING id="script_name_short" default="No Script"/>
	<IF condition="$SCRIPT_EDIT_MODE$">
	  <ACTION id="ShowDefaultScriptsMenu" type="DisplayPlugNScriptMenu" script_file_name_string_id="script_name_short" name="Select script" />
	  <ACTION id="OpenScriptFile" type="Script" script="
		string s = $script_file_path$; 
		string ext = (s.substr(s.length-3)); 
		/* for binary sources open .cpp */
		if ((ext ==&quot;x64&quot;) or (ext ==&quot;x86&quot;) or (ext ==&quot;bin&quot;)) { 
			s = s.substr(0, s.length-3);
			s += &quot;cpp&quot;;
		} 
		script_file_to_edit = s; 
		OpenScriptFile_();"
		name="Edit .cxx file" requires="OpenScriptFile_" />
	</IF>
	<IF condition="!$SCRIPT_EDIT_MODE$">
	  <ACTION id="ShowDefaultScriptsMenu" type="DisplayMessageBox" 
	    title="Action missing"  message="This action is unavailable now." style="info" name="Action Unavailable" />
	   <ACTION id="OpenScriptFile" type="DisplayMessageBox" 
	    title="Action missing"  message="This action is unavailable now." style="info" name="Action Unavailable" />
	</IF>
	<ESTRING id="some_url" default="https://google.com/" />
	<ACTION type="OpenUrl" id="openSomeUrl" url_string_id="some_url" name="Open url or file" />

	<!-- actions for menus -->
	<!-- for windows explorer we have to change / to \ -->
	<STRING id="MainSkinFilenamePrep" default="$SKIN_FILE$" />
	<STRING id="CommonFilenamePrep" default="$_FILE_$" />

	<IF_WINDOWS>
		<ONLOAD requires="MainSkinFilename;MainSkinFilenamePrep;CommonFilenamePrep" script="
			array&lt;string> ars = (MainSkinFilenamePrep+&quot;&quot;).split(&quot;/&quot;); 
			MainSkinFilenamePrep = join(ars, &quot;\\&quot;); 
			ars = (CommonFilenamePrep+&quot;&quot;).split(&quot;/&quot;); 
			CommonFilenamePrep = join(ars, &quot;\\&quot;); 
		" />
	</IF_WINDOWS>
	<ACTION id="OpenSkinFile" type="OpenUrl" url_string_id="MainSkinFilenamePrep" application_string_id="edit_kuiml_app" name="Edit .xml file" />
	<ACTION id="OpenSkinDir" type="OpenUrl" url_string_id="MainSkinFilenamePrep" application="$FILE_EXPLORER_APP$" name="Open skin folder" exposed="true" />

	<ACTION id="OpenLMDir" type="OpenUrl" url_string_id="CommonFilenamePrep" application="$FILE_EXPLORER_APP$" name="Open LM folder" exposed="true" />

	<ACTION id="OpenSkinDirWrap" name="Open skin folder" type="Script" script="if (Kt::Keyboard::IsShiftKeyDown() || Kt::Keyboard::IsCtrlKeyDown()) {
			OpenLMDir();
		} else {
			OpenSkinDir();
		}" />
	
	<ESTRING id="script_file_to_edit" />
	<ACTION id="OpenScriptFile_" type="OpenUrl" url_string_id="script_file_to_edit" application_string_id="edit_script_app" name="Edit .cxx file" />

	<ACTION id="OpenKUIMLFile" type="OpenUrl" url_string_id="$script_gui_path$" application_string_id="edit_kuiml_app" name="Edit .kuiml file" />
	<ACTION id="OpenScriptDirectory" type="OpenUrl" url_string_id="$script_file_path$" application="$FILE_EXPLORER_APP$" name="Open script folder"/>
	<ACTION id="OpenScriptDoc" type="OpenUrl" url_string_id="$script_doc_path$" name="Open script manual" />
	
	<ACTION id="OpenInfo" type="Script" script="string s = a_toolbar_infolink; 
	  if (s == &quot;&quot;) { 
	    DisplayAbout();
	  } else if ((s.findFirstOf(&quot;http://&quot;) == 0) or (s.findFirstOf(&quot;https://&quot;) == 0)) {
	    some_url = s;
	    openSomeUrl();
	  } else {
	    some_url = &quot;&quot;&quot;$SKINS_DIR$&quot;&quot;&quot;+a_toolbar_infolink;
	    openSomeUrl();
	  }" name="Open plugin info" />
	<ACTION id="ToggleControlsSettings" type="Script" script="theme.show_settings = abs(theme.show_settings - 1);" name="Toggle controls settings" />

	<ACTION type="DisplayMessageBox" 
	    id="CustomMenuUnavailable"
	    title="Custom menu missing" 
	    message="Create custom menu and assign it like this:&#10;@a_custom_menu = [your_menu_name].Popup;" 
	    style="info" 
	    timeout="-1" 
	    name="Custom Menu Unavailable Message Box"
	/>
	<ACTION type="DisplayMessageBox" 
	    id="ActionUnavailable"
	    title="Action missing" 
	    message="This action is unavailable now." 
	    style="info" 
	    timeout="-1" 
	    name="Action Unavailable Message Box"
	/>
	

	<EXPOSED_OBJECTS object_ids="ActionUnavailable" />
	<ACT id="toggle_bypass" script="$bypass$ = abs($bypass$-1)" />

	<!-- this menu is used only if toolbar (and so toolbar menu) is not included -->
	<POPUP_MENU id="menu_lm_core">
	  <MENU_ITEM action_id="OpenMenuOriginal" />
		
		

		<IF_EXPORTED>

		  <MENU_SEPARATOR />
		  <MENU_ITEM action_id="ReloadSkin" />
		  <MENU_ITEM action_id="OpenSkinFile" />
		  <MENU_ITEM action_id="OpenSkinDirWrap" />
		  <MENU_SEPARATOR />
			<MENU_ITEM action_id="OpenLogFile" />
		</IF_EXPORTED>
	  

	</POPUP_MENU>
	
	<!-- flag to check if skin settings were included -->
	<VAR id="SKIN_SETTINGS_INCLUDED" value="0" />
	<VAR id="SKIN_SKANNER_INCLUDED" value="0" />
	<VAR id="CONTROLS_GEN_INCLUDED" value="0" />
	<VAR id="GLOBAL_PREFS_INCLUDED" value="0" />
	<VAR id="ABOUT_LMSKIN_INCLUDED" value="0" />
	<VAR id="EXPORT_INCLUDED" value="0" />


	<EXPOSED_OBJECTS object_ids="menu_lm_core.*;menu_lm_custom.*;" />
	<ACT id="OpenMenuLM" name="Open Extended Menu" script="a_lm_menu()" />
	<ACT id="OpenMenuOriginal" name="Main menu" script="OpenMenu()" />
	<ONLOAD script="@a_lm_menu = menu_lm_core.Popup; 
		@a_open_settings = ActionUnavailable;
		@a_show_skin_scanner = ActionUnavailable;
		@a_show_controls_gen = ActionUnavailable;
		@a_show_global_prefs = ActionUnavailable;
		@a_open_export = ActionUnavailable;
		@a_open_about_lmskin = ActionUnavailable;
		" />

	<EXPOSED_OBJECTS object_ids="CustomMenuUnavailable" />
	<ACT id="OpenCustomMenu" name="Open Extended Menu" script="a_custom_menu()" />
	<ONLOAD script="@a_custom_menu = CustomMenuUnavailable;" />

	<!-- ///////////////////// -->
	<!--        METERS         -->
	<!-- ///////////////////// -->

	<!-- create an element for meters box background -->
	<TEMPLATE id="LM_METERS_BOX" data="$METERS_BOX$">
	  <LM_BOX_RAW data="$data$" expand_invert="$METERS_ARE_HORIZONTAL$" />
	</TEMPLATE>

	<!-- ///////////////////// -->
	<!--      DECORATIONS      -->
	<!-- ///////////////////// -->

	<TEMPLATE id="LM_TOOLBAR_DECOR" toolbar="" opacity="1">
	  <!-- show only if decor globally enabled -->
	  <VAR id="_DECOR_DISPLAY" value="$DECOR_DISPLAY$" />
	  <R count="($SKIN_EDITABLE$)*1"><VAR id="_DECOR_DISPLAY" value="1" /></R>
	  <WIDGET id="toolbar_decor" width="100%" height="100%" opacity="$opacity$">
	    <LAYER_STACK width="100%" height="100%">
	      <R count="($_DECOR_DISPLAY$ != 0)" index_name="__dr100">
	        <LM_DECOR group="$toolbar$" />
	      </R>
	    </LAYER_STACK>
	  </WIDGET>
	  <R count="($SKIN_EDITABLE$)*1"><PARAM_LINK from="a_decor_display" to="toolbar_decor.display" /></R>
	</TEMPLATE>

	<TEMPLATE id="LM_BODY_DECOR" body1="" body2="" body3="" body4="" opacity="1">
	  <!-- show only if decor globally enabled -->
	  <VAR id="_DECOR_DISPLAY" value="$DECOR_DISPLAY$" />
	  <R count="($SKIN_EDITABLE$)*1">
	    <VAR id="_DECOR_DISPLAY" value="1" />
	  </R>
	  <WIDGET id="body_decor" width="100%" height="100%" opacity="$opacity$">
	    <LAYER_STACK width="100%" height="100%">
	      <R count="($_DECOR_DISPLAY$ != 0)" index_name="__dr101">
	        <LM_DECOR group="$body1$" />
	        <LM_DECOR group="$body2$" />
	        <LM_DECOR group="$body3$" />
	        <LM_DECOR group="$body4$" />
	      </R>
	    </LAYER_STACK>
	  </WIDGET>
	  <R count="($SKIN_EDITABLE$)*1">
	    <PARAM_LINK from="a_decor_display" to="body_decor.display" />
	  </R>
	</TEMPLATE>

	<TEMPLATE id="LM_STATUS_DECOR" status="" opacity="1">
	  <!-- show only if decor globally enabled -->
	  <VAR id="_DECOR_DISPLAY" value="$DECOR_DISPLAY$" />
	  <R count="($SKIN_EDITABLE$)*1"><VAR id="_DECOR_DISPLAY" value="1" /></R>
	  <WIDGET id="status_decor" width="100%" height="100%" opacity="$opacity$">
	    <LAYER_STACK width="100%" height="100%">
	      <R count="($_DECOR_DISPLAY$ != 0)" index_name="__dr102">
	        <LM_DECOR group="$status$" />
	      </R>
	    </LAYER_STACK>
	  </WIDGET>
	  <R count="($SKIN_EDITABLE$)*1"><PARAM_LINK from="a_decor_display" to="status_decor.display" /></R>
	</TEMPLATE>

	<!-- unparse and display decoration group -->
	<TEMPLATE id="LM_DECOR" group="" opacity="1">
	  
	  <LV id="N1" value="0" /><LV id="N2" value="0" />
	  <VAR id="V01" value="0" />
	  <ROW reflow="true" width="300">
	    <REPEAT index_list="$group$">
	      <VAR id="V$N1$$N2$" value="$index$" />
	      <LV id="N2" formula="$N2$+1" />
	      <LV id="N1" formula="($N2$ > 14)*($N1$+1) + ($N2$ &lt; 15)*($N1$)" />
	      <LV id="N2" formula="($N2$ > 14)*(0) + ($N2$ &lt; 15)*($N2$)" />
	    </REPEAT>
	  </ROW>

	  <!-- unparse: V00-V014- group settings; V10-V114, V20-V214, etc - image settings
	  [GROUP]
	  Name;Display;Opacity;Width;Height;H-Pad;V-Pad;H-Offset;V-Offset;Group-Image-Scaling;Alignment;;;;;
	  [IMAGE]
	  Filename;Display;Opacity;Width;Height;H-Pad;V-Pad;H-Offset;V-Offset;Image-Scaling;Alignment;Mode (Normal/Stretch/Repeat);;;;
	  -->
	  
	  <!-- display group if not empty and enabled -->
	  <REPEAT count="($N1$>1)*$V01$" index_name="_dummyif1">
	    <LV id="GROP" formula="$opacity$ * $V02$" /> <!-- opacity group * template -->
	    <LV id="GHA" value="center" />
	    <LV id="GVA" value="center" />
	    <R count="($V010$>=1)*($V010$&lt;=3)"><LV id="GVA" value="top" /></R>
	    <R count="($V010$>=5)*($V010$&lt;=7)"><LV id="GVA" value="bottom" /></R>
	    <R count="($V010$==1)+($V010$==7)+($V010$==8)"><LV id="GHA" value="left" /></R>
	    <R count="($V010$>=3)*($V010$&lt;=5)"><LV id="GHA" value="right" /></R>
	    
	    <WIDGET opacity="$GROP$" width="$V03$" height="$V04$" h_margin="$V05$" v_margin="$V06$" h_offset="$V07$" v_offset="$V08$" v_align="$GVA$" h_align="$GHA$" >
	      <LAYER_STACK width="100%" height="100%">
	        <REPEAT count="($N1$-1)" start="1" index_name="I">
	          <REPEAT count="($V$I$1$>0)" index_name="_dummyimen1">

	            <LV id="IA" value="$V$I$10$" /> <!-- alignment code 0 to 8 -->
	            <LV id="IVA" value="center" />
	            <LV id="IHA" value="center" />
	            <LV id="IMS" formula="$V$I$9$*$V09$" /> <!-- image scaling + group scaling -->
	            <LV id="IM" value="$V$I$11$" /> <!-- image mode: 0, 1- stretch, 2 - repeat -->
	            <LV id="ISTR" value="" />
	            <LV id="IREP" value="" />
	            <R count="($IA$>=1)*($IA$&lt;=3)"><LV id="IVA" value="top" /></R>
	            <R count="($IA$>=5)*($IA$&lt;=7)"><LV id="IVA" value="bottom" /></R>
	            <R count="($IA$==1)+($IA$==7)+($IA$==8)"><LV id="IHA" value="left" /></R>
	            <R count="($IA$>=3)*($IA$&lt;=5)"><LV id="IHA" value="right" /></R>
	            <R count="($IM$==1)"><LV id="ISTR" value="true" /></R>
	            <R count="($IM$==2)"><LV id="IREP" value="true" /></R>
	            
	            <LS width="100%" height="100%" h_margin="$V$I$5$" v_margin="$V$I$6$">
	              <IMAGE image="$LM_DIR$$DECOR_SUBDIR$/$V$I$0$" opacity="$V$I$2$" width="$V$I$3$" height="$V$I$4$" h_offset="$V$I$7$" v_offset="$V$I$8$" image_scaling="$IMS$" stretch="$ISTR$" repeat="$IREP$" v_align="$IVA$" h_align="$IHA$" />
	            </LS>
	          </REPEAT>
	        </REPEAT>
	      </LAYER_STACK>
	    </WIDGET>
	  </REPEAT>
	</TEMPLATE>


	<!-- ELEMENTS TO MAKE PARTS OF SKIN INDEPENDENT -->
	<PARAM id="instant_link_enabled" type="boolean" default="false" exposed="true" />

	<ESTRING id="a_font_names" default="$FONT_NAMES_COMMON$" />
	<EPARAM id="a_font_names_count" min="0" max="250" default="0" />

	<EPARAM id="a_toolbar_display" min="0" max="1" default="1" />
	<EPARAM id="a_toolbar_invalidate" min="0" max="1000000000" default="0" />
	<STRING id="a_toolbar_bg_innerKUIML" default="&lt;SKIN width='100%' height='100%'>&lt;/SKIN>" />
	<EPARAM id="a_toolbar_area_min_height" max="1000" default="23" />
	<EPARAM id="a_toolbar_v_offset" min="-100" max="100" default="0" />
	<EPARAM id="a_toolbar_pad_h" max="100" default="0" />
	<EPARAM id="a_toolbar_pad_v" max="100" default="0" />
	<ESTRING id="a_toolbar_innerKUIML" default="&lt;SKIN> &lt;/SKIN>" />
	<ESTRING id="a_toolbar_infolink" default="http://www.bluecataudio.com/" />
	<ESTRING id="a_toolbar_items_left" default="MENU;ZOOM" />
	<ESTRING id="a_toolbar_items_center" default="TITLE;PRESETS" />
	<ESTRING id="a_toolbar_items_right" default="UNDO;REDO;INFO;PRESETMENU;SCRIPTMENU;SKINMENU" />
	<ESTRING id="a_toolbar_max_in_row" default="10;2;10" />
	<ESTRING id="a_toolbar_icons" default="18;#333333;1;8;4;0" />
	<ESTRING id="a_toolbar_title" default="500;10;0;0;0;1;;; ;Arial;+2;bold;-;#000000" />
	<ESTRING id="a_toolbar_title_text" default="" />
	<ESTRING id="a_toolbar_title_fixed_text" default="" />
	<IF_NOT_EXPORTED>
		<!-- add text to title for Skin scanner, etc -->
		<STRING id="str_skin_scanner_enabled" default=" | Skin scanner" />
		<STRING id="str_controls_gen_enabled" default=" | Controls generator" />
		<STRING id="str_toolbar_title_after" default="" />
			<STRING_LINK id="slink_title_no_addon" from="str_empty" to="str_toolbar_title_after" enabled="true" />
			<STRING_LINK id="slink_title_addon_ss" from="str_skin_scanner_enabled" to="str_toolbar_title_after" enabled="false" />
			<STRING_LINK id="slink_title_addon_cg" from="str_controls_gen_enabled" to="str_toolbar_title_after" enabled="false" />
		<FORMULA_PARAM id="fp_show_default_title" formula="(skin_scanner_enabled &lt; 0.5)*(controls_gen_enabled &lt; 0.5)" />
		<PARAM_LINK from="skin_scanner_enabled" to="slink_title_addon_ss.enabled" />
		<PARAM_LINK from="controls_gen_enabled" to="slink_title_addon_cg.enabled" />
		<PARAM_LINK from="fp_show_default_title" to="slink_title_no_addon.enabled" />

		<STRING_MULTI_LINK from="$script_name_string$;str_toolbar_title_after" to="a_toolbar_title_text" />
	</IF_NOT_EXPORTED>
	<IF_EXPORTED>
		<STRING_LINK from="$script_name_string$" to="a_toolbar_title_text" />
	</IF_EXPORTED>



	<ESTRING id="a_toolbar_presets" default="130;20;3;3;0;0;0; ;14;#FFFFFF;0.8;-3;1 ;;;;;;;; ;Arial;-2;-;-;#FFFFFF;;0;;;0;1;-2;1;0;7;;;;;#333333;0;31.67;#9F9F9F;0.28;65;#FFFFFF;0.45;0" />

	<STRING id="a_toolbar_font_size" default="+1" />
	<STRING id="a_toolbar_text_color" default="" />
	<EPARAM id="a_toolbar_icons_size" min="0" max="48" default="16" />
	<ESTRING id="a_toolbar_icons_color" default="" />
	<EPARAM id="a_toolbar_icons_opacity" default="1" />
	<ESTRING id="a_toolbar_font_size2" default="-2" />
	<ESTRING id="a_toolbar_text_color2" default="" />
	<EPARAM id="a_toolbar_icons_size2" max="48" default="14" />
	<ESTRING id="a_toolbar_icons_color2" default="" />
	<EPARAM id="a_toolbar_icons_opacity2" default="1" />
	<EPARAM id="a_toolbar_inner_pad_h" max="100" default="0" />
	<EPARAM id="a_toolbar_inner_pad_v" max="100" default="0" />

	<STRING id="a_body_innerKUIML" default="&lt;SKIN width='100%' height='100%'>&lt;/SKIN>" />
	<ESTRING id="a_body_auto_layout" />
	<ESTRING id="a_body_auto_layout_theme" />

	<!-- params to calculate number of auto-layout controls -->
	<!--
	<EPARAM id="AL_switches_count" max="100" />
	<EPARAM id="AL_inputs_count" max="100" />
	<EPARAM id="AL_outputs_count" max="100" />
	-->

	<STRING id="a_body_bg_innerKUIML" default="&lt;SKIN width='100%' height='100%'>&lt;/SKIN>" />
	<EPARAM id="a_custom_kuiml_available" default="0" />
	<IF_NOT_EXPORTED>
		<EPARAM id="a_ignore_custom_kuiml" max="1" default="0" persistent="true" />
	</IF_NOT_EXPORTED>
	<IF_EXPORTED>
		<EPARAM id="a_ignore_custom_kuiml" max="1" default="0" />
	</IF_EXPORTED>
	<FPARAM id="a_ignore_custom_kuiml_checkbox" formula="a_custom_kuiml_available*a_ignore_custom_kuiml" />
	<EPARAM id="a_body_v_align" max="2" default="0" />
	<EPARAM id="a_body_pad_h" max="100" default="100" />
	<EPARAM id="a_body_pad_v" max="100" default="100" />
	<EPARAM id="a_body_min_w" max="1920" default="300" />
	<EPARAM id="a_body_min_h" max="1080" default="100" />
	<STRING id="a_body_font" default="+0" />
	<EPARAM id="body_loaded" max="100000" />

	<EPARAM id="a_meters_display" max="3" default="3" />
	<EPARAM id="a_meters_input_channels" max="10" default="8" />
	<EPARAM id="a_meters_output_channels" max="10" default="8" />

	<EPARAM id="a_meters_placement" max="1" default="0" />
	<EPARAM id="a_meters_min_width" min="1" max="250" default="16" />
	<EPARAM id="a_meters_min_width_single" min="1" max="50" default="8" />
	<EPARAM id="a_meters_pad_v" max="100" default="0" />
	<EPARAM id="a_meters_pad_h" max="100" default="0" />
	<EPARAM id="a_meters_v_offset" min="-100" max="100" default="0" />
	<EPARAM id="a_meters_box_pad_a" max="100" default="0" />
	<EPARAM id="a_meters_box_pad_b" max="100" default="0" />
	<EPARAM id="a_meters_box_offset" min="-100" max="100" default="0" />
	<EPARAM id="a_meters_box_spacing" min="0" max="20" default="0" />

	<EPARAM id="a_meters_group_spacing" max="100" default="0" />
	<EPARAM id="a_meters_group_input" max="8" default="0" />
	<EPARAM id="a_meters_group_output" max="8" default="0" />

	<EPARAM id="a_meters_invalidate" min="0" max="10000000000" default="0" />
	<REQUIRED_OBJECTS object_ids="a_meters_invalidate" />
	<ESTRING id="a_meters_box_widget_innerKUIML" default="&lt;SKIN width='100%' height='100%'>&lt;COMMON_SCRIPTS cnvn='4900' cnvp='MBW' />&lt;/SKIN>" />

	<EPARAM id="a_status_invalidate" min="0" max="1000000" default="0" />
	<EPARAM id="a_status_display" min="0" max="2" default="1" />
	<PARAM id="_a_log_unusual_text" type="boolean" default="false" exposed="true" />
	<PARAM id="_a_do_show_status_area" type="boolean" default="false" exposed="true" />
	<EPARAM id="a_status_area_min_height" max="100" default="22" />
	<STRING id="a_status_bg_innerKUIML" default="&lt;SKIN width='100%' height='100%'>&lt;/SKIN>" />
	<EPARAM id="a_status_v_offset" min="-100" max="100" default="0" />
	<EPARAM id="a_status_pad_h" max="100" default="0" />
	<ESTRING id="a_status_content_params" default=";0;-;-;#FFFFFF;;5" />
	<ESTRING id="a_status_layout_params" default="0;0;135;1;150" />
	
	<ESTRING id="a_status_flex_cells" default="1;0;0" />
	<ESTRING id="a_status_logline_innerKUIML" />
	<ESTRING id="a_status_innerKUIML" default="&lt;SKIN> &lt;/SKIN>" />
	<ESTRING id="a_status_items_left" default="LOG_LINE" />
	<ESTRING id="a_status_items_center" default="" />
	<ESTRING id="a_status_items_right" default="" />
	<ESTRING id="status_custom_text" default="" />
	<ESTRING id="status_custom_text_prev" default="" />
	<REQUIRED_OBJECTS object_ids="status_custom_text;status_custom_text_prev" />
	<ACTION id="OpenLogFile" type="OpenUrl" url_string_id="$log_file_path$"  application_string_id="open_log_app" name="Open log file" />

	<EPARAM id="a_decor_display" default="1" />
	
	<ESTRING id="kuiml_log" default="" />
	<REQUIRED_OBJECTS object_ids="kuiml_log;kuiml_log.*" />
	<EXPOSED_OBJECTS object_ids="kuiml_log" />

	<ESTRING id="a_lm_lic" default="" />
	<REQUIRED_OBJECTS object_ids="a_lm_lic.*" />

	<EPARAM id="skin_scanner_enabled" default="0" />
	<EPARAM id="skin_scanner_init_times" default="0" />
	<ESTRING id="SKIN_SCANNER" default="" />
	<ESTRING id="SKIN_SCANNER_MODE" default="menu::Knobs" />
	<EPARAM id="controls_gen_enabled" default="0" />
	<ESTRING id="CONTROLS_GEN_DRAFT" default="" />
	<ESTRING id="CONTROLS_GEN_PARAMS" default="" />
	<IF_NOT_EXPORTED>
		<PERSISTENT_OBJECTS object_ids="skin_scanner_enabled;SKIN_SCANNER;SKIN_SCANNER_MODE;controls_gen_enabled;CONTROLS_GEN_DRAFT;CONTROLS_GEN_PARAMS" />
	</IF_NOT_EXPORTED>
	<EPARAM id="script_kuiml_loaded_ok" max="100000" />
	
	
	
	<!-- status animation timer and routine -->
	<TIMER refresh_time_ms="40" id="st_anim_timer" enabled="false" />
	<EPARAM id="a_status_custom_text_opacity" default="1" />
	<EPARAM id="st_anim_timer_passed" default="0" max="1000000" />
	<EPARAM id="st_anim_timer_end" default="0" max="1000000" />
	<EXPOSED_OBJECTS object_ids="st_anim_timer.enabled;a_status_custom_text_opacity" />
	<SCRIPT script="bool st_anim_timer_fadeout_started = false;" />
	<ACTION_TRIGGER event_id="st_anim_timer.elapsed" script="
		float interval = 40;
		if (st_anim_timer_passed == 0) { st_anim_timer_fadeout_started = false;	}
		st_anim_timer_passed = interval + st_anim_timer_passed;
		if ((a_status_custom_text_opacity &lt; 1) and (!st_anim_timer_fadeout_started)) {
			a_status_custom_text_opacity = a_status_custom_text_opacity + 0.3;
		}
		if ((!st_anim_timer_fadeout_started) and (st_anim_timer_passed >= (st_anim_timer_end - interval*5)) and (st_anim_timer_passed &lt; (0+st_anim_timer_end))) { 
			st_anim_timer_fadeout_started = true; 
		}
		if (st_anim_timer_fadeout_started) {
			a_status_custom_text_opacity = a_status_custom_text_opacity - 0.2;
		}
		if (st_anim_timer_passed >= (0+st_anim_timer_end)) {
			if (st_anim_timer_fadeout_started) {
				status(&quot;&quot; + status_custom_text_prev);
				status_custom_text_prev = &quot;&quot;;
			}
			st_anim_timer_fadeout_started = false;
			if (a_status_custom_text_opacity &lt; 1) {
				a_status_custom_text_opacity = a_status_custom_text_opacity + 0.3;
			} else {
				st_anim_timer.enabled = false;
			}
		}
		" requires="st_anim_timer.enabled;on_st_anim_timer" />
	

	<!-- ###################################### -->
	<!-- VARS TEMPLATES / DEFAULT VARS VALUES  -->
	<!-- ###################################### -->

	<TEMPLATE id="LOAD_FONTS" dir="$LM_DIR$$FONTS_SUBDIR$/" files="" ignore_missing="true">
	  <VAR id="LOAD_FONTS_DIR" value="$dir$" />
	  <VAR id="LOAD_FONTS_FILES" value="$files$" />
	  <REPEAT index_list="$LOAD_FONTS_FILES$">
	    <LOAD_FONT file="$LOAD_FONTS_DIR$$index$" ignore_missing="$ignore_missing$" />
	  </REPEAT>
	</TEMPLATE>

	<TEMPLATE id="VARS_TOOLBAR" display="1" min_height="25" pad_h="5" pad_v="0" v_offset="0" items_left="MAINMENU;ZOOM" items_center="TITLE" items_right="SCRIPTMENU;SKINMENU" max_in_row="5;1;3" bg="0;0.97;0;1;0;0;;;; ;#404040;1;33 ;#222222;1;100 ;#2C3A47;0.45;100 ;#1A1A1A;1;25" _auto_color="1;-30;0;-12;1;-38;1;0;1;0;1;0" icons="16;#FFFFFF;1;1;0;0" title="500;50;4;0;0;-1;1;; ;;+1;bold;-;#FFFFFF;" presets="130;20;3;3;0;0;0; ;14;#FFFFFF;0.8;-3;1 ;;;;;;;; ;Arial;-2;-;-;#FFFFFF;;0;;;0;1;-2;1;0;7; ;;;;#333333;0;31.67;#9F9F9F;0.28;65;#FFFFFF;0.45;0" title_text="" info_link="http://bluecataudio.com/" title_action_num="3" title_cus_action ="OpenMenu" title_cus_action_hint="Open menu" skinmenu_action="skin_menu.Popup" skinmenu_action_hint="Current script menu" mainmenu_action="OpenMenu" mainmenu_action_hint="Open menu" settings_action="" settings_action_hint="Settings" scriptmenu_action="ShowDefaultScriptsMenu" scriptmenu_action_hint="Select script" custom_items="" custom_items_names="">

	  <VAR id="TOOLBAR_DISPLAY" value="$display$" />
	  <VAR id="TOOLBAR_DISPLAY_BOOL" value="false" />
	  <IF condition="$display$"><VAR id="TOOLBAR_DISPLAY_BOOL" value="true" /></IF>
	  <VAR id="TOOLBAR_MIN_HEIGHT" value="$min_height$" />
	  <VAR id="TOOLBAR_PAD_H" value="$pad_h$" />
	  <VAR id="TOOLBAR_PAD_V" value="$pad_v$" />
	  <VAR id="TOOLBAR_V_OFFSET" value="$v_offset$" />
	  <VAR id="TOOLBAR_BG" value="$bg$" />
	  <VAR id="TOOLBAR_ITEMS_LEFT" value="$items_left$" />
	  <VAR id="TOOLBAR_ITEMS_CENTER" value="$items_center$" />
	  <VAR id="TOOLBAR_ITEMS_RIGHT" value="$items_right$" />
	  <VAR id="TOOLBAR_MAX_IN_ROW" value="$max_in_row$" />
	  <VAR id="TOOLBAR_ICONS" value="$icons$" />
	  <VAR id="TOOLBAR_TITLE" value="$title$" />
	  <VAR id="TOOLBAR_TITLE_TEXT" value="$title_text$" />
	  <VAR id="TOOLBAR_PRESETS" value="$presets$" />
	  <VAR id="TOOLBAR_INFO_LINK" value="$info_link$" />
	  <VAR id="TOOLBAR_INFO_LINK_HINT" value="Display 'About'" />
	  <REPEAT index_list="$TOOLBAR_INFO_LINK$">
	    <VAR id="TOOLBAR_INFO_LINK_HINT" value="$TOOLBAR_INFO_LINK$" />
	  </REPEAT>

	  <VAR id="TOOLBAR_CUSTOM_ITEMS" value="$custom_items$" />
	  <VAR id="TOOLBAR_CUSTOM_ITEMS_NAMES" value="$custom_items_names$" />

	  <VAR id="TOOLBAR_TITLE_ACTION_NUM" value="$title_action_num$" />

	  <VAR id="TOOLBAR_TITLE_CUS_ACTION" value="$title_cus_action$" />
	  <VAR id="TOOLBAR_TITLE_CUS_ACTION_HINT" value="$title_cus_action_hint$" />

	  <VAR id="TOOLBAR_SKINMENU_ACTION" value="$skinmenu_action$" />
	  <VAR id="TOOLBAR_SKINMENU_ACTION_HINT" value="$skinmenu_action_hint$" />

	  <VAR id="TOOLBAR_SCRIPTMENU_ACTION" value="$scriptmenu_action$" />
	  <VAR id="TOOLBAR_SCRIPTMENU_ACTION_HINT" value="$scriptmenu_action_hint$" />

	  <VAR id="TOOLBAR_MAINMENU_ACTION" value="$mainmenu_action$" />
	  <VAR id="TOOLBAR_MAINMENU_ACTION_HINT" value="$mainmenu_action_hint$" />

	  <VAR id="TOOLBAR_SETTINGS_ACTION" value="$settings_action$" />
	  <VAR id="TOOLBAR_SETTINGS_ACTION_HINT" value="$settings_action_hint$" />
	  <VAR id="TOOLBAR_SETTINGS_ACTION_IS_SET" value="0" />
	  <REPEAT index_list="$TOOLBAR_SETTINGS_ACTION$">
	    <VAR id="TOOLBAR_SETTINGS_ACTION_IS_SET" value="1" />
	  </REPEAT>

	</TEMPLATE>
	
	<!-- body shading params -->
	<EPARAM id="body_shading_changed" max="999999999" default="0" />
  <ESTRING id="body_shading_data" />
  <EXPOSED_OBJECTS object_ids="body_shading_changed.*;body_shading_data.*" />

	<TEMPLATE id="VARS_BODY" font="Arial;+0;-;-;#000000;" pad_h="10" pad_v="13" min_width="330" min_height="250" v_align="center" bg="0;1;-5;0;0;0;;;; ;#BBBBBB;1;28 ;#ACACAC;1;100 ;#F8F8F8;1;10 ;#676767;1;100" shading_data="0;0;0;-167;434;-167;0;33;0" _auto_color="1;-15;1;61;1;-84" static_file="" auto_layout="0;graphics_layout.kuiml;0;6;0" auto_layout_theme="101;142;10;10;;;#FFFFFF;0.4;#FFFFFF;0.2;3;5;3;10;15;35;;0;-3;;;;;;;;;;;;LMR_KNOB_BLACK_B;1;;;#DFDFDF;0.85;;;;;;;LMR_VSWITCH_SOLID;1;;;;;;;;;;;LMR_METER_LED;1;#49B1F4;;;;;;;;;;" include_lmr_scripts="0">
	  <VAR id="BODY_FONT" value="$font$" />
	  <VAR id="BODY_PAD_H" value="$pad_h$" />
	  <VAR id="BODY_PAD_V" value="$pad_v$" />
	  <VAR id="BODY_MIN_HEIGHT" value="$min_height$" />
	  <VAR id="BODY_MIN_WIDTH" value="$min_width$" />
	  <R count="($LM_SKIN_RESIZEABLE$ > 0)">
	  	<VAR id="BODY_MIN_HEIGHT" value="1" />
	  	<VAR id="BODY_MIN_WIDTH" value="1" />
	  </R>
	  <VAR id="BODY_V_ALIGN" value="$v_align$" />
	  <VAR id="BODY_BG" value="$bg$" />
	  <VAR id="BODY_SHADING_DATA" value="$shading_data$" />
	  <VAR id="BODY_STATIC_FILE" value="$static_file$" />
	  <VAR id="BODY_STATIC_FILE_SET" value="false" />
	  <REPEAT index_list="$static_file$">
	    <VAR id="BODY_STATIC_FILE_SET" value="true" />
	  </REPEAT>
	  <VAR id="BODY_INCLUDE_LMR_SCRIPTS" formula="($include_lmr_scripts$)" />
	  <VAR id="BODY_AUTO_LAYOUT" value="$auto_layout$" />
	  <VAR id="BODY_AUTO_LAYOUT_THEME" value="$auto_layout_theme$" />
	  <ONLOAD script="a_body_auto_layout = &quot;$BODY_AUTO_LAYOUT$&quot;;" />
	  <ONLOAD script="a_body_auto_layout_theme = &quot;$BODY_AUTO_LAYOUT_THEME$&quot;;" />
	</TEMPLATE>

	<TEMPLATE id="VARS_METERS" display="3" input_channels="2" output_channels="2" min_width="10" min_width_single="6" pad_h="10" pad_v="22" v_offset="0" grouping="2;2;2.8" placement="0" box_pos="3;5;0;0;0" box="1;0.75;15;0.8;0;7.2;;;;;#0B0B0B;1;0;#040404;1;100;#FFFFFF;1;0" orientation="0" render="1;-60;0;;;0.27;30;3;300;5;2.1;0;1.2;0;;0;0;0;0;;;;;;;;;;;;#EA2027;1;0;#E5AB2B;1;15;#2ECC71;1;30;#FFFFFF;1;50;#FFFFFF;0;50;#000000;0;0;" hide_inactive="1">
	  <VAR id="METERS_DISPLAY" value="$display$" />
	  <VAR id="METERS_DISPLAY_input" value="false" />
	  <VAR id="METERS_DISPLAY_output" value="false" />
	  <REPEAT count="($display$==1)+($display$==3)" index_name="__dummyl1_">
	    <VAR id="METERS_DISPLAY_input" value="true" />
	  </REPEAT>
	  <REPEAT count="($display$==2)+($display$==3)" index_name="__dummyl2_">
	    <VAR id="METERS_DISPLAY_output" value="true" />
	  </REPEAT>
	  <VAR id="METERS_INPUT_CHANNELS" value="$input_channels$" />
	  <VAR id="METERS_OUTPUT_CHANNELS" value="$output_channels$" />
	  <VAR id="METERS_input_CHANNELS" value="$input_channels$" />
	  <VAR id="METERS_output_CHANNELS" value="$output_channels$" />
	  <VAR id="METERS_ORIENTATION" value="$orientation$" />
	  <VAR id="METERS_ARE_HORIZONTAL" formula="($METERS_ORIENTATION$==1)+($METERS_ORIENTATION$==3)" />
	  <VAR id="METERS_PLACEMENT" formula="$placement$" />
	  <VAR id="METERS_PLACEMENT_LAYOUT" value="row" />
	  <REPEAT count="($METERS_ARE_HORIZONTAL$==1)*($METERS_PLACEMENT$==0)" index_name="__dummyl1_">
	    <VAR id="METERS_PLACEMENT_LAYOUT" value="column" />
	  </REPEAT>
	  <REPEAT count="($METERS_PLACEMENT$==1)" index_name="__dummyl2_">
	    <VAR id="METERS_PLACEMENT_LAYOUT" value="layer_stack" />
	  </REPEAT> 

	  <VAR id="METERS_MIN_WIDTH" value="$min_width$" />
	  <VAR id="METERS_MIN_WIDTH_SINGLE" value="$min_width_single$" />
	  <VAR id="METERS_PAD_H" value="$pad_h$" />
	  <VAR id="METERS_PAD_H_half" formula="$pad_h$/2" />
	  <VAR id="METERS_PAD_H_half_input" formula="$pad_h$/2" />
	  <VAR id="METERS_PAD_H_half_output" formula="$pad_h$/-2" />
	  <VAR id="METERS_PAD_V" value="$pad_v$" />
	  <VAR id="METERS_V_OFFSET" value="$v_offset$" />
	  <VAR id="METERS_HIDE_INACTIVE" value="$hide_inactive$" />

	  <!-- parsing box params -->
	  <VAR id="METERS_BOX" value="$box$" />

	  <!-- parsing grouping -->
	  <VAR id="N" value="0" />
	  <VAR id="T0" value="0" /><VAR id="T1" value="0" /><VAR id="T2" value="0" />
	  <REPEAT index_list="$box_pos$">
	    <VAR id="T$N$" value="$index$" /><NEXT_N />
	  </REPEAT>
	  <VAR id="METERS_BOX_PAD_A" value="$T0$" />
	  <VAR id="METERS_BOX_PAD_B" value="$T1$" />
	  <VAR id="METERS_BOX_OFFSET" value="$T2$" />
	  <!-- 3 reserved -->
	  <VAR id="METERS_BOX_SPACING" value="$T4$" />

	  <!-- parsing grouping -->
	  <VAR id="N" value="0" />
	  <VAR id="T0" value="0" /><VAR id="T1" value="0" />
	  <REPEAT index_list="$grouping$">
	    <VAR id="T$N$" value="$index$" /><NEXT_N />
	  </REPEAT>
	  <VAR id="METERS_GROUP_input" value="$T0$" />
	  <VAR id="METERS_GROUP_output" value="$T1$" />
	  <VAR id="METERS_GROUP_SPACING" value="$T2$" />
	  <!-- apply to params to be able to change on the fly -->
	  <ONLOAD script="a_meters_group_spacing = $METERS_GROUP_SPACING$; a_meters_group_input = $METERS_GROUP_input$; a_meters_group_output = $METERS_GROUP_output$;" />

	  <!-- parsing $render$ params -->
	  <VAR id="METERS_RENDER" value="$render$" />
	  <ONLOAD script="LM::meters_prepareParams(&quot;$render$&quot;, LM::meterRenderParams)" />
	  <!--
		meterRenderParams structure (params follow the same order as in $render$ string):
		(0-4) style;min_level;max_level;-;-;
		(5-9) level_bg_opacity;desired_leds_count;min_led_size;max_led_size;led_spacing
		(10-14) hold_line_width;hold_line_use_color;inner_pad_h;inner_pad_v;-;
		(15-19) stroke_width;stroke_expand;stroke_round;-;-;
		(20-29) -;-;-;-;-;-;-;-;-;-;
		(30-...) colors[color[r,g,b];opacity;shift];
		-->

	</TEMPLATE>

	<TEMPLATE id="VARS_STATUS" display="2" min_height="23" pad_h="8" v_offset="0" bg="0;1;0;1;3;0;;;; ;#404040;1;0 ;#373737;1;100 ;#5A5A5A;1;3 ;#222222;1;100"  _auto_color="1;-9;1;26;1;-30" flex_cells="1;0;0" content="Arial;-2;-;-;#FFFFFF;;5;5.33;;0;1;12;1;-1;1;0;#BBBBBB;1;#FFCC00;0.9;#FF774E;1;#FFFFFF;0.8;|;#686868;0.63;2;21;2;0;;;1;1" layout="0;0;135;1;150" items_left="ICON;LOG_LINE" items_center="" items_right="SEPARATOR;SAMPLERATE;BLOCKSIZE" custom_text="" custom_items="" custom_items_names="" corner_resizer_space="7">
	  <VAR id="STATUS_DISPLAY" value="$display$" />
	  <VAR id="STATUS_DISPLAY_BOOL" value="false" />
	  <REPEAT count="($STATUS_DISPLAY$>1)" index_name="__dumsd0l2_">
	    <VAR id="STATUS_DISPLAY_BOOL" value="true" />
	  </REPEAT> 
	  <VAR id="STATUS_CONTENT" value="$content$" />
	  <VAR id="STATUS_LAYOUT" value="$layout$" />
	  <VAR id="STATUS_MIN_HEIGHT" value="$min_height$" />
	  <VAR id="STATUS_PAD_H" value="$pad_h$" />
	  <VAR id="STATUS_V_OFFSET" value="$v_offset$" />
	  <VAR id="STATUS_BG" value="$bg$" />
	  <VAR id="STATUS_BG_KUIML" value="$bg_kuiml$" />

	  <VAR id="STATUS_ITEMS_LEFT" value="$items_left$" />
	  <VAR id="STATUS_ITEMS_CENTER" value="$items_center$" />
	  <VAR id="STATUS_ITEMS_RIGHT" value="$items_right$" />
	  
	  <VAR id="STATUS_CUSTOM_ITEMS" value="$custom_items$" />
	  <VAR id="STATUS_CUSTOM_ITEMS_NAMES" value="$custom_items_names$" />

	  <VAR id="STATUS_FLEX_CELLS" value="$flex_cells$" />
		
		<R index_list="$custom_text$">
	  	<ONLOAD script="status_custom_text = &quot;$custom_text$&quot;;" />
	  </R>
	
		<VAR id="STATUS_CORNER_RESIZER_SPACE" value="$corner_resizer_space$" />
	</TEMPLATE>

	<TEMPLATE id="VARS_DECOR" toolbar="" body1="" body2="" body3="" body4="" status="" display="0" opacity="1">
	  <VAR id="DECOR_TOOLBAR" value="$toolbar$" />
	  <VAR id="DECOR_BODY1" value="$body1$" />
	  <VAR id="DECOR_BODY2" value="$body2$" />
	  <VAR id="DECOR_BODY3" value="$body3$" />
	  <VAR id="DECOR_BODY4" value="$body4$" />
	  <VAR id="DECOR_STATUS" value="$status$" />
	  <VAR id="DECOR_DISPLAY" value="$display$" />
	  <VAR id="DECOR_OPACITY" value="$opacity$" />
	  <!-- 
	  <LV id="N" value="0" />
	  <R index_list="$global$">
	    <VAR id="GV$N$" value="$index$" />
	    <LV id="N" formula="$N$+1" />
	  </R>
	-->
	  
	</TEMPLATE>


	<!-- RESIZEABLE SKIN SUPPORT -->
	<VAR id="LM_SKIN_RESIZEABLE" value="0" />
	<VAR id="RE100" value="" /> <!-- RE100 is 100% when skin is resizeable, otherwise empty -->
	<EPARAM id="lm_skin_resizeable" />

	<!-- initialize parameters for resizeable skin support -->
	<TEMPLATE id="MAKE_RESIZEABLE" enabled="true" min="200x100" default="600x400" max="1600x900" root_cell_id="root_cell" proportional="false">
		<R count="$enabled$">
			<!-- init scripts variables, parse min, max, default -->
			<SCRIPT script="
			  float $root_cell_id$_old_width, $root_cell_id$_old_height;

			  float $root_cell_id$_min_width = parseFloat(&quot;$min$&quot;.split(&quot;x&quot;)[0]);
			  float $root_cell_id$_min_height = parseFloat(&quot;$min$&quot;.split(&quot;x&quot;)[1]);

			  float $root_cell_id$_max_width = parseFloat(&quot;$max$&quot;.split(&quot;x&quot;)[0]);
			  float $root_cell_id$_max_height = parseFloat(&quot;$max$&quot;.split(&quot;x&quot;)[1]);

			  float $root_cell_id$_default_width = parseFloat(&quot;$default$&quot;.split(&quot;x&quot;)[0]);
			  float $root_cell_id$_default_height = parseFloat(&quot;$default$&quot;.split(&quot;x&quot;)[1]);

			  bool $root_cell_id$_resize_proportional = $proportional$;
			  " />

			<!-- way to remember window size -->
			<EPARAM id="$root_cell_id$_width" persistent="true" max="4000" />
			<EPARAM id="$root_cell_id$_height" persistent="true" max="4000" />
			<PARAM_LINK from="$root_cell_id$.width" to="$root_cell_id$_width" enabled="false" id="lnk_$root_cell_id$_width" />
			<PARAM_LINK from="$root_cell_id$.height" to="$root_cell_id$_height" enabled="false" id="lnk_$root_cell_id$_height" />
			
			<!-- when window loads, restore last size and enable back links -->
			<ONLOAD script="
				/* set param flag for misc scripts */
				lm_skin_resizeable = 1;
			  /* if saved previous window size */
			  if (($root_cell_id$_width >= $root_cell_id$_min_width) and ($root_cell_id$_height >= $root_cell_id$_min_height)) {
			    root_cell.width = $root_cell_id$_width;
			    root_cell.height = $root_cell_id$_height;
			  } else {
			    root_cell.width = $root_cell_id$_default_width;
			    root_cell.height = $root_cell_id$_default_height;
			  }
			  lnk_$root_cell_id$_width.enabled = true;
			  lnk_$root_cell_id$_height.enabled = true;
			  " requires="lm_skin_resizeable;$root_cell_id$_width;$root_cell_id$_height;lnk_$root_cell_id$_width.enabled;lnk_$root_cell_id$_height.enabled" />

			<VAR id="LM_SKIN_RESIZEABLE" value="1" />
			<VAR id="RE100" value="100%" />
			
			<VAR id="BODY_MIN_HEIGHT" value="1" />
		  <VAR id="BODY_MIN_WIDTH" value="1" />

			<REQUIRED_OBJECTS object_ids="root_cell.*" />
			<EXPOSED_OBJECTS object_ids="root_cell.*" />
		</R>
	</TEMPLATE>

	<!-- a helper for resizeable skin (corner resizer) -->
	<TEMPLATE id="CORNER_RESIZER" root_cell_id="root_cell"  color="#777777" opacity="1" size="16" >

		<LV id="elemid" script="if (&quot;$id$&quot; == &quot;$&quot;+&quot;id$&quot;) return &quot;corner_resizer&quot;; else return &quot;$id$&quot;;" />

	  <LS id="$elemid$" v_position="0" h_position="0">
	    
	    <R count="$LM_SKIN_RESIZEABLE$">
	    	<SVG_ICON_CORNER_RESIZE color="$color$" opacity="$opacity$" size="$size$" />
		    <XYZ_PARAM_SCRATCH_PAD x_param_id="$elemid$_drag_x" y_param_id="$elemid$_drag_y" z_param_id="$elemid$_drag_z" width="100%" height="100%" cursor="$CUR_SIZE_UP_LEFT$" y_pixel_range="4001" x_pixel_range="4001" />
		    <!-- move icon to the right bottom -->
		    <PARAM_LINK from="$root_cell_id$.width" to="$elemid$.h_offset" formula="x-$size$" />
		    <PARAM_LINK from="$root_cell_id$.height" to="$elemid$.v_offset" formula="x-$size$" />

		    <!-- params to make XYZ_PARAM_SCRATCH_PAD work -->
		    <EPARAM id="$elemid$_drag_x" min="-2000" max="2000" />
		    <EPARAM id="$elemid$_drag_y" min="-2000" max="2000" />
		    <EPARAM id="$elemid$_drag_z" min="-2000" max="2000" default="0.5" />

		    <!-- when dragging starts -->
		    <ONCHANGE o="$elemid$_drag_x.capturing" condition="$elemid$_drag_x.capturing == 1" script="
		      $root_cell_id$_old_width=root_cell.width;
		      $root_cell_id$_old_height=root_cell.height;" requires="$elemid$_drag_x;$elemid$_drag_y;$root_cell_id$.width;$root_cell_id$.height" />

		    <!-- when dragging goes on -->
		    <ONCHANGE o="$elemid$_drag_x;$elemid$_drag_y" condition="$elemid$_drag_x.capturing == 1" script="
		      float new_height = $root_cell_id$_old_height - $elemid$_drag_y;
		      float new_width = $root_cell_id$_old_width + $elemid$_drag_x;
		      if (new_height &lt; $root_cell_id$_min_height) new_height = $root_cell_id$_min_height;
		      if (new_width &lt; $root_cell_id$_min_width) new_width = $root_cell_id$_min_width;
		      if (new_height > $root_cell_id$_max_height) new_height = $root_cell_id$_max_height;
		      if (new_width > $root_cell_id$_max_width) new_width = $root_cell_id$_max_width;
					/* if proportional */
		      if ($root_cell_id$_resize_proportional) {
						float prop_w = new_width/$root_cell_id$_default_width;
						float prop_h = new_height/$root_cell_id$_default_height;
						if (prop_w >= prop_h) {
							new_height = $root_cell_id$_default_height*prop_w;
						} else {
							new_width = $root_cell_id$_default_width*prop_h;
						}
		      }
		      $root_cell_id$.height = new_height;
		      $root_cell_id$.width = new_width; " />

		    <!-- when dragging stops -->
		    <ONCHANGE o="$elemid$_drag_x.capturing" condition="$elemid$_drag_x.capturing == 0" script="
		      $elemid$_drag_x=0;$elemid$_drag_y=0;" async="true" />

		    <!-- right click detection (restore default size) -->
		    <PARAM_LINK from="window.loaded" to="$elemid$_drag_z.default" reverse="true" />
		    <ONCHANGE o="$elemid$_drag_z" script="
		      $elemid$_drag_z = 0.1; 
		      $root_cell_id$.width = $root_cell_id$_default_width;
		      $root_cell_id$.height = $root_cell_id$_default_height;
		    " condition="(window.loaded==1)*($elemid$_drag_z==0)" />
		  </R>
	  </LS>
	</TEMPLATE>
	
	<!-- RENDER SETTINGS -->

	<!-- param to keep tracking of light source changes -->
	<EPARAM id="render_settings_changed" max="999999999" default="0" />
	<ESTRING id="render_settings_data" />
	<EXPOSED_OBJECTS object_ids="render_settings_changed.*;render_settings_data.*" />

	<!-- var to keep added light sources and to pass them to script -->
	<VAR id="RENDER_SETTINGS_DATA" value="" />
	
	<!-- legacy way of adding light sources (still works, can be used for adding more than 3 default sources) -->
	<TEMPLATE id="LIGHT_SOURCE" index="0" enabled="1" preview="0" angle="12" alt="75" distance="650" size="100" intensity="0.5" color="#FFFFFF" x="" y="" z="" rel_index="-1" >
	  <VAR id="RENDER_SETTINGS_DATA" value="$RENDER_SETTINGS_DATA$||ls;$index$;$enabled$;$preview$;$angle$;$alt$;$distance$;$size$;$intensity$;$color$;$x$;$y$;$z$;$rel_index$" />
	</TEMPLATE>
	
	<!--
	// legacy way of adding light sources (still works, can be used for adding more than 3 default sources)
	<LIGHT_SOURCE index="0" enabled="1" angle="-12" alt="40" distance="650" size="130" intensity="1.21" color="#FFEE99" />
	<LIGHT_SOURCE index="1" enabled="1" angle="-45" alt="88" distance="1500" size="200" intensity="100.9" color="#FFFFFF" />
	<LIGHT_SOURCE index="2" enabled="1" angle="35" alt="68" distance="2000" size="300" intensity="3" color="#AAFFFF"  />
	-->
	
	<TEMPLATE id="VARS_RENDER" light_source_a="1;0;-12;80;650;130;0.21;#FFFFFF;;;;;;;" light_source_b="1;0;-45;88;1500;200;1.9;#FFEE99;;;;;;;" light_source_c="1;0;35;68;2000;300;3;#AAFFFF;;;;;;;" light_source_d="" light_source_e="" ambient_intensity="0.45" ref_intensity_distance="500" camera="1000;22;0;0" >
		<VAR id="RENDER_SETTINGS_DATA" value="$RENDER_SETTINGS_DATA$||ls;0;$light_source_a$||ls;1;$light_source_b$||ls;2;$light_source_c$||ls;3;$light_source_d$||ls;4;$light_source_e$||cam;$camera$||amb;$ambient_intensity$||rid;$ref_intensity_distance$" />
		<ONLOAD script="render_settings_data = &quot;$RENDER_SETTINGS_DATA$;&quot;; " />
	</TEMPLATE>

	<!-- END OF DEFAULT VARS VALUES -->

	<!-- initialize with default values -->
	<VARS_TOOLBAR />
	<VARS_BODY />
	<VARS_METERS />
	<VARS_STATUS />
	<VARS_DECOR />
	<VARS_RENDER />

	<!-- ###################################### -->
	<!-- INCLUDE COMMON SCRIPTS  -->
	<!-- ###################################### -->
	
	<COMMON_SCRIPTS cnvp='SKIN' /> <!-- include common.as there -->
	
	<EXPOSED_OBJECTS object_ids="$bypass$;body_cell.*;OpenMenuLM;OpenMenu;skin_menu.*;$script_file_path$;$script_gui_path$;a_toolbar_bg_innerKUIML;a_toolbar_innerKUIML;a_body_bg_innerKUIML;a_body_innerKUIML;a_status_bg_innerKUIML;$script_input_params_count$;$script_output_params_count$;a_body_font;$script_name_string$;a_toolbar_font_size;a_toolbar_text_color;dsp.output_string0;$script_name_string$;$script_input_strings_count$;$script_output_strings_count$;dsp.private_output0;DisplayAbout;openSomeUrl;toggle_bypass;Undo;Redo;OpenInfo;OpenPresetsMenu;OpenCustomMenu.*;ShowDefaultScriptsMenu;PreviousPreset;NextPreset;OpenManual;current_preset.*;a_lm_menu;a_custom_menu;ToggleControlsSettings;ReloadScript;a_toolbar_title_fixed_text.*;lm_skin_resizeable;script_kuiml_loaded_ok;$script_description_string$" />
	
	<!-- create some script aliases -->
	<SCRIPT script="Kt::Action@ a_lm_menu, a_custom_menu, a_open_settings, a_show_skin_scanner, a_show_controls_gen, a_show_global_prefs, a_open_export, a_open_about_lmskin;" />	
	
	<!-- NO_EXPORT_IF_STATIC -->
	<!-- include scripts for reloading parts of skin -->
	<SCRIPT src="$LM_DIR$$SCRIPTS_SUBDIR$/update_skin.as" preprocess="true" />
	<!-- END_NO_EXPORT_IF_STATIC -->
</DUMMY>