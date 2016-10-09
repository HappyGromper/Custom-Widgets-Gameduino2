#include <EEPROM.h>
#include <SPI.h>
#include <GD2.h>

static uint16_t valueDi = 15000;
static uint16_t valueSl = 15000;  
static uint16_t valueTo = 15000;  
// every widget is hooked to this value
static char message[41];            // 40 character text entry field
static uint16_t options = OPT_FLAT;
static byte prevkey;

void setup()
{
  memset(message, 7, 40);
  GD.begin();
}

#define TAG_DIAL      200
#define TAG_SLIDER    201
#define TAG_TOGGLE    202
#define TAG_BUTTON1   203
#define TAG_BUTTON2   204

void loop()
{

  GD.get_inputs();

  switch (GD.inputs.track_tag & 0xff) {
  case TAG_DIAL:
    valueDi = GD.inputs.track_val;
  }
  switch (GD.inputs.track_tag & 0xff) {
  case TAG_SLIDER:
    valueSl = GD.inputs.track_val;
  }
  switch (GD.inputs.track_tag & 0xff) {
  case TAG_TOGGLE:
    valueTo = GD.inputs.track_val;
  }
  
  switch (GD.inputs.tag) {
  case TAG_BUTTON1:
    options = OPT_FLAT;
    break;
  case TAG_BUTTON2:
    options = 0;
    break;
  }
  byte key = GD.inputs.tag;
  if ((prevkey == 0x00) && (' ' <= key) && (key < 0x7f)) {
    memmove(message, message + 1, 39);
    message[39] = key;
  }
  prevkey = key;

  GD.cmd_gradient(0, 0,   0x404044,
                  480, 480, 0x606068);
  GD.ColorRGB(0x00002B);

  GD.LineWidth(4 * 16);
  GD.Begin(RECTS);

  GD.Vertex2ii(8, 8);
  GD.Vertex2ii(128, 128);

  GD.Vertex2ii(8, 136 + 8);
  GD.Vertex2ii(128, 136 + 128);

  GD.Vertex2ii(144, 136 + 8);
  GD.Vertex2ii(472, 136 + 128);
  GD.ColorRGB(0x0055D4);

  GD.Tag(TAG_DIAL);
  GD.cmd_dial(68, 68, 50, options, valueDi);
  GD.cmd_track(68, 68, 1, 1, TAG_DIAL);

  GD.Tag(TAG_SLIDER);
  GD.cmd_slider(16, 199, 104, 10, options, valueSl, 65535);
  GD.cmd_track(16, 199, 104, 10, TAG_SLIDER);
  

  GD.Tag(TAG_TOGGLE);
  GD.cmd_toggle(360, 62, 80, 29, options, valueTo,
                "2D" "\xff" "3D");
  GD.cmd_track(360, 62, 80, 20, TAG_TOGGLE);
if (valueTo == 65535){
    options = OPT_FLAT;
    } else if (valueTo == 0) {
    options = 0;
    }
  GD.Tag(255);
  GD.cmd_number(68, 136, 30, OPT_CENTER | 5, valueDi);
  GD.cmd_number(68, 245, 30, OPT_CENTER | 5, valueSl);
  GD.cmd_gauge(184, 48, 40, options, 4, 3, valueSl, 65535);
  GD.cmd_gauge(280, 48, 40, options, 4, 3, valueDi, 65535);

  GD.Tag(TAG_BUTTON1);
  GD.cmd_button(352, 12, 40, 30, 28, options,  "2D");
  GD.Tag(TAG_BUTTON2);
  GD.cmd_button(400, 12, 40, 30, 28, options,  "3D");

  GD.Tag(255);
  GD.cmd_progress(144, 100, 320, 10, options, valueDi, 65535);
  GD.cmd_progress(144, 120, 320, 10, options, valueSl, 65535);

  GD.cmd_keys(144, 168,      320, 24, 28, options | OPT_CENTER | key, "qwertyuiop");
  GD.cmd_keys(144, 168 + 26, 320, 24, 28, options | OPT_CENTER | key,   "asdfghjkl");
  GD.cmd_keys(144, 168 + 52, 320, 24, 28, options | OPT_CENTER | key,   "zxcvbnm,.");
  GD.Tag(' ');
  GD.cmd_button(308 - 60, 172 + 74, 120, 20, 28, options,   "");

  GD.BlendFunc(SRC_ALPHA, ZERO);
  GD.cmd_text(149, 146, 18, 0, message);

  GD.swap();
}
