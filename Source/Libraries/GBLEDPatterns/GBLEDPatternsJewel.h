/****************************************************************************************
   Michael Simone - Arduino Source for 2020 Ghostbusters Proton Pack
   This is a modified Adafruit library to handle the routine operations for both the Cyclotron and Powercell.
   The following are the pattern types supported:


*/
enum  pattern { NONE, POWERCELL, POWERCELL_BOOT, CYCLOTRON, CYCLOTRON_BOOT, POWERDOWNPACK, VENT };
// Patern directions supported:
enum  direction { FORWARD, REVERSE };

// NeoPattern Class - derived from the Adafruit_NeoPixel class
class NeoPatterns : public Adafruit_NeoPixel
{
  public:

    // *** Pattern variables for both the Cyclotron and Powercell
    pattern  ActivePattern1;        // which Cycltron pattern is running
    pattern  ActivePattern2;        // which Powercell pattern is running

    direction Direction;            // direction to run the pattern

    /*
       The following are interval variables to manage timing and the change of timing during firing
    */

    unsigned long Interval;         // Cyclotron milliseconds between updates (These are set in the main program)
    unsigned long IntervalChange;
    unsigned long Interval2;        // Cyclotron milliseconds between updates (These are set in the main program)
    unsigned long lastUpdateC;      // last update of position
    unsigned long lastUpdatePC;     // last update of position
    uint8_t themeMode;

    unsigned long FadeCyc = 60;
    unsigned long FadeCycChange = 60;
    unsigned long cycfadeTimer = 10;
    unsigned long cycfadeTimerChange = 10;
    unsigned long lastFade = 0;
    int fadeStep = 0;               // state variable for fade function

    // Added these variables to handle the change in fading speeds when firing
    bool fadeComplete = false;
    bool IntervalChangeFlag = false;

    /*
       The following provide an index position of the LED's to determine which one you want to change in the loop sequence
    */

    uint16_t IndexC;                // current step within the pattern (Cyclotron)
    uint8_t  IndexPC;               // current step within the pattern (PowerCell)

    uint16_t TotalStepsC;           // total number of steps in the Cyclotron pattern
    uint16_t TotalStepsPC;          // total number of steps in the PowerCell pattern

    uint32_t Color1, Color2;        // What colors are in use

    uint16_t PCSegIndex;            //

    /*
       Cyclotron & Powercell references
    */

    const uint8_t CyclotronLEDs = 28;
    const uint8_t PowerCellLEDs = 16;

    const uint8_t CyclotronLED1Start = 0;
    const uint8_t CyclotronLED1End = 6;
    const uint8_t CyclotronLED2Start = 7;
    const uint8_t CyclotronLED2End = 13;
    const uint8_t CyclotronLED3Start = 14;
    const uint8_t CyclotronLED3End = 20;
    const uint8_t CyclotronLED4Start = 21;
    const uint8_t CyclotronLED4End = 27;

    bool cycNormalStart = false;      // Used on the first routine to clear all LEDs before you start the sequence

    /*
       Call back functions to let you know when a routine is complete E.g. let you know when the pack is powered up or down.
    */

    void (*OnCompleteC)();  // Callback on completion of pattern
    void (*OnCompletePC)();  // Callback on completion of pattern

    // Constructor - calls base-class constructor to initialize strip
    NeoPatterns(uint16_t pixels, uint8_t pin, uint8_t type, void (*callback)())
      : Adafruit_NeoPixel(pixels, pin, type)
    {
      //OnCompleteC = callback;
      OnCompletePC = callback;
    }

    /*
        Main Update that gets called to refresh all your LED's, this checks the routine
        and calls the appropriate updates.
    */
    void Update()
    {
      if ((millis() - lastUpdateC) > Interval) // Time to update Cyclotron ?
      {
        lastUpdateC = millis();
        switch (ActivePattern1)
        {
          case CYCLOTRON:
            CyclotronUpdate();
            break;
          case CYCLOTRON_BOOT:
            CyclotronBootUpdate();
            break;
          case VENT:
            break;
          case POWERDOWNPACK:
            break;
          default:
            break;
        }
      }
      if ((millis() - lastUpdatePC) > Interval2) // Time to update Cyclotron PowerCell?
      {
        lastUpdatePC = millis();
        switch (ActivePattern2)
        {
          case POWERCELL:
            PowercellUpdate();
            break;
          case POWERCELL_BOOT:
            PowercellBootUpdate();
            break;
          case POWERDOWNPACK:
            PowerDownUpdate();
            break;
          case VENT:
            VentPackUpdate();
          default:
            break;
        }
      }
      /*
         Added to handle the fading for the cyclotron.
         Only if the pattern is CYCLOTRON, the routine has started and the theme is not Christmas
         Note - The Christmas theme does not fade
      */
      if (ActivePattern1 == CYCLOTRON && cycNormalStart == true && themeMode != 4)
      {

        if ((millis() - lastFade) > cycfadeTimer) // time to update
        {
          lastFade = millis();
          switch (IndexC)
          {
            case 0:
              if (fadeComplete == false)
              {
                if (themeMode != 2)
                {
                  fade(Red(Color1), 0, Green(Color1), 0, Blue(Color1), 0, FadeCyc, CyclotronLED3Start, CyclotronLED3End);
                }
                else
                {
                  fade(Red(Color1), 0, Green(Color1), 0, Blue(Color1), 0, 120, CyclotronLED3Start, CyclotronLED3End);
                }
              }
              break;
            case 1:
              if (fadeComplete == false)
              {
                if (themeMode != 2)
                {
                  fade(Red(Color1), 0, Green(Color1), 0, Blue(Color1), 0, FadeCyc, CyclotronLED4Start, CyclotronLED4End);
                }
                else
                {
                  fade(Red(Color1), 0, Green(Color1), 0, Blue(Color1), 0, 120, CyclotronLED4Start, CyclotronLED4End);
                }
              }
              break;

            case 2:
              if (fadeComplete == false)
              {
                if (themeMode != 2)
                {
                  fade(Red(Color1), 0, Green(Color1), 0, Blue(Color1), 0, FadeCyc, CyclotronLED1Start, CyclotronLED1End);
                }
                else
                {
                  fade(Red(Color1), 0, Green(Color1), 0, Blue(Color1), 0, 120, CyclotronLED1Start, CyclotronLED1End);
                }
              }
              break;
            case 3:
              if (fadeComplete == false)
              {
                if (themeMode != 2)
                {
                  fade(Red(Color1), 0, Green(Color1), 0, Blue(Color1), 0, FadeCyc, CyclotronLED2Start, CyclotronLED2End);
                }
                else
                {
                  fade(Red(Color1), 0, Green(Color1), 0, Blue(Color1), 0, 120, CyclotronLED2Start, CyclotronLED2End);
                }
              }
              break;
          }
        }
      }
    }

    /*
       Increment the cyclotron state (4 states, called after every interval)
    */
    void IncrementC()
    {
      if (ActivePattern1 == CYCLOTRON)
      {
        IndexC++;
        /*
           Added the following to sync the changes for timed interval changes
        */
        if (IntervalChangeFlag == true)
        {
          IntervalChangeCall();
          IntervalChangeFlag = false;
        }
        // Reset the counter if you have completed through all 4 states
        if (IndexC >= TotalStepsC)
        {
          IndexC = 0;
          if (OnCompleteC != NULL)
          {
            OnCompleteC(); // call the completion callback
          }
        }
      }
    }
    /*
       Increment the Powercell LED state
    */
    void IncrementPC()
    {
      if (ActivePattern2 != POWERDOWNPACK)      // *** dont continue if in the powerdown state *** //
      {
        if (Direction == FORWARD)
        {
          IndexPC++;
          if (IndexPC >= TotalStepsPC)
          {
            IndexPC = CyclotronLEDs;            // *** Reset the index (After the Cyclotron LEDs) *** //
            if (OnCompletePC != NULL)
            {
              OnCompletePC(); // call the completion callback
            }
          }
        }
        else // Direction == REVERSE
        {
          --IndexPC;
          if (ActivePattern2 == POWERCELL_BOOT)
          {
            if (IndexPC - PCSegIndex < CyclotronLEDs)
            {
              IndexPC = TotalStepsPC - 1;               // *** If the powercell index reaches the cyclotron index, reset back to the top
              PCSegIndex++;                             // -1 to account for the zero position
              if (PCSegIndex == 15)
              {
                if (OnCompletePC != NULL)
                {
                  OnCompletePC(); // call the completion callback
                }
              }
            }
          }
          /*
             This should not be called, Reverse Powercell is used during boot, forward in the normal state. This could be cleaned up
          */
          if (IndexPC <= 0)
          {
            IndexPC = TotalStepsPC - 1;
            if (OnCompletePC != NULL)
            {
              ActivePattern1 = NONE;
              ActivePattern2 = NONE;
              OnCompletePC(); // call the completion callback
            }
          }
        }
      }
      else
      {
        /*
           This would be normally called during a power down but the segment system is not used
        */
        IndexPC++;
        if (IndexPC >= TotalStepsPC)
        {
          IndexPC = 0;
          if (OnCompletePC != NULL)
          {
            OnCompletePC(); // call the completion callback
          }
        }
      }
    }

    /*
        Initialize for the Powercell Boot
    */
    void PowercellBoot(uint32_t color1, uint32_t interval, direction dir = REVERSE)
    {
      // ** Reverse direction, reset the powercell index variables ** //
      ActivePattern2 = POWERCELL_BOOT;
      Interval2 = interval;
      PCSegIndex = 0;
      TotalStepsPC = CyclotronLEDs + PowerCellLEDs;
      Color2 = color1;
      IndexPC = TotalStepsPC;
      Direction = dir;
    }

    /*
         Initialize for the Powercell Normal State
    */
    void Powercell(uint32_t color1, uint32_t interval, direction dir = FORWARD)
    {
      // ** Forward direction, reset the powercell index variables ** //
      ActivePattern2 = POWERCELL;
      Interval2 = interval;
      TotalStepsPC = CyclotronLEDs + PowerCellLEDs;
      Color2 = color1;
      IndexPC = CyclotronLEDs;
      Direction = dir;
    }

    /*
        Initialize Cyclotron Boot Cycle
    */
    void CyclotronBoot(uint32_t color1, uint32_t interval, uint8_t tmode)
    {
      // ** reset the cyclotron index variables ** //
      ActivePattern1 = CYCLOTRON_BOOT;
      themeMode = tmode;
      Interval = interval;
      TotalStepsC = 4;
      Color1 = color1;
      IndexC = 0;
      fadeStep = 0;
    }

    /*
       Initialize Normal Cyclotron Cycle
    */
    void Cyclotron(uint32_t color1, uint32_t interval, uint8_t tmode)  //enum PackTheme { MOVIE, STATIS, SLIME, MESON, CHRISTMAS };
    {
      // ** reset the cyclotron index variables ** //
      IndexC = 0;
      themeMode = tmode;
      ActivePattern1 = CYCLOTRON;
      Interval = interval;
      fadeStep = 0;
      cycNormalStart = false;           // has the routine started yet? ( used to clear LED's and start the cycle again )
      fadeComplete = false;
      TotalStepsC = 4;
      lastFade = 0;

      /*
        Changes the cyclotron to the THEME colors
      */
      switch (themeMode)
      {
        case 0:
          Color1 = Wheel(255);
          break;
        case 1:
          Color1 = Wheel(190);
          break;
        case 2:
          Color1 = Wheel(85);
          for (uint8_t i = 0; i < CyclotronLEDs; i++)
          {
            setPixelColor(i, Color1);
          }
          break;
        case 3:
          Color1 = Wheel(42);
          break;
        case 4:
          Color1 = Wheel(85);
          break;
      }
    }

    /*
      Initialize Venting
    */
    void VentPack()
    {
      ActivePattern1 = VENT;
      ActivePattern2 = VENT;
    }

    /*
      Initialize Powerdown Sequence
    */
    void PowerDown(uint8_t interval)
    {
      ActivePattern1 = POWERDOWNPACK;
      Interval = interval;
      TotalStepsC = CyclotronLEDs;
      IndexC = 0;
      ActivePattern2 = POWERDOWNPACK;
      Interval2 = 100;
      TotalStepsPC = 35;
      IndexPC = 0;
      Direction = REVERSE;
    }

    /*
      Update the PowerCell LEDs using the Boot Sequence
    */
    void PowercellBootUpdate()
    {
      for (uint8_t i = CyclotronLEDs; i < CyclotronLEDs + PowerCellLEDs; i++)
      {
        if (i == IndexPC)  // Scan Pixel to the right
        {
          setPixelColor(i, Color2);
        }
        else
        {
          for (uint8_t j = CyclotronLEDs + PCSegIndex; j < CyclotronLEDs + PowerCellLEDs; j++)
          {
            if (j != IndexPC )
            {
              setPixelColor(j, Color(0, 0, 0));
            }
          }
        }
      }
      show();
      IncrementPC();
    }

    /*
      Update the PowerCell using the Normal Sequence
    */
    void PowercellUpdate()
    {
      for (uint8_t i = CyclotronLEDs; i < CyclotronLEDs + PowerCellLEDs; i++)
      {

        if (i == IndexPC)  // Scan Pixel to the right
        {
          setPixelColor(i, Color2);
        }
        if (IndexPC == (CyclotronLEDs + PowerCellLEDs - 1))
        {
          for (uint8_t j = CyclotronLEDs; j < CyclotronLEDs + PowerCellLEDs - 1; j++)
          {
            setPixelColor(j, Color(0, 0, 0));
          }
        }

      }
      show();
      IncrementPC();
    }

    /*
      Update the Cyclotron using the Boot Sequence
    */
    void CyclotronBootUpdate()
    {
      // ** Fade in the correct THEME color ** //
      switch (themeMode)
      {
        case 0:
          fade(0, 255, 0, 0, 0, 0, 200, CyclotronLED1Start, CyclotronLED4End);
          break;
        case 1:
          fade(0, 60, 0, 0, 0, 190, 200, CyclotronLED1Start, CyclotronLED4End);
          break;
        case 2:
          fade(0, 0, 0, 255, 0, 0, 200, CyclotronLED1Start, CyclotronLED4End);
          break;
        case 3:
          fade(0, 129, 0, 126, 0, 0, 200, CyclotronLED1Start, CyclotronLED4End);
          break;
        case 4:
          fade(0, 0, 0, 255, 0, 0, 200, CyclotronLED1Start, CyclotronLED4End);
          break;
      }
      show();
      IncrementC();
    }

    /*
      Update the Cyclotron using the Normal Sequence
    */
    void CyclotronUpdate()
    {
      fadeStep = 0;
      fadeComplete = false;
      if (themeMode != 4)                   // *** Update LED's using normal pattern unless in Christmas Mode *** //
      {
        switch (IndexC)
        {
          case 0:
            if (cycNormalStart == false && themeMode != 2)     // *** First clear boot LED's unless in Slime Mode *** //
            {
              for (int i = CyclotronLED1Start; i <= CyclotronLED4End; i++)
              {
                setPixelColor(i, 0);
              }
            }
            cyclotronLEDUpdate(CyclotronLED1Start,  CyclotronLED1End,  1);
            //              cyclotronLEDUpdate(CyclotronLED2Start,  CyclotronLED2End,  0);
            //              cyclotronLEDUpdate(CyclotronLED3Start,  CyclotronLED3End,  0);
            //              cyclotronLEDUpdate(CyclotronLED4Start,  CyclotronLED4End,  0);

            break;
          case 1:
            cycNormalStart = true;

            cyclotronLEDUpdate(CyclotronLED2Start,  CyclotronLED2End,  1);
            //              cyclotronLEDUpdate(CyclotronLED1Start,  CyclotronLED1End,  0);
            //              cyclotronLEDUpdate(CyclotronLED3Start,  CyclotronLED3End,  0);
            //              cyclotronLEDUpdate(CyclotronLED4Start,  CyclotronLED4End,  0);
            break;
          case 2:
            cyclotronLEDUpdate(CyclotronLED3Start,  CyclotronLED3End,  1);
            //              cyclotronLEDUpdate(CyclotronLED1Start,  CyclotronLED1End,  0);
            //              cyclotronLEDUpdate(CyclotronLED2Start,  CyclotronLED2End,  0);
            //              cyclotronLEDUpdate(CyclotronLED4Start,  CyclotronLED4End,  0);

            break;
          case 3:
            //              cyclotronLEDUpdate(CyclotronLED1Start,  CyclotronLED1End,  0);
            //              cyclotronLEDUpdate(CyclotronLED2Start,  CyclotronLED2End,  0);
            //              cyclotronLEDUpdate(CyclotronLED3Start,  CyclotronLED3End,  0);
            cyclotronLEDUpdate(CyclotronLED4Start,  CyclotronLED4End,  1);
            break;
        }
      }
      else
      { 
        // *** Christmas THEME LED routine *** //
        switch (IndexC)
        {
          case 0:
            cyclotronLEDUpdate(CyclotronLED1Start,  CyclotronLED1End,  1);
            cyclotronLEDUpdate(CyclotronLED2Start,  CyclotronLED2End,  2);
            cyclotronLEDUpdate(CyclotronLED3Start,  CyclotronLED3End,  1);
            cyclotronLEDUpdate(CyclotronLED4Start,  CyclotronLED4End,  2);
            break;
          case 1:
            cyclotronLEDUpdate(CyclotronLED1Start,  CyclotronLED1End,  2);
            cyclotronLEDUpdate(CyclotronLED2Start,  CyclotronLED2End,  1);
            cyclotronLEDUpdate(CyclotronLED3Start,  CyclotronLED3End,  2);
            cyclotronLEDUpdate(CyclotronLED4Start,  CyclotronLED4End,  1);
            break;
          case 2:
            cyclotronLEDUpdate(CyclotronLED1Start,  CyclotronLED1End,  1);
            cyclotronLEDUpdate(CyclotronLED2Start,  CyclotronLED2End,  2);
            cyclotronLEDUpdate(CyclotronLED3Start,  CyclotronLED3End,  1);
            cyclotronLEDUpdate(CyclotronLED4Start,  CyclotronLED4End,  2);
            break;
          case 3:
            cyclotronLEDUpdate(CyclotronLED1Start,  CyclotronLED1End,  2);
            cyclotronLEDUpdate(CyclotronLED2Start,  CyclotronLED2End,  1);
            cyclotronLEDUpdate(CyclotronLED3Start,  CyclotronLED3End,  2);
            cyclotronLEDUpdate(CyclotronLED4Start,  CyclotronLED4End,  1);
            break;

        }
      }

      show();
      IncrementC();
    }

    /*
     * Cyclotron LED helper for the Neopixel Jewels
     */
    void cyclotronLEDUpdate(uint8_t cycStart, uint8_t cycEnd, uint8_t cycState)
    {
      switch (cycState)
      {
        case 0:
          for (int i = cycStart; i <= cycEnd; i++)
          {
            setPixelColor(i, 0);
          }
          break;

        case 1:
          for (int i = cycStart; i <= cycEnd; i++)
          {
            setPixelColor(i, Color1);
          }
          break;
        case 2:
          for (int i = cycStart; i <= cycEnd; i++)
          {
            setPixelColor(i, Wheel(255));
          }
          break;
      }
    }
    
    /*
     * Venting Pack LED Update Function
     */
    void VentPackUpdate()
    {
      for (uint8_t i = 0; i < CyclotronLEDs; i++)
      {
        setPixelColor(i, Color1);
      }
      for (uint8_t i = CyclotronLEDs; i < CyclotronLEDs + PowerCellLEDs; i++)
      {
        setPixelColor(i, Color2);
      }
      show();
    }
    
    /*
     * Power down LED Update Function
     */
    void PowerDownUpdate()
    {
      for (uint8_t i = 0; i < numPixels(); i++)
      {
        setPixelColor(i, DimColor(getPixelColor(i)));
      }
      show();
      IncrementPC();
    }
    
    /*
     * Clear Powercell Update Function
     */
    void PowercellClear()
    {
      for (uint8_t i = CyclotronLEDs; i < CyclotronLEDs + PowerCellLEDs; i++)
      {
        setPixelColor(i, Color(0, 0, 0));
      }
      show();
    }

    /*
     * Change the time interval of the powercell during firing
     */
    void PowercellInterval(uint32_t interval)
    {
      Interval2 = interval;
    }

    // Reverse pattern direction
    void Reverse()
    {
      if (Direction == FORWARD)
      {
        Direction = REVERSE;
        IndexC = TotalStepsC - 1;
      }
      else
      {
        Direction = FORWARD;
        IndexC = 0;
      }
    }

    /*
     * Change the time interval of the cyclotron during firing
     */
    void CyclotronInterval(uint32_t interval, uint8_t fadeInt )
    {
      // *** The following adjusts the fading speed to keep up with the cyclotron intervan change *** //
      if (fadeInt && (((float(interval)) / float(Interval)) < 0.92))
      {

        if (cycfadeTimer > 5)
        {
          Serial.println(cycfadeTimer);
          cycfadeTimer--;
          if (FadeCycChange > 0)
          {
            FadeCycChange = FadeCyc - 10;
          }
        }
      }
      IntervalChange = interval;
      IntervalChangeFlag = true;
    }
    
    /*
     * Change the cyclotron intervals
     */
    void IntervalChangeCall()
    {
      Interval = IntervalChange;
      FadeCyc = FadeCycChange;
    }

    /*
     * The following are support functions from Adafruit to help with setting colors, diming.
     */
    // Calculate 50% dimmed version of a color (used by ScannerUpdate)
    uint32_t DimColor(uint32_t color)
    {
      // Shift R, G and B components one bit to the right
      uint32_t dimColor = Color(Red(color) >> 1, Green(color) >> 1, Blue(color) >> 1);
      return dimColor;
    }

    // Returns the Red component of a 32-bit color
    uint8_t Red(uint32_t color)
    {
      return (color >> 16) & 0xFF;
    }

    // Returns the Green component of a 32-bit color
    uint8_t Green(uint32_t color)
    {
      return (color >> 8) & 0xFF;
    }

    // Returns the Blue component of a 32-bit color
    uint8_t Blue(uint32_t color)
    {
      return color & 0xFF;
    }

    // Input a value 0 to 255 to get a color value.
    // The colours are a transition r - g - b - back to r.
    uint32_t Wheel(byte WheelPos)
    {
      WheelPos = 255 - WheelPos;
      if (WheelPos < 85)
      {
        return Color(255 - WheelPos * 3, 0, WheelPos * 3);
      }
      else if (WheelPos < 170)
      {
        WheelPos -= 85;
        return Color(0, WheelPos * 3, 255 - WheelPos * 3);
      }
      else
      {
        WheelPos -= 170;
        return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
      }
    }

    /*
     * This is a fading function that is used to help with the cyclotron fading and power up.
     * 
     * Its sligntly modified to help fade only selected LED's that are called.
     */
    void fade(byte redStartValue, byte redEndValue, byte greenStartValue, byte greenEndValue, byte blueStartValue, byte blueEndValue, int totalSteps, int startLED, int endLED)
    {
      static float redIncrement, greenIncrement, blueIncrement;
      static float red, green, blue;
      static bool fadeUp = false;

      if (fadeStep == 0) // first step is to initialise the initial colour and increments
      {
        red = redStartValue;
        green = greenStartValue;
        blue = blueStartValue;
        fadeUp = false;

        redIncrement = (float)(redEndValue - redStartValue) / (float)totalSteps;
        greenIncrement = (float)(greenEndValue - greenStartValue) / (float)totalSteps;
        blueIncrement = (float)(blueEndValue - blueStartValue) / (float)totalSteps;
        fadeStep = 1; // next time the function is called start the fade

      }
      else  // all other steps make a new colour and display it
      {
        // make new colour
        red += redIncrement;
        green +=  greenIncrement;
        blue += blueIncrement;

        // set up the pixel buffer

        for (byte i = startLED; i <= endLED; i++)
        {
          //Serial.println((byte)red);
          setPixelColor(i, Color((byte)red, (byte)green, (byte)blue));
        }

        // go on to next step
        fadeStep += 1;

        // finished fade
        if (fadeStep >= totalSteps)
        {
          fadeComplete = true;
          if ((byte)red > 0 && (byte)red < 50)
          {
            for (byte i = startLED; i <= endLED; i++)
            {
              //Serial.println((byte)red);
              setPixelColor(i, 0);
            }
          }
          if ((byte)green > 0 && (byte)green < 50)
          {
            for (byte i = startLED; i <= endLED; i++)
            {
              //Serial.println((byte)red);
              setPixelColor(i, 0);
            }
          }
          if ((byte)blue > 0 && (byte)blue < 50)
          {
            for (byte i = startLED; i <= endLED; i++)
            {
              //Serial.println((byte)red);
              setPixelColor(i, 0);
            }
          }
          if (fadeUp) // finished fade up and back
          {
            fadeStep = 0;

            return; // so next call recalabrates the increments
          }

          // now fade back
          fadeUp = true;
          redIncrement = -redIncrement;
          greenIncrement = -greenIncrement;
          blueIncrement = -blueIncrement;
          fadeStep = 1; // don't calculate the increments again but start at first change
        }
      }
    }

};
