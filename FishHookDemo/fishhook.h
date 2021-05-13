// Copyright (c) 2013, Facebook, Inc.
// All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//   * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//   * Neither the name Facebook nor the names of its contributors may be used to
//     endorse or promote products derived from this software without specific
//     prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef fishhook_h
#define fishhook_h

#include <stddef.h>
#include <stdint.h>

#if !defined(FISHHOOK_EXPORT)
#define FISHHOOK_VISIBILITY __attribute__((visibility("hidden")))
#else
#define FISHHOOK_VISIBILITY __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
//extern "C"的主要作用就是为了能够正确实现C++代码调用其他C语言代码。加上extern "C"后，会指示编译器这部分代码按C语言（而不是C++）的方式进行编译。由于C++支持函数重载，因此编译器编译函数的过程中会将函数的参数类型也加到编译后的代码中，而不仅仅是函数名；而C语言并不支持函数重载，因此编译C语言代码的函数时不会带上函数的参数类型，一般只包括函数名。
//https://www.cnblogs.com/xiangtingshen/p/10980055.html
extern "C" {
#endif //__cplusplus

/*
 * A structure representing a particular intended rebinding from a symbol
 * name to its replacement
 */
struct rebinding {
  const char *name;//需要HOOK的函数名称，C字符串
  void *replacement;//新函数的地址
  void **replaced;//原始函数地址的指针！
};

/*
 * For each rebinding in rebindings, rebinds references to external, indirect
 * symbols with the specified name to instead point at replacement for each
 * image in the calling process as well as for all future images that are loaded
 * by the process. If rebind_functions is called more than once, the symbols to
 * rebind are added to the existing list of rebindings, and if a given symbol
 * is rebound more than once, the later rebinding will take precedence.
 */
FISHHOOK_VISIBILITY
int rebind_symbols(struct rebinding rebindings[], size_t rebindings_nel);

/*
 * Rebinds as above, but only in the specified image. The header should point
 * to the mach-o header, the slide should be the slide offset. Others as above.
 */
FISHHOOK_VISIBILITY
int rebind_symbols_image(void *header,
                         intptr_t slide,
                         struct rebinding rebindings[],
                         size_t rebindings_nel);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //fishhook_h

