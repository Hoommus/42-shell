#include "twenty_one_sh.h"

#ifdef __linux__
int				gethostname(char *arr, size_t size)
{
	ft_memcpy(arr, "(null)\0", size);
	return (0);
}
#endif
