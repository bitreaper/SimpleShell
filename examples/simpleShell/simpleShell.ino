/*
 * simpleShell.ino
 */

/* 
   SimpleShell - An arduino library for processing simple shell commands, and
   invoking callback functions
   Copyright (C) 2016 Bitreaper <bitreaper AT n357 DOT com>
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "SimpleShell.h"

/* 
 * functions to use for the shell
 */
ExecStatus foo( CommandLine* clp )
{
     Serial.println("Foo called");
     return SUCCESS;
}
ExecStatus bar( CommandLine* clp )
{
     Serial.println("Bar called");
     return SUCCESS;
}
ExecStatus failme( CommandLine* clp )
{
     Serial.println("failme called");
     return FAILED;
}

/*
 * shell command table
 */
Command cmdlist[] = {
   {"foo",foo},
   {"bar",bar},
   {"failme",failme},
   {NULL,NULL}
};

/* 
 * an instance of the shell
 */
SimpleShell shell(cmdlist);

/* 
 * standard arduino setup/loop pair
 */
void setup( void )
{
   Serial.begin(115200);
   Serial.println("Setup complete");
}

void loop( void )
{
   char input[256];
   memset(input, 0, 256);
   
   if( Serial.available() > 0 )
   {
      Serial.readBytesUntil('\r',input, 256);
      // do whatever just came in
      Serial.println("Searching for command");
      switch(shell.executeCommand( input ))
      {
         case SUCCESS:
            break;
         case FAILED:
            Serial.println("Command failed!");
            break;
         case NOTFOUND:
            Serial.println("Command not found!");
            break;
      }
   }
}

