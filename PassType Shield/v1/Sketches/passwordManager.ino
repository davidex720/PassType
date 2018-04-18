
//TODO add more settings
//TODO login/unlock
//TODO psw names with upper and lower case
//TODO accept only showrt password names
//TODO optimize memory
//TODO graphic revision for arrows
//TODO long press = multiple press
//TODO add more chars
//TODO standby function
//TODO get a better randPSW
//TODO RESET SYSTEM AFTER 3 WRONG LOGIN ATTEMPTS
//TODO add scrolling text for long names





#include <U8g2lib.h>
#include <Wire.h>
#include "Keyboard.h"


//DISPLAY CONSTRUCTOR
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, SCL, SDA, U8X8_PIN_NONE);
//U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R2, SCL, SDA, U8X8_PIN_NONE);



//EEPROM VALUE
#define disk1 0x50
#define byteResetValue 255
#define endAddressZone 255
#define dimPSW 40
#define startPSWzone 5000
#define endPSWzone 15200
#define settingStartAddress 32500

//Settings
#define firstUse 32501              //255 first use
#define pswLength 32502             //5,6,7,8,9,10,11,12,13,14,15,16  ---------default 10 value 255
#define pswType  32503              //1 only lowercase, 2 lower+upper case, 3 letters+numbers, 4 letters+numbers+symbols -------255 default letters+numbers

//JOYSTICK
#define upPin  4
#define centerPin  5
#define leftPin  6
#define rightPin  7
#define downPin  8

//setting per UI
#define longPressedTime 40
#define longPressQuant  7


#define spacing 12   //spacing
#define pswLength 10

int buttonState;
int lastButtonState = 0;
int cyclePressed = 0;



//BITMAPS
const u8g2_uint_t leftArrow[]  = {
  0x00,          // 00000000
  0x20,          // 00100000
  0x60,          // 01100000
  0xE0,          // 11100000
  0xE0,          // 11100000
  0x60,          // 01100000
  0x20,          // 00100000
  0x00,          // 00000000
};
const u8g2_uint_t rigthArrow[]  = {
  0x00,          // 00000000
  0x80,          // 10000000
  0xC0,          // 11000000
  0xE0,          // 11100000
  0xE0,          // 11100000
  0xC0,          // 11000000
  0x80,          // 10000000
  0x00,          // 00000000
};
const u8g2_uint_t downArrow[]  = {
  0x00,          // 00000000
  //0x7E,          // 01111110
  0x3C,          // 00111100
  0x18,          // 00011000
};
const u8g2_uint_t upArrow[]  = {
  0x18,          // 00011000
  0x3C,          // 00111100
  //0x7E,          // 01111110
  0x00,          // 00000000
};

const u8g2_uint_t center[]  = {
  0x3C,          // 00111100
  0x42,          // 01000010
  0x5A,          // 01011010
  0x5A,          // 01011010
  0x42,          // 01000010
  0x3C,          // 00111100
};

const u8g2_uint_t verticalTYPE[]  = { //TODO could use only letters ad mitmaps to optimize memory
  0x70,          // 01110000
  0x20,          // 00100000
  0x20,          // 00100000
  0x20,          // 00100000
  0x20,          // 00100000
  0x00,          // 00000000

  0x50,          // 01010000
  0x50,          // 01010000
  0x20,          // 00100000
  0x20,          // 00100000
  0x20,          // 00100000
  0x00,          // 00000000

  0x70,          // 01110000
  0x50,          // 01010000
  0x70,          // 01110000
  0x40,          // 01000000
  0x40,          // 01000000
  0x00,          // 00000000

  0x70,          // 01110000
  0x40,          // 01000000
  0x70,          // 01110000
  0x40,          // 01000000
  0x70,          // 01110000
  0x00,          // 00000000

};

const u8g2_uint_t verticalEDIT[]  = {
  0x70,          // 01110000
  0x40,          // 01000000
  0x70,          // 01110000
  0x40,          // 01000000
  0x70,          // 01110000
  0x00,          // 00000000

  0x60,          // 01100000
  0x50,          // 01010000
  0x50,          // 01010000
  0x50,          // 01010000
  0x60,          // 01100000
  0x00,          // 00000000

  0x20,          // 00100000
  0x20,          // 00100000
  0x20,          // 00100000
  0x20,          // 00100000
  0x20,          // 00100000
  0x00,          // 00000000

  0x70,          // 01110000
  0x20,          // 00100000
  0x20,          // 00100000
  0x20,          // 00100000
  0x20,          // 00100000
  0x00,          // 00000000

};

const u8g2_uint_t verticalBACK[]  = {
  0x70,          // 01110000
  0x48,          // 01001000
  0x70,          // 01110000
  0x48,          // 01001000
  0x70,          // 01110000
  0x00,          // 00000000

  0x20,          // 00100000
  0x50,          // 01010000
  0x70,          // 01110000
  0x50,          // 01010000
  0x50,          // 01010000
  0x00,          // 00000000

  0x30,          // 00110000
  0x40,          // 01000000
  0x40,          // 01000000
  0x40,          // 01000000
  0x30,          // 00110000
  0x00,          // 00000000

  0x40,          // 01000000
  0x50,          // 01010000
  0x60,          // 01100000
  0x50,          // 01010000
  0x48,          // 01001000
  0x00,          // 00000000

};

const u8g2_uint_t verticalOK[]  = {
  0x20,          // 00100000
  0x50,          // 01010000
  0x50,          // 01010000
  0x50,          // 01010000
  0x20,          // 00100000
  0x00,          // 00000000

  0x40,          // 01000000
  0x50,          // 01010000
  0x60,          // 01100000
  0x50,          // 01010000
  0x48,          // 01001000
  0x00,          // 00000000

};

const uint8_t logo[] PROGMEM  = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0x03, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x07, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x03, 0x00, 0x0e, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x01, 0x00, 0x1c, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0xe0, 0x00, 0x00, 0x38, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x70, 0x00, 0x00, 0x70, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x3f, 0x00, 0x00, 0xe0, 0x00
  , 0x00, 0x00, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0xfe, 0x1f, 0x00, 0x00, 0xc0, 0x00
  , 0x00, 0x00, 0xe0, 0x1f, 0x00, 0x00, 0x0e, 0x03, 0x00, 0x7e, 0xfe, 0x1f, 0x00, 0x00, 0xc0, 0x00
  , 0x00, 0x00, 0x64, 0x38, 0x00, 0x83, 0x0f, 0x07, 0x00, 0xff, 0x00, 0x1c, 0x00, 0x00, 0xc0, 0x00
  , 0x00, 0x00, 0x66, 0x30, 0x80, 0xc7, 0x03, 0xbf, 0x40, 0xc3, 0x7c, 0x1c, 0x00, 0xf8, 0xc0, 0x00
  , 0x00, 0x00, 0xe7, 0x9f, 0xc1, 0x6c, 0x00, 0xbf, 0x61, 0xc3, 0xfe, 0x1c, 0x00, 0x04, 0xc1, 0x00
  , 0x00, 0x80, 0xe7, 0x8f, 0x67, 0x60, 0x00, 0x83, 0x61, 0x7f, 0xc6, 0x1c, 0x00, 0x02, 0xc2, 0x00
  , 0x00, 0xc0, 0xe3, 0x00, 0x6f, 0xc0, 0x07, 0x83, 0x63, 0x3f, 0xc6, 0x1c, 0x00, 0x02, 0xc2, 0x00
  , 0x00, 0xe0, 0xe3, 0x00, 0xcc, 0x87, 0x0f, 0x03, 0x7f, 0x03, 0xfe, 0x1c, 0x00, 0x02, 0xc2, 0x00
  , 0x00, 0xe0, 0xe3, 0x00, 0x8c, 0x0f, 0x0c, 0x03, 0x7e, 0x03, 0x7e, 0x1c, 0x00, 0x02, 0xc2, 0x00
  , 0x00, 0xe0, 0xe3, 0xc0, 0x0f, 0x0c, 0x0c, 0x03, 0x60, 0x03, 0x06, 0x1c, 0x00, 0x02, 0xc2, 0x00
  , 0x00, 0xc0, 0xe3, 0x60, 0x0c, 0x0e, 0x0f, 0x07, 0x70, 0x03, 0x06, 0x1c, 0x00, 0x04, 0xc1, 0x00
  , 0x00, 0x80, 0xe7, 0x60, 0xec, 0xe7, 0x07, 0xbe, 0x3f, 0x03, 0x7c, 0x1c, 0x00, 0xf8, 0xc0, 0x00
  , 0x00, 0x00, 0xe7, 0xc0, 0xef, 0xe3, 0x01, 0xbc, 0x1f, 0x03, 0x7c, 0x1c, 0x00, 0x00, 0xc0, 0x00
  , 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0xc0, 0x00
  , 0x00, 0x00, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0x00, 0x00, 0xc0, 0x00
  , 0x00, 0x00, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0x00, 0x00, 0xe0, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x70, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x38, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x01, 0x00, 0x1c, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x03, 0x00, 0x0e, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x07, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0x03, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

};




























String text = "no";
int pointedAddr = 0;
int statusMenu = 0;
String addName;
int charInName = 0;
int charIndex = 0;
int wait = 5;
int fUse = 255;


char consentiti[] = {
  ' '                                                                                                                                            //  1
  , 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'             //+26
  , 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'             //+26
  , '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'                                                                                             //+10 =63
  , '?', '(', ',', ')', '/', '&', '%', '$', '<', '>', '!', ']', '+', '_', '*', '.', '}', '@', '^',                                                //+19 =82
  // _     )   ,    =   -     /    %    $    ;    :    !    +    ^    ?    (    .    *    "    &


};



void setup(void) {

  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);

  u8g2.begin();
  u8g2.setFont(u8g2_font_crox4h_tr);

  Serial.begin(9600);
  Wire.begin();
  Keyboard.begin();
  randomSeed(analogRead(0));

  fUse = (int) readEEPROM(firstUse);
  if (fUse == 255) {
    //TODO first use
    //TODO set to firs use done
  } else {
    //animation
  }

}

void loop(void) {

  if (wait == 0) {
  } else if (wait == 5) {
    wait++;
  } else if (wait == 6) {
    delay(2000);
    wait = 0;
  } else {
    delay(1000);
    wait = 0;
  }

  String n;
  String v;

  if (statusMenu <= 1) {
    if (simpleAutoReadPSW(pointedAddr, &n, &v)) {
      text = n;
      statusMenu = 1;
    } else {
      text = "New PSW";
      statusMenu = 2;
    }
  }

  debounceButtons();

  //0 nothing
  //1 left
  //2 up
  //3 right
  //4 down
  //5 center
  //6 dont know

  if (statusMenu == 1) {          //------------------------------------show psw name
    if (buttonState == 0) {
      //nothing
    } else if (buttonState == 1) { //left
      //TODO typing animation
      Keyboard.print(v);
    } else if (buttonState == 2) { //up
      if (pointedAddr > 0) {
        pointedAddr--;
      }
    } else if (buttonState == 3) { //right
      text = "Edit name";
      statusMenu = 4;
    } else if (buttonState == 4) { //down
      if (pointedAddr < 250) {
        pointedAddr++;
      }
    } else if (buttonState == 5) { //center
      statusMenu = 2;
    } else if (buttonState == 6) { //boh
    }


  } else if (statusMenu == 2) {    //-------------------------------------new psw
    if (buttonState == 0) {
      //nothing
    } else if (buttonState == 2) { //up
      int tm = 0;
      if (pointedAddr > 0) {
        tm = pointedAddr - 1;
      }
      if (simpleAutoReadPSW(tm, &n, &v)) {
        text = n;
        statusMenu = 1;
        pointedAddr--;
      } else {
        text = "New PSW";
        statusMenu = 2;
      }
    } else if (buttonState == 3) { //right
      text = "Insert name";
      statusMenu = 7;
    } else if (buttonState == 4) { //down
      text = "Settings";
      statusMenu = 3;
    } else if (buttonState == 5) { //center
      statusMenu = 1;
      pointedAddr = 0;
    } else if (buttonState == 6) { //boh
    }



  } else if (statusMenu == 3) {    //----------------------------------------- settings
    if (buttonState == 0) {
      //nothing
    } else if (buttonState == 2) { //up
      text = "New PSW";
      statusMenu = 2;
    } else if (buttonState == 3) { //right
      statusMenu = 10;
      text = "Psw length";
    } else if (buttonState == 4) { //down
    } else if (buttonState == 5) { //center
    } else if (buttonState == 6) { //boh
    }



  } else if (statusMenu == 4) {    //-----------------------------------------"edit name"
    if (buttonState == 0) {
      //nothing
    } else if (buttonState == 1) { //left
      statusMenu = 1;
    } else if (buttonState == 2) { //up
    } else if (buttonState == 3) { //right
      simpleAutoReadPSW(pointedAddr, &n, &v);
      addName = n;
      charInName = n.length();
      charIndex = 0;
      statusMenu = 9;
    } else if (buttonState == 4) { //down
      text = "Edit Val";
      statusMenu = 5;
    } else if (buttonState == 5) { //center
    } else if (buttonState == 6) { //boh
    }



  } else if (statusMenu == 5) {    //-----------------------------------------"edit val"
    if (buttonState == 0) {
      //nothing
    } else if (buttonState == 1) { //left
      statusMenu = 1;
    } else if (buttonState == 2) { //up
      text = "Edit name";
      statusMenu = 4;
    } else if (buttonState == 3) { //right
      //TODO editing
      recreatePSW(pointedAddr);
      statusMenu = 1;
      wait = 4;
    } else if (buttonState == 4) { //down
      text = "Delete PSW";
      statusMenu = 6;
    } else if (buttonState == 5) { //center
    } else if (buttonState == 6) { //boh
    }



  } else if (statusMenu == 6) {    //----------------------------------------- "delete psw"
    if (buttonState == 0) {
      //nothing
    } else if (buttonState == 1) { //left
      statusMenu = 1;
    } else if (buttonState == 2) { //up
      text = "Edit val";
      statusMenu = 5;
    } else if (buttonState == 3) { //right
      deletePSW(pointedAddr);
      if (pointedAddr != 0) {
        pointedAddr--;
      }
      statusMenu = 1;
      wait = 3;
      //TODO schermata di conferma
    } else if (buttonState == 4) { //down
    } else if (buttonState == 5) { //center
    } else if (buttonState == 6) { //boh
    }



  } else if (statusMenu == 7) {    //----------------------------------------------"insert name"
    if (buttonState == 0) {
      //nothing
    } else if (buttonState == 1) { //left
      text = "New PSW";
      statusMenu = 2;
    } else if (buttonState == 2) { //up
    } else if (buttonState == 3) { //right
      //TODO insert value
      statusMenu = 8;
    } else if (buttonState == 4) { //down
    } else if (buttonState == 5) { //center
    } else if (buttonState == 6) { //boh
    }



  } else if (statusMenu == 8) {    //---------------------------------------------- inserting the name
    if (buttonState == 0) {
      //nothing
    } else if (buttonState == 1) { //left
      if (charInName == 0) {
        //TODO return back
        text = "Insert name";
        statusMenu = 7;
      } else {
        addName.remove(addName.length() - 1);
        charInName--;
        charIndex = 0;
      }
    } else if (buttonState == 2) { //up
      charIndex++;
      if (charIndex >= 27) {
        charIndex = 0;
      }
    } else if (buttonState == 3) { //right
      if (charIndex == 0) {
        if (charInName != 0) {
          autoWritePSW(addName , rndPSWgen());
          //TODO messaggio
          charIndex = 0;
          charInName = 0;
          addName = "";
          pointedAddr--;
          statusMenu = 0;
          wait = 2;
        } else {
          //TODO nto valid name
        }
      } else {
        addName = addName + consentiti[charIndex];
        charIndex = 0;
        charInName++;
      }
    } else if (buttonState == 4) { //down
      charIndex--;
      if (charIndex < 0) {
        charIndex = 26;
      }
    } else if (buttonState == 5) { //center
    } else if (buttonState == 6) { //boh
    }



  } else if (statusMenu == 9) {    //---------------------------------------------- editing the name
    if (buttonState == 0) {
      //nothing
    } else if (buttonState == 1) { //left
      if (charInName == 0) {
        //TODO return back
        text = "Edit name";
        statusMenu = 4;
      } else {
        addName.remove(addName.length() - 1);
        charInName--;
        charIndex = 0;
      }
    } else if (buttonState == 2) { //up
      charIndex++;
      if (charIndex >= 27) {
        charIndex = 0;
      }
    } else if (buttonState == 3) { //right
      if (charIndex == 0) {
        if (charInName != 0) {
          //TODO change name
          //n;
          //v;
          //pointedAddr;
          simpleAutoReadPSW(pointedAddr, &n, &v);
          int indx = (int) readEEPROM(pointedAddr);
          int eeaddress = startPSWzone + (dimPSW * indx);
          resetEEPROM(eeaddress, eeaddress + dimPSW - 1);
          writeEEPROM(eeaddress, 1);
          for (int i = 0; i < addName.length(); i++) {
            writeEEPROM(eeaddress + 1 + i, addName.charAt(i));
          }
          int tmp = eeaddress + 1 + addName.length();
          writeEEPROM(tmp, 2);
          for (int k = 0; k < v.length(); k++) {
            writeEEPROM(tmp + 1 + k, v.charAt(k));
          }
          writeEEPROM(tmp + 1 + v.length(), 3);
          //TODO message
          charIndex = 0;
          charInName = 0;
          addName = "";
          statusMenu = 0;
          wait = 1;
        } else {
          //TODO not valid message
        }
      } else {
        addName = addName + consentiti[charIndex];
        charIndex = 0;
        charInName++;
      }
    } else if (buttonState == 4) { //down
      charIndex--;
      if (charIndex < 0) {
        charIndex = 26;
      }
    } else if (buttonState == 5) { //center
    } else if (buttonState == 6) { //boh
    }



  } else if (statusMenu == 10) {                                           // setting "psw length"
    if (buttonState == 0) {
      //nothing
    } else if (buttonState == 1) { //left
      text = "Settings";
      statusMenu = 3;
    } else if (buttonState == 2) { //up
    } else if (buttonState == 3) { //right
      statusMenu = 12;
      charIndex = (int) readEEPROM(pswLength);
      if (charIndex == 255) {
        charIndex = 10;
        writeEEPROM(pswLength, 10);
      }
    } else if (buttonState == 4) { //down
      statusMenu = 11;
      text = "Psw type";
    } else if (buttonState == 5) { //center
    } else if (buttonState == 6) { //boh
    }



  } else if (statusMenu == 11) {                                           // setting "psw type"
    if (buttonState == 0) {
      //nothing
    } else if (buttonState == 1) { //left
      text = "Settings";
      statusMenu = 3;
    } else if (buttonState == 2) { //up
      statusMenu = 10;
      text = "Psw length";
    } else if (buttonState == 3) { //right
      statusMenu = 13;
      text = "alphabetic";
    } else if (buttonState == 4) { //down
      statusMenu = 17;
      text = "Erase all";
    } else if (buttonState == 5) { //center
    } else if (buttonState == 6) { //boh
    }


  } else if (statusMenu == 17) {                                           // setting erase all
    if (buttonState == 0) {
      //nothing
    } else if (buttonState == 1) { //left
      text = "Settings";
      statusMenu = 3;
    } else if (buttonState == 2) { //up
      statusMenu = 11;
      text = "Psw type";
    } else if (buttonState == 3) { //right
      statusMenu = 18;
      text = "Sure?";
    } else if (buttonState == 4) { //down
    } else if (buttonState == 5) { //center
    } else if (buttonState == 6) { //boh
    }


  } else if (statusMenu == 18) {                                           // "sure?"
    if (buttonState == 0) {
      //nothing
    } else if (buttonState == 1) { //left
      text = "Settings";
      statusMenu = 3;
    } else if (buttonState == 2) { //up
    } else if (buttonState == 3) { //right
      resetEEPROM(0, 7000);
      statusMenu = 0;
      wait = 7;
    } else if (buttonState == 4) { //down
    } else if (buttonState == 5) { //center
    } else if (buttonState == 6) { //boh
    }



  } else if (statusMenu == 12) {    //---------------------------------------------- editing the PSW length
    if (buttonState == 0) {
      //nothing
    } else if (buttonState == 1) { //left
      statusMenu = 10;
      text = "Psw length";
    } else if (buttonState == 2) { //up
      charIndex++;
      if (charIndex >= 17) {
        charIndex = 6;
      }
    } else if (buttonState == 3) { //right
      int t = (int) readEEPROM(pswLength);
      if (charIndex != t) {
        writeEEPROM(pswLength, charIndex);
      }
      charIndex = 0;
      statusMenu = 10;
      text = "Psw length";
      wait = 1;
    } else if (buttonState == 4) { //down
      charIndex--;
      if (charIndex < 6) {
        charIndex = 16;
      }
    } else if (buttonState == 5) { //center
    } else if (buttonState == 6) { //boh
    }



  } else if (statusMenu == 13) {                                           //psw alphabetic
    if (buttonState == 0) {
      //nothing
    } else if (buttonState == 1) { //left
      statusMenu = 11;
      text = "Psw type";
    } else if (buttonState == 2) { //up
    } else if (buttonState == 3) { //right
      int val = (int) readEEPROM(pswType);
      if (val == 255 || val != 1) {
        writeEEPROM(pswType, 1);
      }
      statusMenu = 11;
      text = "Psw type";
      wait = 1;
    } else if (buttonState == 4) { //down
      statusMenu = 14;
      text = "AlphaBetic";
    } else if (buttonState == 5) { //center
    } else if (buttonState == 6) { //boh
    }



  } else if (statusMenu == 14) {                                           //psw aLpHabeTic   P1KCYX2sL9CbIi26Li
    if (buttonState == 0) {
      //nothing
    } else if (buttonState == 1) { //left
      statusMenu = 11;
      text = "Psw type";
    } else if (buttonState == 2) { //up
      statusMenu = 13;
      text = "alphabetic";
    } else if (buttonState == 3) { //right
      int val = (int) readEEPROM(pswType);
      if (val == 255 || val != 2) {
        writeEEPROM(pswType, 2);
      }
      statusMenu = 11;
      text = "Psw type";
      wait = 1;
    } else if (buttonState == 4) { //down
      statusMenu = 15;
      text = "AlpH4num";
    } else if (buttonState == 5) { //center
    } else if (buttonState == 6) { //boh
    }



  } else if (statusMenu == 15) {                                           //psw AlpH4num3ric
    if (buttonState == 0) {
      //nothing
    } else if (buttonState == 1) { //left
      statusMenu = 11;
      text = "Psw type";
    } else if (buttonState == 2) { //up
      statusMenu = 14;
      text = "AlphaBetic";
    } else if (buttonState == 3) { //right
      int val = (int) readEEPROM(pswType);
      if (val == 255 || val != 3) {
        writeEEPROM(pswType, 3);
      }
      statusMenu = 11;
      text = "Psw type";
      wait = 1;
    } else if (buttonState == 4) { //down
      statusMenu = 16;
      text = "Alphanum+";
    } else if (buttonState == 5) { //center
    } else if (buttonState == 6) { //boh
    }



  } else if (statusMenu == 16) {                                           //psw alphanumeric+
    if (buttonState == 0) {
      //nothing
    } else if (buttonState == 1) { //left
      statusMenu = 11;
      text = "Psw type";
    } else if (buttonState == 2) { //up
      statusMenu = 15;
      text = "AlpH4num";
    } else if (buttonState == 3) { //right
      int val = (int) readEEPROM(pswType);
      if (val == 255 || val != 4) {
        writeEEPROM(pswType, 4);
      }
      wait = 1;
      statusMenu = 11;
      text = "Psw type";
    } else if (buttonState == 4) { //down
    } else if (buttonState == 5) { //center
    } else if (buttonState == 6) { //boh
    }



  }



  /*
    else if (statusMenu == k) {
    if (buttonState == 0) {
      //nothing
    } else if (buttonState == 1) { //left

    } else if (buttonState == 2) { //up

    } else if (buttonState == 3) { //right

    } else if (buttonState == 4) { //down

    } else if (buttonState == 5) { //center

    } else if (buttonState == 6) { //boh

    }
    }

  */


  // picture loop
  u8g2.firstPage();
  do {

    if (wait == 1) {
      drawTxt("Saved!");
    } else if (wait == 2) {
      u8g2.drawStr( 0, 24, string2char("PSW created!"));
    } else if (wait == 3) {
      u8g2.drawStr( 0, 24, string2char("PSW deleted!"));
    } else if (wait == 4) {
      u8g2.drawStr( 0, 24, string2char("PSW changed!"));
    } else if (wait == 6) {
      u8g2.drawXBMP( 0, 0, 128, 32, logo);
    } else if (wait == 7) {
      u8g2.drawStr( 0, 24, string2char("Done!"));
    } else {
      drawActions();
      if (statusMenu == 8 || statusMenu == 9 || statusMenu == 12) {
        //editing screen with fixed text
      } else {
        drawTxt(text);
      }
    }
  } while ( u8g2.nextPage() );

}







































void drawSampleText(void) {
  if (statusMenu != 8) {
    u8g2.drawStr( 18, 27, "Mail");
  }
}

void drawTxt(String t) {
  u8g2.drawStr( 17, 24, string2char(t));
}
void drawTxtAt(int k, String t) {
  u8g2.drawStr( k, 24, string2char(t));
}
char* string2char(String command) {
  if (command.length() != 0) {
    char *p = const_cast<char*>(command.c_str());
    return p;
  }
}


void drawActions(void) {

  //u8g2.drawBitmap( 90, 25, 1, 6, center);

  if (statusMenu <= 1) {                                                     //PSW mostrate
    if (pointedAddr != 0) {
      u8g2.drawBitmap( 60, 0, 1, 3, upArrow);
    }
    u8g2.drawBitmap( 0, 12, 1, 8, leftArrow);
    u8g2.drawBitmap( 4, 4, 1, 23, verticalTYPE);
    u8g2.drawBitmap( 119, 4, 1, 23, verticalEDIT);
    u8g2.drawBitmap( 60, 29, 1, 3, downArrow);
    u8g2.drawBitmap( 125, 12, 1, 8, rigthArrow);
  } else  if (statusMenu == 2) {                                             //New psw
    if (pointedAddr != 0) {
      u8g2.drawBitmap( 60, 0, 1, 3, upArrow);
    }

    u8g2.drawBitmap( 119, 4, 1, 23, verticalEDIT);
    u8g2.drawBitmap( 60, 29, 1, 3, downArrow); 
    u8g2.drawBitmap( 125, 12, 1, 8, rigthArrow);
  } else  if (statusMenu == 3) {                                            // settings
    u8g2.drawBitmap( 60, 0, 1, 3, upArrow);
    u8g2.drawBitmap( 119, 4, 1, 23, verticalEDIT); 
    u8g2.drawBitmap( 125, 12, 1, 8, rigthArrow);
  } else  if (statusMenu == 4) {                                              //edit name
    u8g2.drawBitmap( 60, 29, 1, 3, downArrow);
    u8g2.drawBitmap( 119, 4, 1, 23, verticalEDIT);
    u8g2.drawBitmap( 0, 12, 1, 8, leftArrow);
    u8g2.drawBitmap( 4, 4, 1, 23, verticalBACK); 
    u8g2.drawBitmap( 125, 12, 1, 8, rigthArrow);
  } else  if (statusMenu == 5) {                                              //edit val
    u8g2.drawBitmap( 60, 0, 1, 3, upArrow);
    u8g2.drawBitmap( 60, 29, 1, 3, downArrow);
    u8g2.drawBitmap( 119, 4, 1, 23, verticalEDIT);
    u8g2.drawBitmap( 0, 12, 1, 8, leftArrow);
    u8g2.drawBitmap( 4, 4, 1, 23, verticalBACK); 
    u8g2.drawBitmap( 125, 12, 1, 8, rigthArrow);
  } else  if (statusMenu == 6) {                                              //delete psw
    u8g2.drawBitmap( 60, 0, 1, 3, upArrow);
    u8g2.drawBitmap( 119, 11, 1, 12, verticalOK);
    u8g2.drawBitmap( 0, 12, 1, 8, leftArrow);
    u8g2.drawBitmap( 4, 4, 1, 23, verticalBACK); 
    u8g2.drawBitmap( 125, 12, 1, 8, rigthArrow);
  } else  if (statusMenu == 7) {                                              //insert name
    u8g2.drawBitmap( 0, 12, 1, 8, leftArrow);
    u8g2.drawBitmap( 4, 4, 1, 23, verticalBACK);
    u8g2.drawBitmap( 119, 11, 1, 12, verticalOK);
    u8g2.drawBitmap( 125, 12, 1, 8, rigthArrow);
  } else if (statusMenu == 8) {                                                //inserting name
    u8g2.drawBitmap( 0, 12, 1, 8, leftArrow);
    u8g2.drawBitmap( 4, 4, 1, 23, verticalBACK);
    u8g2.drawBitmap( 119, 11, 1, 12, verticalOK);
    //TODO review distances    
    int i = 0;
    for (i = 0; i <= charInName; i++) {
      u8g2.drawStr( 17 + (i * spacing), 26, "_");
    }
    i--;
    u8g2.drawBitmap( 18 + (i * spacing), 0, 1, 3, upArrow);
    u8g2.drawBitmap( 18 + (i * spacing), 29, 1, 3, downArrow);
    String s = String(consentiti[charIndex]);
    //u8g2.drawStr( 17 + (i * spacing), 27, s);
    drawTxtAt(17 + (i * spacing), s);
    drawTxt(addName); u8g2.drawBitmap( 125, 12, 1, 8, rigthArrow);
  } else if (statusMenu == 9) {                                                //editing name
    u8g2.drawBitmap( 4, 4, 1, 23, verticalBACK);
    u8g2.drawBitmap( 119, 11, 1, 12, verticalOK);
    u8g2.drawBitmap( 0, 12, 1, 8, leftArrow);
    //TODO review distances
    int i = 0;
    for (i = 0; i <= charInName; i++) {
      u8g2.drawStr( 17 + (i * spacing), 26, "_");
    }
    i--;
    u8g2.drawBitmap( 18 + (i * spacing), 0, 1, 3, upArrow);
    u8g2.drawBitmap( 18 + (i * spacing), 29, 1, 3, downArrow);
    String s = String(consentiti[charIndex]);
    //u8g2.drawStr( 17 + (i * spacing), 27, s);
    drawTxtAt(17 + (i * spacing), s);
    drawTxt(addName); u8g2.drawBitmap( 125, 12, 1, 8, rigthArrow);
  } else  if (statusMenu == 10) {                                              //psw length
    u8g2.drawBitmap( 0, 12, 1, 8, leftArrow);
    u8g2.drawBitmap( 60, 29, 1, 3, downArrow);
    u8g2.drawBitmap( 4, 4, 1, 23, verticalBACK);
    u8g2.drawBitmap( 119, 4, 1, 23, verticalEDIT);
    u8g2.drawBitmap( 125, 12, 1, 8, rigthArrow);
  } else  if (statusMenu == 11) {                                              //psw type
    u8g2.drawBitmap( 0, 12, 1, 8, leftArrow);
    u8g2.drawBitmap( 60, 0, 1, 3, upArrow);
    u8g2.drawBitmap( 4, 4, 1, 23, verticalBACK);
    u8g2.drawBitmap( 60, 29, 1, 3, downArrow);
    u8g2.drawBitmap( 119, 4, 1, 23, verticalEDIT);
    u8g2.drawBitmap( 125, 12, 1, 8, rigthArrow);
  } else if (statusMenu == 12) {                                                //editing psw length
    //TODO in the center?
    u8g2.drawBitmap( 4, 4, 1, 23, verticalBACK);
    u8g2.drawBitmap( 119, 11, 1, 12, verticalOK);
    u8g2.drawBitmap( 0, 12, 1, 8, leftArrow);
    u8g2.drawStr( 17, 26, "_");
    u8g2.drawStr( 25, 26, "_");
    u8g2.drawBitmap( 22 , 0, 1, 3, upArrow);
    u8g2.drawBitmap( 22 , 29, 1, 3, downArrow);
    u8g2.drawBitmap( 125, 12, 1, 8, rigthArrow);
    drawTxt(String(charIndex));
  } else  if (statusMenu == 13) {                                              //alphabetic
    u8g2.drawBitmap( 0, 12, 1, 8, leftArrow);
    u8g2.drawBitmap( 60, 29, 1, 3, downArrow);
    u8g2.drawBitmap( 4, 4, 1, 23, verticalBACK);
    u8g2.drawBitmap( 119, 11, 1, 12, verticalOK);
    u8g2.drawBitmap( 125, 12, 1, 8, rigthArrow);
  } else  if (statusMenu == 14) {                                              //aLpHabeTic
    u8g2.drawBitmap( 0, 12, 1, 8, leftArrow);
    u8g2.drawBitmap( 60, 29, 1, 3, downArrow);
    u8g2.drawBitmap( 4, 4, 1, 23, verticalBACK);
    u8g2.drawBitmap( 119, 11, 1, 12, verticalOK);
    u8g2.drawBitmap( 60, 0, 1, 3, upArrow); 
    u8g2.drawBitmap( 125, 12, 1, 8, rigthArrow);
  } else  if (statusMenu == 15) {                                              //aLpHanum3ric
    u8g2.drawBitmap( 0, 12, 1, 8, leftArrow);
    u8g2.drawBitmap( 60, 29, 1, 3, downArrow);
    u8g2.drawBitmap( 4, 4, 1, 23, verticalBACK);
    u8g2.drawBitmap( 119, 11, 1, 12, verticalOK);
    u8g2.drawBitmap( 60, 0, 1, 3, upArrow); 
    u8g2.drawBitmap( 125, 12, 1, 8, rigthArrow);
  } else  if (statusMenu == 16) {                                              //Alphanumeric+
    u8g2.drawBitmap( 0, 12, 1, 8, leftArrow);
    u8g2.drawBitmap( 4, 4, 1, 23, verticalBACK);
    u8g2.drawBitmap( 119, 11, 1, 12, verticalOK);
    u8g2.drawBitmap( 60, 0, 1, 3, upArrow);
    u8g2.drawBitmap( 125, 12, 1, 8, rigthArrow);
  } else  if (statusMenu == 17) {                                              //erase all?
    u8g2.drawBitmap( 0, 12, 1, 8, leftArrow);
    u8g2.drawBitmap( 4, 4, 1, 23, verticalBACK);
    u8g2.drawBitmap( 119, 11, 1, 12, verticalOK);
    u8g2.drawBitmap( 60, 0, 1, 3, upArrow); 
    u8g2.drawBitmap( 125, 12, 1, 8, rigthArrow);
  } else  if (statusMenu == 18) {                                              //sure?
    u8g2.drawBitmap( 0, 12, 1, 8, leftArrow);
    u8g2.drawBitmap( 4, 4, 1, 23, verticalBACK);
    u8g2.drawBitmap( 119, 11, 1, 12, verticalOK);
    u8g2.drawBitmap( 125, 12, 1, 8, rigthArrow);
  }


}

String rndPSWgen() {                                                                           //rnd PSW generator  gGzRNfsBZh0GmGm  Wf9JCc6vLh
  randomSeed(analogRead(0));
  //TODO better rnd
  String t = "";
  int r = 0;
  int l = (int) readEEPROM(pswLength);
  if (l == 255) {
    l = 10;
  } else {
    l--; //TODO fix
  }
  int val = (int) readEEPROM(pswType);
  if (val == 1) {
    for (int i = 0; i < l; i++) {
      r = random(1, 27);        //lower OK
      t = t + consentiti[r];
      delay(5);
    }
  } else if (val == 2) {
    for (int i = 0; i < l; i++) {
      r = random(1, 53);          //uppper and lower case
      t = t + consentiti[r];
      delay(5);
    }
  }  else if (val == 4) {
    for (int i = 0; i < l; i++) {
      r = random(1, 82);        //All
      t = t + consentiti[r];
      delay(5);
    }
  } else {
    for (int i = 0; i < l; i++) {
      r = random(1, 63);          //lowe upper and numbers
      t = t + consentiti[r];
      delay(5);
    }
  }
  return t;
}



void writeEEPROM( unsigned int eeaddress, byte data )                                             //WRITE ADDRESS
{
  Wire.beginTransmission(disk1);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(data);
  Wire.endTransmission();

  delay(5);
}

byte readEEPROM(unsigned int eeaddress )                                                           // READ ADDRESS
{
  byte rdata = 0xFF;

  Wire.beginTransmission(disk1);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();

  Wire.requestFrom(disk1, 1);

  if (Wire.available()) rdata = Wire.read();

  return rdata;
}

void resetEEPROM(int start, int en) {                                                                 //  RESET FROM TO
  if (start == en) {
    for (int i = 0; i < 32767; i++) {
      //Serial.println(i);
      int tm = (int) readEEPROM(i);
      if (tm != byteResetValue)writeEEPROM(i, byteResetValue);
    }
    Serial.println("total reset done");
  } else {
    for (int i = start; i < en; i++) {
      int tm = (int) readEEPROM(i);
      if (tm != byteResetValue)writeEEPROM(i, byteResetValue);
    }
    Serial.println(" ");
    Serial.print("reset done from ");
    Serial.print(start, DEC);
    Serial.print(" to ");
    Serial.print(en, DEC);
    Serial.println(" ");
  }
}

void copyByte (unsigned int from, unsigned int to) {                                                         //COPY BYTE FROM TO
  int t = readEEPROM(from);
  writeEEPROM(to, t);
}


void showEEPROM(unsigned int startAd, unsigned int endAd) {                                                  //SHOW EEPROM FROM TO
  int range = endAd - startAd;
  for (int i = 0; i < range; i++) {
    int val = (int) readEEPROM(i + startAd);
    Serial.print(String(val, DEC) + " ");
    if ((i + 1) % 10 == 0) {
      Serial.println();
    }
  }
}

void switchByteVal(unsigned int a, unsigned int b) {                                                         //SWITCH 2 BYTE VALUE
  int tm = (int) readEEPROM(a);
  copyByte(b, a);
  writeEEPROM(b, tm);
}


boolean autoWritePSW(String id , String psw) {                                                              //AUTO FIND ADDRESSES AND WRITE PSW
  //Find the rigth spot
  //check if space is available
  //if not available return false
  //else write

  //check if address space is available
  int addrToWrite = 0;
  boolean found = false;
  int val = (int) readEEPROM(0);
  if (val == byteResetValue) {
    found = true;
  } else {
    int m = 1;
    while (val != byteResetValue && m < endAddressZone) {
      val = (int) readEEPROM(m);
      m++;
    }
    if (val == byteResetValue) {
      if (m < endAddressZone) {
        addrToWrite = m - 1;
        found = true;
      }
    }
    if (!found) {
      return false;
    }
  }

  unsigned int eeaddress = startPSWzone;
  for (int j = startPSWzone; j < endPSWzone; j = j + dimPSW) {
    //j al massimo scorre 250 circa, veloce e indolore
    int val = (int) readEEPROM(j);
    if (val == byteResetValue) {
      int lastByteAddress = j + id.length() + psw.length() + 3;
      if (lastByteAddress >= endPSWzone) {
        Serial.println("not storing PSW");
        return false;
      }

      eeaddress = j;
      j = endPSWzone + 2;
    }
  }

  int ad = (eeaddress - startPSWzone) / dimPSW;
  writeEEPROM(addrToWrite, ad);


  writeEEPROM(eeaddress, 1);
  for (int i = 0; i < id.length(); i++) {
    writeEEPROM(eeaddress + 1 + i, id.charAt(i));
  }
  int tmp = eeaddress + 1 + id.length();
  writeEEPROM(tmp, 2);
  for (int k = 0; k < psw.length(); k++) {
    writeEEPROM(tmp + 1 + k, psw.charAt(k));
  }
  writeEEPROM(tmp + 1 + psw.length(), 3);
  return true;

}

boolean autoReadPSW(unsigned int eeaddress, String *nam, String *val, boolean forward) {                     //AUTO READ AT ADDRESS PSW

  if (forward) {
    for (int i = eeaddress; i < settingStartAddress; i++) {
      int addrVal = (int) readEEPROM(i);
      if ( addrVal != byteResetValue) {
        int pswAddr = startPSWzone + (addrVal * dimPSW) + 1;
        while (((int) readEEPROM(pswAddr)) != 2) {
          *nam = *nam + char(readEEPROM(pswAddr));
          pswAddr++;
        }
        pswAddr++;
        while (((int) readEEPROM(pswAddr)) != 3) {
          *val = *val + char(readEEPROM(pswAddr));
          pswAddr++;
        }
        return true;
      }
    }
    return false;
  } else {
    eeaddress--;
    for (int i = eeaddress; i >= 0; i--) {
      int addrVal = (int) readEEPROM(i);
      if ( addrVal != byteResetValue) {
        int pswAddr = startPSWzone + (addrVal * dimPSW) + 1;
        while (((int) readEEPROM(pswAddr)) != 2) {
          *nam = *nam + char(readEEPROM(pswAddr));
          pswAddr++;
        }
        pswAddr++;
        while (((int) readEEPROM(pswAddr)) != 3) {
          *val = *val + char(readEEPROM(pswAddr));
          pswAddr++;
        }
        return true;
      }
    }
    return false;
  }
}


boolean simpleAutoReadPSW(unsigned int eeaddress, String *nam, String *val) {                     //SIMPLE -----------------------_AUTO READ AT ADDRESS PSW

  int addrVal = (int) readEEPROM(eeaddress);
  if ( addrVal != byteResetValue) {
    int pswAddr = startPSWzone + (addrVal * dimPSW) + 1;
    while (((int) readEEPROM(pswAddr)) != 2) {
      *nam = *nam + char(readEEPROM(pswAddr));
      pswAddr++;
    }
    pswAddr++;
    while (((int) readEEPROM(pswAddr)) != 3) {
      *val = *val + char(readEEPROM(pswAddr));
      pswAddr++;
    }
    return true;
  }
  return false;
}

boolean recreatePSW(unsigned int eeaddress) {                                                        //recreate psw value mantaining name
  int addrVal = (int) readEEPROM(eeaddress);
  int pswAddr = startPSWzone + (addrVal * dimPSW) + 1;
  while (((int) readEEPROM(pswAddr)) != 2) {
    pswAddr++;
  }
  pswAddr++;
  String t = "";
  t = rndPSWgen();
  int i = 0;
  for ( i = 0; i < t.length(); i++) {
    writeEEPROM(pswAddr + i, t.charAt(i));
  }
  writeEEPROM(pswAddr + i + 1, 3);

}


void deletePSW(unsigned int ad) {                                                                      //DELETE PSW GIVEN THE POINTER
  int tm = (int) readEEPROM(ad); //number of cell of PSW
  tm = startPSWzone + (tm * dimPSW); //real address of PSW
  resetEEPROM(tm, tm + dimPSW - 1);

  writeEEPROM(ad, byteResetValue);

  int i = ad + 1;
  tm = (int) readEEPROM(i);
  while (tm != byteResetValue) {
    copyByte(i, i - 1);
    i++;
    tm = (int) readEEPROM(i);
  }
  writeEEPROM(i - 1, byteResetValue);

}
















//INPUT LOGIC
//TODO long press equal to multiple press?



int getPressedButton() {

  int ub = digitalRead(upPin);
  int cb = digitalRead(centerPin);
  int lb = digitalRead(leftPin);
  int rb = digitalRead(rightPin);
  int db = digitalRead(downPin);

  if (lb == 0) {
    return 1;   //left pressed
  } else if (ub == 0) {
    return 2;   //up pressed
  } else if (rb == 0) {
    return 3;   //right pressed
  } else if (db == 0) {
    return 4;   //down pressed
  } else if (cb == 0) {
    return 5;   //center pressed
  } else {
    return 0;   //nothing pressed
  }

}

int debounceButtons() {
  int tm = getPressedButton();
  if (tm == lastButtonState) {
    buttonState = 0;
    if (tm != 0 ) {
      if ( cyclePressed < longPressedTime) {
        cyclePressed++;
        Serial.println(cyclePressed, DEC);
      } else {
        cyclePressed = cyclePressed - longPressQuant;
        buttonState = tm;
      }
    }
  } else {
    cyclePressed = 0;
    lastButtonState = tm;
    buttonState = tm;
  }
}

