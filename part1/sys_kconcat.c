// AI generated - too lazy to reinstall openBSD

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/malloc.h>
#include <sys/proc.h>
#include <sys/syscallargs.h>

int
sys_kconcat(struct proc *p, void *v, register_t *retval)
{
    struct sys_kconcat_args *uap = v;
    
    // Extragerea argumentelor din userland folosind SCARG
    char *s1 = SCARG(uap, s1);
    char *s2 = SCARG(uap, s2);
    char *dst = SCARG(uap, dst);
    size_t len = SCARG(uap, len);

    char *kbuf;
    size_t done1 = 0, done2 = 0;
    int error;

    // 1. Validarea parametrilor primiti
    if (len == 0 || dst == NULL || s1 == NULL || s2 == NULL)
        return EINVAL;

    // 2. Alocarea dinamica a unui buffer in spatiul kernel cu malloc(9)
    kbuf = malloc(len, M_TEMP, M_WAITOK);
    if (kbuf == NULL)
        return ENOMEM;

    // 3. Copierea primului sir (s1) din userland in bufferul din kernel
    error = copyinstr(s1, kbuf, len, &done1);
    if (error) {
        free(kbuf, M_TEMP, len);
        return error;
    }
    // done1 include caracterul '\0', deci lungimea lui s1 este (done1 - 1)
    size_t len1 = done1 - 1;

    // Verificăm dacă mai există spațiu în buffer pentru al doilea șir
    if (len1 >= len) {
        free(kbuf, M_TEMP, len);
        return ENAMETOOLONG;
    }

    // 4. Copierea celui de-al doilea sir (s2) direct dupa primul in kbuf
    error = copyinstr(s2, kbuf + len1, len - len1, &done2);
    if (error) {
        free(kbuf, M_TEMP, len);
        return error;
    }

    size_t len2 = done2 - 1;
    size_t total_len = len1 + len2;

    // 5. Copierea rezultatului concatenat din kernel inapoi in bufferul dst din userland
    error = copyoutstr(kbuf, dst, len, NULL);
    if (error) {
        free(kbuf, M_TEMP, len);
        return error;
    }

    // 6. Eliberarea memoriei alocate in kernel
    free(kbuf, M_TEMP, len);

    // 7. Setarea valorii de retur (lungimea totala a sirului concatenat)
    *retval = total_len;

    return 0; // Returnam 0 pentru succes
}

💡 Dorești să parcurgem și rezolvarea pentru exercițiul 4 (numărarea numerelor prime cu `fork` și memorie partajată `shm_open`/`mmap`) sau exercițiul 5 (masurarea timpului de execuție cu `gettimeofday`) din modelul de colocviu?
