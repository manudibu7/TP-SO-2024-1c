#include "instrucciones.h"

void* obtener_registro(char registro[20]) {
    if (strcmp(registro, "AX") == 0) {
        return &(pcb->registros_cpu->ax);
    } else if (strcmp(registro, "BX") == 0) {
        return &(pcb->registros_cpu->bx);
    } else if (strcmp(registro, "CX") == 0) {
        return &(pcb->registros_cpu->cx);
    } else if (strcmp(registro, "DX") == 0) {
        return &(pcb->registros_cpu->dx);
    } else if (strcmp(registro, "EAX") == 0) {
        return &(pcb->registros_cpu->eax);
    } else if (strcmp(registro, "EBX") == 0) {
        return &(pcb->registros_cpu->ebx);
    } else if (strcmp(registro, "ECX") == 0) {
        return &(pcb->registros_cpu->ecx);
    } else if (strcmp(registro, "EDX") == 0) {
        return &(pcb->registros_cpu->edx);
    } else if (strcmp(registro, "SI") == 0) {
        return &(pcb->registros_cpu->si);
    } else if (strcmp(registro, "DI") == 0) {
        return &(pcb->registros_cpu->di);
    } else {
        return NULL;
    }
}

//ATOI PARA CHAR[]->INT
void ejecutar_set(char registro[20], char valor[20]) {
    if (strcmp(registro, "AX") == 0) {
        pcb->registros_cpu->ax = (uint8_t)atoi(valor);
    } else if (strcmp(registro, "BX") == 0) {
        pcb->registros_cpu->bx = (uint8_t)atoi(valor);
    } else if (strcmp(registro, "CX") == 0) {
        pcb->registros_cpu->cx = (uint8_t)atoi(valor);
    } else if (strcmp(registro, "DX") == 0) {
        pcb->registros_cpu->dx = (uint8_t)atoi(valor);
    } else if (strcmp(registro, "EAX") == 0) {
        pcb->registros_cpu->eax = (uint32_t)atoi(valor);
    } else if (strcmp(registro, "EBX") == 0) {
        pcb->registros_cpu->ebx = (uint32_t)atoi(valor);
    } else if (strcmp(registro, "ECX") == 0) {
        pcb->registros_cpu->ecx = (uint32_t)atoi(valor);
    } else if (strcmp(registro, "EDX") == 0) {
        pcb->registros_cpu->edx = (uint32_t)atoi(valor);
    } else if (strcmp(registro, "SI") == 0) {
        pcb->registros_cpu->si = (uint32_t)atoi(valor);
    } else if (strcmp(registro, "DI") == 0) {
        pcb->registros_cpu->di = (uint32_t)atoi(valor);
    } else {
        log_error(cpu_logger, "No se hallo el registro");
    } 
}

//SUM
void ejecutar_sum(char registro_destino[20] , char registro_origen[20] ){
    void* puntero_destino = obtener_registro(registro_destino); //void* : direccion de ALGO, 
    void* puntero_origen = obtener_registro(registro_origen);


    if (puntero_destino != NULL && puntero_origen != NULL) {
        //determino si los registros son de 1 o 4 bytes
        if (strcmp(registro_destino, "AX") == 0 || strcmp(registro_destino, "BX") == 0 ||
            strcmp(registro_destino, "CX") == 0 || strcmp(registro_destino, "DX") == 0) {
            *(uint8_t*)puntero_destino += *(uint8_t*)puntero_origen;
        } else {
            *(uint32_t*)puntero_destino += *(uint32_t*)puntero_origen;
        }
    } else {
        log_error(cpu_logger, "Registro invalido");
    }
}


//SUB
void ejecutar_sub(char registro_destino[20] , char registro_origen[20] ){
        void* puntero_destino = obtener_registro(registro_destino);
    void* puntero_origen = obtener_registro(registro_origen);


    if (puntero_destino != NULL && puntero_origen != NULL) {
        //determino si los registros son de 1 o 4 bytes
        if (strcmp(registro_destino, "AX") == 0 || strcmp(registro_destino, "BX") == 0 ||
            strcmp(registro_destino, "CX") == 0 || strcmp(registro_destino, "DX") == 0) {
            *(uint8_t*)puntero_destino -= *(uint8_t*)puntero_origen;
        } else {
            *(uint32_t*)puntero_destino -= *(uint32_t*)puntero_origen;
        }
    } else {
        log_error(cpu_logger, "Registro invalido");
    }
}


//JNZ
void ejecutar_jnz(char registro[20] , char instruccion[20]){
     void* puntero_registro = obtener_registro(registro);
     uint32_t nuevo_ip = atoi (instruccion);

      if (puntero_registro != NULL) {
        if (strcmp(registro, "AX") == 0 || strcmp(registro, "BX") == 0 ||
            strcmp(registro, "CX") == 0 || strcmp(registro, "DX") == 0) {
            if (*(uint8_t*)puntero_registro != 0) {
                pcb->program_counter = nuevo_ip;
            }
        } else {
            if (*(uint32_t*)puntero_registro != 0) {
                pcb->program_counter = nuevo_ip;
            }
        }
    } else {
        log_error(cpu_logger, "Registro invalido");
    }
}

//IO_GEN_SLEEP
void ejecutar_io_gen_sleep(char interfaz[20] , char unidades_de_trabajo[20] ){
    t_buffer *un_buffer = crear_buffer();
    t_paquete *paquete = crear_super_paquete(OP_IO_GEN_SLEEP, un_buffer);
    agregar_uint32_a_buffer(un_buffer, atoi(unidades_de_trabajo));
    agregar_string_a_buffer(un_buffer, interfaz);

    enviar_paquete(paquete, socket_kernel_dispatch);
    eliminar_paquete(paquete);
    destruir_buffer(un_buffer);
}

//EXIT
void ejecutar_exit (){
    t_buffer *buffer = crear_buffer();
    agregar_pcb_a_buffer(buffer, pcb);
    agregar_cop_a_buffer(buffer, CAMBIAR_ESTADO);
    agregar_estado_a_buffer(buffer, FINISH_EXIT);
    t_paquete *paquete = crear_super_paquete(ENVIO_PCB, buffer);
    enviar_paquete(paquete, socket_kernel_dispatch);
    eliminar_paquete(paquete);
    destruir_buffer(buffer);
}


//RESIZE
void ejecutar_resize(char tamanio [20]){
    t_buffer *un_buffer = crear_buffer();
    agregar_int_a_buffer(un_buffer, atoi(tamanio));
    agregar_uint32_a_buffer(un_buffer, pcb->pid);

    t_paquete *paquete = crear_super_paquete(AJUSTAR_TAMANIO, un_buffer);
    enviar_paquete(paquete, socket_memoria);

    eliminar_paquete(paquete);
    destruir_buffer(un_buffer);

    atender_memoria();
}

void procesar_resultado_resize(char* resultado){
    if(strcmp(resultado,"Out of Memory")){
        pcb->motivo_exit = OUT_OF_MEMORY;
        t_buffer *buffer = crear_buffer();
        agregar_pcb_a_buffer(buffer, pcb);
        agregar_cop_a_buffer(buffer, CAMBIAR_ESTADO);
        agregar_estado_a_buffer(buffer, FINISH_ERROR);

        t_paquete *paquete = crear_super_paquete(ENVIO_PCB, buffer);
        enviar_paquete(paquete, socket_kernel_dispatch);

        eliminar_paquete(paquete);

    }else if(strcmp(resultado,"Ok")){
        log_info(cpu_logger,"RESIZE exitoso");
    } else{
        log_error(cpu_logger, "Registro invalido");
    }
}

/* void ejecutar_copy_string(instruccion.param1){

} */

//IO_STDIN_READ
void ejecutar_io_stdin_read(char interfaz[20], char reg_dir_logica[20], char reg_tam [20]){
    void* dir_logica = obtener_registro(reg_dir_logica); //las dl son de 32
    uint32_t direccion_logica = *(uint32_t*)dir_logica; //hago void* -> uint32_t*; y *(uint32_t*) para obtener el valor almacenado en la dirección apuntada
    uint32_t direccion_fisica =  traducir_direccion_logica(direccion_logica);

    void* dir_tam = obtener_registro(reg_tam);

    t_buffer *un_buffer = crear_buffer();
    agregar_string_a_buffer(un_buffer,interfaz);
    agregar_uint32_a_buffer(un_buffer,direccion_fisica);
    t_paquete *paquete = crear_super_paquete(OP_IO_STDIN_READ, un_buffer);

    if (dir_tam != NULL) {
        if (strcmp(reg_tam, "AX") == 0 || strcmp(reg_tam, "BX") == 0 ||
            strcmp(reg_tam, "CX") == 0 || strcmp(reg_tam, "DX") == 0) {
                uint8_t tamanio = *(uint8_t*) dir_tam;
                agregar_uint8_a_buffer(un_buffer, tamanio);

        } else {
                uint32_t tamanio = *(uint32_t*) dir_tam;
                agregar_uint32_a_buffer(un_buffer, tamanio);
            } 
        }
        enviar_paquete(paquete, socket_kernel_dispatch);
        eliminar_paquete(paquete);
}







