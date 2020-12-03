#include <math.h>
#include "../date.h"
#include "test_utilities.h"
#include <stdlib.h>
#include <assert.h>

#define ASSERT(expr) ASSERT_TEST(expr, destroy)


/* ========== TESTING SANITY ========== */

// Tests super basic functionality that will make other tests crash and make problems
bool testDateSanityTests() {
    bool result = true;
    Date date = dateCreate(1, 1, 2000);

    ASSERT(date != NULL);
    dateDestroy(NULL); // Make sure destroy can receive NULL

destroy:
    dateDestroy(date);
    return result;
}

/* ========== TESTING dateCreate ========== */
bool testDateCreateIllegalDatesReturnsNull() {
    bool result = true;

    ASSERT(dateCreate(0, 0, 2000) == NULL);
    ASSERT(dateCreate(0, 1, 2000) == NULL);
    ASSERT(dateCreate(1, 0, 2000) == NULL);
    ASSERT(dateCreate(-500, 1, 2000) == NULL);
    ASSERT(dateCreate(1, -2, 2000) == NULL);
    ASSERT(dateCreate(0, 0, -1000) == NULL);
    ASSERT(dateCreate(31, 10, 2000) == NULL);
    ASSERT(dateCreate(27, 13, 2000) == NULL);
    ASSERT(dateCreate(22, 19, 2000) == NULL);

destroy:
    return result;
}


bool testDateCreateWeirdButAllowedDatesAreAccepted() {
    bool result = true;
    Date date;

    date = dateCreate(1, 1, 0);
    ASSERT(date != NULL);
    dateDestroy(date);

    date = dateCreate(1, 1, -5);
    ASSERT(date != NULL);
    dateDestroy(date);

    date = dateCreate(30, 12, 35000);
    ASSERT(date != NULL);

destroy:
    dateDestroy(date);
    return result;
}


/* ========== TESTING dateCopy ========== */
bool testDateCopyNullArgument() {
    bool result = true;

    Date date = dateCopy(NULL);
    ASSERT(date == NULL);

destroy:
    dateDestroy(date);
    return result;
}

bool testDateCopyStandardTest() {
    bool result = true;
    Date date = dateCreate(1, 2, 2000);
    Date new_date = dateCopy(date);

    int copied_day;
    int copied_month;
    int copied_year;
    ASSERT(new_date != NULL);
    ASSERT(dateGet(new_date, &copied_day, &copied_month, &copied_year) == true);
    ASSERT(copied_day == 1);
    ASSERT(copied_month == 2);
    ASSERT(copied_year == 2000);

destroy:
    dateDestroy(date);
    dateDestroy(new_date);
    return result;
}



/* ========== TESTING dateGet ========== */
bool testDateGetStandardTest() {
    bool result = true;
    Date date = dateCreate(2, 3, 2000);

    int day;
    int month;
    int year;
    ASSERT(dateGet(date, &day, &month, &year) == true);
    ASSERT(day == 2);
    ASSERT(month == 3);
    ASSERT(year == 2000);

destroy:
    dateDestroy(date);
    return result;
}

bool testDateGetNullArgument() {
    bool result = true;
    Date date = dateCreate(1, 1, 2000);

    int num = 0;
    ASSERT(dateGet(date, &num, &num, NULL) == false);

destroy:
    dateDestroy(date);
    return result;
}



/* ========== TESTING dateCompare ========== */

bool testDateCompareNullArgument() {
    bool result = true;
    Date date = dateCreate(1, 1, 2000);

    ASSERT(dateCompare(date, NULL) == 0);
    ASSERT(dateCompare(NULL, date) == 0);

destroy:
    dateDestroy(date);
    return result;
}

bool testDateCompareStandardTest() {
    bool result = true;
    Date date1;
    Date date2;

    date1 = dateCreate(1, 1, 2000);
    date2 = dateCreate(1, 1, 2000);
    ASSERT(date1 != NULL && date2 != NULL);
    ASSERT(dateCompare(date1, date2) == 0);
    dateDestroy(date1);
    dateDestroy(date2);

    date1 = dateCreate(1, 1, -1);
    date2 = dateCreate(1, 1, -1);
    ASSERT(date1 != NULL && date2 != NULL);
    ASSERT(dateCompare(date1, date2) == 0);
    dateDestroy(date1);
    dateDestroy(date2);

    date1 = dateCreate(1, 1, 2000);
    date2 = dateCreate(1, 1, 2001);
    ASSERT(date1 != NULL && date2 != NULL);
    ASSERT(dateCompare(date1, date2) < 0);
    dateDestroy(date1);
    dateDestroy(date2);

    date1 = dateCreate(1, 1, 2000);
    date2 = dateCreate(2, 1, 2000);
    ASSERT(date1 != NULL && date2 != NULL);
    ASSERT(dateCompare(date1, date2) < 0);
    dateDestroy(date1);
    dateDestroy(date2);

    date1 = dateCreate(1, 1, -1000);
    date2 = dateCreate(1, 1, 2000);
    ASSERT(date1 != NULL && date2 != NULL);
    ASSERT(dateCompare(date1, date2) < 0);
    dateDestroy(date1);
    dateDestroy(date2);

    date1 = dateCreate(1, 1, 2000);
    date2 = dateCreate(1, 2, 2000);
    ASSERT(date1 != NULL && date2 != NULL);
    ASSERT(dateCompare(date1, date2) < 0);
    dateDestroy(date1);
    dateDestroy(date2);

    date1 = dateCreate(30, 12, 2000);
    date2 = dateCreate(1, 1, 2001);
    ASSERT(date1 != NULL && date2 != NULL);
    ASSERT(dateCompare(date1, date2) < 0);
    dateDestroy(date1);
    dateDestroy(date2);

    date1 = dateCreate(30, 1, 2000);
    date2 = dateCreate(1, 2, 2000);
    ASSERT(date1 != NULL && date2 != NULL);
    ASSERT(dateCompare(date1, date2) < 0);
    dateDestroy(date1);
    dateDestroy(date2);

    date2 = dateCreate(1, 1, 2000);
    date1 = dateCreate(1, 1, 2001);
    ASSERT(date1 != NULL && date2 != NULL);
    ASSERT(dateCompare(date1, date2) > 0);
    dateDestroy(date1);
    dateDestroy(date2);

    date2 = dateCreate(1, 1, 2000);
    date1 = dateCreate(2, 1, 2000);
    ASSERT(date1 != NULL && date2 != NULL);
    ASSERT(dateCompare(date1, date2) > 0);
    dateDestroy(date1);
    dateDestroy(date2);

    date2 = dateCreate(1, 1, -1000);
    date1 = dateCreate(1, 1, 2000);
    ASSERT(date1 != NULL && date2 != NULL);
    ASSERT(dateCompare(date1, date2) > 0);
    dateDestroy(date1);
    dateDestroy(date2);

    date2 = dateCreate(1, 1, 2000);
    date1 = dateCreate(1, 2, 2000);
    ASSERT(date1 != NULL && date2 != NULL);
    ASSERT(dateCompare(date1, date2) > 0);
    dateDestroy(date1);
    dateDestroy(date2);

    date2 = dateCreate(30, 12, 2000);
    date1 = dateCreate(1, 1, 2001);
    ASSERT(date1 != NULL && date2 != NULL);
    ASSERT(dateCompare(date1, date2) > 0);
    dateDestroy(date1);
    dateDestroy(date2);

    date2 = dateCreate(30, 1, 2000);
    date1 = dateCreate(1, 2, 2000);
    ASSERT(date1 != NULL && date2 != NULL);
    ASSERT(dateCompare(date1, date2) > 0);

destroy:
    dateDestroy(date1);
    dateDestroy(date2);
    return result;
}




/* ========== TESTING dateTick ========== */
bool checkDate(int day, int month, int year, int day_expected, int month_expected, int year_expected) {
    return day == day_expected && month == month_expected && year == year_expected;
}

bool testDateTickStandardTest() {
    bool result = true;
    Date date;
    int day, month, year;

    date = dateCreate(1, 1, 2000);
    dateTick(date);
    ASSERT(date != NULL);
    ASSERT(dateGet(date, &day, &month, &year) == true);
    ASSERT(checkDate(day, month, year, 2, 1, 2000) == true);
    dateDestroy(date);

    date = dateCreate(30, 1, 2000);
    dateTick(date);
    ASSERT(date != NULL);
    ASSERT(dateGet(date, &day, &month, &year) == true);
    ASSERT(checkDate(day, month, year, 1, 2, 2000) == true);
    dateDestroy(date);

    date = dateCreate(1, 12, 2000);
    dateTick(date);
    ASSERT(date != NULL);
    ASSERT(dateGet(date, &day, &month, &year) == true);
    ASSERT(checkDate(day, month, year, 2, 12, 2000) == true);
    dateDestroy(date);

    date = dateCreate(30, 12, 2000);
    dateTick(date);
    ASSERT(date != NULL);
    ASSERT(dateGet(date, &day, &month, &year) == true);
    ASSERT(checkDate(day, month, year, 1, 1, 2001) == true);
    dateDestroy(date);

    date = dateCreate(30, 12, -1);
    dateTick(date);
    ASSERT(date != NULL);
    ASSERT(dateGet(date, &day, &month, &year) == true);
    ASSERT(checkDate(day, month, year, 1, 1, 0) == true);

destroy:
    dateDestroy(date);
    return result;
}


#define TEST_NAMES \
    X(testDateSanityTests) \
    X(testDateCreateIllegalDatesReturnsNull) \
    X(testDateCreateWeirdButAllowedDatesAreAccepted) \
    X(testDateCopyNullArgument) \
    X(testDateCopyStandardTest) \
    X(testDateGetNullArgument) \
    X(testDateGetStandardTest) \
    X(testDateCompareNullArgument) \
    X(testDateCompareStandardTest) \
    X(testDateTickStandardTest)


bool (*tests[])(void) = {
#define X(test_name) test_name,
        TEST_NAMES
#undef X
};

const char* testNames[] = {
#define X(name) #name,
        TEST_NAMES
#undef X
};


#define NUMBER_TESTS 10

int main(int argc, char** argv) {
    if (argc == 1) {
        for (int test_idx = 0; test_idx < NUMBER_TESTS; test_idx++) {
            RUN_TEST(tests[test_idx], testNames[test_idx], 0);
        }
        return 0;
    }
    if (argc != 2) {
        fprintf(stdout, "Usage: priority_queue_tests <test index>\n");
        return 0;
    }

    int test_idx = strtol(argv[1], NULL, 10);
    if (test_idx < 1 || test_idx > NUMBER_TESTS) {
        fprintf(stderr, "Invalid test index %d\n", test_idx);
        return 0;
    }

    RUN_TEST(tests[test_idx - 1], testNames[test_idx - 1], 0);
    return 0;

}
