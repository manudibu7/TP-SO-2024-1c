#include "consola.h"
#include "planificador.h"

void inicializar_consola()
{
    log_info(kernel_logger, "CONSOLA INICIALIZADA");
    char *leido;
    leido = readline("> Ingrese un comando: ");

    while (strcmp(leido, "\0") != 0)
    {
        if (!validar_instruccion(leido))
        {
            free(leido);
            leido = readline("> Ingrese un comando: ");
            continue; // Salta y continua con el resto de la iteracion
        }

        atender_instruccion(leido);
        free(leido);
        leido = readline("> Ingrese un comando: ");
    }
    free(leido);
}

bool validar_instruccion(char *leido)
{
    bool resultado_validacion = false;

    // TODO: VALIDAR PATH, PID y VALOR

    char **comando_consola = string_split(leido, " ");

    if (strcmp(comando_consola[0], "EJECUTAR_SCRIPT") == 0)
    {
        resultado_validacion = true;
    }
    else if (strcmp(comando_consola[0], "INICIAR_PROCESO") == 0)
    {
        resultado_validacion = true;
    }
    else if (strcmp(comando_consola[0], "FINALIZAR_PROCESO") == 0)
    {
        resultado_validacion = true;
    }
    else if (strcmp(comando_consola[0], "INICIAR_PLANIFICACION") == 0)
    {
        if (planif_iniciada == false)
        {
            resultado_validacion = true;
        }
        else
        {
            resultado_validacion = false;
            printf("La planificacion ya ha sido iniciada previamente \n");
        }
    }
    else if (strcmp(comando_consola[0], "DETENER_PLANIFICACION") == 0)
    {
        if (planif_iniciada == true)
        {
            resultado_validacion = true;
        }
        else
        {
            resultado_validacion = false;
            printf("La planificacion ya ha sido pausada previamente \n");
        }
    }
    else if (strcmp(comando_consola[0], "MULTIPROGRAMACION") == 0)
    {
        resultado_validacion = true;
    }
    else if (strcmp(comando_consola[0], "PROCESO_ESTADO") == 0)
    {
        resultado_validacion = true;
    }
    else
    {
        log_error(kernel_logger, "ERROR: Comando NO reconocido");
        resultado_validacion = false;
    }
    string_array_destroy(comando_consola);

    return resultado_validacion;
}

void atender_instruccion(char *leido)
{
    char **comando_consola = string_split(leido, " ");
    t_buffer *un_buffer = crear_buffer();

    if (strcmp(comando_consola[0], "EJECUTAR_SCRIPT") == 0)
    {
        // agregar_string_a_buffer(un_buffer, comando_consola[1]); // [path]
    }
    else if (strcmp(comando_consola[0], "INICIAR_PROCESO") == 0)
    {
        // CREAR PCB
        int pid = asignar_pid();
        t_pcb *pcb = crear_pcb(pid);
        
        // AGREGAR PCB A NEW
        log_info(kernel_logger, "Se crea el proceso %d en NEW", pid);
        agregar_pcb(cola_new, pcb, &mutex_cola_new);
        sem_post(&sem_new);

        // TOMAR PATH DE CONSOLA
        agregar_string_a_buffer(un_buffer, comando_consola[1]); // [path]
        char *path = extraer_string_de_buffer(un_buffer);
        destruir_buffer(un_buffer);

        // ENVIAR A MEMORIA PATH Y PID
        t_buffer *a_enviar = crear_buffer();
        agregar_string_a_buffer(a_enviar, path);
        agregar_uint32_a_buffer(a_enviar, pcb->pid);
        t_paquete *paquete = crear_super_paquete(CREAR_PROCESO, a_enviar);
        enviar_paquete(paquete, socket_conexion_memoria);
        eliminar_paquete(paquete);
    }
    else if (strcmp(comando_consola[0], "FINALIZAR_PROCESO") == 0)
    {
        // agregar_int_a_buffer(un_buffer, (intptr_t)comando_consola[1]); // [pid]
    }
    else if (strcmp(comando_consola[0], "DETENER_PLANIFICACION") == 0)
    {
        //  TODO
        planif_iniciada = false;
        printf("La planificacion ha sido pausada. \n");
    }
    else if (strcmp(comando_consola[0], "INICIAR_PLANIFICACION") == 0)
    {
        //  TODO
        planif_iniciada = true;
        planificar();
        printf("La planificacion ha sido iniciada. \n");
    }
    else if (strcmp(comando_consola[0], "MULTIPROGRAMACION") == 0)
    {
        size_t comando_length = strlen(comando_consola[1] + 1);
        char *comando = malloc(comando_length);
        // TODO: ACTUALIZAR VALOR MAXIMO DEL sem_multiprogramacion cuando se actualiza la var global
        strcpy(comando, comando_consola[1]); 
        grado_multiprogramacion = atoi(comando);
        printf("El grado de multiprogramacion ahora es de %d\n", grado_multiprogramacion);

        free(comando);
    }
    else if (strcmp(comando_consola[0], "PROCESO_ESTADO") == 0)
    {
        //   TODO
    }
}

int asignar_pid()
{
    int valor_pid;

    pthread_mutex_lock(&mutex_pid);
    valor_pid = identificador_pid;
    identificador_pid++;
    pthread_mutex_unlock(&mutex_pid);

    return valor_pid;
}

t_pcb *crear_pcb(int pid)
{
    // CREACION DE PCB CON VALORES EN 0

    t_pcb *pcb = malloc(sizeof(t_pcb));

    pcb->pid = pid;
    pcb->program_counter = 0;
    pcb->estado = NEW;
    pcb->motivo_block = NONE_BLOCK;
    pcb->motivo_exit = NONE_EXIT;
    pcb->quantum_remanente = 0;

    t_registros *registros = malloc(sizeof(t_registros));
    pcb->registros_cpu = registros;
    inicializar_registros_pcb(pcb);
    
    return pcb;
}

void inicializar_registros_pcb(t_pcb *pcb)
{

    pcb->registros_cpu->ax = 0;
    pcb->registros_cpu->bx = 0;
    pcb->registros_cpu->cx = 0;
    pcb->registros_cpu->dx = 0;
    pcb->registros_cpu->eax = 0;
    pcb->registros_cpu->ebx = 0;
    pcb->registros_cpu->ecx = 0;
    pcb->registros_cpu->edx = 0;
    pcb->registros_cpu->si = 0;
    pcb->registros_cpu->di = 0;
}