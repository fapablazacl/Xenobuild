
# Functional Specification
Esta publicación describe las interacciones entre el usuario y el sistema de software que nos encontramos construyendo. *Recordar que Xenobuild debe solucionar el proceso de construcción de paquetes y la gestión de la instalación, desinstalación y actualización sus dependencias*.

## Main Points
- Eases the build processes from the command line
- Manage CMake-based build configurations, locally in the source code locations
- Import CMake-based dependencies from Git repositories
- CLI Application, centered around subcommands

## Command-Line Interface (CLI)

Recordemos que un paquete borc se define por un conjunto de reglas que describen la estructura de sus componentes y sus dependencias. Ahora bien, el usuario puede elegir con cuales toolchains construir, cuales son las arquitecturas y plataforma objetivos a las que puede apuntar (cross-compiling), y cual es el build-type actual.
Es por esto que *se debe diseñar una estructura de datos en la que almacenar las referencias a estos elementos, dentro del repositorio (ya que esto es particular para cada usuario que haga uso del paquete)*.
El usuario interactuará con la aplicación por una interfaz de línea de comandos, llamada “borc”, y que pondrá a disposición ciertos subcomandos, con los cuales se administrará indirectamente la estructura de datos antes dicha. *Cada subcomando debe ser una acción, ya que el desarrollador siempre trabaja enfocado en un workflow dentro de un tiempo determinado*. Entre los comandos identificados tenemos:


1. Set of command line tools for ease the non-gui based development. For example:
  $ borc run test01
  $ borc build
  $ borc install

## Subcommands
### Configure
Establece ciertas variables requeridas tanto por el paquete (para configurar su construcción con ciertas opciones), así como también las opciones requeridas por alguna dependencia directa (o posiblemente, indirecta, sujeto a evaluación funcional).

### Manage
Maneja las dependencias (descarga, incorporación, actualización, etc)versiones, las ubica en caso de ser externas), maneja los toolchains (agrega, elimina, comprueba, establece el actual), maneja los build-types (agrega, elimina debug, release, coverage).

### Build
Compila las dependencias, compila incrementalmente las fuentes asociadas a un paquete o modulo en particular de ese paquete.
NOTA: La compilación incremental compila únicamente una fuente si ésta o una de sus dependencias fue modificada después de la ultima compilación.

  - Construction of software artifacts (binaries, documentation, installers, etc).

### Run 
Ejecuta una aplicación del paquete, estableciendo las variables de entorno apropiadas para ubicar sus dependencias en tiempo de ejecución (en caso de que use alguna librería dinámica), por ejemplo.

  - Run the specified executable. Add to the path the internal dependency DLLs/SOs/dylib

### Test
  - Run unit tests as part of the build process.

### Install

### Pack
  - Package the application in a setup / makefile, compress data files, etc.

## Workflows
### Simple Case:
  $ git clone https://github.com/somespace/myproject.git
  $ cd myproject 
  $ xb build

  There is no current configured toolchain. Hints:
      - use 'xb configure --toolchain <id> [--path <installPath>]' to associate the current project with a certain toolchain.
      - use 'xb toolchain --scan' to get a list of installed toolchains

  $ xb configure --toolchain gcc

  Using toolchain gcc-7.0.0, with current native architecture (x64).
  Configured toolchains:

      [1] gcc-7.0.0 (x64)

## Declarative DSL
Se utilizará un DSL del tipo declarativo, para especificar componentes de software, llamados “paquetes”, que constan de un conjunto de módulos, sus fuentes y dependencias (ya sean éstas compilables o preconstruidas). Usar un DSL declarativo tiene las siguientes ventajas:

- Análisis estático.
- Comprensible a primera vista.

Este DSL declarativo está definido, por ahora, en JSON. A continuación, tenemos un archivo de definición del paquete:

```yaml
name: “borc”
description: “Build system for C++, focused on software modularity and scability”
version: “1.0.0”
license: “MIT”
author: “Felipe Apablaza”
email: “ing.apablaza@gmail.com” 
definition_files:
	- “modules/borc/module.borc”
```

Las reglas de construcción en Borc son “modulares” y explícitas. Se deja en claro, directamente por medio de sus rutas, y por reglas de carga, que el paquete contiene un componente, definido en la ruta relativa “modules/borc/module.borc”. Borc cargará recursivamente todas las sentencias “definition_files” para ir incorporando nuevos componentes dentro del paquete.
El archivo de reglas para “src/borc/module.borc” es el siguiente:

```yaml
name: “borc”
description: “Build Orchestration Command Line Interface”
type: “application/console”
languages:
  - “c++/17”
sources:
  - “src/**”
dependencies:
  - “boost>=1.70.0/[filesystem, system, process, program_options]
  - “jbeder/yaml-cpp”
```

En este archivo de reglas, se dejan explícitas cuales son las dependencias para esta librería en particular.

### Pending Examples
- Definir pruebas automatizadas, header y modulos
- Dependencias en tiempo de compilacion / en tiempo de ejecucion
- Dependencias publicas y privadas.
- Dependencias en librerias estáticas y dinamicas
- En donde definir el tipo de linkae contra la libreria estandar
- En donde definir el tipo de arquitectura y plataforma


### Other Examples for CLI Usage

    Project "Xenobuild" prototype.



  Concepts:
    project: The software project, composed of many 'components'
    component: A software component. Can be a component of the project, or be a external library.

  Features:
  I want this to have several operation modes.

  * Build Mode:

  $ Xenobuild build

  On the Build Mode the tool acts like a make tool: It will generate all the build artifacts (binaries, documentation, installers, etc).
  Whenever the user invokes the tool in build mode, it will make a hidden directory at the project root folder, called '.Xenobuild',
  with the same structure as the source files. For example, let's suppose the following project structure:

  myproject
    myexecutable
      include
        myexe.h
        module.h
      src
        myexe.c
        module.c
    mylibrary
      include
        mylib.h
      src
        mylib.c

  The build artifacts will have the following folder structure:
    .Xenobuild
      myexecutable
        myexecutable (*)
        src
          myexe.c.obj
          module.c.obj
      mylibrary
        mylibrary (*)
        src
          mylib.c.obj

  This has to be as shown like this because this way we will avoid file name collision cross language.
  This behaviour can't be configured because we want the tool simple and fast.

  * Debug Mode.
  On the build mode, the tool will execute the specified executable component. This will be like this because:
    - We want to select the debugger
    - The executable maybe have software library dependencies, constructed in the same

  $ Xenobuild debug myexecutable

  * Generate Mode
      Prototype in Python, but final version must be in C.
      Support for multiple toolchains.
      Support for multiple programming languages in the same project.
      Support for dependencies (libraries and another projects).
      Syntax based on YAML



  other notes:

  Constructor de Software multi lenguaje, multi plataforma.

  'mula' es una herramienta de linea de comandos que se encarga de gestionar todo el proceso de construccion de proyectos de software. Esto es:

  De forma similar a CMake,

  Ejemplos de uso:

  Construccion

  $ cd felide
  $ mula build

  Building software components  ...
  Resolving software components in project felide... OK
  Building xe.sys ... OK
  Building libfelide ... OK
  Building felide ... OK
  Building unit testings ... OK

  Running unit testings ... OK
  Generating dev documentation ... OK.
  Generating user documentation ... OK
  Packaging

  Build done in %d seconds.

  # En este punto, el proyecto esta completamente construido.

  Ejecucion

  $ mula run --nodebug
  Hello, World!

  # ejecuta el proyecto, agregando a una lista de paths temporales asociadas al proceso ejecutable para que no tenga con las dependencias dinamicas.
  # estas dependencias deben estar primero que las paths del sistema, para asi tomar siempre la version que esta siendo actualmente desarrollada.
  # La idea es que sea como una familia de comandos, de forma similar a como opera git.

  Generadores

  Los generadores son entidades que generan scripts de instalacion para distintas plataformas y sistemas operativos. La idea es que estas soportes compilacion cruzada, y no dependendan de que 'mula' este instalado en el sistema del cliente.

  Ejemplo:

  $ mula gen --makefile

    Generating configure and makefiles for Linux ...
    Generating configure and makefiles for Windows ...
    Generating configure and makefiles for OS X ...
    Generating configure and makefiles for Android ...
    Done.



  Por defecto, se generaran makefiles para la plataforma actual.

  Para generar makefiles para todas las plataformas soportadas:

  $ mula gen --makefile --all

      Generating installation scripts for all supported platforms ...

  $ mula init

    Scanning possible


  Compatibilidad multilenguaje

  Xenobuild tiene la gran ventaja de que soporta multiples lenguajes de programacion en un mismo proyecto. Por ejemplo, supongamos que tenemos la siguiente estructura de datos:


  multiproj
      multiproj.Xenobuild

      clib
          include
              pool.h
              vector.h
              mesh.h
          src
              pool.c
              vector.c
              mesh.c
      jclib
          src
              Pool.java
              Vector.java
              Mesh.java
      jario
          src
              Jario.java


  Los proyectos seran construidos de la siguiente manera:

  $ mula build
      Building software components  ...
  Resolving software components in project multiproj... OK
  Building clib ... OK
  Building jclib ... OK
  Building jario ... OK

  $ Xenobuild list
  clib : C Library.
  jclib: Java Class Library
  jario: Java Application.

  $ Xenobuild run

### Yet Another Old CLI Usage Example
  BORC Description Document
  Version 1.0

  BORC Is a suite of tools (binaries and libraries) for build software products,

  based on a project description file.

  ==============================
  1. A Simple Example
  ==============================

  Let's consider the following directory hierarchy:

      ConsoleApp01/
        borcfile.xml
        ConsoleApp01.cpp

  Listing 1.1: borcfile.xml content

      <borc:project name="ConsoleApp01">

          <borc:program name="ConsoleApp01" language="C++11">
              <!-- all paths are relative -->
              <borc:directory path="." />
          </borc:program>

      </borc:project>


  In order to build this project, one could enter the following command:

      $ borc build

      Scanning Components ...
      Building C++11 Program 'ConsoleApp01' ...
      Done.


  Now the folder hierarchy looks like this:

      ConsoleApp01/
        borcfile.xml
        ConsoleApp01.cpp
        .borc/
          .borcmetadata
          gcc-unknown-x86_64/
            ConsoleApp01[.exe]
            ConsoleApp01.cpp.obj

  ==============================
  2. A Program with a Library

  ==============================
  Now, consider a program that computes the distance between two 2D points via
  the command line, with the following directory hierarchy:

      App02/
        borcfile.xml
        MathLib/
          MathLib.cpp
          MathLib.hpp
        App02
          App02.cpp


  Listing 2.1: MathLib.hpp content

      #ifndef __mathlib_hpp__
      #define __mathlib_hpp__

      namespace MathLib {
          extern float distance(float x1, float y1, float x2, float y2);
      }

      #endif

  Listing 2.2: App02.cpp content:

      #include <iostream>
      #include "MathLib/MathLib.hpp"

      using MathLib::distance;

      int main(int argc, char **argv) {
          if (argc < 5) {
              std::cout << "Usage: " << argv[0] << "x1 y1 x2 y2" << std::endl;
          }

          const float x1 = std::stof(argv[1]);
          const float y1 = std::stof(argv[2]);
          const float x2 = std::stof(argv[3]);
          const float y2 = std::stof(argv[4]);

          std::cout << distance(x1, y1, x2, y2);

          return 0;
      }


  The program 'App02' uses the library 'MathLib'

