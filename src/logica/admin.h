#ifndef ADMIN_H
#define ADMIN_H

typedef struct {
    char usuario[50];
    char contrasena[17];   // hash (16 hex + '\0'), igual que cliente
} Admin;

// Guarda un admin nuevo en el archivo
void guardarAdmin(Admin *a);

// Busca un admin por usuario. Si lo encuentra, lo carga en *a y devuelve 1.
int buscarAdmin(const char *usuario, Admin *a);

// Crea el admin inicial si el archivo no existe todavia (seed).
void seedAdmin(void);

#endif