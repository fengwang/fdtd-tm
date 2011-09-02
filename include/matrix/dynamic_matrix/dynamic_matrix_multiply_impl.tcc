#if 0
	
	ALGORITHM FOR:
	
	self_type &
	operator *= ( const self_type& other );
	with this[R]{C], other[C][OC]


	//ALGORITHM DETAILS:
		//threshold should be determined from experiments
	0)  if ( ( max(all dims) < threshold ) || min(all dims) == 1 ) ) 
		do direct multiplicaiton

		//case row is odd
	1) 	else if (row() & 1)
		{
			//<1>
			if (row() & 2)
			{
				append one empty row to the matrix from downside 
					[ new_this <- this ]
				do the multiplication 
					[new_ans <- new_this * other ]
				remove the last row to generate ans
					[ ans <- new_ans ]	
			}
			//<2>
			else
			{
				cut last row from the matrix, get two new matrices 	
					[ new_this <- this ]
					[ last_row <- this ]
				do the multiplications 	
					[ new_ans <- new_this * other ]
					[ last_row_ans <- last_row * other ]
				 merge the two matrices to generate the ans
				 	[ ans <- new_ans | last_row_ans ]
					[ i.e. last_row_ans appended to new_ans as the last row ]
			}
		}
	
		//case col is odd
	2) 	else if (col() & 1)
		{
			//<1>
			if ( col() & 2 )
			{
				append one emtpy col to this from right side
					[new_this <- this]
				append one empty row to other from downside
					[new_other <- other]
				do the multiplication
					[ans <- new_this * new_other]
			}
			//<2>
			else
			{
				cut last col of this from right side
					[new_this <- this]
					[last_col <- this]
				cut last row of other from downside
					[new_other <- other]
					[last_row <- other]
				do the multiplicaitons
					[new_ans <- new_this * new_other]
					[res_col_row <- last_col * last_row]
				do the addition to generate ans
					[ans <- new_ans + res_col_row]
			}
		}

		//case other.col is odd
	3) 	else if ( other.col() & 1 )
		{
			//<1>
			if ( other.col() & 2 )
			{
				append one empty col to other from right side
					[new_other <- other]
				do the multiplicaiton
					[new_ans = this * other]
				remove the last col to generate the ans
					[ans <- new_ans]
			}
			//<2>
			else
			{
				cut the last col from other get two new matrices
					[new_other <- other]
					[last_col <- other]
				do the multications
					[new_ans <- this * new_other]
					[last_col_ans <- this * last_col]
				merge the two matrices
					[ans <- new_ans|last_col_ans]
					[i.e. last_col_ans as the last col of]
			}
		}

		//all dims even, using strassen algorithm
	4) else
		{
			//strassen algorithm 

			( a_00 a_01 )   ( b_00 b_01 )    ( c_00 c_01 )
			(           ) * (           )  = (           )
			( a_10 a_11 )   ( b_10 b_11 )    ( c_10 c_11 )

			Q_0 = ( a_00 + a_11 ) * ( b_00 + b_11 )
			Q_1 = ( a_10 + a_11 ) * b_00
			Q_2 = a_00 * ( b_01 - b_11 )
			Q_3 = a_11 * ( -b_00 + b_10 )
			Q_4 = ( a_00 + a_01 ) * b_11
			Q_5 = ( -a_00 + a_10 ) * ( b_00 + b_10 )
			Q_6 = ( a_01 - a_11 ) * ( b_10 + b_11 )
			
			c_00 = Q_0 + Q_3 - Q_4 +Q_6
			c_10 = Q_1 + Q_3
			c_01 = Q_2 + Q_4
			c_11 = Q_0 - Q_1 +Q_2 + Q_5
		}

#endif

	private:

	size_type
	max_dim() const
	{
		return std::max( row(), col() );
	}

	size_type 
	min_dim() const
	{
		return std::min( row(), col() );
	}

	self_type&
	direct_multiply( const self_type& other )
	{
		self_type tmp(row(), other.col());
        for (size_type i = 0; i < row(); ++i)
        {
            for (size_type j = 0; j < other.col(); ++j)
            {
                tmp[i][j] = std::inner_product(
												   row_begin(i), row_end(i),
												   other.col_begin(j), value_type(0)
											   );
            }
        }
        *this = tmp;
        return *this;	
	}

#if 0
	1) 	else if (row() & 1)
		{
			//<1>
			if (row() & 2)
			{
				append one empty row to the matrix from downside 
					[ new_this <- this ] [R+1,C]
				do the multiplication 
					[new_ans <- new_this * other ] [R+1, OC]
				remove the last row to generate ans
					[ ans <- new_ans ]	
			}
#endif 
	self_type&
	rr1( const self_type& other )
	{
		const self_type new_this = *this && value_type(0);

		const self_type 
		new_ans = new_this * other;

		const self_type 
		ans( 	new_ans,
				range_type( 0, row() ),
				range_type( 0, other.col() )
		   );

		*this = ans;
		return *this;
	}

#if 0
	1) 	else if (row() & 1)
		{
			//<1>
			if (row() & 2) {...}
			//<2>
			else
			{
				cut last row from the matrix, get two new matrices 	
					[ new_this <- this ] [R-1,C]
					[ last_row <- this ] [1,C]
				do the multiplications 	
					[ new_ans <- new_this * other ] [R-1,OC]
					[ last_row_ans <- last_row * other ] [1,OC]
				 merge the two matrices to generate the ans
				 	[ ans <- new_ans | last_row_ans ] [R,OC]
					[ i.e. last_row_ans appended to new_ans as the last row ]
			}
		}
#endif

	self_type&
	rr2( const self_type& other )
	{
		const self_type 
		new_this( 	*this,
					range_type( 0, row()-1 ),
					range_type( 0, col() )
				);

		const self_type 
		last_row( 	*this,
					range_type( row()-1, row() ),
					range_type( 0, col() )
				);
		
		const self_type 
		new_ans = new_this * other;
		
		const self_type 
		last_row_ans = last_row * other;
		
		const self_type
		ans = new_ans && last_row_ans;

		*this = ans;
		return *this;
	}

#if 0
		//case col is odd
	2) 	else if (col() & 1)
		{
			//<1>
			if ( col() & 2 )
			{
				append one emtpy col to this from right side
					[new_this <- this] [R,C+1]
				append one empty row to other from downside
					[new_other <- other] [C+1,OC]
				do the multiplication
					[ans <- new_this * new_other] [R,OC]
			}
			//<2>
			else {...}
#endif
	self_type&
	cc1( const self_type& other )
	{
		const self_type 
		new_this = *this || value_type(0);

		self_type new_other( col()+1, other.col() );
		std::copy( other.begin(), other.end(), new_other.begin() );

		const self_type ans = new_this * new_other;

		*this = ans;
		return *this;
	}

#if 0
		//case col is odd
	2) 	else if (col() & 1)
		{
			//<1>
			if ( col() & 2 ) {...}
			//<2>
			else
			{
				cut last col of this from right side
					[new_this <- this] [R,C-1]
					[last_col <- this] [R,1]
				cut last row of other from downside
					[new_other <- other] [C-1,OC]
					[last_row <- other]  [1, OC]
				do the multiplicaitons
					[new_ans <- new_this * new_other]    [R,OC]
					[res_col_row <- last_col * last_row] [R,OC]
				do the addition to generate ans
					[ans <- new_ans + res_col_row] [R,OC]
			}
#endif
	self_type&
	cc2( const self_type& other )
	{   
		//[new_this <- this] [R,C-1]
		const self_type 
		new_this( 	*this,
					range_type( 0, row() ),
					range_type( 0, col()-1 )
				);
		//[last_col <- this] [R,1]
		const self_type 
		last_col( 	*this,
					range_type( 0, row() ),
					range_type( col()-1, col() )
				);

		//[new_other <- other] [C-1,OC]
		const self_type
		new_other( 	other,
					range_type( 0, other.row()-1 ),
					range_type( 0, other.col() )
				 );
					
		//[last_row <- other]  [1, OC]
		const self_type
		last_row( 	other,
					range_type( other.row()-1, other.row() ),
					range_type( 0, other.col() )
				);

		self_type new_ans = new_this * new_other;
		self_type res_col_row = last_col * last_row;

		const self_type ans = new_ans + res_col_row;

		*this = ans;
		return *this;
	}
#if 0
		//case other.col is odd
	3) 	else if ( other.col() & 1 )
		{
			//<1>
			if ( other.col() & 2 )
			{
				append one empty col to other from right side
					[new_other <- other] [C,OC+1]
				do the multiplicaiton
					[new_ans = this * new_other] [R,OC+1]
				remove the last col to generate the ans
					[ans <- new_ans] [R,OC]
			}
			//<2>
			else {...}
		}
#endif
	self_type&
	oc1( const self_type& other )
	{
		//[new_other <- other] [C,OC+1]
		const self_type
		new_other = other || value_type(0);

		const self_type
		new_ans = *this * new_other;

		const self_type 
		ans( 	new_ans,
				range_type( 0, row() ),
				range_type( 0, other.col() ) 
			);

		*this = ans;
		return *this;
	}



#if 0
		//case other.col is odd
	3) 	else if ( other.col() & 1 )
		{
			//<1>
			if ( other.col() & 2 ) {...}
			//<2>
			else
			{
				cut the last col from other get two new matrices
					[new_other <- other] [C,OC-1]
					[last_col <- other]  [C,1]
				do the multications
					[new_ans <- this * new_other]     [R,OC-1]
					[last_col_ans <- this * last_col] [R,1]
				merge the two matrices
					[ans <- new_ans|last_col_ans] [R,1]
					[i.e. last_col_ans as the last col of ans]
			}
		}
#endif
	self_type&
	oc2( const self_type& other )
	{
		const self_type 
		new_other( 	other,
					range_type(0,other.row()),
					range_type(0, other.col()-1)
			   );

		const self_type 
		last_col( 	other, 
					range_type(0, other.row()), 
					range_type(other.col()-1, other.col()) 
			  );

		const self_type 
		new_ans = (*this) * new_other;
		
		const self_type 
		last_col_ans = (*this) * last_col;

		const self_type 
		ans = new_ans || last_col_ans;

		*this = ans;
		return *this;
	}

#if 0
	4) else
		{
			//strassen algorithm 
			( a_00 a_01 )   ( b_00 b_01 )    ( c_00 c_01 )
			(           ) * (           )  = (           )
			( a_10 a_11 )   ( b_10 b_11 )    ( c_10 c_11 )

			Q_0 = ( a_00 + a_11 ) * ( b_00 + b_11 )
			Q_1 = ( a_10 + a_11 ) * b_00
			Q_2 = a_00 * ( b_01 - b_11 )
			Q_3 = a_11 * ( -b_00 + b_10 )
			Q_4 = ( a_00 + a_01 ) * b_11
			Q_5 = ( -a_00 + a_10 ) * ( b_00 + b_10 )
			Q_6 = ( a_01 - a_11 ) * ( b_10 + b_11 )
			
			c_00 = Q_0 + Q_3 - Q_4 +Q_6
			c_10 = Q_1 + Q_3
			c_01 = Q_2 + Q_4
			c_11 = Q_0 - Q_1 +Q_2 + Q_5
		}
#endif

	self_type&
	strassen_multiply( const self_type& other )
	{
		const size_type R_2 = row() >> 1;          
		const size_type C_2 = col() >> 1;
		const size_type OR_2 = C_2;
		const size_type OC_2 = other.col() >> 1;

		const self_type a_00( 	*this, 
								range_type( 0, R_2 ),
								range_type( 0, C_2 )
							);
		const self_type a_01( 	*this,
								range_type( 0, R_2 ),
								range_type( C_2, col() ) 
							);
		const self_type a_10( 	*this,
								range_type( R_2, row() ),
								range_type( 0, C_2 )
							);
		const self_type a_11( 	*this,
								range_type( R_2, row() ),
								range_type( C_2, col() )
							);

		const self_type b_00( 	other, 
								range_type( 0, OR_2 ),
								range_type( 0, OC_2 )
							);
		const self_type b_01( 	other,
								range_type( 0, OR_2 ),
								range_type( OC_2, other.col() ) 
							);
		const self_type b_10( 	other,
								range_type( OR_2, other.row() ),
								range_type( 0, OC_2 )
							);
		const self_type b_11( 	other,
								range_type( OR_2, other.row() ),
								range_type( OC_2, other.col() )
							);


		const self_type Q_0 = ( a_00 + a_11 ) * ( b_00 + b_11 );
		const self_type Q_1 = ( a_10 + a_11 ) * b_00;
		const self_type Q_2 = a_00 * ( b_01 - b_11 ); 
		const self_type Q_3 = a_11 * ( -b_00 + b_10 );
		const self_type Q_4 = ( a_00 + a_01 ) * b_11;
		const self_type Q_5 = ( -a_00 + a_10 ) * ( b_00 + b_01 );
		const self_type Q_6 = ( a_01 - a_11 ) * ( b_10 + b_11 );

		const self_type c_00 = Q_0 + Q_3 - Q_4 +Q_6;
		const self_type c_10 = Q_1 + Q_3;
		const self_type c_01 = Q_2 + Q_4;
		const self_type c_11 = Q_0 - Q_1 +Q_2 + Q_5;

		const self_type ans = ( c_00 || c_01 ) && ( c_10 || c_11 );

		*this = ans;
		return *this;
	}


	public:
		self_type&
		operator *= ( const self_type& other )
		{
			assert( col() == other.row() );
			
			static const size_type threshold = 17;

			const size_type max_dims = std::max( max_dim(), other.max_dim() );
			const size_type min_dims = std::min( min_dim(), other.min_dim() );

			//0
			if ( (max_dims < threshold)  || (min_dims == 1) )
				return direct_multiply( other ); 

			const size_type R = row();
			const size_type C = col();
			const size_type OC = other.col();

			//1
			if ( R & 1 )
			{
				//1 <1>
				if ( R & 2 )
					return rr1(other);	
				//1 <2>
				return rr2(other);
			}
			//2
			if ( C & 1 )
			{ 	//2 <1>
				if ( C & 2 )
					return cc1(other);
				//2 <2>
				return cc2(other);
			}
			//3
			if ( OC & 1 )
			{ 	//3 <1>
				if ( OC & 2 )
					return oc1(other);
				//3 <2>
				return oc2(other);
			}
			//4
			return strassen_multiply( other );
		}







