/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: demre <demre@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 19:52:13 by demre             #+#    #+#             */
/*   Updated: 2024/08/05 19:54:34 by demre            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef CORE_HPP
#define CORE_HPP

#include "dependencies.hpp"
#include "utils.hpp"

int sendall(int s, const char *buf, int len);

#endif
