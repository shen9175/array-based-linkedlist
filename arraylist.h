
#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <stdexcept>
#include <initializer_list>
#include <iostream>
#include <iomanip>



#define MAX 15
template <typename T> class ArrayList
{
	public:
		using iter=short;
		using size_type=short;
		ArrayList();
		ArrayList(const ArrayList&)=default;
		ArrayList& operator=(const ArrayList&)=default;
		ArrayList(std::initializer_list<T> il);
		~ArrayList();
		void push_back(const T& x);
		void push_front(const T& x);
		iter insert(const iter & p,const T& x);
		iter insert(const iter& p,const size_type& n,const T& x);
		iter insert(iter& p,std::initializer_list<T> il);
		iter erase(const iter& p);
		iter erase(const T& x);
		char validate_b_e(const iter& b, const iter& e);
		iter erase(const iter& b, const iter& e);
		iter search(const T& x);
		double fpRound(const double& num, const unsigned int & dec);
		iter begin(){return head;}
		iter end(){return next[tail];}
		const iter cbegin() const {return head;}
		const iter cend() const {if(tail==-1) return tail; else return next[tail];}
		size_type length(){return size;}
		bool empty(){return true?false:size==0,size!=0;}
		void clear();
		void printlink();
		void print();
		T key_value(const iter& index){return key[index];}

	private:
		T key[MAX];
		iter next[MAX];
		iter prev[MAX];
		iter Free;
		iter head;
		iter tail;
		size_type size;
		iter allocate_object();
		void free_object(const iter& index);

};

template <typename T> ArrayList<T>::~ArrayList()
{
	clear();
}
template <typename T> ArrayList<T>::ArrayList()
{
	head=-1;//empty
	Free=0;//first element of the array
	size=0;
	tail=-1;//empty
	for(iter i=0;i<MAX-1;++i)//initial free list next[i] always points next number except the last one which points null(-1)
		next[i]=i+1;
	next[MAX-1]=-1;

	for(iter i=0;i<MAX;++i)//initial prev points to null(-1)
		{
		prev[i]=-1;
		key[i]=-1;
		}
}

template <typename T> ArrayList<T>::ArrayList(std::initializer_list<T> il)
{
	//ArrayList();
	head=-1;//empty
	Free=0;//first element of the array
	size=0;
	tail=-1;//empty
	for(iter i=0;i<MAX-1;++i)//initial free list next[i] always points next number except the last one which points null(-1)
		next[i]=i+1;
	next[MAX-1]=-1;

	for(iter i=0;i<MAX;++i)//initial prev points to null(-1)
		{
		prev[i]=-1;
		key[i]=-1;
		}

	for(auto i=il.begin();i!=il.end();++i)
		push_back(*i);
}


template <typename T> typename ArrayList<T>::iter ArrayList<T>::allocate_object()
{
	iter ret;
	if(Free==-1)
		throw std::runtime_error("Out of space!");
	else
		{
		ret=Free;
		Free=next[ret];
		}
	return ret;
	
}

template <typename T> void ArrayList<T>::free_object(const iter& index)
{
	next[index]=Free;
	Free=index;
}

template <typename T> void ArrayList<T>::push_back(const T& x)
{
	if(head==-1)//empty
	{
		iter p=allocate_object();
		key[p]=x;
		next[p]=-1;
		prev[p]=-1;
		head=p;
		tail=p;
	}
	else
	{
		iter p=allocate_object();
		key[p]=x;
		next[p]=-1;
		prev[p]=tail;
		next[tail]=p;
		tail=p;
	}
	++size;
}


template <typename T> void ArrayList<T>::push_front(const T& x)
{
	if(head==-1)
	{
		iter p=allocate_object();
		key[p]=x;
		next[p]=-1;
		prev[p]=-1;
		head=p;
		tail=p;	
	}
	else
	{
		iter p=allocate_object();
		key[p]=x;
		next[p]=head;
		prev[p]=-1;
		prev[head]=p;
		head=p;		
	}
	++size;
}

template <typename T> typename ArrayList<T>::iter ArrayList<T>::insert(const iter& p, const T& x)
{
	iter n;
	if(p==head)
	{
		n=allocate_object();
		key[n]=x;
		prev[n]=-1;
		next[n]=head;
		prev[head]=n;
		head=n;
	}
	else
	{
		n=allocate_object();
		key[n]=x;
		next[prev[p]]=n;
		prev[n]=prev[p];
		prev[p]=n;
		next[n]=p;

	}
	++size;
	return n;

}


template <typename T> typename ArrayList<T>::iter ArrayList<T>::insert(const iter& p,const size_type& n,const T& x)
{
			auto ret=p;
			for(auto i=0;i<n;++i)
				ret=insert(p,x);
			return ret;
}

template <typename T> typename ArrayList<T>::iter ArrayList<T>::insert(iter& p,std::initializer_list<T> il)
{
	auto it=il.end();
	--it;
	for(;it!=il.begin();--it)
		p=insert(p,*it);

	p=insert(p,il.begin());
	return p;
}

template <typename T> typename ArrayList<T>::iter ArrayList<T>::erase(const iter& p)
{
	iter ret=-1;
	if(p==-1)
		throw std::runtime_error("The list is empty!");
	else
	{
		if(p==head)
		{
			head=next[head];
			key[p]=-1;//prev[p] is already -1 since p is head prev[head]=-1. next[p] will overide by free_object. it will contains the index of the second free memory location.
			free_object(p);
			if(head!=-1)
				prev[head]=-1;
			else
				tail=-1;
			ret=head;
		}
		else
		{
			if(p==tail)
			{
				next[prev[p]]=-1;
				tail=prev[p];
				prev[p]=-1;
				key[p]=-1;
				free_object(p);
				ret=-1;
			}
			else
			{
				ret=next[p];
				next[prev[p]]=next[p];
				prev[next[p]]=prev[p];
				next[p]=-1;
				key[p]=-1;
				prev[p]=-1;
				free_object(p);
			}
		}
	--size;
	}
	return ret;
}

template <typename T> typename ArrayList<T>::iter ArrayList<T>::erase(const T& x)
{
	auto p=serach(x);
	if(p!=-1)
		return erase(p);
	else
		throw std::runtime_error("There is no such an element");
}


template <typename T>char ArrayList<T>::validate_b_e(const iter& b, const iter& e)
{
	auto p=b;
	while(p!=e&&p!=-1)
		p=next[p];
	if(p==-1)
	{
		p=b;
		while(p!=e&&p!=-1)
			p=prev[p];
		if(p==-1)
			return 0;
		else
			return -1; 
	}
	else
	return 1;
}
template <typename T>typename ArrayList<T>::iter ArrayList<T>::erase(const iter& b, const iter& e)
{
	if(e!=-1)
	{
		auto p=b;
		auto saved_next_e=next[e];
		while(p!=saved_next_e)
			p=erase(p);
		return p;
	}
	else
	{
		auto p=b;
		while(p!=-1)
			p=erase(p);
		return p;
	}
}

template <typename T> double ArrayList<T>::fpRound(const double& num, const unsigned int & dec)
{
	unsigned int digit=pow(10,dec);
	double ret=floor(num*digit+0.5)/digit;
	return ret;
}

template <typename T> typename ArrayList<T>::iter ArrayList<T>::search(const T& x)
{
	iter p=head;
	while(p!=-1 && key[p]!=x)
		p=next[p];
	if(p==-1)//probably is double, the input x is not precise as p->key, so give another chance.
	{
		p=head;
		while(p!=-1 && fpRound(key[p],4)!=fpRound(x,4))
			p=next[p];
	}
	return p;
}

template <typename T> void ArrayList<T>::clear()
{
	if(head!=-1)
	{
		/*
		auto p=head;
		while(p!=-1)
		{
			prev[p]=-1;
			key[p]=-1;
			p=next[p];
			next[prev[p]]=-1;
			free_object(prev[p]);
			--size;
			//printlink();
		}
		head=-1;
		tail=-1;*/
		erase(cbegin(),cend());
		if(size!=0)
			throw std::runtime_error("The list size is not 0 after deleting all elements!");
	}
	else
	{
		if(size!=0)
			throw std::runtime_error("The list size is not 0 even though the head is empty");
	}
}

template <typename T>void ArrayList<T>::print()
{
	auto p=head;
	std::cout<<"prev[0] address="<<&prev<<std::endl;
	std::cout<<" key[0] address="<<&key<<std::endl;
	std::cout<<"next[0] address="<<&next<<std::endl<<std::endl;

		while(p!=-1)
		{
		std::cout<<key[p]<<" ";
		p=next[p];
		}
		std::cout<<std::endl;
}

template <typename T>void ArrayList<T>::printlink()
{
	std::cout<<"         "<<"head="<<head<<"    "<<"tail="<<tail<<"    "<<"size="<<size<<"    Free="<<Free<<std::endl<<std::endl;
	std::cout<<"prev[0] address="<<&prev<<std::endl;
	std::cout<<" key[0] address="<<&key<<std::endl;
	std::cout<<"next[0] address="<<&next<<std::endl<<std::endl;
	std::cout<<"         "<<"Index"<<"   |   "<<"prev"<<"   |      "<<"key"<<"      |   "<<"next"<<std::endl;
	std::cout<<"______________________________________________________________________"<<std::endl;
	for(iter i=0;i<MAX;++i)
	{
		if(i==head)
		{
			if(i==tail)
				std::cout<<"head tail";
			else
				std::cout<<"  head   ";
		}
		else if(i==tail)
		std::cout<<"  tail   ";
		else if(i==Free)
		std::cout<<"  Free   ";
		else
		std::cout<<"         ";
		std::cout<<std::right<<std::setw(5)<<i<<"   |"<<std::setw(6)<<prev[i]<<"    |"<<std::setw(8)<<key[i]<<"       |"<<std::setw(6)<<next[i]<<std::endl;
		std::cout<<"______________________________________________________________________"<<std::endl;
	}
}

#endif
















