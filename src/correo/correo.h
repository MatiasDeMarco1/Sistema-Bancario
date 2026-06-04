#ifndef CORREO_H
#define CORREO_H

#include "config.h"

// Envia un mail con libcurl usando la config SMTP.
// Devuelve 1 si se envio bien, 0 si fallo.
//   cfg:       credenciales ya cargadas
//   destino:   mail del destinatario
//   asunto:    asunto del mail
//   cuerpo:    texto del mensaje
int enviarCorreo(const ConfigSMTP *cfg, const char *destino,
                 const char *asunto, const char *cuerpo);

#endif