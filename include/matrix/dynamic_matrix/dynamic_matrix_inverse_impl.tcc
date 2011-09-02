	public:
	const self_type
	inverse() const
	{
		assert( row() == col() );

		if ( row() & 1 )
			return direct_inverse();

		return strassen_inverse();
	}

	private:
	//
	//A=[P Q, R S], where P is m*m, Q is m*n, R is n*m and S is n*n
	//suppose the inverse is A'=[P' Q', R' S'],
	//then we could get :
	//  P' = (P-Q*S^{-1}*R)^{-1}
	//  Q' = -(P-Q*S^{-1}*R)^{-1} * (Q*S^{-1})
	//  R' = -(S^{-1}*R) * (P-Q*S^{-1}*R)^{-1}
	//  S' = S^{-1} + (S^{-1}*R) * (P-Q*S^{-1}*R)^{-1} * (Q*S^{-1})
	//or short terms: 
	// a) s[n,n] = S^{-1}
	// b) p[m,m] = P^{-1}
	// c) Qs[m,n]= Q*s
	// d) sR[n,m]= s*R
	// e) QsR[m,m]= Q*sR
	// f) L[m,m] = P-QsR
	// g) P'[m,m] = L^{-1}
	// h) Q'[m,n] = -P'*Qs
	// i) R'[n,m] = -sR*P'
	// j) S'[n,n] = s + sR * P' * Qs
	//
	const self_type 
	direct_inverse() const
	{
		if (1 == size())
		{
			self_type ans(*this);

			*ans.begin() = value_type(1) / (*ans.begin());
			return ans;
		}

		self_type ans(row(), col());

		const size_type m = row() >> 1;
		const size_type n = row() - m;
		self_type P(m, m), Q(m, n), R(n, m), S(n, n);

		for (size_type i = 0; i < m; ++i)
		{
			std::copy(row_begin(i), row_begin(i) + m, P.row_begin(i));
			std::copy(row_begin(i) + m, row_end(i), Q.row_begin(i));
		}
		for (size_type i = m; i < row(); ++i)
		{
			std::copy(row_begin(i), row_begin(i) + m, R.row_begin(i - m));
			std::copy(row_begin(i) + m, row_end(i), S.row_begin(i - m));
		}

		// a)
		const self_type s = S.inverse();
		// b)
		const self_type p = P.inverse();
		// c)
		const self_type Qs = Q * s;
		// d)
		const self_type sR = s * R;
		// e)
		const self_type QsR = Q * sR;
		// f)
		const self_type L = P - QsR;
		// g)
		const self_type P_ = L.inverse();
		// h)
		const self_type Q_ = -P_ * Qs;
		// i)
		const self_type R_ = -sR * P_;
		// j)
		const self_type S_ = s - R_ * Qs;

		for (size_type i = 0; i < m; ++i)
		{
			std::copy(P_.row_begin(i), P_.row_end(i), ans.row_begin(i));
			std::copy(Q_.row_begin(i), Q_.row_end(i), ans.row_begin(i) + m);
		}
		for (size_type i = 0; i < n; ++i)
		{
			std::copy(R_.row_begin(i), R_.row_end(i), ans.row_begin(i + m));
			std::copy(S_.row_begin(i), S_.row_end(i), ans.row_begin(i + m) + m);
		}

		return ans;
	}

	//
	// A:	suppose the matrices (a_11 a_12, a_21 a_22) and (c_11 c_12, c_21 c_22) are inverses of each other.
	//		the c's can be obtained by following operations:
	//	1		R1 = inverse(a_11)
	//	2		R2 = a_21 * R1
	//	3		R3 = R1 * a_12
	//	4		R4 = a_21 * R3
	//	5		R5 = R4 - a_22
	//	6		R6 = inverse(R5)
	//	7		c_12 = R3*R6
	//	8		c_21 = R6*R2
	//	9		R7 = R3*c_21
	//	10  	c_11 = R1-R7
	//	11  	c_22 = -R6
	// B:	merge	c_11 c_12
	//				c_21 c_22
	//		to generate the reverse matrix.
	//
	const self_type 
	strassen_inverse() const
	{
		const size_type n = row();
		const size_type n_2 = n >> 1;

		//A
		const self_type a_11( *this, range_type(0, n_2), range_type(0, n_2) );
		const self_type a_12( *this, range_type(0, n_2), range_type(n_2, n) );
		const self_type a_21( *this, range_type(n_2, n), range_type(0, n_2) );
		const self_type a_22( *this, range_type(n_2, n), range_type(n_2, n) );
		
		//1
		const self_type R1 = a_11.inverse();
		//2
		const self_type R2 = a_21 * R1;
		//3
		const self_type R3 = R1 * a_12;
		//4
		const self_type R4 = a_21 * R3;
		//5
		const self_type R5 = R4 - a_22;
		//6
		const self_type R6 = R5.inverse();
		//7
		const self_type c_12 = R3 * R6;
		//8
		const self_type c_21 = R6 * R2;
		//9
		const self_type R7 = R3 * c_21;
		//10
		const self_type c_11 = R1 - R7;
		//11
		const self_type c_22 = -R6;

		//B
		const self_type ans = (c_11 || c_12) && (c_21 || c_22);

		return ans;
	}






