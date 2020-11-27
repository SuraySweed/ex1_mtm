#include "test_utilities.h"
#include "../event_manager.h"
#include <stdlib.h>
#include <string.h>

#define NUMBER_TESTS 3

bool testEventManagerCreateDestroy() {
    bool result = true;
    Date start_date = dateCreate(1,12,2020);
    EventManager em = createEventManager(start_date);

    ASSERT_TEST(em != NULL, destroyEventManagerCreateDestroy);
    ASSERT_TEST(emGetEventsAmount(em) == 0, destroyEventManagerCreateDestroy);
    ASSERT_TEST(emGetNextEvent(em) == NULL, destroyEventManagerCreateDestroy);

destroyEventManagerCreateDestroy:
    destroyEventManager(em);
    dateDestroy(start_date);
    return result;

}

bool testAddEventByDiffAndSize() {
    bool result = true;

    Date start_date = dateCreate(1,12,2020);
    EventManager em = createEventManager(start_date);

    char* event_name = "event1";
    ASSERT_TEST(emAddEventByDiff(em, event_name, 2, 1) == EM_SUCCESS, destroyAddEventByDiffAndSize);
    ASSERT_TEST(emGetEventsAmount(em) == 1, destroyAddEventByDiffAndSize);
    ASSERT_TEST(strcmp(event_name, emGetNextEvent(em)) == 0, destroyAddEventByDiffAndSize);

destroyAddEventByDiffAndSize:
    dateDestroy(start_date);
    destroyEventManager(em);
    return result;
}

bool testEMTick() {
bool result = true;

    Date start_date = dateCreate(1,12,2020);
    EventManager em = createEventManager(start_date);

    char* event_name = "event1";
    ASSERT_TEST(emAddEventByDiff(em, event_name, 1, 1) == EM_SUCCESS, destroyEMTick);

    ASSERT_TEST(emGetEventsAmount(em) == 1, destroyEMTick);
    ASSERT_TEST(emTick(em, 2) == EM_SUCCESS, destroyEMTick);
    ASSERT_TEST(emGetEventsAmount(em) == 0, destroyEMTick);
destroyEMTick:
    dateDestroy(start_date);
    destroyEventManager(em);
    return result;
}

bool (*tests[]) (void) = {
        testEventManagerCreateDestroy,
        testAddEventByDiffAndSize,
        testEMTick
};

const char* testNames[] = {
        "testEventManagerCreateDestroy",
        "testAddEventByDiffAndSize",
        "testEMTick"
};

int main(int argc, char *argv[]) {
    if (argc == 1) {
        for (int test_idx = 0; test_idx < NUMBER_TESTS; test_idx++) {
            RUN_TEST(tests[test_idx], testNames[test_idx]);
        }
        return 0;
    }
    if (argc != 2) {
        fprintf(stdout, "Usage: event_manager_tests <test index>\n");
        return 0;
    }

    int test_idx = strtol(argv[1], NULL, 10);
    if (test_idx < 1 || test_idx > NUMBER_TESTS) {
        fprintf(stderr, "Invalid test index %d\n", test_idx);
        return 0;
    }

    RUN_TEST(tests[test_idx - 1], testNames[test_idx - 1]);
    return 0;
}
