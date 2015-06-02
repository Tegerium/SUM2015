#include <stdio.h>
#include <Windows.h>

void main( void )
{
  if(MessageBox( NULL, "Text", "Cap", MB_YESNO | MB_ICONQUESTION) == IDYES)
    MessageBox( NULL, "You pressed YES", "CAP", MB_OK);
  else
    MessageBox( NULL, "You pressed NO", "CAP", MB_OK);  
}