#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>

int computerTurn()
{
  return rand() % 5;
}
  
void allOff(int leds[])
{
  int LED;
  for (LED=0; LED<5; LED++)
    {
      gpioWrite(leds[LED], 0);
    }
  return;  
}

void playPattern(int leds[], int numTurns)
{
  int LED;
  for (LED=0; LED<numTurns; LED++)
    {
      gpioWrite(leds[LED], 1);
      time_sleep(0.5);
      gpioWrite(leds[LED], 0);
      time_sleep(0.5);
    }
  return;
}

void allFlash(int leds[])
{
  int LED, i;
  for (i=0; i<3; i++)
    {
      for (LED=0; LED<5; LED++)
	{
	  gpioWrite(leds[LED], 1);
	}
      time_sleep(0.2);
      for (LED=0; LED<5; LED++)
	{
	  gpioWrite(leds[LED], 0);
	}
      time_sleep(0.20);
    }
}

void youWin(int leds[])
{
  int LED, i;
  for (i=0; i<3; i++)
    {
      for (LED=0; LED<5; LED++)
	{
	  gpioWrite(leds[LED], 1);
	  time_sleep(0.1);
	  gpioWrite(leds[LED], 0);
	  time_sleep(0.1);
	}
    }
}

int getPlayerTurn()
{
  while(1)
    {
      if (gpioRead(13) == 0) return 13;
      else if (gpioRead(19) == 0) return 19;
      else if (gpioRead(26) == 0) return 26;
      else if (gpioRead(20) == 0) return 20;
      else if (gpioRead(21) == 0) return 21;
    }
}


int main()
{
  if (gpioInitialise() < 0)
    {
      printf("pigpio initialization failed\n");
      return 1;
    }

  // Initialize random number generator
  time_t t;
  srand((unsigned) time(&t));

  // Declare buttons
  int butOne = 13;
  int butTwo = 19;
  int butThr = 26;
  int butFor = 20;
  int butFiv = 21;

  // Declare LED pins
  int ledOne = 14;
  int ledTwo = 15;
  int ledThr = 18;
  int ledFor = 23;
  int ledFiv = 24;

  // int buttonArray[5] = {butOne, butTwo, butThr, butFor, butFiv};
  int ledArray[5] = {ledOne, ledTwo, ledThr, ledFor, ledFiv};
  
  // Declare computer array
  // and turn counter
  int simon[50];
  int turns;

  // counter variable
  int i, x, compareSteps;
  
  // Set up buttons
  gpioSetMode(butOne, PI_INPUT);
  gpioSetMode(butTwo, PI_INPUT);
  gpioSetMode(butThr, PI_INPUT);
  gpioSetMode(butFor, PI_INPUT);
  gpioSetMode(butFiv, PI_INPUT);
  
  gpioSetPullUpDown(butOne, PI_PUD_UP);
  gpioSetPullUpDown(butTwo, PI_PUD_UP);
  gpioSetPullUpDown(butThr, PI_PUD_UP);
  gpioSetPullUpDown(butFor, PI_PUD_UP);
  gpioSetPullUpDown(butFiv, PI_PUD_UP);

  // Set up LEDs
  gpioSetMode(ledOne, PI_OUTPUT);
  gpioSetMode(ledTwo, PI_OUTPUT);
  gpioSetMode(ledThr, PI_OUTPUT);
  gpioSetMode(ledFor, PI_OUTPUT);
  gpioSetMode(ledFiv, PI_OUTPUT);

  // Turn off all LEDs to start
  allOff(ledArray);
  
  for (turns=0; turns<50; turns++)
    {
      // Computer adds random number
      x = computerTurn();
      switch (x)
	{
	case 0:
	  simon[turns] = ledOne;
	  break;
	case 1:
	  simon[turns] = ledTwo;
	  break;
	case 2:
	  simon[turns] = ledThr;
	  break;
	case 3:
	  simon[turns] = ledFor;
	  break;
	case 4:
	  simon[turns] = ledFiv;
	  break;
	}
      // Computer plays pattern
      playPattern(simon, turns);
      time_sleep(1);

      // simon[] now contains turns-long list of lit LEDs
      for (compareSteps=0; compareSteps<turns; compareSteps++)
	{
	  i = getPlayerTurn();
	  switch(i)
	    {
	    case 13:
	      gpioWrite(ledOne, 1);
	      time_sleep(0.5);
	      gpioWrite(ledOne, 0);
	      if(simon[compareSteps] != ledOne)
		{
		  allFlash(ledArray);
		  printf("You lose\n");
		  return 0;
		}
	      break;
	    case 19:
	      gpioWrite(ledTwo, 1);
	      time_sleep(0.5);
	      gpioWrite(ledTwo, 0);
	      if(simon[compareSteps] != ledTwo)
		{
		  allFlash(ledArray);
		  printf("You lose\n");
		  return 0;
		}
	      break;
	    case 26:
	      gpioWrite(ledThr, 1);
	      time_sleep(0.5);
	      gpioWrite(ledThr, 0);
	      if(simon[compareSteps] != ledThr)
		{
		  allFlash(ledArray);
		  printf("You lose\n");
		  return 0;
		}
	      break;
	    case 20:
	      gpioWrite(ledFor, 1);
	      time_sleep(0.5);
	      gpioWrite(ledFor, 0);
	      if(simon[compareSteps] != ledFor)
		{
		  allFlash(ledArray);
		  printf("You lose\n");
		  return 0;
		}
	      break;
	    case 21:
	      gpioWrite(ledFiv, 1);
	      time_sleep(0.5);
	      gpioWrite(ledFiv, 0);
	      if(simon[compareSteps] != ledFiv)
		{
		  allFlash(ledArray);
		  printf("You lose\n");
		  return 0;
		}
	      break;
	    }
	}
      time_sleep(1);
    }
  // If you've reached this far, you've won
  printf("You won!\n");
  youWin(ledArray);
  
  gpioTerminate();
} // end main
  
  
