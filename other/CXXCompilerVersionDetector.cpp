
#include <stdio.h>

struct compiler_t {
    const char *name;
    int version_major;
    int version_minor;
    int version_patch;
};

int main() {
    struct compiler_t compiler = {
        NULL, 0, 0, 0
    };

#if defined(__clang__)
    compiler.name = "clang";
    compiler.version_major = __clang_major__;
    compiler.version_minor = __clang_minor__;
    compiler.version_patch = __clang_patchlevel__;
#elif defined(__GNUC__)
    compiler.name = "gcc";
    compiler.version_major = __GNUC__;
    compiler.version_minor = __GNUC_MINOR__;
    compiler.version_patch = __GNUC_PATCHLEVEL__;
#endif

    if (compiler.name) {
        printf("%s-%d.%d.%d\n", compiler.name, compiler.version_major, compiler.version_minor, compiler.version_patch);
        
        return 0;
    } else {
        return 1;
    }
}
