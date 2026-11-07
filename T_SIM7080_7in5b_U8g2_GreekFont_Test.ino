/*
  Δοκιμή ελληνικών γραμματοσειρών UTF-8 με U8g2.

  Υλικό / Hardware:
  - LILYGO T-SIM7080 ESP32-S3
  - Waveshare 7.5inch e-Paper HAT (B), 800x480, B/W/Red, SKU 13505

  Προτεινόμενες γραμματοσειρές / Recommended fonts:
  - u8g2_font_10x20_t_greek: κύριο κείμενο
  - u8g2_font_cu12_t_greek: μικρότερο, συμπαγές κείμενο
  - u8g2_font_unifont_t_greek: εναλλακτική Unicode γραμματοσειρά

  Το παράδειγμα χρησιμοποιεί αποκλειστικά U8g2. Ο παλιός χειροποίητος
  renderer 6x10 αφαιρέθηκε, ώστε ο κώδικας να είναι απλούστερος.
*/

#include <Arduino.h>
#include <SPI.h>
#include <GxEPD2_3C.h>
#include <U8g2_for_Adafruit_GFX.h>

// Συνδέσεις e-paper στο T-SIM7080 / e-paper pin connections.
static constexpr int EPD_SCK  = 12;
static constexpr int EPD_MOSI = 11;
static constexpr int EPD_CS   = 10;
static constexpr int EPD_DC   = 13;
static constexpr int EPD_RST  = 14;
static constexpr int EPD_BUSY = 21;
static constexpr int EPD_PWR  = 47;

GxEPD2_3C<GxEPD2_750c_Z08, GxEPD2_750c_Z08::HEIGHT> display(
  GxEPD2_750c_Z08(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY)
);

U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;

// Σχεδιάζει απλό ASCII κείμενο με την ενσωματωμένη γραμματοσειρά GFX.
void drawAsciiText(int16_t x, int16_t y, const char* text, uint16_t color,
                   uint8_t size = 1)
{
  display.setFont(nullptr);
  display.setTextSize(size);
  display.setTextColor(color);
  display.setCursor(x, y);
  display.print(text);
}

// Σχεδιάζει UTF-8 ελληνικό κείμενο μέσω U8g2_for_Adafruit_GFX.
void drawU8g2GreekText(int16_t x, int16_t y, const char* text,
                       const uint8_t* font, uint16_t color)
{
  u8g2Fonts.setFont(font);
  u8g2Fonts.setForegroundColor(color);
  u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
  u8g2Fonts.drawUTF8(x, y, text);
}

/*
  Στη συγκεκριμένη οθόνη, η πρώτη σχεδίαση μετά από αλλαγή γραμματοσειράς
  μπορεί να εμφανίσει μαύρα ορθογώνια. Κάνουμε μία δοκιμαστική σχεδίαση
  εκτός της ορατής περιοχής και καθαρίζουμε αμέσως το framebuffer.

  On this display, the first draw after a font change may produce black
  rectangles. This disposable draw initializes the font renderer safely.
*/
void warmUpU8g2Font(const uint8_t* font, int16_t baseline,
                    int16_t clearTop, int16_t clearHeight)
{
  u8g2Fonts.setFont(font);
  u8g2Fonts.setForegroundColor(GxEPD_BLACK);
  u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
  u8g2Fonts.drawUTF8(24, baseline, "Αθήνα");
  display.fillRect(20, clearTop, display.width() - 40, clearHeight, GxEPD_WHITE);
}

void drawTestPage()
{
  display.setFullWindow();
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);

    // Κόκκινη επικεφαλίδα / Red page header.
    display.fillRect(0, 0, display.width(), 58, GxEPD_RED);
    drawAsciiText(24, 38, "GREEK UTF-8 / U8G2 FONT TEST", GxEPD_WHITE, 2);

    // Κύρια γραμματοσειρά 10x20 / Primary 10x20 font.
    warmUpU8g2Font(u8g2_font_10x20_t_greek, 122, 100, 28);
    drawAsciiText(24, 84, "10x20 - MAIN TEXT", GxEPD_RED, 1);
    drawU8g2GreekText(24, 122, "Αθήνα  Θεσσαλονίκη  Ελλάδα",
                      u8g2_font_10x20_t_greek, GxEPD_BLACK);
    drawU8g2GreekText(24, 157, "Ναύπλιο  Θερμοκρασία 39°  Άνεμος ΒΑ",
                      u8g2_font_10x20_t_greek, GxEPD_BLACK);
    drawU8g2GreekText(24, 192, "Υγρασία 66%  Θερμή ημέρα  Ανδριανή  Ναταλία",
                      u8g2_font_10x20_t_greek, GxEPD_BLACK);

    // Συμπαγής γραμματοσειρά CU12 / Compact CU12 font.
    warmUpU8g2Font(u8g2_font_cu12_t_greek, 263, 246, 22);
    drawAsciiText(24, 230, "CU12 - COMPACT TEXT", GxEPD_RED, 1);
    drawU8g2GreekText(24, 263, "Αθήνα Θεσσαλονίκη Ελλάδα Θερμοκρασία 39°",
                      u8g2_font_cu12_t_greek, GxEPD_BLACK);
    drawU8g2GreekText(24, 290, "ά έ ή ί ό ύ ώ   ϊ ϋ ΐ ΰ   Υγρασία Άνεμος ΒΑ",
                      u8g2_font_cu12_t_greek, GxEPD_BLACK);

    // Unicode εναλλακτική / Unicode alternative.
    warmUpU8g2Font(u8g2_font_unifont_t_greek, 361, 342, 24);
    drawAsciiText(24, 328, "UNIFONT - ALTERNATIVE", GxEPD_RED, 1);
    drawU8g2GreekText(24, 361, "Αθήνα Θεσσαλονίκη Ελλάδα Θερμοκρασία 39°",
                      u8g2_font_unifont_t_greek, GxEPD_BLACK);
    drawU8g2GreekText(24, 388, "ά έ ή ί ό ύ ώ   ϊ ϋ ΐ ΰ   Υγρασία Άνεμος ΒΑ",
                      u8g2_font_unifont_t_greek, GxEPD_BLACK);

    display.drawLine(20, 420, display.width() - 20, 420, GxEPD_RED);
    drawAsciiText(24, 448, "RECOMMENDED: 10x20 MAIN / CU12 COMPACT",
                  GxEPD_BLACK, 1);
  }
  while (display.nextPage());
}

void setup()
{
  Serial.begin(115200);

  // Τροφοδοσία του e-paper HAT από το T-SIM7080.
  pinMode(EPD_PWR, OUTPUT);
  digitalWrite(EPD_PWR, HIGH);
  delay(100);

  SPI.begin(EPD_SCK, -1, EPD_MOSI, EPD_CS);
  display.init(115200, true, 2, false);
  display.setRotation(0);

  u8g2Fonts.begin(display);
  u8g2Fonts.setFontMode(1);       // Διαφανές φόντο / transparent mode.
  u8g2Fonts.setFontDirection(0);  // Κανονική φορά / normal direction.

  drawTestPage();

  // Η εικόνα παραμένει στην e-paper χωρίς συνεχή κατανάλωση.
  display.hibernate();
  digitalWrite(EPD_PWR, LOW);
}

void loop()
{
  // Δεν χρειάζεται επανάληψη: η e-paper διατηρεί την τελευταία εικόνα.
}
