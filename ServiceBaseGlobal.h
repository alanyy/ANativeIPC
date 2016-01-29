#ifndef SERVICEBASEGLOBAL_H
#define SERVICEBASEGLOBAL_H

// symbols visibility setting
#ifndef SERVICEBASE_API
#   ifdef SERVICEBASE_LIBRARY
#       define SERVICEBASE_API __attribute__((visibility("default")))
#   else
#       define SERVICEBASE_API
#   endif
#endif

/*
   Some classes do not permit copies to be made of an object. These
   classes contains a private copy constructor and assignment
   operator to disable copying (the compiler gives an error message).
*/
#define DISABLE_COPY(Class) \
    Class(const Class &); \
    Class &operator=(const Class &);

/*
   Avoid "unused parameter" warnings
*/
#define UNUSED(variable) (void)(variable)

#endif // SERVICEBASEGLOBAL_H
