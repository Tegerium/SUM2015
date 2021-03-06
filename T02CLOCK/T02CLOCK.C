 /* FILE NAME: T02CLOCK.C
 * PROGRAMMER: PK2
 * DATE: 02.06.2015
 * PURPOSE: WinAPI windowed application sample.
 */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include <windows.h>

/* ��� ������ ���� */
#define WND_CLASS_NAME "My window class"
#define PI 3.14159265
/* ������ ������ */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam );

/* ������� ������� ���������.
 *   - ���������� ���������� ����������:
 *       HINSTANCE hInstance;
 *   - ���������� ����������� ���������� ����������
 *     (�� ������������ � ������ ���� NULL):
 *       HINSTANCE hPrevInstance;
 *   - ��������� ������:
 *       CHAR *CmdLine;
 * ������������ ��������:
 *   (INT) ��� �������� � ������������ �������.
 *   0 - ��� ������.
 */

VOID HAND_Draw(HDC hDC, INT Xc, INT Yc, INT L, INT W, FLOAT Angle)
{
  INT i;
  POINT pnts[] = 
  {
    {0, W}, {-W, 0}, {0, L}, {W, 0}
  }, pntdraw[sizeof(pnts) / sizeof(pnts[0])];
  DOUBLE si = sin(Angle), co = cos(Angle);
  for(i=0; i<sizeof(pnts) / sizeof(pnts[0]); i++)
  {
    pntdraw[i].x= Xc + pnts[i].x * co - pnts[i].y * si;
    pntdraw[i].y= Yc + pnts[i].y * si + pnts[i].y * co;
  }
  Polygon(hDC, pntdraw, sizeof(pnts) / sizeof(pnts[0]));
}

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;
  /* HINSTANCE hIns = LoadLibrary("shell32"); */

  /* ����������� ������ ���� */
  wc.style = CS_VREDRAW | CS_HREDRAW; /* ����� ����: ��������� ��������������
                                       * ��� ��������� ������������� ���
                                       * ��������������� ��������
                                       * ��� ����� CS_DBLCLKS ��� ����������
                                       * ��������� �������� ������� */
  wc.cbClsExtra = 0; /* �������������� ���������� ���� ��� ������ */
  wc.cbWndExtra = 0; /* �������������� ���������� ���� ��� ���� */
  wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 0));
  wc.hCursor = LoadCursor(NULL, IDC_HAND); /* �������� ������� (����������) */
  wc.hIcon = LoadIcon(NULL, IDI_ASTERISK); /* �������� ����������� (���������) */
  wc.hInstance = hInstance; /* ���������� ����������, ��������������� ����� */
  wc.lpszMenuName = NULL; /* ��� ������� ���� */
  wc.lpfnWndProc = MyWindowFunc; /* ��������� �� ������� ��������� */
  wc.lpszClassName = WND_CLASS_NAME;

  /* ����������� ������ � ������� */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK);
    return 0;
  }

  /* �������� ���� */
  hWnd =
    CreateWindow(WND_CLASS_NAME,    /* ��� ������ ���� */
      "Title",                      /* ��������� ���� */
      WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,          /* ����� ���� - ���� ������ ���� */
      CW_USEDEFAULT, CW_USEDEFAULT, /* ������� ���� (x, y) - �� ��������� */
      CW_USEDEFAULT, CW_USEDEFAULT, /* ������� ���� (w, h) - �� ��������� */
      NULL,                         /* ���������� ������������� ���� */
      NULL,                         /* ���������� ������������ ���� */
      hInstance,                    /* ���������� ���������� */
      NULL);                        /* ��������� �� �������������� ��������� */

  ShowWindow(hWnd, ShowCmd);
  UpdateWindow(hWnd);

  /* ������ ����� ��������� ���� */
  while (GetMessage(&msg, NULL, 0, 0))
  {
    /* ��������� ��������� �� ���������� */
    TranslateMessage(&msg);
    /* �������� ��������� � ������� ���� */
    DispatchMessage(&msg);
  }

  return msg.wParam;
} /* End of 'WinMain' function */

#define sqr(X) ((X) * (X))

/* ��������� ����.
 * ���������:
 *   - ���������� ��������� ���������:
 *       HDC hDC;
 *   - ������� ����:
 *       INT W, H;
 *   - ������� ����:
 *       INT Xc, Yc;
 * ������������ ��������: ���.
 */

/* ������� ��������� ��������� ����.
 * ���������:
 *   - ���������� ����:
 *       HWND hWnd;
 *   - ����� ��������� (��. WM_***):
 *       UINT Msg;
 *   - �������� ��������� ('word parameter'):
 *       WPARAM wParam;
 *   - �������� ��������� ('long parameter'):
 *       LPARAM lParam;
 * ������������ ��������:
 *   (LRESULT) - � ����������� �� ���������.
 */

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  INT x, y, i;
  CREATESTRUCT *cs;
  POINT pt;
  //PAINTSTRUCT ps;
  SYSTEMTIME st;
  CHAR Buf[100];
  HFONT hFnt, hOldFnt;
  RECT rc;
  static INT xx[100], yy[100];
  static BITMAP bm;
  static HBITMAP hBm, hBmLogo;
  static HDC hMemDC, hMemDCLogo;
  static INT w, h;

  switch (Msg)
  {
  case WM_CREATE:
    cs = (CREATESTRUCT *)lParam;
    SetTimer(hWnd, 111, 50, NULL);

    for (i = 0; i < 100; i++)
      xx[i] = rand() % 1000, yy[i] = rand() % 768;

    hBmLogo = LoadImage(NULL, "Chasy.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hBmLogo, sizeof(bm), &bm);

    /* ������� �������� � ������ */
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    hMemDCLogo = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);

    SelectObject(hMemDCLogo, hBmLogo);
    return 0;

  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);

    /* ������� �������� �������� � ���� */
    if (hBm != NULL)
      DeleteObject(hBm);

    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, w, h);
    ReleaseDC(hWnd, hDC);

    SelectObject(hMemDC, hBm);
    SendMessage(hWnd, WM_TIMER, 111, 0);
    return 0;

  case WM_TIMER:
    /* Clear Background */
    SelectObject(hMemDC, GetStockObject(NULL_PEN));
    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hMemDC, RGB(0, 50, 200));
    Rectangle(hMemDC, 0, 0, w + 1, h + 1);

    StretchBlt(hMemDC, 0, 0, w, h,
      hMemDCLogo, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

    ScreenToClient(hWnd, &pt);  

    GetLocalTime(&st);
    HAND_Draw(hMemDC, w/2, h/2, 300, 30, (90 + st.wSecond) * 6 * PI /180); 
    hFnt = CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE,
      FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS,
      CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
      VARIABLE_PITCH | FF_ROMAN, "");
    hOldFnt = SelectObject(hMemDC, hFnt);
    SetTextColor(hMemDC, RGB(0, 0, 255));
    SetBkColor(hMemDC, RGB(255, 255, 0));
    SetBkMode(hMemDC, TRANSPARENT);
    TextOut(hMemDC, 50, 50, Buf,
      sprintf(Buf, "%02d:%02d:%02d (%02d.%02d.%d)",
        st.wHour, st.wMinute, st.wSecond,
        st.wDay, st.wMonth, st.wYear));
    
    DeleteObject(hFnt);

    hFnt = CreateFont(30, 0, 0, 0, FW_BOLD, TRUE, FALSE,
      TRUE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS,
      CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
      VARIABLE_PITCH | FF_SWISS, "");
    SelectObject(hMemDC, hFnt); 

    rc.left = 5;
    rc.top = 5;
    rc.right = w - 5;
    rc.bottom = h - 5;

    SelectObject(hMemDC, hOldFnt);
    DeleteObject(hFnt);

    SelectObject(hMemDC, GetStockObject(DC_PEN));
    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    srand(30);

    InvalidateRect(hWnd, NULL, TRUE);
    return 0;

  case WM_CLOSE:
    if (MessageBox(hWnd, "Are you shure to exit from program?",
          "Exit", MB_YESNO | MB_ICONQUESTION) == IDNO)
      return 0;
    break;

  case WM_LBUTTONDOWN:
    SetCapture(hWnd);

    x = LOWORD(lParam);
    y = HIWORD(lParam);
    
    return 0;

  case WM_LBUTTONUP:
    ReleaseCapture();
    return 0;

  case WM_MOUSEMOVE:
    x = (SHORT)LOWORD(lParam);
    y = (SHORT)HIWORD(lParam);
    if (wParam & MK_LBUTTON)
    {
      Ellipse(hMemDC, x - 5, y - 5, x + 5, y + 5);
    }
    SelectObject(hMemDC, GetStockObject(DC_PEN));
    
    return 0;

  case WM_ERASEBKGND:
    BitBlt((HDC)wParam, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    return 0;

  case WM_DESTROY:
    DeleteDC(hMemDC);
    DeleteObject(hBm);
    DeleteDC(hMemDCLogo);
    DeleteObject(hBmLogo);
    KillTimer(hWnd, 111);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* END OF 'T02DBLB.C' FILE */