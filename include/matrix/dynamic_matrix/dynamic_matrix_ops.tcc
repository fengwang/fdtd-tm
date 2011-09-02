template<typename T, std::size_t D, typename A>
std::ostream & 
operator <<(std::ostream& lhs, const Dynamic_Matrix<T, D, A>& rhs)
{
    typedef typename Dynamic_Matrix<T>::size_type size_type;
    typedef typename Dynamic_Matrix<T>::value_type value_type;

    for (size_type i = 0; i < rhs.row(); ++i)
    {
        std::copy(rhs.row_begin(i), rhs.row_end(i),
                  std::ostream_iterator<value_type> (lhs, "\t"));
        lhs << "\n";
    }

    return lhs;
}

