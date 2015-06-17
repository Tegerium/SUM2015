/* FILENAME: ANIM.C
 * PROGRAMMER: PK2
 * PURPOSE: Animation system module.
 * LAST UPDATE: 15.06.2015
 */

#include "anim.h"

#include <mmsystem.h>
#pragma comment(lib, "winmm")

#define PK2_GET_AXIS_VALUE(Axis) \
  (2.0 * (ji.dw ## Axis ## pos - jc.w ## Axis ## min) / (jc.w ## Axis ## max - jc.w ## Axis ## min) - 1.0)

PK2ANIM PK2_Anim;

static INT64
  TimeFreq, 
  TimeStart, 
  TimeOld,   
  TimePause, 
  TimeFPS;  
static INT
  FrameCounter;

static INT
  PK2_MouseOldX, PK2_MouseOldY;

BOOL PK2_AnimInit( HWND hWnd )
{
  INT i;
  LARGE_INTEGER li;
  POINT pt;
  PIXELFORMATDESCRIPTOR pfd = {0};

  memset(&PK2_Anim, 0, sizeof(PK2ANIM));
  PK2_Anim.hWnd = hWnd;
  PK2_Anim.hDC = GetDC(hWnd);

  PK2_Anim.W = 30;
  PK2_Anim.H = 30;
  PK2_Anim.NumOfUnits = 0;

  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(PK2_Anim.hDC, &pfd);
  DescribePixelFormat(PK2_Anim.hDC, i, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
  SetPixelFormat(PK2_Anim.hDC, i, &pfd);

  PK2_Anim.hGLRC = wglCreateContext(PK2_Anim.hDC);
  wglMakeCurrent(PK2_Anim.hDC, PK2_Anim.hGLRC);

  if (glewInit() != GLEW_OK ||
      !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(PK2_Anim.hGLRC);
    ReleaseDC(PK2_Anim.hWnd, PK2_Anim.hDC);
    memset(&PK2_Anim, 0, sizeof(PK2ANIM));
    return FALSE;
  }

  PK2_RndProg = PK2_ShaderLoad("TEST");

  glActiveTexture(GL_TEXTURE0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glActiveTexture(GL_TEXTURE1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glActiveTexture(GL_TEXTURE0);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;
  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  PK2_Anim.IsPause = FALSE;
  FrameCounter = 0;

  GetCursorPos(&pt);
  ScreenToClient(PK2_Anim.hWnd, &pt);
  PK2_MouseOldX = pt.x;
  PK2_MouseOldY = pt.y;
  GetKeyboardState(PK2_Anim.KeysOld);
  return TRUE;
} /* End of 'PK2_AnimInit' function */

VOID PK2_AnimClose( VOID )
{
  INT i;

  for (i = 0; i < PK2_Anim.NumOfUnits; i++)
  {
    PK2_Anim.Units[i]->Close(PK2_Anim.Units[i], &PK2_Anim);
    free(PK2_Anim.Units[i]);
    PK2_Anim.Units[i] = NULL;
  }

  PK2_ShaderFree(PK2_RndProg);
  PK2_RndProg = 0;
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(PK2_Anim.hGLRC);
  ReleaseDC(PK2_Anim.hWnd, PK2_Anim.hDC);
} /* End of 'PK2_AnimClose' function */

VOID PK2_AnimResize( INT W, INT H )
{
  PK2_Anim.W = W;
  PK2_Anim.H = H;

  glViewport(0, 0, W, H);

  if (W > H)
    PK2_RndWp = (DBL)W / H * 3, PK2_RndHp = 3;
  else
    PK2_RndHp = (DBL)H / W * 3, PK2_RndWp = 3;

  PK2_RndMatrProj = MatrFrustum(-PK2_RndWp / 2, PK2_RndWp / 2,
                                -PK2_RndHp / 2, PK2_RndHp / 2,
                                PK2_RndProjDist, 800);

} /* End of 'PK2_AnimResize' function */

VOID PK2_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER li;
  POINT pt;

  QueryPerformanceCounter(&li);
  PK2_Anim.GlobalTime = (DBL)(li.QuadPart - TimeStart) / TimeFreq;
  PK2_Anim.GlobalDeltaTime = (DBL)(li.QuadPart - TimeOld) / TimeFreq;

  if (!PK2_Anim.IsPause)
    PK2_Anim.DeltaTime = PK2_Anim.GlobalDeltaTime;
  else
  {
    TimePause += li.QuadPart - TimeOld;
    PK2_Anim.DeltaTime = 0;
  }

  PK2_Anim.Time = (DBL)(li.QuadPart - TimePause - TimeStart) / TimeFreq;

  if (li.QuadPart - TimeFPS > TimeFreq)
  {
    PK2_Anim.FPS = FrameCounter / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
    FrameCounter = 0;
  }

  TimeOld = li.QuadPart;

  GetKeyboardState(PK2_Anim.Keys);
  for (i = 0; i < 256; i++)
    PK2_Anim.Keys[i] >>= 7;
  for (i = 0; i < 256; i++)
    PK2_Anim.KeysClick[i] = PK2_Anim.Keys[i] && !PK2_Anim.KeysOld[i];
  memcpy(PK2_Anim.KeysOld, PK2_Anim.Keys, sizeof(PK2_Anim.KeysOld));


  PK2_Anim.MsWheel = PK2_MouseWheel;
  PK2_MouseWheel = 0;

  GetCursorPos(&pt);
  ScreenToClient(PK2_Anim.hWnd, &pt);
  PK2_Anim.MsX = pt.x;
  PK2_Anim.MsY = pt.y;

  PK2_Anim.MsDeltaX = pt.x - PK2_MouseOldX;
  PK2_Anim.MsDeltaY = pt.y - PK2_MouseOldY;
  PK2_MouseOldX = pt.x;
  PK2_MouseOldY = pt.y;


  if ((i = joyGetNumDevs()) > 0)
  {
    JOYCAPS jc;


    if (joyGetDevCaps(JOYSTICKID2, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      ji.dwSize = sizeof(JOYCAPS);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID2, &ji) == JOYERR_NOERROR)
      {
        memcpy(PK2_Anim.JButsOld, PK2_Anim.JButs, sizeof(PK2_Anim.JButs));
        for (i = 0; i < 32; i++)
          PK2_Anim.JButs[i] = (ji.dwButtons >> i) & 1;
        for (i = 0; i < 32; i++)
          PK2_Anim.JButsClick[i] = PK2_Anim.JButs[i] && !PK2_Anim.JButsOld[i];

        PK2_Anim.JX = PK2_GET_AXIS_VALUE(X);
        PK2_Anim.JY = PK2_GET_AXIS_VALUE(Y);
        if (jc.wCaps & JOYCAPS_HASZ)
          PK2_Anim.JZ = PK2_GET_AXIS_VALUE(Z);
        if (jc.wCaps & JOYCAPS_HASU)
          PK2_Anim.JU = PK2_GET_AXIS_VALUE(U);
        if (jc.wCaps & JOYCAPS_HASV)
          PK2_Anim.JV = PK2_GET_AXIS_VALUE(V);
        if (jc.wCaps & JOYCAPS_HASR)
          PK2_Anim.JR = PK2_GET_AXIS_VALUE(R);

        if (jc.wCaps & JOYCAPS_HASPOV)
        {
          if (ji.dwPOV == 0xFFFF)
            PK2_Anim.JPOV = 0;
          else
            PK2_Anim.JPOV = ji.dwPOV / 4500 + 1;
        }
      }
    }
  }

  for (i = 0; i < PK2_Anim.NumOfUnits; i++)
    PK2_Anim.Units[i]->Response(PK2_Anim.Units[i], &PK2_Anim);


  glClearColor(0.3, 0.5, 0.7, 1);
  glClearDepth(1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColorMask(TRUE, TRUE, TRUE, TRUE);


  /*
  glColor3d(0.3, 0.5, 0.7);
  glRectd(-2, -2, 2, 2);
  glColor3d(1, 0, 0);
  glRectd(0, 0, 0.88, 0.30);
  */


  for (i = 0; i < PK2_Anim.NumOfUnits; i++)
  {
    static DBL time = 5;

    time += PK2_Anim.GlobalDeltaTime;
    if (time > 5)
    {
      time = 0;
      PK2_ShaderFree(PK2_RndProg);
      PK2_RndProg = PK2_ShaderLoad("TEST");
    }

    PK2_RndMatrWorld = MatrIdentity();

    PK2_Anim.Units[i]->Render(PK2_Anim.Units[i], &PK2_Anim);
  }
  glFinish();
  FrameCounter++;
} /* End of 'PK2_AnimRender' function */

VOID PK2_AnimCopyFrame( VOID )
{
  SwapBuffers(PK2_Anim.hDC);
} /* End of 'PK2_AnimCopyFrame' function */


VOID PK2_AnimAddUnit( PK2UNIT *Unit )
{
  if (PK2_Anim.NumOfUnits < PK2_MAX_UNITS)
  {
    PK2_Anim.Units[PK2_Anim.NumOfUnits++] = Unit;
    Unit->Init(Unit, &PK2_Anim);
  }
} /* End of 'PK2_AnimAddUnit' function */


VOID PK2_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE; 
  static RECT SaveRC;              

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

 
    GetWindowRect(PK2_Anim.hWnd, &SaveRC);


    hmon = MonitorFromWindow(PK2_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

  
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    rc = moninfo.rcMonitor;

    AdjustWindowRect(&rc, GetWindowLong(PK2_Anim.hWnd, GWL_STYLE), FALSE);

    SetWindowPos(PK2_Anim.hWnd, HWND_TOPMOST,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else
  {
    SetWindowPos(PK2_Anim.hWnd, HWND_TOP,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
} /* End of 'PK2_AnimFlipFullScreen' function */

VOID PK2_AnimDoExit( VOID )
{
  PostMessage(PK2_Anim.hWnd, WM_CLOSE, 0, 0);
} /* End of 'PK2_AnimDoExit' function */

VOID PK2_AnimSetPause( BOOL NewPauseFlag )
{
  PK2_Anim.IsPause = NewPauseFlag;
} /* End of 'PK2_AnimSetPause' function */

/* END OF 'ANIM.C' FILE */
