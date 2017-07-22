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
    SGE_EventHolder holder;
    if (SGE_PollEvents(&holder)) {
        switch (holder.code) {
        case SGE_EventCode_TextEntered: {
            sexp result = sexp_make_vector(ctx, SEXP_TWO, SEXP_NULL);
            sexp_vector_set(result, SEXP_ZERO,
                            sexp_make_integer(ctx, holder.code));
            sexp_vector_set(result, SEXP_ONE,
                            sexp_make_unsigned_integer(ctx, holder.event.text.unicode));
            return result;
        }
            
        case SGE_EventCode_KeyReleased:
        case SGE_EventCode_KeyPressed: {
            sexp result = sexp_make_vector(ctx, SEXP_TWO, SEXP_NULL);
            sexp_vector_set(result, SEXP_ZERO,
                            sexp_make_integer(ctx, holder.code));
            sexp_vector_set(result, SEXP_ONE,
                            sexp_make_integer(ctx, holder.event.key.keyCode));
            return result;
        }
        }
    }
    return SEXP_NULL;
}

static sexp sexp_Microsleep(COMMON_ARGS, sexp arg0) {
    MUST_BE_EXACT(arg0);
    SGE_Microsleep(sexp_uint_value(arg0));
    return SEXP_NULL;
}

static sexp sexp_ResourcePath(COMMON_ARGS) {
    return sexp_c_string(ctx, SGE_ResourcePath(), -1);
}

void sexp_SGE_LibraryExport(sexp ctx, sexp env) {
#define FFI_EXPORTP(__STR_NAME_, __ARGC_, __C_NAME_) \
    sexp_define_foreign(ctx, env, __STR_NAME_, __ARGC_, (sexp_proc1)__C_NAME_)
#define FFI_EXPORTI(__STR_NAME_, __VALUE_) \
    sexp_env_define(ctx, env, sexp_intern(ctx, __STR_NAME_, sizeof(__STR_NAME_) - 1), sexp_make_integer(ctx, __VALUE_))
    FFI_EXPORTP("sge-is-running?", 0, sexp_IsRunning);
    FFI_EXPORTP("sge-set-refresh-rgba", 1, sexp_SetRefreshColor);
    FFI_EXPORTP("sge-window-size", 0, sexp_GetWindowSize);
    FFI_EXPORTP("sge-entity-create", 0, sexp_EntityCreate);
    FFI_EXPORTP("sge-entity-clone", 1, sexp_EntityClone);
    FFI_EXPORTP("sge-entity-add-attrib", 2, sexp_EntityAddAttrib);
    FFI_EXPORTP("sge-entity-set-animation", 2, sexp_EntitySetAnimation);
    FFI_EXPORTP("sge-entity-set-keyframe", 2, sexp_EntitySetKeyframe);
    FFI_EXPORTP("sge-entity-set-position", 2, sexp_EntitySetPosition);
    FFI_EXPORTP("sge-entity-set-scale", 2, sexp_EntitySetScale);
    FFI_EXPORTP("sge-entity-set-blend-mode", 2, sexp_EntitySetBlendMode);
    FFI_EXPORTP("sge-entity-set-zorder", 2, sexp_EntitySetZOrder);
    FFI_EXPORTP("sge-entity-set-rgba", 2, sexp_EntitySetColor);
    FFI_EXPORTP("sge-entity-get-position", 1, sexp_EntityGetPosition);
    FFI_EXPORTP("sge-entity-get-keyframe", 1, sexp_EntityGetKeyframe);
    FFI_EXPORTP("sge-entity-remove", 1, sexp_EntityRemove);
    FFI_EXPORTP("sge-entity-remove-attrib", 2, sexp_EntityRemoveAttrib);
    FFI_EXPORTP("sge-camera-set-target", 1, sexp_CameraSetTarget);
    FFI_EXPORTP("sge-camera-set-center", 1, sexp_CameraSetCenter);
    FFI_EXPORTP("sge-camera-set-springiness", 1, sexp_CameraSetSpringiness);
    FFI_EXPORTP("sge-camera-set-zoom", 1, sexp_CameraSetZoom);
    FFI_EXPORTP("sge-camera-get-viewsize", 0, sexp_CameraGetViewSize);
    FFI_EXPORTP("sge-timer-create", 0, sexp_TimerCreate);
    FFI_EXPORTP("sge-timer-reset", 1, sexp_TimerReset);
    FFI_EXPORTP("sge-timer-remove", 1, sexp_TimerRemove);
    FFI_EXPORTP("sge-animation-create", 4, sexp_AnimationCreate);
    FFI_EXPORTP("sge-poll-event", 0, sexp_PollEvent);
    FFI_EXPORTP("sge-microsleep", 1, sexp_Microsleep);
    FFI_EXPORTP("sge-resource-path", 0, sexp_ResourcePath);
    FFI_EXPORTI("sge-key-a", SGE_KeyA);
    FFI_EXPORTI("sge-key-b", SGE_KeyB);
    FFI_EXPORTI("sge-key-c", SGE_KeyC);
    FFI_EXPORTI("sge-key-d", SGE_KeyD);
    FFI_EXPORTI("sge-key-e", SGE_KeyE);
    FFI_EXPORTI("sge-key-f", SGE_KeyF);
    FFI_EXPORTI("sge-key-g", SGE_KeyG);
    FFI_EXPORTI("sge-key-h", SGE_KeyH);
    FFI_EXPORTI("sge-key-i", SGE_KeyI);
    FFI_EXPORTI("sge-key-j", SGE_KeyJ);
    FFI_EXPORTI("sge-key-k", SGE_KeyK);
    FFI_EXPORTI("sge-key-l", SGE_KeyL);
    FFI_EXPORTI("sge-key-m", SGE_KeyM);
    FFI_EXPORTI("sge-key-n", SGE_KeyN);
    FFI_EXPORTI("sge-key-o", SGE_KeyO);
    FFI_EXPORTI("sge-key-p", SGE_KeyP);
    FFI_EXPORTI("sge-key-q", SGE_KeyQ);
    FFI_EXPORTI("sge-key-r", SGE_KeyR);
    FFI_EXPORTI("sge-key-s", SGE_KeyS);
    FFI_EXPORTI("sge-key-t", SGE_KeyT);
    FFI_EXPORTI("sge-key-u", SGE_KeyU);
    FFI_EXPORTI("sge-key-v", SGE_KeyV);
    FFI_EXPORTI("sge-key-w", SGE_KeyW);
    FFI_EXPORTI("sge-key-x", SGE_KeyX);
    FFI_EXPORTI("sge-key-y", SGE_KeyY);
    FFI_EXPORTI("sge-key-z", SGE_KeyZ);
    FFI_EXPORTI("sge-key-0", SGE_KeyNum0);
    FFI_EXPORTI("sge-key-1", SGE_KeyNum1);
    FFI_EXPORTI("sge-key-2", SGE_KeyNum2);
    FFI_EXPORTI("sge-key-3", SGE_KeyNum3);
    FFI_EXPORTI("sge-key-4", SGE_KeyNum4);
    FFI_EXPORTI("sge-key-5", SGE_KeyNum5);
    FFI_EXPORTI("sge-key-6", SGE_KeyNum6);
    FFI_EXPORTI("sge-key-7", SGE_KeyNum7);
    FFI_EXPORTI("sge-key-8", SGE_KeyNum8);
    FFI_EXPORTI("sge-key-9", SGE_KeyNum9);
    FFI_EXPORTI("sge-key-esc", SGE_KeyEscape);
    FFI_EXPORTI("sge-key-lctrl", SGE_KeyLControl);
    FFI_EXPORTI("sge-key-lshift", SGE_KeyLShift);
    FFI_EXPORTI("sge-key-lalt", SGE_KeyLAlt);
    FFI_EXPORTI("sge-key-lsystem", SGE_KeyLSystem);
    FFI_EXPORTI("sge-key-lctrl", SGE_KeyRControl);
    FFI_EXPORTI("sge-key-rshift", SGE_KeyRShift);
    FFI_EXPORTI("sge-key-ralt", SGE_KeyRAlt);
    FFI_EXPORTI("sge-key-rsystem", SGE_KeyRSystem);
    FFI_EXPORTI("sge-key-menu", SGE_KeyMenu);
    FFI_EXPORTI("sge-key-lbracket", SGE_KeyLBracket);
    FFI_EXPORTI("sge-key-rbracket", SGE_KeyRBracket);
    FFI_EXPORTI("sge-key-semicolon", SGE_KeySemiColon);
    FFI_EXPORTI("sge-key-comma", SGE_KeyComma);
    FFI_EXPORTI("sge-key-period", SGE_KeyPeriod);
    FFI_EXPORTI("sge-key-quote", SGE_KeyQuote);
    FFI_EXPORTI("sge-key-slash", SGE_KeySlash);
    FFI_EXPORTI("sge-key-backslash", SGE_KeyBackSlash);
    FFI_EXPORTI("sge-key-tilde", SGE_KeyTilde);
    FFI_EXPORTI("sge-key-equal", SGE_KeyEqual);
    FFI_EXPORTI("sge-key-dash", SGE_KeyDash);
    FFI_EXPORTI("sge-key-space", SGE_KeySpace);
    FFI_EXPORTI("sge-key-return", SGE_KeyReturn);
    FFI_EXPORTI("sge-key-backspace", SGE_KeyBackSpace);
    FFI_EXPORTI("sge-key-tab", SGE_KeyTab);
    FFI_EXPORTI("sge-key-pageup", SGE_KeyPageUp);
    FFI_EXPORTI("sge-key-pagedown", SGE_KeyPageDown);
    FFI_EXPORTI("sge-key-end", SGE_KeyEnd);
    FFI_EXPORTI("sge-key-home", SGE_KeyHome);
    FFI_EXPORTI("sge-key-insert", SGE_KeyInsert);
    FFI_EXPORTI("sge-key-delete", SGE_KeyDelete);
    FFI_EXPORTI("sge-key-plus", SGE_KeyAdd);
    FFI_EXPORTI("sge-key-minus", SGE_KeySubtract);
    FFI_EXPORTI("sge-key-multiply", SGE_KeyMultiply);
    FFI_EXPORTI("sge-key-divide", SGE_KeyDivide);
    FFI_EXPORTI("sge-key-left", SGE_KeyLeft);
    FFI_EXPORTI("sge-key-right", SGE_KeyRight);
    FFI_EXPORTI("sge-key-up", SGE_KeyUp);
    FFI_EXPORTI("sge-key-down", SGE_KeyDown);
    FFI_EXPORTI("sge-key-num0", SGE_KeyNumpad0);
    FFI_EXPORTI("sge-key-num1", SGE_KeyNumpad1);
    FFI_EXPORTI("sge-key-num2", SGE_KeyNumpad2);
    FFI_EXPORTI("sge-key-num3", SGE_KeyNumpad3);
    FFI_EXPORTI("sge-key-num4", SGE_KeyNumpad4);
    FFI_EXPORTI("sge-key-num5", SGE_KeyNumpad5);
    FFI_EXPORTI("sge-key-num6", SGE_KeyNumpad6);
    FFI_EXPORTI("sge-key-num7", SGE_KeyNumpad7);
    FFI_EXPORTI("sge-key-num8", SGE_KeyNumpad8);
    FFI_EXPORTI("sge-key-num9", SGE_KeyNumpad9);
    FFI_EXPORTI("sge-key-f1", SGE_KeyF1);
    FFI_EXPORTI("sge-key-f2", SGE_KeyF2);
    FFI_EXPORTI("sge-key-f3", SGE_KeyF3);
    FFI_EXPORTI("sge-key-f4", SGE_KeyF4);
    FFI_EXPORTI("sge-key-f5", SGE_KeyF5);
    FFI_EXPORTI("sge-key-f6", SGE_KeyF6);
    FFI_EXPORTI("sge-key-f7", SGE_KeyF7);
    FFI_EXPORTI("sge-key-f8", SGE_KeyF8);
    FFI_EXPORTI("sge-key-f9", SGE_KeyF9);
    FFI_EXPORTI("sge-key-f10", SGE_KeyF10);
    FFI_EXPORTI("sge-key-f11", SGE_KeyF11);
    FFI_EXPORTI("sge-key-f12", SGE_KeyF12);
    FFI_EXPORTI("sge-key-f13", SGE_KeyF13);
    FFI_EXPORTI("sge-key-f14", SGE_KeyF14);
    FFI_EXPORTI("sge-key-f15", SGE_KeyF15);
    FFI_EXPORTI("sge-key-pause", SGE_KeyPause);
    FFI_EXPORTI("sge-key-count", SGE_KeyCount);
    FFI_EXPORTI("sge-event-text-entered", SGE_EventCode_TextEntered);
    FFI_EXPORTI("sge-event-key-pressed", SGE_EventCode_KeyPressed);
    FFI_EXPORTI("sge-event-key-released", SGE_EventCode_KeyReleased);
}

void schemeEntry() {
    sexp ctx, env;
    ctx = sexp_make_eval_context(NULL, NULL, NULL, 0, 0);
    env = sexp_load_standard_env(ctx, NULL, SEXP_SEVEN);
    sexp_SGE_LibraryExport(ctx, env);
    sexp_load_standard_ports(ctx, NULL, stdin, stdout, stderr, 0);
    sexp_gc_var2(mainFile, result);
    sexp_gc_preserve2(ctx, mainFile, result);
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
    result = sexp_load(ctx, mainFile, NULL);
    if (sexp_exceptionp(result)) {
        printf("\n[[ Exception ]]");
        printf("\n  kind: ");
        sexp_write(ctx, sexp_exception_kind(result), sexp_current_output_port(ctx));
        printf("\n  message: ");
        sexp_write(ctx, sexp_exception_message(result), sexp_current_output_port(ctx));
        printf("\n  irritants: ");
        sexp_write(ctx, sexp_exception_irritants(result), sexp_current_output_port(ctx));
        printf("\n  procedure: ");
        sexp_write(ctx, sexp_exception_procedure(result), sexp_current_output_port(ctx));
        printf("\n  source: ");
        sexp_write(ctx, sexp_exception_source(result), sexp_current_output_port(ctx));
        printf("\n\n");
    }
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
