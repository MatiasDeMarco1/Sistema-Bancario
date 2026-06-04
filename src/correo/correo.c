#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include "correo.h"

// libcurl pide el cuerpo del mail a traves de un "lector" que va
// entregando el texto de a pedazos. Usamos esta estructura para
// llevar la cuenta de cuanto ya entregamos.
struct DatosLectura {
    const char *texto;
    size_t      pos;
};

// Callback que libcurl llama para obtener el contenido del mail.
static size_t leer_callback(char *buffer, size_t size, size_t nmemb, void *userp) {
    struct DatosLectura *datos = (struct DatosLectura *)userp;
    size_t disponible = size * nmemb;
    size_t restante = strlen(datos->texto) - datos->pos;

    if (restante == 0) return 0;            // ya entregamos todo
    size_t a_copiar = (restante < disponible) ? restante : disponible;

    memcpy(buffer, datos->texto + datos->pos, a_copiar);
    datos->pos += a_copiar;
    return a_copiar;
}

int enviarCorreo(const ConfigSMTP *cfg, const char *destino,
                 const char *asunto, const char *cuerpo) {
    if (!cfg->cargado) {
        printf("Correo no enviado: config SMTP no disponible.\n");
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) return 0;

    // Armar el mail completo (headers + cuerpo) en formato RFC 5322
    char mensaje[2048];
    snprintf(mensaje, sizeof(mensaje),
             "To: %s\r\n"
             "From: %s\r\n"
             "Subject: %s\r\n"
             "\r\n"
             "%s\r\n",
             destino, cfg->usuario, asunto, cuerpo);

    struct DatosLectura datos = { mensaje, 0 };

    // URL del servidor: smtps://smtp.gmail.com:465
    char url[200];
    snprintf(url, sizeof(url), "smtps://%s:%d", cfg->servidor, cfg->puerto);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_USERNAME, cfg->usuario);
    curl_easy_setopt(curl, CURLOPT_PASSWORD, cfg->password);
    curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);

    // Remitente y destinatario (el "sobre" SMTP)
    char from[120];
    snprintf(from, sizeof(from), "<%s>", cfg->usuario);
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, from);

    struct curl_slist *destinatarios = NULL;
    char to[120];
    snprintf(to, sizeof(to), "<%s>", destino);
    destinatarios = curl_slist_append(destinatarios, to);
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, destinatarios);

    // El cuerpo se entrega via callback
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, leer_callback);
    curl_easy_setopt(curl, CURLOPT_READDATA, &datos);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

    // Ejecutar el envio
    CURLcode res = curl_easy_perform(curl);

    int exito = (res == CURLE_OK);
    if (!exito)
        printf("Error al enviar correo: %s\n", curl_easy_strerror(res));

    curl_slist_free_all(destinatarios);
    curl_easy_cleanup(curl);
    return exito;
}