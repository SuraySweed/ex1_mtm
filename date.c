#include "date.h"
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define MAX_DAY 30
#define MIN_DAY 1
#define MAX_MONTH 12
#define MIN_MONTH 1
#define DAYS_IN_YEAR 365
#define POSTIVE_NUMBER 1
#define NEGATIVE_NUMBER -1
#define ZERO 0

struct Date_t{
    int day;
    int month;
    int year;
};

static bool isDayValid(int day)
{
    return (day >= MIN_DAY && day <= MAX_DAY);
}

static bool isMonthValid(int month)
{
    return (month >= MIN_MONTH && month <= MAX_MONTH);
}

static bool isDateValid(Date date)
{
    assert(date != NULL);
    return (isMonthValid(date->month) && isDayValid(date->day));
}

static int dateToDays(Date date)
{
    assert(date != NULL);
    return (date->day + ((date->month) * (MAX_DAY - MIN_DAY)) + (DAYS_IN_YEAR * (date->year))); 
}

static int dateDifference(Date date1, Date date2)
{
    assert(isDayValid(date1) && isDateValid(date2));

    int days1 = dateToDays(date1);
    int days2 = dateToDays(date2);

    return days1 - days2;
}

Date dateCreate(int day, int month, int year)
{
    if (!isDayValid(day) || !isMonthValid(month))
    {
        return NULL;
    }

    Date date = malloc(sizeof(*date));
    
    if(!date)
    {
        return NULL;
    }

    date->day = day;
    date->month = month;
    date->year = year;

    return date;
}

void dateDestroy(Date date)
{
    free(date);
}

Date dateCopy(Date date)
{
    if (!date)
    {
        return NULL;
    }

    return dateCreate(date->day, date->month, date->year);
}

bool dateGet(Date date, int* day, int* month, int* year)
{
    if(!date || !day || !month || !year)
    {
        return false;
    }

    date->day = *day;
    date->month = *month;
    date->year = *year;

    return true;
}

int dateCompare(Date date1, Date date2)
{
    int date_diffrence = 0;
    
    if(!date1 || !date2)
    {
        return ZERO;
    }
    assert(isDateValid(date1) && isDateValid(date2));

    date_diffrence = dateDifference(date1, date2);
 
    if(date_diffrence == ZERO)
    {
        return ZERO;
    }

    return date_diffrence > ZERO ? POSTIVE_NUMBER : NEGATIVE_NUMBER; 
}

void dateTick(Date date)
{
    if(!date)
    {
        return;
    }

    assert(isDateValid(date));

    if(date->day < 30)
    {
        date->day = date->day + 1;       
    }

    else if (date->day == 30 && date->month < 12)
    {
        date->day = 1;
        date->month = date->month + 1;
    }

    else if(date->day == 30 && date->month == 12)
    {
        date->day = 1;
        date->month = 1;
        date->year = date->year + 1;       
    } 
}