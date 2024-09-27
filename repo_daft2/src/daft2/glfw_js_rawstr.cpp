// this is a copy from https://github.com/pongasoft/emscripten-glfw/blob/master/src/js/lib_emscripten_glfw3.js
// under the form of a raw C string

const char* glfw_js_code = R"(
window.emscripten_glfw3_impl = {
    $GLFW3__deps: ['$GL', '$stringToNewUTF8', 'free'],
    $GLFW3__postset: `
    // exports
    Module["requestFullscreen"] = (lockPointer, resizeCanvas) => { GLFW3.requestFullscreen(null, lockPointer, resizeCanvas); }
    Module["glfwGetWindow"] = (any) => { const ctx = GLFW3.findContext(any); return ctx ? ctx.glfwWindow : null; };
    Module["glfwGetCanvas"] = (any) => { const ctx = GLFW3.findContext(any); return ctx ? ctx.canvas : null; };
    Module["glfwGetCanvasSelector"] = (any) => { const ctx = GLFW3.findContext(any); return ctx ? ctx.selector : null; };
    Module["glfwMakeCanvasResizable"] = (any, resizableSelector, handleSelector) => { GLFW3.makeCanvasResizable(any, resizableSelector, handleSelector); };
    Module["glfwUnmakeCanvasResizable"] = (any) => { GLFW3.unmakeCanvasResizable(any); };
    Module["glfwRequestFullscreen"] = GLFW3.requestFullscreen;
    Module["glfwIsRuntimePlatformApple"] = () => { return _emscripten_glfw3_context_is_runtime_platform_apple() };
    `,

    $GLFW3: {
        fDestructors: [],
        fWindowContexts: null,
        fScaleMQL: null,
        fScaleChangeCallback: null,
        fWindowResizeCallback: null,
        fKeyboardCallback: null,
        fClipboardCallback: null,
        fRequestFullscreen: null,
        fDeferredActions: [],
        fContext: null,
        fCSSValues: null, // key is element, value is {property_name: property_value}
        fErrorCodes: {GLFW_INVALID_VALUE: 0x00010004, GLFW_PLATFORM_ERROR: 0x00010008},

        //! onError
        onError(errorCode, errorMessage) {
            if(GLFW3.fErrorHandler) {
                const ptr = stringToNewUTF8(errorMessage);
                {{{ makeDynCall('vip', 'GLFW3.fErrorHandler') }}}(GLFW3.fErrorCodes[errorCode], ptr);
                _free(ptr);
            }
        },

        //! onScaleChange
        onScaleChange() {
            if(GLFW3.fScaleChangeCallback) {
                {{{ makeDynCall('vp', 'GLFW3.fScaleChangeCallback') }}}(GLFW3.fContext);
            }
        },

        //! onWindowResize
        onWindowResize(glfwWindow, width, height) {
            if(GLFW3.fWindowResizeCallback) {
                {{{ makeDynCall('vppii', 'GLFW3.fWindowResizeCallback') }}}(GLFW3.fContext, glfwWindow, width, height);
            }
        },

        //! isAnyElementFocused
        isAnyElementFocused: () => {
            return document.activeElement !== document.body;
        },

        //! isAnyOtherElementFocused
        isAnyOtherElementFocused() {
            return GLFW3.isAnyElementFocused() && GLFW3.findContext(document.activeElement) == null;
        },

        //! onPaste
        onPaste(e) {
            if(!GLFW3.isAnyOtherElementFocused()) {
                e.preventDefault();
            }
            let clipboardData = e.clipboardData || window.clipboardData;
            let pastedData = clipboardData.getData('text/plain');
            if(pastedData !== '' && GLFW3.fClipboardCallback) {
                const pastedString = stringToNewUTF8(pastedData);
                {{{ makeDynCall('vppp', 'GLFW3.fClipboardCallback') }}}(GLFW3.fContext, pastedString, null);
                _free(pastedString);
            }
        },

        //! onCutOrCopy
        onCutOrCopy(e) {
            if(GLFW3.fClipboardCallback) {
                const windowSelection = window.getSelection();
                if(windowSelection && windowSelection.toString() !== '') {
                    const selection = stringToNewUTF8(windowSelection.toString());
                    {{{ makeDynCall('vppp', 'GLFW3.fClipboardCallback') }}}(GLFW3.fContext, selection, null);
                    _free(selection);
                } else {
                    if(!GLFW3.isAnyOtherElementFocused()) {
                        // this is to prevent the browser to beep on empty clipboard
                        e.clipboardData.setData('text/plain', ' ');
                        e.preventDefault();
                    }
                }
            }
        },

        // onMouseEvent
        onMouseEvent(e) {
            requestAnimationFrame(GLFW3.executeDeferredActions);
        },

        // onKeyboardEvent
        onKeyboardEvent(e) {
            if(e.type === 'keydown')
                requestAnimationFrame(GLFW3.executeDeferredActions);

            if(GLFW3.fKeyboardCallback)
            {
                const code = stringToNewUTF8(e.code);
                const key = stringToNewUTF8(e.key);
                // see https://developer.mozilla.org/en-US/docs/Web/API/UI_Events/Keyboard_event_key_values
                var codepoint = e.key.charCodeAt(0);
                if(codepoint < 0x7f && e.key.length > 1)
                    // case when eventKey is something like "Tab" (eventKey.charCodeAt(0) would be "T")
                    codepoint = 0;
                var modifierBits = 0;
                if(e.shiftKey) modifierBits |= 0x0001;
                if(e.ctrlKey) modifierBits |= 0x0002;
                if(e.altKey) modifierBits |= 0x0004;
                if(e.metaKey) modifierBits |= 0x0008;
                // if(e.getModifierState('CapsLock')) modifierBits |= 0x0010;
                // if(e.getModifierState('NumLock')) modifierBits |= 0x0020;
                if({{{ makeDynCall('ipippiii', 'GLFW3.fKeyboardCallback') }}}(GLFW3.fContext, e.type === 'keydown', code, key, e.repeat, codepoint, modifierBits))
                e.preventDefault();
                _free(key);
                _free(code);
            }
        },

    }, // $GLFW3


  //! emscripten_glfw3_context_get_now
  emscripten_glfw3_context_get_now: () => {
    return performance.now();
  },
};

//autoAddDeps(emscripten_glfw3_impl, '$GLFW3')
//mergeInto(LibraryManager.library, emscripten_glfw3_impl);


)";
