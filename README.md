Design Criteria
---

* Criteria 1
* Criteria 2


Functional Specification
---

This is a simple workflow:

```bash
$ git clone https://github.com/somespace/myproject.git
$ cd myproject 
$ bok build

There is no current configured toolchain. Hints:
    - use 'bok configure --toolchain <id> [--path <installPath>]' to associate the current project with a certain toolchain.
    - use 'bok toolchain --scan' to get a list of installed toolchains

$ bok configure --toolchain gcc

Using toolchain gcc-7.0.0, with current native architecture (x64).
Configured toolchains:

    [1] gcc-7.0.0 (x64)

```
