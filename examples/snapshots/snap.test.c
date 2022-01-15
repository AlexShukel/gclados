#include "gclados.h"

#include <string.h>

TEST(test1) {
    char *value = "asasdfasdfasdfsadfas";
    ensure(value, gclados.toMatchSnapshot(strlen(value)));
}

TEST(test2) {
    char *snap2 = "aabb";
    ensure(snap2, gclados.toMatchSnapshot(strlen(snap2)));

    char *snap3 = "aacd";
    ensure(snap3, gclados.toMatchSnapshot(strlen(snap3)));
}