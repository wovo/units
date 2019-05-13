#include <iostream>


// ==========================================================================
//
// a dimesion type
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
   
   constexpr auto operator!=( const xy & right ) const {
      return ( x != right.x ) || ( y != right.y );
   }
   
   template< typename T >
   friend T & operator<<( T & left, const xy & right ){
      return left << "[" << right.x << "," << right.y << "]";
   }
   
};


// ==========================================================================
//
// the value_ad_dimension type
//
// ==========================================================================

template< typename T >
concept bool is_dimension = requires ( T a, T b ){  
   { T()     } -> T;
   { T( a )  } -> T;
   { a + b   } -> T;
   { a - b   } -> T;
   { a == b  } -> bool;
   { a != b  } -> bool;
};

template< typename T >
concept bool is_value = requires ( T a ){  
   { T()     } -> T;
   { T( a )  } -> T;
};

template< 
   is_value               _value_type,
   is_dimension           _dimension_type,
   const _dimension_type  _dimension
>
struct value_and_dimension {
   
   static const bool is_value_and_dimension  = true;
   
   using value_type                 = _value_type;
   using dimension_type             = _dimension_type;
   static auto constexpr dimension  = _dimension;

   value_type value;

   value_and_dimension(){}
   constexpr value_and_dimension( value_type value ): 
      value( value )
   {}
   
   template< typename T >
   requires requires (
      T & left,
      value_type value,
      dimension_type dimension
   ){   
      ( left << value ) -> T;
      ( left << ':' ) -> T;
      ( left << dimension ) -> T;
   }
   friend T & operator<<( 
      T & left, 
      const value_and_dimension & right 
   ){
      return left << right.value << ':' << right.dimension;
   }   
};

template< typename T>
concept bool is_value_and_dimension = requires ( T a ) {  
   is_value< T >;
   T::is_value_and_dimension;
   T::dimension;
};

// ==========================================================================
//
// addition & subtraction
//
// v<n,a>  +  v<m,b>  ->  v<n,a+b> 
// v<n,a>  -  v<m,b>  ->  v<n,a+b> 
//
// ==========================================================================

template<
   is_value_and_dimension  left_type, 
   is_value_and_dimension  right_type
>
constexpr auto operator+( 
   const left_type   & left,
   const right_type  & right
) requires 
   
   requires(
   left_type  t,
   right_type u
){ 
   t.value + u.value;
}

   ( left_type::dimension == right_type::dimension )

{
   return 
      value_and_dimension< 
         decltype( left.value + right.value ), 
         decltype( left_type::dimension + right_type::dimension ),
         left_type::dimension + right_type::dimension
      >( left.value * right.value );
}


// ==========================================================================
//
// multiplication
//
// a       *  v<m,b>  ->  v<m,a*b> 
// v<n,a>  *  b       ->  v<n,a*b> 
// v<n,a>  *  v<m,b>  ->  v<n+m,a*b> | a*b
//
// ==========================================================================

template<
   typename                 left_type, 
   is_value_and_dimension  right_type
>
constexpr auto operator*( 
   const left_type   & left,
   const right_type  & right
) requires requires(
   left_type  t,
   right_type u
){ 
   t * u.value;
}{
   return value_and_dimension< 
      decltype( left * right.value ), 
      typename right_type::dimension_type,
      right_type::dimension
   >( left * right.value );
}

/*
template<
   is_value_and_dimension  left_type, 
   typename                 right_type
>
constexpr auto operator*( 
   const left_type   & left,
   const right_type  & right
) requires requires(
   left_type  t,
   right_type u
){ 
   t.value * u;
}{
   return value_and_dimension< 
      decltype( left.value * right ), 
      typename left_type::dimension_type,
      left_type::dimension
   >( left.value * right );
}
*/

template<
   is_value_and_dimension  left_type, 
   is_value_and_dimension  right_type
>
constexpr auto operator*( 
   const left_type   & left,
   const right_type  & right
) requires requires(
   left_type  t,
   right_type u
){ 
   t.value * u.value;
}

{
   return 
      value_and_dimension< 
         decltype( left.value * right.value ), 
         decltype( left_type::dimension + right_type::dimension ),
         left_type::dimension + right_type::dimension
      >( left.value * right.value );
}



// ==========================================================================

auto m = value_and_dimension< int, xy, xy( 1, 0 ) >( 1 );
auto s = value_and_dimension< int, xy, xy( 0, 1 ) >( 1 );

int main(int argc, char **argv){
   auto a = 10 * m;
   auto b = 20 * s;
   auto c = a * b;
//   auto d = a / a;
//   auto e = "hello" * a;
   
   std::cout << a << "\n";
   std::cout << b << "\n";
   std::cout << c << "\n";
//   std::cout << d << "\n";
//   std::cout << c / (m*s) << "\n";
   
   //hwlib::cout << "Hello world\n";
}
