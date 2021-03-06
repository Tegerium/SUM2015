#include <stdlib.h>

#include <windows.h>

#include <math.h>

#define WND_CLASS_NAME "My window class"

/* Forward references */
INT CALLBACK WinFunc( HWND hWnd, unsigned Message,
                           WPARAM wParam, LPARAM lParam );
/* Draw eye function */
VOID DrawEye( HDC hDC, INT WS, INT HS, INT WE, INT HE, INT X, INT Y );

/* Main program function */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  wc.style = CS_VREDRAW | CS_HREDRAW; /* ����� ����: ��������� ��������������
                                       * ��� ��������� ������������� ���
                                       * ��������������� �������� */
  wc.cbClsExtra = 0; /* �������������� ���������� ���� ��� ������ */
  wc.cbWndExtra = 0; /* �������������� ���������� ���� ��� ���� */
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW; /* ������� ���� - ��������� � ������� */
  wc.hCursor = LoadCursor(NULL, IDC_ARROW); /* �������� ������� (����������) */
  wc.hIcon = LoadIcon(NULL, IDI_ASTERISK); /* �������� ����������� (���������) */
  wc.lpszMenuName = NULL; /* ��� ������� ���� */
  wc.hInstance = hInstance; /* ���������� ����������, ��������������� ����� */
  wc.lpfnWndProc = WinFunc; /* ��������� �� ������� ��������� */
  wc.lpszClassName = WND_CLASS_NAME; /* ��� ������ */

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
      WS_OVERLAPPEDWINDOW,          /* ����� ���� - ���� ������ ���� */
      CW_USEDEFAULT, CW_USEDEFAULT, /* ������� ���� (x, y) - �� ��������� */
      CW_USEDEFAULT, CW_USEDEFAULT, /* ������� ���� (w, h) - �� ��������� */
      NULL,                         /* ���������� ������������� ���� */
      NULL,                         /* ���������� ������������ ���� */
      hInstance,                    /* ���������� ���������� */
      NULL);                        /* ��������� �� �������������� ��������� */

  /* �������� � ������������ ���� */
  ShowWindow(hWnd, ShowCmd);
  UpdateWindow(hWnd);

  /* ���� ��������� ���������, ���� �� ����� �������� ��������� 'WM_QUIT' */
  while (GetMessage(&msg, NULL, 0, 0))
  {
    /* ��������� ��������� �� ���������� */
    TranslateMessage(&msg);
    /* �������� ��������� � ������� ���� */
    DispatchMessage(&msg);
  }


  return msg.wParam;
} /* End of 'WinMain' function */

/* Main window message handle function */
INT CALLBACK WinFunc( HWND hWnd, unsigned Msg,
                           WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  static INT x, y, w, h;
  static HDC hMemDC;
  static HBITMAP hBm;

  switch (Msg)
  {
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    SetTimer(hWnd, 111, 100, NULL);
    return 0;

  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    if (hBm != NULL)
      DeleteObject(hBm);

    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, w, h);
    ReleaseDC(hWnd, hDC);

    SelectObject(hMemDC, hBm);
    return 0;

  case WM_MOUSEMOVE:
    x = LOWORD(lParam);
    y = HIWORD(lParam);
    return 0;

  case WM_TIMER:
    SelectObject(hMemDC, GetStockObject(NULL_PEN));
    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hMemDC, RGB(100, 200, 50));
    Rectangle(hMemDC, 0, 0, w, h);

   
    DrawEye(hMemDC, 0, 0, w / 2, h / 2, x, y);
    DrawEye(hMemDC, w / 2, 0, w, h / 2, x, y);

    InvalidateRect(hWnd, NULL, 0);
    return 0;

  /*
  case WM_ERASEBKGND:
    return 0;
  */
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);

    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);

    EndPaint(hWnd, &ps);
    return 0;

  case WM_DESTROY:
    if (hBm != NULL)
      DeleteObject(hBm);
    KillTimer(hWnd, 111);
    DeleteDC(hMemDC);
    PostQuitMessage(0);
    return 0;
  }


  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'WinFunc' function */

/* Draw eye function */
VOID DrawEye( HDC hDC, INT WS, INT HS, INT WE, INT HE, INT X, INT Y )
{
  DOUBLE c, s, l, x2, y2;
  INT rw, rh, h, w;

  SelectObject(hDC, GetStockObject(NULL_PEN));
  SelectObject(hDC, GetStockObject(DC_BRUSH));

  SetDCBrushColor(hDC, RGB(0, 255, 255));
  Ellipse(hDC, WS, HS, WE, HE);

  w = WE - WS;
  h = HE - HS;
  rw = w / 10;
  rh = h / 10;
  l = (w + h) / 20;

  x2 = (X - w / 2.0 - WS) * (X - w / 2.0 - WS);
  y2 = (Y - h / 2.0 - HS) * (Y - h / 2.0 - HS);

  c = (Y - h / 2.0 - HS) / sqrt(x2 + y2);
  s = (X - w / 2.0 - WS) / sqrt(x2 + y2);

  SetDCBrushColor(hDC, RGB(70, 140, 210));

  if (x2 + y2 < l * l)
    Ellipse(hDC, X - rw, Y - rh, X + rw, Y + rh);
  else
    Ellipse(hDC, (INT)(WS + w / 2 - rw + l * s), (INT)(HS + h / 2 - rh + l * c),
      (INT)(WS + w / 2 + rw + l * s), (INT)(HS + h / 2 + rh + l * c));
}