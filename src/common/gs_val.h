#pragma once

#include <functional>

template<typename T>
class gs_val{
	public:
		typedef std::function<T(T var)> getter_func;
		typedef std::function<void(T& var, T val)> setter_func;
	private:
		gs_val<T>::getter_func getter;
		gs_val<T>::setter_func setter;
		T val;
	public:
		static const gs_val<T>::getter_func std_getter;
		static const gs_val<T>::setter_func std_setter;

		gs_val():
			getter(std_getter),
			setter(std_setter),
			val(T{}) {
		};
		
		gs_val(gs_val<T>&& mv):
			getter(mv.getter),
			setter(mv.setter),
			val(mv.val) {
		};

		gs_val(gs_val<T>::getter_func getter, gs_val<T>::setter_func setter, T val = {}):
			getter(getter),
			setter(setter),
			val(val) {
		};
/*
        gs_val(gs_val&&) = delete;
        gs_val& operator=(const gs_val&) = delete;*/
        gs_val& operator=(gs_val<T>&& mv){
			getter = mv.getter;
			setter = mv.setter;
			val = mv.val;
			return *this;
		};


		operator T(){
			return getter(val);
		}

		gs_val<T>& operator=(const T& r_val){
			setter(val, r_val);
			return *this;
		}

		template<typename U>
		gs_val<T>& operator+=(U r_val){
			setter(val, getter(val) + r_val);
			return *this;
		}

		template<typename U>
		gs_val<T>& operator-=(U r_val){
			setter(val, getter(val) - r_val);
			return *this;
		}

		template<typename U>
		gs_val<T>& operator*=(U r_val){
			setter(val, getter(val) * r_val);
			return *this;
		}

		template<typename U>
		gs_val<T>& operator/=(U r_val){
			setter(val, getter(val) / r_val);
			return *this;
		}

		template<typename U>
		gs_val<T>& operator%=(U r_val){
			setter(val, getter(val) % r_val);
			return *this;
		}

		template<typename U>
		gs_val<T>& operator^=(U r_val){
			setter(val, getter(val) ^ r_val);
			return *this;
		}

		template<typename U>
		gs_val<T>& operator&=(U r_val){
			setter(val, getter(val) & r_val);
			return *this;
		}

		template<typename U>
		gs_val<T>& operator|=(U r_val){
			setter(val, getter(val) | r_val);
			return *this;
		}

		template<typename U>
		gs_val<T>& operator>>=(U r_val){
			setter(val, getter(val) >> r_val);
			return *this;
		}

		template<typename U>
		gs_val<T>& operator<<=(U r_val){
			setter(val, getter(val) << r_val);
			return *this;
		}

		gs_val<T>& operator++(){
			setter(val, getter(val) + 1);
			return *this;
		}

		gs_val<T>& operator--(){
			setter(val, getter(val) - 1);
			return *this;
		}

		gs_val<T> operator++(int){
			gs_val<T> tmp(*this);
			setter(val, getter(val) + 1);
			return tmp;
		}

		gs_val<T> operator--(int){
			gs_val<T> tmp(*this);
			setter(val, getter(val) - 1);
			return tmp;
		}

		friend std::ostream& operator<<(std::ostream& os, const gs_val<T>& obj){
			os << obj.getter(obj.val);
		    return os;
		}

		friend std::istream& operator>>(std::istream& is, gs_val<T>& obj){
			T val;
			is >> val;
			obj.setter(obj.val, val);
		    return is;
		}
};

template<typename T>
const typename gs_val<T>::getter_func gs_val<T>::std_getter = [](T var){ return var; };
template<typename T>
const typename gs_val<T>::setter_func gs_val<T>::std_setter = [](T& var, T val){ var = val; };

template<typename T>
class g_val{
	public:
		typedef std::function<T(T var)> getter_func;
	private:
        g_val(const g_val&) = default;

		T& val;
		const getter_func getter;
	public:
		static const getter_func std_getter;

		g_val(getter_func getter, T& val):
			getter(getter),
			val(val) {
		};

        g_val(g_val&&) = delete;
        g_val& operator=(const g_val&) = delete;
        g_val& operator=(g_val&&) = delete;

		operator T(){
			return getter(val);
		}

		g_val<T>& operator=(const T& r_val) = delete;


		friend std::ostream& operator<<(std::ostream& os, const g_val<T>& obj){
			os << obj.getter(obj.val);
		    return os;
		}

};

template<typename T>
const typename g_val<T>::getter_func g_val<T>::std_getter = [](T var){ return var; };
