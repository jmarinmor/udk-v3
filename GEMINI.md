# uncannymoon
El propósito final de uncannymoon es ser un motor para desarrollar videojuegos o aplicaciones basadas
en generación de gráficos en tiempo real. También debería poder servir como librería de enlace dinámico 
o estático para poderla incorporar a proyectos.

## Instrucciones
- El código debe ser crossplatform porque debe funcionar en: Windows, MAC, Linux, Android, iOS, iOS Simulator entre otros
- Usa cmake
- El código core sera desarrollado en C/C++
- Puede usar vulkan, opengl, directx, openal, opencl, metal, ... El desarrollo principal se basará en vulkan y opengl
- Tiene que tener bindings para usar la librería en proyectos hechos en Java, C#, Swift y python
- Usa entre otras librerias: assimp, opensl, glad, glfw, freetype

## Arquitectura del sistema
- Hay un código principal (core)
- Tiene una fuerte orientación al uso de interfaces para soportar diferentes adapters
- Es posible que sea necesario usar archivos en C para poder generar los bindings a otros lenguajes

## Reglas de negocio
- Es importante poder generar con cmake proyectos (librería, ejemplos, tests, ...) para poder debugear el código


## Estructura del proyecto
- ./core # Codigo principal en C++
- ./adapters # Código C++ para implementar las diferentes adaptaciones de las interfaces que hay en core
- ./binginds # Código para generar los bindings para que el motor pueda ser usado en diferentes lenguajes
- ./bindings/c # Aquí estará el código en C que permitirá crear bindings a otros lenguajes
- ./bindings/java # Aquí estará el código que se tiene que generar en Java que atacará al de c. Lo mismo para los otros lenguajes como C#, python, ...
- ./dependencies # Aquí se pondrá si es necesario las librerías de terceros que se tengan que compilar manualmente y no puedan encontrarse ni el el sistema ni en cmake
- ./project # En este directorio se encontrará el cmake principal para compilar core y adapters
- ./examples # Aquí irán los diferentes proyectos de ejemplos en C++ para poder usar core y adapters.
- ./examples_bindings # Ejemplos usando otros lenguajes
- ./tests # Aquí van los unit test
