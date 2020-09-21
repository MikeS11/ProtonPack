/*
    Ghostbusters Bargraph Sequences

    This is the source code for 5 seperate bargraph sequence patterns for a ghostbusters neutrino wand
    Author Mike Simone - August 2020

    Simple Circuit:
    The pinout for the BarGraph can be found online, the supplier is BarMeter.com
    There are 7 "row" outputs and 4 "column" references (7 blocks of 4 leds in total,Reference pins A0-A6 and C0-C3 of the HT16K33 Board)
    The HT16K33 board is powered by 5V and SDA / SCL pins go directly to pins A4 and A5 of the Arduino.
    SCL ----- A5
    SDA ----- A4

    Please check out Github for any other details or other projects.
    https://github.com/MikeS11
*/

#include "BGSequence.h"
#include <HT16K33.h>
HT16K33 bargraph = HT16K33();

void BGSequence::BGSeq()
{
  /*
     Initiate the HT16K33
  */
  bargraph.init(0x70);
  delay(1000);
  bargraph.setBrightness(10);
}

/*
   Firing Routine 1 - Initiation (Reset indexes)
*/
void BGSequence::initiateVariables(uint8_t BGMODE)
{
  switch (BGMODE)
  {
    case 0:
      IndexSegment = 0;
      lastUpdateBG = 0;
      IntervalBG = 60;
      DirectionBG = FORWARD;
      break;
    case 1:
      IndexSegment = 27;
      lastUpdateBG = 0;
      IntervalBG = 20;
      DirectionBG = FORWARD;
      break;
    case 2:
      IndexSegment = 0;
      lastUpdateBG = 0;
      IntervalBG = 50;
      DirectionBG = FORWARD;
      break;
    case 3:
      IndexSegment = 0;
      lastUpdateBG = 0;
      IntervalBG = 50;
      DirectionBG = FORWARD;
      break;
    case 4:
      IndexSegment = 27;
      lastUpdateBG = 0;
      IntervalBG = 75;
      lastUpdateVENT = 0;
      IntervalVENT = 2000;
      startVent = true;
      DirectionBG = FORWARD;
      break;
  }
}

void BGSequence::changeInterval(uint8_t NewInterval)
{
  IntervalBG = NewInterval;
}

/*
   Fire 1 Sequence
   2 Segments, 2 LED's moving from the middle to the top or bottom
   E.g. Start at 13-2 move to 0, at the same time at 14-15 move to 28 and go back to 14-15
*/
void BGSequence::sequenceFire1(unsigned long currentMillis)
{
  if ((currentMillis - lastUpdateBG) > IntervalBG)
  {
    lastUpdateBG = currentMillis;
    for (int i = 0; i < 28; i++)
    {
      if (pgm_read_byte_near(BGLEDStateFire1[IndexSegment] + i) == 1)
      {
        drawBarGraph(barGraph[i][0], barGraph[i][1], 1);
      }
      if (pgm_read_byte_near(BGLEDStateFire1[IndexSegment]+ i) == 0)
      {
        if (DirectionBG == FORWARD && pgm_read_byte_near(BGLEDStateFire1[IndexSegment - 1]+ i) == 1 )
        {
          drawBarGraph(barGraph[i][0], barGraph[i][1], 0);
        }
        else if (DirectionBG == REVERSE && pgm_read_byte_near(BGLEDStateFire1[IndexSegment + 1]+ i) == 1)
        {
          drawBarGraph(barGraph[i][0], barGraph[i][1], 0);
        }
      }
    }
    if (IndexSegment >= 12)
    {
      DirectionBG = REVERSE;
    }
    else if (IndexSegment <= 0)
    {
      DirectionBG = FORWARD;
    }
    if (DirectionBG == FORWARD)
    {
      IndexSegment++;
    }
    else
    {
      IndexSegment--;
    }
    bargraph.write();
  }
}

/*
   Firing Routine 2
   3 LED's push away from the center
*/
void BGSequence::sequenceFire2(unsigned long currentMillis)
{
  if ((currentMillis - lastUpdateBG) > IntervalBG)
  {
    lastUpdateBG = currentMillis;
    for (int i = 0; i < 28; i++)
    {
      if (pgm_read_byte_near(BGLEDStateFire2[IndexSegment] + i) == 1)
      {
        if (pgm_read_byte_near(BGLEDStateFire2[IndexSegment - 1] + i) == 0 || (IndexSegment == 0 || IndexSegment == 1 || IndexSegment == 2))
        {
          drawBarGraph(barGraph[i][0], barGraph[i][1], 1);
        }
      }
      if (pgm_read_byte_near(BGLEDStateFire2[IndexSegment] + i) == 0 )
      {
        if (pgm_read_byte_near(BGLEDStateFire2[IndexSegment - 1] + i) == 1 || IndexSegment == 0)
        {
          drawBarGraph(barGraph[i][0], barGraph[i][1], 0);
        }
      }
    }
    if (IndexSegment >= 15)
    {
      IndexSegment = 0;
    }
    else
    {
      IndexSegment++;
    }
    bargraph.write();
  }
}

/*
   Starting Routine (Wand activation only)
   LED's climb then drop
*/
void BGSequence::sequenceStart(unsigned long currentMillis)
{
  if ((currentMillis - lastUpdateBG) > IntervalBG)
  {
    lastUpdateBG = currentMillis;
    if (DirectionBG == FORWARD)
    {
      for (int i = 0; i <= 27; i++)
      {
        if (i == IndexSegment )
        {
          drawBarGraph(barGraph[i][0], barGraph[i][1], 1);
        }
      }
    }
    else
    {
      for (int i = 0; i <= 27; i++)
      {
        if (i == IndexSegment )
        {
          drawBarGraph(barGraph[i + 1][0], barGraph[i + 1][1], 0);
        }
      }
    }
    if (IndexSegment == 27)
    {
      DirectionBG = REVERSE;
    }
    if (IndexSegment == 0)
    {
      DirectionBG = FORWARD;
    }
    if (DirectionBG == FORWARD)
    {
      IndexSegment++;
    }
    else
    {
      IndexSegment--;
    }

    bargraph.write();
  }
}
/*
   Pack on Routine (Wand and Pack are both on)
   All LED's turn on then quickly drop (and repeat)
*/
void BGSequence::sequencePackOn(unsigned long currentMillis)
{
  if ((currentMillis - lastUpdateBG) > IntervalBG)
  {
    lastUpdateBG = currentMillis;
    if (IndexSegment == 15)
    {
      for (int i = 0; i < 28; i++)
      {
        drawBarGraph(barGraph[i][0], barGraph[i][1], 1);
      }
      IndexSegment = 27;

    }
    for (int i = 15; i <= 27; i++)
    {
      if (i == IndexSegment )
      {
        drawBarGraph(barGraph[i][0], barGraph[i][1], 0);
      }
    }
    IndexSegment--;
    bargraph.write();
  }
}

/*
   Vent Routine
   All LED's turn on for 2.5 seconds then slowly turn off starting from the top
*/
void BGSequence::sequenceVent(unsigned long currentMillis)
{
  if (startVent == true)
  {
    lastUpdateVENT = currentMillis;
    startVent = false;

    for (int i = 0; i < 28; i++)
    {
      drawBarGraph(barGraph[i][0], barGraph[i][1], 1);
    }
    bargraph.write();
  }
  else
  {
    if ((currentMillis - lastUpdateVENT) > IntervalVENT)
    {
	
      if ((currentMillis - lastUpdateBG) > IntervalBG)
      {
        lastUpdateBG = currentMillis;
        for (int i = 0; i <= 27; i++)
        {
          if (i == IndexSegment )
          {
            drawBarGraph(barGraph[i][0], barGraph[i][1], 0);
          }
        }
        IndexSegment--;
        bargraph.write();
      }
    }
  }
}

/*
   Shutdown Routine
   start from the latest segment and turn off all the leds
*/
void BGSequence::sequenceShutdown(unsigned long currentMillis)
{
  if ((currentMillis - lastUpdateBG) > IntervalBG)
  {
    lastUpdateBG = currentMillis;
    /*
	for (int i = 0; i <= 27; i++)
    {
      if (i == IndexSegment )
      {
        drawBarGraph(barGraph[i][0], barGraph[i][1], 0);
      }
    }
*/
    drawBarGraph(barGraph[IndexSegment+1 ][0], barGraph[IndexSegment+1 ][1], 0);
    
    
    if ( IndexSegment >= -1)
    {
IndexSegment--;
      bargraph.write();
    }
  }
}

/*
   Shutdown Routine
   start from the latest segment and turn off all the leds
*/
void BGSequence::clearLEDs()
{
  for (int i = 0; i <= 27; i++)
  {
    drawBarGraph(barGraph[i][0], barGraph[i][1], 0);
  }
  bargraph.write();
}

/*
   Draw function to write changes to the bargraph
*/
void BGSequence::drawBarGraph( uint8_t row, uint8_t col, uint8_t val )
{
  bargraph.setPixel(row, col, val);
}
