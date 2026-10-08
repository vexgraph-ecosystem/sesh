#ifndef SESH_LANG_ARITY_H
#define SESH_LANG_ARITY_H
/* Shared mechanical chooser; only declared arities are constructible. */
#define SESH_ARITY_SELECT(_0, _1, _2, _3, _4, _5, _6, N, ...) N
#define SESH_ARITY(...) SESH_ARITY_SELECT(_, __VA_ARGS__ __VA_OPT__(,) 6, 5, 4, 3, 2, 1, 0)
#define SESH_CONSTRUCTOR_JOIN_(Class, N) Class##_##N
#define SESH_CONSTRUCTOR_JOIN(Class, N) SESH_CONSTRUCTOR_JOIN_(Class, N)
#define SESH_CONSTRUCT(Class, ...) SESH_CONSTRUCTOR_JOIN(Class, SESH_ARITY(__VA_ARGS__))(__VA_ARGS__)
#endif
