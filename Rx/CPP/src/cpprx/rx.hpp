// Copyright (c) Microsoft Open Technologies, Inc. All rights reserved. See License.txt in the project root for license information.

#pragma once
#include "rx-includes.hpp"

#if !defined(CPPRX_RX_HPP)
#define CPPRX_RX_HPP

namespace rxcpp
{
    template<class T, class Obj>
    class BinderBase
    {
    protected:
        Obj obj;

    public:
        typedef T item_type;
        typedef Obj observable_type;

        BinderBase(Obj obj) : obj(std::move(obj))
        {
        }
    };

    template<class T, class Obj, bool IsTObservable>
    class BinderNested;

    template<class T, class Obj>
    class BinderNested<T, Obj, false> : public BinderBase<T, Obj>
    {
    protected:
        typedef BinderBase<T, Obj> base;
        typedef typename base::item_type item_type;
        using base::obj;
    public:
        static const bool is_item_observable = false;
        BinderNested(Obj obj) : BinderBase<T, Obj>(std::move(obj))
        {
        }
    };

    template<class T, class Obj>
    class BinderNested<T, Obj, true> : public BinderBase<T, Obj>
    {
    protected:
        typedef BinderBase<T, Obj> base;
        typedef typename base::item_type item_type;
        using base::obj;
    public:
        static const bool is_item_observable = true;

        BinderNested(Obj obj) : base(std::move(obj))
        {
        }
        
        auto select_many()
            -> decltype(from(SelectMany<item_type>(obj, util::pass_through(), util::pass_through_second()))) {
            return      from(SelectMany<item_type>(obj, util::pass_through(), util::pass_through_second()));
        }
        template <class CS>
        auto select_many(CS collectionSelector)
            -> decltype(from(SelectMany<item_type>(obj, std::move(collectionSelector), util::pass_through_second()))) {
            return      from(SelectMany<item_type>(obj, std::move(collectionSelector), util::pass_through_second()));
        }
        template <class CS, class RS>
        auto select_many(CS collectionSelector, RS resultSelector)
            -> decltype(from(SelectMany<item_type>(obj, std::move(collectionSelector), std::move(resultSelector)))) {
            return      from(SelectMany<item_type>(obj, std::move(collectionSelector), std::move(resultSelector)));
        }
    };
    
    template<class Obj>
    class Binder : public BinderNested<
        typename observable_item<Obj>::type,
        Obj,
        is_observable<typename observable_item<Obj>::type>::value>
    {
        typedef BinderNested<
        typename observable_item<Obj>::type,
        Obj,
        is_observable<typename observable_item<Obj>::type>::value> base;
        typedef typename base::item_type item_type;
        using base::obj;
    public:

        Binder(Obj obj) : base(std::move(obj))
        {
        }

        template <class S>
        auto select(S selector) -> decltype(from(Select<item_type>(obj, selector))) {
            return from(Select<item_type>(obj, selector));
        }
#if RXCPP_USE_VARIADIC_TEMPLATES
        template <class... MergeSource>
        auto merge(const MergeSource&... source) 
            -> decltype(from(Merge(obj, source...))) {
            return      from(Merge(obj, source...));
        }
        template <class S, class... ZipSource>
        auto zip(S selector, const ZipSource&... source) 
            -> decltype(from(Zip(selector, obj, source...))) {
            return      from(Zip(selector, obj, source...));
        }
        template <class... Zip1Source>
        auto zip(const Zip1Source&... source) 
            -> decltype(from(Zip(util::as_tuple(), obj, source...))) {
            return      from(Zip(util::as_tuple(), obj, source...));
        }
        template <class S, class... CombineLSource>
        auto combine_latest(S selector, const CombineLSource&... source) 
            -> decltype(from(CombineLatest(selector, obj, source...))) {
            return      from(CombineLatest(selector, obj, source...));
        }
        template <class... CombineL1Source>
        auto combine_latest(const CombineL1Source&... source) 
            -> decltype(from(CombineLatest(util::as_tuple(), obj, source...))) {
            return      from(CombineLatest(util::as_tuple(), obj, source...));
        }
#endif //RXCPP_USE_VARIADIC_TEMPLATES
        template <class P>
        auto where(P predicate) -> decltype(from(Where<item_type>(obj, predicate))) {
            return from(Where<item_type>(obj, predicate));
        }
        Obj publish() {
            return obj;
        }
        template <class KS>
        auto group_by(
            KS keySelector)
            -> decltype(from(GroupBy<item_type>(obj, keySelector, util::pass_through(), std::less<decltype(keySelector((*(item_type*)0)))>()))) {
            return      from(GroupBy<item_type>(obj, keySelector, util::pass_through(), std::less<decltype(keySelector((*(item_type*)0)))>()));
        }
        template <class KS, class VS>
        auto group_by(
            KS keySelector,
            VS valueSelector)
            -> decltype(from(GroupBy<item_type>(obj, keySelector, valueSelector, std::less<decltype(keySelector((*(item_type*)0)))>()))) {
            return      from(GroupBy<item_type>(obj, keySelector, valueSelector, std::less<decltype(keySelector((*(item_type*)0)))>()));
        }
        template <class KS, class VS, class L>
        auto group_by(
            KS keySelector,
            VS valueSelector,
            L less)
            -> decltype(from(GroupBy<item_type>(obj, keySelector, valueSelector, less))) {
            return      from(GroupBy<item_type>(obj, keySelector, valueSelector, less));
        }
        template <class Integral>
        auto take(Integral n) -> decltype(from(Take<item_type>(obj, n))) {
            return from(Take<item_type>(obj, n));
        }
        template<template<class Value>class Allocator>
        auto to_vector() 
            -> decltype(from(ToStdCollection<std::vector<item_type, Allocator<item_type>>>(obj))) {
            return      from(ToStdCollection<std::vector<item_type, Allocator<item_type>>>(obj));
        }
        auto to_vector() 
            -> decltype(from(ToStdCollection<std::vector<item_type>>(obj))) {
            return      from(ToStdCollection<std::vector<item_type>>(obj));
        }
        template<template<class Value>class Allocator>
        auto to_list() 
            -> decltype(from(ToStdCollection<std::list<item_type, Allocator<item_type>>>(obj))) {
            return      from(ToStdCollection<std::list<item_type, Allocator<item_type>>>(obj));
        }
        auto to_list() 
            -> decltype(from(ToStdCollection<std::list<item_type>>(obj))) {
            return      from(ToStdCollection<std::list<item_type>>(obj));
        }
        auto delay(Scheduler::clock::duration due, Scheduler::shared scheduler) -> decltype(from(Delay<item_type>(obj, due, scheduler))) {
            return from(Delay<item_type>(obj, due, scheduler));
        }
        auto limit_window(int milliseconds) -> decltype(from(LimitWindow<item_type>(obj, milliseconds))) {
            return from(LimitWindow<item_type>(obj, milliseconds));
        }
        auto distinct_until_changed() -> decltype(from(DistinctUntilChanged<item_type>(obj))) {
            return from(DistinctUntilChanged<item_type>(obj));
        }
        auto subscribe_on(Scheduler::shared scheduler)
        -> decltype(from(SubscribeOnObservable<item_type>(obj, std::move(scheduler))))
        {
            return from(SubscribeOnObservable<item_type>(obj, std::move(scheduler)));
        }
        auto observe_on(Scheduler::shared scheduler)
        -> decltype(from(ObserveOnObserver<item_type>(obj, std::move(scheduler))))
        {
            return from(ObserveOnObserver<item_type>(obj, std::move(scheduler)));
        }
        auto on_dispatcher() 
        -> decltype(from(ObserveOnDispatcher<item_type>(obj)))
        {
            return from(ObserveOnDispatcher<item_type>(obj));
        }
        template <class OnNext>
        void for_each(OnNext onNext) {
            ForEach<item_type>(obj, onNext);
        }
        template <class OnNext>
        auto subscribe(OnNext onNext) -> decltype(Subscribe(obj, onNext)) {
            auto result = Subscribe(obj, onNext);
            return result;
        }
        template <class OnNext, class OnComplete>
        auto subscribe(OnNext onNext, OnComplete onComplete) 
            -> decltype(Subscribe(obj, onNext, onComplete)) {
            auto result = Subscribe(obj, onNext, onComplete);
            return result;
        }
        template <class OnNext, class OnComplete, class OnError>
        auto subscribe(OnNext onNext, OnComplete onComplete, OnError onError) 
            -> decltype(Subscribe(obj, onNext, onComplete, onError)) {
            auto result = Subscribe(obj, onNext, onComplete, onError);
            return result;
        }
#if RXCPP_USE_VARIADIC_TEMPLATES
        template <class Tag, class... ChainArg>
        auto chain(ChainArg&&... arg) 
            -> decltype(from(rxcpp_chain(Tag(), obj, std::forward<ChainArg>(arg)...))) {
            return from(rxcpp_chain(Tag(), obj, std::forward<ChainArg>(arg)...));
        }
#endif
    };

    template<class T>
    Binder<std::shared_ptr<Observable<T>>> from(std::shared_ptr<Observable<T>> obj) { 
        return Binder<std::shared_ptr<Observable<T>>>(std::move(obj)); }

    template<class K, class T>
    Binder<std::shared_ptr<GroupedObservable<K, T>>> from(std::shared_ptr<GroupedObservable<K, T>> obj) { 
        return Binder<std::shared_ptr<GroupedObservable<K, T>>>(std::move(obj)); }

    template<class T>
    Binder<std::shared_ptr<Observable<T>>> from(std::shared_ptr<Subject<T>> obj) {
        return Binder<std::shared_ptr<Observable<T>>>(std::move(obj)); }
    
    template<class K, class T>
    Binder<std::shared_ptr<GroupedObservable<K, T>>> from(std::shared_ptr<GroupedSubject<K, T>> obj) {
        return Binder<std::shared_ptr<GroupedObservable<K, T>>>(std::move(obj)); }

    template<class Obj>
    Binder<Obj> from(Binder<Obj> binder) { 
        return std::move(binder); }

    template<class T>
    T item(const Binder<std::shared_ptr<Observable<T>>>&);

    template<class T, class K>
    T item(const Binder<std::shared_ptr<GroupedObservable<K, T>>>&);
}

#endif