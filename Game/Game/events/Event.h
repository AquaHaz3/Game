#pragma once

typedef __int64 EventID;

#define ________END_OF_MACRO________

#define    MAKE_CLASS_UNIQUE           \
                                       \
static char __uuid_field;              \
                                       \
static __int64 getClassUUID(){         \
	return ((__int64)(&__uuid_field)); \
}                                      \
     ________END_OF_MACRO________      \

#define RESOLVE_UNIQUE_SYMBOL(ClassName) char ClassName::__uuid_field = 0;

/* if it show error, add MAKE_CLASS_UNIQUE to class*/
#define GET_CLASS_UUID() ((__int64)(&__uuid_field))

class Event {

public:

	EventID uuid;

	Event() { uuid = 0; }
	Event(EventID type)
	{
		uuid = type;
	}

};
