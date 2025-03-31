#ifndef MINITALK_H
# define MINITALK_H

# include<stdlib.h>
# include<unistd.h>
# include<signal.h>

char	*itoa_pid(int nb);
int		ft_strlen(char *str);
void	ft_putnbr(int nb);
int		ft_atoi(char *str);
char	*convertbinary(unsigned char a);

#endif