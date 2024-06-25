/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 20:21:36 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/04/20 14:49:51 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <stdio.h>
# include <unistd.h>

int	ft_printf(const char *form_spec, ...);
int	ft_char(char c);
int	ft_str(char *str);
int	ft_putnbr(int nb);
int	ft_putunbr(unsigned long int num);
int	ft_ptradress(unsigned long int num);
int	ft_hex(unsigned int num, char format);

#endif
