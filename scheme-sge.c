#include <stdio.h>
#include <SGE.h>
#include <chibi/eval.h>

#define TYPE_EXCEPTION(__TYPE_, __ARG_) \
    return sexp_type_exception(ctx, self, __TYPE_, __ARG_);

#define MUST_BE_EXACT(__EXACT_VAR) \
    if (!sexp_exact_integerp(__EXACT_VAR)) TYPE_EXCEPTION(SEXP_FIXNUM, __EXACT_VAR);

#define MUST_BE_FLOAT(__FLOAT_VAR) \
    if (!sexp_flonump(__FLOAT_VAR)) TYPE_EXCEPTION(SEXP_FLONUM, __FLOAT_VAR);

#define MUST_BE_PAIR(__PAIR_VAR) \
    if (!sexp_pairp(__PAIR_VAR)) TYPE_EXCEPTION(SEXP_PAIR, __PAIR_VAR)

#define MUST_BE_STRING(__STRING_VAR) \
    if (!sexp_stringp(__STRING_VAR)) TYPE_EXCEPTION(SEXP_STRING, __STRING_VAR);

#define COMMON_ARGS \
    sexp ctx, sexp self, sexp_sint_t n

#define TRY(__EXPR__) \
    if (!__EXPR__) { return sexp_user_exception(ctx, self, SGE_GetError(), SEXP_NULL); }

static sexp sexp_IsRunning(COMMON_ARGS) {
    return sexp_make_boolean(SGE_IsRunning());
}

static sexp sexp_SetRefreshColor(COMMON_ARGS, sexp arg0) {
    SGE_Color color = {
        sexp_uint_value(sexp_vector_ref(arg0, SEXP_ZERO)),
        sexp_uint_value(sexp_vector_ref(arg0, SEXP_ONE)),
        sexp_uint_value(sexp_vector_ref(arg0, SEXP_TWO)),
        sexp_uint_value(sexp_vector_ref(arg0, SEXP_THREE))
    };
    SGE_SetRefreshColor(color);
    return SEXP_NULL;
}

static sexp sexp_GetWindowSize(COMMON_ARGS) {
    SGE_UIVec2 size = SGE_GetWindowSize();
    return sexp_cons(ctx,
                     sexp_make_unsigned_integer(ctx, size.x),
                     sexp_make_unsigned_integer(ctx, size.y));
}

static sexp sexp_EntityCreate(COMMON_ARGS) {
    SGE_UUID entity;
    SGE_EntityCreate(&entity);
    return sexp_make_unsigned_integer(ctx, entity);
}

static sexp sexp_EntityClone(COMMON_ARGS, sexp arg0) {
    MUST_BE_EXACT(arg0);
    SGE_UUID clone;
    TRY(SGE_EntityClone(sexp_uint_value(arg0), &clone));
    return sexp_make_unsigned_integer(ctx, clone);
}

static sexp sexp_EntityAddAttrib(COMMON_ARGS, sexp arg0, sexp arg1) {
    MUST_BE_EXACT(arg0);
    MUST_BE_EXACT(arg1);
    TRY(SGE_EntityAddAttribute(sexp_uint_value(arg0), sexp_sint_value(arg1)));
    return arg0;
}

static sexp sexp_EntitySetAnimation(COMMON_ARGS, sexp arg0, sexp arg1) {
    MUST_BE_EXACT(arg0);
    MUST_BE_EXACT(arg1);
    TRY(SGE_EntitySetAnimation(sexp_uint_value(arg0), sexp_uint_value(arg1)));
    return arg0;
}

static sexp sexp_EntitySetKeyframe(COMMON_ARGS, sexp arg0, sexp arg1) {
    MUST_BE_EXACT(arg0);
    MUST_BE_EXACT(arg1);
    TRY(SGE_EntitySetKeyframe(sexp_uint_value(arg0), sexp_uint_value(arg1)));
    return arg0;
}

static sexp sexp_EntitySetPosition(COMMON_ARGS, sexp arg0, sexp arg1) {
    MUST_BE_EXACT(arg0);
    MUST_BE_PAIR(arg1);
    SGE_Vec2 position = {
        sexp_flonum_value(sexp_car(arg1)),
        sexp_flonum_value(sexp_cdr(arg1))
    };
    const SGE_UUID entity = sexp_uint_value(arg0);
    TRY(SGE_EntitySetPosition(entity, position));
    return arg0;
}

static sexp sexp_EntitySetScale(COMMON_ARGS, sexp arg0, sexp arg1) {
    MUST_BE_EXACT(arg0);
    MUST_BE_PAIR(arg1);
    SGE_Vec2 scale = {
        sexp_flonum_value(sexp_car(arg1)),
        sexp_flonum_value(sexp_cdr(arg1))
    };
    TRY(SGE_EntitySetScale(sexp_uint_value(arg0), scale));
    return arg0;
}

static sexp sexp_EntitySetBlendMode(COMMON_ARGS, sexp arg0, sexp arg1) {
    MUST_BE_EXACT(arg0);
    MUST_BE_EXACT(arg1);
    TRY(SGE_EntitySetBlendMode(sexp_uint_value(arg0), sexp_sint_value(arg1)));
    return arg0;
}

static sexp sexp_EntitySetZOrder(COMMON_ARGS, sexp arg0, sexp arg1) {
    MUST_BE_EXACT(arg0);
    MUST_BE_EXACT(arg1);
    TRY(SGE_EntitySetZOrder(sexp_uint_value(arg0), sexp_uint_value(arg1)));
    return arg0;
}

static sexp sexp_EntitySetColor(COMMON_ARGS, sexp arg0, sexp arg1) {
    MUST_BE_EXACT(arg0);
    SGE_Color color = {
        sexp_uint_value(sexp_vector_ref(arg1, SEXP_ZERO)),
        sexp_uint_value(sexp_vector_ref(arg1, SEXP_ONE)),
        sexp_uint_value(sexp_vector_ref(arg1, SEXP_TWO)),
        sexp_uint_value(sexp_vector_ref(arg1, SEXP_THREE))
    };
    TRY(SGE_EntitySetColor(sexp_uint_value(arg0), color));
    return arg0;
}

static sexp sexp_EntityGetPosition(COMMON_ARGS, sexp arg0) {
    MUST_BE_EXACT(arg0);
    SGE_Vec2 position;
    TRY(SGE_EntityGetPosition(sexp_uint_value(arg0), &position));
    return sexp_cons(ctx,
                     sexp_make_flonum(ctx, position.x),
                     sexp_make_flonum(ctx, position.y));
}

static sexp sexp_EntityGetKeyframe(COMMON_ARGS, sexp arg0) {
    MUST_BE_EXACT(arg0);
    SGE_Keyframe keyframe;
    TRY(SGE_EntityGetKeyframe(sexp_uint_value(arg0), &keyframe));
    return sexp_make_unsigned_integer(ctx, keyframe);
}

static sexp sexp_EntityRemove(COMMON_ARGS, sexp arg0) {
    MUST_BE_EXACT(arg0);
    TRY(SGE_EntityRemove(sexp_uint_value(arg0)));
    return SEXP_NULL;
}

static sexp sexp_EntityRemoveAttrib(COMMON_ARGS, sexp arg0, sexp arg1) {
    MUST_BE_EXACT(arg0);
    MUST_BE_EXACT(arg1);
    TRY(SGE_EntityAddAttribute(sexp_uint_value(arg0), sexp_sint_value(arg1)));
    return arg0;
}

static sexp sexp_CameraSetTarget(COMMON_ARGS, sexp arg0) {
    MUST_BE_EXACT(arg0);
    TRY(SGE_CameraSetTarget(sexp_uint_value(arg0)));
    return SEXP_NULL;
}

static sexp sexp_CameraSetCenter(COMMON_ARGS, sexp arg0) {
    MUST_BE_PAIR(arg0);
    SGE_Vec2 position = {
        sexp_flonum_value(sexp_car(arg0)),
        sexp_flonum_value(sexp_cdr(arg0))
    };
    SGE_CameraSetCenter(position);
    return SEXP_NULL;
}

static sexp sexp_CameraSetSpringiness(COMMON_ARGS, sexp arg0) {
    MUST_BE_FLOAT(arg0);
    SGE_CameraSetSpringiness(sexp_flonum_value(arg0));
    return SEXP_NULL;
}

static sexp sexp_CameraSetZoom(COMMON_ARGS, sexp arg0) {
    MUST_BE_FLOAT(arg0);
    SGE_CameraSetZoom(sexp_flonum_value(arg0));
    return SEXP_NULL;
}

static sexp sexp_CameraGetViewSize(COMMON_ARGS) {
    SGE_Vec2 position = SGE_CameraGetViewSize();
    return sexp_cons(ctx,
                     sexp_make_flonum(ctx, position.x),
                     sexp_make_flonum(ctx, position.y));
}

static sexp sexp_TimerCreate(COMMON_ARGS) {
    SGE_UUID timer;
    TRY(SGE_TimerCreate(&timer));
    return sexp_make_unsigned_integer(ctx, timer);
}

static sexp sexp_TimerReset(COMMON_ARGS, sexp arg0) {
    MUST_BE_EXACT(arg0);
    SGE_USec elapsed;
    TRY(SGE_TimerReset(sexp_uint_value(arg0), &elapsed));
    return sexp_make_unsigned_integer(ctx, elapsed);
}

static sexp sexp_TimerRemove(COMMON_ARGS, sexp arg0) {
    MUST_BE_EXACT(arg0);
    TRY(SGE_TimerRemove(sexp_uint_value(arg0)));
    return SEXP_NULL;
}

static sexp sexp_AnimationCreate(COMMON_ARGS, sexp arg0, sexp arg1, sexp arg2, sexp arg3) {
    MUST_BE_STRING(arg0);
    MUST_BE_PAIR(arg1);
    MUST_BE_PAIR(arg2);
    MUST_BE_PAIR(arg3);
    SGE_UUID animation;
    SGE_IVec2 start = {
        sexp_uint_value(sexp_car(arg1)),
        sexp_uint_value(sexp_cdr(arg1))
    };
    SGE_IVec2 frameSize = {
        sexp_uint_value(sexp_car(arg2)),
        sexp_uint_value(sexp_cdr(arg2))
    };
    SGE_Vec2 origin = {
        sexp_flonum_value(sexp_car(arg3)),
        sexp_flonum_value(sexp_cdr(arg3))
    };
    TRY(SGE_AnimationCreate(&animation,
                            sexp_string_data(arg0),
                            start, frameSize, origin));
    return sexp_make_unsigned_integer(ctx, animation);
}

static sexp sexp_PollEvent(COMMON_ARGS) {
    
}

// TODO: pollevents
// TODO: recordevents

static sexp sexp_Microsleep(COMMON_ARGS, sexp arg0) {
    MUST_BE_EXACT(arg0);
    SGE_Microsleep(sexp_uint_value(arg0));
    return SEXP_NULL;
}

static sexp sexp_ResourcePath(COMMON_ARGS) {
    return sexp_c_string(ctx, SGE_ResourcePath(), -1);
}

#define FFI_EXPORT(__LISP_NAME_, __ARGC_, __C_NAME_) \
    sexp_define_foreign(ctx, env, __LISP_NAME_, __ARGC_, (sexp_proc1)__C_NAME_)

void sexp_SGE_LibraryWrap(sexp ctx, sexp env) {
    FFI_EXPORT("sge-is-running?", 0, sexp_IsRunning);
    FFI_EXPORT("sge-set-refresh-rgba", 1, sexp_SetRefreshColor);
    FFI_EXPORT("sge-window-size", 0, sexp_GetWindowSize);
    FFI_EXPORT("sge-entity-create", 0, sexp_EntityCreate);
    FFI_EXPORT("sge-entity-clone", 1, sexp_EntityClone);
    FFI_EXPORT("sge-entity-add-attrib", 2, sexp_EntityAddAttrib);
    FFI_EXPORT("sge-entity-set-animation", 2, sexp_EntitySetAnimation);
    FFI_EXPORT("sge-entity-set-keyframe", 2, sexp_EntitySetKeyframe);
    FFI_EXPORT("sge-entity-set-position", 1, sexp_EntitySetPosition);
    FFI_EXPORT("sge-entity-set-scale", 2, sexp_EntitySetScale);
    FFI_EXPORT("sge-entity-set-blend-mode", 2, sexp_EntitySetBlendMode);
    FFI_EXPORT("sge-entity-set-zorder", 2, sexp_EntitySetZOrder);
    FFI_EXPORT("sge-entity-set-rgba", 2, sexp_EntitySetColor);
    FFI_EXPORT("sge-entity-get-position", 1, sexp_EntityGetPosition);
    FFI_EXPORT("sge-entity-get-keyframe", 1, sexp_EntityGetKeyframe);
    FFI_EXPORT("sge-entity-remove", 1, sexp_EntityRemove);
    FFI_EXPORT("sge-entity-remove-attrib", 2, sexp_EntityRemoveAttrib);
    FFI_EXPORT("sge-camera-set-target", 1, sexp_CameraSetTarget);
    FFI_EXPORT("sge-camera-set-center", 1, sexp_CameraSetCenter);
    FFI_EXPORT("sge-camera-set-springiness", 1, sexp_CameraSetSpringiness);
    FFI_EXPORT("sge-camera-set-zoom", 1, sexp_CameraSetZoom);
    FFI_EXPORT("sge-camera-get-viewsize", 0, sexp_CameraGetViewSize);
    FFI_EXPORT("sge-timer-create", 0, sexp_TimerCreate);
    FFI_EXPORT("sge-timer-reset", 1, sexp_TimerReset);
    FFI_EXPORT("sge-timer-remove", 1, sexp_TimerRemove);
    FFI_EXPORT("sge-animation-create", 4, sexp_AnimationCreate);

    FFI_EXPORT("sge-microsleep", 1, sexp_Microsleep);
    FFI_EXPORT("sge-resource-path", 0, sexp_ResourcePath);
}

void schemeEntry() {
    sexp ctx, env;
    ctx = sexp_make_eval_context(NULL, NULL, NULL, 0, 0);
    env = sexp_load_standard_env(ctx, NULL, SEXP_SEVEN);
    sexp_SGE_LibraryWrap(ctx, env);
    sexp_load_standard_ports(ctx, NULL, stdin, stdout, stderr, 0);
    sexp_gc_var1(mainFile);
    sexp_gc_preserve1(ctx, mainFile);
    static const char* subPath = "scripts/main.scm";
    const char* resPath = SGE_ResourcePath();
    const size_t resPathLen = strlen(resPath);
    const size_t subPathLen = strlen(subPath);
    char* fullStartPath = malloc(resPathLen + subPathLen + 1);
    if (!fullStartPath) {
        goto CLEANUP;
    }
    strncpy(fullStartPath, resPath, resPathLen);
    strncpy(fullStartPath + resPathLen, subPath, subPathLen);
    fullStartPath[resPathLen + subPathLen] = '\0';
    mainFile = sexp_c_string(ctx, fullStartPath, -1);
    free(fullStartPath);
    sexp_load(ctx, mainFile, NULL);
 CLEANUP:
    sexp_destroy_context(ctx);
    SGE_Exit();
}

int main(int argc, char** argv) {
    if (argc == 2) {
        SGE_ConfigureResourcePath(argv[1]);
    } else if (argc != 1) {
        puts("usage: scheme-sge <path-to-package>");
    }
    SGE_Main(schemeEntry);
    return 0;
}
