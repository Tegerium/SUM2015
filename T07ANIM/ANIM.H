/* FILENAME: ANIM.H
 * PROGRAMMER: PK2
 * PURPOSE: Animation system declaration module.
 * LAST UPDATE: 11.06.2015
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "render.h"

/* Максимальное количество элементов анимации */
#define PK2_MAX_UNITS 3000

/* Объявление типа объектов анимации "вперед" */
typedef struct tagPK2UNIT PK2UNIT;

/* Структура хранения контекста анимации */
typedef struct
{
  HWND hWnd;          /* Окно вывода */
  INT W, H;           /* Размер окна вывода */
  HDC hDC;            /* Контекст окна вывода */
  HGLRC hGLRC;        /* Контекст рендеринга */

  /* Массив элементов анимации и их количество */
  PK2UNIT *Units[PK2_MAX_UNITS]; 
  INT NumOfUnits;

  /* Подсистема синхронизации */
  DBL
    Time,            /* время в секундах со старта анимации */
    GlobalTime,      /* время -"-, но без паузы */
    DeltaTime,       /* межкадровое время в секундах */
    GlobalDeltaTime, /* межкадровое время в секундах без паузы */
    FPS;             /* количество кадров в секунду */
  BOOL
    IsPause;         /* флаг паузы */

  /* Подсистема ввода */
  BYTE
    Keys[256],       /* Сотояние клавиш клавиатуры и мыши */
    KeysOld[256],    /* Сотояние клавиш на предыдущем кадре */
    KeysClick[256];  /* Флаги однократного нажатия */
  INT
    MsDeltaX, MsDeltaY, /* Относительные координаты курсора мыши */
    MsX, MsY,        /* Абсолютные координаты курсора мыши */
    MsWheel;         /* Состояние колеса мыши */
  BYTE
    JButs[32],       /* Сотояние кнопок джойстика */
    JButsOld[32],    /* Сотояние кнопок джойстика на предыдущем кадре */
    JButsClick[32],  /* Флаги однократного нажатия кнопок джойстика */
    JPOV;            /* Переключатель POV - 0..8 */
  DBL
    JX, JY, JZ, JR, JU, JV; /* Оси джойстика (-1.0 .. 1.0*/
} PK2ANIM;

/* Системный контекст анимации */
extern PK2ANIM PK2_Anim;

/* Базовые поля объекта анимации:
 * - размер структуры для наследования
 *     INT Size;
 * - идентификатор объеткта:
 *     INT Id;
 * - указатель на функцию инициализации
 *     VOID (*Init)( PK2UNIT *Unit, PK2ANIM *Ani );
 * - указатель на функцию деинициализации
 *     VOID (*Close)( PK2UNIT *Unit, PK2ANIM *Ani );
 * - указатель на функцию обновления межкадровых параметров
 *     VOID (*Response)( PK2UNIT *Unit, PK2ANIM *Ani );
 * - указатель на функцию построения
 *     VOID (*Render)( PK2UNIT *Unit, PK2ANIM *Ani );
 */
#define PK2_UNIT_BASE_FIELDS \
  INT Size;                                          \
  VOID (*Init)( PK2UNIT *Unit, PK2ANIM *Ani );       \
  VOID (*Close)( PK2UNIT *Unit, PK2ANIM *Ani );      \
  VOID (*Response)( PK2UNIT *Unit, PK2ANIM *Ani );   \
  VOID (*Render)( PK2UNIT *Unit, PK2ANIM *Ani )

/* Базовый тип объекта анимации */
struct tagPK2UNIT
{
  PK2_UNIT_BASE_FIELDS;
};

/* Глобальная переменная - счетчик прокрутки колеса мыши */
extern INT PK2_MouseWheel;

/* Функция инициализации анимации.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, иначе FALSE.
 */
BOOL PK2_AnimInit( HWND hWnd );

/* Функция деинициализации анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PK2_AnimClose( VOID );

/* Функция обработки изменения размера области вывода.
 * АРГУМЕНТЫ:
 *   - новый размер области вывода:
 *       INT W, H;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PK2_AnimResize( INT W, INT H );

/* Функция построения кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PK2_AnimRender( VOID );

/* Функция вывода кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PK2_AnimCopyFrame( VOID );

/* Функция добавления в систему объекта анимации.
 * АРГУМЕНТЫ:
 *   - добавляемый объект анимации:
 *       PK2UNIT *Unit;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PK2_AnimAddUnit( PK2UNIT *Unit );

/* Функция переключения в/из полноэкранного режима
 * с учетом нескольких мониторов.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PK2_AnimFlipFullScreen( VOID );

/* Функция создания объекта анимации.
 * АРГУМЕНТЫ:
 *   - размер структуры объекта анимации:
 *       INT Size;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (PK2UNIT *) указатель на созданный объект анимации.
 */
PK2UNIT * PK2_AnimUnitCreate( INT Size );

/* Функция выхода из анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PK2_AnimDoExit( VOID );

/* Функция установки паузы анимации.
 * АРГУМЕНТЫ:
 *   - флаг паузы:
 *       BOOL NewPauseFlag;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PK2_AnimSetPause( BOOL NewPauseFlag );

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */