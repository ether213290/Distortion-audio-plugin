////////////////////////////////////
// LIBRARY OF COMMON FUNCTIONS 
// INDEPENDENT, USED BY MANY PIECES OF CODE
////////////////////////////////////

// often used constants
const double UNKNOWN_VALUE = -99999999;
const double pi = 3.141592653589793238462;
const double pi2 = pi/2;
const double twopi = pi*2;
const double sqrt2rev = 0.707106781186;
const double sqrt2 = sqrt(2);

////////////////////////////////////
// HELPER FUNCTIONS
////////////////////////////////////

string getDirForFile(string filename, bool with_ending_slash = true) {
  confirmPath(filename);
  int shift = 0;
  if (with_ending_slash) shift = 1;
  return filename.substr(0, filename.findLast("/")+shift); 
}

string getBaseFilename(string filename, bool including_ext = true) {
  confirmPath(filename);
  string res = filename.substr(filename.findLast("/")+1);
  if (!including_ext) {
    res = strTill(res, ".");
  }
  return res; 
}

string getFileRelToDir(string filename, string dir, int shift = -1) {
  int pos = filename.findFirst(dir);
  if (pos == 0) {
    return filename.substr(dir.length+shift);
  } else {
    return filename;
  }
}

bool isRelativePath(string path){
  confirmPath(path);
  if ((path.findFirst("/") < 0) or (path.findFirst("/") > 3) or (path[0]=='.')) {
    return true;
  } else {
    return false;
  }
}

void confirmPath(string & path){
  replaceString(path, "\\", "/");
  replaceString(path, "//", "/");
  // replacing .. dirs if needed
  if (path.findFirst("..") >-1) {
    array<string> pieces = path.split("/");
    array<string> pieces_res;
    if (pieces.length>0) {
      string prev_piece = "";
      pieces_res.insertLast(pieces[0]);
      for(uint i=1;i<pieces.length;i++) {
          if (pieces[i] == "..") {
            pieces_res.removeLast();
            continue;
          };
          pieces_res.insertLast(pieces[i]);
      }
      path = join(pieces_res, "/");
    }
  }
}

int copyFile(string src, string dst){
  filesystem fs;
  return fs.copyFile(src,dst);
}

/*
int copyFileMacOld(string src, string dst){
  file srcFile;
  if (srcFile.open(src, "r") >= 0) {
    file destFile;
    if (destFile.open(dst, "w") >= 0) {
      while (!srcFile.isEndOfFile()) {
          destFile.writeUInt(srcFile.readUInt(1), 1);
      }
      destFile.close();
    } else return -1;
    srcFile.close();
  } else return -1;
  return 0;
}
*/

// get current time in format like 21:33:44
string getHMS(){
    datetime d;
    string s = formatFloat(d.get_hour(),"0", 2, 0) + ":" + formatFloat(d.get_minute(),"0", 2, 0) + ":" + formatFloat(d.get_second(),"0", 2, 0);
    // print("time: " + s);
    return s;
}
// get current date in format yyyy-mm-dd
string getYMD(){
    datetime d;
    string s = formatFloat(d.get_year(),"0", 4, 0) + "-" + formatFloat(d.get_month(),"0", 2, 0) + "-" + formatFloat(d.get_day(),"0", 2, 0);
    // print("date: " + s);
    return s;
}

double round(double d, double p = 2) {
    double x = pow(10, p);
    // if ((abs(d)<0.5) and (p==0)) return 0;
    double r = floor(d*x+0.5)/x ;
    //print("x: " + x + ", r: " + r);
    return r;
}

int roundDoubleToInt(double d){
 if(d<0)
     return int(d-.5);
 else
     return int(d+.5);
}

int rint(double d){
 if(d<0)
     return int(d-.5);
 else
     return int(d+.5);
}

// shortcut to parseFloat
double f(string s) { return parseFloat(s); }

// denormalization
double de(double d){
  if (! (d < -1.0e-8 || d > 1.0e-8)) d = 0;
  return d;
}

void denorm(double & d){
  if (! (d < -1.0e-8 || d > 1.0e-8)) d = 0;
}

// calculate how much angles differ (keeping sign, range from -pi to pi)
double angDiffR(double a1, double a2){
  if (a1>pi) a1-=twopi;
  if (a1<-pi) a1+=twopi;
  if (a2>pi) a2-=twopi;
  if (a2<-pi) a2+=twopi;
  double adiff = (a1-a2);
  if (abs(adiff) > pi) {
    a1+=twopi;
    adiff = (a1-a2);
    if (abs(adiff) > pi) {
      a1-=twopi;
      a2+=twopi;
      adiff = (a1-a2);
    }
  }
  return adiff;
}

void replaceString(string & ioString,const string &in stringToFind,const string &in replaceString) {
  if (ioString.length == 0) return;
  array<string>@ pieces=ioString.split(stringToFind);
  if(@pieces!=null) {
    if(pieces.length>0) ioString=pieces[0];
    for(uint i=1;i<pieces.length;i++) {
        ioString+=replaceString;
        ioString+=pieces[i];
    }
  }
}

string strtolower(string s){
  for(uint i=0;i<s.length;i++){
    if ((s[i]>=65) and (s[i]<=90)) s[i]=s[i]+32;
  }
  return s;
}

// utility to trim strings
string trim(string s, string char = "") {
  if (s.length < 1) return "";
  int first_not_empty = s.findFirstNotOf(" \n\r\t");
  if (first_not_empty > 0) {
    s.erase(0, first_not_empty);
  }
  /* findLastNotOf not yet working
  int last_not_empty = s.findLastNotOf(" \n\r");
  if (last_not_empty > 0) {
    s.erase(last_not_empty, 1000);
  } */
  for(int n=int(s.length)-1; n>=0; n--) {
    int tc = s[n];
    // if ((tc == 32) or (tc == 13) or (tc == 10) or (tc == 9)) 
    if (tc<33) {
      s.erase(n);
    } else {
      break;
    }
  }

  return s;
}

// prepares string to put into XML file
string escape(string s){
  string se = s;
  replaceString(se, "<", "&lt;");
  replaceString(se, "\"", "&quot;");
  return se;
}

string escapeq(string s){
  string se = escape(s);
  replaceString(se, "'", "`");
  return se;
}

string str_repeat(string s, int repeat){
  string r;
  for(int n=0;n<repeat;n++) {
    r = r+s;
  }
  return r;
}



string strFrom(string s, string from, int shift = 0){
  int pos = s.findFirst(from);
  if (pos >-1) return s.substr(pos+shift);
  return "";
}

string strFrom(string s, array<string> from_ar, int shift = 0){
  int pos = findFirstAny(s, from_ar);
  if (pos >-1) return s.substr(pos+shift);
  return "";
}

string strTill(string s, string till, int shift = 0){
  int pos = s.findFirst(till);
  if (pos >-1) return s.substr(0, pos+shift);
  return s;
}

string strTill(string s, array<string> till_ar, int shift = 0){
  int pos = findFirstAny(s, till_ar);
  if (pos >-1) return s.substr(0, pos+shift);
  return s;
}



int findFirstAny(string s, array<string> needles_ar, int lastpos = 0){
  int pos = -1, best_pos = -1;
  for(uint i=0;i<needles_ar.length;i++) {
    pos = s.findFirst(needles_ar[i], lastpos);
    if ((pos >=0) and ((best_pos > pos) or (best_pos < 0))) best_pos = pos;
  }
  return best_pos;
}

int removeFromArray(array<string> & list, string &in value){
    int pos = list.find(value);
    if (pos>-1) {
        list.removeAt(pos);
    }
    return pos;
}

//////////////
// color conversion

void convertRGBtoHSL(double &in r, double &in g, double &in b, double &out h, double &out s, double &out l){
   double cmax = r;
   if (g>cmax) cmax = g;
   if (b>cmax) cmax = b;
   double cmin = r;
   if (g<cmin) cmin = g;
   if (b<cmin) cmin = b;
   double delta = cmax-cmin;
   if (delta == 0) {
    h = 0;
   } else if (cmax == r) {
    h = 60 * (((g-b)/delta));
    if (g<b) h+=360;
   } else if (cmax == g) {
    h = 60 * (((b-r)/delta) + 2);
   } else {
    h = 60 * (((r-g)/delta) + 4);
   }
   l=(cmax+cmin)/2;
   if (delta == 0) {
    s = 0;
   } else {
    s = delta/(1-abs(2*l-1));
   }
}

// convert hsl to rgb (0..1)
void convertHSLtoRGB(double &in h, double &in s, double &in l, double &out r, double &out g, double &out b){
  double c = (1-abs(2*l-1))*s;
  double x = c*(1-abs(((h/60)%2) - 1));
  double m = l-c/2;
  if (h<60) {
    r = c; g = x; b = 0;
  } else if (h<120) {
    r = x; g = c; b = 0;
  } else if (h<180) {
    r = 0; g = c; b = x;
  } else if (h<240) {
    r = 0; g = x; b = c;
  } else if (h<300) {
    r = x; g = 0; b = c;
  } else {
    r = c; g = 0; b = x;
  }
  r += m; g += m; b += m;
}

//////////////

void saveDebug(string data, string filename = ""){
  file f;
  if (filename == "") filename = """$SKINS_DIR$debug.txt""";
  confirmPath(filename);
  datetime d;
  data = data+"\n\nSAVED: "+formatFloat(d.get_hour(),"0", 2, 0) + ":" + formatFloat(d.get_minute(),"0", 2, 0) + ":" + formatFloat(d.get_second(),"0", 2, 0);

  if (f.open(filename, "w") >= 0) {
    bool ok = (f.writeString(data) > 0);
    f.close();
  }
}

string cp866_cp1251( string & s ) {
  for (uint i=0; i<s.length; i++) {
    if ((s[i]>127) && (s[i]<176)) {
      s[i] += 64;
    } else if ((s[i]>223) && (s[i]<240)) {
      s[i] += 16;
    }
  }
  return s;
}

////////////
