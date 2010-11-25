/*==============================================================================
    Copyright (c) 2005-2010 Joel de Guzman
    Copyright (c) 2010 Thomas Heller

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef PHOENIX_OPERATOR_LOGICAL_HPP
#define PHOENIX_OPERATOR_LOGICAL_HPP

#include <boost/phoenix/core/meta_grammar.hpp>

namespace boost { namespace phoenix
{
	
	PHOENIX_UNARY_OPERATORS(
		(logical_not)
	)
	PHOENIX_BINARY_OPERATORS(
		(logical_and)
		(logical_or)
	)
}}

#endif