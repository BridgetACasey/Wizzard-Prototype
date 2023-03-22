//@BridgetACasey

#pragma once

namespace Wizzard
{
	template<typename T>
	class WizRef
	{
	public:
		WizRef() : refInstance(nullptr) {}
		WizRef(T* instance) : refInstance(instance) {}
		~WizRef() = default;

		template<typename T2>
		WizRef(const WizRef<T2>& other)
		{
			refInstance = (T*)other.refInstance;
		}

		template<typename T2>
		WizRef(WizRef<T2>&& other)
		{
			refInstance = (T*)other.refInstance;
			other.refInstance = nullptr;
		}

		template<typename ... Args>
		static WizRef<T> CreateRef(Args&& ... args)
		{
			//return std::make_shared<T>(std::forward<Args>(args)...);
			return WizRef<T>(new T(std::forward<Args>(args)...));
		}

		T* Get() { return  refInstance; }
		const T* Get() const { return  refInstance; }

		template<typename T2>
		WizRef<T2> GetAs() const
		{
			return WizRef<T2>(*this);
		}

		WizRef& operator=(std::nullptr_t)
		{
			refInstance = nullptr;
			return *this;
		}

		WizRef& operator=(const WizRef<T>& other)
		{
			refInstance = other.refInstance;
			return *this;
		}

		template<typename T2>
		WizRef& operator=(const WizRef<T2>& other)
		{
			refInstance = other.refInstance;
			return *this;
		}

		template<typename T2>
		WizRef& operator=(WizRef<T2>&& other)
		{
			refInstance = other.refInstance;
			other.refInstance = nullptr;
			return *this;
		}

		bool operator==(const WizRef<T>& other) const
		{
			return refInstance == other.refInstance;
		}

		bool operator!=(const WizRef<T>& other) const
		{
			return !(*this == other);
		}

		operator bool() { return refInstance != nullptr; }
		operator bool() const { return refInstance != nullptr; }

		T* operator->() { return refInstance; }
		const T* operator->() const { return refInstance; }

		T& operator*() { return *refInstance; }
		const T& operator*() const { return *refInstance; }

	private:
		template<class T2>
		friend class WizRef;
		mutable T* refInstance;
	};
}