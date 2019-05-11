#include <ostream>


// ==========================================================================
//
// the value type
//
// ==========================================================================

struct xy {
   const int x;
   const int y;
   
   constexpr xy(): x( 0 ), y( 0 ){}
   
   constexpr xy( int x, int y ): x( x ), y( y ){}
   
   constexpr auto operator+( const xy & right ) const {
      return xy( x + right.x, y + right.y );
   }
   
   constexpr auto operator-( const xy & right ) const {
      return xy( x - right.x, y - right.y );
   }
   
   constexpr auto operator==( const xy & right ) const {
      return ( x == right.x ) && ( y == right.y );
   }
   
};

template< 
   typename          _value_type,
   typename          _dimension_type,
   _dimension_type   _dimension
>
struct united_value {
   
   using value_type                 = _value_type;
   using dimension_type             = _dimension_type;
   static auto constexpr dimension  = _dimension;

   value_type value;

   united_value(){}
   constexpr united_value( value_type value ): value( value ){}
};


// ==========================================================================
//
// v * v
//
// multiplication
//
// v<n,a> * v<m,b> -> v<n+m,a*b> | a*b
//
// The dimensions are added, the values are multiplied.
// When the dimension is zero, the result is a naked value.
//
// ==========================================================================

template<
   typename              left_value_type, 
   typename              right_value_type, 
   typename              dimension_type,
   const dimension_type  right_dimension
>
constexpr auto operator*( 
   const left_value_type & left,
   const united_value< 
      right_value_type, dimension_type, right_dimension > & right
){
   return 
      united_value< 
         decltype( left * right.value ), 
         dimension_type,
         right_dimension
      >( left.value * right.value );
}

template<
   typename              left_value_type, 
   typename              right_value_type, 
   typename              dimension_type,
   const dimension_type  left_dimension,
   const dimension_type  right_dimension
>
constexpr auto operator*( 
   const united_value< 
      left_value_type,  dimension_type, left_dimension > & left,
   const united_value< 
      right_value_type, dimension_type, right_dimension > & right
){
   return 
      united_value< 
         decltype( left.value * right.value ), 
         dimension_type,
         left_dimension + right_dimension
      >( left.value * right.value );
}


auto m = united_value< int, xy, xy( 1, 0 ) >( 1 );
auto s = united_value< int, xy, xy( 0, 1 ) >( 1 );

int main(int argc, char **argv){
   auto a = 10 * m;
   auto b = 20 * s;
   auto c = a * b;
   a = c;
   
   
   
   hwlib::cout << "Hello world\n";
}
