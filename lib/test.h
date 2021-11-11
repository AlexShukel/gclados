#ifndef __PTF_TEST_H__
#define __PTF_TEST_H__

struct PtfTest {
    void (*execute)(void);
    const char* description;
};

struct PtfTest createPtfTest(void (*execute)(void), const char* description);

#endif