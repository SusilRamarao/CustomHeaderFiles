#include<iostream>
#include<cassert>


template<typename T>
class List
{
	struct Node {

		T m_Data;

		Node *m_pNext{};
		Node *m_pPrev{};

		Node(const T &data) :m_Data{ data } {

		}

		template<typename...Args>
		Node(Args&&...data) : m_Data{
				std::forward<Args>(data)...
		} {
		}

	};

	Node *m_pHead;
	Node *m_pTail;
	size_t m_Length{};
public:

	using iterator_category = std::bidirectional_iterator_tag;

	//****************************
	class Iterator {

		Node *m_pCurrent;
	public:
		Iterator() = default;
		Iterator(Node *p) :m_pCurrent{ p } {

		}

		friend class List<T>;
		T & operator *() {
			return m_pCurrent->m_Data;
		}

		T * operator ->() {

			return &m_pCurrent->m_Data;
		}

		Iterator & operator++() {
			m_pCurrent = m_pCurrent->m_pNext;
			return *this;
		}
		Iterator & operator++(int) {
			auto temp{ *this };

			m_pCurrent = m_pCurrent->m_pNext;
			return temp;
		}

		Iterator & operator--() {
			m_pCurrent = m_pCurrent->m_pPrev;
			return *this;
		}
		Iterator & operator--(int) {
			auto temp{ *this };
			m_pCurrent = m_pCurrent->m_pPrev;
			return temp;
		}

		Iterator & operator+=(int offset) {
			int cur = 0;
			while (cur < offset) {
				m_pCurrent = m_pCurrent->m_pNext;
				cur++;
			}
			return *this;
		}

		Iterator & operator+(int offset) {
			auto temp{ *this };
			int cur = 0;
			while (cur < offset) {
				temp = temp->m_pNext;
				cur++;
			}
			return temp;
		}

		Iterator & operator-=(int offset) {
			int cur = 0;
			while (cur < offset) {
				m_pCurrent = m_pCurrent->m_pPrev;
				cur++;
			}
			return *this;
		}

		Iterator & operator-(int offset) {
			auto temp{ *this };
			int cur = 0;
			while (cur < offset) {
				temp = temp->m_pPrev;
				cur++;
			}
			return temp;
		}

		bool operator ==(const Iterator &right)const {
			return m_pCurrent == right.m_pCurrent;
		}
		bool operator !=(const Iterator &right)const {
			return m_pCurrent != right.m_pCurrent;
		}

	};

	Iterator GetBegin() {

		return Iterator{ m_pHead };
	}
	Iterator GetEnd() {

		return Iterator{ nullptr };
	}

	//**************************************
	class ReverseIterator {

		Node *m_pCurrent;
	public:
		ReverseIterator() = default;
		ReverseIterator(Node *p) :m_pCurrent{ p } {

		}

		friend class List<T>;
		T & operator *() {
			return m_pCurrent->m_Data;
		}
		ReverseIterator & operator++() {
			m_pCurrent = m_pCurrent->m_pPrev;
			return *this;
		}
		ReverseIterator & operator++(int) {
			auto temp{ *this };

			m_pCurrent = m_pCurrent->m_pPrev;
			return temp;
		}

		ReverseIterator & operator--() {
			m_pCurrent = m_pCurrent->m_pNext;
			return *this;
		}
		ReverseIterator & operator--(int) {
			auto temp{ *this };
			m_pCurrent = m_pCurrent->m_pNext;
			return temp;
		}
		bool operator ==(const ReverseIterator &right)const {
			return m_pCurrent == right.m_pCurrent;
		}
		bool operator !=(const ReverseIterator &right)const {
			return m_pCurrent != right.m_pCurrent;
		}

	};

	ReverseIterator GetRBegin()const {

		return ReverseIterator{ m_pTail };
	}
	ReverseIterator GetREnd()const {

		return ReverseIterator{ nullptr };
	}

	List() = default;
	~List() {

		auto temp = m_pHead;
		do {

			m_pHead = m_pHead->m_pNext;
			delete temp;
			temp = m_pHead;
		} while (m_pHead != nullptr);
	}

	void AddBack(const T &elem) {

		if (m_Length == 0) {
			//If forwarding is not used copy constructor is invoked, else forward constructor
			//is invoked.
			//Node *temp = new Node(elem);
			m_pHead = m_pTail = new Node{ elem };
			//m_pHead = m_pTail = temp;
		}
		else {

			m_pTail->m_pNext = new Node{ elem };

			m_pTail->m_pNext->m_pPrev = m_pTail;
			m_pTail = m_pTail->m_pNext;
		}

		++m_Length;
	}

	template<typename...Args>
	void EmplaceBack(Args&&...elem) {

		if (m_Length == 0) {
			//Forward constructor is invoked
			m_pHead = m_pTail = new Node{ std::forward<Args>(elem)... };
		}
		else {

			m_pTail->m_pNext = new Node{ std::forward<Args>(elem)... };

			m_pTail->m_pNext->m_pPrev = m_pTail;
			m_pTail = m_pTail->m_pNext;
		}

		++m_Length;
	}
	void AddFront(const T &elem) {

		if (m_Length == 0) {

			m_pHead = m_pTail = new Node{ elem };
		}
		else {

			m_pHead->m_pPrev = new Node{ elem };

			m_pHead->m_pPrev->m_pNext = m_pHead;
			m_pHead = m_pHead->m_pPrev;
		}

		++m_Length;
	}

	auto GetLength()const {
		return m_Length;
	}

	void Insert(const T &elem, size_t position) {
		assert(position < m_Length);
		auto *pTemp = m_pHead;
		for (int i = 0; i < m_Length; i++, pTemp = pTemp->m_pNext) {

			if (i == position)
				break;
		}
		auto newNode = new Node{ elem };
		/*newNode->m_pPrev = pTemp;
		newNode->m_pNext = pTemp->m_pNext;
		pTemp->m_pNext->m_pPrev = newNode;
		pTemp->m_pNext = newNode;*/

		newNode->m_pNext = pTemp;
		newNode->m_pPrev = pTemp->m_pPrev;
		pTemp->m_pPrev->m_pNext = newNode;
		pTemp->m_pPrev = newNode;
		++m_Length;
	}

	void Erase(Node * pTemp) {

		assert(pTemp != nullptr);

		if (pTemp == m_pHead) {

			m_pHead = m_pHead->m_pNext;
			m_pHead->m_pPrev = nullptr;
		}
		else if (pTemp == m_pTail) {

			m_pTail = m_pTail->m_pPrev;
			m_pTail->m_pNext = nullptr;
		}
		else {
			pTemp->m_pPrev->m_pNext = pTemp->m_pNext;
			pTemp->m_pNext->m_pPrev = pTemp->m_pPrev;
		}


		delete pTemp;
		--m_Length;
	}


	const T & GetItem(size_t position) {

		assert(position < m_Length);
		auto *pTemp = m_pHead;
		for (int i = 0; i < m_Length; i++, pTemp = pTemp->m_pNext) {

			if (i == position)
				break;
		}
		return pTemp->m_Data;

	}

	/*auto GetHead()const {

		return m_pHead;
	}*/

	/*std::ostream & operator<<(std::ostream & out, const Number &n) {
		out << n.getNum();
		return out;
	}*/


};
