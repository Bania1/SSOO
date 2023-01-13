Semaforos

Semaforos binarios:

    1)Crear semaforo(varibale global):
    pthread_mutex_t semaforo;

    2)Inicializar semaforo(principio main):
    pthread_mutex_init(&semaforo, NULL);

    3)Cerrar semaforo(antes seccion critica)
    pthread_mutex_lok(&semaforo);

    4)Abrir semaforo(despues seccion critica):
    pthread_mutex_unlock(&semaforo);

    5)Eliminar semaforo(final main):
    pthread_mutex_destroy(&semaforo)
    