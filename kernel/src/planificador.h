#ifndef PLANIFICADOR_H_
#define PLANIFICADOR_H_

#include "gestor_kernel.h"

void cambiar_estado(t_pcb *pcb, estado_proceso nuevo_estado);
void dispatch_pcb(t_pcb *pcb);

void planificar();
void planificar_corto_plazo();
void planificar_largo_plazo();

void agregar_pcb(t_list *list, t_pcb *pcb, pthread_mutex_t *mutex);
void pasar_a_ready(t_pcb *pcb);
void pasar_a_ready_prioridad(t_pcb *pcb);
char *list_pids_to_string(t_list *list);
void block_pcb();
void new_pcb();
void exit_pcb();
void destroy_pcb(t_pcb* pcb);
void execute_pcb();
t_pcb *remover_pcb(t_list *list, pthread_mutex_t *mutex);
t_pcb *pcb_segun_algoritmo();

t_pcb *obtener_pcb_RR();
void manejar_quantum(void *void_arg);
t_pcb *obtener_pcb_VRR();

#endif