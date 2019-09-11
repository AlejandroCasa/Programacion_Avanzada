# Programacion_Avanzada
Trabajos de la asignatura Programación Avanzada del Master en Automática y Robótica de la Universidad Politécnica de Madrid.

Practica	1:	procesos	y	fork
Objetivos:
1. recuperar	datos	de	los	proceso	(usuario,	ID,	...)
2. funciones	fork y	execve.

La práctica 1 consistirá en practicas para obtener datos del proceso, con la función pid, y la creación de hijos con fork y execve.
Los 4 primeros programas son ejemplos proporcionados por la asignatura, mientras que el diseño del programa 5 se ha hecho siguiente las siguientes indicaciones:

Escribir un programa que crea DOS hijos. El programa deberá tener una variable Z.
Cada uno de los hijos deberá cambiar el valor de Z y imprimirlo en pantalla (usar
valores distinos!). Se comprobará así que el segmento TEXO se clona, pero el
segmento DATOS es distinto!
