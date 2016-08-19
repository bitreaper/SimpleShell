/*
 * SimpleShell.cpp
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

#include "SimpleShell.h"

#ifdef linux
#include <cstring>
#else
#include <stdlib.h>
#include <string.h>
#include <Arduino.h>
#endif

/*******************************************************************************
 *  constructor
 *******************************************************************************/

SimpleShell::SimpleShell( Command* cmdlist )
{
   this->cmdlist = cmdlist;
}

/*******************************************************************************
 * parseLine() tears apart the string, and pieces get put into the command structure.
 *******************************************************************************/
void SimpleShell::parseLine( char* processMe )
{

  int argIdx = 0;
  CommandLine* cmdLinePtr = &this->cmdline; // for convenience give it a local name
  char* saveplace = NULL;
  char* token = NULL;

  // parse out the command itself, 
  token = strtok_r( processMe, " ", &saveplace );

  if( token == NULL )
  {
     // in the case of a blank line or line of spaces, we have to deal with the NULL pointer coming back.
     cmdLinePtr->cmd[0] = 0;
     return;
  }
  strncpy( cmdLinePtr->cmd, token, SIMPLESHELL_COMMAND_SIZE -1 );
  cmdLinePtr->cmd[SIMPLESHELL_COMMAND_SIZE] = 0;  // ensure null termination since strncpy doesn't guarantee it.

  // now parse out it's args.
  token = strtok_r( NULL, " ", &saveplace );
  while( (token != NULL) && (argIdx < SIMPLESHELL_MAX_ARGS) )        // if token is null, then we're done.
  {
     strncpy( cmdLinePtr->argv[argIdx], token, SIMPLESHELL_ARG_SIZE - 1 );
     cmdLinePtr->argv[argIdx][SIMPLESHELL_ARG_SIZE] = 0; // ensure null termination.
     token = strtok_r( NULL, " ", &saveplace );
     argIdx++;  // unconditional bumping, by the time we get to end of string it'll be the right value for argc.
  }
  cmdLinePtr->argc = argIdx;
}

/*******************************************************************************
 * loop over the command list, check for a match.  if found, then execute the
 * match's cmd_function callback.
 *******************************************************************************/
ExecStatus SimpleShell::executeCommand( char* input )
{

   this->parseLine( input );

   CommandLine* cmdLinePtr;
   Command* cmdPtr = this->cmdlist;

   while(cmdPtr->cmd_name != NULL)
   {
      if (strcasecmp(cmdPtr->cmd_name, this->cmdline.cmd) == 0)
      {
         // calling the function pointer in cmdPtr->cmd_function, and handing it the address of
         // the cmdline member variable.
         return (cmdPtr->cmd_function)(&(this->cmdline)); 
      }
      cmdPtr++;
   }
   // cmd not found, need to return something
   return NOTFOUND;
}
