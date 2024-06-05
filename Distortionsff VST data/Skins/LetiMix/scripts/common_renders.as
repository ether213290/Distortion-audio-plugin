////////////////////////////////////////
// RENDER BOX 
////////////////////////////////////////

namespace LM{

  // create light object upon script loading
  CanvasLight Light();
  CanvasCamera Camera();
  CanvasRenderSettingsParser RenderSettingsParser();

  // this is an order params are transmitted from KUIML
  enum RenderSettingsFromKUIMLLight{
    _RS_TYPE, _RS_INDEX, _RS_ENABLED, _RS_PREVIEW, _RS_ANGLE, _RS_ALT, _RS_DISTANCE, _RS_SIZE, _RS_INTENSITY, _RS_COLOR, _RS_X, _RS_Y, _RS_Z, _RS_REL_INDEX 
  }

  enum RenderSettingsFromKUIMLCamera{
    _RS_TYPE_, _RS_CAM_DISTANCE, _RS_CAM_PERSPECTIVE, _RS_CAM_X_OFFSET, _RS_CAM_Y_OFFSET
  }

  // this is the order of params in "sources" array
  enum LightSourceParams{
    LS_ENABLED, LS_PREVIEW, LS_ANGLE, LS_ANGLE_RAD, LS_ALT, LS_ALT_RAD, LS_DISTANCE, LS_SIZE, LS_INTENSITY, LS_COLOR_R, LS_COLOR_G, LS_COLOR_B, LS_COLOR_H, LS_COLOR_S, LS_COLOR_L, LS_X, LS_Y, LS_Z, LS_RELATIVE_INDEX, LS_REL_ANGLE, LS_REL_ALT, LS_REL_DISTANCE, LS_REL_DISTANCE_MULT, LS_REL_SIZE, LS_REL_SIZE_MULT, LS_REL_INTENSITY, LS_REL_INTENSITY_MULT, LS_REL_X, LS_REL_Y, LS_REL_Z
  }

  // class to parse render settings
  class CanvasRenderSettingsParser{

    // constructor
    CanvasRenderSettingsParser(){
      // string rs = render_settings_data;
      if (render_settings_data == "") {
        render_settings_data = "$RENDER_SETTINGS_DATA$";
      }
      parseString(render_settings_data);

      // recalculate relative sources
      if (Light.has_relative_sources) Light.recalcAllRelativeLightSources();
    }

    // parse render settings string
    void parseString(string rs){
      Light.has_relative_sources = false;

      // parse line from KUIML
      array<string> arr = rs.split("||");

      // step through all possible "light sources"
      for(uint n=0;n<arr.length;n++) {
        if (!arr[n].isEmpty()) {
          array<string> ar = arr[n].split(";");
          if (ar[_RS_TYPE] == "ls") {
            ar.resize(_RS_REL_INDEX+1);
            if (ar[_RS_INDEX] == "") continue;
            if (ar[_RS_ENABLED] == "") continue;
            // relative index (one color can be relative to another)
            int ls_index = parseInt(ar[_RS_INDEX]);

            int rel_index = parseInt(ar[_RS_REL_INDEX]);
            if (ar[_RS_REL_INDEX] == "") rel_index = -1;
            if (rel_index > -1) Light.has_relative_sources = true;

            // source can be added via x, y, z coonrds
            double x = f(ar[_RS_X]), y = f(ar[_RS_Y]), z = f(ar[_RS_Z]);
            if ((x+y+z) != 0) {
              Light.setSourceXYZ(ls_index, rel_index, f(ar[_RS_ENABLED]), f(ar[_RS_PREVIEW]), x, y, z, f(ar[_RS_SIZE]), f(ar[_RS_INTENSITY]), ar[_RS_COLOR], arr[n] );
            } else {
              // or can be added with angle, altitude and distance
              Light.setSource(ls_index, rel_index, f(ar[_RS_ENABLED]), f(ar[_RS_PREVIEW]), f(ar[_RS_ANGLE]), f(ar[_RS_ALT]), f(ar[_RS_DISTANCE]), f(ar[_RS_SIZE]), f(ar[_RS_INTENSITY]), ar[_RS_COLOR], arr[n] );
            }
          } else if (ar[_RS_TYPE] == "cam") {
            // keep camera settings
            Camera.distance = f(ar[_RS_CAM_DISTANCE]);
            Camera.perspective = f(ar[_RS_CAM_PERSPECTIVE]);
            Camera.x_offset = f(ar[_RS_CAM_X_OFFSET]);
            Camera.y_offset = f(ar[_RS_CAM_Y_OFFSET]);
          } else if (ar[_RS_TYPE] == "amb") {
            Light.ambient_intensity = f(ar[_RS_TYPE_+1]);
          } else if (ar[_RS_TYPE] == "rid") {
            Light.ref_intensity_distance = f(ar[_RS_TYPE_+1]);
          }
        }
      }

      Light.calcCombinedLight();
    }


  }

  class CanvasCamera{
    // simplest camera seetings here
    double perspective = 22, distance = 1000, x_offset = 0, y_offset = 0;
  }

  // the object of this class is used for keep the information about all the light sources
  // light sources are later used by CanvasObjects to drop shadows etc.
  class CanvasLight{

    array<array<double>> sources; // main array for keeping info of light sources

    // a distance (in pixels) where set light intensity is measured ()
    double ref_intensity_distance = 500;
    double ambient_intensity = 0.45; // minimal lightness of objects

    private array<double> _default_source = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-1,0,0,0,1,0,1,0,1,0,0,0}; // values for empty source

    bool has_relative_sources = false;

    // set light source using x,y,z coords
    void setSourceXYZ(int index, int rel_to, double enabled, double preview, double x, double y, double z, double size, double intensity, string color, string sourceLine = ""){
      
      // covert xyz coordinates to angles
      double distance = sqrt(x*x + y*y + z*z); // vector length to light source
      double alt = atan2(z, sqrt(distance*distance - z*z))*180/pi;
      // double angle = -atan2(x,-y)*180/pi;
      double angle = atan2(x,-y)*180/pi;
      // status("angle: " + angle);

      // add as usual source via angles
      setSource(index, rel_to, enabled, preview, angle, alt, distance, size, intensity, color, sourceLine);
    }

    // set light source using angles in degrees (for angle 0 deg is at 12 o'clock, for altitude (alt) 90 deg is right on top, distance is in pixels)
    void setSource(int index, int rel_to, double enabled, double preview, double angle, double alt, double distance, double size, double intensity, string color, string sourceLine = "") {
      
      // convert angle to Radians
      double angleR = angle*pi/180;

      // limit alt and convert to Radians
      if (alt <= 0) alt = 0.05;
      if (alt >= 180) alt = 179.95;
      double altR = alt*pi/180;

      // prepare colors
      double r = double(parseInt(color.substr(1,2),16))/255.0;
      double g = double(parseInt(color.substr(3,2),16))/255.0;
      double b = double(parseInt(color.substr(5,2),16))/255.0;
      double hue,sat,lig;
      convertRGBtoHSL(r,g,b,hue,sat,lig);

      // calculate coordinates according to angles and distance
      //double x = sin(angleR+pi)*distance*cos(altR);
      // double y = cos(angleR+pi)*distance*cos(altR);
      double x = cos(angleR-pi2)*distance*cos(altR);
      double y = sin(angleR-pi2)*distance*cos(altR);
      double z = sin(altR)*distance;

      // check if index exists
      verifyIndexes(index);

      // update light source data
      array<double>@ s = sources[index];
      if (enabled > -1) s[LS_ENABLED] = enabled;
      s[LS_ENABLED] = enabled;
      s[LS_PREVIEW] = preview;
      s[LS_ANGLE] = angle;
      s[LS_ANGLE_RAD] = angleR;
      s[LS_ALT] = alt;
      s[LS_ALT_RAD] = altR;
      s[LS_DISTANCE] = distance;
      s[LS_SIZE] = size;
      s[LS_INTENSITY] = intensity;
      s[LS_COLOR_R] = r;
      s[LS_COLOR_G] = g;
      s[LS_COLOR_B] = b;
      s[LS_COLOR_H] = hue;
      s[LS_COLOR_S] = sat;
      s[LS_COLOR_L] = lig;
      s[LS_X] = x;
      s[LS_Y] = y;
      s[LS_Z] = z;

      // if this is a relative source
      if ((rel_to != index) and (rel_to > -1) and (rel_to < int(sources.length))) {
        setSourceRelative(index, rel_to, sourceLine);
      }

      // calculate relatives if any
      calcRelativeSources(index);
    }

    // add information for relative light source
    void setSourceRelative(int index, int rel_to, string sourceLine = ""){

      array<double>@ s = sources[index];
      s[LS_RELATIVE_INDEX] = rel_to;

      if (sourceLine.isEmpty()) return;

      array<string> ar = sourceLine.split(";");

      // if params start with + or -
      if (isPlusMinus(ar[_RS_ANGLE])) s[LS_REL_ANGLE] = f(ar[_RS_ANGLE]);
      if (isPlusMinus(ar[_RS_ALT])) s[LS_REL_ALT] = f(ar[_RS_ALT]);
      if (isPlusMinus(ar[_RS_DISTANCE])) s[LS_REL_DISTANCE] = f(ar[_RS_DISTANCE]);
      if (isPlusMinus(ar[_RS_SIZE])) s[LS_REL_SIZE] = f(ar[_RS_SIZE]);
      if (isPlusMinus(ar[_RS_INTENSITY])) s[LS_REL_INTENSITY] = f(ar[_RS_INTENSITY]);

      // if params start with *
      if (isMultiply(ar[_RS_DISTANCE])) s[LS_REL_DISTANCE_MULT] = f(ar[_RS_DISTANCE].substr(1));
      if (isMultiply(ar[_RS_SIZE])) s[LS_REL_SIZE_MULT] = f(ar[_RS_SIZE].substr(1));
      if (isMultiply(ar[_RS_INTENSITY])) s[LS_REL_INTENSITY_MULT] = f(ar[_RS_INTENSITY].substr(1));
      if (!ar[_RS_X].isEmpty()) s[LS_REL_X] = f(ar[_RS_X]);
      if (!ar[_RS_Y].isEmpty()) s[LS_REL_Y] = f(ar[_RS_Y]);
      if (!ar[_RS_Z].isEmpty()) s[LS_REL_Z] = f(ar[_RS_Z]);
    }


    // change angle of light source (for quick dev automation)
    void setAngle(int index, double angle) {
      verifyIndexes(index);

      array<double>@ s = sources[index];

      // convert angle to Radians
      double angleR = angle*pi/180;
      double altR = s[LS_ALT_RAD];
      double distance = s[LS_DISTANCE];

      // calculate coordinates according to angles and distance
      double x = cos(angleR-pi2)*distance*cos(altR);
      double y = sin(angleR-pi2)*distance*cos(altR);
      double z = sin(altR)*distance;

      s[LS_ANGLE] = angle;
      s[LS_ANGLE_RAD] = angleR;
      s[LS_X] = x;
      s[LS_Y] = y;
      s[LS_Z] = z;

      // calculate relatives if any
      if (has_relative_sources) calcRelativeSources(index);
    }

    // recalculate all relative sources (if any)
    void recalcAllRelativeLightSources() {
      if (!has_relative_sources) return;
      for(uint n=0;n<sources.length;n++) {
        calcRelativeSources(int(n));
      }
    }

    // calculate relatives for source
    void calcRelativeSources(int index) {
      if (!has_relative_sources) return;
      double parent = rint(index);
      array<double>@ p = sources[index];
      double angle, angleR, alt, altR, distance, size, intensity, mult, x, y, z, xr, yr, zr;

      // step through all light sources
      for(uint n=0;n<sources.length;n++) {
        // if this light source is relative
        if (closeTo(sources[n][LS_RELATIVE_INDEX],parent, 0.1)) {
          array<double>@ c = sources[n];
          // recalculate angle
          angle = p[LS_ANGLE] + c[LS_REL_ANGLE];
          if (angle > 180) angle -= 360;
          if (angle < -180) angle += 360;
          c[LS_ANGLE] = angle;
          angleR = angle*pi/180;
          c[LS_ANGLE_RAD] = angleR;
          // recalculate alt
          alt = p[LS_ALT] + c[LS_REL_ALT];
          if (alt <= 0) alt = 0.05;
          if (alt >= 180) alt = 179.95;
          c[LS_ALT] = alt;
          altR = alt*pi/180;
          c[LS_ALT_RAD] = altR;
          // recalculate distance
          distance = p[LS_DISTANCE] + c[LS_REL_DISTANCE];
          mult = c[LS_REL_DISTANCE_MULT];
          if (mult != 0) distance *= mult;
          c[LS_DISTANCE] = distance;
          // recalculate size
          size = p[LS_SIZE] + c[LS_REL_SIZE];
          mult = c[LS_REL_SIZE_MULT];
          if (mult != 0) size *= mult;
          c[LS_SIZE] = size;
          // recalculate intensity
          intensity = p[LS_INTENSITY] + c[LS_REL_INTENSITY];
          mult = c[LS_REL_INTENSITY_MULT];
          if (mult != 0) intensity *= mult;
          c[LS_INTENSITY] = intensity;

          // calculate coordinates according to angles and distance
          // x = sin(angleR+pi)*distance*cos(altR);
          // y = cos(angleR+pi)*distance*cos(altR);
          x = cos(angleR-pi2)*distance*cos(altR);
          y = sin(angleR-pi2)*distance*cos(altR);
          z = sin(altR)*distance;

          xr = c[LS_REL_X]; yr = c[LS_REL_Y]; zr = c[LS_REL_Z];

          x += xr; y += yr; z += zr;
          c[LS_X] = x; c[LS_Y] = y; c[LS_Z] = z;

          // if used xyz offset, recalculate angles
          if ((xr + yr + zr) != 0) {
            distance = sqrt(x*x + y*y + z*z); // vector length to light source
            alt = atan2(z, sqrt(distance*distance - z*z))*180/pi;
            angle = -atan2(x,-y)*180/pi;

            c[LS_DISTANCE] = distance;
            c[LS_ANGLE] = angle;
            angleR = angle*pi/180;
            c[LS_ANGLE_RAD] = angleR;
            c[LS_ALT] = alt;
            altR = alt*pi/180;
            c[LS_ALT_RAD] = altR;
          }

          // calc relatives of this (if any)
          calcRelativeSources(n);
        }
      }
    }

    double CL_x, CL_y, CL_z, CL_r, CL_g, CL_b, CL_AngleR, CL_distance, CL_AltR;
    void calcCombinedLight(){
      
      // set default values
      CL_x = 0; CL_y = 0; CL_z = 0;
      CL_r = 1; CL_g = 1; CL_b = 1;
      CL_AltR = pi*0.5;
      CL_AngleR = 0;
      CL_distance = 600;

      // get number of light sources
      array<array<double>>@ lights = Light.sources;
      int nlights = int(lights.length);
      if (nlights == 0) return;

      double total_intensity = 0;

      for(int ls_n = 0; ls_n < nlights; ls_n++) {
        
        array<double>@ ls = lights[ls_n];
        if (ls[LS_ENABLED] == 0) continue; // skip if light source is off
        
        // calc Light intensity in center of GUI
        double Lintensity = ls[LS_INTENSITY]*(1/pow(ls[LS_DISTANCE]/Light.ref_intensity_distance, 2));

        CL_x += ls[LS_X]*Lintensity;
        CL_y += ls[LS_Y]*Lintensity;
        CL_z += ls[LS_Z]*Lintensity;

        CL_r += ls[LS_COLOR_R]*Lintensity;
        CL_g += ls[LS_COLOR_G]*Lintensity;
        CL_b += ls[LS_COLOR_B]*Lintensity;

        total_intensity += Lintensity;
      }

      CL_r /= total_intensity;
      CL_g /= total_intensity;
      CL_b /= total_intensity;

      // coordinates of combined light source
      CL_x /= total_intensity;
      CL_y /= total_intensity;
      CL_z /= total_intensity;

      // combined light source properties
      CL_AngleR = atan2(CL_x, -CL_y);
      // vector length to light source
      CL_distance = sqrt(CL_x*CL_x + CL_y*CL_y + CL_z*CL_z); 
      CL_AltR = atan2(CL_z, sqrt(CL_distance*CL_distance - CL_z*CL_z));  

      // status("LS A: " + lights[0][LS_X] + "," + lights[0][LS_Y] + " | Comb: " + CL_x+","+CL_y);
    }

    // check sources bounds and fill empty arrays
    void verifyIndexes(int index) {
      // add, if this light source doesn't yet exists
      if (int(sources.length) <= index) {
        sources.resize(index+1);
      }

      // check for skipped sources and set them to disabled
      for(uint n=0;n<sources.length;n++) {
        if (sources[n].length == 0) {
          sources[n] = _default_source;
        } 
      }
    }


    // used for debugging
    void Debug() {

      // echo("sources.length: " + int(sources.length));

      // for(int n=0;n<int(sources.length);n++) {
      //  array<double>@ ar = sources[n];
      //  if (ar.length > 0) if (ar[LS_ENABLED] > 0)
      //    echo("["+n+"] rel: " + ar[LS_RELATIVE_INDEX] + ", angle: " + ar[LS_ANGLE] + ", angleR: " + ar[LS_ANGLE_RAD] + ", alt: " + ar[LS_ALT] + ", altR: " + ar[LS_ALT_RAD] + ", dist: " + ar[LS_DISTANCE] + ", size: " + ar[LS_SIZE] + ", intens: " + ar[LS_INTENSITY] + ", color: " + ar[LS_COLOR_R] + "," + ar[LS_COLOR_G] + "," + ar[LS_COLOR_B] + ", x: " + ar[LS_X] + ", y: " + ar[LS_Y] + ", z: " + ar[LS_Z] );

      //}

      // echo_flush(lm_text3);
    }

    // check if string starts with + or -
    bool isPlusMinus(string val) {
      // 43 = +, 45 = -
      return ((val[0] == 43) || (val[0] == 45));
    }
    // check if string starts with *
    bool isMultiply(string val) {
      // 42 = *
      return ((val[0] == 42));
    }
    
  } // end of CanvasLight class


  


enum renderBoxEnum{
  RB_GRADIENT_TYPE,
  RB_GLOBAL_OPACITY,
  RB_GRADIENT_SHIFT,
  RB_STROKE_WIDTH,
  RB_STROKE_EXPAND,
  RB_STROKE_ROUND,
  RB_Y_OFFSET,
  RB_RESERVED_1,
  RB_RESERVED_2,
  RB_RESERVED_3,
  RB_BG1_COLOR = 10,
  RB_BG1_OPACITY,
  RB_BG1_POS,
  RB_BG2_COLOR,
  RB_BG2_OPACITY,
  RB_BG2_POS,
  RB_STROKE_COLOR,
  RB_STROKE_OPACITY,
  RB_RESERVED_4
}


void renderBox(double w, double h, string data, bool expand_invert = false) {
 auto ctx=Kt::Graphics::GetCurrentContext();
    array<string> ar(20);
    ar = data.split(";");
    ar.resize(20);
    
    int gtype=parseInt(ar[RB_GRADIENT_TYPE]);
    double gop = parseFloat(ar[RB_GLOBAL_OPACITY]);
    double stroke_width=parseFloat(ar[RB_STROKE_WIDTH]);
    double expand=parseFloat(ar[RB_STROKE_EXPAND]);
    double round_size=parseFloat(ar[RB_STROKE_ROUND]);

    string fill_color=ar[RB_BG1_COLOR];
    double fill_opacity=parseFloat(ar[RB_BG1_OPACITY]);

    string stroke_color=ar[RB_STROKE_COLOR];
    double stroke_opacity=parseFloat(ar[RB_STROKE_OPACITY]);
    
    if (expand < 0) { expand_invert = !expand_invert; expand=abs(expand); }
    double y_top = parseFloat(ar[RB_Y_OFFSET]);
    double s = round_size/2, sw2 = stroke_width/2;
    double x = s+sw2, y = y_top+sw2,  r, g, b, r2, g2, b2;
    ctx.path.Clear();

      /* draw corners using arcs */
      if (!expand_invert) { x -= expand; } else { y-=expand; }
      ctx.path.MoveTo(x,y);
      ctx.path.ArcNegative(x,y+s, s, 270, 180 ); 
      x-=s; y=h-s-sw2;
      if (expand_invert) { y+=expand; }
      ctx.path.LineTo(x, y);
      ctx.path.ArcNegative(x+s, y, s, 180, 90 ); 
      y+=s; x=w-s-sw2;
      if (!expand_invert) { x+=expand; }
      ctx.path.LineTo(x,y);
      ctx.path.ArcNegative(x,y-s, s, 90, 0); x+=s;
      y = y_top+s+sw2;
      if (expand_invert) { y-=expand; }
      ctx.path.LineTo(x, y);
      ctx.path.ArcNegative(x-s,y, s, 0, -90); 

    ctx.path.Close();
    
    r=double(parseInt(fill_color.substr(1,2),16))/255.0;
    g=double(parseInt(fill_color.substr(3,2),16))/255.0;
    b=double(parseInt(fill_color.substr(5,2),16))/255.0;

    if ((fill_opacity>0) or (gtype != 0)) {
      if (gtype == 0) {
      ctx.source.SetRGBA(r,g,b,fill_opacity*gop); 
      ctx.FillPath(); 
      } else {
        /* for gradients */
        double fill_pos=parseFloat(ar[RB_BG1_POS])/100;
        string fill_color2=ar[RB_BG2_COLOR];
        double fill_opacity2=parseFloat(ar[RB_BG2_OPACITY]);
        double fill_pos2=parseFloat(ar[RB_BG2_POS])/100;
        double shift = parseFloat(ar[RB_GRADIENT_SHIFT])/100;
        r2=double(parseInt(fill_color2.substr(1,2),16))/255.0;
        g2=double(parseInt(fill_color2.substr(3,2),16))/255.0;
        b2=double(parseInt(fill_color2.substr(5,2),16))/255.0;

        double ga=0, gb=0, gc=0, gd=0, gp1=fill_pos, gp2=fill_pos2, gp3=fill_pos2, gp4=fill_pos2;
        if ((gtype==1) or (gtype==3) or (gtype==4)) {
          gd = h;
        } else if (gtype==2) {
          gb = h;
        } else if ((gtype==5) or (gtype==7) or (gtype==8)) {
          gc = w;
        } else if (gtype==6) {
          ga = w;
        }
        if ((gtype==4) or (gtype==8)) {
          double z; 
          z=r; r=r2; r2=z;
          z=g; g=g2; g2=z;
          z=b; b=b2; b2=z;
          z=fill_opacity; fill_opacity=fill_opacity2; fill_opacity2=z;
          z=fill_pos; fill_pos=1-fill_pos2; fill_pos2=1-z;
        }
        if ((gtype==3) or (gtype==4) or (gtype==7) or (gtype==8)) {
          gp1=fill_pos;
          gp2=-0.5+fill_pos2;
          gp3=1.5-fill_pos2;
          gp4=1-fill_pos;
        }
        auto gradlb = ctx.get_patterns().NewLinearGradient(ga, gb, gc, gd);
        gradlb.AddColorStopRGBA(gp1+shift, r, g, b, fill_opacity*gop);
        gradlb.AddColorStopRGBA(gp2+shift, r2, g2, b2, fill_opacity2*gop);
        if ((gtype==3) or (gtype==4) or (gtype==7) or (gtype==8)) {
          gradlb.AddColorStopRGBA(gp3+shift, r2, g2, b2, fill_opacity2*gop);
          gradlb.AddColorStopRGBA(gp4+shift, r, g, b, fill_opacity*gop);
        }
        gradlb.SelectAsSource(); 
        ctx.FillPath(); 
      }
    } 
    ctx.settings.set_lineWidth(stroke_width);
    r=double(parseInt(stroke_color.substr(1,2),16))/255.0;
    g=double(parseInt(stroke_color.substr(3,2),16))/255.0;
    b=double(parseInt(stroke_color.substr(5,2),16))/255.0;
    ctx.source.SetRGBA(r,g,b,stroke_opacity*gop);
    ctx.StrokePath();
}

////////////////////////////////////
// METERS rendering and preparation
////////////////////////////////////

// meterRenderParams structure (params follow the same order as in $render_params_string$ string):
// (0-4) style;min_level;max_level;-;-;
// (5-9) level_bg_opacity;desired_leds_count;min_led_size;max_led_size;led_spacing
// (10-14) hold_line_width;hold_line_use_color;inner_pad_h;inner_pad_v;-;
// (15-19) stroke_width;stroke_expand;stroke_round;led_round;-;
// (20-29) -;-;-;-;-;-;-;-;-;-;
// (30-...) colors[color[r,g,b];opacity;shift];

enum renderMeterEnum{
  RM_STYLE,
  RM_MIN_VAL,
  RM_MAX_VAL,
  RM_RES_1,
  RM_RES_2,
  RM_LEVEL_BG_OP = 5,
  RM_LED_DESIRED_QTY,
  RM_LED_MIN_SIZE,
  RM_LED_MAX_SIZE,
  RM_LED_SPACING,
  RM_HOLD_WIDTH = 10,
  RM_HOLD_COLORIZE,
  RM_INNER_PAD_H,
  RM_INNER_PAD_V,
  RM_RES_3,
  RM_STROKE_WIDTH = 15,
  RM_STROKE_EXPAND,
  RM_STROKE_ROUND,
  RM_LED_ROUND,
  /* 19-29 reserved */
  /* colors */
  RM_LH_R = 30, RM_LH_G, RM_LH_B, RM_LH_OP, RM_LH_POS, /* Level hi */
  RM_LM_R = 35, RM_LM_G, RM_LM_B, RM_LM_OP, RM_LM_POS, /* Level mid */
  RM_LL_R = 40, RM_LL_G, RM_LL_B, RM_LL_OP, RM_LL_POS, /* Level low */
  RM_HO_R = 45, RM_HO_G, RM_HO_B, RM_HO_OP, RM_HO_RESERVED, /* Hold line */
  RM_SS_R = 50, RM_SS_G, RM_SS_B, RM_SS_OP, RM_SS_RESERVED, /* Stripe stroke */
  RM_SB_R = 55, RM_SB_G, RM_SB_B, RM_SB_OP, RM_SB_RESERVED, /* Stripe bg */
}

// requires width, height, orientation (0..3), array of meterRenderParams, level value, hold_level value
void renderMeter(Kt::Graphics::Context@ ctx, double w, double h, double orientation, array<double> &in m, double level, double level_hold) {

  /* calculate coordinates */
  double style = m[RM_STYLE];
  double horizontal = 0, reverse = 0;
  if ((orientation == 1) or (orientation == 3)) { horizontal = 1; }
  if (orientation > 1.5) { reverse = 1; }
  
  double pad_left = m[RM_INNER_PAD_H];
  double pad_top = m[RM_INNER_PAD_V];
  if (horizontal > 0.5) {
    pad_top = m[RM_INNER_PAD_H];
    pad_left = m[RM_INNER_PAD_V];
  }
  double pad_right = pad_left;
  double pad_bottom = pad_top;
  double min_level=m[RM_MIN_VAL]; 
  double max_level=m[RM_MAX_VAL];
  double lev_bg_opacity=m[RM_LEVEL_BG_OP];
  double pad_v=pad_top+pad_bottom, pad_h=pad_left+pad_right;
  double hl = h-pad_v, wl = w-pad_h; /* height or width of levels */
  bool stroke_horizontal=(horizontal>0.5);
  double expand = m[RM_STROKE_EXPAND];
  double led_round = m[RM_LED_ROUND];
  double led_stroke_width = 0.01;

  if (expand < 0) { stroke_horizontal = !stroke_horizontal; expand=abs(expand); }
  double g1, g2, g3, g4, r1, r2, r3, r4, hl1, hl2, hl3, hl4;

  /* fill back and stroke around all */
  ctx.path.Clear();
  ctx.settings.set_lineWidth(m[RM_STROKE_WIDTH]);
  double s=m[RM_STROKE_ROUND]/2, sw2 = m[RM_STROKE_WIDTH]/2; // s - roundness size, sw2 - strokewidth/2
  if (s > (hl/2)) s = (hl/2);
  if (s > (wl/2)) s = (wl/2);
  double x=s+sw2, y=sw2;
  if (!stroke_horizontal) { x -= expand; } else { y-=expand; }
  ctx.path.MoveTo(x,y);
  ctx.path.ArcNegative(x,y+s, s, 270, 180 ); 
  x-=s; y=h-s-sw2;
  if (stroke_horizontal) { y+=expand; }
  ctx.path.LineTo(x, y);
  ctx.path.ArcNegative(x+s, y, s, 180, 90 ); 
  y+=s; x=w-s-sw2;
  if (!stroke_horizontal) { x+=expand; }
  ctx.path.LineTo(x,y);
  ctx.path.ArcNegative(x,y-s, s, 90, 0); x+=s;
  y = s+sw2;
  if (stroke_horizontal) { y-=expand; }
  ctx.path.LineTo(x, y);
  ctx.path.ArcNegative(x-s,y, s, 0, -90); 
  ctx.path.Close();
  ctx.source.SetRGBA(m[RM_SB_R], m[RM_SB_G], m[RM_SB_B], m[RM_SB_OP]); 
  ctx.FillPath(); 
  ctx.source.SetRGBA(m[RM_SS_R], m[RM_SS_G], m[RM_SS_B], m[RM_SS_OP]);
  ctx.StrokePath();
    
  // limit level and hold level
  if (level < min_level) level = min_level;
  if (level > max_level) level = max_level;
  if (level_hold != UNKNOWN_VALUE) {
    if (level_hold < min_level) level_hold = min_level;
    if (level_hold > max_level) level_hold = max_level;
  }

  // vertical meters (standard)
  if (horizontal < 0.5) {
    double multfactor = (hl)/(max_level - min_level);
    double ys=(-level+max_level)*multfactor;
    double ysh=(-level_hold+max_level)*multfactor;
    g1 = pad_left; g3 = pad_left; g2 = pad_top; g4 = h-pad_bottom;
    r1 = pad_left; r2 = ys+pad_top; r3 = w-pad_h; r4 = hl-ys;
    /* vertical reversed */
    if (reverse > 0.5) {
      g2 = h-pad_bottom;
      g4 = pad_top;
      r2 = pad_top;
      ys = -(min_level-level)*multfactor;
      ysh = -(min_level-level_hold)*multfactor;
      r4 = ys;
    }
    /* hold level */
    hl1=pad_left; hl2=ysh+pad_top; hl3=w-pad_right; hl4=ysh+pad_top;
  }

  if (horizontal>0.5) {
    double multfactor = (wl)/(max_level - min_level);
    double xsh = pad_left+(level_hold-min_level)*multfactor;
    g1 = w-pad_right; g2 = pad_top; g3 = pad_left; g4 = pad_top; 
    r1 = pad_left; r2 = pad_top; r3 = (level-min_level)*multfactor; r4 = h-pad_v;
    if (reverse > 0.5) {
      g1 = pad_left; g3 = w-pad_right;
      r1 = pad_left+wl-(level-min_level)*multfactor; r3 = (level-min_level)*multfactor;
      xsh = w-pad_right-(level_hold-min_level)*multfactor;
    }
    hl1=xsh; hl2=pad_top; hl3=xsh; hl4=h-pad_bottom;
  }

  bool flat = ((style > 0.5) and (style < 1.5));
  bool flat_with_bg = (flat and (lev_bg_opacity>0.001));
  bool leds = (style > 1.5);



  if (leds) {
    int desired_leds_count = int(m[RM_LED_DESIRED_QTY]);
    double min_led_spacing = m[RM_LED_SPACING];
    double led_spacing = min_led_spacing;
    double min_led_size = m[RM_LED_MIN_SIZE];
    double max_led_size = m[RM_LED_MAX_SIZE];
    double mlspace = h-pad_v; if (horizontal>0.5) mlspace = w-pad_h;
    double led_size;
    int leds_count = desired_leds_count+1;
    if (max_led_size < min_led_size) max_led_size = min_led_size;
    
    if (min_led_size < max_led_size){
      // make sure LED min-height is preserved (reducing LEDs count)
      do { 
        leds_count--;
        led_size = (mlspace - (leds_count-1)*led_spacing)/leds_count;
      } while( led_size < min_led_size );
      // make sure LED max-height is preserved (increasing LEDs count)
      leds_count--;
      do { 
        leds_count++;
        led_size = (mlspace - (leds_count-1)*led_spacing)/leds_count;
      } while( led_size > max_led_size );

      led_size = (mlspace - (leds_count-1)*led_spacing)/leds_count;
    } else {
      // if min and max height are the same, adjust spacing
      led_size = min_led_size;
      // reduce leds count to preserve min spacing
      do { 
        leds_count--;
        led_spacing = (mlspace - leds_count*led_size)/(leds_count-1);
      } while( led_spacing < min_led_spacing );
    }



    double led_pos = pad_top; if (horizontal>0.5) led_pos = pad_left;
    double led_pos_perc = 0, led_pos_next_perc = 0; // from 0 to 1
    double lev_perc = (level-min_level)/(max_level - min_level);
    double lev_hold_perc = (level_hold-min_level)/(max_level - min_level);
    int led_hold_no = int(lev_hold_perc*(leds_count-1.0) +.5);
    if (lev_hold_perc < 0.000001) led_hold_no = -1; // dont show hold led on min level
    double r, g, b, a;
    double step_size;
    bool show_led = false;
    for(int i=0;i<leds_count;i++) {
      led_pos_perc = i/float(leds_count);
      led_pos_next_perc = (i+1)/float(leds_count);
      step_size = led_pos_next_perc-led_pos_perc;
      if ((1-led_pos_perc) <= m[RM_LM_POS]+0.00005) {
        r = m[RM_LH_R]; g = m[RM_LH_G]; b = m[RM_LH_B]; a = m[RM_LH_OP];
      } else if ((1-led_pos_perc) <= m[44]+0.00005) {
        r = m[RM_LM_R]; g = m[RM_LM_G]; b = m[RM_LM_B]; a = m[RM_LM_OP];
      } else {
        r = m[RM_LL_R]; g = m[RM_LL_G]; b = m[RM_LL_B]; a = m[RM_LL_OP];
      }

      // define current rect coords
      if (horizontal<0.5) {
        r1 = pad_left; r2 = h-led_pos-led_size; r3 = w-pad_h; r4 = led_size;
        if (reverse > 0.5) { r2 = led_pos; }
      } else {
        r1 = led_pos; r2 = pad_top; r3 = led_size; r4 = h-pad_v;
        if (reverse > 0.2) { r1 = w-led_pos-led_size; }
      }

      // limit roundness to half of size
      if (led_round > (r3/2)) led_round = (r3/2);
      if (led_round > (r4/2)) led_round = (r4/2);
      if (led_round < 0.5) led_round = 0;

      // if we need to draw LED backgrounds
      if (lev_bg_opacity > 0.001) {
        ctx.path.Clear();
        ctx.source.SetRGBA(r, g, b, lev_bg_opacity*a);
        if (led_round == 0) { // simple rectangle
          ctx.path.Rectangle(r1, r2, r3, r4); 
        } else { // rounded rectancle
          double tro=led_round; 
          double tsw=led_stroke_width, tsw2=tsw/2, tx=r1+tro+tsw2, ty=r2+tsw2;
          ctx.path.MoveTo(tx,ty);
          ctx.path.ArcNegative(tx,ty+tro, tro, 270, 180 ); tx-=tro; ty=ty+r4-tro-tsw2;
          ctx.path.LineTo(tx, ty);
          ctx.path.ArcNegative(tx+tro, ty, tro, 180, 90 ); ty+=tro; tx=tx+r3-tro-tsw2;
          ctx.path.LineTo(tx,ty);
          ctx.path.ArcNegative(tx,ty-tro, tro, 90, 0); tx+=tro; ty = r2+(tro+tsw2);
          ctx.path.LineTo(tx, ty);
          ctx.path.ArcNegative(tx-tro,ty, tro, 0, -90); 
          ctx.path.Close();
        }
        ctx.FillPath();
        // ctx.source.SetRGBA(1, 1, 1, lev_bg_opacity);
        // ctx.StrokePath();
      }
      // now decide whether to draw LED itself
      double a_ = a;
      show_led = false;
      if (lev_perc > led_pos_next_perc) {
        show_led = true; // surely visible
      } else if (lev_perc > led_pos_perc) {
        show_led = true;
        a = a_*(lev_perc-led_pos_perc)/(led_pos_next_perc-led_pos_perc);
      }
      // if we need to show HOLD level
      if ((m[RM_HO_OP] > 0.005) and (i == led_hold_no)){
        show_led = true;
        a = m[RM_HO_OP]*a_; // hold led opacity
      }
      
      // now draw LED itself
      if (show_led) {
        ctx.path.Clear();
        ctx.source.SetRGBA(r, g, b, a);
        if (led_round == 0) { // simple rectangle
          ctx.path.Rectangle(r1, r2, r3, r4); 
        } else { // rounded rectancle
          double tro=led_round; 
          double tsw=led_stroke_width, tsw2=tsw/2, tx=r1+tro+tsw2, ty=r2+tsw2;
          ctx.path.MoveTo(tx,ty);
          ctx.path.ArcNegative(tx,ty+tro, tro, 270, 180 ); tx-=tro; ty=ty+r4-tro-tsw2;
          ctx.path.LineTo(tx, ty);
          ctx.path.ArcNegative(tx+tro, ty, tro, 180, 90 ); ty+=tro; tx=tx+r3-tro-tsw2;
          ctx.path.LineTo(tx,ty);
          ctx.path.ArcNegative(tx,ty-tro, tro, 90, 0); tx+=tro; ty = r2+(tro+tsw2);
          ctx.path.LineTo(tx, ty);
          ctx.path.ArcNegative(tx-tro,ty, tro, 0, -90); 
          ctx.path.Close();
        }
        ctx.FillPath();
        //ctx.settings.set_lineWidth(1);
        //ctx.source.SetRGBA(1, 1, 1, 0.5);
        //ctx.StrokePath();
      }
      led_pos = led_pos + led_spacing+led_size;
    }

  } else {

    // basic gradient and flat styles
    if (flat_with_bg) {
      // if we need to draw level bg
      auto gradlb = ctx.get_patterns().NewLinearGradient(g1, g2, g3, g4);
      double op3 = m[RM_LL_OP]*lev_bg_opacity;
      double op2 = m[RM_LM_OP]*lev_bg_opacity;
      double op1 = m[RM_LH_OP]*lev_bg_opacity;
      gradlb.AddColorStopRGBA(m[RM_LL_POS], m[RM_LL_R], m[RM_LL_G], m[RM_LL_B], op3);
      gradlb.AddColorStopRGBA(m[RM_LL_POS]-0.0015, m[RM_LM_R], m[RM_LM_G], m[RM_LM_B], op2);
      gradlb.AddColorStopRGBA(m[RM_LM_POS], m[RM_LM_R], m[RM_LM_G], m[RM_LM_B], op2);
      gradlb.AddColorStopRGBA(m[RM_LM_POS]-0.0015, m[RM_LH_R], m[RM_LH_G], m[RM_LH_B], op1);
      gradlb.AddColorStopRGBA(m[RM_LH_POS], m[RM_LH_R], m[RM_LH_G], m[RM_LH_B], op1);
      gradlb.SelectAsSource(); 
      ctx.path.Clear();
      ctx.path.Rectangle(pad_left, pad_top, w-pad_h, h-pad_v);
      ctx.FillPath();
    }


    /* draw main level bar */
    auto gradl = ctx.get_patterns().NewLinearGradient(g1, g2, g3, g4);
    gradl.AddColorStopRGBA(m[RM_LL_POS], m[RM_LL_R], m[RM_LL_G], m[RM_LL_B], m[RM_LL_OP]);
      if (flat) { gradl.AddColorStopRGBA(m[RM_LL_POS]-0.0015, m[RM_LM_R], m[RM_LM_G], m[RM_LM_B], m[RM_LM_OP]); }
    gradl.AddColorStopRGBA(m[RM_LM_POS], m[RM_LM_R], m[RM_LM_G], m[RM_LM_B], m[RM_LM_OP]);
      if (flat) { gradl.AddColorStopRGBA(m[RM_LM_POS]-0.0015, m[RM_LH_R], m[RM_LH_G], m[RM_LH_B], m[RM_LH_OP]); }
    gradl.AddColorStopRGBA(m[RM_LH_POS], m[RM_LH_R], m[RM_LH_G], m[RM_LH_B], m[RM_LH_OP]);
    gradl.SelectAsSource(); 
    ctx.path.Clear();
    ctx.path.Rectangle(r1, r2, r3, r4);
    ctx.FillPath();

    
    /* now draw hold-level line */
    if (m[RM_HOLD_COLORIZE] < 0.5) ctx.source.SetRGBA(m[RM_HO_R], m[RM_HO_G], m[RM_HO_B], m[RM_HO_OP]); // if we need to set color
    ctx.settings.set_lineWidth(m[RM_HOLD_WIDTH]);
    ctx.path.Clear();
    ctx.path.MoveTo(hl1, hl2);
    ctx.path.LineTo(hl3, hl4);
    ctx.StrokePath();
  }
}

// METERS RELATED STUFF

/*
meterRenderParams structure (params follow the same order as in $render_params_string$ string):
(0-4) style;min_level;max_level;-;-;
(5-9) level_bg_opacity;desired_leds_count;min_led_size;max_led_size;led_spacing
(10-14) hold_line_width;hold_line_use_color;inner_pad_h;inner_pad_v;-;
(15-19) stroke_width;stroke_expand;stroke_round;led_round;-;
(20-29) -;-;-;-;-;-;-;-;-;-;
(30-...) colors[color[r,g,b];opacity;shift];
*/
array<double> meterRenderParams(80);

void meters_prepareParams(string render_params_string, array<double> & m){
  array<string> ar(m.length); // at least this size (m is bigger)
  ar = render_params_string.split(";");
  // reading normal params as float
  for (int i=0;i<30;i++) {
    if (i+1 >= int(ar.length)) break;
    m[i] = parseFloat(ar[i]);
  }
  // now reading colors and splitting them into r, g, b values
  int cs = 30; // colors start index
  for (int i=0;i<6;i++) {
    if (cs+i*3+3 >= int(ar.length)) break;
    meters_prepareParams_addColor(m, i, ar[cs+i*3], ar[cs+i*3+1], ar[cs+i*3+2]);
  }
}

// convert HEX-color to color usable for CANVAS renderer
void meters_prepareParams_addColor(array<double> & m, int i, string hexcolor, string op = "1", string pos = "0"){
  double _r, _g, _b;
  if (hexcolor == "") hexcolor = "#777777";
  if (hexcolor.substr(0,1) != "#") hexcolor = "#"+hexcolor;
  if (op == "") op = "1";
  if (pos == "") pos = "0";
  int cl = 5; // color length (r, g, b, op, pos)
  int cs = 30; // colors start index in array
  m[cl*i+cs+0] = double(parseInt(hexcolor.substr(1,2),16))/255.0;
  m[cl*i+cs+1] = double(parseInt(hexcolor.substr(3,2),16))/255.0;
  m[cl*i+cs+2] = double(parseInt(hexcolor.substr(5,2),16))/255.0;
  m[cl*i+cs+3] = parseFloat(op);
  m[cl*i+cs+4] = parseFloat(pos)/100;
}

///////////////////////
// BODY SHADER OBJECT
///////////////////////

enum bodyShaderParams{
  BS_OPACITY,
  BS_STYLE,
  BS_X1, 
  BS_Y1, 
  BS_R1,
  BS_X0, 
  BS_Y0, 
  BS_R0, 
  BS_LINK_LS
}

enum bodyShaderStyles{
  BS_S_RADIAL,
  BS_S_LINEAR
}

class bodyShaderClass{
  double x1, y1, r1; // outer circle
  double x0, y0, r0; // inner circle
  double op;
  int style, link_ls;
  bool preview_mode = false;

  bodyShaderClass(string bsdconst, Kt::String@ bsdparam, bool preview_mode = false){
    bsdparam = bsdconst;
    parseData(bsdparam);
    this.preview_mode = preview_mode;
  }

  void parseData(string s_params) {
    array<string> ar = s_params.split(";");
    ar.resize(BS_LINK_LS+1);
    this.style = parseInt(ar[BS_STYLE]);
    this.x1 = f(ar[BS_X1]);
    this.y1 = f(ar[BS_Y1]);
    this.r1 = f(ar[BS_R1]);
    this.x0 = f(ar[BS_X0]);
    this.y0 = f(ar[BS_Y0]);
    this.r0 = f(ar[BS_R0]);
    this.op = f(ar[BS_OPACITY]);
    this.link_ls = parseInt(ar[BS_LINK_LS]);
  }

  void Draw(double h, double w){  
    // double maxsize = h; if (w>maxsize) maxsize = w;
    
    auto ctx=Kt::Graphics::GetCurrentContext();
    ctx.settings.set_blendMode(Kt::Graphics::kDrawOpXor);

    double xc = w*0.5, yc = h*0.5;
    double xi = xc+x0, yi = yc+y0;

    if (!preview_mode) {
      // in preview theme mode ignore light linking
      if (link_ls > 1) {
        // linked to individual light sources A, B, C
        array<array<double>>@ lights = Light.sources;
        int nsources = lights.length;
        int source_no = link_ls-2;
        xi = xc;
        yi = yc;
        if (source_no < nsources) {
          if (lights[source_no][LS_ENABLED] > 0) {
            xi = xc+lights[source_no][LS_X];
            yi = yc+lights[source_no][LS_Y];
          }
        }
      } else if (link_ls == 1) {
        // linked to combined light source
        xi = xc+Light.CL_x;
        yi = yc+Light.CL_y;
        //status(""+Light.CL_x+","+Light.CL_y);
      }
    }

    if (style == BS_S_RADIAL) {
      auto g = ctx.patterns.NewRadialGradient(xc+x1, yc+y1, r1, xi, yi, r0);
      g.AddColorStopRGBA(0, 1, 1, 1, op);
      g.AddColorStopRGBA(1, 1, 1, 1, 0);
      g.SelectAsSource(); 
    } else {
      auto g = ctx.patterns.NewLinearGradient(xc+x1, yc+y1, xi, yi);
      g.AddColorStopRGBA(0, 1, 1, 1, op);
      g.AddColorStopRGBA(1, 1, 1, 1, 0);
      g.SelectAsSource(); 
    }
    
    ctx.path.Clear();
    ctx.path.Rectangle(0,0,w,h);
    ctx.FillPath();
    
    ctx.settings.set_blendMode(Kt::Graphics::kDrawOpOver);
  }
}

///////////////////////
// SIMPLE RENDER KNOB OBJECT
///////////////////////


  enum SimpleKnobParams{
    RSK_ANGLE_START, RSK_ANGLE_END, RSK_BODY_COLOR, RSK_BODY_OPACITY, RSK_BODY_SIZE, RSK_MARKER_START, RSK_MARKER_END, RSK_MARKER_WIDTH, RSK_MARKER_COLOR, RSK_MARKER_OPACITY, RSK_MARKER_TYPE, RSK_MARKER_STROKE_WIDTH
  }

  enum SimpleKnobMarkers { RSK_MT_NONE, RSK_MT_LINE, RSK_MT_ROUNDED, RSK_MT_ROUNDED_FILLED, RSK_MT_CIRCLE, RSK_MT_CIRCLE_FILLED }

  class SimpleKnob{
    double marker_start, marker_end, marker_width = 3, marker_stroke_width;
    double angle_start = -135, angle_end = 135, angle_center = 0, angle_width = 270;
    double cw, ch, cw2, ch2, body_radius;
    double bodyR, bodyG, bodyB, bodyA;
    double markerR, markerG, markerB, markerA;
    int marker_type = 1;

    // constructor
    SimpleKnob(double size, string s_params){
      this.cw = size;
      this.ch = size;
      cw2 = cw/2; ch2 = ch/2;

      // parse params array
      array<string> ar = s_params.split(";");

      body_radius = cw2*f(ar[RSK_BODY_SIZE]);
      marker_width = size*0.1*f(ar[RSK_MARKER_WIDTH]);
      marker_start = cw2*(1-f(ar[RSK_MARKER_START]))*f(ar[RSK_BODY_SIZE]);
      marker_end = cw2*(1-f(ar[RSK_MARKER_END]))*f(ar[RSK_BODY_SIZE]);
      marker_type = parseInt(ar[RSK_MARKER_TYPE]);
      marker_stroke_width = f(ar[RSK_MARKER_STROKE_WIDTH])*0.025*size;

      // calculate working angles
      angle_start = f(ar[RSK_ANGLE_START]);
      angle_end = f(ar[RSK_ANGLE_END]);
      if (angle_end < angle_start) {
        angle_start = -f(ar[RSK_ANGLE_START]);
        angle_end = -f(ar[RSK_ANGLE_END]);
      }

      angle_width = (angle_end-angle_start);
      angle_center = 90 - (angle_start+angle_end)/2 ; // center with shift (in normal grads)

      bodyR = double(parseInt(ar[RSK_BODY_COLOR].substr(1,2),16))/255.0;
      bodyG = double(parseInt(ar[RSK_BODY_COLOR].substr(3,2),16))/255.0;
      bodyB = double(parseInt(ar[RSK_BODY_COLOR].substr(5,2),16))/255.0;
      bodyA = f(ar[RSK_BODY_OPACITY]);

      markerR = double(parseInt(ar[RSK_MARKER_COLOR].substr(1,2),16))/255.0;
      markerG = double(parseInt(ar[RSK_MARKER_COLOR].substr(3,2),16))/255.0;
      markerB = double(parseInt(ar[RSK_MARKER_COLOR].substr(5,2),16))/255.0;
      markerA = f(ar[RSK_MARKER_OPACITY]);
    }

    // rendering function
    void render(double nval){
      auto ctx=Kt::Graphics::GetCurrentContext();
      if (nval<0) nval=0;
      if (nval>1) nval=1;
      double nvalc = nval-0.5; // centered normalized value (-0.5 .. 0.5)
      double adeg = (angle_center - nvalc*angle_width);
      double arad = adeg*pi/180;

      double si = sin(arad), co = cos(arad);
      double dxme = cw2 + co*marker_end;
      double dyme = ch2 - si*marker_end;
      double dxms = cw2 + co*marker_start;
      double dyms = ch2 - si*marker_start;
      double mdeg = 270-adeg;

      // draw body
      ctx.path.Clear();
      ctx.source.SetRGBA(bodyR, bodyG, bodyB, bodyA);
      ctx.path.Arc(cw2, ch2, body_radius, 0.001, 0);
      ctx.FillPath();

      // draw marker
      ctx.path.Clear();
      ctx.source.SetRGBA(markerR, markerG, markerB, markerA);
      
      switch(marker_type) {
        // rounded or circle marker
        case RSK_MT_ROUNDED:
        case RSK_MT_ROUNDED_FILLED:
        case RSK_MT_CIRCLE:
        case RSK_MT_CIRCLE_FILLED:
          if ((marker_type == RSK_MT_CIRCLE) or (marker_type == RSK_MT_CIRCLE_FILLED)) {
            ctx.path.Arc(dxms, dyms, marker_width*0.5, 0.001, 0);
          } else {
            ctx.path.Arc(dxms, dyms, marker_width*0.5, mdeg, mdeg+180);
            ctx.path.Arc(dxme, dyme, marker_width*0.5, mdeg+180, mdeg+360);
          }
          ctx.path.Close();
          if ((marker_type == RSK_MT_ROUNDED_FILLED) or (marker_type == RSK_MT_CIRCLE_FILLED)) {
            ctx.FillPath();
          } else {
            ctx.settings.set_lineWidth(marker_stroke_width);
            ctx.StrokePath();
          }
          break;

        default:
        // simple line marker
          ctx.settings.set_lineWidth(marker_width);
          ctx.path.MoveTo(dxme, dyme);
          ctx.path.LineTo(dxms, dyms);
          ctx.StrokePath(); 
      }
    }
  }

}