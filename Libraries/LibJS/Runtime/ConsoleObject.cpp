/*
 * Copyright (c) 2020, Andreas Kling <kling@serenityos.org>
 * Copyright (c) 2020, Linus Groh <mail@linusgroh.de>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <AK/FlyString.h>
#include <AK/Function.h>
#include <LibJS/Interpreter.h>
#include <LibJS/Runtime/ConsoleObject.h>
#include <stdio.h>

namespace JS {

static void print_args(Interpreter& interpreter)
{
    for (size_t i = 0; i < interpreter.argument_count(); ++i) {
        printf("%s", interpreter.argument(i).to_string().characters());
        if (i != interpreter.argument_count() - 1)
            putchar(' ');
    }
    putchar('\n');
}

ConsoleObject::ConsoleObject()
{
    put_native_function("log", log);
    put_native_function("debug", debug);
    put_native_function("info", info);
    put_native_function("warn", warn);
    put_native_function("error", error);
    put_native_function("trace", trace);
}

ConsoleObject::~ConsoleObject()
{
}

Value ConsoleObject::log(Interpreter& interpreter)
{
    print_args(interpreter);
    return js_undefined();
}

Value ConsoleObject::debug(Interpreter& interpreter)
{
    printf("\033[36;1m");
    print_args(interpreter);
    printf("\033[0m");
    return js_undefined();
}

Value ConsoleObject::info(Interpreter& interpreter)
{
    print_args(interpreter);
    return js_undefined();
}

Value ConsoleObject::warn(Interpreter& interpreter)
{
    printf("\033[33;1m");
    print_args(interpreter);
    printf("\033[0m");
    return js_undefined();
}

Value ConsoleObject::error(Interpreter& interpreter)
{
    printf("\033[31;1m");
    print_args(interpreter);
    printf("\033[0m");
    return js_undefined();
}

Value ConsoleObject::trace(Interpreter& interpreter)
{
    print_args(interpreter);
    auto call_stack = interpreter.call_stack();
    // -2 to skip the console.trace() call frame
    for (ssize_t i = call_stack.size() - 2; i >= 0; --i) {
        auto function_name = call_stack[i].function_name;
        if (String(function_name).is_empty())
            function_name = "<anonymous>";
        printf("%s\n", function_name.characters());
    }
    return js_undefined();
}

}
