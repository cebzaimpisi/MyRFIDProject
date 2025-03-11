#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <VirtualWire.h>


// TFT Display Setup
#define TFT_RST   8      // TFT RST pin is connected to arduino pin 8
#define TFT_CS    10      // TFT CS  pin is connected to arduino pin 10
#define TFT_DC    9     // TFT DC  pin is connected to arduino pin 9

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);


// Buttons
#define UP_BUTTON    4
#define DOWN_BUTTON  5
#define ENTER_BUTTON 6

// Buzzer & LED
#define BUZZER_PIN  7
#define LED_PIN     A0

// Menu Variables
String locations[] = {">ICU", ">Emergency", ">Theater", ">Pediatrics"};
int menuIndex = 0;
bool searching = false;
String foundLocation = "";

// Function to draw a dotted line
void drawDottedLine(int x, int y, int length) {
    for (int i = 0; i < length; i += 4) {
        tft.drawPixel(x + i, y, ST7735_WHITE);
    }
}

// ICU Display
void displayICU() {
    tft.setTextColor(ST7735_WHITE);
    tft.setTextSize(1);
    tft.setCursor(10, 10);
    tft.print("ICU Medical Devices");

    int y = 40;
    tft.setCursor(10, y);
    tft.setTextColor(ST7735_GREEN);
    tft.print("Device: Ventilator | ID: 12A3B4");
    y += 20;
    tft.setCursor(10, y);
    tft.print("Last Seen: 14:35 | Status: In Use");
    y += 10;
    drawDottedLine(10, y, 300);
    y += 20;

    tft.setCursor(10, y);
    tft.print("Device: Defibrillator | ID: 34C5D6");
    y += 20;
    tft.setCursor(10, y);
    tft.print("Last Seen: 14:37 | Status: Available");
    y += 10;
    drawDottedLine(10, y, 300);
    y += 20;

    tft.setCursor(10, y);
    tft.setTextColor(ST7735_RED);
    tft.print("ALERT: Infusion Pump (ID: 78E9F0) MISSING!");
}

// Emergency Room Display
void displayEmergency() {
    tft.setTextColor(ST7735_WHITE);
    tft.setTextSize(1);
    tft.setCursor(10, 10);
    tft.print("Emergency Room Devices");

    int y = 40;
    tft.setCursor(10, y);
    tft.setTextColor(ST7735_MAGENTA);
    tft.print("Device: Portable ECG | ID: 45F2B1");
    y += 20;
    tft.setCursor(10, y);
    tft.print("Last Seen: 15:20 | Status: In Use");
    y += 10;
    drawDottedLine(10, y, 300);
    y += 20;

    tft.setCursor(10, y);
    tft.print("Device: Suction Pump | ID: 67G3H2");
    y += 20;
    tft.setCursor(10, y);
    tft.print("Last Seen: 15:22 | Status: Available");
    y += 10;
    drawDottedLine(10, y, 300);
    y += 20;

    tft.setCursor(10, y);
    tft.setTextColor(ST7735_RED);
    tft.print("ALERT: Oxygen Cylinder (ID: 89J4K5) MISSING!");
}

// Operating Theater Display
void displayTheater() {
    tft.setTextColor(ST7735_WHITE);
    tft.setTextSize(1);
    tft.setCursor(10, 10);
    tft.print("Operating Theater Devices");

    int y = 40;
    tft.setCursor(10, y);
    tft.setTextColor(ST7735_BLUE);
    tft.print("Device: Anesthesia Machine | ID: 56D7E8");
    y += 20;
    tft.setCursor(10, y);
    tft.print("Last Seen: 13:10 | Status: In Use");
    y += 10;
    drawDottedLine(10, y, 300);
    y += 20;

    tft.setCursor(10, y);
    tft.print("Device: Surgical Lamp | ID: 23A4B5");
    y += 20;
    tft.setCursor(10, y);
    tft.print("Last Seen: 13:15 | Status: Available");
    y += 10;
    drawDottedLine(10, y, 300);
    y += 20;

    tft.setCursor(10, y);
    tft.setTextColor(ST7735_RED);
    tft.print("ALERT: Electrocautery Unit (ID: 90L6M7) MISSING!");
}

// Pediatrics Ward Display
void displayPediatrics() {
    tft.setTextColor(ST7735_WHITE);
    tft.setTextSize(1);
    tft.setCursor(10, 10);
    tft.print("Pediatrics Ward Devices");

    int y = 40;
    tft.setCursor(10, y);
    tft.setTextColor(ST7735_YELLOW);
    tft.print("Device: Infant Warmer | ID: 78X9Y1");
    y += 20;
    tft.setCursor(10, y);
    tft.print("Last Seen: 16:05 | Status: In Use");
    y += 10;
    drawDottedLine(10, y, 300);
    y += 20;

    drawDottedLine(10, y, 300);
    y += 20;

    tft.setCursor(10, y);
    tft.setTextColor(ST7735_RED);
    tft.print("ALERT: Pulse Oximeter (ID: 32P5Q6) MISSING!");
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
     delay(100);
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
}



// Function to display the menu
void displayMenu() {
    tft.fillScreen(ST7735_BLACK);
    tft.setTextColor(ST7735_WHITE);
    tft.setTextSize(2);
    
    tft.setCursor(4, 10);
    tft.print("Select Loca..");
    tft.drawFastHLine(0, 30,  tft.width(), ST7735_YELLOW);
    
    for (int i = 0; i < 4; i++) {
        tft.setCursor(20, 40 + (i * 20));
        if (i == menuIndex) {
            tft.setTextColor(ST7735_GREEN);  // Highlight selection
        } else {
            tft.setTextColor(ST7735_WHITE);
        }
        tft.print(locations[i]);
    }
}

// Function to show searching animation
void showSearchingAnimation() {
    tft.fillScreen(ST7735_BLACK);
    tft.setTextColor(ST7735_YELLOW);
    tft.setTextSize(2);
    tft.setCursor(20, 50);
    tft.print("Searching");

    for (int i = 0; i < 2; i++) {
        tft.print(".");
        delay(50);
    }
}

// Function to display the found location
void displayLocation() {
    tft.fillScreen(ST7735_BLACK);
    tft.setTextSize(1);
    int ICU = 0, Emergency = 1, Theater = 2, Pediatrics = 3;
    if (ICU == menuIndex) {
        displayICU();
        // Turn on LED & beep buzzer
        digitalWrite(LED_PIN, HIGH);
        tone(BUZZER_PIN, 1000, 500);
        delay(500);
        digitalWrite(LED_PIN, LOW);
    } 
     if (Emergency == menuIndex) {
        displayEmergency();
        // Turn on LED & beep buzzer
        digitalWrite(LED_PIN, HIGH);
        tone(BUZZER_PIN, 1000, 500);
        delay(500);
        digitalWrite(LED_PIN, LOW);
    } 
    if (Theater == menuIndex) {
        displayTheater();
        // Turn on LED & beep buzzer
        digitalWrite(LED_PIN, HIGH);
        tone(BUZZER_PIN, 1000, 500);
        delay(500);
        digitalWrite(LED_PIN, LOW);
    } 
    if (Pediatrics == menuIndex) {
        displayPediatrics();
        // Turn on LED & beep buzzer
        digitalWrite(LED_PIN, HIGH);
        tone(BUZZER_PIN, 1000, 500);
        delay(500);
        digitalWrite(LED_PIN, LOW);
    } 
     
    
    delay(300); // Show result for 300 MILLIseconds
    displayMenu(); // Return to menu
}
// Function to show a loading screen
void showLoadingScreen() {
    tft.fillScreen(ST7735_BLACK);
    tft.setTextColor(ST7735_GREEN);
    tft.setTextSize(1);
    tft.setCursor(2, 40);
    tft.print("Medical Device Tracking");

    tft.fillScreen(ST7735_BLACK);
    tft.setTextColor(ST7735_RED);
    tft.setTextSize(2);
    tft.setCursor(2, 60);
    tft.print("Loading...");
    
    for (int i = 0; i < 3; i++) {
        delay(70);
        tft.setCursor(0 + (i * 10), 100);
        tft.fillCircle(30 + (i * 40), 120, 10, ST7735_MAGENTA);
    }
    delay(100);
}
void setup() {
// TFT Setup
    tft.initR(INITR_BLACKTAB);
    
    tft.fillScreen(ST7735_BLACK);
    tft.setRotation(3);

 vw_set_ptt_inverted(true); // Required for DR3100
vw_set_rx_pin(12);
vw_setup(4000); // Bits per sec
Serial.begin(9600);
pinMode(LED_PIN, OUTPUT);
pinMode(BUZZER_PIN, OUTPUT);
 //tone(BUZZER_PIN, 1000, 500);

     pinMode(LED_PIN, OUTPUT);
    Serial.println("RFID Medical TRACKING SYSTEM");
     digitalWrite(LED_PIN, HIGH);
        //tone(BUZZER_PIN, 1000, 500);
        digitalWrite(BUZZER_PIN, HIGH);

   //ICU Medical Devices display
   Serial.println("ICU Medical Devices");
   Serial.println("Device: Ventilator | ID: 12A3B4");
   Serial.println("Last Seen: 14:35 | Status: In Use");
   Serial.println("Device: Defibrillator | ID: 34C5D6");
   Serial.println("Last Seen: 14:37 | Status: Available");
   Serial.println("ALERT: Infusion Pump (ID: 78E9F0) MISSING!");

   Serial.println();
// Emergency Room Display

  Serial.println("Emergency Room Devices");
  Serial.println("Device: Portable ECG | ID: 45F2B1");
  Serial.println("Last Seen: 15:20 | Status: In Use");
  Serial.println("Device: Suction Pump | ID: 67G3H2"); 
  Serial.println("Last Seen: 15:22 | Status: Available");
  Serial.println("ALERT: Oxygen Cylinder (ID: 89J4K5) MISSING!");

 Serial.println();
// Operating Theater Display

  Serial.println("Operating Theater Devices");
  Serial.println("Device: Anesthesia Machine | ID: 56D7E8");
  Serial.println("Last Seen: 13:10 | Status: In Use");
  Serial.println("Device: Surgical Lamp | ID: 23A4B5");
  Serial.println("Last Seen: 13:15 | Status: Available");
  Serial.println("ALERT: Electrocautery Unit (ID: 90L6M7) MISSING!");

  Serial.println();
// Pediatrics Ward Display

  Serial.println("Pediatrics Ward Devices");
  Serial.println("Device: Infant Warmer | ID: 78X9Y1");
  Serial.println("Last Seen: 16:05 | Status: In Use");
  Serial.println("Device: Nebulizer | ID: 65B7C8");
  Serial.println("Last Seen: 16:08 | Status: Available");
  Serial.println("ALERT: Pulse Oximeter (ID: 32P5Q6) MISSING!");




        delay(500);
        digitalWrite(LED_PIN, LOW);
         digitalWrite(BUZZER_PIN, LOW);

    // Pin Modes
    pinMode(UP_BUTTON, INPUT_PULLUP);
    pinMode(DOWN_BUTTON, INPUT_PULLUP);
    pinMode(ENTER_BUTTON, INPUT_PULLUP);
    
  
    showLoadingScreen();

    displayMenu();
  
}

void loop() {
    // Read RF data
 vw_rx_start(); // Start the receiver PLL running
uint8_t buf[VW_MAX_MESSAGE_LEN];
uint8_t buflen = VW_MAX_MESSAGE_LEN;
   

if (vw_get_message(buf, &buflen)) // making Non-blocking

{
if(buf[0]=='0')
{

digitalWrite(LED_PIN, HIGH);
foundLocation = ">ICU";

}
if(buf[0]=='1')
{

digitalWrite(LED_PIN, HIGH);
foundLocation = ">Emergency";

}
if(buf[0]=='2')
{

digitalWrite(LED_PIN, HIGH);
foundLocation = ">Theater";

}
if(buf[0]=='3')
{

digitalWrite(LED_PIN, HIGH);
foundLocation = ">Pediatrics";

}
} 
    
    // Button for  Navigation
    if (digitalRead(UP_BUTTON) == LOW) {
        menuIndex = (menuIndex - 1 + 4) % 4;
        displayMenu();
        delay(300);
    }
    
    if (digitalRead(DOWN_BUTTON) == LOW) {
        menuIndex = (menuIndex + 1) % 4;
        displayMenu();
        delay(300);
    }

    // ENTER button triggers search different locations
    if (digitalRead(ENTER_BUTTON) == LOW) {
        showSearchingAnimation();
        displayLocation();
    }
}

