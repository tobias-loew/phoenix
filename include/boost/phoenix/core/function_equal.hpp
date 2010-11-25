/*==============================================================================
    Copyright (c) 2005-2010 Joel de Guzman
    Copyright (c) 2010 Thomas Heller

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef PHOENIX_CORE_FUNCTION_EQUAL_HPP
#define PHOENIX_CORE_FUNCTION_EQUAL_HPP

#include <boost/phoenix/core/terminal.hpp>

namespace boost
{
    template <typename> class weak_ptr;
}

namespace boost { namespace phoenix
{
    namespace detail
    {
        struct compare
            : proto::callable
        {
            typedef bool result_type;

            template <typename A0, typename A1>
            result_type operator()(A0 const & a0, A1 const & a1) const
            {
                return a0 == a1;
            }
            
            // hard wiring reference_wrapper and weak_ptr here ...
            // **TODO** find out why boost bind does this ...
            template <typename A0, typename A1>
            result_type operator()(reference_wrapper<A0> const & a0, reference_wrapper<A1> const & a1) const
            {
                return a0.get_pointer() == a1.get_pointer();
            }
            template <typename A0, typename A1>
            result_type operator()(weak_ptr<A0> const & a0, weak_ptr<A1> const & a1) const
            {
                return !(a0 < a1) && !(a1 < a0);
            }
        };

        struct test;

        struct function_equal_
            : proto::when<
                proto::if_<
                    proto::matches<proto::_, proto::_state>()
                  , proto::or_<
                        /*proto::when<
                            rule::custom_terminal
                          , compare(proto::_value, proto::_value(proto::_state))
                        >
                      ,*/ proto::when<
                            proto::terminal<proto::_>
                          , proto::if_<
                                mpl::false_()//is_custom_terminal<proto::_value>()
                              , compare(
                                    proto::lazy<custom_terminal<proto::_value>(proto::_value)>
                                  , proto::lazy<custom_terminal<proto::_value(proto::_state)>(proto::_value(proto::_state))>
                                )
                              , compare(proto::_value, proto::_value(proto::_state))
                            >
                        >
                      , proto::otherwise<test(proto::_, proto::_state)>
                    >
                  , test()
                >
            >
        {};

        struct test
            : proto::callable
        {
            typedef bool result_type;

            result_type operator()() const
            {
                return false;
            }

            template <typename Expr1>
            result_type operator()(Expr1 const& e1, Expr1 const& e2) const
            {
                return eval(e1, e2, typename proto::arity_of<Expr1>::type());
            }

            private:
                #define FUNCTION_EQUAL_R(Z, N, DATA)                            \
                    BOOST_PP_IF(N, &&, BOOST_PP_EMPTY())                        \
                    function_equal_()(                                          \
                            proto::child_c< N >(e1)                             \
                          , proto::child_c< N >(e2)                             \
                        )                                                       \
                /**/

                #define FUNCTION_EQUAL(Z, N, DATA) \
                    template <typename Expr1> \
                    result_type eval(Expr1 const& e1, Expr1 const& e2, mpl::long_< N >) const \
                    { \
                        return BOOST_PP_REPEAT(N, FUNCTION_EQUAL_R, _); \
                    } \
                /**/

                BOOST_PP_REPEAT(BOOST_PROTO_MAX_ARITY, FUNCTION_EQUAL, _)
        };
    }

    template <typename Expr1, typename Expr2>
    bool function_equal_impl(actor<Expr1> const& a1, actor<Expr2> const& a2)
    {
        return detail::function_equal_()(a1, a2);
    }

    template <typename Expr1, typename Expr2>
    bool function_equal(actor<Expr1> const& a1, actor<Expr2> const& a2)
    {
        return function_equal_impl(a1, a2);
    }

}}

#endif
