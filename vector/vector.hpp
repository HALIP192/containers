#ifndef VECTOR_HPP
# define VECTOR_HPP

# include <memory>

namespace ntitan {
    template <class T, class Allocator = std::allocator<T>>
    class vector {

    public:
        typedef T                                       value_type;
        typedef const T                                 const_value_type;
        typedef Allocator                               allocator_type;
        typedef std::size_t                             size_type;
        typedef std::ptrdiff_t                          defference_type;
        typedef value_type&                             reference;
        typedef const value_type&                       const_reference;
        typedef Allocator::pointer                      pointer;
        typedef Allocator::const_pointer                const_pointer;
        typedef wraper_iter<value_type>                 iterator;
        typedef wraper_iter<const_value_type>           const_iterator;
        typedef wraper_reverse_iter<iterator>           reverse_iterator;
        typedef wraper_reverse_iter<const_iterator>     const_reverse_iterator;

    private:
		size_type 		sizeAllocMem;
		allocator_type	allocator;
		pointer			pArrBegin;
		pointer			pArrEnd;

		void copyContextFromBegin(pointer pArgDst, pointer pArgSrc, size_type argCount) {
			while (argCount-- > 0) {
				*(pArgDst++) = *(pArgSrc++);
			}
		}

		template <class InputIt>
		void copyContextFromBegin(pointer pArgDst,
								  typename enable_if<!is_integral<InputIt>::value,
								  InputIt>::type argItBegin, InputIt argItEnd) {
			for (InputIt it = argItBegin; it != argItEnd; ++it)
				*(pArgDst++) = *it;
		}
		
		void copyContext(pointer pArgDst, size_type argCount, size_type argValueInit) {
			for (size_type i = 0; i < argCount; ++i) {
				pArgDst[i] = argValueInit;
			}
		}

		void copyContextFromEnd(pointer pArgDst, pointer pArgSrc, size_type argCount) {
			pArgDst += argCount;
			pArgSrc += argCount;
			while (argCount-- > 0) {
				*(--pArgDst) = *(--pArgSrc);
			}
		}

		template <class InputIt>
		void copyContextFromEnd(pointer pArgDst,
								typename enable_if<!is_integral<InputIt>::value,
								InputIt>::type argItBegin, InputIt argItEnd) {
			pArgDst += std::distance(argItBegin, argItEnd);
			for (InputIt it = argItEnd - 1; it >= argItBegin; --it)
				*(--pArgDst) = *it;
		}

		void initMem(pointer pArgBegin, size_type argCount) {
			value_type value;

			while(argCount-- > 0) {
				allocator.construct(pArgBegin++, value);
			}
		}

		void cpy(pointer pArgDst, pointer pArgSrc, size_type argCount) {
			for(size_type i = 0; i < argCount; ++i) {
				allocator.construct(pArgDst++, *(pArgSrc++));
			}
		}

		void cpy(pointer pArgDst,size_type argCount, const_reference argValueInit) {
			for (size_type i = 0; i < argCount; ++i) {
				allocator.construct(pArgDst++, argValueInit);
			}
		}

		template <InputIt>
		void cpy(pointer pArgDst,
				 typename enable_if<!is_integral<InputIt>::value,
				 InputIt>::type argItBegin, InputIt argItEnd) {
			for (; argItBegin != argItEnd; ++argItBegin) {
				allocator.construct(pArgDst++, *argItBegin);
			}
		}

		void init(size_type argCount, const_reference argValueInit) {
			sizeAllocMem = argCount;
			pArgBegin = allocator.allocate(sizeAllocMem);
			pArrEnd = pArrBegin + argCount;
			cpy(pArrBegin, argCount, argValueInit);
		}

		template<class InputIt>
		void init(typename enable_if<!is_integral<InputIt>::value, InputIt>::type argItBegin,
				  InputIt argItEnd) {
			difference_type diffPtr = std::distance(argItBegin, argItEnd);
			sizeAllocMem = diffPtr;
			pArrBegin = allocator.allocate(diffPtr);
			pArrEnd = pArgBegin + diffPtr;
			cpy(pArrBegin, argItBegin, argItEnd);
		}

		void clean() {
			for (pointer it = pArrBegin; it != pArrEnd; ++it)
				allocator.destroy(it);
			allocator.deallocate(pArrBegin, sizeAllocMem);
			pArrBegin = NULL;
			pArrEnd = NULL;
		}

		void cleanRange(pointer argStart, size_type argCount) {
			pointer end = argStart + argCount;

			for (; argStart != end; ++argStart) {
				allcoator.destroy(argStart);
			}
		}
		
    public:
        vector()
		:	sizeAllocMem(0),
			allocator(),
			pArrBegin(NULL),
			pArrEnd(NULL) { }

        explicit vector( const allocator_type& allocator ) noexcept
		:	sizeAllocMem(0),
			Allocator(allocator),
			pArrBegin(NULL),
			pArrEnd(NULL) { }

        explicit vector( size_type count,
                 const value_type& value = T(),
                 const allocator_type& alloc = allocator_type())
		:	sizeAllocMem(0),
			allocator(alloc),
			pArrBegin(NULL),
			pArrEnd(NULL) {
			if (count > 0)
				init(count, value);
		}
            
        template< class InputIt >
        vector( typename enable_if<!is_integral<InputIt>::value,
				InputIt>::type first, InputIt last, 
				const allocator_type& alloc = allocator_type() )
		:	sizeAllocMem(0),
			allocator(alloc),
			pArrBegin(NULL),
			pArrEnd(NULL) {
			init(first, last);
		}

		vector (const vector &argOtherVector) {
			init(argOtherVector.begin(), argOtherVector.end());
		}

        ~vector(void) {
			clean();
		}

        vector& operator=( const vector& other ) {
			clean();;
			init(argOtherVector.begin(), argOtherVector.end());
			return (*this);
		}

        allocator_type get_allocator() const {
			return (allocator);
		}

        void assign( size_type size_arr, const T& value ) {
			clean();
			init(size_arr, value);
		}

        template< class InputIt >
        void assign( typename enable_if<!is_integral<InputIt>::value, InputIt>::type first,
					 typename enable_if<!is_integral<InputIt>::value, InputIt>::type last ) {
			clean();
			init(first, last);
		}

        reference at( size_type pos ) {
			if (pos >= this->size())
				throw std::out_of_range("vector<..> out_of_range");
			return (*(pArrBegin + pos));
		}

        const_reference at( size_type pos ) const {
			if (pos >= this.size())
				throw std::out_of_range("vector<..> out_of_range");
			return (*(pArrBegin + pos));
		}

        reference operator[]( size_type pos ) {
			return (*(pArrBegin + pos));
		}

        const_reference operator[]( size_type pos ) const {
			return (*(pArrBegin + pos));
		}

        reference front() {
			return (*pArrBegin);
		}

        const_reference front() const {
			return (*pArrBegin);
		}

        reference back() {
			return (*(pArrEnd - 1));
		}

        const_reference back() const {
			return (*(pArrEnd - 1));
		}

        value_type* data() {
			if (this->size() == 0)
				return (NULL);
			return (pArgBegin);
		}

        const value_type* data() const {
			if (this->size() == 0)
				return (NULL);
			return (pArrBegin);
		}

        iterator begin();
        const_iterator begin() const;
        iterator end();
        const_iterator end() const;
        reverse_iterator rbegin();
        const_reverse_iterator rbegin() const;
        reverse_iterator rend();
        const_reverse_iterator rend() const;
        bool empty() const;
        size_type size() const;
        size_type max_size() const;
        void reserve( size_type new_cap );
        size_type capacity() const;
        void clear();
        iterator insert( const_iterator pos, const T& value );
        iterator insert( const_iterator pos, size_type count, const T& value );
        template< class InputIt >
        iterator insert( const_iterator pos, InputIt first, InputIt last );
        iterator erase( iterator pos );
        iterator erase( iterator first, iterator last );
        void push_back( const T& value );
        void pop_back();
        void resize( size_type count, T value = T() );
        void swap( vector& other );
        
        
        constexpr explicit vector( const allocator_type &argAllocator) noexcept {};
        explicit vector( size_type argCount, const value_type &argValueInit = T(),
                        const allocator_type &argAllocator = allocator_type())
        {
            if (argCount > 0)
                init(argCount, argValueInit);
        };

        template< class Inp >
        vector( Inp first, Inp last, const Allocator &alloc = Allocator());
        vector( const vector &other);
        ~vector();

    };
}


#endif // VECTOR_HPP
