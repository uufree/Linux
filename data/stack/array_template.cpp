template<typename T>
arraystack<T>::arraystack(int arraystack_=10)
{
	arraystack = arraystack_;
	stacksize = 0;
	head = new T [arraystack];
} 

template<typename T>
arraystack<T>::arraystack(const arraystack<T>& st)
{
	delete [] head;
	arraystack = st.arraystack;
	stacksize = st.stacksize;
	copy(st.head,st.head+stacksize,head);
}

template<typename T>
void arraystack<T>::pop()
{
	T* st = head;
	for(int i=0;i<stacksize;i++)
		st++;
	delete st;
}

template<typename T>
void arraystack<T>::push(T& key)
{
	T* st = head;
	for(int i=0;i<stacksize;i++)
		st++;
	*st = key;
}
