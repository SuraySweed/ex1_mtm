#ifndef DATE_H_
#define DATE_H_

#include <stdbool.h>

/** Type for defining the date */
typedef struct Date_t *Date;

/**
* dateCreate: Allocates a new date.
*
* @param day - the day of the date.
* @param month - the month of the date.
* @param year - the year of the date.
* @return
* 	NULL - if allocation failed or date is illegal.
* 	A new Date in case of success.
*/
Date dateCreate(int day, int month, int year);

/**
* dateDestroy: Deallocates an existing Date.
*
* @param date - Target date to be deallocated. If priority queue is NULL nothing will be done
*/
void dateDestroy(Date date);

/**
* dateCopy: Creates a copy of target Date.
*
* @param date - Target Date.
* @return
* 	NULL if a NULL was sent or a memory allocation failed.
* 	A Date containing the same elements as date otherwise.
*/
Date dateCopy(Date date);

/**
* dateGet: Returns the day, month and year of a date
*
* @param date - Target Date
* @param day - the pointer to assign to day of the date into.
* @param month - the pointer to assign to month of the date into.
* @param year - the pointer to assign to year of the date into.
*
* @return
* 	false if one of pointers is NULL.
* 	Otherwise true and the date is assigned to the pointers.
*/
bool dateGet(Date date, int* day, int* month, int* year);

/**
* dateCompare: compares to dates and return which comes first
*
* @return
* 		A negative integer if date1 occurs first;
* 		0 if they're equal or one of the given dates is NULL;
*		A positive integer if date1 arrives after date2.
*/
int dateCompare(Date date1, Date date2);

/**
* dateTick: increases the date by one day, if date is NULL should do nothing.
*
* @param date - Target Date
*
*/
void dateTick(Date date);

#endif //DATE_H_
