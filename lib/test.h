#ifndef __PTF_TEST_H__
#define __PTF_TEST_H__

struct PtfTest {
    void (*execute)(void);
    char* description;
};

struct PtfTest createPtfTest(void (*execute)(void), char* description);

#endif