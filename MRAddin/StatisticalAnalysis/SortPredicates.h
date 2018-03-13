#ifndef MR_STATISTICAL_ANALYSIS_SORT_PREDICATES_H
#define MR_STATISTICAL_ANALYSIS_SORT_PREDICATES_H

#ifndef LACKS_PRAGMA_ONCE
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace mr 
{ 
    namespace statistical_analysis
    {
////////////////////////////////////////////////////////////////////////////////

///
/// Sort by "when" then by "what"
/// i.e. result in blocks where each block has the same "when"
/// e.g. Sorted by "Dt" then by "Mkt"
///
template <typename Ptr>
class SimultaneousPred
:   public std::binary_function<Ptr, Ptr, bool>
{
public:
    bool operator()(Ptr lhs, Ptr rhs) const
    {
        return ( lhs->when() != rhs->when() )
            ?   lhs->when() < rhs->when()
            :   lhs->what() < rhs->what();
    }
};
////////////////////////////////////////////////////////////////////////////////

///
/// Sort by "what" then by "when"
/// i.e. result in blocks where each block has the same "what"
/// e.g. Sorted by "Mkt" then by "Dt"
///
template <typename Ptr>
class CongruousPred
:   public std::binary_function<Ptr, Ptr, bool>
{
public:
    bool operator()(Ptr lhs, Ptr rhs) const
    {
        return ( lhs->what() != rhs->what() )
            ?   lhs->what() < rhs->what()
            :   lhs->when() < rhs->when();
    }
};
////////////////////////////////////////////////////////////////////////////////
template <typename Ptr>
class PartitionPred
:   public std::binary_function<Ptr, Ptr, bool>
{
public:
    bool operator()(Ptr lhs, Ptr rhs) const
    {
        return lhs->key() < rhs->key();
    }
};
////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
////////////////////////////////////////////////////////////////////////////////
#endif // MR_STATISTICAL_ANALYSIS_SORT_PREDICATES_H
////////////////////////////////////////////////////////////////////////////////
