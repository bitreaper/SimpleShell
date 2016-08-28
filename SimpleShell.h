/*
 * SimpleShell.h
 */

/* 
   SimpleShell - An arduino library for processing simple shell commands, and
   invoking callback functions
   Copyright (C) 2016 Bitreaper <bitreaper AT n357 DOT com>
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License (LGPL) as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU Lesser General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SIMPLESHELL_H
#define SIMPLESHELL_H

/*
 * Define more platforms here, should you want them.
 */
#ifdef ESP8266
#define SIMPLESHELL_COMMAND_SIZE 16
#define SIMPLESHELL_MAX_ARGS 9
#define SIMPLESHELL_ARG_SIZE 256
#elif linux
#define SIMPLESHELL_COMMAND_SIZE 16
#define SIMPLESHELL_MAX_ARGS 9
#define SIMPLESHELL_ARG_SIZE 256
#else
#define SIMPLESHELL_COMMAND_SIZE 16
#define SIMPLESHELL_MAX_ARGS 9
#define SIMPLESHELL_ARG_SIZE 16
#endif

/*
 * command and args from the terminal
 */
typedef struct CommandLine_t{
   char cmd[SIMPLESHELL_COMMAND_SIZE];
   int argc;
   char argv[SIMPLESHELL_MAX_ARGS][SIMPLESHELL_ARG_SIZE];
} CommandLine;

/*
 * command return status
 */
enum ExecStatus{SUCCESS,FAILED,NOTFOUND};

/*
 * command function pointer type 
 */
typedef ExecStatus (*cmd_t)(CommandLine *cmdline);

/* 
 * command specification datatype.  Use this to specify the name for a command, and the function it should call.
 */
typedef struct {
  const char            *cmd_name;
  cmd_t                  cmd_function;
} Command;

/*  You'll need to create a commands array like this:

static const ShellCommand commands[] = {
  {"hsv", cmd_hsv},
  {"rgb", cmd_rgb},
  {"type", cmd_type},
  {NULL, NULL}
};

Shell myShell( commands );

*/

class SimpleShell
{

 private:
  CommandLine cmdline;
  Command* cmdlist;
  void parseLine( char* );
  
 public:
  SimpleShell( Command* );
  ExecStatus executeCommand(char*);
};

#endif 
