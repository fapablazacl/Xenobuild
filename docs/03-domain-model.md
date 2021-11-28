

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
