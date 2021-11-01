# Introducción
El ecosistema de herramientas para el lenguaje C++ es uno maduro, y con gran variedad. Sin embargo, en el área de los Sistemas de Construcción (Build Systems), es uno que palidece con respecto a las opciones que podemos encontrar en otros ecosistemas de lenguajes, como Gradle o Node Package Manager.

Los principales problemas que surgen a partir de los build system tradicionales son:

- Bajo nivel de abstracción: Proveen una poca capacidad de abstracción sobre el proceso de compilación. Esto provoca que el desarrollador tenga que invertir grandes cantidades de tiempo para corregir los archivos de definición.
- Poca o nula gestión de paquetes: Esto provoca que el desarrollador tenga que descargar, compilar e instalar paquetes externos de librerías de forma separada.
- Poco soporte para múltiples compiladores y targets: En CMake, el desarrollador debe configurar (llámese, ubicar dependencias externas, establecer opciones de compilación, etc) varias veces la compilación de un mismo paquete cada vez que quiere y/o debe usar un compilador distinto, o el mismo compilador con diferentes configuraciones.
Es por esto que nace “borc”, una alternativa que toma inspiración de las mejores características de los gestores de compilación de otras plataformas para remediar la situación antes descrita.

# Problemas de otras herramientas de construcción a Resolver
Los problemas que se encuentran en estos build systems son:
1. Los DSL’s para definir los modulos a compilar, son por definición turing-complete: Esto, si bien permite una gran flexibilidad, en la práctica conduce a complejos archivos para definir proyectos no-triviales.
2. Gestión de las dependencias: CMake tiene un sistema de librerias/paquetes, que solo está definido para referenciar dependencias locales. Los proyectos deben definir muchas veces procesos manuales para “automatizar” en parte este problema. Boost, por ejemplo, debe ser compilado y descargado de forma manual.
3. Builds manuales: Para agregar un nuevo compilador / tipo de construcción, en CMake, se debe reproducir el proceso de configuración desde 0, definiendo manualmente los paths hacia las dependencias externas. Peor aún si un proyecto está definiendo multiples arquitecturas y compilación.
4. Pipelines personalizados de compilación: Algunos proyectos (como videojuegos, o interfaces gráficas como Qt), requieren pipelines de compilación personalizados para compilar archivos fuente que no corresponden a los estándares de C++. Estos frameworks proveen compiladores adicionales para que se puedan conectar al pipeline de compilación. Sin embargo, para crear estos complementos, es necesario introducirse en la arcana definición para crear reglas de compilación.
5. Dificultad/manualidad durante el proceso de desarrollo de los componentes: Durante los procesos iterativos de desarrollo, se van creando diversos módulos que, posteriormente, es necesario refactorizar código existente en nuevos módulos con funcionalidades más refinadas. Esto implica un cierto esfuerzo de trabajo en crear y mantener las reglas de definición de los componentes cuando podría ser automatizado. Lo mismo aplica para crear proyectos desde 0: Es necesario crear la estructura de los proyectos desde 0 cuando podría ser generada automáticamente en base ciertos parámetros base que ingresa el usuario (de forma similar a como lo hace el tooling de Angular)
6. Interactividad en techstacks cross-lenguaje: Crear proyectos cross-lenguaje se torna complicado cuando el sistema de construcción base no entiende su lenguaje primario, debiendo crear hacks y forzando la compilació.
7. Incapacidad de ejecutar componentes que dependen de bibliotecas dinamicas: En CMake, es muy engorroso ejecutar componentes que dependen de librerias dinamicas dentro del mismo proyecto, por lo que es necesario realizar un proceso manual con las variables de entorno PATH para lograrlo.
8. Integración con sistemas de construcción pre-existentes: CMake es el estándar defacto para las herramientas de construcción de aplicaciones C++ en la actualidad.

# Visión
BORC debe ser una herramienta que permite reducir al máximo el tiempo y esfuerzo que el desarrollador invierte en el sistema de construcción/compilación de sus soluciones de software, facilitando, automatizando y acelerando (según donde sea aplicable) las siguientes actividades (que mapean a funcionalidades del Producto):
- Definición de las reglas de construcción.
- Refactorización de la estructura física de la solución de software.
- Ubicación, descarga y construcción de las dependencias.
- Interoperación con otras sistemas de construcción (CMake es una prioridad!!).
- Personalización/extensibilidad del proceso de construcción
- Generación de Código (clases, patrones de diseño?).

# Criterios de diseño:
## Criterio general de diseño
Facilitar, automatizar y acelerar las actividades relacionadas con la definición y construcción de una solución de software, de tal forma que el tiempo que se quita el foco en el software sea mínimo (en todo sentido).
## Reglas de Construcción
Simple e intuitivas: Una persona que no conoce la sintaxis de las reglas de construcción debe ser capaz de poder entenderlas inmediatamente.
Preferir el estilo Declarativo sobre el Imperativo: Un estilo declarativo tiene la ventaja de ser más fácil de usar. Uno imperativo provee más flexibilidad.
Proveer un nivel de abstracción superior con respecto a los otros sistemas de construcción.


# Metodología de Desarrollo
Estas son algunas directrices a seguir a rajatabla en el proceso de diseño y desarrollo. El primer MVP (o versión 1.0) debe ser capaz de:
- Auto-hostearse!
- Compilar paquetes que contengan aplicaciones de consola y librerías, usando gcc y Visual C++, sobre Windows y Linux
- Descargar dependencias via HTTP y desde Git.
- Partir Monolítico, refactorizar gradualmente

La idea acá es cumplir con los focos en cada iteración de desarrollo. El board inicial contemplará las HdU que permitirán acercarnos a la versión 1.0. Al llegar a la versión 1.0 deberíamos tener varios módulos (a lo menos dos), los cuales se compongan por librerías y un ejecutable.

# Especificación Funcional
Esta publicación describe las interacciones entre el usuario y el sistema de software que nos encontramos construyendo. *Recordar que BORC debe solucionar el proceso de construcción de paquetes y la gestión de la instalación, desinstalación y actualización sus dependencias*.

## Análisis Requerimientos Funcionales

### Interfaz de Usuario
Recordemos que un paquete borc se define por un conjunto de reglas que describen la estructura de sus componentes y sus dependencias. Ahora bien, el usuario puede elegir con cuales toolchains construir, cuales son las arquitecturas y plataforma objetivos a las que puede apuntar (cross-compiling), y cual es el build-type actual.
Es por esto que *se debe diseñar una estructura de datos en la que almacenar las referencias a estos elementos, dentro del repositorio (ya que esto es particular para cada usuario que haga uso del paquete)*.
El usuario interactuará con la aplicación por una interfaz de línea de comandos, llamada “borc”, y que pondrá a disposición ciertos subcomandos, con los cuales se administrará indirectamente la estructura de datos antes dicha. *Cada subcomando debe ser una acción, ya que el desarrollador siempre trabaja enfocado en un workflow dentro de un tiempo determinado*. Entre los comandos identificados tenemos:

#### configure
Establece ciertas variables requeridas tanto por el paquete (para configurar su construcción con ciertas opciones), así como también las opciones requeridas por alguna dependencia directa (o posiblemente, indirecta, sujeto a evaluación funcional).

#### manage
Maneja las dependencias (descarga, incorporación, actualización, etc)versiones, las ubica en caso de ser externas), maneja los toolchains (agrega, elimina, comprueba, establece el actual), maneja los build-types (agrega, elimina debug, release, coverage).

#### build
Compila las dependencias, compila incrementalmente las fuentes asociadas a un paquete o modulo en particular de ese paquete.
NOTA: La compilación incremental compila únicamente una fuente si ésta o una de sus dependencias fue modificada después de la ultima compilación.

#### run
Ejecuta una aplicación del paquete, estableciendo las variables de entorno apropiadas para ubicar sus dependencias en tiempo de ejecución (en caso de que use alguna librería dinámica), por ejemplo.

### DSL Declarativo
Se utilizará un DSL del tipo declarativo, para especificar componentes de software, llamados “paquetes”, que constan de un conjunto de módulos, sus fuentes y dependencias (ya sean éstas compilables o preconstruidas). Usar un DSL declarativo tiene las siguientes ventajas:
Análisis estático.
Comprensible a primera vista.
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

#### Ejemplos pendientes
- Definir pruebas automatizadas, header y modulos
- Dependencias en tiempo de compilacion / en tiempo de ejecucion
- Dependencias publicas y privadas.
- Dependencias en librerias estáticas y dinamicas
- En donde definir el tipo de linkae contra la libreria estandar
- En donde definir el tipo de arquitectura y plataforma

## Análisis Requerimientos No Funcionales

### Lenguaje de las Reglas de Construcción
Las reglas de construcción se definirán usando un lenguaje declarativo, ya que con esto se facilita mucho la comprensión de los componentes de software. Entre los candidatos para esto tenemos:


# Modelo de Dominio de la Solución
La solución propuesta establece ciertas entidades que proveen un cierto nivel de abstracción sobre el modelo de construcción subyacente para un amplio tipo de aplicaciones.
Primero, se describirán a grandes rasgos los elementos, y luego, se concretizará a nivel de Entidades el modelo vía un diagrama, junto con sus propiedades y las relaciones entre ellos.

## Tiempos
Los “tiempos” son instantes en que se produce un cierto “workflow” desde el punto de vista de un Paquete (descrito más adelante). Identificamos los siguientes:
- Tiempo de Construcción. Momento en que un paquete es construido.
- Tiempo de Ejecución. El tiempo de ejecución es el tiempo en que un componente “ejecutable” es ejecutado a petición del desarrollador.

## Estructura de Paquetes
La unidad de trabajo equivalente a lo que podríamos considerar Proyecto, Workspace, o Solución es el Paquete, que corresponde a una colección cohesiva de Componentes interrelacionados. Estos componentes pueden corresponder a librerías, ejecutables, u otros tipos dependiendo de otras variables. Para que BORC pueda construir estos artefactos, depende de un Toolchain, proveído directamente por el entorno de desarrollo local del usuario, el cual debe corresponder y/o ser compatibles con la tecnología/lenguaje/framework usado en esos componentes. Estos artefactos se componen de uno o varios archivos de código fuente, y pueden estar escritos en código portable, o usando código específico de ciertas plataformas y/o arquitecturas. En este último caso, se debe especificar a nivel del Build System este soporte.

### File
Archivo que al ser usado como entrada en un Pipe de compilación, para generar otro archivo. Los File’s pueden provenir desde el disco local, siendo referenciados desde una ruta relativa o absoluta.

### Component
Es un artefacto generado durante el proceso de construcción. Un artefacto se compone de uno o varios archivos de código fuente, escritos en uno o varios lenguajes de programación, y tiene una serie de meta-datos asociados, tales como:
Nombre.
- Lenguaje(s) de programación usados.
- Listado de archivos de código fuente y/o directorios
- Tipo de Componente
- Dependencias (de paquetes y/o componentes)

### Package
Es una colección de componentes (ejecutables, librerías, etc), que conforman un paquete determinado.

## Dependencias
Para posibilitar el consumo de librerías externas, los artefactos pueden referenciar otros artefactos (librerías u otros). Estas dependencias externas pueden venir en las siguientes formas:
- Código fuente completo, usando BORC como Build System.
- Código fuente completo, usando otro Build System externo (por ejemplo, Makefiles, CMake, Meson, QMake)
- Paquete con artefactos precompilados, el cual se encuentra instalado previamente en el sistema de archivos local.
- Paquetes que proveen tanto librerías como ejecutables y que incorporan nuevos ajustes en la forma en que se construyen los paquetes. Estos pueden ser conocidos como Frameworks, se pueden importar otros componentes que provean un complemento externo al toolchain empleado (como ejemplo Qt, que tiene un compilador de interfaces de usuario y un Meta-Object-Compiler, para generar metadatos a partir de classes C++).

## Toolchain
- Tool: Herramienta de línea de comandos que se usa para proveer la transformación requerida por un pipe. Tiene parámetros de entrada para controlar parte del funcionamiento del proceso de construcción.
- Toolchain: Conjunto de Tools, que implementan todo (o una parte) de la infraestructura necesaria para construir paquetes.

## Build Pipeline
El Build Pipeline provee una capa de abstracción para la construcción de paquetes. Describe en terminos generales las distintas tuberías (entradas y salidas) para irlas conectando dependiendo de la operación.

### FileType
Patrón de expresión regular que permite identificar el tipo de un SourceFile a partir de su propiedad Name. Por ejemplo:
- CMake Build Definition: CMakeLists.txt, *.cmake
- C Header File: .h
- C++ Header File: .h, .hh, .hxx, .h++, .hpp
- etc.

### PipePin
TODO

### Pipe
Regla de mapeo que indica cómo se transforman Files correspondiente a un cierto FileType en otro tipo de Files. Estas reglas se definen como:
- Input: Determina el tipo de archivo requerido para que este Pipe se pueda disparar y la cardinalidad.
- Tool: La herramienta de línea de comandos que se ejecutará para realizar la transformación. Esta herramienta puede ser proveída por una Dependencia.
- ArgumentList: Listado de argumentos, que influencian el proceso para generar la salida.
- Output: Determina el tipo de archivo de destino que se genera cuando el Pipe es ejecutado, junto con la cardinalidad.

### Pipeline
Conjunto de Pipes que, en cooperación complementaria entre ellos, permiten construir conjuntos de componentes de software.
