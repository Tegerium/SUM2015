/* FILENAME: ANIM.C
 * PROGRAMMER: PK2
 * PURPOSE: Animation system module.
 * LAST UPDATE: 08.06.2015
 */

#include "anim.h"

/* Системный контекст анимации */
static PK2ANIM PK2_Anim;

static INT64
  TimeFreq,  /* единиц измерения в секунду */
  TimeStart, /* время начала анимации */
  TimeOld,   /* время прошлого кадра */
  TimePause, /* время простоя в паузе */
  TimeFPS;   /* время для замера FPS */
static INT
  FrameCounter; /* счетчик кадров */
/* Функция инициализации анимации.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PK2_AnimInit( HWND hWnd )
{
  HDC hDC = GetDC(hWnd);
  LARGE_INTEGER li;

  PK2_Anim.hWnd = hWnd;
  /* Инициализируем буфер кадра */
  PK2_Anim.hDC = CreateCompatibleDC(hDC);
  PK2_Anim.hBmFrame = CreateCompatibleBitmap(hDC, 30, 30);
  SelectObject(PK2_Anim.hDC, PK2_Anim.hBmFrame);
  PK2_Anim.W = 30;
  PK2_Anim.H = 30;
  PK2_Anim.NumOfUnits = 0;
  ReleaseDC(hWnd, hDC);

  /* Инициализация таймера */
  QueryPerformanceFrequency(&li);
  QueryPerformanceCounter(&li);
} /* End of 'PK2_AnimInit' function */

/* Функция деинициализации анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PK2_AnimClose( VOID )
{
  INT i;

  /* Освобождение памяти из-под объектов анимации */
  for (i = 0; i < PK2_Anim.NumOfUnits; i++)
  {
    PK2_Anim.Units[i]->Close(PK2_Anim.Units[i], &PK2_Anim);
    free(PK2_Anim.Units[i]);
    PK2_Anim.Units[i] = NULL;
  }

  /* Удаляем объекты GDI */
  DeleteObject(PK2_Anim.hBmFrame);
  DeleteDC(PK2_Anim.hDC);
} /* End of 'PK2_AnimClose' function */

/* Функция обработки изменения размера области вывода.
 * АРГУМЕНТЫ:
 *   - новый размер области вывода:
 *       INT W, H;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PK2_AnimResize( INT W, INT H )
{
  HDC hDC = GetDC(PK2_Anim.hWnd);

  /* Перевыделение памяти у буфера кадра */
  DeleteObject(PK2_Anim.hBmFrame);
  PK2_Anim.hBmFrame = CreateCompatibleBitmap(hDC, W, H);
  SelectObject(PK2_Anim.hDC, PK2_Anim.hBmFrame);

  /* Сохранение размера */
  PK2_Anim.W = W;
  PK2_Anim.H = H;

  ReleaseDC(PK2_Anim.hWnd, hDC);
} /* End of 'PK2_AnimResize' function */

/* Функция построения кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PK2_AnimRender( VOID )
{
  INT i;

  /* опрос на изменение состояний объектов */
  for (i = 0; i < PK2_Anim.NumOfUnits; i++)
    PK2_Anim.Units[i]->Response(PK2_Anim.Units[i], &PK2_Anim);

  /* очистка фона */
  SelectObject(PK2_Anim.hDC, GetStockObject(DC_BRUSH));
  SelectObject(PK2_Anim.hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(PK2_Anim.hDC, RGB(0, 0, 0));
  Rectangle(PK2_Anim.hDC, 0, 0, PK2_Anim.W + 1, PK2_Anim.H + 1);

  /* рисование объектов */
  for (i = 0; i < PK2_Anim.NumOfUnits; i++)
  {
    SelectObject(PK2_Anim.hDC, GetStockObject(DC_BRUSH));
    SelectObject(PK2_Anim.hDC, GetStockObject(DC_PEN));
    SetDCBrushColor(PK2_Anim.hDC, RGB(255, 255, 255));
    SetDCPenColor(PK2_Anim.hDC, RGB(0, 0, 0));

    PK2_Anim.Units[i]->Render(PK2_Anim.Units[i], &PK2_Anim);
  }
} /* End of 'PK2_AnimRender' function */

/* Функция вывода кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PK2_AnimCopyFrame( VOID )
{
  HDC hDC;

  /* вывод кадра */
  hDC = GetDC(PK2_Anim.hWnd);
  BitBlt(hDC, 0, 0, PK2_Anim.W, PK2_Anim.H, PK2_Anim.hDC, 0, 0, SRCCOPY);
  ReleaseDC(PK2_Anim.hWnd, hDC);
} /* End of 'PK2_AnimCopyFrame' function */

/* Функция добавления в систему объекта анимации.
 * АРГУМЕНТЫ:
 *   - добавляемый объект анимации:
 *       PK2UNIT *Unit;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PK2_AnimAddUnit( PK2UNIT *Unit )
{
  if (PK2_Anim.NumOfUnits < PK2_MAX_UNITS)
  {
    PK2_Anim.Units[PK2_Anim.NumOfUnits++] = Unit;
    Unit->Init(Unit, &PK2_Anim);
  }
} /* End of 'PK2_AnimAddUnit' function */


/* Функция переключения в/из полноэкранного режима
 * с учетом нескольких мониторов.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PK2_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE; /* текущий режим */
  static RECT SaveRC;               /* сохраненный размер */

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    /* сохраняем старый размер окна */
    GetWindowRect(PK2_Anim.hWnd, &SaveRC);

    /* определяем в каком мониторе находится окно */
    hmon = MonitorFromWindow(PK2_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    /* получаем информацию для монитора */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* переходим в полный экран */
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
    /* восстанавливаем размер окна */
    SetWindowPos(PK2_Anim.hWnd, HWND_TOP,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
} /* End of 'PK2_AnimFlipFullScreen' function */

/* Функция выхода из анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PK2_AnimDoExit( VOID )
{
  PostMessage(PK2_Anim.hWnd, WM_CLOSE, 0, 0);
} /* End of 'PK2_AnimDoExit' function */

/* END OF 'ANIM.C' FILE */
