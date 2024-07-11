#include "solicitar_instruccion.h"
#include "instrucciones_mem.h"

cod_instruccion obtener_codigo_instruccion(char *operacion) {
    if (strcmp(operacion, "SET") == 0) return SET;
    if (strcmp(operacion, "MOV_IN") == 0) return MOV_IN;
    if (strcmp(operacion, "MOV_OUT") == 0) return MOV_OUT;
    if (strcmp(operacion, "SUM") == 0) return SUM;
    if (strcmp(operacion, "SUB") == 0) return SUB;
    if (strcmp(operacion, "JNZ") == 0) return JNZ;
    if (strcmp(operacion, "RESIZE") == 0) return RESIZE;
    if (strcmp(operacion, "COPY_STRING") == 0) return COPY_STRING;
    if (strcmp(operacion, "WAIT") == 0) return WAIT;
    if (strcmp(operacion, "SIGNAL") == 0) return SIGNAL;
    if (strcmp(operacion, "IO_GEN_SLEEP") == 0) return IO_GEN_SLEEP;
    if (strcmp(operacion, "IO_STDIN_READ") == 0) return IO_STDIN_READ;
    if (strcmp(operacion, "IO_STDOUT_WRITE") == 0) return IO_STDOUT_WRITE;
    if (strcmp(operacion, "IO_FS_CREATE") == 0) return IO_FS_CREATE;
    if (strcmp(operacion, "IO_FS_DELETE") == 0) return IO_FS_DELETE;
    if (strcmp(operacion, "IO_FS_TRUNCATE") == 0) return IO_FS_TRUNCATE;
    if (strcmp(operacion, "IO_FS_WRITE") == 0) return IO_FS_WRITE;
    if (strcmp(operacion, "IO_FS_READ") == 0) return IO_FS_READ;
    if (strcmp(operacion, "EXIT") == 0) return EXIT;
    return UNKNOWN;
}
 
t_instruccion formular_instruccion(char* string){
    t_instruccion instruccion;
    //armo un array de string con cada palabra de la instruccion en modo string
    char** lineas = string_split(string, " ");

    //busco el codigo de instruccion a partir del elemento 0
    instruccion.codigo_instruccion = obtener_codigo_instruccion(lineas[0]);

    //si existe el parametro, lo copia en la estructura de la instruccion
    strncpy(instruccion.param1, lineas[1] ? lineas[1] : "", sizeof(instruccion.param1) - 1);
    strncpy(instruccion.param2, lineas[2] ? lineas[2] : "", sizeof(instruccion.param2) - 1);
    strncpy(instruccion.param3, lineas[3] ? lineas[3] : "", sizeof(instruccion.param3) - 1);
    strncpy(instruccion.param3, lineas[4] ? lineas[4] : "", sizeof(instruccion.param4) - 1);
    strncpy(instruccion.param3, lineas[5] ? lineas[5] : "", sizeof(instruccion.param5) - 1);

    instruccion.param1[sizeof(instruccion.param1) - 1] = '\0';
    instruccion.param2[sizeof(instruccion.param2) - 1] = '\0';
    instruccion.param3[sizeof(instruccion.param3) - 1] = '\0';
    instruccion.param4[sizeof(instruccion.param4) - 1] = '\0';
    instruccion.param5[sizeof(instruccion.param5) - 1] = '\0';

    for (int i = 0; lineas[i] != NULL; i++) {
        free(lineas[i]);
    }
    free(lineas);

    return instruccion;

}

t_instruccion solicitar_instruccion(u_int32_t direccion_instruccion){
       
       //envio direccion instruccion
    t_buffer *un_buffer = crear_buffer(); 
    agregar_uint32_a_buffer(un_buffer, direccion_instruccion);
    agregar_uint32_a_buffer(un_buffer, pcb->pid);
    t_paquete *paquete = crear_super_paquete(ENVIAR_INSTRUCCIONES, un_buffer);
    enviar_paquete(paquete, socket_memoria);
    eliminar_paquete(paquete);
    free(un_buffer);
        //recibir instruccion en foma de string
    t_buffer *otro_buffer = crear_buffer();
    char* string;
    int cod_op = recibir_operacion(socket_memoria);
    if (cod_op == RECIBIR_INSTRUCCION){
        otro_buffer=recibir_buffer(socket_memoria);
        string = atender_instrucciones(otro_buffer);
    }
    else{
        log_warning(cpu_logger, "Operacion desconocida: No es una instruccion");
        exit(EXIT_FAILURE);
    }
    destruir_buffer(otro_buffer);

    t_instruccion instruccion = formular_instruccion(string);
    free(string);

    return instruccion;

}
