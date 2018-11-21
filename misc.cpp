#include <string.h>
#include "misc.h"

void ExtractName(const char *path_name_ext, char *name)
{
    int i = 0;
    int start_name = 0;
    int end_name = 0;
    while (true)
    {
        if ((path_name_ext[i] == '\\') || (path_name_ext[i] == '/'))
            start_name = i + 1;
        if (path_name_ext[i] == '.')
            end_name = i;
        if (path_name_ext[i] == 0)
        {
            if (end_name == 0)
                end_name = i;
            break;
        }
        i++;
    }
    if (end_name >= start_name)
    {
        memcpy(name, &path_name_ext[start_name], end_name - start_name);
        name[end_name - start_name] = 0;
    }
    else
        name[0] = 0;
}
