/***************************************************************************

    strmoper.cc - Stream operations
    ---------------------------------------------------------------------
    May, 2000
    Copyright (C) 2000 by Warlei Alves
    walves@usa.net
    
    Modified by Salvador E. Tropea to compile without warnings.
    
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
 
#include <stdio.h>

#define Uses_MsgBox
#define Uses_TFileDialog
#define Uses_TProgram
#define Uses_TApplication
#define Uses_TDeskTop
#define Uses_ofpstream
#define Uses_ifpstream

#include <tv.h>
#include "strmoper.h"

bool fileExists(const char * FileName)
{
   FILE * test;
   
   bool rst = ((test = fopen(FileName, "r")) != 0);
   if (rst) fclose(test);
   return rst;
}

char * getFileName(char * aTitle, const char * ext, int Mode)
{
   TFileDialog *d = 0;
   char * param;
   int cmd = cmCancel;
   
   if (Mode == 0)
     d = (TFileDialog *) TProgram::application->validView( new
       TFileDialog(ext, aTitle, _("File"), fdOpenButton, 100) );
   else
     d = (TFileDialog *) TProgram::application->validView( new
       TFileDialog(ext, aTitle, _("File"), fdOKButton, 100) );
   if (d != 0) cmd = TProgram::deskTop->execView( d );
   if ( cmd != cmCancel )
   {
      char fileName[PATH_MAX];
      d->getFileName( fileName );
      delete d;
      return _(fileName);
   }
   else return NULL;
}


ifpstream * openFile(const char * FileName, char * Signature)
{
   if (!fileExists(FileName)) return 0;
   ifpstream * rst = new ifpstream( FileName, ios::in|ios::binary );
   char buf[50];
   char c;
   int i;
   if (rst)
   {
      rst->readBytes(&buf, strlen(Signature) + 1);
      if (strcmp(buf, Signature) != 0)
      {
          delete rst;
          return 0;
      }
   }
   return rst;
}

ofpstream * initFile(char * FileName, char * current, char * Signature)
{
   if ( fileExists(FileName) &&
      ( current == 0 || strcmp(FileName, current) != 0 ) )
   {
      if ( messageBox(_("The file already exists. Overwrite it?"),
                    mfYesNoCancel) != cmYes ) return 0;
   }
   ofpstream * rst = new ofpstream( FileName, ios::in|ios::binary );
   if (rst) rst->writeBytes(Signature, strlen(Signature) + 1);
   return rst;
}

