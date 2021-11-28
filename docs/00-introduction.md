
# Introduction

El ecosistema de herramientas para el lenguaje C++ es uno maduro, y con gran variedad. Sin embargo, en el área de los Sistemas de Construcción (Build Systems), es uno que palidece con respecto a las opciones que podemos encontrar en otros ecosistemas de lenguajes, como Gradle o Node Package Manager.


## Main Pain-Points
Los principales problemas (or pain-points) que surgen a partir de los build system tradicionales son:

- Bajo nivel de abstracción: Proveen una poca capacidad de abstracción sobre el proceso de compilación. Esto provoca que el desarrollador tenga que invertir grandes cantidades de tiempo para corregir los archivos de definición.
- Dependency Managemen: Poca o nula gestión de paquetes: Esto provoca que el desarrollador tenga que descargar, compilar e instalar paquetes externos de librerías de forma separada.
- Poco soporte para múltiples compiladores y targets: En CMake, el desarrollador debe configurar (llámese, ubicar dependencias externas, establecer opciones de compilación, etc) varias veces la compilación de un mismo paquete cada vez que quiere y/o debe usar un compilador distinto, o el mismo compilador con diferentes configuraciones.
- Some of the mainstream build systems are too complex, meaning that the user must learn a new programming language.

Es por esto que nace “borc”, una alternativa que toma inspiración de las mejores características de los gestores de compilación de otras plataformas para remediar la situación antes descrita.

## Problems found in other Build Tools
1. Los DSL’s para definir los modulos a compilar, son por definición turing-complete: Esto, si bien permite una gran flexibilidad, en la práctica conduce a complejos archivos para definir proyectos no-triviales.
2. Gestión de las dependencias: CMake tiene un sistema de librerias/paquetes, que solo está definido para referenciar dependencias locales. Los proyectos deben definir muchas veces procesos manuales para “automatizar” en parte este problema. Boost, por ejemplo, debe ser compilado y descargado de forma manual.
3. Builds manuales: Para agregar un nuevo compilador / tipo de construcción, en CMake, se debe reproducir el proceso de configuración desde 0, definiendo manualmente los paths hacia las dependencias externas. Peor aún si un proyecto está definiendo multiples arquitecturas y compilación.
4. Pipelines personalizados de compilación: Algunos proyectos (como videojuegos, o interfaces gráficas como Qt), requieren pipelines de compilación personalizados para compilar archivos fuente que no corresponden a los estándares de C++. Estos frameworks proveen compiladores adicionales para que se puedan conectar al pipeline de compilación. Sin embargo, para crear estos complementos, es necesario introducirse en la arcana definición para crear reglas de compilación.
5. Dificultad/manualidad durante el proceso de desarrollo de los componentes: Durante los procesos iterativos de desarrollo, se van creando diversos módulos que, posteriormente, es necesario refactorizar código existente en nuevos módulos con funcionalidades más refinadas. Esto implica un cierto esfuerzo de trabajo en crear y mantener las reglas de definición de los componentes cuando podría ser automatizado. Lo mismo aplica para crear proyectos desde 0: Es necesario crear la estructura de los proyectos desde 0 cuando podría ser generada automáticamente en base ciertos parámetros base que ingresa el usuario (de forma similar a como lo hace el tooling de Angular)
6. Interactividad en techstacks cross-lenguaje: Crear proyectos cross-lenguaje se torna complicado cuando el sistema de construcción base no entiende su lenguaje primario, debiendo crear hacks y forzando la compilació.
7. Incapacidad de ejecutar componentes que dependen de bibliotecas dinamicas: En CMake, es muy engorroso ejecutar componentes que dependen de librerias dinamicas dentro del mismo proyecto, por lo que es necesario realizar un proceso manual con las variables de entorno PATH para lograrlo.
8. Integración con sistemas de construcción pre-existentes: CMake es el estándar defacto para las herramientas de construcción de aplicaciones C++ en la actualidad.


## Vision
Xenobuild debe ser una herramienta que permite reducir al máximo el tiempo y esfuerzo que el desarrollador invierte en el sistema de construcción/compilación de sus soluciones de software, facilitando, automatizando y acelerando (según donde sea aplicable) las siguientes actividades (que mapean a funcionalidades del Producto):
- Definición de las reglas de construcción.
- Refactorización de la estructura física de la solución de software.
- Ubicación, descarga y construcción de las dependencias.
- Interoperación con otras sistemas de construcción (CMake es una prioridad!!).
- Personalización/extensibilidad del proceso de construcción
- Generación de Código (clases, patrones de diseño?).

## Design Criteria
### Criterio general de diseño
Facilitar, automatizar y acelerar las actividades relacionadas con la definición y construcción de una solución de software, de tal forma que el tiempo que se quita el foco en el software sea mínimo (en todo sentido).
### Reglas de Construcción
Simple e intuitivas: Una persona que no conoce la sintaxis de las reglas de construcción debe ser capaz de poder entenderlas inmediatamente.
Preferir el estilo Declarativo sobre el Imperativo: Un estilo declarativo tiene la ventaja de ser más fácil de usar. Uno imperativo provee más flexibilidad.
Proveer un nivel de abstracción superior con respecto a los otros sistemas de construcción.

## Development Methodology
Estas son algunas directrices a seguir a rajatabla en el proceso de diseño y desarrollo. El primer MVP (o versión 1.0) debe ser capaz de:
- Auto-hostearse!
- Compilar paquetes que contengan aplicaciones de consola y librerías, usando gcc y Visual C++, sobre Windows y Linux
- Descargar dependencias via HTTP y desde Git.
- Partir Monolítico, refactorizar gradualmente

La idea acá es cumplir con los focos en cada iteración de desarrollo. El board inicial contemplará las HdU que permitirán acercarnos a la versión 1.0. Al llegar a la versión 1.0 deberíamos tener varios módulos (a lo menos dos), los cuales se compongan por librerías y un ejecutable.
