#pragma once
#include <exception>
#include<iostream>
#include <random>
#include <complex>

template<typename BasicType >
class Randomizer
{
	std::random_device rd;
	std::uniform_real_distribution<double> ud;
public:
	Randomizer(const BasicType& low_limit, const BasicType& high_limit) : ud((low_limit), (high_limit)), rd() {}
	BasicType Calculate()
	{
		return BasicType(ud(rd));
	}
};

template<>
class Randomizer<std::string>
{
	std::random_device rd;
	std::uniform_int_distribution<int> ud_char;
	std::uniform_int_distribution<int> ud_size;

public:
	Randomizer(const std::string& low_limit, const std::string& high_limit) :
		ud_char(0, 256), // генерится уникальный код символа
		ud_size(
			std::min(static_cast<unsigned>(low_limit.length()), static_cast<unsigned>(high_limit.length())),
			std::max(static_cast<unsigned> (low_limit.length()), static_cast<unsigned>(high_limit.length()))
		),
		rd() {}
	std::string Calculate()
	{
		std::string new_str; // создана пустая строка
		const size_t size = ud_size(rd); // рандомный размер строки
		for (size_t i = 0; i < size; ++i)
		{
			new_str += static_cast<char>(ud_char(rd)); // рандомный элемент строки
		}
		return new_str;
	}
};


template<>
class Randomizer< std::pair<int, double> >
{
	std::random_device rd;
	std::uniform_int_distribution<int> ud_first;
	std::uniform_real_distribution<double> ud_second;
public:
	Randomizer(const  std::pair<int, double> low_limit, const  std::pair<int, double> high_limit) :
		ud_first(
			std::min(low_limit.first, high_limit.first),
			std::max(low_limit.first, high_limit.first)
		),
		ud_second(
			std::min(low_limit.second, high_limit.second),
			std::max(low_limit.second, high_limit.second)
		), // формируем выравниватель
		rd() {}
	std::pair<int, double> Calculate()
	{
		return  std::pair<int, double>((ud_first(rd)), ud_second(rd));
	}
};
template<typename T>
class Set
{
private:
	size_t _capacity;
	size_t _size;
	T* _data;
private:
	void swap(Set<T>& other) noexcept
	{
		std::swap(_capacity, other._capacity);
		std::swap(_size, other._size);
		std::swap(_data, other._data);
	}
	void MemoryOptimization(const size_t new_size) //формируем новый размер
	{	                                      
		_capacity = new_size * 2;  	
		T* new_data = new T[_capacity];       	 
		for (size_t i = 0; i < _size && i < _capacity; ++i)
		{
			new_data[i] = _data[i];
		}
		delete[] _data;
		_data = new_data;
	}
	void erase(const T& value)
	{
		size_t i = 0;
		for (; i < _size; ++i)
		{
			if (_data[i] == value) break;
		}
		if (i == _size)  return; // если не нашли элемент
		_data[i] = _data[_size - 1];
		_size--;
		if (_size < _capacity) MemoryOptimization(_size); // уменьшили размер. если сайз меньше капасити то нужно уменьшить резерв
	}


	
public:

	Set() : _size(0), _capacity(0), _data(nullptr) {}
	Set(const T* arr, const size_t size) : _size(0), _capacity(size * 2)//конструктор с параметром
	{
		if (size == 0)
		{
			_data = nullptr;
			return;
		}
		_data = new T[_capacity];
		for (size_t i = 0; i < size; ++i)
		{
			push_back(arr[i]);//решаем добавляем или нет элемент
		}
	}
	void push_back(const T& value)
	{
		size_t j = 0;
		for (; j < _size; ++j)
		{
			if (value == _data[j]) {
				break;
			}
		}
		if (j == _size)
		{
			if (_capacity == _size) MemoryOptimization(_size + 1); //запрос резерва
			_data[_size] = value;
			_size++;
		}
	}
	Set(const Set<T>& other) : _capacity(other._capacity), _size(other._size)
	{
		if (other._data == nullptr)
		{
			_data = nullptr;
			return;
		}
		_data = new T[_capacity];
		for (size_t i = 0; i < _size && i < _capacity; ++i)
		{
			_data[i] = other._data[i];
		}
	}
	Set(const size_t size, T low_limit, T high_limit) : _size(0), _capacity(size * 2), _data(new T[_capacity])
	{
		Randomizer<T> rand(low_limit, high_limit);
		while (_size != size)
		{
			push_back(rand.Calculate());
		}
	}
	Set<T>& operator= (const Set<T>& other) 
	{
		if (this/*личный указатель объекта*/ == &other) return *this;//арисвоение объекта самому себе
		Set<T> tmp(other);
		swap(tmp);
		return *this;
	}
	T operator[](const size_t idx) const // получения числа по его индексу;
	{
		if (idx >= _size) throw std::out_of_range("Array index out of bounds!");
		return _data[idx];
	}
	bool operator==(const Set<T>& other) const   
	{
		if (_size != other._size) return false;
		for (size_t i = 0; i < _size; ++i)
		{
			if (!SearchValue(other._data[i])) return false;
		}
		return true;
	}
	bool operator!=(const Set<T>& other) const
	{
		return !((*this) == other);
	}
	Set<T>& operator+=(const T& value)
	{
		push_back(value);
		return (*this);
	}
	Set<T> operator+(const T& value) const     
	{
		Set<T> sum(*this);
		sum += value;
		return sum;
	}
	Set<T>& operator-=(const T& value)
	{
		erase(value);
		return (*this);
	}
	Set<T> operator-(const T& value) const
	{
		Set<T> minus(*this);
		minus -= value;
		return minus;
	}
	Set<T> operator+(const Set<T>& rht) const
	{
		Set<T> sum_sets(*this);
		for (size_t i = 0; i < rht._size; ++i)
		{
			sum_sets += rht._data[i];
		}
		return sum_sets;
	}
	Set<T> operator-(const Set<T>& rht) const
	{
		Set<T> minus_sets(*this);
		for (size_t i = 0; i < _size; ++i)
		{
			minus_sets -= rht._data[i];
		}
		return minus_sets;
	}
	bool SearchValue(const T& value) const
	{
		for (size_t i = 0; i < _size; ++i)
		{
			if (_data[i] == value) return true;
		}
		return false;
	}
	Set<T> IntersecSets(const Set<T>& other)  const //пересечение множеств
	{
		Set<T> intersect;
		for (size_t i = 0; i < other._size; ++i)
		{
			if (SearchValue(other._data[i])) intersect += other._data[i];
		 
		}
		return intersect;
	}
	size_t GetSize() const
	{
		return _size;
	}
	size_t GetCapacity() const
	{
		return _capacity;
	}

	~Set()
	{
		if (_data != nullptr) delete[] _data;
	}
};

std::ostream& operator<<(std::ostream& os, const std::pair<int, double>& pair)
{
	os << " first arg = " << pair.first << " second arg " << pair.second;
	return os;
}
template<typename T>
std::ostream& operator<<(std::ostream& os, const Set<T>& set)
{
	for (size_t i = 0; i < set.GetSize(); ++i)
	{
		os << set[i] << std::endl;
	}
	return os;
};

