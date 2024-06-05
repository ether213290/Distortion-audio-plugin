//////////////////////////////////
// RELOAD TOOLBAR, BODY, STATUSBAR
//////////////////////////////////

enum autoLayoutParams{
  AL_PRIORITY, AL_FILENAME, AL_ORIENTATION, AL_CONTROLSINLINE, AL_GROUPBY
}

enum autoLayoutTypes{
  AL_TYPE_TEXT, AL_TYPE_GRAPHICS, AL_TYPE_CUSTOM
}

enum autoLayoutThemeParams{
  AL_ITEM_WIDTH, AL_ITEM_HEIGHT, AL_ITEM_H_MARGIN, AL_ITEM_V_MARGIN, AL__1, AL__2, 
  AL_GROUP_FILL_COLOR, AL_GROUP_FILL_COLOROP, AL_GROUP_LINE_COLOR, AL_GROUP_LINE_COLOROP, AL_GROUP_LINE_WIDTH, AL_GROUP_H_MARGIN, AL_GROUP_V_MARGIN, AL_GROUP_CONVERGE, AL_GROUP_ROUND, AL_RC_PAD, AL__4, AL_NAME_V_OFFSET, AL_VALUE_V_OFFSET, AL_TEXT_COLOR, 
  AL_EXTRAS, AL__11, AL__12, AL__13, AL__14, AL__15, AL__16, AL__17, AL__18, AL__19, 
  AL_KNOB, AL_KNOB_SCALING, AL_KNOB_C1, AL_KNOB_OP1, AL_KNOB_C2, AL_KNOB_OP2, AL_KNOB_C3, AL_KNOB_OP3, AL_KNOB_C4, AL_KNOB_OP4, AL_KNOB_C5, AL_KNOB_OP5, 
  AL_SWITCH, AL_SWITCH_SCALING, AL_SWITCH_C1, AL_SWITCH_OP1, AL_SWITCH_C2, AL_SWITCH_OP2, AL_SWITCH_C3, AL_SWITCH_OP3, AL_SWITCH_C4, AL_SWITCH_OP4, AL_SWITCH_C5, AL_SWITCH_OP5, 
  AL_METER, AL_METER_SCALING, AL_METER_C1, AL_METER_OP1, AL_METER_C2, AL_METER_OP2, AL_METER_C3, AL_METER_OP3, AL_METER_C4, AL_METER_OP4, AL_METER_C5, AL_METER_OP5
}

enum autoLayoutPlaceHolders{
  AL_PH_GENERAL,
  AL_PH_KNOB,
  AL_PH_METER,
  AL_PH_SWITCH
}

// paths to skin scanner and controls generator
string skin_scanner_main_file = "$LM_DIR$$EDITORS_SUBDIR$/skin_scanner/ss_browser.kuiml";
string controls_gen_main_file = "$LM_DIR$$EDITORS_SUBDIR$/controls_gen/main_cg.kuiml";

// function to load/reload .kuiml subskin (body content inside KUIML_WIDGET)
bool is_first_reloadBody = true;

// used for exported plugins with flattened skin and dynamically loaded body
string preloadedBodyContent = "";

void reloadBody() {
    file f;

    _a_log_unusual_text = false; // reset log flag  

    string auto_layout_s = a_body_auto_layout;
    string auto_layout_theme_s = a_body_auto_layout_theme;

    string body_content = "";
    string kuiml_file_path;

    // parse auto-layout settings
    array<string> al = auto_layout_s.split(";");
    al.resize(AL_GROUPBY+1); // to prevent array errors
    array<string> alt = auto_layout_theme_s.split(";");
    alt.resize(AL_METER_OP5+1); // to prevent array errors

    string custom_kuiml_file_path = $script_gui_path$;
    if (custom_kuiml_file_path.isEmpty()) {
      custom_kuiml_file_path = """$SKINS_DIR$custom.kuiml""";
      confirmPath(custom_kuiml_file_path);
    }

    // check if we have custom .kuiml available
    if (f.open(custom_kuiml_file_path, "r") >= 0) {
      a_custom_kuiml_available = 1;
    } else {
      a_custom_kuiml_available = 0;
    }
    f.close();

    // on first reload check for auto-layout priority
    if (is_first_reloadBody) {
      if (al[AL_PRIORITY] == "1") {
        a_ignore_custom_kuiml = 1;
      }
      is_first_reloadBody = false;
    }



    // if we should load skin scanner
    if (skin_scanner_enabled > 0.5) {
      kuiml_file_path = skin_scanner_main_file;
      confirmPath(kuiml_file_path);
      if (f.open(kuiml_file_path, "r") >= 0) {
        body_content = f.readString(f.getSize());

        f.close();
      }

    } else // if we should load controls generator
    if (controls_gen_enabled > 0.5) {
      kuiml_file_path = controls_gen_main_file;
      confirmPath(kuiml_file_path);
      if (f.open(kuiml_file_path, "r") >= 0) {
        body_content = f.readString(f.getSize());

        f.close();
      }
      
    } else // if loading normal custom .kuiml
    if ((a_ignore_custom_kuiml < 0.5) and (a_custom_kuiml_available > 0.5)) {
        kuiml_file_path = custom_kuiml_file_path;
        confirmPath(kuiml_file_path);

        // status("custom " + kuiml_file_path);

        if (f.open(kuiml_file_path, "r") >= 0) {
          body_content = f.readString(f.getSize());
          f.close();
        }

    } else {

      

      // if loading auto-layout
      kuiml_file_path = "$LM_DIR$$LAYOUTS_SUBDIR$/" + al[AL_FILENAME];
      confirmPath(kuiml_file_path);

      // if loading auto-layout 
      string content;
      if (f.open(kuiml_file_path, "r") >= 0) {
        content = f.readString(f.getSize());
      }

      // if we're exported and flattened but with dynamic body
      if (preloadedBodyContent.length>0) {
        content = preloadedBodyContent;
      }
      


      if (content.length > 0) {

        // replacing basic params
        string value = int($script_input_params_count$);
        replaceString(content, "#in_params_count#", value);
        value = int($script_output_params_count$);
        replaceString(content, "#out_params_count#", value);
        value = int($script_input_strings_count$);
        replaceString(content, "#in_strings_count#", value);
        value = int($script_output_strings_count$);
        replaceString(content, "#out_strings_count#", value);

        // replacing auto-layout params
        replaceString(content, "#group_stroke_color#", alt[AL_GROUP_LINE_COLOR]);
        replaceString(content, "#group_stroke_opacity#", alt[AL_GROUP_LINE_COLOROP]);
        replaceString(content, "#group_stroke_width#", alt[AL_GROUP_LINE_WIDTH]);
        replaceString(content, "#group_fill_color#", alt[AL_GROUP_FILL_COLOR]);
        replaceString(content, "#group_fill_opacity#", alt[AL_GROUP_FILL_COLOROP]);
        replaceString(content, "#group_round#", alt[AL_GROUP_ROUND]);
        replaceString(content, "#text_color#", alt[AL_TEXT_COLOR]);

        // additional for graphic controls layout
        if (al[AL_FILENAME] != "text_layout.kuiml") {
          replaceString(content, "#orientation#", al[AL_ORIENTATION]);
          replaceString(content, "#max_in_line#", al[AL_CONTROLSINLINE]);
          replaceString(content, "#group_by#", al[AL_GROUPBY]);

          replaceString(content, "#knob_name#", alt[AL_KNOB]);
          replaceString(content, "#switch_name#", alt[AL_SWITCH]);
          replaceString(content, "#meter_name#", alt[AL_METER]);

          replaceString(content, "#item_width#", alt[AL_ITEM_WIDTH]);
          replaceString(content, "#item_height#", alt[AL_ITEM_HEIGHT]);
          replaceString(content, "#item_h_margin#", alt[AL_ITEM_H_MARGIN]);
          replaceString(content, "#item_v_margin#", alt[AL_ITEM_V_MARGIN]);

          replaceString(content, "#group_h_margin#", alt[AL_GROUP_H_MARGIN]);
          replaceString(content, "#group_v_margin#", alt[AL_GROUP_V_MARGIN]);
          replaceString(content, "#group_converge#", alt[AL_GROUP_CONVERGE]);
          
          replaceString(content, "#textname_v_offset#", alt[AL_NAME_V_OFFSET]);
          replaceString(content, "#textvalue_v_offset#", alt[AL_VALUE_V_OFFSET]);

          replaceString(content, "#rc_pad#", alt[AL_RC_PAD]);
          replaceString(content, "#extras#", alt[AL_EXTRAS]);

          replaceString(content, "#knob_scaling#", alt[AL_KNOB_SCALING]);
          replaceString(content, "#switch_scaling#", alt[AL_SWITCH_SCALING]);
          replaceString(content, "#meter_scaling#", alt[AL_METER_SCALING]);

          // get customized color for controls
          array<string> layout_params = { 
            "", 
            reloadBody_getRCColorsStrings(alt, AL_KNOB_C1), 
            reloadBody_getRCColorsStrings(alt, AL_METER_C1), 
            reloadBody_getRCColorsStrings(alt, AL_SWITCH_C1) 
          };

          // unparse "extras" string and apply params to controls
          array<string>@ pieces=alt[AL_EXTRAS].split(",");
          if(@pieces!=null)
          {
            if(pieces.length>0)
              for(uint i=0;i<pieces.length;i++) {
                  string sd = "";
                  int phn = AL_PH_GENERAL;
                  if (pieces[i].substr(0,5) == "knob.") {
                    sd = pieces[i].substr(5,-1); 
                    phn = AL_PH_KNOB;
                  }
                  if (pieces[i].substr(0,6) == "meter.") {
                    sd = pieces[i].substr(6,-1); 
                    phn = AL_PH_METER;
                  }
                  if (pieces[i].substr(0,7) == "switch.") {
                    sd = pieces[i].substr(7,-1); 
                    phn = AL_PH_SWITCH;
                  }

                  int eqpos = sd.findFirst("=");
                  if (eqpos > 0) {
                    layout_params[phn] += " "+sd.substr(0,eqpos)+"=\""+sd.substr(eqpos+1,-1)+"\"";
                  }
                  
              }
          }

          // turn off switch animation when editing skin
          // if (show_skin_settings_window > 0.5) {
          //   layout_params[AL_PH_SWITCH] += " use_animation='0'";
          // }
          
          // replace in auto-layout file
          replaceString(content, "#knob_params_placeholder#", layout_params[AL_PH_KNOB]);
          replaceString(content, "#meter_params_placeholder#", layout_params[AL_PH_METER]);
          replaceString(content, "#switch_params_placeholder#", layout_params[AL_PH_SWITCH]);
        }

        body_content = content;
        f.close();
      }
    }


    // add filename without extension just for utility purposes
   
    // we add more info after SKIN 
    // to be able to change font on the fly (live preview)
    // this way we also can add KUIML_DIR and KUIML_FILE variables 
    int pos = body_content.findFirst("<SKIN");
    
    if (pos>-1) {
      
      int pos2 = body_content.findFirst(">", pos+1);
      string skin_attrs = body_content.substr(pos+5, pos2-pos-5);
      string skin_attrs_orig = trim(skin_attrs);

      // add info about included file and its location
      string var_kuiml_dir = getDirForFile(kuiml_file_path);
      string var_kuiml_filename = getBaseFilename(kuiml_file_path);
      string var_kuiml_filename_no_ext = var_kuiml_filename.substr(0, var_kuiml_filename.findLast("."));

      // prepare SCRIPT_DATA_PATH variable
      string var_script_data_path = """$SCRIPT_DATA_PATH$"""; // for exported plugins it's filled
      confirmPath(var_script_data_path);

      if (var_script_data_path.findFirst("SCRIPT_DATA_PATH$")>-1) {
        string subskin_file_path = $script_file_path$;
        if (subskin_file_path.length > 0) {
          confirmPath(subskin_file_path);
          var_script_data_path = getDirForFile(subskin_file_path)+""+getBaseFilename(subskin_file_path,false)+"-data";
        } else {
          var_script_data_path = "";
        }
      }

      // for resizeable skins add 100% width and height to cell
      string resizeable_addon = "";
      if (lm_skin_resizeable > 0.5) {
        resizeable_addon = " width=\"100%\" height=\"100%\"";
        replaceString(skin_attrs, " width", " _width");
        replaceString(skin_attrs, " height", " _height");
      }

      

      // calculate inverse color for text backgrounds
      array<string> body_text_color_ar = (""+a_body_font).split(";");

      string body_text_color = "#000000";
      if (body_text_color_ar.length > 4) {
        body_text_color = body_text_color_ar[4];
      }
      int body_text_color_avg = ((parseInt(body_text_color.substr(1,2),16) + parseInt(body_text_color.substr(3,2),16) +parseInt(body_text_color.substr(5,2),16)) / 3);
      string body_text_color_inv = "#FFFFFF";
      if (body_text_color_avg > 180) body_text_color_inv = "#000000";

      // kuiml subskin font attributes have more value than skin settings
      string dis_fsi = (skin_attrs.findFirst(" font_size")>-1) ? "_" : "";
      string dis_ff = (skin_attrs.findFirst(" font_face")>-1) ? "_" : "";
      string dis_fw = (skin_attrs.findFirst(" font_weight")>-1) ? "_" : "";
      string dis_fst = (skin_attrs.findFirst(" font_style")>-1) ? "_" : "";
      string dis_fq = (skin_attrs.findFirst(" font_quality")>-1) ? "_" : "";
      string dis_tc = (skin_attrs.findFirst(" text_color")>-1) ? "_" : "";

      // we add KUIML_TIME variable so that .kuiml really reloads each time, even if not changed
      string s = "<VAR id='SCRIPT_DATA_PATH' value=\""+var_script_data_path+"\" /><VAR id='KUIML_DIR' value=\""+var_kuiml_dir+"\" /><VAR id='KUIML_FILENAME' value=\""+var_kuiml_filename+"\" /><VAR id='KUIML_FILENAME_NO_EXT' value=\""+var_kuiml_filename_no_ext+"\" /><VAR id='KUIML_TIME' value='"+getHMS()+"' /><VAR id='BODY_TEXT_COLOR_INV' value='"+body_text_color_inv+"' /><COMMON_SCRIPTS n='20000' p='BODY' include_lmr_scripts=\"$BODY_INCLUDE_LMR_SCRIPTS$\" /><PARSE_BODY_FONT data='"+a_body_font+"' /><CELL"+resizeable_addon+" font_size"+dis_fsi+"=\"$BODY_FONT_SIZE$\" font_face"+dis_ff+"=\"$BODY_FONT_FACE$\" font_weight"+dis_fw+"=\"$BODY_FONT_WEIGHT$\" font_style"+dis_fst+"=\"$BODY_FONT_STYLE$\" font_quality"+dis_fq+"=\"$BODY_FONT_QUALITY$\" text_color"+dis_tc+"=\"$BODY_TEXT_COLOR$\" "+skin_attrs+">";

      body_content.insert(pos2+1, s); // insert data after skin
      body_content.erase(pos+5, pos2-pos-5); // remove old skin attrs

      // removing some attrs from skin to prevent duplicating (cause they are mirrored in CELL)
      replaceString(skin_attrs_orig, "margin", "margin_");
      replaceString(skin_attrs_orig, "spacing", "spacing_");
      replaceString(skin_attrs_orig, "flip", "flip_");
      replaceString(skin_attrs_orig, "font_size", "font_size_");
      replaceString(skin_attrs_orig, "scaling", "scaling_");
      
      body_content.insert(pos+5, " "+skin_attrs_orig);

      replaceString(body_content, "</SKIN>", "</CELL><ONLOAD script=\"script_kuiml_loaded_ok = 1;\" requires=\"script_kuiml_loaded_ok\" /></SKIN>");
    } else {
      if (kuiml_file_path != "") {
        body_content = "<SKIN><TEXT value=\"Inner-kuiml file not found\" /><TEXT value=\""+escape(kuiml_file_path)+"\" /><ONLOAD script=\"script_kuiml_loaded_ok = 0.1;\" requires=\"script_kuiml_loaded_ok\" /></SKIN>";
      }
    }

    

    // if body content has changed, update innerKUIML
    if (body_content != (""+a_body_innerKUIML)) {
      script_kuiml_loaded_ok = 0; // set flag to check if kuiml is successfully loaded
      a_body_innerKUIML = body_content;  
    }
}

// helper to get color for rendered controls for auto-layout mode
string reloadBody_getRCColorsStrings(array<string> alt, int i){
  string s;
  for (int n=1;n<=5;n++) {
    if (alt[i]!="") s += " color"+n+"=\""+alt[i]+"\"";
    if (alt[i+1]!="") s +=" opacity"+n+"=\""+alt[i+1]+"\""; 
    i+=2;
  }
  return s;
}