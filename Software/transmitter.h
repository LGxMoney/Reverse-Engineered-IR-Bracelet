
#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include "Arduino.h"

class transmitterStructure
{

   public: 
   
   struct
   {
      int numBytes;
      long hexCode;
      String protocol;
    } signalSpecs;

    
    {
      int screenID; 
      String title;
      String onSendDisplay; 
      signalSpecs sendCode;
    } screen;
};

#endif