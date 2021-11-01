# Objetivos
Especificar las características que debe poseer una herramienta destinada a apoyar en la construcción de proyectos de software en lenguaje C++, para facilitar la gestión de los distintos aspectos en la generación, construcción y empaquetamiento de proyectos C/C++ en distintas plataformas host y target.

El objetivo es que hacer que la integración de proyectos C++ con librerías externas sea lo más placentero posible respecto de la forma en que se maneja actualmente.

# Gestión de Dependencias
## Tipos de dependencias admitidas.
- Librerías estáticas.
- Librerías dinámicas
- SDKs
    - SDKs pueden proveer librerías, y ejecutables que sirven como compiladores para otros tipos de archivos (como el SDK de Vulkan, por ejemplo).

## Ubicación de las dependencias
- Ruta de la dependencia
  - Directorio Local
    - Se puede indicar una ruta absoluta
    - Se puede indicar una ruta relativa.
  - Repositorio de código Git
    - Se puede referenciar un {branch / tag / commit} especifico como parámetro de la descarga.
    - Se puede indicar una ruta relativa al repositorio Git. Esto es util en los repositorios con varios subproyectos (ej: Monorepo).
    - El repositorio Git puede requerir autenticación de Git.
      - Autenticación por usuario / pass
      - Autenticación por clave ssh
      - Autenticación por Key (depende del proveedor?).
  - Repositorio de código SVN.
    - Se puede hacer referencia a un release especifico para la integración.
    - El repositorio SVN puede requerir autenticación (Autenticación por usuario / pass)
  - Descarga de archivo por URL HTTP.
    - Se puede indicar una ruta parcial dentro del archivo comprimido. Esto es util para paquetes comprimidos que tengan multiples proyectos.
    - Servidor HTTP puede requerir autenticación (Autenticación con usuario / password)
  - Descarga de archivo por Servidor FTP.
    - Se puede usar una ruta parcial para indicar el archivo a descargar.
    - Servidor FTP puede requerir autenticación (Autenticación con usuario / password)
  - Consideración: Los paquetes comprimidos se pueden validar mediante el cómputo de un Hash.
  - Consideración: Los paquetes comprimidos requieren descompresión como etapa previa para obtener el código a compilar.

- Formatos de paquetes comprimidos, una vez obtenida la dependencia:
  - Formato tar.gz
  - Formato zip
  - Formato rar.
  - Directorio local

## Formas de integrar las dependencias, en etapa de construcción:
- Dependencia es código fuente.
  - Código fuente que requiere ser compilado.
  - Código fuente que no requiere ser compilado (header-only).
- Dependencia es código precompilado.
  - Compilado para uno o mas Triplets (Compilador, Arquitectura, Plataforma)
- Enlazado automático de dependencias al momento de Construir, Ejecutar y Empaquetar, de acuerdo al tipo de dependencia.

# Gestión de Construcciones
## Configuración de Targets
- Target plataforma Host 
- Target otras plataformas (cross-build).

## Configuración de BuildTypes
- Debug y Release
- Otros BuildTypes definidos por el usuario.

## Soporte para multiples Build-Systems
- Soporte para CMake.
- Soporte para Configure / Make.
- Soporte para Meson.

# Gestión de ejecuciones
# Gestión de empaquetamiento
# Gestión de publicación
# Misceláneo
- Wrappers de comandos (similar a npm run), para simplificar tareas rutinarias.
- Poder ejecutar una línea de comando especifica para ciertas tareas rutinarias, como linting, pruebas unitarias, análisis estatico, etc.
