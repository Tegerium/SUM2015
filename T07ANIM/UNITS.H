/* FILENAME: UNITS.H
 * PROGRAMMER: PK2
 * PURPOSE: Animation units samples declaration module.
 * LAST UPDATE: 10.06.2015
 */

#ifndef __UNITS_H_
#define __UNITS_H_

#include "def.h"

/* Функция создания объекта анимации "мяч".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (PK2UNIT *) указатель на созданный объект анимации.
 */
PK2UNIT * PK2_UnitBallCreate( VOID );

/* Функция создания объекта анимации "управление".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (PK2UNIT *) указатель на созданный объект анимации.
 */
PK2UNIT * PK2_UnitControlCreate( VOID );

/* Функция создания объекта анимации "модель".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (PK2UNIT *) указатель на созданный объект анимации.
 */
PK2UNIT * PK2_UnitModelCreate( VOID );

#endif /* __UNITS_H_ */

/* END OF 'UNITS.H' FILE */
