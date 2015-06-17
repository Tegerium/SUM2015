/* FILENAME: UNIT.C
 * PROGRAMMER: PK2
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 10.06.2015
 */

#include <string.h>

#include "anim.h"

/* Функция по-умолчанию инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       PK2UNIT *Uni;
 *   - указатель на контекст анимации:
 *       PK2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID PK2_AnimUnitInit( PK2UNIT *Uni, PK2ANIM *Ani )
{
} /* End of 'PK2_AnimUnitInit' function */

/* Функция по-умолчанию деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       PK2UNIT *Uni;
 *   - указатель на контекст анимации:
 *       PK2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID PK2_AnimUnitClose( PK2UNIT *Uni, PK2ANIM *Ani )
{
} /* End of 'PK2_AnimUnitClose' function */

/* Функция по-умолчанию обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       PK2UNIT *Uni;
 *   - указатель на контекст анимации:
 *       PK2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID PK2_AnimUnitResponse( PK2UNIT *Uni, PK2ANIM *Ani )
{
} /* End of 'PK2_AnimUnitResponse' function */

/* Функция по-умолчанию построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       PK2UNIT *Uni;
 *   - указатель на контекст анимации:
 *       PK2ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID PK2_AnimUnitRender( PK2UNIT *Uni, PK2ANIM *Ani )
{
} /* End of 'PK2_AnimUnitRender' function */

/* Функция создания объекта анимации.
 * АРГУМЕНТЫ:
 *   - размер структуры объекта анимации:
 *       INT Size;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (PK2UNIT *) указатель на созданный объект анимации.
 */
PK2UNIT * PK2_AnimUnitCreate( INT Size )
{
  PK2UNIT *Uni;

  if (Size < sizeof(PK2UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);
  /* заполняем поля по-умолчанию */
  Uni->Size = Size;
  Uni->Init = PK2_AnimUnitInit;
  Uni->Close = PK2_AnimUnitClose;
  Uni->Response = PK2_AnimUnitResponse;
  Uni->Render = PK2_AnimUnitRender;
  return Uni;
} /* End of 'PK2_AnimUnitCreate' function */

/* END OF 'UNIT.C' FILE */
