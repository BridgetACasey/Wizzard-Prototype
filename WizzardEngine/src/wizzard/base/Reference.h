//@BridgetACasey

#pragma once

namespace Wizzard
{
	class WizRefCounter
	{
	public:
		void IncrementReferenceTotal() const { ++references; }
		void DecrementReferenceTotal() const { --references; }

		uint32_t GetReferencesTotal() const { return references; }

	private:
		mutable uint32_t references = 0;
	};

	template<typename T>
	class WizRef
	{
	public:
		WizRef() : refInstance(nullptr) {}

		WizRef(std::nullptr_t n) : refInstance(nullptr) {}

		WizRef(T* instance) : refInstance(instance)
		{
			static_assert(std::is_base_of<WizRefCounter, T>::value, "Class is not WizRefCounter!");

			IncrementReference();
		}

		WizRef(const WizRef& other) : refInstance(other.refInstance)
		{
			IncrementReference();
		}

		template<typename T2>
		WizRef(const WizRef<T2>& other)
		{
			refInstance = (T*)other.refInstance;
			IncrementReference();
		}

		template<typename T2>
		WizRef(WizRef<T2>&& other)
		{
			refInstance = (T*)other.refInstance;
			other.refInstance = nullptr;
		}

		~WizRef() { DecrementReference(); }

		template<typename ... Args>
		static WizRef<T> CreateRef(Args&& ... args)
		{
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
			DecrementReference();
			refInstance = nullptr;
			return *this;
		}

		WizRef& operator=(const WizRef& other)
		{
			other.IncrementReference();
			DecrementReference();

			refInstance = other.refInstance;
			return *this;
		}

		template<typename T2>
		WizRef& operator=(const WizRef<T2>& other)
		{
			other.IncrementReference();
			DecrementReference();

			refInstance = other.refInstance;
			return *this;
		}

		template<typename T2>
		WizRef& operator=(WizRef<T2>&& other)
		{
			DecrementReference();

			refInstance = other.refInstance;
			other.refInstance = nullptr;
			return *this;
		}

		bool operator==(const WizRef& other) const
		{
			return refInstance == other.refInstance;
		}

		bool operator!=(const WizRef& other) const
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
		void AddActiveReference(void* instance);
		void EraseActiveReference(void* instance);
		bool IsActive(void* instance);

		//TODO: Fix this!
		void IncrementReference() const
		{
			//if (refInstance)
			//{
			//	refInstance->IncrementReferenceTotal();
			//	AddActiveReference(refInstance);
			//}
		}

		void DecrementReference() const
		{
			//if (refInstance)
			//{
			//	refInstance->DecrementReferenceTotal();
			//	if (refInstance->GetReferencesTotal() == 0)
			//	{
			//		delete refInstance;
			//		EraseActiveReference(refInstance);
			//		refInstance = nullptr;
			//	}
			//}
		}

		template<class T2>
		friend class WizRef;
		mutable T* refInstance;
	};
}