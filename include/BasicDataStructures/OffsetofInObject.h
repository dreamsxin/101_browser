#ifndef _OffsetofInObject_h
#define _OffsetofInObject_h

#define offsetof_in_object(OBJECT, MEMBER) \
          ((size_t) ((char *)&(OBJECT.MEMBER) - (char*)&OBJECT))

#endif
