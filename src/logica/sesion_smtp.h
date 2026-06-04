#ifndef SESION_SMTP_H
#define SESION_SMTP_H

#include "config.h"

// Config SMTP global, cargada una sola vez al inicio del programa.
extern ConfigSMTP g_config_smtp;

// Carga la config SMTP (llamar una vez en el main, antes de usar el correo).
void inicializarSMTP(void);

#endif