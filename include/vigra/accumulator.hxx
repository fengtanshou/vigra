/************************************************************************/
/*                                                                      */
/*               Copyright 2011-2012 by Ullrich Koethe                  */
/*                                                                      */
/*    This file is part of the VIGRA computer vision library.           */
/*    The VIGRA Website is                                              */
/*        http://hci.iwr.uni-heidelberg.de/vigra/                       */
/*    Please direct questions, bug reports, and contributions to        */
/*        ullrich.koethe@iwr.uni-heidelberg.de    or                    */
/*        vigra@informatik.uni-hamburg.de                               */
/*                                                                      */
/*    Permission is hereby granted, free of charge, to any person       */
/*    obtaining a copy of this software and associated documentation    */
/*    files (the "Software"), to deal in the Software without           */
/*    restriction, including without limitation the rights to use,      */
/*    copy, modify, merge, publish, distribute, sublicense, and/or      */
/*    sell copies of the Software, and to permit persons to whom the    */
/*    Software is furnished to do so, subject to the following          */
/*    conditions:                                                       */
/*                                                                      */
/*    The above copyright notice and this permission notice shall be    */
/*    included in all copies or substantial portions of the             */
/*    Software.                                                         */
/*                                                                      */
/*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND    */
/*    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES   */
/*    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND          */
/*    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT       */
/*    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,      */
/*    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING      */
/*    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR     */
/*    OTHER DEALINGS IN THE SOFTWARE.                                   */
/*                                                                      */
/************************************************************************/

#ifndef VIGRA_ACCUMULATOR_HXX
#define VIGRA_ACCUMULATOR_HXX

#include "config.hxx"
#include "metaprogramming.hxx"
#include "mathutil.hxx"
#include "utilities.hxx"
#include "matrix.hxx"
#include "multi_math.hxx"
#include "eigensystem.hxx"
#include "bit_array.hxx"
#include <algorithm>
#include <iostream>

namespace vigra {

namespace acc1 {

    // this class closes an accumulator chain
struct AccumulatorBase 
{
	typedef AccumulatorBase Tag;
    typedef void                                       result_type;
        // 'qualified_result_type' can be 'result_type' or 'result_type const &'
        // depending on the return value of operator()()
    typedef void                                       qualified_result_type;
    
    void operator()() const
    {
        vigra_precondition(false,
            std::string("get(accumulator): attempt to access inactive statistic."));
    }
    
    void activate()
    {}
    
    void reset()
    {}
    
    unsigned int passesRequired() const
    {
        return 1;
    }

    template <class Shape>
    void reshape(Shape const &)
    {}
};

template <class T, unsigned LEVEL=0>
struct TypedAccumulatorBase
: public AccumulatorBase
{
	typedef AccumulatorBase Tag;
    typedef AccumulatorBase BaseType;
    
    static const UInt64 level = LEVEL;

    typedef T const &       argument_type;
    typedef argument_type   first_argument_type;
    typedef double          second_argument_type;
    
        // used for dynamic accumulators to keep track of the active ones.
    BitArray<LEVEL> active_accumulators_;

    void reset()
    {
        active_accumulators_.clear();
    }
    
	void operator+=(TypedAccumulatorBase const &)
    {}
    
    using BaseType::operator();
    
	void operator()(argument_type)
    {}
    
    void operator()(first_argument_type, second_argument_type)
    {}
    
    void updatePass2(argument_type)
    {}
    
    void updatePass2(first_argument_type, second_argument_type)
    {}
};

// class Count;

// template <class TAG = void>
// class Coord;

// template <class Accumulators>
// struct CoordDependencies
// {
    // typedef typename Push<typename Coord<typename Accumulators::Head>::type, 
                          // typename CoordDependencies<typename Accumulators::Tail>::type>::type type;
// };

// template <>
// struct CoordDependencies<void>
// {
    // typedef void type;
// };

// template <class TAG>
// class Coord
// {
  // public:
  
    // typedef typename CoordDependencies<typename TAG::Dependencies::type>::type Dependencies;
    // typedef Coord<TAG> type;
    
    // template<class U, class Base>
    // struct Impl
    // : public TAG::template Impl<typename U::CoordType, Base>
    // {
        // typedef Coord<TAG> Tag;
        // typedef Base BaseType;
        // typedef typename TAG::template Impl<typename U::CoordType, Base> WrappedType;
    // };
// };

// template <>
// class Coord<Count>
// {
  // public:
    // typedef Count type;
// };

// template <>
// class Coord<void>
// {
  // public:
    // typedef void type;
// };

// struct Coupled
// {
    // typedef Shape2 CoordType;
    // typedef double DataType;
// };

// template <class T01=void, class T02=void, class T03=void, class T04=void, class T05=void,
          // class T06=void, class T07=void, class T08=void, class T09=void, class T10=void,
          // class T11=void, class T12=void, class T13=void, class T14=void, class T15=void,
          // class T16=void, class T17=void, class T18=void, class T19=void, class T20=void>
// class UseCoord
// : public MakeTypeList<typename Coord<T01>::type, typename Coord<T02>::type, typename Coord<T03>::type, 
                      // typename Coord<T04>::type, typename Coord<T05>::type, typename Coord<T06>::type, 
                      // typename Coord<T07>::type, typename Coord<T08>::type, typename Coord<T09>::type, 
                      // typename Coord<T10>::type, typename Coord<T11>::type, typename Coord<T12>::type, 
                      // typename Coord<T13>::type, typename Coord<T14>::type, typename Coord<T15>::type, 
                      // typename Coord<T16>::type, typename Coord<T17>::type, typename Coord<T18>::type, 
                      // typename Coord<T19>::type, typename Coord<T20>::type >
// {};

// template <class T>
// struct Expand
// {
    // typedef T type;
// };

// template <class T01, class T02, class T03, class T04, class T05,
          // class T06, class T07, class T08, class T09, class T10,
          // class T11, class T12, class T13, class T14, class T15,
          // class T16, class T17, class T18, class T19, class T20>
// struct Expand<UseCoord<T01, T02, T03, T04, T05, T06, T07, T08, T09, T10,
                  // T11, T12, T13, T14, T15, T16, T17, T18, T19, T20> >
// : public UseCoord<T01, T02, T03, T04, T05, T06, T07, T08, T09, T10,
                  // T11, T12, T13, T14, T15, T16, T17, T18, T19, T20>
// {};

    // // Select is a synonym for MakeTypeList
// template <class T01=void, class T02=void, class T03=void, class T04=void, class T05=void,
          // class T06=void, class T07=void, class T08=void, class T09=void, class T10=void,
          // class T11=void, class T12=void, class T13=void, class T14=void, class T15=void,
          // class T16=void, class T17=void, class T18=void, class T19=void, class T20=void>
// struct Select
// : public MakeTypeList<typename Expand<T01>::type, typename Expand<T02>::type, typename Expand<T03>::type, 
                      // typename Expand<T04>::type, typename Expand<T05>::type, typename Expand<T06>::type, 
                      // typename Expand<T07>::type, typename Expand<T08>::type, typename Expand<T09>::type, 
                      // typename Expand<T10>::type, typename Expand<T11>::type, typename Expand<T12>::type, 
                      // typename Expand<T13>::type, typename Expand<T14>::type, typename Expand<T15>::type, 
                      // typename Expand<T16>::type, typename Expand<T17>::type, typename Expand<T18>::type, 
                      // typename Expand<T19>::type, typename Expand<T20>::type >
// {};

    // Select is a synonym for MakeTypeList
template <class T01=void, class T02=void, class T03=void, class T04=void, class T05=void,
          class T06=void, class T07=void, class T08=void, class T09=void, class T10=void,
          class T11=void, class T12=void, class T13=void, class T14=void, class T15=void,
          class T16=void, class T17=void, class T18=void, class T19=void, class T20=void>
struct Select
: public MakeTypeList<T01, T02, T03, T04, T05, T06, T07, T08, T09, T10,
                      T11, T12, T13, T14, T15, T16, T17, T18, T19, T20>
{};

namespace detail {

    // Insert the dependencies of the selected functors into the TypeList and sort
    // the list such that dependencies come after the functors using them. Make sure 
    // that each functor is contained only once.
template <class T>
struct AddDependencies
{
    typedef typename AddDependencies<typename T::type>::type type;
};

template <class HEAD, class TAIL>
struct AddDependencies<TypeList<HEAD, TAIL> >
{
    typedef typename AddDependencies<TAIL>::type                                   TailWithDependencies;
    typedef typename AddDependencies<typename HEAD::Dependencies>::type            HeadDependencies;
    typedef TypeList<HEAD, HeadDependencies>                                       HeadWithDependencies;
    typedef typename PushUnique<HeadWithDependencies, TailWithDependencies>::type  type;
    typedef typename type::Head Head;
    typedef typename type::Tail Tail;
};

template <>
struct AddDependencies<void>
{
    typedef void type;
    typedef void Head;
    typedef void Tail;
};

    // Helper class to activate dependencies at runtime (i.e. when activate<Tag>(accu) is called,
    // activate() must also be called for Tag's dependencies).
template <class Dependencies>
struct ActivateDependencies
{
    template <class Accumulator>
    static void exec(Accumulator & a)
    {
        activate<typename Dependencies::Head>(a);
        ActivateDependencies<typename Dependencies::Tail>::exec(a);
    }
};

template <>
struct ActivateDependencies<void>
{
    template <class Accumulator>
    static void exec(Accumulator & a)
    {}
};

    // This helper class adds dynamic (runtime) activation to the 
    // accumulator specified by TAG. It follows a simple principle: call 
    // the wrapped functionality in Base if the accumulator is active, but skip 
    // Base and call BaseBase otherwise. The activation flags are stored in 
    // TypedAccumulatorBase::active_accumulators_.
template <class T, class TAG, class BaseBase, unsigned LEVEL>
struct DynamicAccumulatorWrapper
: public TAG::template Impl<T, BaseBase>
{
    static const unsigned level = LEVEL;

    typedef TAG Tag;
    typedef typename TAG::template Impl<T, BaseBase> Base;
    
    typedef typename Base::result_type           result_type;
    typedef typename Base::qualified_result_type qualified_result_type;

    void activate()
    {
        this->active_accumulators_.set<LEVEL>();
        ActivateDependencies<typename Tag::Dependencies::type>::exec(*this);
    }
     
    void reset()
    {
        if(this->active_accumulators_.test<LEVEL>())
            Base::reset();
        else
            BaseBase::reset();
    }
   
    unsigned int passesRequired() const
    {
        if(this->active_accumulators_.test<LEVEL>())
            return Base::passesRequired();
        else
            return BaseBase::passesRequired();
    }
        
    template <class Shape>
    void reshape(Shape const & s)
    {
        if(this->active_accumulators_.test<LEVEL>())
            Base::reshape(s);
        else
            BaseBase::reshape(s);
    }
   
    void operator+=(DynamicAccumulatorWrapper const & o)
    {
        if(this->active_accumulators_.test<LEVEL>())
            Base::operator+=(o);
        else
            BaseBase::operator+=(o);
    }
    
	qualified_result_type operator()() const
    {
        vigra_precondition(this->active_accumulators_.test<LEVEL>(),
            std::string("get(accumulator): attempt to access inactive statistic '") << typeid(Tag).name() << "'.");
        return Base::operator()();
    }
    
	void operator()(T const & t)
    {
        if(this->active_accumulators_.test<LEVEL>())
            Base::operator()(t);
        else
            BaseBase::operator()(t);
    }
    
    void operator()(T const & t, double weight)
    {
        if(this->active_accumulators_.test<LEVEL>())
            Base::operator()(t, weight);
        else
            BaseBase::operator()(t, weight);
    }
    
    void updatePass2(T const & t)
    {
        if(this->active_accumulators_.test<LEVEL>())
            Base::updatePass2(t);
        else
            BaseBase::updatePass2(t);
    }
    
    void updatePass2(T const & t, double weight)
    {
        if(this->active_accumulators_.test<LEVEL>())
            Base::updatePass2(t, weight);
        else
            BaseBase::updatePass2(t, weight);
    }
};

    // Generic reshape function (expands to a no-op when T has fixed shape, and to
    // the appropriate specialized call otherwise). Shape is an instance of MultiArrayShape<N>::type.
template <class T, class Shape>
void reshapeImpl(T &, Shape const &)
{}

template <class T, class Shape, class Initial>
void reshapeImpl(T &, Shape const &, Initial const & = T())
{}

template <unsigned int N, class T, class Alloc>
void reshapeImpl(MultiArray<N, T, Alloc> & a, typename MultiArrayShape<N>::type const & s, T const & initial = T())
{
    MultiArray<N, T, Alloc>(s, initial).swap(a);
}

template <class T, class Alloc>
void reshapeImpl(Matrix<T, Alloc> & a, Shape2 const & s, T const & initial = T())
{
    Matrix<T, Alloc>(s, initial).swap(a);
}

    // check if the accumulator chain A contains result_types that require runtime reshape
template <class A, class ResultType = typename A::result_type>
struct NeedsReshape
{
    typedef typename NeedsReshape<typename A::BaseType>::type type;
};

template <class A, unsigned int N, class T, class Alloc>
struct NeedsReshape<A, MultiArray<N, T, Alloc> >
{
    typedef VigraTrueType type;
};

template <class A, class T>
struct NeedsReshape<A, Matrix<T> >
{
    typedef VigraTrueType type;
};

template <>
struct NeedsReshape<AccumulatorBase, AccumulatorBase::result_type>
{
    typedef VigraFalseType type;
};

    // This accumulator is inserted on top of an accumulator chain to call reshape() 
    // when the first data item arrives. This is necessary if the shape of the result 
    // depends on the shape of the input and cannot be determined at compile time. 
    // The above NeedsReshape traits specify the types where this applies. If the chain
    // doesn't contain such types, ReshapeHelper will expand into a do-nothing version.
template <class T, class Base, class NeedsReshape=typename NeedsReshape<Base>::type>
struct ReshapeHelper
: public Base
{
    typedef VigraFalseType Tag;
    typedef Base BaseType;
    
    bool needs_reshape_;
    
    ReshapeHelper()
    : needs_reshape_(true)
    {}
    
    using Base::operator();
    
	void operator()(T const & t)
    {
        if(needs_reshape_)
        {
            Base::reshape(shape(t));
            needs_reshape_ = false;
        }
        Base::operator()(t);
    }
    
    void operator()(T const & t, double weight)
    {
        if(needs_reshape_)
        {
            Base::reshape(shape(t));
            needs_reshape_ = false;
        }
        Base::operator()(t, weight);
    }
    
    template <unsigned int N, class U, class Stride>
    typename MultiArrayShape<N>::type
    shape(MultiArrayView<N, U, Stride> const & a)
    {
        return a.shape();
    }
    
    template <class U, int N>
    Shape1
    shape(TinyVector<U, N> const & a)
    {
        return Shape1(N);
    }
};

template <class T, class Base>
struct ReshapeHelper<T, Base, VigraFalseType>
: public Base
{
    typedef VigraFalseType Tag;
    typedef Base BaseType;
};

    // helper classes to create an accumulator chain from a TypeList
template <class T, class Accumulators>
struct Compose
{
    typedef typename Accumulators::Head Tag; 
    typedef typename Compose<T, typename Accumulators::Tail>::type BaseType;
    typedef typename Tag::template Impl<T, BaseType> type;
};

template <class T> 
struct Compose<T, void> 
{ 
    typedef TypedAccumulatorBase<T> type; 
};

    // helper classes to create a dynamic accumulator chain from a TypeList
template <class T, class Accumulators, unsigned level=0>
struct DynamicCompose
{
    typedef typename Accumulators::Head Tag; 
    typedef typename DynamicCompose<T, typename Accumulators::Tail, level+1>::type BaseType;
    typedef DynamicAccumulatorWrapper<T, Tag, BaseType, level> type;
};

template <class T, unsigned level> 
struct DynamicCompose<T, void, level> 
{ 
    typedef TypedAccumulatorBase<T, level+1> type; 
};

} // namespace detail

    // create an accumulator chain containing the Selected statistics and their dependencies
template <class T, class Selected>
struct Accumulator
: public detail::ReshapeHelper<T, 
           typename detail::Compose<T, typename detail::AddDependencies<typename Selected::type>::type>::type>
{
    typedef typename detail::AddDependencies<typename Selected::type>::type Accumulators;
    typedef typename detail::Compose<T, Accumulators>::type BaseType;
    typedef VigraFalseType Tag;
};

    // create a dynamic accumulator chain containing the Selected statistics and their dependencies.
    // Statistics will only be computed if activate<Tag>() is called at runtime.
template <class T, class Selected>
struct DynamicAccumulator
: public detail::ReshapeHelper<T, 
           typename detail::DynamicCompose<T, typename detail::AddDependencies<typename Selected::type>::type>::type>
{
    typedef typename detail::AddDependencies<typename Selected::type>::type Accumulators;
    typedef typename detail::DynamicCompose<T, Accumulators>::type BaseType;
    typedef VigraFalseType Tag;
};

    // find the type in an accumulator chain holding the given Tag
template <class Tag, class Accumulator>
struct LookupTag
{
    typedef typename IsSameType<Tag, typename Accumulator::Tag>::type Found;
    typedef typename If<Found, Accumulator, typename LookupTag<Tag, typename Accumulator::BaseType>::type>::type type;
    // FIXME !!!!!!!!!!!!!!!!!!!!!!!!
    // typedef typename If<Found, typename Accumulator::qualified_result_type, 
    typedef typename If<Found, typename Accumulator::result_type, 
                               typename LookupTag<Tag, typename Accumulator::BaseType>::result_type>::type result_type;
};

template <class Tag>
struct LookupTag<Tag, AccumulatorBase>
{
    typedef AccumulatorBase type;
    typedef void result_type;
};

    // cast an accumulator chain to the type specified by Tag
template <class Tag, class Accumulator>
typename LookupTag<Tag, Accumulator>::type &
cast(Accumulator & a)
{
    return static_cast<typename LookupTag<Tag, Accumulator>::type &>(a);
}

template <class Tag, class Accumulator>
typename LookupTag<Tag, Accumulator>::type const &
cast(Accumulator const & a)
{
    return static_cast<typename LookupTag<Tag, Accumulator>::type const &>(a);
}

namespace detail {

template <class Tag, class Accumulator>
typename LookupTag<Tag, Accumulator>::result_type
getImpl(Accumulator const & a)
{
    return a();
}

template <class Tag>
void getImpl(AccumulatorBase const & a)
{
    vigra_precondition(false,
        std::string("get(accumulator): attempt to access inactive statistic '") << typeid(Tag).name() << "'.");
}

} // namespace detail

    // get the result of the accumulator specified by Tag
template <class Tag, class Accumulator>
typename LookupTag<Tag, Accumulator>::result_type
get(Accumulator const & a)
{
    return detail::getImpl<Tag>(cast<Tag>(a));
}

    // activate the dynamic accumulator specified by Tag
template <class Tag, class Accumulator>
void activate(Accumulator & a)
{
    cast<Tag>(a).activate();
}

#if 0
Accumulator<StridedPairPointer<...>, Bind<Data, Select<...> >,    // data statistics
                                     Bind<Coord, Select<...> >,    // coordinate statstics
                                     Bind<Coord, Data, Select<...> >  // weighted coordinate statistics
CoupledAccumulator<StridedTriplePointer<...>, Bind<1, Select<...> >,    // data statistics
                                            Bind<0, Select<...> >,    // coordinate statstics
                                            Bind<0, 1, Select<...> >  // weighted coordinate statistics
                                            Bind<1, 2, Select<...> >  // weighted data statistics
#endif

template <unsigned N, class T>
struct CoupledPointer
{
    typedef typename MultiArrayShape<N>::type Coord;
    typedef T value_type;
    
    Coord coord_;
    value_type value_;
    
    CoupledPointer(Coord const & c, value_type v)
    : coord_(c),
      value_(v)
    {}
};

template <class Handle, unsigned index>
struct CoupledHandleTraits
{
    typedef void type;
};

template <unsigned N, class T>
struct CoupledHandleTraits<CoupledPointer<N, T>, 0>
{
    typedef typename MultiArrayShape<N>::type type;
    static type exec(CoupledPointer<N, T> const & c)
    {
        return c.coord_;
    }
};

template <unsigned N, class T>
struct CoupledHandleTraits<CoupledPointer<N, T>, 1>
{
    typedef T type;
    static type exec(CoupledPointer<N, T> const & c)
    {
        return c.value_;
    }
};

template <unsigned INDEX, unsigned N, class T>
typename CoupledHandleTraits<CoupledPointer<N, T>, INDEX>::type
get(CoupledPointer<N, T> const & c)
{
    return CoupledHandleTraits<CoupledPointer<N, T>, INDEX>::exec(c);
}

template <unsigned INDEX, class Selected>
struct Simple
{
    static const unsigned index = INDEX;
    typedef VigraFalseType IsWeighted;
    typedef Selected Accumulators;
};

template <unsigned INDEX, unsigned WEIGHT_INDEX, class Selected>
struct Weighted
{
    static const unsigned index = INDEX;
    static const unsigned weight_index = WEIGHT_INDEX;
    typedef VigraTrueType IsWeighted;
    typedef Selected Accumulators;
};

template <class A, class IsWeighted=typename A::IsWeighted>
struct CallSelector
{
    template <class Accumulator, class CoupledHandle>
    static void pass1(Accumulator & a, CoupledHandle const & h)
    {
        a(get<A::index>(h));
    }
    template <class Accumulator, class CoupledHandle>
    static void pass2(Accumulator & a, CoupledHandle const & h)
    {
        a.updatePass2(get<A::index>(h));
    }
};

template <class A>
struct CallSelector<A, VigraTrueType>
{
    template <class Accumulator, class CoupledHandle>
    static void pass1(Accumulator & a, CoupledHandle const & h)
    {
        a(get<A::index>(h), get<A::weight_index>(h));
    }
    template <class Accumulator, class CoupledHandle>
    static void pass2(Accumulator & a, CoupledHandle const & h)
    {
        a.updatePass2(get<A::index>(h), get<A::weight_index>(h));
    }
};

template <class CoupledHandle, class Selected, int LEVEL>
struct ComposeCoupledAccumulator;

template <class CoupledHandle, class Head, class Tail, int LEVEL>
struct ComposeCoupledAccumulator<CoupledHandle, TypeList<Head, Tail>, LEVEL>
: public ComposeCoupledAccumulator<CoupledHandle, Tail, LEVEL+1>
{
    typedef ComposeCoupledAccumulator<CoupledHandle, Tail, LEVEL+1> BaseType;
    
    static const unsigned index = Head::index;
    static const int level = LEVEL;
    typedef typename CoupledHandleTraits<CoupledHandle, index>::type DataType;
    typedef Accumulator<DataType, typename Head::Accumulators> AccumulatorType;
    
    AccumulatorType value_;
    
    unsigned int passesRequired() const
    {
        return std::max(BaseType::passesRequired(), value_.passesRequired());
    }
    
    void operator+=(ComposeCoupledAccumulator const & o)
    {
        value_ += o.value_;
        BaseType::operator+=(o);
    }
    
    void operator()(CoupledHandle const & h)
    {
        CallSelector<Head>::pass1(value_, h);
        BaseType::operator()(h);
    }
    
    void updatePass2(CoupledHandle const & h)
    {
        CallSelector<Head>::pass2(value_, h);
        BaseType::updatePass2(h);
    }
    
    void reset()
    {
        value_.reset();
        BaseType::reset();
    }
};

template <class CoupledHandle, int LEVEL>
struct ComposeCoupledAccumulator<CoupledHandle, void, LEVEL>
{
    unsigned int passesRequired() const
    {
        return 0;
    }
    
    void operator+=(ComposeCoupledAccumulator const &)
    {}
    
    void operator()(CoupledHandle const &)
    {}
    
    void updatePass2(CoupledHandle const &)
    {}
    
    void reset()
    {}
};

template <class CoupledHandle, class A1=void, class A2=void, class A3=void, class A4=void, class A5=void>
struct CoupledAccumulator
: public ComposeCoupledAccumulator<CoupledHandle, typename MakeTypeList<A1, A2, A3, A4, A5>::type, 0>
{
    static const int level = -1;
    typedef ComposeCoupledAccumulator<CoupledHandle, typename MakeTypeList<A1, A2, A3, A4, A5>::type, 0> BaseType;
};

// template<int LEVEL, class Tag, class CoupledHandle, class Selected>
// typename LookupTag<Tag, typename ComposeCoupledAccumulator<CoupledHandle, Selected, LEVEL>::AccumulatorType>::type const &
// cast(ComposeCoupledAccumulator<CoupledHandle, Selected, LEVEL> const & a)
// {
    // return cast<Tag>(a.value_);
// }

template <int LEVEL, class Accumulator>
struct LookupLevel
{
    typedef typename IfBool<(LEVEL == Accumulator::level), 
               Accumulator, typename LookupLevel<LEVEL, typename Accumulator::BaseType>::type>::type type;
};

template <int LEVEL, class T, int ACTUAL_LEVEL>
struct LookupLevel<LEVEL, ComposeCoupledAccumulator<T,void,ACTUAL_LEVEL> >
{
    typedef void type;
};

template<int LEVEL, class Tag, class CoupledHandle, class A1, class A2, class A3, class A4, class A5>
typename LookupTag<Tag, typename LookupLevel<LEVEL, 
         CoupledAccumulator<CoupledHandle, A1, A2, A3, A4, A5> >::type::AccumulatorType>::type const &
cast(CoupledAccumulator<CoupledHandle, A1, A2, A3, A4, A5> const & a)
{
    typedef typename LookupLevel<LEVEL, CoupledAccumulator<CoupledHandle, A1, A2, A3, A4, A5> >::type LevelType;
    return cast<Tag>(((LevelType const &)a).value_);
}

template<int LEVEL, class Tag, class CoupledHandle, class A1, class A2, class A3, class A4, class A5>
typename LookupTag<Tag, typename LookupLevel<LEVEL, 
         CoupledAccumulator<CoupledHandle, A1, A2, A3, A4, A5> >::type::AccumulatorType>::result_type
get(CoupledAccumulator<CoupledHandle, A1, A2, A3, A4, A5> const & a)
{
    typedef typename LookupLevel<LEVEL, CoupledAccumulator<CoupledHandle, A1, A2, A3, A4, A5> >::type LevelType;
    return get<Tag>(((LevelType const &)a).value_);
}

//#endif

/****************************************************************************/
/*                                                                          */
/*                        the actual accumulators                           */
/*                                                                          */
/****************************************************************************/

template <class T>
struct AccumulatorTraits
{
    typedef T                                       type;
    typedef T                                       element_type;
    typedef typename NumericTraits<T>::RealPromote  element_promote_type;
    typedef T                                       MinmaxType;
    typedef element_promote_type                    SumType;
    typedef element_promote_type                    FlatCovarianceType;
    typedef element_promote_type                    CovarianceType;
};

template <class T, int N>
struct AccumulatorTraits<TinyVector<T, N> >
{
    typedef TinyVector<T, N>                             type;
    typedef T                                            element_type;
    typedef typename NumericTraits<T>::RealPromote       element_promote_type;
    typedef TinyVector<T, N>                             MinmaxType;
    typedef TinyVector<element_promote_type, N>          SumType;
    typedef TinyVector<element_promote_type, N*(N+1)/2>  FlatCovarianceType;
    typedef Matrix<element_promote_type>                 CovarianceType;
};

template <unsigned int N, class T, class Stride>
struct AccumulatorTraits<MultiArrayView<N, T, Stride> >
{
    typedef MultiArrayView<N, T, Stride>            type;
    typedef T                                       element_type;
    typedef typename NumericTraits<T>::RealPromote  element_promote_type;
    typedef MultiArray<N, T>                        MinmaxType;
    typedef MultiArray<N, element_promote_type>     SumType;
    typedef MultiArray<1, element_promote_type>     FlatCovarianceType;
    typedef Matrix<element_promote_type>            CovarianceType;
};

class Count
{
  public:
    typedef Select<> Dependencies;
    
    template <class T, class BASE>
    struct Impl
    : public BASE
    {
        typedef Count Tag;
        typedef BASE BaseType;

        typedef double result_type;
        typedef double qualified_result_type;
        
        result_type value_;
        
        Impl()
        : value_(0.0)
        {}
        
        void reset()
        {
            value_ = 0.0;
            BaseType::reset();
        }
        
        void operator+=(Impl const & o)
        {
            BaseType::operator+=(o);
            value_ += o.value_;
        }
    
        void operator()(T const & t)
        {
            BaseType::operator()(t);
            ++value_;
        }
        
        void operator()(T const & t, double weight)
        {
            BaseType::operator()(t, weight);
            value_ += weight;
        }
        
        result_type operator()() const
        {
            return value_;
        }
    };
};

class Minimum
{
  public:
    typedef Select<> Dependencies;
    
    template <class T, class BASE>
    struct Impl
    : public BASE
    {
        typedef Minimum Tag;
        typedef BASE BaseType;

        typedef typename AccumulatorTraits<T>::element_type element_type;
        typedef typename AccumulatorTraits<T>::MinmaxType   result_type;
        typedef result_type const &                         qualified_result_type;

        result_type value_;
        
        Impl()
        {
            value_ = NumericTraits<element_type>::max();
        }
        
        void reset()
        {
            value_ = NumericTraits<element_type>::max();
            BaseType::reset();
        }
    
        template <class Shape>
        void reshape(Shape const & s)
        {
            detail::reshapeImpl(value_, s, NumericTraits<element_type>::max());
            BaseType::reshape(s);
        }
        
       void operator+=(Impl const & o)
        {
            BaseType::operator+=(o);
            using namespace multi_math;
            value_ = min(value_, o.value_);
        }
    
        void operator()(T const & t)
        {
            BaseType::operator()(t);
            using namespace multi_math;
            value_ = min(value_, t);
        }
        
        void operator()(T const & t, double weight)
        {
            vigra_precondition(false, "Minimum accumulator does not support weights.");
        }
        
        qualified_result_type operator()() const
        {
            return value_;
        }
    };
};

class Maximum
{
  public:
    typedef Select<> Dependencies;
    
    template <class T, class BASE>
    struct Impl
    : public BASE
    {
        typedef Maximum Tag;
        typedef BASE BaseType;

        typedef typename AccumulatorTraits<T>::element_type element_type;
        typedef typename AccumulatorTraits<T>::MinmaxType   result_type;
        typedef result_type const &                         qualified_result_type;

        result_type value_;
        
        Impl()
        {
            value_ = NumericTraits<element_type>::min();
        }
        
        void reset()
        {
            value_ = NumericTraits<element_type>::min();
            BaseType::reset();
        }
    
        template <class Shape>
        void reshape(Shape const & s)
        {
            detail::reshapeImpl(value_, s, NumericTraits<element_type>::min());
            BaseType::reshape(s);
        }
        
        void operator+=(Impl const & o)
        {
            BaseType::operator+=(o);
            using namespace multi_math;
            value_ = max(value_, o.value_);
        }
    
        void operator()(T const & t)
        {
            BaseType::operator()(t);
            using namespace multi_math;
            value_ = max(value_, t);
        }
        
        void operator()(T const & t, double weight)
        {
            vigra_precondition(false, "Maximum accumulator does not support weights.");
        }
        
        qualified_result_type operator()() const
        {
            return value_;
        }
    };
};

class Sum
{
  public:
    typedef Select<> Dependencies;
     
    template <class T, class BASE>
    struct Impl
    : public BASE
    {
        typedef Sum Tag;
        typedef BASE BaseType;
        
        typedef typename AccumulatorTraits<T>::element_promote_type element_type;
        typedef typename AccumulatorTraits<T>::SumType              result_type;
        typedef result_type const &                                 qualified_result_type;

        result_type value_;
        
        Impl()
        : value_()  // call default constructor explicitly to ensure zero initialization
        {}
        
        void reset()
        {
            value_ = element_type();
            BaseType::reset();
        }
    
        template <class Shape>
        void reshape(Shape const & s)
        {
            detail::reshapeImpl(value_, s);
            BaseType::reshape(s);
        }
        
        void operator+=(Impl const & o)
        {
            BaseType::operator+=(o);
            value_ += o.value_;
        }
    
        void operator()(T const & t)
        {
            BaseType::operator()(t);
            value_ += t;
        }
        
        void operator()(T const & t, double weight)
        {
            BaseType::operator()(t, weight);
            value_ += weight*t;
        }
        
        qualified_result_type operator()() const
        {
            return value_;
        }
    };
};

class Mean
{
  public:
    typedef Select<Sum, Count> Dependencies;
    
    template <class T, class BASE>
    struct Impl
    : public BASE
    {
        typedef Mean Tag;
        typedef BASE BaseType;
        
        typedef typename AccumulatorTraits<T>::element_promote_type element_type;
        typedef typename AccumulatorTraits<T>::SumType              result_type;
        typedef result_type const &                                 qualified_result_type;

        mutable result_type value_;
        
        Impl()
        : value_()  // call default constructor explicitly to ensure zero initialization
        {}
        
        void reset()
        {
            value_ = element_type();
            BaseType::reset();
        }
    
        template <class Shape>
        void reshape(Shape const & s)
        {
            detail::reshapeImpl(value_, s);
            BaseType::reshape(s);
        }

        using BaseType::operator();
        
        qualified_result_type operator()() const
        {
			using namespace multi_math;
            value_ = get<Sum>(*this) / get<Count>(*this);
            return value_;
        }
    };
};

namespace detail
{

template <class SSD, class Sum1, class Sum2> 
void updateSSD(SSD & l, Sum1 const & s1, double n1, Sum2 const & s2, double n2 = 1.0)
{
    if(n1 != 0.0 && n2 != 0.0)
    {
        using namespace vigra::multi_math;
        l += n1 * n2 / (n1 + n2) * sq(s1 / n1 - s2 / n2);
    }
}

template <unsigned int N>
struct CentralMomentsHelper
{
    template <class Left, class Right>
    static void merge(Left &, Right const &)
    {
        vigra_precondition(false,
            "CentralMoment<N>::operator+=(): not implemented for N > 4.");
    }
};

template <>
struct CentralMomentsHelper<2u>
{
    template <class Accu>
    static void merge(Accu & l, Accu const & r)
    {
        updateSSD(l.value_, get<Sum>(l), get<Count>(l), get<Sum>(r), get<Count>(r));
        l.value_ += r.value_;
    }
};

template <>
struct CentralMomentsHelper<3u>
{
    template <class Accu>
    static void merge(Accu & l, Accu const & r)
    {
        using namespace vigra::multi_math;
        typedef typename LookupTag<Sum, Accu>::result_type SumType;
        double n1 = get<Count>(l);
        double n2 = get<Count>(r);
        double n = n1 + n2;
        double weight = n1 * n2 * (n1 - n2) / sq(n);
        SumType delta = get<Sum>(r) / n2 - get<Sum>(l) / n1;
        l.value_ += r.value_ + weight * pow(delta, 3) +
                       3.0 / n * delta * (  n1 * cast<CentralMoment<2> >(r).value_
                                          - n2 * cast<CentralMoment<2> >(l).value_);
    }
};

template <>
struct CentralMomentsHelper<4u>
{
    template <class Accu>
    static void merge(Accu & l, Accu const & r)
    {
        using namespace vigra::multi_math;
        typedef typename LookupTag<Sum, Accu>::result_type SumType;
        double n1 = get<Count>(l);
        double n2 = get<Count>(r);
        double n = n1 + n2;
        double n1_2 = sq(n1);
        double n2_2 = sq(n2);
        double n_2 = sq(n);
        double weight = n1 * n2 * (n1_2 - n1*n2 + n2_2) / n_2 / n;
        SumType delta = get<Sum>(r) / n2 - get<Sum>(l) / n1;
        l.value_ += r.value_ + weight * pow(delta, 4) +
                      6.0 / n_2 * sq(delta) * (  n1_2 * cast<CentralMoment<2> >(r).value_
                                               + n2_2 * cast<CentralMoment<2> >(l).value_ ) +
                      4.0 / n * delta * (  n1 * cast<CentralMoment<3> >(r).value_
                                         - n2 * cast<CentralMoment<3> >(l).value_);
    }
};

} // namsspace detail

template <unsigned int N>
class CentralMoment
{
  public:
    typedef Select<Mean, Count, typename IfBool<(N > 2), CentralMoment<N-1>, void>::type > Dependencies;
     
    template <class T, class BASE>
    struct Impl
    : public BASE
    {
        typedef CentralMoment<N> Tag;
        typedef BASE BaseType;
        
        typedef typename AccumulatorTraits<T>::element_promote_type element_type;
        typedef typename AccumulatorTraits<T>::SumType              result_type;
        typedef result_type                                         qualified_result_type;

        result_type value_;
        
        Impl()
        : value_()  // call default constructor explicitly to ensure zero initialization
        {}
        
        void reset()
        {
            value_ = element_type();
            BaseType::reset();
        }
    
        template <class Shape>
        void reshape(Shape const & s)
        {
            detail::reshapeImpl(value_, s);
            BaseType::reshape(s);
        }
        
        unsigned int passesRequired() const
        {
            return std::max(2u, BaseType::passesRequired());
        }
        
        void operator+=(Impl const & o)
        {
            detail::CentralMomentsHelper<N>::merge(*this, o);
            // this must be last because the above computation needs the old values
            BaseType::operator+=(o);
        }
    
        using BaseType::operator();
        
        void updatePass2(T const & t)
        {
            BaseType::updatePass2(t);
            using namespace vigra::multi_math;            
            value_ += pow(t - get<Sum>(*this) / get<Count>(*this), (int)N);
        }
        
        void updatePass2(T const & t, double weight)
        {
            BaseType::updatePass2(t, weight);
            using namespace vigra::multi_math;            
            value_ += weight*pow(t - get<Sum>(*this) / get<Count>(*this), (int)N);
        }
        
        qualified_result_type operator()() const
        {
            using namespace vigra::multi_math;
            return value_ / get<Count>(*this);
        }
    };
};

class Skewness
{
  public:
    typedef Select<CentralMoment<3> > Dependencies;
    
    template <class T, class BASE>
    struct Impl
    : public BASE
    {
        typedef Skewness Tag;
        typedef BASE BaseType;
        
        typedef typename LookupTag<CentralMoment<3>, Impl>::result_type result_type;
        typedef result_type                                             qualified_result_type;

        using BaseType::operator();
        
        qualified_result_type operator()() const
        {
			using namespace multi_math;
            return sqrt(get<Count>(*this)) * 
                     cast<CentralMoment<3> >(*this).value_ / pow(cast<CentralMoment<2> >(*this).value_, 1.5);
        }
    };
};

class Kurtosis
{
  public:
    typedef Select<CentralMoment<4> > Dependencies;
    
    template <class T, class BASE>
    struct Impl
    : public BASE
    {
        typedef Kurtosis Tag;
        typedef BASE BaseType;
        
        typedef typename LookupTag<CentralMoment<4>, Impl>::result_type result_type;
        typedef result_type                                             qualified_result_type;

        using BaseType::operator();
        
        qualified_result_type operator()() const
        {
			using namespace multi_math;
            return get<Count>(*this) * 
                     cast<CentralMoment<4> >(*this).value_ / sq(cast<CentralMoment<2> >(*this).value_);
        }
    };
};

class SumSquaredDifferences
{
  public:
    typedef Select<Mean, Count> Dependencies;
    
    template <class T, class BASE>
    struct Impl
    : public BASE
    {
        typedef SumSquaredDifferences Tag;
        typedef BASE BaseType;
        
        typedef typename AccumulatorTraits<T>::element_promote_type  element_type;
        typedef typename AccumulatorTraits<T>::SumType               result_type;
        typedef result_type const &                                  qualified_result_type;
       
        result_type value_;
        
        Impl()
        : value_()  // call default constructor explicitly to ensure zero initialization
        {}
        
        void reset()
        {
            value_ = element_type();
            BaseType::reset();
        }
    
        template <class Shape>
        void reshape(Shape const & s)
        {
            detail::reshapeImpl(value_, s);
            BaseType::reshape(s);
        }
        
        void operator+=(Impl const & o)
        {
            detail::CentralMomentsHelper<2>::merge(*this, o);
            // this must be last because the above computation needs the old values
            BaseType::operator+=(o);
        }
    
        void operator()(T const & t)
        {
            detail::updateSSD(value_, get<Sum>(*this), get<Count>(*this), t);
            BaseType::operator()(t);
        }
        
        void operator()(T const & t, double weight)
        {
            detail::updateSSD(value_, get<Sum>(*this), get<Count>(*this), t, weight);
            BaseType::operator()(t, weight);
        }
        
        qualified_result_type operator()() const
        {
            return value_;
        }
    };
};

typedef SumSquaredDifferences SSD;

class Variance
{
  public:
    typedef Select<SumSquaredDifferences, Count> Dependencies;
    
    template <class T, class BASE>
    struct Impl
    : public BASE
    {
        typedef Variance Tag;
        typedef BASE BaseType;
        
        typedef typename LookupTag<SumSquaredDifferences, Impl>::result_type result_type;
        typedef result_type                              qualified_result_type;


        using BaseType::operator();
        
        qualified_result_type operator()() const
        {
			using namespace multi_math;
            return get<SumSquaredDifferences>(*this) / get<Count>(*this);
        }
    };
};

class StdDev
{
  public:
    typedef Select<Variance> Dependencies;
    
    template <class T, class BASE>
    struct Impl
    : public BASE
    {
        typedef StdDev Tag;
        typedef BASE BaseType;
        
        typedef typename LookupTag<Variance, Impl>::result_type result_type;
        typedef result_type                              qualified_result_type;

        using BaseType::operator();
        
        qualified_result_type operator()() const
        {
			using namespace multi_math;
            return sqrt(get<SumSquaredDifferences>(*this) / get<Count>(*this));
        }
    };
};

class UnbiasedVariance
{
  public:
    typedef Select<SumSquaredDifferences, Count> Dependencies;
    
    template <class T, class BASE>
    struct Impl
    : public BASE
    {
        typedef UnbiasedVariance Tag;
        typedef BASE BaseType;
        
        typedef typename LookupTag<SumSquaredDifferences, Impl>::result_type result_type;
        typedef result_type                              qualified_result_type;

        using BaseType::operator();
        
        qualified_result_type operator()() const
        {
			using namespace multi_math;
            return get<SumSquaredDifferences>(*this) / (get<Count>(*this) - 1.0);
        }
    };
};

class UnbiasedStdDev
{
  public:
    typedef Select<UnbiasedVariance> Dependencies;
    
    template <class T, class BASE>
    struct Impl
    : public BASE
    {
        typedef UnbiasedStdDev Tag;
        typedef BASE BaseType;
        
        typedef typename LookupTag<Variance, Impl>::result_type result_type;
        typedef result_type                              qualified_result_type;

        using BaseType::operator();
        
        qualified_result_type operator()() const
        {
			using namespace multi_math;
            return sqrt(get<SumSquaredDifferences>(*this) / (get<Count>(*this) - 1.0));
        }
    };
};

namespace detail {

template <class Scatter, class Sum>
void updateFlatScatterMatrix(Scatter & sc, Sum const & s, double w)
{
    int size = s.size();
    for(MultiArrayIndex j=0, k=0; j<size; ++j)
        for(MultiArrayIndex i=j; i<size; ++i, ++k)
            sc[k] += w*s[i]*s[j];
}

template <class Sum>
void updateFlatScatterMatrix(double & sc, Sum const & s, double w)
{
    sc += w*s*s;
}

template <class Cov, class Scatter>
void flatScatterMatrixToCovariance(Cov & cov, Scatter const & sc, double n)
{
    int size = cov.shape(0), k=0;
    for(MultiArrayIndex j=0; j<size; ++j)
    {
        cov(j,j) = sc[k++] / n;
        for(MultiArrayIndex i=j+1; i<size; ++i)
        {
            cov(i,j) = sc[k++] / n;
            cov(j,i) = cov(i,j);
        }
    }
}

template <class Scatter>
void flatScatterMatrixToCovariance(double & cov, Scatter const & sc, double n)
{
    cov = sc / n;
}

} // namespace detail

// we only store the flattened upper triangular part of the scatter matrix
class FlatScatterMatrix
{
  public:
    typedef Select<Mean, Count> Dependencies;
    
    template <class T, class BASE>
    struct Impl
    : public BASE
    {
        typedef FlatScatterMatrix Tag;
        typedef BASE BaseType;
        
        typedef typename AccumulatorTraits<T>::element_promote_type  element_type;
        typedef typename AccumulatorTraits<T>::FlatCovarianceType    result_type;
        typedef result_type const &                                  qualified_result_type;
       
        typedef typename AccumulatorTraits<T>::SumType        SumType;

        result_type value_;
        SumType     diff_;
        
        Impl()
        : value_(),  // call default constructor explicitly to ensure zero initialization
          diff_()
        {}
        
        void reset()
        {
            value_ = element_type();
            BaseType::reset();
        }
    
        template <class Shape>
        void reshape(Shape const & s)
        {
            int size = prod(s);
            detail::reshapeImpl(value_, Shape1(size*(size+1)/2));
            detail::reshapeImpl(diff_, s);
            BaseType::reshape(s);
        }
        
        void operator+=(Impl const & o)
        {
            compute(get<Mean>(o), get<Count>(o));
            value_ += o.value_;
            // this must be last because the above computation needs the old values
            BaseType::operator+=(o);
        }
    
        void operator()(T const & t)
        {
            compute(t);
            // this must be last because the above computation needs the old values
            BaseType::operator()(t);
        }
        
        void operator()(T const & t, double weight)
        {
            compute(t, weight);
            // this must be last because the above computation needs the old values
            BaseType::operator()(t, weight);
        }
        
        qualified_result_type operator()() const
        {
            return value_;
        }
        
      private:
        void compute(T const & t, double weight = 1.0)
        {
            double old_count = get<Count>(*this);
            if(old_count != 0.0)
            {
                using namespace vigra::multi_math;
                diff_ = get<Sum>(*this) / old_count - t;
                weight = old_count / (old_count + weight) * weight;
                detail::updateFlatScatterMatrix(value_, diff_, weight);
            }
        }
    };
};

class Covariance
{
  public:
    typedef Select<FlatScatterMatrix, Count> Dependencies;
    
    template <class T, class BASE>
    struct Impl
    : public BASE
    {
        typedef Covariance Tag;
        typedef BASE BaseType;
        
        typedef typename AccumulatorTraits<T>::element_promote_type  element_type;
        typedef typename AccumulatorTraits<T>::CovarianceType        result_type;
        typedef result_type const &                                  qualified_result_type;

        mutable result_type value_;
        
        Impl()
        : value_()  // call default constructor explicitly to ensure zero initialization
        {}
        
        void reset()
        {
            value_ = element_type();
            BaseType::reset();
        }
    
        template <class Shape>
        void reshape(Shape const & s)
        {
            int size = prod(s);
            detail::reshapeImpl(value_, Shape2(size,size));
            BaseType::reshape(s);
        }
        
        using BaseType::operator();
        
        qualified_result_type operator()() const
        {
            detail::flatScatterMatrixToCovariance(value_, cast<FlatScatterMatrix>(*this).value_, get<Count>(*this));
            return value_;
        }
    };
};

class CovarianceEigensystem
{
  public:
    typedef Select<Covariance> Dependencies;
    
    template <class T, class BASE>
    struct Impl
    : public BASE
    {
        typedef CovarianceEigensystem Tag;
        typedef BASE BaseType;
        
        typedef typename AccumulatorTraits<T>::element_promote_type        element_type;
        typedef typename AccumulatorTraits<T>::SumType                     EigenvalueType;
        typedef typename AccumulatorTraits<T>::CovarianceType              EigenvectorType;
        typedef std::pair<EigenvalueType const &, EigenvectorType const &> result_type;
        typedef result_type                                                qualified_result_type;

        mutable EigenvalueType eigenvalues_;
        mutable EigenvectorType eigenvectors_;
        
        Impl()
        : eigenvalues_(),  // call default constructor explicitly to ensure zero initialization
          eigenvectors_()
        {}
        
        void reset()
        {
            eigenvalues_ = element_type();
            eigenvectors_ = element_type();
            BaseType::reset();
        }
    
        template <class Shape>
        void reshape(Shape const & s)
        {
            int size = prod(s);
            detail::reshapeImpl(eigenvalues_, Shape2(size,1));
            detail::reshapeImpl(eigenvectors_, Shape2(size,size));
            BaseType::reshape(s);
        }
        
        using BaseType::operator();
        
        qualified_result_type operator()() const
        {
            compute(get<Covariance>(*this), eigenvalues_, eigenvectors_);
            return result_type(eigenvalues_, eigenvectors_);
        }
        
      private:
        template <class Cov, class EW, class EV>
        static void compute(Cov const & cov, EW & ew, EV & ev)
        {
            // create a view because EW could be a TinyVector
            MultiArrayView<2, element_type> ewview(Shape2(cov.shape(0), 1), &ew[0]);
            symmetricEigensystem(cov, ewview, ev);
        }
        
        static void compute(double cov, double & ew, double & ev)
        {
            ew = cov;
            ev = 1.0;
        }
    };
};

class UnbiasedCovariance
{
  public:
    typedef Select<FlatScatterMatrix, Count> Dependencies;
    
    template <class T, class BASE>
    struct Impl
    : public BASE
    {
        typedef UnbiasedCovariance Tag;
        typedef BASE BaseType;
        
        typedef typename AccumulatorTraits<T>::element_promote_type  element_type;
        typedef typename AccumulatorTraits<T>::CovarianceType        result_type;
        typedef result_type const &                                  qualified_result_type;

        mutable result_type value_;
        
        Impl()
        : value_()  // call default constructor explicitly to ensure zero initialization
        {}
        
        void reset()
        {
            value_ = element_type();
            BaseType::reset();
        }
    
        template <class Shape>
        void reshape(Shape const & s)
        {
            int size = prod(s);
            detail::reshapeImpl(value_, Shape2(size,size));
            BaseType::reshape(s);
        }
        
        using BaseType::operator();
        
        qualified_result_type operator()() const
        {
            detail::flatScatterMatrixToCovariance(value_, cast<FlatScatterMatrix>(*this).value_, get<Count>(*this)-1.0);
            return value_;
        }
    };
};

}} // namespace vigra::acc1

#endif // VIGRA_ACCUMULATOR_HXX
