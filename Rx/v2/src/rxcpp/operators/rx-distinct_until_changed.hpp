// Copyright (c) Microsoft Open Technologies, Inc. All rights reserved. See License.txt in the project root for license information.

#pragma once

#if !defined(RXCPP_OPERATORS_RX_DISTINCT_UNTIL_CHANGED_HPP)
#define RXCPP_OPERATORS_RX_DISTINCT_UNTIL_CHANGED_HPP

#include "../rx-includes.hpp"

namespace rxcpp {

namespace operators {

namespace detail {

template<class T>
struct distinct_until_changed
{
    typedef typename std::decay<T>::type source_value_type;

    template<class Subscriber>
    struct distinct_until_changed_observer : public observer_base<source_value_type>
    {
        typedef distinct_until_changed_observer<Subscriber> this_type;
        typedef observer_base<source_value_type> base_type;
        typedef typename base_type::value_type value_type;
        typedef typename std::decay<Subscriber>::type dest_type;
        typedef observer<value_type, this_type> observer_type;
        dest_type dest;
        mutable rxu::detail::maybe<source_value_type> remembered;

        distinct_until_changed_observer(dest_type d)
            : dest(d)
        {
        }
        void on_next(source_value_type v) const {
            if (remembered.empty() || v != remembered.get()) {
                remembered.reset(v);
                dest.on_next(v);
            }
        }
        void on_error(std::exception_ptr e) const {
            dest.on_error(e);
        }
        void on_completed() const {
            dest.on_completed();
        }

        static subscriber<value_type, this_type> make(dest_type d) {
            return make_subscriber<value_type>(d, this_type(d));
        }
    };

    template<class Subscriber>
    auto operator()(Subscriber dest) const
        -> decltype(distinct_until_changed_observer<Subscriber>::make(std::move(dest))) {
        return      distinct_until_changed_observer<Subscriber>::make(std::move(dest));
    }
};

class distinct_until_changed_factory
{
public:
    template<class Observable>
    auto operator()(Observable&& source)
        -> decltype(source.lift(distinct_until_changed<typename std::decay<Observable>::type>::value_type)) {
        return      source.lift(distinct_until_changed<typename std::decay<Observable>::type>::value_type);
    }
};

}

inline auto distinct_until_changed()
    ->      detail::distinct_until_changed_factory {
    return  detail::distinct_until_changed_factory();
}

}

}

#endif