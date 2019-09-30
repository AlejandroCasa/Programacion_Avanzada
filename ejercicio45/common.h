/* Datos comunes */


/* Definición de los nombres de los semáforos  */ 
#define SEM_1 1 
#define SEM_2 0
#define CLAVE_SHM ((key_t) 1001)

/* Estructura de datos que se comparte en la zona de memoria común */
typedef struct{
  int intento;
  int criterio;
}  datos;