#include "get_next_line.h"
#include <stdio.h>

ssize_t ft_read(int fd, char **buf, t_gnl *gnl)
{
    ssize_t     read_value;

    *buf = (char *) malloc(BUFFER_SIZE + 1);
    if (*buf == 0)
        return (-3);
    read_value = read(fd, *buf, BUFFER_SIZE);
    if (read_value > 0)
    {
        (*buf)[read_value] = '\0';
        return (read_value);
    }
    else if (read_value == 0)
    {
        free(*buf);
        return (-2);
    }
    else
    {
        free(*buf);
        clear_gnl(gnl);
        return (-3);
    }
}

char    *get_one_line(int fd, t_gnl *gnl)
{
    char    *buf;
    ssize_t val;

    gnl->nl_index = -1;
    while (gnl->nl_index == -1)
    {
        val = ft_read(fd, &buf, gnl);
        if (val == -3)
           return (0);
        if (val == -2)
            return (make_one_line(gnl, gnl->len - 1, -2));
        if (gnl->len + BUFFER_SIZE > gnl->buffer)
            gnl->buffer = (gnl->buffer) * 2;
        gnl->temp = make_temp(buf, gnl);
        gnl->nl_index = check_nl_temp(gnl->temp);
        if (gnl->nl_index >= 0)
            return(make_one_line(gnl, gnl->nl_index, 0));
    }
    return (0);
}

char    *get_next_line(int fd)
{
    static  t_gnl  gnl_array[4092];
    ssize_t        idx; 

    if (fd < 0 || fd == 1 || fd == 2)
        return (0);
    if (gnl_array[fd].buffer < BUFFER_SIZE)
        gnl_array[fd].buffer = BUFFER_SIZE;
    idx = 0;
    while (gnl_array[fd].temp != 0 && gnl_array[fd].temp[idx] != '\0')
    {
        if (gnl_array[fd].temp[idx] == '\n')
            break ;
        idx++;
    }
    if (idx != gnl_array[fd].len)
        return (make_one_line(&gnl_array[fd], idx, 0));
    return (get_one_line(fd, &gnl_array[fd]));
}
