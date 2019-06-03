
// ==========================================================================
//
// a dimension type
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
      
  constexpr auto equal( const xy & right ) const {
      return true; // ( x == right.x ) && ( y == right.y );
   }
   
  constexpr auto operator==( const xy & right ) const {
      return true; // ( x == right.x ) && ( y == right.y );
   }
   
   constexpr auto operator!=( const xy & right ) const {
      return ( x != right.x ) || ( y != right.y );
   }
   
   template< typename S >
   friend S & operator<<( S & left, const xy & right ){
      return left << "[" << right.x << "," << right.y << "]";
   }
   
};

// ==========================================================================
//
// unit
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

template<
   typename base,
   typename exponents_type,
   exponents exponents_values
>   
class unit {
private:

   base value;
   
public:   

   unit(): value( 1 ){}
   
   // construct
   // assign
   // add 
   // subtract
   // multiply
   
   template<
      typename right_type,
   >
   constexpr auto operator*( 
      const right_type & right
   ) const {
      return 
         unit< 
            decltype( left.value * right ), 
            exponents_type,
            exponent_values
         >( value * right );
   }
   
   // reverse multiply

   template<
      typename right_base,
      typename right_exponents_type,
      typename right_exponents_values
   >
   constexpr auto operator*( 
      const unit< right_base, right_exponents_type, right_exponent_values > 
         & right
   ) const {
      return 
         unit< 
            decltype( left.value * right.value ), 
            decltype( exponent_values + right_exponent_values ),
            exponent_values + right_exponent_values
         >( value * right.value );
   }

   
   // divide
   // compare

};

// create new one with different base


// ==========================================================================
//
// main
//
// ==========================================================================

constexpr auto m = unit< int, const xy, xy( 1, 0 ) >();
constexpr auto s = unit< int, const xy, xy( 0, 1 ) >();

void main(){
   auto a = m * 1;
}