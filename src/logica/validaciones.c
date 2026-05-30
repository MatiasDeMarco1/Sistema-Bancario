#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "validaciones.h"
#include "cliente.h"
#include <ctype.h>


void validarContrasena(char *buffer, int tamBuffer) {
    do {
        printf("Ingrese la contrasena (minimo 8 caracteres): ");
        fgets(buffer, tamBuffer, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strlen(buffer) < 8)
            printf("Error: la contrasena debe tener al menos 8 caracteres.\n");
    } while (strlen(buffer) < 8);
}


/*
 * Retorna 1 si el CUIL es válido, 0 si no.
 * Acepta el formato "XX-XXXXXXXX-X" o "XXXXXXXXXXX" (11 dígitos sin guiones).
 */
int validar_Cuil(const char *cuil) {
    char digitos[12] = {0};
    int len = strlen(cuil);
    int j = 0;

    /* Normalizar: extraer solo dígitos */
    for (int i = 0; i < len && j < 11; i++) {
        if (isdigit((unsigned char)cuil[i])) {
            digitos[j++] = cuil[i] - '0';
        } else if (cuil[i] != '-') {
            return 0; /* carácter inválido */
        }
    }

    if (j != 11) return 0; /* longitud incorrecta */

    /* Validar prefijo (tipo de CUIL) */
    int prefijo = digitos[0] * 10 + digitos[1];
    if (prefijo != 20 && prefijo != 23 && prefijo != 24 &&
        prefijo != 27 && prefijo != 30 && prefijo != 33 && prefijo != 34) {
        return 0;
    }

    /* Calcular dígito verificador */
    int mult[] = {5, 4, 3, 2, 7, 6, 5, 4, 3, 2};
    int suma = 0;
    for (int i = 0; i < 10; i++) {
        suma += digitos[i] * mult[i];
    }

    int resto = 11 - (suma % 11);
    if (resto == 11) resto = 0;
    if (resto > 9)   return 0; /* dígito verificador imposible */

    return resto == digitos[10];
}



/*
 * Retorna 1 si el mail es válido, 0 si no.
 * Reglas aplicadas:
 *   - Exactamente un '@'
 *   - Parte local (antes del '@'): al menos 1 carácter
 *   - Dominio (después del '@'): al menos un punto, sin punto al inicio
 *     ni al final, sin puntos consecutivos
 *   - Extensión final de al menos 2 caracteres (ej: .com, .ar)
 */
int validar_Mail(const char *mail) {
    int len = strlen(mail);
    int at = -1;

    /* Buscar '@' y verificar que sea único */
    for (int i = 0; i < len; i++) {
        if (mail[i] == '@') {
            if (at != -1) return 0; /* más de un '@' */
            at = i;
        }
    }

    if (at == -1) return 0; /* no hay '@' */
    if (at == 0)  return 0; /* parte local vacía */

    /* Validar parte local: no puede tener espacios ni punto al inicio/fin */
    if (mail[0] == '.' || mail[at - 1] == '.') return 0;
    for (int i = 0; i < at; i++) {
        if (mail[i] == ' ') return 0;
        if (mail[i] == '.' && mail[i + 1] == '.') return 0; /* puntos consecutivos */
    }

    /* Validar dominio: todo lo que viene después del '@' */
    const char *dominio = mail + at + 1;
    int dlen = strlen(dominio);

    if (dlen == 0)           return 0; /* dominio vacío */
    if (dominio[0] == '.')   return 0; /* empieza con punto */
    if (dominio[dlen-1]=='.') return 0; /* termina con punto */

    /* Buscar último punto en el dominio para verificar la extensión */
    int ultimo_punto = -1;
    for (int i = 0; i < dlen; i++) {
        if (dominio[i] == ' ') return 0;
        if (dominio[i] == '.' && dominio[i + 1] == '.') return 0; /* puntos consecutivos */
        if (dominio[i] == '.') ultimo_punto = i;
    }

    if (ultimo_punto == -1)              return 0; /* sin punto en el dominio */
    if (dlen - ultimo_punto - 1 < 2)    return 0; /* extensión menor a 2 chars */

    return 1;
}
/*
 * Retorna 1 si el teléfono es válido, 0 si no.
 * Formatos aceptados (Argentina):
 *   - +54 9 11 1234-5678
 *   - +54911 12345678
 *   - 011 1234-5678
 *   - 11 1234-5678
 *   - 1234-5678
 * Reglas:
 *   - Solo dígitos, espacios, guiones y '+' al inicio
 *   - Entre 8 y 15 dígitos en total 
 *   - No puede empezar con 0 y otro 0 seguido
 *   - No puede tener guiones o espacios consecutivos
 */
int validar_Telefono(const char *tel) {
    int len = strlen(tel);
    int digitos = 0;
    int i = 0;

    if (len == 0) return 0;

    /* '+' solo permitido al inicio */
    if (tel[0] == '+') i = 1;

    for (; i < len; i++) {
        char c = tel[i];

        if (isdigit((unsigned char)c)) {
            digitos++;
        } else if (c == '-' || c == ' ') {
            /* no puede haber separadores consecutivos */
            if (i + 1 < len && (tel[i+1] == '-' || tel[i+1] == ' ')) return 0;
            /* no puede empezar ni terminar con separador */
            if (i == 1 && tel[0] == '+') return 0;
            if (i == len - 1)            return 0;
        } else {
            return 0; /* carácter no permitido */
        }
    }

    /* Cantidad de dígitos : mínimo 8, máximo 15 */
    if (digitos < 8 || digitos > 15) return 0;

    /* No puede haber dos ceros al inicio (después del '+' si lo hay) */
    int inicio = (tel[0] == '+') ? 1 : 0;
    while (!isdigit((unsigned char)tel[inicio])) inicio++;
    if (tel[inicio] == '0' && isdigit((unsigned char)tel[inicio+1])
        && tel[inicio+1] == '0') return 0;

    return 1;
}

/*
 * Retorna 1 si el CUIL NO existe en el archivo (es único), 0 si ya está registrado.
 * En caso de error al abrir el archivo, retorna -1.
 */
int validar_cuil_unico(const char *cuit) {
    FILE *f = fopen("./datos/clientes.dat", "rb");

    if (f == NULL) return -1; /* no se pudo abrir el archivo */

    Cliente c;
    while (fread(&c, sizeof(Cliente), 1, f) == 1) {
        if (strcmp(c.cuit, cuit) == 0) {
            fclose(f);
            return 0; /* CUIL ya registrado */
        }
    }

    fclose(f);
    return 1; /* CUIL único, no encontrado */
}

// Valida la contrasena ya capturada (misma regla que validarContrasena).
// Devuelve 1 si es valida, 0 si no.
int validar_contrasena_str(const char *pass) {
    return strlen(pass) >= 8;
}

int validar_mail_unico(const char *mail) {
    FILE *f = fopen("./datos/clientes.dat", "rb");
    if (f == NULL) return -1;
    Cliente c;
    while (fread(&c, sizeof(Cliente), 1, f) == 1) {
        if (c.activo && strcmp(c.mail, mail) == 0) {
            fclose(f);
            return 0;
        }
    }
    fclose(f);
    return 1;
}

int validar_telefono_unico(const char *tel) {
    FILE *f = fopen("./datos/clientes.dat", "rb");
    if (f == NULL) return -1;
    Cliente c;
    while (fread(&c, sizeof(Cliente), 1, f) == 1) {
        if (c.activo && strcmp(c.telefono, tel) == 0) {
            fclose(f);
            return 0;
        }
    }
    fclose(f);
    return 1;
}

// Hash educativo de contrasena (FNV-1a + salt).
// NO es criptograficamente seguro: es solo para no guardar texto plano.
// 'salida' debe tener al menos 17 bytes (16 hex + '\0').
void hashearContrasena(const char *pass, char *salida) {
    // Salt fijo: agrega una constante antes de hashear.
    // (Un sistema real usaria un salt aleatorio distinto por usuario.)
    const char *salt = "BancoUCEL2024_";

    unsigned long long hash = 14695981039346656037ULL; // offset FNV-1a 64-bit
    const unsigned long long prime = 1099511628211ULL;

    // Hashear primero el salt, luego la contrasena
    for (const char *p = salt; *p; p++) {
        hash ^= (unsigned char)(*p);
        hash *= prime;
    }
    for (const char *p = pass; *p; p++) {
        hash ^= (unsigned char)(*p);
        hash *= prime;
    }

    // Convertir a string hexadecimal de 16 caracteres
    snprintf(salida, 17, "%016llx", hash);
}