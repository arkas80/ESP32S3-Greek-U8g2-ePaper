/*
  Greek UTF-8 font comparison for Waveshare 7.5inch e-Paper HAT (B),
  800x480 red/black/white (SKU 13505), and LILYGO T-SIM7080 ESP32-S3.

  Compares a handmade 6x10 renderer with these U8g2 Greek fonts:
    u8g2_font_10x20_t_greek
    u8g2_font_cu12_t_greek
    u8g2_font_unifont_t_greek

  The tested recommendation is 10x20 for primary text and CU12 when space
  is limited. See README.md for wiring, library versions and attribution.
*/

#include <Arduino.h>
#include <SPI.h>
#include <GxEPD2_3C.h>
#include <U8g2_for_Adafruit_GFX.h>
#include "GreekFont5x7.h"

static constexpr int EPD_SCK = 12;
static constexpr int EPD_MOSI = 11;
static constexpr int EPD_CS = 10;
static constexpr int EPD_DC = 13;
static constexpr int EPD_RST = 14;
static constexpr int EPD_BUSY = 21;
static constexpr int EPD_PWR = 47;

GxEPD2_3C<GxEPD2_750c_Z08, GxEPD2_750c_Z08::HEIGHT> display(
  GxEPD2_750c_Z08(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY)
);

U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;

uint8_t mapGreekUtf8ToWin1253(uint8_t c1, uint8_t c2)
{
  if (c1 == 0xC2 && c2 == 0xB0)
  {
    return 0xB0; // °
  }

  if (c1 == 0xCE)
  {
    switch (c2)
    {
      case 0x86: return 0xA2; // Ά
      case 0x88: return 0xB8; // Έ
      case 0x89: return 0xB9; // Ή
      case 0x8A: return 0xBA; // Ί
      case 0x8C: return 0xBC; // Ό
      case 0x8E: return 0xBE; // Ύ
      case 0x8F: return 0xBF; // Ώ
      case 0x90: return 0xC0; // ΐ
      case 0x91: return 0xC1; // Α
      case 0x92: return 0xC2; // Β
      case 0x93: return 0xC3; // Γ
      case 0x94: return 0xC4; // Δ
      case 0x95: return 0xC5; // Ε
      case 0x96: return 0xC6; // Ζ
      case 0x97: return 0xC7; // Η
      case 0x98: return 0xC8; // Θ
      case 0x99: return 0xC9; // Ι
      case 0x9A: return 0xCA; // Κ
      case 0x9B: return 0xCB; // Λ
      case 0x9C: return 0xCC; // Μ
      case 0x9D: return 0xCD; // Ν
      case 0x9E: return 0xCE; // Ξ
      case 0x9F: return 0xCF; // Ο
      case 0xA0: return 0xD0; // Π
      case 0xA1: return 0xD1; // Ρ
      case 0xA3: return 0xD3; // Σ
      case 0xA4: return 0xD4; // Τ
      case 0xA5: return 0xD5; // Υ
      case 0xA6: return 0xD6; // Φ
      case 0xA7: return 0xD7; // Χ
      case 0xA8: return 0xD8; // Ψ
      case 0xA9: return 0xD9; // Ω
      case 0xAA: return 0xDA; // Ϊ
      case 0xAB: return 0xDB; // Ϋ
      case 0xAC: return 0xDC; // ά
      case 0xAD: return 0xDD; // έ
      case 0xAE: return 0xDE; // ή
      case 0xAF: return 0xDF; // ί
      case 0xB0: return 0xE0; // ΰ
      case 0xB1: return 0xE1; // α
      case 0xB2: return 0xE2; // β
      case 0xB3: return 0xE3; // γ
      case 0xB4: return 0xE4; // δ
      case 0xB5: return 0xE5; // ε
      case 0xB6: return 0xE6; // ζ
      case 0xB7: return 0xE7; // η
      case 0xB8: return 0xE8; // θ
      case 0xB9: return 0xE9; // ι
      case 0xBA: return 0xEA; // κ
      case 0xBB: return 0xEB; // λ
      case 0xBC: return 0xEC; // μ
      case 0xBD: return 0xED; // ν
      case 0xBE: return 0xEE; // ξ
      case 0xBF: return 0xEF; // ο
    }
  }

  if (c1 == 0xCF)
  {
    switch (c2)
    {
      case 0x80: return 0xF0; // π
      case 0x81: return 0xF1; // ρ
      case 0x82: return 0xF2; // ς
      case 0x83: return 0xF3; // σ
      case 0x84: return 0xF4; // τ
      case 0x85: return 0xF5; // υ
      case 0x86: return 0xF6; // φ
      case 0x87: return 0xF7; // χ
      case 0x88: return 0xF8; // ψ
      case 0x89: return 0xF9; // ω
      case 0x8A: return 0xFA; // ϊ
      case 0x8B: return 0xFB; // ϋ
      case 0x8C: return 0xFC; // ό
      case 0x8D: return 0xFD; // ύ
      case 0x8E: return 0xFE; // ώ
    }
  }

  return '?';
}

const uint8_t *getGreekGlyph(uint8_t code)
{
  static const uint8_t degreeSymbol[5] PROGMEM = {0x02, 0x05, 0x05, 0x02, 0x00};

  static const uint8_t greekAlphaTonosUpper[5] PROGMEM = {0x03, 0x3E, 0x09, 0x3E, 0x00};
  static const uint8_t greekEpsilonTonosUpper[5] PROGMEM = {0x03, 0x3F, 0x25, 0x21, 0x00};
  static const uint8_t greekEtaTonosUpper[5] PROGMEM = {0x03, 0x3F, 0x04, 0x3F, 0x00};
  static const uint8_t greekIotaTonosUpper[5] PROGMEM = {0x03, 0x21, 0x3F, 0x21, 0x00};
  static const uint8_t greekOmicronTonosUpper[5] PROGMEM = {0x03, 0x1E, 0x21, 0x1E, 0x00};
  static const uint8_t greekUpsilonTonosUpper[5] PROGMEM = {0x03, 0x07, 0x38, 0x07, 0x00};
  static const uint8_t greekOmegaTonosUpper[5] PROGMEM = {0x03, 0x2E, 0x11, 0x2E, 0x00};
  static const uint8_t greekIotaDialUpper[5] PROGMEM = {0x00, 0x25, 0x3C, 0x25, 0x00};
  static const uint8_t greekUpsilonDialUpper[5] PROGMEM = {0x0D, 0x30, 0x0D, 0x00, 0x00};

  static const uint8_t greekZetaUpper[5] PROGMEM = {0x61, 0x51, 0x49, 0x45, 0x43};
  static const uint8_t greekEpsilonLower[5] PROGMEM = {0x28, 0x54, 0x54, 0x44, 0x00};
  static const uint8_t greekAlphaTonosLower[5] PROGMEM = {0x38, 0x44, 0x26, 0x7D, 0x00};
  static const uint8_t greekEpsilonTonosLower[5] PROGMEM = {0x28, 0x54, 0x56, 0x45, 0x00};
  static const uint8_t greekEtaTonosLower[5] PROGMEM = {0x3C, 0x06, 0x05, 0x78, 0x00};
  static const uint8_t greekIotaTonosLower[5] PROGMEM = {0x00, 0x4A, 0x79, 0x40, 0x00};
  static const uint8_t greekOmicronTonosLower[5] PROGMEM = {0x18, 0x24, 0x26, 0x19, 0x00};
  static const uint8_t greekUpsilonTonosLower[5] PROGMEM = {0x3C, 0x40, 0x42, 0x3D, 0x00};
  static const uint8_t greekOmegaTonosLower[5] PROGMEM = {0x3C, 0x30, 0x32, 0x3D, 0x00};
  static const uint8_t greekIotaDialLower[5] PROGMEM = {0x00, 0x49, 0x78, 0x41, 0x00};
  static const uint8_t greekUpsilonDialLower[5] PROGMEM = {0x3C, 0x41, 0x40, 0x3D, 0x00};
  static const uint8_t greekIotaDialTonosLower[5] PROGMEM = {0x00, 0x49, 0x7A, 0x41, 0x00};
  static const uint8_t greekUpsilonDialTonosLower[5] PROGMEM = {0x3C, 0x41, 0x42, 0x3D, 0x00};
  static const uint8_t greekGammaLower[5] PROGMEM = {0x02, 0x04, 0x78, 0x08, 0x06};
  static const uint8_t greekMuLower[5] PROGMEM = {0xFC, 0x40, 0x40, 0x20, 0x7C};
  static const uint8_t greekRhoLower[5] PROGMEM = {0xFC, 0x24, 0x24, 0x24, 0x18};
  static const uint8_t greekFinalSigmaLower[5] PROGMEM = {0x98, 0xA4, 0xA4, 0x44, 0x00};
  static const uint8_t greekPhiLower[5] PROGMEM = {0x30, 0x48, 0xFE, 0x48, 0x30};
  static const uint8_t greekChiLower[5] PROGMEM = {0x88, 0x50, 0x20, 0x50, 0x88};
  static const uint8_t greekPsiLower[5] PROGMEM = {0x38, 0x40, 0xFC, 0x40, 0x38};

  switch (code)
  {
    case 0xB0: return degreeSymbol;        // °
    case 0xC0: return greekIotaDialTonosLower;// ΐ
    case 0xA2: return greekAlphaTonosUpper;// Ά
    case 0xB8: return greekEpsilonTonosUpper;// Έ
    case 0xB9: return greekEtaTonosUpper;  // Ή
    case 0xBA: return greekIotaTonosUpper; // Ί
    case 0xBC: return greekOmicronTonosUpper;// Ό
    case 0xBE: return greekUpsilonTonosUpper;// Ύ
    case 0xBF: return greekOmegaTonosUpper;// Ώ
    case 0xDA: return greekIotaDialUpper;  // Ϊ
    case 0xDB: return greekUpsilonDialUpper;// Ϋ
    case 0xC6: return greekZetaUpper;      // Ζ
    case 0xDC: return greekAlphaTonosLower;// ά
    case 0xDD: return greekEpsilonTonosLower;// έ
    case 0xDE: return greekEtaTonosLower;  // ή
    case 0xDF: return greekIotaTonosLower; // ί
    case 0xE0: return greekUpsilonDialTonosLower;// ΰ
    case 0xE3: return greekGammaLower;     // γ
    case 0xE5: return greekEpsilonLower;   // ε
    case 0xEC: return greekMuLower;        // μ
    case 0xF1: return greekRhoLower;       // ρ
    case 0xF2: return greekFinalSigmaLower;// ς
    case 0xF6: return greekPhiLower;       // φ
    case 0xF7: return greekChiLower;       // χ
    case 0xF8: return greekPsiLower;       // ψ
    case 0xFA: return greekIotaDialLower;  // ϊ
    case 0xFB: return greekUpsilonDialLower;// ϋ
    case 0xFC: return greekOmicronTonosLower;// ό
    case 0xFD: return greekUpsilonTonosLower;// ύ
    case 0xFE: return greekOmegaTonosLower;// ώ
    default:
      return &greekFont5x7[static_cast<uint16_t>(code) * 5];
  }
}

void drawGreekChar(int16_t x, int16_t y, uint8_t code, uint16_t color, uint8_t scale)
{
  const uint8_t *glyph = getGreekGlyph(code);

  for (uint8_t col = 0; col < 5; ++col)
  {
    const uint8_t bits = pgm_read_byte(&glyph[col]);
    for (uint8_t row = 0; row < 8; ++row)
    {
      if (bits & (1 << row))
      {
        display.fillRect(x + col * scale, y + row * scale, scale, scale, color);
      }
    }
  }
}

void drawGreekText(int16_t x, int16_t y, const char *text, uint16_t color, uint8_t scale)
{
  int16_t cursorX = x;
  const uint8_t step = 6 * scale;

  for (size_t i = 0; text[i] != '\0';)
  {
    const uint8_t c1 = static_cast<uint8_t>(text[i]);
    uint8_t code = c1;

    if (c1 >= 0xC0 && text[i + 1] != '\0')
    {
      const uint8_t c2 = static_cast<uint8_t>(text[i + 1]);
      code = mapGreekUtf8ToWin1253(c1, c2);
      i += 2;
    }
    else
    {
      i += 1;
    }

    if (code == '\n')
    {
      cursorX = x;
      y += 10 * scale;
      continue;
    }

    drawGreekChar(cursorX, y, code, color, scale);
    cursorX += step;
  }
}

uint16_t readUtf8Codepoint(const char *text, size_t &i)
{
  const uint8_t c1 = static_cast<uint8_t>(text[i]);

  if (c1 < 0x80)
  {
    i += 1;
    return c1;
  }

  if ((c1 & 0xE0) == 0xC0 && text[i + 1] != '\0')
  {
    const uint8_t c2 = static_cast<uint8_t>(text[i + 1]);
    i += 2;
    return static_cast<uint16_t>(((c1 & 0x1F) << 6) | (c2 & 0x3F));
  }

  i += 1;
  return '?';
}

const uint8_t *getGreek6x10Glyph(uint16_t codepoint)
{
  static const uint8_t degree[10] PROGMEM = {0x00, 0x60, 0x90, 0x90, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};

  static const uint8_t alphaTonosUpper[10] PROGMEM = {0xA0, 0x50, 0x88, 0x88, 0xF8, 0x88, 0x88, 0x00, 0x00, 0x00};
  static const uint8_t alphaUpper[10] PROGMEM = {0x00, 0x20, 0x50, 0x88, 0x88, 0xF8, 0x88, 0x88, 0x00, 0x00};
  static const uint8_t gammaUpper[10] PROGMEM = {0x00, 0xF8, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00};
  static const uint8_t deltaUpper[10] PROGMEM = {0x00, 0x20, 0x50, 0x50, 0x50, 0x88, 0x88, 0xF8, 0x00, 0x00};
  static const uint8_t epsilonUpper[10] PROGMEM = {0x00, 0xF8, 0x80, 0x80, 0xF0, 0x80, 0x80, 0xF8, 0x00, 0x00};
  static const uint8_t etaUpper[10] PROGMEM = {0x00, 0x88, 0x88, 0x88, 0xF8, 0x88, 0x88, 0x88, 0x00, 0x00};
  static const uint8_t iotaUpper[10] PROGMEM = {0x00, 0x70, 0x20, 0x20, 0x20, 0x20, 0x20, 0x70, 0x00, 0x00};
  static const uint8_t muUpper[10] PROGMEM = {0x00, 0x88, 0x88, 0xD8, 0xA8, 0x88, 0x88, 0x88, 0x00, 0x00};
  static const uint8_t nuUpper[10] PROGMEM = {0x00, 0x88, 0x88, 0xC8, 0xA8, 0x98, 0x88, 0x88, 0x00, 0x00};
  static const uint8_t sigmaUpper[10] PROGMEM = {0x00, 0xF8, 0x80, 0x40, 0x20, 0x40, 0x80, 0xF8, 0x00, 0x00};

  static const uint8_t iotaDialTonos[10] PROGMEM = {0x10, 0xA8, 0x00, 0x20, 0x20, 0x20, 0x28, 0x10, 0x00, 0x00};
  static const uint8_t alphaTonos[10] PROGMEM = {0x10, 0x20, 0x00, 0x70, 0x88, 0x88, 0x98, 0x68, 0x00, 0x00};
  static const uint8_t epsilonTonos[10] PROGMEM = {0x10, 0x20, 0x00, 0x70, 0x88, 0x60, 0x88, 0x70, 0x00, 0x00};
  static const uint8_t etaTonos[10] PROGMEM = {0x10, 0x20, 0x00, 0xB0, 0xC8, 0x88, 0x88, 0x88, 0x08, 0x08};
  static const uint8_t iotaTonos[10] PROGMEM = {0x20, 0x40, 0x00, 0x40, 0x40, 0x40, 0x50, 0x20, 0x00, 0x00};
  static const uint8_t upsilonDialTonos[10] PROGMEM = {0x10, 0xA8, 0x00, 0x88, 0x88, 0x88, 0x88, 0x70, 0x00, 0x00};
  static const uint8_t alpha[10] PROGMEM = {0x00, 0x00, 0x00, 0x70, 0x88, 0x88, 0x98, 0x68, 0x00, 0x00};
  static const uint8_t gamma[10] PROGMEM = {0x00, 0x00, 0x00, 0x88, 0x88, 0x50, 0x50, 0x20, 0x20, 0x20};
  static const uint8_t delta[10] PROGMEM = {0x00, 0x70, 0x88, 0x60, 0x90, 0x88, 0x88, 0x70, 0x00, 0x00};
  static const uint8_t epsilon[10] PROGMEM = {0x00, 0x00, 0x00, 0x70, 0x88, 0x60, 0x88, 0x70, 0x00, 0x00};
  static const uint8_t zeta[10] PROGMEM = {0x00, 0xF8, 0x20, 0x40, 0x40, 0x80, 0x80, 0x70, 0x08, 0x30};
  static const uint8_t eta[10] PROGMEM = {0x00, 0x00, 0x00, 0xB0, 0xC8, 0x88, 0x88, 0x88, 0x08, 0x08};
  static const uint8_t theta[10] PROGMEM = {0x00, 0x30, 0x48, 0x48, 0x78, 0x48, 0x48, 0x30, 0x00, 0x00};
  static const uint8_t iota[10] PROGMEM = {0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x50, 0x20, 0x00, 0x00};
  static const uint8_t kappa[10] PROGMEM = {0x00, 0x00, 0x00, 0x88, 0x90, 0xE0, 0x90, 0x88, 0x00, 0x00};
  static const uint8_t lambda[10] PROGMEM = {0x00, 0x70, 0x88, 0x68, 0x98, 0x88, 0x88, 0x88, 0x00, 0x00};
  static const uint8_t mu[10] PROGMEM = {0x00, 0x00, 0x00, 0x88, 0x88, 0x88, 0xD8, 0xA8, 0x80, 0x80};
  static const uint8_t nu[10] PROGMEM = {0x00, 0x00, 0x00, 0x88, 0x88, 0x50, 0x50, 0x20, 0x00, 0x00};
  static const uint8_t omicron[10] PROGMEM = {0x00, 0x00, 0x00, 0x70, 0x88, 0x88, 0x88, 0x70, 0x00, 0x00};
  static const uint8_t pi[10] PROGMEM = {0x00, 0x00, 0x00, 0xF8, 0x50, 0x50, 0x50, 0x50, 0x00, 0x00};
  static const uint8_t rho[10] PROGMEM = {0x00, 0x00, 0x00, 0x70, 0x88, 0x88, 0x88, 0xF0, 0x80, 0x80};
  static const uint8_t finalSigma[10] PROGMEM = {0x00, 0x00, 0x00, 0x70, 0x88, 0x80, 0x80, 0x70, 0x08, 0x30};
  static const uint8_t sigma[10] PROGMEM = {0x00, 0x00, 0x00, 0x78, 0x90, 0x88, 0x88, 0x70, 0x00, 0x00};
  static const uint8_t tau[10] PROGMEM = {0x00, 0x00, 0x00, 0xF8, 0x20, 0x20, 0x20, 0x10, 0x00, 0x00};
  static const uint8_t upsilon[10] PROGMEM = {0x00, 0x00, 0x00, 0x88, 0x88, 0x88, 0x88, 0x70, 0x00, 0x00};
  static const uint8_t omega[10] PROGMEM = {0x00, 0x00, 0x00, 0x88, 0xA8, 0xA8, 0xA8, 0x50, 0x00, 0x00};
  static const uint8_t iotaDial[10] PROGMEM = {0x00, 0x88, 0x00, 0x20, 0x20, 0x20, 0x28, 0x10, 0x00, 0x00};
  static const uint8_t upsilonDial[10] PROGMEM = {0x00, 0x88, 0x00, 0x88, 0x88, 0x88, 0x88, 0x70, 0x00, 0x00};
  static const uint8_t omicronTonos[10] PROGMEM = {0x10, 0x20, 0x00, 0x70, 0x88, 0x88, 0x88, 0x70, 0x00, 0x00};
  static const uint8_t upsilonTonos[10] PROGMEM = {0x10, 0x20, 0x00, 0x88, 0x88, 0x88, 0x88, 0x70, 0x00, 0x00};
  static const uint8_t omegaTonos[10] PROGMEM = {0x10, 0x20, 0x00, 0x88, 0xA8, 0xA8, 0xA8, 0x50, 0x00, 0x00};

  switch (codepoint)
  {
    case 0x00B0: return degree;
    case 0x0386: return alphaTonosUpper;
    case 0x0391: return alphaUpper;
    case 0x0393: return gammaUpper;
    case 0x0394: return deltaUpper;
    case 0x0395: return epsilonUpper;
    case 0x0397: return etaUpper;
    case 0x0399: return iotaUpper;
    case 0x039C: return muUpper;
    case 0x039D: return nuUpper;
    case 0x03A3: return sigmaUpper;
    case 0x0390: return iotaDialTonos;
    case 0x03AC: return alphaTonos;
    case 0x03AD: return epsilonTonos;
    case 0x03AE: return etaTonos;
    case 0x03AF: return iotaTonos;
    case 0x03B1: return alpha;
    case 0x03B3: return gamma;
    case 0x03B4: return delta;
    case 0x03B5: return epsilon;
    case 0x03B6: return zeta;
    case 0x03B7: return eta;
    case 0x03B8: return theta;
    case 0x03B9: return iota;
    case 0x03BA: return kappa;
    case 0x03BB: return lambda;
    case 0x03BC: return mu;
    case 0x03BD: return nu;
    case 0x03BF: return omicron;
    case 0x03C0: return pi;
    case 0x03C1: return rho;
    case 0x03C2: return finalSigma;
    case 0x03C3: return sigma;
    case 0x03C4: return tau;
    case 0x03C5: return upsilon;
    case 0x03C9: return omega;
    case 0x03CA: return iotaDial;
    case 0x03CB: return upsilonDial;
    case 0x03CC: return omicronTonos;
    case 0x03CD: return upsilonTonos;
    case 0x03CE: return omegaTonos;
    case 0x03B0: return upsilonDialTonos;
    default: return nullptr;
  }
}

void drawGreek6x10Char(int16_t x, int16_t y, uint16_t codepoint, uint16_t color, uint8_t scale)
{
  const uint8_t *glyph = getGreek6x10Glyph(codepoint);

  if (glyph == nullptr)
  {
    if (codepoint < 0x80)
    {
      display.drawChar(x, y + 2 * scale, static_cast<char>(codepoint), color, GxEPD_WHITE, scale);
      return;
    }

    display.drawChar(x, y + 2 * scale, '?', color, GxEPD_WHITE, scale);
    return;
  }

  for (uint8_t row = 0; row < 10; ++row)
  {
    const uint8_t bits = pgm_read_byte(&glyph[row]);
    for (uint8_t col = 0; col < 5; ++col)
    {
      if (bits & (0x80 >> col))
      {
        display.fillRect(x + col * scale, y + row * scale, scale, scale, color);
      }
    }
  }
}

void drawGreek6x10Text(int16_t x, int16_t y, const char *text, uint16_t color, uint8_t scale)
{
  int16_t cursorX = x;
  const uint8_t step = 7 * scale;

  for (size_t i = 0; text[i] != '\0';)
  {
    const uint16_t codepoint = readUtf8Codepoint(text, i);

    if (codepoint == '\n')
    {
      cursorX = x;
      y += 12 * scale;
      continue;
    }

    drawGreek6x10Char(cursorX, y, codepoint, color, scale);
    cursorX += step;
  }
}

void drawU8g2GreekTextFont(int16_t x, int16_t baseline, const char *text,
                          uint16_t color, const uint8_t *font)
{
  u8g2Fonts.setFontMode(1);
  u8g2Fonts.setFontDirection(0);
  u8g2Fonts.setForegroundColor(color);
  u8g2Fonts.setFont(font);
  u8g2Fonts.drawUTF8(x, baseline, text);
}

void drawU8g2GreekText(int16_t x, int16_t baseline, const char *text, uint16_t color)
{
  drawU8g2GreekTextFont(x, baseline, text, color, u8g2_font_10x20_t_greek);
}

void drawGreekFontTest()
{
  display.setRotation(0);
  display.setFullWindow();

  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    display.drawRect(0, 0, display.width(), display.height(), GxEPD_BLACK);

    display.fillRect(0, 0, display.width(), 44, GxEPD_RED);
    drawGreekText(24, 12, "GREEK FONT COMPARISON", GxEPD_WHITE, 2);

    drawGreekText(24, 52, "HANDMADE 6x10:", GxEPD_RED, 2);
    drawGreek6x10Text(24, 76, "Α Γ Δ Ε Η Θ Ι Μ Ν Σ", GxEPD_BLACK, 2);
    drawGreek6x10Text(24, 102, "α γ δ ε ζ η θ ι κ λ μ ν ο π ρ σ ς τ υ ω", GxEPD_BLACK, 2);
    drawGreek6x10Text(24, 128, "ά έ ή ί ό ύ ώ   ϊ ϋ ΐ ΰ", GxEPD_BLACK, 2);
    drawGreek6x10Text(24, 154, "Ναύπλιο  Θερμοκρασία 39°", GxEPD_BLACK, 2);
    drawGreek6x10Text(24, 180, "Υγρασία 66%  Άνεμος ΒΑ", GxEPD_BLACK, 2);

    display.drawFastHLine(24, 208, display.width() - 48, GxEPD_RED);
    drawGreekText(24, 218, "U8G2 GREEK SIZE TEST:", GxEPD_RED, 2);

    // Προθέρμανση U8g2: η πρώτη σχεδίαση μπορεί να αφήσει μαύρα κουτάκια.
    // U8g2 warm-up: erase this disposable pass before the real text.
    drawU8g2GreekText(24, 258, "Αθήνα", GxEPD_BLACK);
    display.fillRect(20, 236, display.width() - 40, 30, GxEPD_WHITE);

    drawGreekText(24, 240, "10x20:", GxEPD_RED, 1);
    drawU8g2GreekTextFont(24, 270, "Αθήνα  Θεσσαλονίκη  Ελλάδα", GxEPD_BLACK,
                          u8g2_font_10x20_t_greek);
    drawU8g2GreekTextFont(24, 294, "Ναύπλιο  Θερμοκρασία 39°  Άνεμος ΒΑ", GxEPD_BLACK,
                          u8g2_font_10x20_t_greek);

    drawGreekText(24, 306, "CU12:", GxEPD_RED, 1);
    // Κάθε αλλαγή font χρειάζεται μία προσωρινή σχεδίαση και καθάρισμα.
    // Each font change also needs one disposable draw on this target.
    drawU8g2GreekTextFont(24, 330, "Αθήνα", GxEPD_BLACK,
                          u8g2_font_cu12_t_greek);
    display.fillRect(20, 314, display.width() - 40, 22, GxEPD_WHITE);
    drawU8g2GreekTextFont(24, 330, "Αθήνα  Θεσσαλονίκη  Ελλάδα  Θερμοκρασία 39°", GxEPD_BLACK,
                          u8g2_font_cu12_t_greek);
    drawU8g2GreekTextFont(24, 348, "ά έ ή ί ό ύ ώ   ϊ ϋ ΐ ΰ   Υγρασία  Άνεμος ΒΑ", GxEPD_BLACK,
                          u8g2_font_cu12_t_greek);

    drawGreekText(24, 362, "UNIFONT:", GxEPD_RED, 1);
    drawU8g2GreekTextFont(24, 390, "Αθήνα", GxEPD_BLACK,
                          u8g2_font_unifont_t_greek);
    display.fillRect(20, 370, display.width() - 40, 25, GxEPD_WHITE);
    drawU8g2GreekTextFont(24, 390, "Αθήνα  Θεσσαλονίκη  Ελλάδα  Θερμοκρασία 39°", GxEPD_BLACK,
                          u8g2_font_unifont_t_greek);
    drawU8g2GreekTextFont(24, 412, "ά έ ή ί ό ύ ώ   ϊ ϋ ΐ ΰ   Υγρασία  Άνεμος ΒΑ", GxEPD_BLACK,
                          u8g2_font_unifont_t_greek);
  } while (display.nextPage());
}

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println();
  Serial.println("Greek 5x7 e-paper font test starting...");

  pinMode(EPD_PWR, OUTPUT);
  digitalWrite(EPD_PWR, HIGH);
  delay(100);

  SPI.begin(EPD_SCK, -1, EPD_MOSI, EPD_CS);
  display.epd2.selectSPI(SPI, SPISettings(2000000, MSBFIRST, SPI_MODE0));
  display.init(115200, true, 2, false);
  u8g2Fonts.begin(display);

  drawGreekFontTest();
  display.hibernate();
  Serial.println("Done. Display is in hibernate mode.");
}

void loop() {}
