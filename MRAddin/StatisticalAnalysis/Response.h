#ifndef MR_STATISTICAL_ANALYSIS_RESPONSE_H
#define MR_STATISTICAL_ANALYSIS_RESPONSE_H

#ifndef LACKS_PRAGMA_ONCE
#pragma once
#endif

#include "Infrastructure/MathUtils.h"

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace mr 
{ 
    namespace statistical_analysis
    {
////////////////////////////////////////////////////////////////////////////////
using namespace boost;
////////////////////////////////////////////////////////////////////////////////
class Response
{
public:
//  Lifetime:
               
	Response( ) 
		: _index  ( 0 ),
		_value  ( infrastructure::math::NAN_NOT_SET )
	{
	}

    explicit    Response( double value )
		:   _index  ( 0 ),
		_value  ( value )
	{
	}
	
	Response( double value, uint32_t index )
		:   _index  ( index ),
			_value  ( value )
	{
	}
	
	~Response( ) {};

//  Commands:
    void operator=  ( double value ){_value = value;}
	void operator*= ( double multiplier ) { _value *= multiplier; }

//  Queries:
    uint32_t    index       ( ) const {return _index;}
    double      operator()  ( ) const {return _value;}

private:
    uint32_t    _index;
    double      _value;
};
////////////////////////////////////////////////////////////////////////////////
    }   // namespace statistical_analysis
}   // namespace mr
////////////////////////////////////////////////////////////////////////////////
#endif // MR_STATISTICAL_ANALYSIS_RESPONSE_H
////////////////////////////////////////////////////////////////////////////////
