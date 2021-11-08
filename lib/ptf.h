#ifndef PTF
#define PTF

void createUnit(int line, void* value);

#define CONCAT(a, b) CONCAT_INNER(a, b)
#define CONCAT_INNER(a, b) a ## b

#define UNIT_INNER(varName, value) \
typeof(value) varName = value; \
createUnit(__LINE__, &varName);

#define unit(value) UNIT_INNER(CONCAT(unitVal, __COUNTER__), value)

#endif